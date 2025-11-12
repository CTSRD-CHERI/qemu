/*
 * QEMU RISC-V Hobgoblin Board
 *
 * Copyright (c) 2023 Stuart Menefy, stuart.menefy@codasip.com
 * Copyright (c) 2023 Codasip Limited
 *
 * This provides a RISC-V Board with the following devices:
 *
 * 1) CLINT (Timer and IPI)
 * 2) PLIC (Platform Level Interrupt Controller)
 * 3) 16550 UART
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2 or later, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "qemu-version.h"
#include "qemu/osdep.h"
#include "qemu/log.h"
#include "qemu/error-report.h"
#include "qapi/error.h"
#include "hw/boards.h"
#include "hw/irq.h"
#include "hw/loader.h"
#include "hw/sysbus.h"
#include "hw/sd/sd.h"
#include "hw/ssi/ssi.h"
#include "target/riscv/cpu.h"
#include "hw/riscv/cmu.h"
#include "hw/riscv/numa.h"
#include "hw/riscv/riscv_hart.h"
#include "hw/intc/riscv_aclint.h"
#include "hw/intc/sifive_plic.h"
#include "hw/nvram/hobgoblin_nvemu.h"
#include "hw/riscv/hobgoblin.h"
#include "hw/riscv/boot.h"
#include "hw/char/serial.h"
#include "hw/char/xilinx_uartlite.h"
#include "hw/misc/codasip_trng.h"
#include "hw/pci-host/xilinx-pcie.h"
#include "chardev/char.h"
#include "sysemu/device_tree.h"
#include "sysemu/sysemu.h"
#include "sysemu/runstate.h"
#include "exec/address-spaces.h"
#include "net/net.h"
#include <libfdt.h>
#ifdef TARGET_CHERI
#include "cheri_tagmem.h"
#endif

#define TYPE_XILINX_SPI "xlnx.xps-spi"
#define TYPE_XLNX_AXI_GPIO "xlnx.axi-gpio"
#define TYPE_XILINX_ETHLITE "xlnx.xps-ethernetlite"
#define TYPE_XILINX_AXI_ETHERNET "xlnx.axi-ethernet"
#define TYPE_XILINX_AXI_DMA "xlnx.axi-dma"

#define CACHE_LINE_SIZE 64
#define L1_ICACHE_SIZE (32 * 1024)
#define L1_ICACHE_WAYS 4
#define L1_DCACHE_SIZE (32 * 1024)
#define L1_DCACHE_WAYS 4

#define L2_CACHE_SIZE (2 * 1024 * 1024)
#define L2_CACHE_WAYS 8

#define ITLB_ENTRIES 32
#define DTLB_ENTRIES 32

static const memmapEntry_t v1_memmap[] = {
    [HOBGOBLIN_MROM] =     {     0x1000,      0x100,
        "riscv.hobgoblin.mrom", MEM_ROM },
    [HOBGOBLIN_BOOT_ROM] = { 0x10000000, 0x00020000,
        "riscv.hobgoblin.boot.rom", MEM_ROM },
    [HOBGOBLIN_SRAM] =     { 0x20000000, 0x00100000,
        "riscv.hobgoblin.sram", MEM_RAM_CHERI },
    [HOBGOBLIN_PLIC] =     { 0x40000000,  0x4000000 },
    [HOBGOBLIN_ID_REG] =   { 0x60000000,      0x1000,
        "id_register", MEM_ROM },
    [HOBGOBLIN_CLINT] =    { 0x60014000,     0xc000 },
    [HOBGOBLIN_ETHLITE] =  { 0x60020000,     0x2000 },
    [HOBGOBLIN_FMC_AXI_DMA] = { 0x60030000,    0x10000 },
    [HOBGOBLIN_FMC_AXI_ETH] = { 0x60040000,    0x40000 },
    [HOBGOBLIN_AXI_DMA] =  { 0x600a0000,    0x10000 },
    [HOBGOBLIN_AXI_ETH] =  { 0x600c0000,    0x40000 },
    /*
     * The Hobgoblin FPGA uses a Xilinx AXI UART 16550 v2.0, which is at
     * 0x60100000 and uses 8 KiB in the address space. However, the lower 4 KiB
     * do not contain any registers, they start at offset 4 KiB. To keep things
     * simple, we leave out the lower 4 KiB and just declare the upper 4 KiB
     * here. The acessible register are fully compatible with QEMU's existing
     * NS16550A UART emulation.
     */
    [HOBGOBLIN_UART0] =    { 0x60101000,     0x1000 },
    [HOBGOBLIN_UART1] =    { 0x60110000,     0x1000 },
    [HOBGOBLIN_SPI] =      { 0x60210000,     0x1000 },
    [HOBGOBLIN_GPIO0] =    { 0x60300000,    0x10000 },
    [HOBGOBLIN_GPIO1] =    { 0x60310000,    0x10000 },
    [HOBGOBLIN_TRNG] =     { 0x60510000,     0x1000 },
    [HOBGOBLIN_NVEMU] =    { 0x60560000,      0xD00 },
    [HOBGOBLIN_TIMER] =    { 0x60600000,     0x8000 },
    [HOBGOBLIN_INTL_CMU] = { 0x60680000,    0x10000 },
    [HOBGOBLIN_CMU_DDR0] = { 0x60690000,    0x10000 },
    [HOBGOBLIN_CMU_DDR1] = { 0x606a0000,    0x10000 },
    /* Each virtio transport channel uses 512 byte */
    [HOBGOBLIN_VIRTIO] =   { 0x70000000,    0x10000 },
};

static const memmapEntry_t v2_memmap[] = {
    [HOBGOBLIN_MROM] = { 0x1000, 0x100, "riscv.hobgoblin.mrom",
                         MEM_ROM }, // Not listed in v1 map
    [HOBGOBLIN_BOOT_ROM] = { 0x28000000, 0x00020000, "riscv.hobgoblin.boot.rom",
                             MEM_ROM }, // Possibly the same location but
                                        // renambed PLATFORM ID Rom
    [HOBGOBLIN_SRAM] = { 0x20000000, 0x00100000, "riscv.hobgoblin.sram",
                         MEM_RAM_CHERI }, // Same location
    [HOBGOBLIN_PLIC] = { 0x18000000, 0x00400000 },
    [HOBGOBLIN_ID_REG] = { 0x10000000, 0x1000, "id_register", MEM_ROM },
    [HOBGOBLIN_CLINT] = { 0x10804000, 0x00010000 },   // moved...
    [HOBGOBLIN_ETHLITE] = { 0x10020000, 0x00010000 }, // moved
    [HOBGOBLIN_FMC_AXI_DMA] = { 0x10030000, 0x10000 },
    [HOBGOBLIN_FMC_AXI_ETH] = { 0x10040000, 0x40000 },
    [HOBGOBLIN_AXI_DMA] = { 0x100a0000, 0x10000 },
    [HOBGOBLIN_AXI_ETH] = { 0x100c0000, 0x40000 },
    /*
     * The Hobgoblin FPGA uses a Xilinx AXI UART 16550 v2.0, which is at
     * 0x60100000 and uses 8 KiB in the address space. However, the lower 4
     * KiB do not contain any registers, they start at offset 4 KiB. To keep
     * things simple, we leave out the lower 4 KiB and just declare the
     * upper 4 KiB here. The acessible register are fully compatible with
     * QEMU's existing NS16550A UART emulation.
     */
    [HOBGOBLIN_UART0] = { 0x10101000, 0x1000 },
    [HOBGOBLIN_UART1] = { 0x10110000, 0x1000 },
    [HOBGOBLIN_SPI] = { 0x10210000, 0x1000 },
    [HOBGOBLIN_GPIO0] = { 0x10300000, 0x10000 }, // listed as GPIO io
    [HOBGOBLIN_GPIO1] = { 0x10310000, 0x10000 }, // listed as GPIO auxialliary
    [HOBGOBLIN_TRNG] = { 0x10510000, 0x1000 },
    [HOBGOBLIN_NVEMU] = { 0x10560000, 0x1000 },
    [HOBGOBLIN_TIMER] = { 0x10600000,
                          0x1000 }, // this has shrunk, is it a problem?
    [HOBGOBLIN_INTL_CMU] = { 0x10680000, 0x10000 },
    [HOBGOBLIN_CMU_DDR0] = { 0x10690000, 0x10000 },
    [HOBGOBLIN_CMU_DDR1] = { 0x106a0000, 0x10000 },
    /* Each virtio transport channel uses 512 byte */
    [HOBGOBLIN_VIRTIO] = { 0x1c000000,
                           0x10000 }, // may need to check this as it will not
                                      // be part of map and likely needs
                                      // checking to not overlay
    [HOBGOBLIN_PCIE0] =         {0x40000000,  0x10000000},// ecam 256M
    [HOBGOBLIN_PCIE0_MMIO0] =   {0x50000000,  0x10000000}, // Window 256M
    [HOBGOBLIN_PCIE1] =         {0x60000000,  0x10000000}, // ecam 256M
    [HOBGOBLIN_PCIE1_MMIO0] =   {0x70000000,  0x10000000}, // Window 256M
    [HOBGOBLIN_PCIE0_MMIO1] =   {0x1000000000,    0x100000000}, // Prefetch window 4G
    [HOBGOBLIN_PCIE1_MMIO1] =   {0x1100000000,    0x100000000}, // prefetch window 4G
};

typedef enum{
    V1=0,
    V2=1,
    VERSION_MAX=2
} HOBGOBLIN_VERSION;

static const memmapEntry_t *address_maps[VERSION_MAX] = {
    v1_memmap,
    v2_memmap
};


static const memmapEntry_t genesys2_dram_memmap[] = {
    { 0x80000000, 0x40000000, "riscv.hobgoblin.ram", MEM_RAM_CHERI},
};

static const memmapEntry_t profpga_dram_memmap[] = {
    { 0x2000000000, 0x400000000, "riscv.hobgoblin.ram", MEM_RAM_CHERI},
};

static const memmapEntry_t vcu118_dram_memmap[] = {
    { 0x0080000000, 0x0080000000, "riscv.hobgoblin.ram0", MEM_RAM_CHERI},
    { 0x0100000000, 0x0080000000, "riscv.hobgoblin.ram1", MEM_RAM_CHERI},
};

uint8_t irqmap[2][HOBGOBLIN_IRQ_END] = {
    {
    /* Interrupt 0 is reserved */
        [HOBGOBLIN_UART0_IRQ]    = 1,
        [HOBGOBLIN_UART1_IRQ]    = 2,
        [HOBGOBLIN_SPI_IRQ]      = 4,
        [HOBGOBLIN_GPIO0_IRQ]    = 7,
        [HOBGOBLIN_GPIO1_IRQ]    = 8,
        [HOBGOBLIN_TIMER_IRQ]    = 9,
        [HOBGOBLIN_FMC_ETH_IRQ]     = 10,
        [HOBGOBLIN_FMC_AXIDMA_IRQ0] = 11,
        [HOBGOBLIN_FMC_AXIDMA_IRQ1] = 12,
        [HOBGOBLIN_FMC_ETH_PHY_IRQ] = 13,
        [HOBGOBLIN_ETH_IRQ]      = 15,
        [HOBGOBLIN_AXIDMA_IRQ0]  = 16,
        [HOBGOBLIN_AXIDMA_IRQ1]  = 17,
        [HOBGOBLIN_VIRTIO0_IRQ]  = 28,
        [HOBGOBLIN_MAX_IRQ] = 31,
    }, // the MAX IRQ number corresponds to VIRTIO7
    {
        [HOBGOBLIN_UART0_IRQ]    = 1,
        [HOBGOBLIN_UART1_IRQ]    = 2,
        [HOBGOBLIN_SPI_IRQ]      = 4,
        [HOBGOBLIN_GPIO0_IRQ]    = 7,
        [HOBGOBLIN_GPIO1_IRQ]    = 8,
        [HOBGOBLIN_TIMER_IRQ]    = 9,
        [HOBGOBLIN_FMC_ETH_IRQ]     = 15,
        [HOBGOBLIN_FMC_AXIDMA_IRQ0] = 16,
        [HOBGOBLIN_FMC_AXIDMA_IRQ1] = 17,
        [HOBGOBLIN_FMC_ETH_PHY_IRQ] = 18,
        [HOBGOBLIN_ETH_IRQ]      = 10,
        [HOBGOBLIN_AXIDMA_IRQ0]  = 11,
        [HOBGOBLIN_AXIDMA_IRQ1]  = 12,
        [HOBGOBLIN_ETH_PHY_IRQ] = 13,
        [HOBGOBLIN_VIRTIO0_IRQ]  = 56,
        [HOBGOBLIN_MAX_IRQ] = 63,
        [HOBGOBLIN2_PCIE0_IRQ]      = 41,
        [HOBGOBLIN2_PCIE0_MSI0_IRQ] = 42,
        [HOBGOBLIN2_PCIE0_MSI1_IRQ] = 43,
        [HOBGOBLIN2_PCIE1_IRQ]      = 44,
        [HOBGOBLIN2_PCIE1_MSI0_IRQ] = 45,
        [HOBGOBLIN2_PCIE1_MSI1_IRQ] = 46,
    }
};

// define a couple of helpers for the mmap and irqmap
#define HIRQ(_hs_, _idx_) (irqmap[HOBGOBLIN_MACHINE_GET_CLASS(_hs_)->irq_map_version][_idx_])
#define MAPVERSION(_hs_) (HOBGOBLIN_MACHINE_GET_CLASS(_hs_)->map_version)

#define V1_VIRTIO_TRANSPORTS 4
#define V2_VIRTIO_TRANSPORTS 8

/* sifive_plic_create() parameters */
#define HOBGOBLIN_PLIC_NUM_SOURCES      64
#define HOBGOBLIN_PLIC_NUM_PRIORITIES   7
#define HOBGOBLIN_PLIC_PRIORITY_BASE    0x0000
#define HOBGOBLIN_PLIC_PENDING_BASE     0x1000
#define HOBGOBLIN_PLIC_ENABLE_BASE      0x2000
#define HOBGOBLIN_PLIC_ENABLE_STRIDE    0x80
#define HOBGOBLIN_PLIC_CONTEXT_BASE     0x200000
#define HOBGOBLIN_PLIC_CONTEXT_STRIDE   0x1000

/* CLINT timebase frequency */
#define CLINT_TIMEBASE_FREQ             100000000 /* 100 MHz */

static int hobgoblin_load_images(HobgoblinState *s, const memmapEntry_t *dram)
{
    MachineState *machine = &s->machine;
    hwaddr start_addr;
    uint64_t kernel_entry = 0;
    uint64_t fdt_load_addr = 0;
    target_ulong firmware_end_addr;

    const memmapEntry_t *memmap = address_maps[MAPVERSION(s)];

    if (s->boot_from_rom) {
        /* Load the FSBL into ROM and set the ZSBL to point to it */
        start_addr = memmap[HOBGOBLIN_BOOT_ROM].base;
        firmware_end_addr = riscv_find_and_load_firmware(machine,
                                                         "fsbl_rom.xexe",
                                                         start_addr,
                                                         NULL);
    } else {
        target_ulong kernel_start_addr = 0;
        int fdt_size = 0;

        start_addr = dram->base;

        /* Read DTB */
        if (machine->dtb) {
            machine->fdt = load_device_tree(machine->dtb, &fdt_size);
            if (!machine->fdt) {
                error_report("load_device_tree() failed");
                exit(1);
            }
        }

        /* Load SBI into RAM */
        firmware_end_addr = riscv_find_and_load_firmware(machine,
                                                         RISCV64_BIOS_BIN,
                                                         start_addr,
                                                         NULL);

        /* Load Kernel into RAM */
        if (machine->kernel_filename) {
            kernel_start_addr = riscv_calc_kernel_start_addr(&s->soc,
                                                             firmware_end_addr);
            kernel_entry = riscv_load_kernel(machine->kernel_filename,
                                             kernel_start_addr, NULL);

            if (machine->initrd_filename) {
                hwaddr start, end;
                end = riscv_load_initrd(machine->initrd_filename,
                                        machine->ram_size, kernel_entry,
                                        &start);
                if (machine->fdt) {
                    qemu_fdt_setprop_cell(machine->fdt, "/chosen",
                                          "linux,initrd-start", start);
                    qemu_fdt_setprop_cell(machine->fdt, "/chosen",
                                          "linux,initrd-end", end);
                }
            }

            if (machine->fdt && machine->kernel_cmdline &&
                *machine->kernel_cmdline) {
                qemu_fdt_setprop_string(machine->fdt, "/chosen",
                                        "bootargs", machine->kernel_cmdline);
            }
        }

        /* Store (potentially modified) FDT into RAM */
        if (machine->fdt) {
            fdt_load_addr = riscv_load_fdt(dram->base,
                                           dram->size,
                                           machine->fdt);
        }
    }

    /*
     * If no kernel or FTD has been provided, kernel_entry and fdt_load_addr
     * can be 0 here. For QEMU, this is fine, as they are just parameters
     * passed to the bootloader, which has to cope with that.
     */
    riscv_setup_rom_reset_vec(machine, &s->soc, start_addr,
            memmap[HOBGOBLIN_MROM].base, memmap[HOBGOBLIN_MROM].size,
            kernel_entry, fdt_load_addr, machine->fdt);

    return 0;
}

static void hobgoblin_add_soc(HobgoblinState *s, const int smp_cpus)
{
    MachineState *machine = &s->machine;

    object_initialize_child(OBJECT(machine), "soc", &s->soc,
                            TYPE_RISCV_HART_ARRAY);

    object_property_set_str(OBJECT(&s->soc), "cpu-type",
                            machine->cpu_type, &error_abort);

    object_property_set_int(OBJECT(&s->soc), "num-harts",
                            smp_cpus, &error_abort);

    sysbus_realize(SYS_BUS_DEVICE(&s->soc), &error_fatal);
}

static MemoryRegion *hobgoblin_add_memory_area(MemoryRegion *system_memory,
                                      const memmapEntry_t *e)
{
    MemoryRegion *reg = g_new(MemoryRegion, 1);
    memory_region_init_ram(reg, NULL, e->name, e->size, &error_fatal);
    if (e->type == MEM_ROM) {
        memory_region_set_readonly(reg, true);
    }
#ifdef TARGET_CHERI
    else if (e->type == MEM_RAM_CHERI) {
        cheri_tag_init(reg, e->size);
    }
#endif

    memory_region_add_subregion(system_memory, e->base, reg);
    return reg;
}

static void hobgoblin_add_interrupt_controller(HobgoblinState *s,
                                               const int num_harts)
{
    const memmapEntry_t *memmap = address_maps[MAPVERSION(s)];
    const memmapEntry_t *mem_plic = &memmap[HOBGOBLIN_PLIC];
    const memmapEntry_t *mem_clint = &memmap[HOBGOBLIN_CLINT];
    const int hartid_base = 0; /* Hart IDs start at 0 */
    char *plic_hart_config;

    /* PLIC */
    assert(HOBGOBLIN_PLIC_NUM_SOURCES > HIRQ(s, HOBGOBLIN_MAX_IRQ));
    plic_hart_config = riscv_plic_hart_config_string(num_harts);
    DeviceState *plic = sifive_plic_create(
        mem_plic->base,
        plic_hart_config,
        num_harts,
        hartid_base,
        HOBGOBLIN_PLIC_NUM_SOURCES,
        HOBGOBLIN_PLIC_NUM_PRIORITIES,
        HOBGOBLIN_PLIC_PRIORITY_BASE,
        HOBGOBLIN_PLIC_PENDING_BASE,
        HOBGOBLIN_PLIC_ENABLE_BASE,
        HOBGOBLIN_PLIC_ENABLE_STRIDE,
        HOBGOBLIN_PLIC_CONTEXT_BASE,
        HOBGOBLIN_PLIC_CONTEXT_STRIDE,
        mem_plic->size);
    g_free(plic_hart_config);

    /* CLINT with SWI in M-Mode */
    riscv_aclint_swi_create(mem_clint->base, hartid_base, num_harts, false);

    /* CLINT timer */
    assert(mem_clint->size >= RISCV_ACLINT_SWI_SIZE);
    riscv_aclint_mtimer_create(
        mem_clint->base + RISCV_ACLINT_SWI_SIZE,
        RISCV_ACLINT_DEFAULT_MTIMER_SIZE,
        hartid_base,
        num_harts,
        RISCV_ACLINT_DEFAULT_MTIMECMP,
        RISCV_ACLINT_DEFAULT_MTIME,
        CLINT_TIMEBASE_FREQ,
        true); /* provide_rdtime */

    /* publish */
    s->plic = plic;
}

static qemu_irq hobgoblin_make_plic_irq(HobgoblinState *s, int number)
{
    DeviceState *plic = s->plic;
    assert(plic); /* PLIC instance must exist. */
    return qdev_get_gpio_in(DEVICE(plic), number);
}

static void hobgoblin_connect_plic_irq(HobgoblinState *s,
        SysBusDevice *busDev, int dev_irq, int number)
{
    qemu_irq irq = hobgoblin_make_plic_irq(s, number);
    sysbus_connect_irq(busDev, dev_irq, irq);
}

static void hobgoblin_add_id_register(HobgoblinState *s,
                                      MemoryRegion *system_memory)
{
    int i;
    const memmapEntry_t *memmap = address_maps[MAPVERSION(s)];
    const memmapEntry_t *mem_id = &memmap[HOBGOBLIN_ID_REG];
    const uint32_t ethernet_types[] = {
        [ETH_TYPE_ETHERNETLITE] = 0x0,
        [ETH_TYPE_AXI_ETHERNET] = 0x1,
    };
    const uint8_t platform_types[] = {
        [BOARD_TYPE_GENESYS2] = 0x1,
        [BOARD_TYPE_PROFPGA] = 0x2,
        [BOARD_TYPE_VCU118] = 0x3,
    };
    HobgoblinClass *hc = HOBGOBLIN_MACHINE_GET_CLASS(s);

#if defined(TARGET_RISCV64)
#if defined(TARGET_CHERI)
    char core_prefix='X';
    uint32_t core_type = 5;
#else
    char core_prefix='A';
    uint32_t core_type = 1;
#endif
#elif defined(TARGET_RISCV32)
#if defined(TARGET_CHERI)
    char core_prefix='V';
    uint32_t core_type = 7;
#else
    char core_prefix='L';
    uint32_t core_type = 3;
#endif
#endif
    // on hobgoblinv1 memory map we just had core_type=1
    //
    if (MAPVERSION(s) == V1)
    {
        core_type = 1;
    }
    uint32_t id_register[] = {
        /* (0x0000) Platform ID register version */
        1 << 8 | 1,
        /* (0x0004) Platform version */
        platform_types[hc->board_type] << 16 | 1 << 8 | 0,
        /* (0x0008) Core type */
        core_type,
        /* (0x000C) Core frequency in MHz */
        50,
        /* (0x0010) Ethernet type */
        ethernet_types[s->eth_type],
        /* (0x0014) Platform features */
#ifdef TARGET_CHERI
        (1 << 0), /* CHERI */
#else
        (0 << 0), /* CHERI */
#endif
        /* (0x0018) Security features */
        (1 << 0) | /* TRNG (True Random Number Generator) */
        (0 << 1) | /* TPU (Trace Prodection Unit) */
        (1 << 2),  /* NVE (Non Volatile flash Emulator) */
         [0x28/4] = 0x03000200,
        /* (0x0100-0x0110) Platform SHA<0:4> */
        [0x0100/4] = 0, 0, 0, 0, 0,
        /* (0x0120-0x012c) Core artifact<0:3> */
        [0x0120/4] = 0, 0, 0, 0,
        [0x200/4] = ('U' << 24) | ('M' << 16) | ('E' << 8) | 'Q',
        [0x300/4] = ('0' << 24) | ('3' << 16) | ('7' << 8) | core_prefix,
    };

    const uint8_t platform_hash[20] = QEMU_GIT_HASH;

    for (i=0; i<5; i++) {
        id_register[0x100/4 + i] = *(uint32_t*)&platform_hash[i*4];
    }

    for (i = 0; i < ARRAY_SIZE(id_register); i++) {
        id_register[i] = cpu_to_le32(id_register[i]);
    }

    hobgoblin_add_memory_area(system_memory, mem_id);
    rom_add_blob_fixed(mem_id->name, id_register, sizeof(id_register),
                       mem_id->base);
}

static void __attribute__((unused))
hobgoblin_add_cmu(HobgoblinState *s, DeviceState **d, const memmapEntry_t *io, const MemoryRegion *ram)
{
    SysBusDevice *bus_cmu;
    *d = qdev_new(TYPE_CMU_DEVICE);
    bus_cmu = SYS_BUS_DEVICE(*d);
    qdev_prop_set_uint32(*d, "reg-map-version", MAPVERSION(s));
    qdev_prop_set_uint64(*d, "ram-base", ram->addr);
    /*
     * int128_get64 assert()s that the upper 64bits are zero. ram->size comes
     * from our memory map, this check makes sense.
     */
    qdev_prop_set_uint64(*d, "ram-size", int128_get64(ram->size));
    object_property_set_link(OBJECT(*d), "managed-ram", OBJECT(ram), &error_fatal);
    sysbus_realize_and_unref(bus_cmu, &error_fatal);
    sysbus_mmio_map(bus_cmu, 0, io->base);
}

static void hobgoblin_add_uart(HobgoblinState *s,
                               MemoryRegion *system_memory)
{
    const memmapEntry_t *memmap = address_maps[MAPVERSION(s)];
    const memmapEntry_t *mem_uart = &memmap[HOBGOBLIN_UART0];

    /* there must be an actual QEMU uart device */
    Chardev *chardev = serial_hd(0);
    assert(chardev);

    qemu_irq irq = hobgoblin_make_plic_irq(s, HIRQ(s, HOBGOBLIN_UART0_IRQ));

    serial_mm_init(system_memory, mem_uart->base, 2, irq, 115200,
                   chardev, DEVICE_LITTLE_ENDIAN);
}

static void hobgoblin_add_uartlite(HobgoblinState *s,
                                   MemoryRegion *system_memory)
{
    const memmapEntry_t *memmap = address_maps[MAPVERSION(s)];
    const memmapEntry_t *mem_uart = &memmap[HOBGOBLIN_UART1];
    Chardev *chardev = serial_hd(1);
    qemu_irq irq = hobgoblin_make_plic_irq(s, HIRQ(s, HOBGOBLIN_UART1_IRQ));

    xilinx_uartlite_create(mem_uart->base, irq, chardev);
}

static void hobgoblin_gpio_1_3_event(void *opaque, int n, int level)
{
    /* gpio pin active high triggers reset */
    if (level) {
        qemu_system_reset_request(SHUTDOWN_CAUSE_GUEST_RESET);
    }
}

static void hobgoblin_add_gpio(HobgoblinState *s)
{
    const memmapEntry_t *memmap = address_maps[MAPVERSION(s)];
    for (int i = 0; i < 2; i++) {
        /* create GPIO */
        DeviceState *gpio = qdev_new(TYPE_XLNX_AXI_GPIO);
        SysBusDevice *bus_gpio = SYS_BUS_DEVICE(gpio);
        sysbus_realize_and_unref(bus_gpio, &error_fatal);
        sysbus_mmio_map(bus_gpio, 0, memmap[HOBGOBLIN_GPIO0 + i].base);
        /* connect PLIC interrupt */
        hobgoblin_connect_plic_irq(s, bus_gpio, 0, HIRQ(s, HOBGOBLIN_GPIO0_IRQ) + i);
        /* publish GPIO device */
        s->gpio[i] = gpio;
    }

    /* Reset via GPIO 1.3 */
    qdev_connect_gpio_out(DEVICE(s->gpio[1]), 3,
                          qemu_allocate_irq(hobgoblin_gpio_1_3_event, NULL, 0));

}

static void hobgoblin_add_spi(HobgoblinState *s)
{
    const memmapEntry_t *memmap = address_maps[MAPVERSION(s)];
    const memmapEntry_t *mem_spi = &memmap[HOBGOBLIN_SPI];

    /* create SPI device */
    DeviceState *spi = qdev_new(TYPE_XILINX_SPI);
    SysBusDevice *bus_spi = SYS_BUS_DEVICE(spi);
    sysbus_realize_and_unref(bus_spi, &error_fatal);
    sysbus_mmio_map(bus_spi, 0, mem_spi->base);
    /* connect PLIC interrupt */
    hobgoblin_connect_plic_irq(s, bus_spi, 0, HIRQ(s, HOBGOBLIN_SPI_IRQ));

    /* publish SPI device */
    s->spi = spi;
}

static void hobgoblin_add_sd(HobgoblinState *s)
{
    /* create SD Card in SPI mode */
    DeviceState *sd_card_spi = qdev_new(TYPE_SD_CARD);
    DriveInfo *dinfo = drive_get(IF_SD, 0, 0);
    BlockBackend *blk = dinfo ? blk_by_legacy_dinfo(dinfo) : NULL;
    qdev_prop_set_drive_err(sd_card_spi, "drive", blk, &error_fatal);
    qdev_prop_set_bit(sd_card_spi, "spi", true);

    /* Connect SD card to SPI */
    SSIBus *bus_ssi = (SSIBus *)qdev_get_child_bus(s->spi, "spi");
    DeviceState *sd_dev = ssi_create_peripheral(bus_ssi, "ssi-sd");
    qdev_realize_and_unref(sd_card_spi,
                           qdev_get_child_bus(sd_dev, "sd-bus"),
                           &error_fatal);

    /*
     * gpio1 (0x60310000), pin 1 is used as card detect signal. This pin is
     * active low, it comes directly from the sd card and goes into the
     * hobgoblin machine.
     * We don't initialise this pin here. For axi gpio, all pins default to
     * output. Reading an ouput pin always returns 0.
     *
     * TODO: Should we set the card detect pin explicitly? And if so, how
     * would this work? I tried
     * qemu_irq_lower(qdev_get_gpio_in(DEVICE(s->gpio[1]), 1));
     * This should simulate an external signal that pulls the pin to low.
     * However, the setting is ignored since the pin is set as an output...
     */
}

static void hobgoblin_add_ethernetlite(HobgoblinState *s)
{
    const memmapEntry_t *memmap = address_maps[MAPVERSION(s)];
    const memmapEntry_t *mem_eth = &memmap[HOBGOBLIN_ETHLITE];

    NICInfo *nd = &nd_table[0];
    const char *model = TYPE_XILINX_ETHLITE;

    /* Ethernet (ethernetlite) */
    qemu_check_nic_model(nd, model);
    DeviceState *eth = qdev_new(model);
    qdev_set_nic_properties(eth, nd);

    SysBusDevice *bus_eth = SYS_BUS_DEVICE(eth);
    sysbus_realize_and_unref(bus_eth, &error_fatal);
    sysbus_mmio_map(bus_eth, 0, mem_eth->base);
    /* connect PLIC interrupt */
    hobgoblin_connect_plic_irq(s, bus_eth, 0, HIRQ(s, HOBGOBLIN_ETH_IRQ));

    /* publish ETH device */
    s->eth[0] = eth;
}

static void hobgoblin_add_axi_ethernet(HobgoblinState *s, int eth_num,
    int phy_addr,
    int eth_memmap, int dma_memmap,
    int eth_irq, int dma_irq0, int dma_irq1)
{
    const memmapEntry_t *memmap = address_maps[MAPVERSION(s)];
    const memmapEntry_t *mem_eth = &memmap[eth_memmap];
    const memmapEntry_t *mem_dma = &memmap[dma_memmap];
    NICInfo *nd = &nd_table[eth_num];
    const char *eth_model = TYPE_XILINX_AXI_ETHERNET;
    const char *eth_name = g_strdup_printf("xilinx-eth%d", eth_num);
    const char *dma_name = g_strdup_printf("xilinx-dma%d", eth_num);

    qemu_check_nic_model(nd, eth_model);

    DeviceState *eth = qdev_new(eth_model);
    DeviceState *dma = qdev_new(TYPE_XILINX_AXI_DMA);

    /* FIXME: attach to the sysbus instead */
    object_property_add_child(qdev_get_machine(), eth_name, OBJECT(eth));
    object_property_add_child(qdev_get_machine(), dma_name, OBJECT(dma));

    Object *ds, *cs;
    ds = object_property_get_link(OBJECT(dma),
                                  "axistream-connected-target", NULL);
    cs = object_property_get_link(OBJECT(dma),
                                  "axistream-control-connected-target", NULL);
    assert(ds && cs);
    qdev_set_nic_properties(eth, nd);
    qdev_prop_set_uint32(eth, "phyaddr", phy_addr);
    qdev_prop_set_uint32(eth, "rxmem", 0x4000);
    qdev_prop_set_uint32(eth, "txmem", 0x4000);
    object_property_set_link(OBJECT(eth), "axistream-connected", ds,
                             &error_abort);
    object_property_set_link(OBJECT(eth), "axistream-control-connected", cs,
                             &error_abort);

    SysBusDevice *eth_busdev = SYS_BUS_DEVICE(eth);
    sysbus_realize_and_unref(eth_busdev, &error_fatal);
    sysbus_mmio_map(eth_busdev, 0, mem_eth->base);
    hobgoblin_connect_plic_irq(s, eth_busdev, 0, eth_irq);

    ds = object_property_get_link(OBJECT(eth),
                                  "axistream-connected-target", NULL);
    cs = object_property_get_link(OBJECT(eth),
                                  "axistream-control-connected-target", NULL);
    assert(ds && cs);
    qdev_prop_set_uint32(dma, "freqhz", 100 * 1000000);
    qdev_prop_set_bit(dma, "64bit", true);
    object_property_set_link(OBJECT(dma), "axistream-connected", ds,
                             &error_abort);
    object_property_set_link(OBJECT(dma), "axistream-control-connected", cs,
                             &error_abort);

    SysBusDevice *dma_busdev = SYS_BUS_DEVICE(dma);
    sysbus_realize_and_unref(dma_busdev, &error_fatal);
    sysbus_mmio_map(dma_busdev, 0, mem_dma->base);
    hobgoblin_connect_plic_irq(s, dma_busdev, 0, dma_irq0);
    hobgoblin_connect_plic_irq(s, dma_busdev, 1, dma_irq1);

    /* publish ETH device */
    s->eth[eth_num] = eth;
}

static void hobgoblin_add_trng(HobgoblinState *s)
{
    SysBusDevice *ss;
    const memmapEntry_t *memmap = address_maps[MAPVERSION(s)];

    s->trng = qdev_new(TYPE_CODASIP_TRNG);
    ss = SYS_BUS_DEVICE(s->trng);
    sysbus_realize_and_unref(ss, &error_fatal);
    sysbus_mmio_map(ss, 0, memmap[HOBGOBLIN_TRNG].base);
}

static void hobgoblin_add_nvemu(HobgoblinState *s)
{
    SysBusDevice *ss;
    Error *e = NULL;
    const memmapEntry_t *memmap = address_maps[MAPVERSION(s)];

    s->nvemu = qdev_new(TYPE_HOB_NVEMU);
    ss = SYS_BUS_DEVICE(s->nvemu);
    /*
     * To realize the nvemu device, the caller must have provided a memory
     * backend on the command line.
     * Most hobgoblin users don't need the nvemu simulation. Don't fail the
     * hobgoblin machine if nvemu can't be realized (most likely due to
     * missing mem backend).
     */
    if (!sysbus_realize_and_unref(ss, &e)) {
        object_unparent(OBJECT(s->nvemu));
        s->nvemu = NULL;
        return;
    }
    sysbus_mmio_map(ss, 0, memmap[HOBGOBLIN_NVEMU].base);
}

static void hobgoblin_add_timer(HobgoblinState *s)
{
    SysBusDevice *ss;
    const memmapEntry_t *memmap = address_maps[MAPVERSION(s)];

    s->timer = qdev_new("xlnx.xps-timer");
    qdev_prop_set_uint32(s->timer, "one-timer-only", 1);
    qdev_prop_set_uint32(s->timer, "clock-frequency", 100 * 1000000);
    ss = SYS_BUS_DEVICE(s->timer);
    sysbus_realize_and_unref(ss, &error_fatal);
    sysbus_mmio_map(ss, 0, memmap[HOBGOBLIN_TIMER].base);
    sysbus_connect_irq(ss, 0,
                       qdev_get_gpio_in(DEVICE(s->plic), HIRQ(s, HOBGOBLIN_TIMER_IRQ)));
}

static void hobgoblin_add_virtio(HobgoblinState *s)
{
    const memmapEntry_t *memmap = address_maps[MAPVERSION(s)];
    const memmapEntry_t *mem_virtio = &memmap[HOBGOBLIN_VIRTIO];
    HobgoblinClass *hc = HOBGOBLIN_MACHINE_GET_CLASS(s);

    int virtio_transports = (MAPVERSION(s) == V2 && hc->board_type == BOARD_TYPE_VCU118) ?
                            V2_VIRTIO_TRANSPORTS : V1_VIRTIO_TRANSPORTS;

    for (int i = 0; i < virtio_transports; i++) {
        hwaddr offset = 0x200 * i;
        assert(offset < mem_virtio->size);
        hwaddr base = mem_virtio->base + offset;
        qemu_irq irq = hobgoblin_make_plic_irq(s, HIRQ(s, HOBGOBLIN_VIRTIO0_IRQ) + i);
        sysbus_create_simple("virtio-mmio", base, irq);
    }
}

static void hobgoblin_add_xilinx_pcie(HobgoblinState *s, MemoryRegion *sys_mem,
    uint32_t bus_nr, int cfg_memmap, int mmio_memmap[], int mmio_num,
    int irq, int irq_msi[2], bool link_up)
{
    DeviceState *dev;
    MemoryRegion *region;
    const memmapEntry_t *memmap = address_maps[MAPVERSION(s)];

    dev = qdev_new(TYPE_XILINX_PCIE_HOST);

    qdev_prop_set_uint32(dev, "bus_nr", bus_nr);
    qdev_prop_set_uint64(dev, "cfg_base", memmap[cfg_memmap].base);
    qdev_prop_set_uint64(dev, "cfg_size", memmap[cfg_memmap].size);
    qdev_prop_set_uint64(dev, "mmio_base", memmap[mmio_memmap[0]].base);
    qdev_prop_set_uint64(dev, "mmio_size", memmap[mmio_memmap[0]].size);
#if 1
    if (mmio_num > 1) {
        qdev_prop_set_uint64(dev, "mmio1_base", memmap[mmio_memmap[1]].base);
        qdev_prop_set_uint64(dev, "mmio1_size", memmap[mmio_memmap[1]].size);
    }
#endif
    qdev_prop_set_bit(dev, "intr_fifo_mode", false);
    qdev_prop_set_bit(dev, "link_up", link_up);

    sysbus_realize_and_unref(SYS_BUS_DEVICE(dev), &error_fatal);

    region = sysbus_mmio_get_region(SYS_BUS_DEVICE(dev), 0);
    memory_region_add_subregion_overlap(sys_mem,
        memmap[cfg_memmap].base, region, 0);

#if 0
    region = sysbus_mmio_get_region(SYS_BUS_DEVICE(dev), 1);
    memory_region_add_subregion_overlap(sys_mem,
        0, region, 0);
#endif

    qdev_connect_gpio_out_named(dev, "interrupt_out", 0,
        hobgoblin_make_plic_irq(s, irq));

    for (int i=0; i<2; i++) {
        qdev_connect_gpio_out_named(dev, "interrupt_out_msi", i,
            hobgoblin_make_plic_irq(s, irq_msi[i]));
    }
}

static void create_fdt_aliases(HobgoblinState *s, const memmapEntry_t *memmap)
{
    char *name;
    char *ethernet0_alias, *ethernet1_alias, *serial0_alias, *spi0_alias;
    MachineState *mc = MACHINE(s);

    name = g_strdup_printf("/aliases");
    qemu_fdt_add_subnode(mc->fdt, name);

    ethernet0_alias =
        g_strdup_printf("/soc@0/ethernet@%lx", memmap[HOBGOBLIN_AXI_ETH].base);
    qemu_fdt_setprop_string(mc->fdt, name, "ethernet0", ethernet0_alias);

    ethernet1_alias = g_strdup_printf("/soc@0/ethernet@%lx",
                                      memmap[HOBGOBLIN_FMC_AXI_ETH].base);
    qemu_fdt_setprop_string(mc->fdt, name, "ethernet1", ethernet1_alias);

    serial0_alias =
        g_strdup_printf("/soc@0/serial@%lx", memmap[HOBGOBLIN_UART0].base);
    qemu_fdt_setprop_string(mc->fdt, name, "serial0", serial0_alias);

    spi0_alias = g_strdup_printf("/soc@0/spi@%lx", memmap[HOBGOBLIN_SPI].base);
    qemu_fdt_setprop_string(mc->fdt, name, "spi0", spi0_alias);

    g_free(name);
    g_free(ethernet0_alias);
    g_free(ethernet1_alias);
    g_free(serial0_alias);
    g_free(spi0_alias);
}

static char *custom_riscv_isa_string(RISCVCPU *cpu, bool is_32_bit)
{
    typedef struct {
        bool *flag;
        const char *ext;
    } ext_map_t;

    ext_map_t base_exts[] = {
        { &cpu->cfg.ext_i, "i" }, { &cpu->cfg.ext_m, "m" },
        { &cpu->cfg.ext_a, "a" }, { &cpu->cfg.ext_f, "f" },
        { &cpu->cfg.ext_d, "d" }, { &cpu->cfg.ext_c, "c" },
        { &cpu->cfg.ext_h, "h" }, { &cpu->cfg.ext_j, "j" },
        { &cpu->cfg.ext_v, "v" }
    };

    ext_map_t multi_exts[] = {
                               { &cpu->cfg.ext_icbom, "_zicbom" },
                               { &cpu->cfg.ext_icboz, "_zicboz" },
                               { &cpu->cfg.ext_zca, "_zca" },
                               { &cpu->cfg.ext_zcb, "_zcb" },
                               { &cpu->cfg.ext_zcd, "_zcd" },
                               { &cpu->cfg.ext_zcf, "_zcf" },
#if !defined(TARGET_CHERI_RISCV_V9)
                               { &cpu->cfg.ext_svpbmt, "_svpbmt" }
#endif
    };

    GString *result = (is_32_bit) ? g_string_new("rv32") : g_string_new("rv64");

    for (int i = 0; i < ARRAY_SIZE(base_exts); i++) {
        if (*base_exts[i].flag)
            g_string_append(result, base_exts[i].ext);
    }

    for (int i = 0; i < ARRAY_SIZE(multi_exts); i++) {
        if (*multi_exts[i].flag)
            g_string_append(result, multi_exts[i].ext);
    }

    uint64_t features = cpu->env.features;
    if ((features & (1ULL << RISCV_FEATURE_CHERI))) {
        g_string_append(result, "_zcheripurecap");
    }
    if ((features & (1ULL << RISCV_FEATURE_CHERI_HYBRID))) {
        g_string_append(result, "_zcherihybrid");
    }

    return g_string_free(result, FALSE);
}

static char *format_extensions(const char *ext_str, size_t *len)
{
    GString *result = g_string_new("");
    const char *p = ext_str;
    const char *underscore = strchr(ext_str, '_');

    if (underscore) {
        while (p < underscore) {
            if (result->len > 0)
                g_string_append_c(result, '\0');
            g_string_append_c(result, *p++);
        }
        char **parts = g_strsplit(underscore + 1, "_", -1);
        for (int i = 0; parts[i]; i++) {
            if (result->len > 0)
                g_string_append_c(result, '\0');
            g_string_append(result, parts[i]);
        }
        g_strfreev(parts);
    } else {
        while (*p) {
            if (result->len > 0)
                g_string_append_c(result, '\0');
            g_string_append_c(result, *p++);
        }
    }

    g_string_append_c(result, '\0');
    *len = result->len;
    return g_string_free(result, FALSE);
}

static void create_fdt_socket_cpus(HobgoblinState *s, int socket,
                                   char *clust_name, bool is_32_bit,
                                   uint32_t l2cache_phandle,
                                   uint32_t *intc_phandles)
{
    int cpu;
    MachineState *mc = MACHINE(s);
    char *name, *cpu_name, *intc_name, *formatted_ext;
    size_t ext_len;

    for (cpu = 0; cpu < mc->smp.cores; cpu++) {

        cpu_name = g_strdup_printf("/cpus/cpu@%d", s->soc.hartid_base + cpu);
        qemu_fdt_add_subnode(mc->fdt, cpu_name);
        qemu_fdt_setprop_string(mc->fdt, cpu_name, "mmu-type",
                                (is_32_bit) ? "riscv,sv32" : "riscv,sv39");
        name = custom_riscv_isa_string(&s->soc.harts[cpu], is_32_bit);
        qemu_fdt_setprop_string(mc->fdt, cpu_name, "riscv,isa", name);

        qemu_fdt_setprop_string(mc->fdt, cpu_name, "riscv,isa-base",
                                g_strndup(name, 5));

        formatted_ext = format_extensions(name + 4, &ext_len);
        qemu_fdt_setprop(mc->fdt, cpu_name, "riscv,isa-extensions",
                         formatted_ext, ext_len);

        qemu_fdt_setprop_cell(mc->fdt, cpu_name, "i-cache-block-size", CACHE_LINE_SIZE);
        qemu_fdt_setprop_cell(mc->fdt, cpu_name, "i-cache-sets", L1_ICACHE_SIZE / (CACHE_LINE_SIZE * L1_ICACHE_WAYS));
        qemu_fdt_setprop_cell(mc->fdt, cpu_name, "i-cache-size", L1_ICACHE_SIZE);
        qemu_fdt_setprop_cell(mc->fdt, cpu_name, "i-tlb-sets", 1);
        qemu_fdt_setprop_cell(mc->fdt, cpu_name, "i-tlb-size", ITLB_ENTRIES);

        qemu_fdt_setprop_cell(mc->fdt, cpu_name, "d-cache-block-size", CACHE_LINE_SIZE);
        qemu_fdt_setprop_cell(mc->fdt, cpu_name, "d-cache-sets", L1_DCACHE_SIZE / (CACHE_LINE_SIZE * L1_DCACHE_WAYS));
        qemu_fdt_setprop_cell(mc->fdt, cpu_name, "d-cache-size", L1_DCACHE_SIZE);
        qemu_fdt_setprop_cell(mc->fdt, cpu_name, "d-tlb-sets", 1);
        qemu_fdt_setprop_cell(mc->fdt, cpu_name, "d-tlb-size", DTLB_ENTRIES);

        qemu_fdt_setprop_cell(mc->fdt, cpu_name, "next-level-cache", l2cache_phandle);
        qemu_fdt_setprop(mc->fdt, cpu_name, "tlb-split", NULL, 0);

        qemu_fdt_setprop_cell(mc->fdt, cpu_name, "riscv,cbom-block-size", CACHE_LINE_SIZE);
        // qemu_fdt_setprop_cell(mc->fdt, cpu_name, "riscv,cbop-block-size", CACHE_LINE_SIZE);
        qemu_fdt_setprop_cell(mc->fdt, cpu_name, "riscv,cboz-block-size", CACHE_LINE_SIZE);

        qemu_fdt_setprop_string(mc->fdt, cpu_name, "compatible", "riscv");
        qemu_fdt_setprop_string(mc->fdt, cpu_name, "status", "okay");
        qemu_fdt_setprop_cell(mc->fdt, cpu_name, "reg",
                              s->soc.hartid_base + cpu);
        qemu_fdt_setprop_string(mc->fdt, cpu_name, "device_type", "cpu");

        intc_name = g_strdup_printf("%s/interrupt-controller", cpu_name);
        qemu_fdt_add_subnode(mc->fdt, intc_name);
        qemu_fdt_setprop_cell(mc->fdt, intc_name, "phandle",
                              intc_phandles[cpu]);
        qemu_fdt_setprop_string(mc->fdt, intc_name, "compatible",
                                "riscv,cpu-intc");
        qemu_fdt_setprop(mc->fdt, intc_name, "interrupt-controller", NULL, 0);
        qemu_fdt_setprop_cell(mc->fdt, intc_name, "#interrupt-cells", 1);
        qemu_fdt_setprop_cell(mc->fdt, intc_name, "#address-cells", 0);
        qemu_fdt_setprop_string(mc->fdt, intc_name, "status", "okay");

        g_free(intc_name);
        g_free(cpu_name);
        g_free(name);
        g_free(formatted_ext);
    }
}

static void create_fdt_socket_memory(HobgoblinState *s,
                                     const memmapEntry_t *memmap, int socket)
{
    MachineState *mc = MACHINE(s);
    HobgoblinClass *hc = HOBGOBLIN_MACHINE_GET_CLASS(s);

    uint64_t dram0_base = hc->dram[0].base;
    uint64_t dram1_base = hc->dram[1].base;

    char *name = g_strdup_printf("/memory@%lx", (long)dram0_base);
    qemu_fdt_add_subnode(mc->fdt, name);
    qemu_fdt_setprop_cells(mc->fdt, name, "reg", dram0_base >> 32, dram0_base,
                           dram1_base >> 32, dram1_base);
    qemu_fdt_setprop_string(mc->fdt, name, "device_type", "memory");
    riscv_socket_fdt_write_id(mc, mc->fdt, name, socket);
    g_free(name);
}

static void create_fdt_socket_reserved_memory(HobgoblinState *s,
                                              const memmapEntry_t *memmap,
                                              int socket)
{
    MachineState *mc = MACHINE(s);

    uint64_t cmu0_addr = 0x17f000000ULL;
    uint64_t cmu1_addr = 0x0ff000000ULL;
    uint64_t cmu_size = 0x001000000ULL;

    char *name = g_strdup_printf("/reserved-memory");
    qemu_fdt_add_subnode(mc->fdt, name);
    qemu_fdt_setprop_cell(mc->fdt, name, "#address-cells", 2);
    qemu_fdt_setprop_cell(mc->fdt, name, "#size-cells", 2);
    qemu_fdt_setprop(mc->fdt, name, "ranges", NULL, 0);
    g_free(name);

    name = g_strdup_printf("/reserved-memory/cmu@%" PRIx64, cmu0_addr);
    qemu_fdt_add_subnode(mc->fdt, name);
    qemu_fdt_setprop(mc->fdt, name, "no-map", NULL, 0);
    qemu_fdt_setprop_cells(mc->fdt, name, "reg", cmu0_addr >> 32, cmu0_addr,
                           cmu_size >> 32, cmu_size);
    g_free(name);

    name = g_strdup_printf("/reserved-memory/cmu@%" PRIx64, cmu1_addr);
    qemu_fdt_add_subnode(mc->fdt, name);
    qemu_fdt_setprop(mc->fdt, name, "no-map", NULL, 0);
    qemu_fdt_setprop_cells(mc->fdt, name, "reg", cmu1_addr >> 32, cmu1_addr,
                           cmu_size >> 32, cmu_size);
    g_free(name);
}

static void create_fdt_sockets(HobgoblinState *s, const memmapEntry_t *memmap,
                               bool is_32_bit, uint32_t l2cache_phandle,
                               uint32_t *intc_phandles)
{
    int socket;
    char *clust_name;
    MachineState *mc = MACHINE(s);

    qemu_fdt_add_subnode(mc->fdt, "/cpus");
    qemu_fdt_setprop_cell(mc->fdt, "/cpus", "timebase-frequency",
                          CLINT_TIMEBASE_FREQ);
    qemu_fdt_setprop_cell(mc->fdt, "/cpus", "#size-cells", 0);
    qemu_fdt_setprop_cell(mc->fdt, "/cpus", "#address-cells", 1);

    for (socket = 0; socket >= 0; socket--) {
        clust_name = g_strdup_printf("/cpus");

        create_fdt_socket_cpus(s, socket, clust_name, is_32_bit, l2cache_phandle, intc_phandles);

        create_fdt_socket_memory(s, memmap, socket);
        create_fdt_socket_reserved_memory(s, memmap, socket);

        g_free(clust_name);
    }
}

static void create_fdt_clock(HobgoblinState *s, const memmapEntry_t *memmap,
                             uint32_t clock_phandle)
{
    char *name;
    MachineState *mc = MACHINE(s);
    name = g_strdup_printf("/clock");
    qemu_fdt_add_subnode(mc->fdt, name);
    qemu_fdt_setprop_cell(mc->fdt, name, "#clock-cells", 0);
    qemu_fdt_setprop_cell(mc->fdt, name, "clock-frequency",
                          CLINT_TIMEBASE_FREQ);
    qemu_fdt_setprop_string(mc->fdt, name, "compatible", "fixed-clock");
    qemu_fdt_setprop_cell(mc->fdt, name, "phandle", clock_phandle);

    g_free(name);
}

static void create_fdt_l2cache(HobgoblinState *s, uint32_t l2cache_phandle)
{
    char *name;
    MachineState *mc = MACHINE(s);

    name = g_strdup_printf("/l2-cache");
    qemu_fdt_add_subnode(mc->fdt, name);
    qemu_fdt_setprop_string(mc->fdt, name, "compatible", "cache");
    qemu_fdt_setprop_cell(mc->fdt, name, "cache-block-size", CACHE_LINE_SIZE);
    qemu_fdt_setprop_cell(mc->fdt, name, "cache-level", 2);
    qemu_fdt_setprop_cell(mc->fdt, name, "cache-sets", L2_CACHE_SIZE / (CACHE_LINE_SIZE * L2_CACHE_WAYS));
    qemu_fdt_setprop_cell(mc->fdt, name, "cache-size", L2_CACHE_SIZE);
    qemu_fdt_setprop(mc->fdt, name, "cache-unified", NULL, 0);
    qemu_fdt_setprop_cell(mc->fdt, name, "phandle", l2cache_phandle);

    g_free(name);
}

static void create_fdt_virtio(HobgoblinState *s, const memmapEntry_t *memmap,
                              uint32_t irq_virtio_phandle)
{
    int i;
    char *name;
    MachineState *mc = MACHINE(s);
    const memmapEntry_t *mem_virtio = &memmap[HOBGOBLIN_VIRTIO];
    HobgoblinClass *hc = HOBGOBLIN_MACHINE_GET_CLASS(s);

    int virtio_transports =
        (MAPVERSION(s) == V2 && hc->board_type == BOARD_TYPE_VCU118)
            ? V2_VIRTIO_TRANSPORTS
            : V1_VIRTIO_TRANSPORTS;

    for (i = 0; i < virtio_transports; i++) {
        hwaddr offset = 0x200 * i;
        hwaddr base = mem_virtio->base + offset;

        name = g_strdup_printf("/soc@0/virtio@%lx", (long)base);
        qemu_fdt_add_subnode(mc->fdt, name);
        qemu_fdt_setprop_string(mc->fdt, name, "compatible", "virtio,mmio");

        qemu_fdt_setprop_cells(mc->fdt, name, "reg", base, 0x200);
        qemu_fdt_setprop_cells(mc->fdt, name, "interrupts-extended",
                               irq_virtio_phandle,
                               HIRQ(s, HOBGOBLIN_VIRTIO0_IRQ) + i);

        g_free(name);
    }
}

static void create_pcie_node(HobgoblinState *s, const memmapEntry_t *memmap,
                             uint32_t irq_mmio_phandle, int memmap_index,
                             int memmap_mmio0_index, int memmap_mmio1_index,
                             int irq_index, uint32_t phandle)
{
    char *name;
    const memmapEntry_t *entry = &memmap[memmap_index];
    MachineState *mc = MACHINE(s);

    name = g_strdup_printf("/soc%s@0/pcie@%lx",
                           riscv_is_32bit(&s->soc) ? "32" : "64", entry->base);
    qemu_fdt_add_subnode(mc->fdt, name);

    qemu_fdt_setprop_cell(mc->fdt, name, "#address-cells", 3);
    qemu_fdt_setprop_cell(mc->fdt, name, "#interrupt-cells", 1);
    qemu_fdt_setprop_cell(mc->fdt, name, "#size-cells", 2);
    qemu_fdt_setprop_cells(mc->fdt, name, "bus-range", 0, 0xff);

    qemu_fdt_setprop_string(mc->fdt, name, "compatible",
                            "xlnx,axi-pcie-host-1.00.a");
    qemu_fdt_setprop_string(mc->fdt, name, "device_type", "pci");

    // Interrupt map: maps PCI interrupts INTA-INTD to the interrupt controller
    uint32_t interrupt_map[] = {
        cpu_to_be32(0x00), cpu_to_be32(0x00),    cpu_to_be32(0x00),
        cpu_to_be32(0x01), cpu_to_be32(phandle), cpu_to_be32(0x01),
        cpu_to_be32(0x00), cpu_to_be32(0x00),    cpu_to_be32(0x00),
        cpu_to_be32(0x02), cpu_to_be32(phandle), cpu_to_be32(0x02),
        cpu_to_be32(0x00), cpu_to_be32(0x00),    cpu_to_be32(0x00),
        cpu_to_be32(0x03), cpu_to_be32(phandle), cpu_to_be32(0x03),
        cpu_to_be32(0x00), cpu_to_be32(0x00),    cpu_to_be32(0x00),
        cpu_to_be32(0x04), cpu_to_be32(phandle), cpu_to_be32(0x04)
    };
    qemu_fdt_setprop(mc->fdt, name, "interrupt-map", interrupt_map,
                     sizeof(interrupt_map));

    uint32_t interrupt_map_mask[] = { cpu_to_be32(0x00), cpu_to_be32(0x00),
                                      cpu_to_be32(0x00), cpu_to_be32(0x07) };
    qemu_fdt_setprop(mc->fdt, name, "interrupt-map-mask", interrupt_map_mask,
                     sizeof(interrupt_map_mask));

    uint32_t interrupts_extended[] = {
        cpu_to_be32(irq_mmio_phandle), cpu_to_be32(irqmap[1][irq_index]),
        cpu_to_be32(irq_mmio_phandle), cpu_to_be32(irqmap[1][irq_index + 1]),
        cpu_to_be32(irq_mmio_phandle), cpu_to_be32(irqmap[1][irq_index + 2])
    };
    qemu_fdt_setprop(mc->fdt, name, "interrupts-extended", interrupts_extended,
                     sizeof(interrupts_extended));

    const memmapEntry_t *mmio0 = &memmap[memmap_mmio0_index];
    const memmapEntry_t *mmio1 = &memmap[memmap_mmio1_index];
    uint32_t ranges[] = {
        cpu_to_be32(0x2000000),  // properties
        cpu_to_be32(mmio0->base >> 32), cpu_to_be32(mmio0->base),
        cpu_to_be32(mmio0->base >> 32), cpu_to_be32(mmio0->base),
        cpu_to_be32(mmio0->size >> 32), cpu_to_be32(mmio0->size),

        cpu_to_be32(0x43000000), // properties
        cpu_to_be32(mmio1->base >> 32), cpu_to_be32(mmio1->base),
        cpu_to_be32(mmio1->base >> 32), cpu_to_be32(mmio1->base),
        cpu_to_be32(mmio1->size >> 32), cpu_to_be32(mmio1->size),
    };
    qemu_fdt_setprop(mc->fdt, name, "ranges", ranges, sizeof(ranges));

    qemu_fdt_setprop_cells(mc->fdt, name, "reg", 0, entry->base, 0,
                           entry->size);

    qemu_fdt_setprop(mc->fdt, name, "xlnx,msi-decode", NULL, 0);

    char *ic_name = g_strdup_printf("%s/interrupt-controller", name);
    qemu_fdt_add_subnode(mc->fdt, ic_name);
    qemu_fdt_setprop_cell(mc->fdt, ic_name, "#address-cells", 0);
    qemu_fdt_setprop_cell(mc->fdt, ic_name, "#interrupt-cells", 1);
    qemu_fdt_setprop(mc->fdt, ic_name, "interrupt-controller", NULL, 0);
    qemu_fdt_setprop_cell(mc->fdt, ic_name, "phandle", phandle);

    g_free(ic_name);
    g_free(name);
}

static void create_fdt_pcie(HobgoblinState *s, const memmapEntry_t *memmap,
                            uint32_t irq_mmio_phandle, uint32_t pcie0_phandle,
                            uint32_t pcie1_phandle)
{
    MachineState *mc = MACHINE(s);
    char *soc_name =
        g_strdup_printf("/soc%s@0", riscv_is_32bit(&s->soc) ? "32" : "64");

    qemu_fdt_add_subnode(mc->fdt, soc_name);
    qemu_fdt_setprop_cell(mc->fdt, soc_name, "#address-cells", 0x02);
    qemu_fdt_setprop_cell(mc->fdt, soc_name, "#size-cells", 0x02);
    qemu_fdt_setprop_string(mc->fdt, soc_name, "compatible", "simple-bus");
    qemu_fdt_setprop_cells(mc->fdt, soc_name, "ranges", 0, 0, 0, 0, 64, 0);
    qemu_fdt_setprop(mc->fdt, soc_name, "dma-noncoherent", NULL, 0);

    create_pcie_node(s, memmap, irq_mmio_phandle, HOBGOBLIN_PCIE0,
                     HOBGOBLIN_PCIE0_MMIO0, HOBGOBLIN_PCIE0_MMIO1,
                     HOBGOBLIN2_PCIE0_IRQ, pcie0_phandle);
    create_pcie_node(s, memmap, irq_mmio_phandle, HOBGOBLIN_PCIE1,
                     HOBGOBLIN_PCIE1_MMIO0, HOBGOBLIN_PCIE1_MMIO1,
                     HOBGOBLIN2_PCIE1_IRQ, pcie1_phandle);

    g_free(soc_name);
}

static void create_fdt_plic(HobgoblinState *s, const memmapEntry_t *memmap,
                            uint32_t irq_mmio_phandle, uint32_t *intc_phandles)
{
    MachineState *mc = MACHINE(s);
    char *name = g_strdup_printf("/soc@0/interrupt-controller@%lx",
                                 memmap[HOBGOBLIN_PLIC].base);
    uint32_t *plic_cells = g_new0(uint32_t, s->soc.num_harts * 4);

    qemu_fdt_add_subnode(mc->fdt, name);
    qemu_fdt_setprop(mc->fdt, name, "interrupt-controller", NULL, 0);
    qemu_fdt_setprop_cell(mc->fdt, name, "#address-cells", 0);
    qemu_fdt_setprop_cell(mc->fdt, name, "#interrupt-cells", 1);
    qemu_fdt_setprop_cell(mc->fdt, name, "riscv,ndev", 63);
    qemu_fdt_setprop_cell(mc->fdt, name, "phandle", irq_mmio_phandle);
    qemu_fdt_setprop_cells(mc->fdt, name, "reg", memmap[HOBGOBLIN_PLIC].base,
                           memmap[HOBGOBLIN_PLIC].size);

    // Build interrupts-extended array: each hart gets M-mode and S-mode
    // external interrupts
    for (int cpu = 0; cpu < s->soc.num_harts; cpu++) {
        plic_cells[cpu * 4 + 0] = cpu_to_be32(intc_phandles[cpu]);
        plic_cells[cpu * 4 + 1] = cpu_to_be32(IRQ_M_EXT);
        plic_cells[cpu * 4 + 2] = cpu_to_be32(intc_phandles[cpu]);
        plic_cells[cpu * 4 + 3] = cpu_to_be32(IRQ_S_EXT);
    }
    qemu_fdt_setprop(mc->fdt, name, "interrupts-extended", plic_cells,
                     s->soc.num_harts * 4 * sizeof(uint32_t));

    const char compatible_strings[] =
        "codasip,plic\0sifive,plic-1.0.0\0riscv,plic0\0";
    qemu_fdt_setprop(mc->fdt, name, "compatible", compatible_strings,
                     sizeof(compatible_strings) - 1);

    g_free(plic_cells);
    g_free(name);
}

static void create_fdt_axi(HobgoblinState *s, const memmapEntry_t *memmap,
                           uint32_t irq_mmio_phandle, uint32_t fmc_axi_phandle,
                           uint32_t axi_phandle)
{
    char *name;
    MachineState *mc = MACHINE(s);

    name = g_strdup_printf("/soc@0/axistream_dma@%lx",
                           memmap[HOBGOBLIN_FMC_AXI_DMA].base);
    qemu_fdt_add_subnode(mc->fdt, name);
    qemu_fdt_setprop_string(mc->fdt, name, "compatible", "xlnx,eth-dma");
    qemu_fdt_setprop_cells(mc->fdt, name, "reg",
                           memmap[HOBGOBLIN_FMC_AXI_DMA].base,
                           memmap[HOBGOBLIN_FMC_AXI_DMA].size);

    uint32_t interrupts_extended_fmc[] = {
        cpu_to_be32(irq_mmio_phandle),
        cpu_to_be32(HIRQ(s, HOBGOBLIN_FMC_AXIDMA_IRQ0)),
        cpu_to_be32(irq_mmio_phandle),
        cpu_to_be32(HIRQ(s, HOBGOBLIN_FMC_AXIDMA_IRQ1))
    };
    qemu_fdt_setprop(mc->fdt, name, "interrupts-extended",
                     interrupts_extended_fmc, sizeof(interrupts_extended_fmc));

    qemu_fdt_setprop_cell(mc->fdt, name, "phandle", fmc_axi_phandle);

    g_free(name);

    name = g_strdup_printf("/soc@0/axistream_dma@%lx",
                           memmap[HOBGOBLIN_AXI_DMA].base);
    qemu_fdt_add_subnode(mc->fdt, name);
    qemu_fdt_setprop_string(mc->fdt, name, "compatible", "xlnx,eth-dma");
    qemu_fdt_setprop_cells(mc->fdt, name, "reg", memmap[HOBGOBLIN_AXI_DMA].base,
                           memmap[HOBGOBLIN_AXI_DMA].size);

    uint32_t interrupts_extended_axi[] = {
        cpu_to_be32(irq_mmio_phandle),
        cpu_to_be32(HIRQ(s, HOBGOBLIN_AXIDMA_IRQ0)),
        cpu_to_be32(irq_mmio_phandle),
        cpu_to_be32(HIRQ(s, HOBGOBLIN_AXIDMA_IRQ1))
    };
    qemu_fdt_setprop(mc->fdt, name, "interrupts-extended",
                     interrupts_extended_axi, sizeof(interrupts_extended_axi));

    qemu_fdt_setprop_cell(mc->fdt, name, "phandle", axi_phandle);

    g_free(name);
}

static void create_fdt_trng(HobgoblinState *s, const memmapEntry_t *memmap)
{
    char *name;
    MachineState *mc = MACHINE(s);
    name = g_strdup_printf("/soc@0/trng@%lx", memmap[HOBGOBLIN_TRNG].base);

    qemu_fdt_add_subnode(mc->fdt, name);
    qemu_fdt_setprop_string(mc->fdt, name, "compatible", "codasip,codasip-rng");
    qemu_fdt_setprop_cells(mc->fdt, name, "reg", memmap[HOBGOBLIN_TRNG].base,
                           memmap[HOBGOBLIN_TRNG].size);

    g_free(name);
}

static void create_fdt_timer(HobgoblinState *s, const memmapEntry_t *memmap,
                             uint32_t irq_mmio_phandle, uint32_t *intc_phandles)
{
    MachineState *mc = MACHINE(s);
    char *name;

    // CLINT timer node
    name = g_strdup_printf("/soc@0/timer@%lx", memmap[HOBGOBLIN_CLINT].base);
    qemu_fdt_add_subnode(mc->fdt, name);

    const char clint_compatible[] =
        "codasip,clint\0sifive,clint0\0riscv,clint0\0";
    qemu_fdt_setprop(mc->fdt, name, "compatible", clint_compatible,
                     sizeof(clint_compatible) - 1);

    uint32_t *clint_interrupts = g_new0(uint32_t, s->soc.num_harts * 4);
    for (int hart = 0; hart < s->soc.num_harts; hart++) {
        clint_interrupts[hart * 4 + 0] = cpu_to_be32(intc_phandles[hart]);
        clint_interrupts[hart * 4 + 1] = cpu_to_be32(3);
        clint_interrupts[hart * 4 + 2] = cpu_to_be32(intc_phandles[hart]);
        clint_interrupts[hart * 4 + 3] = cpu_to_be32(7);
    }
    qemu_fdt_setprop(mc->fdt, name, "interrupts-extended", clint_interrupts,
                     s->soc.num_harts * 4 * sizeof(uint32_t));
    qemu_fdt_setprop_cells(mc->fdt, name, "reg", memmap[HOBGOBLIN_CLINT].base,
                           memmap[HOBGOBLIN_CLINT].size);

    g_free(clint_interrupts);
    g_free(name);

    // AXI timer node
    name = g_strdup_printf("/soc@0/timer@%lx", memmap[HOBGOBLIN_TIMER].base);
    qemu_fdt_add_subnode(mc->fdt, name);
    qemu_fdt_setprop_cell(
        mc->fdt, name, "clock-frequency",
        object_property_get_uint(OBJECT(s->timer), "clock-frequency", NULL));
    qemu_fdt_setprop_cells(mc->fdt, name, "reg", memmap[HOBGOBLIN_TIMER].base,
                           memmap[HOBGOBLIN_TIMER].size);

    const char timer_types[] = "xlnx,axi-timer-2.0\0xlnx,xps-timer-1.00.a\0";
    qemu_fdt_setprop(mc->fdt, name, "compatible", timer_types,
                     sizeof(timer_types) - 1);

    uint32_t interrupts_extended[] = { cpu_to_be32(irq_mmio_phandle),
                                       cpu_to_be32(
                                           HIRQ(s, HOBGOBLIN_TIMER_IRQ)) };
    qemu_fdt_setprop(mc->fdt, name, "interrupts-extended", interrupts_extended,
                     sizeof(interrupts_extended));

    qemu_fdt_setprop_cell(
        mc->fdt, name, "xlnx,one-timer-only",
        object_property_get_uint(OBJECT(s->timer), "one-timer-only", NULL));
    qemu_fdt_setprop_cell(mc->fdt, name, "xlnx,count-width", 32);

    g_free(name);
}

static void create_fdt_spi(HobgoblinState *s, const memmapEntry_t *memmap,
                           uint32_t irq_mmio_phandle, uint32_t gpio_phandle)
{
    char *name, *mmc_name;
    MachineState *mc = MACHINE(s);

    name = g_strdup_printf("/soc@0/spi@%lx", memmap[HOBGOBLIN_SPI].base);
    qemu_fdt_add_subnode(mc->fdt, name);
    qemu_fdt_setprop_cell(mc->fdt, name, "#address-cells", 1);
    qemu_fdt_setprop_cell(mc->fdt, name, "#size-cells", 0);

    qemu_fdt_setprop_cell(
        mc->fdt, name, "clock-frequency",
        object_property_get_uint(OBJECT(s->timer), "clock-frequency", NULL));
    qemu_fdt_setprop_string(mc->fdt, name, "compatible", "xlnx,xps-spi-2.00.a");
    qemu_fdt_setprop_cell(mc->fdt, name, "fifo-size", 0x100);
    qemu_fdt_setprop_cell(
        mc->fdt, name, "xlnx,num-ss-bits",
        object_property_get_uint(OBJECT(s->spi), "num-ss-bits", NULL));
    qemu_fdt_setprop_cell(mc->fdt, name, "xlnx,num-transfer-bits", 0x08);

    uint32_t interrupts_extended[] = {
        cpu_to_be32(irq_mmio_phandle),
        cpu_to_be32(HIRQ(s, HOBGOBLIN_SPI_IRQ)),
    };
    qemu_fdt_setprop(mc->fdt, name, "interrupts-extended", interrupts_extended,
                     sizeof(interrupts_extended));
    qemu_fdt_setprop_cells(mc->fdt, name, "reg", memmap[HOBGOBLIN_SPI].base,
                           memmap[HOBGOBLIN_SPI].size);

    // Create MMC child node
    mmc_name = g_strdup_printf("%s/mmc@0", name);
    qemu_fdt_add_subnode(mc->fdt, mmc_name);
    qemu_fdt_setprop_string(mc->fdt, mmc_name, "compatible", "mmc-spi-slot");
    qemu_fdt_setprop(mc->fdt, mmc_name, "disable-wp", NULL,
                     0); // Boolean property
    qemu_fdt_setprop_cell(mc->fdt, mmc_name, "reg", 0);
    qemu_fdt_setprop_cell(mc->fdt, mmc_name, "spi-max-frequency", 20000000);

    uint32_t gpios[] = { cpu_to_be32(gpio_phandle), // GPIO controller phandle
                         cpu_to_be32(0), cpu_to_be32(1) };
    qemu_fdt_setprop(mc->fdt, mmc_name, "gpios", gpios, sizeof(gpios));

    uint32_t voltage_ranges[] = { cpu_to_be32(0xce4), cpu_to_be32(0xce4) };
    qemu_fdt_setprop(mc->fdt, mmc_name, "voltage-ranges", voltage_ranges,
                     sizeof(voltage_ranges));

    g_free(mmc_name);
    g_free(name);
}

static void create_fdt_uart(HobgoblinState *s, const memmapEntry_t *memmap,
                            uint32_t irq_mmio_phandle)
{
    char *name;
    MachineState *mc = MACHINE(s);

    name =
        g_strdup_printf("/soc@0/serial@%lx", memmap[HOBGOBLIN_UART0].base);
    qemu_fdt_add_subnode(mc->fdt, name);
    qemu_fdt_setprop_cell(
        mc->fdt, name, "clock-frequency",
        object_property_get_uint(OBJECT(s->timer), "clock-frequency", NULL));
    qemu_fdt_setprop_string(mc->fdt, name, "compatible", "ns16550a");
    qemu_fdt_setprop_cell(mc->fdt, name, "current-speed", 115200);
    uint32_t interrupts_extended0[] = {
        cpu_to_be32(irq_mmio_phandle),
        cpu_to_be32(HIRQ(s, HOBGOBLIN_UART0_IRQ)),
    };
    qemu_fdt_setprop(mc->fdt, name, "interrupts-extended", interrupts_extended0,
                     sizeof(interrupts_extended0));
    qemu_fdt_setprop_cells(mc->fdt, name, "reg",
                           memmap[HOBGOBLIN_UART0].base,
                           memmap[HOBGOBLIN_UART0].size);
    qemu_fdt_setprop_cell(mc->fdt, name, "reg-shift", 2);
    g_free(name);

    name = g_strdup_printf("/soc@0/serial@%lx", memmap[HOBGOBLIN_UART1].base);
    qemu_fdt_add_subnode(mc->fdt, name);
    qemu_fdt_setprop_string(mc->fdt, name, "compatible",
                            "xlnx,xps-uartlite-1.00.a");
    qemu_fdt_setprop_cell(mc->fdt, name, "current-speed", 115200);
    uint32_t interrupts_extended1[] = {
        cpu_to_be32(irq_mmio_phandle),
        cpu_to_be32(HIRQ(s, HOBGOBLIN_UART1_IRQ)),
    };
    qemu_fdt_setprop(mc->fdt, name, "interrupts-extended", interrupts_extended1,
                     sizeof(interrupts_extended1));
    qemu_fdt_setprop_cells(mc->fdt, name, "reg", memmap[HOBGOBLIN_UART1].base,
                           0x10);
    qemu_fdt_setprop_cell(mc->fdt, name, "xlnx,data-bits", 8);
    qemu_fdt_setprop_cell(mc->fdt, name, "xlnx,use-parity", 0);
    g_free(name);
}

static void add_fdt_ethernet_node(
    HobgoblinState *s, const memmapEntry_t *memmap, uint32_t irq_mmio_phandle,
    int eth_idx, uint32_t axistream_connected, uint32_t eth_irq,
    uint32_t phy_phandle, const char *phy_mode, uint32_t phy_reg,
    uint32_t phy_irq, uint32_t clock_phandle, bool has_ti_quirks)
{
    char *name, *mdio_name, *phy_name;
    MachineState *mc = MACHINE(s);

    name = g_strdup_printf("/soc@0/ethernet@%lx", memmap[eth_idx].base);
    qemu_fdt_add_subnode(mc->fdt, name);

    qemu_fdt_setprop_cell(mc->fdt, name, "axistream-connected",
                          axistream_connected);
    qemu_fdt_setprop_string(mc->fdt, name, "clock-names", "s_axi_lite_clk");
    qemu_fdt_setprop_cell(mc->fdt, name, "clocks", clock_phandle);
    qemu_fdt_setprop_string(mc->fdt, name, "compatible",
                            "xlnx,axi-ethernet-1.00.a");

    uint32_t interrupts_extended[] = {
        cpu_to_be32(irq_mmio_phandle),
        cpu_to_be32(eth_irq),
    };
    qemu_fdt_setprop(mc->fdt, name, "interrupts-extended", interrupts_extended,
                     sizeof(interrupts_extended));

    uint8_t mac_addr[6] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    qemu_fdt_setprop(mc->fdt, name, "local-mac-address", mac_addr,
                     sizeof(mac_addr));

    qemu_fdt_setprop_cell(mc->fdt, name, "phy-handle", phy_phandle);
    qemu_fdt_setprop_string(mc->fdt, name, "phy-mode", phy_mode);
    qemu_fdt_setprop_cells(mc->fdt, name, "reg", memmap[eth_idx].base,
                           memmap[eth_idx].size);

    qemu_fdt_setprop_cell(mc->fdt, name, "xlnx,rxcsum", 0x02);
    qemu_fdt_setprop_cell(mc->fdt, name, "xlnx,rxmem", 0x4000);
    qemu_fdt_setprop_cell(mc->fdt, name, "xlnx,txcsum", 0x02);

    mdio_name = g_strdup_printf("%s/mdio", name);
    qemu_fdt_add_subnode(mc->fdt, mdio_name);
    qemu_fdt_setprop_cell(mc->fdt, mdio_name, "#address-cells", 0x01);
    qemu_fdt_setprop_cell(mc->fdt, mdio_name, "#size-cells", 0x00);

    phy_name = g_strdup_printf("%s/ethernet-phy@%x", mdio_name, phy_reg);
    qemu_fdt_add_subnode(mc->fdt, phy_name);
    qemu_fdt_setprop_string(mc->fdt, phy_name, "device_type", "ethernet-phy");

    uint32_t phy_interrupts_extended[] = {
        cpu_to_be32(irq_mmio_phandle),
        cpu_to_be32(phy_irq),
    };
    qemu_fdt_setprop(mc->fdt, phy_name, "interrupts-extended",
                     phy_interrupts_extended, sizeof(phy_interrupts_extended));

    qemu_fdt_setprop_cell(mc->fdt, phy_name, "phandle", phy_phandle);
    qemu_fdt_setprop_cell(mc->fdt, phy_name, "reg", phy_reg);

    if (strcmp(phy_mode, "rgmii-rxid") == 0) {
        qemu_fdt_setprop_cell(mc->fdt, phy_name, "rx-internal-delay-ps", 0x7d0);
    }

    if (has_ti_quirks) {
        qemu_fdt_setprop(mc->fdt, phy_name, "ti,dp83867-rxctrl-strap-quirk",
                         NULL, 0);
        qemu_fdt_setprop(mc->fdt, phy_name, "ti,sgmii-ref-clock-output-enable",
                         NULL, 0);
    }

    g_free(name);
    g_free(mdio_name);
    g_free(phy_name);
}

static void create_fdt_ethernet(HobgoblinState *s, const memmapEntry_t *memmap,
                                uint32_t irq_mmio_phandle,
                                uint32_t fmc_axi_phandle,
                                uint32_t fmc_phy_phandle, uint32_t axi_phandle,
                                uint32_t axi_phy_phandle,
                                uint32_t clock_phandle)
{
    // Add FMC ethernet (RGMII interface)
    add_fdt_ethernet_node(
        s, memmap, irq_mmio_phandle,
        HOBGOBLIN_FMC_AXI_ETH,                // memmap index
        fmc_axi_phandle,                      // axistream-connected
        HIRQ(s, HOBGOBLIN_FMC_ETH_IRQ),       // ethernet interrupt
        fmc_phy_phandle,                      // phy phandle
        "rgmii-rxid",                         // phy-mode
        0x01,                                 // phy reg address
        HIRQ(s, HOBGOBLIN_FMC_ETH_PHY_IRQ),   // phy interrupt
        clock_phandle,                        // clock phandle
        false);                               // no TI quirks

    // Add ethernet (SGMII interface)
    add_fdt_ethernet_node(
        s, memmap, irq_mmio_phandle,
        HOBGOBLIN_AXI_ETH,                    // memmap index
        axi_phandle,                          // axistream-connected
        HIRQ(s, HOBGOBLIN_ETH_IRQ),           // ethernet interrupt
        axi_phy_phandle,                      // phy phandle
        "sgmii",                              // phy-mode
        0x03,                                 // phy reg address
        HIRQ(s, HOBGOBLIN_ETH_PHY_IRQ),       // phy interrupt
        clock_phandle,                        // clock phandle
        true);                                // has TI quirks
}

static void create_gpio_node(HobgoblinState *s, const memmapEntry_t *memmap,
                             uint32_t irq_mmio_phandle, int memmap_index,
                             int irq_index, uint32_t phandle,
                             uint32_t tri_default, const char *line_name,
                             int gpio_hog)
{
    char *name;
    const memmapEntry_t *entry = &memmap[memmap_index];
    MachineState *mc = MACHINE(s);

    name = g_strdup_printf("/soc@0/gpio@%lx", entry->base);
    qemu_fdt_add_subnode(mc->fdt, name);

    qemu_fdt_setprop_cell(mc->fdt, name, "#address-cells", 0);
    qemu_fdt_setprop_cell(mc->fdt, name, "#gpio-cells", 2);
    qemu_fdt_setprop_cell(mc->fdt, name, "#interrupt-cells", 2);

    qemu_fdt_setprop_string(mc->fdt, name, "compatible",
                            "xlnx,xps-gpio-1.00.a");
    qemu_fdt_setprop(mc->fdt, name, "gpio-controller", NULL, 0);
    qemu_fdt_setprop(mc->fdt, name, "interrupt-controller", NULL, 0);

    uint32_t interrupts_extended[] = { cpu_to_be32(irq_mmio_phandle),
                                       cpu_to_be32(HIRQ(s, irq_index)) };
    qemu_fdt_setprop(mc->fdt, name, "interrupts-extended", interrupts_extended,
                     sizeof(interrupts_extended));
    qemu_fdt_setprop_cell(mc->fdt, name, "phandle", phandle);

    qemu_fdt_setprop_cells(mc->fdt, name, "reg", entry->base, entry->size);

    qemu_fdt_setprop_cell(mc->fdt, name, "xlnx,all-inputs", 0);
    qemu_fdt_setprop_cell(mc->fdt, name, "xlnx,all-outputs", 0);
    qemu_fdt_setprop_cell(mc->fdt, name, "xlnx,dout-default", 0);
    qemu_fdt_setprop_cell(mc->fdt, name, "xlnx,gpio-width", 32);
    qemu_fdt_setprop_cell(mc->fdt, name, "xlnx,interrupt-present", 1);
    qemu_fdt_setprop_cell(mc->fdt, name, "xlnx,is-dual", 0);
    qemu_fdt_setprop_cell(mc->fdt, name, "xlnx,tri-default", tri_default);

    if (gpio_hog) {
        char *hog_name = g_strdup_printf("%s/sdcard_hog", name);
        qemu_fdt_add_subnode(mc->fdt, hog_name);
        qemu_fdt_setprop(mc->fdt, hog_name, "gpio-hog", NULL, 0);
        uint32_t gpios[] = { cpu_to_be32(0x02), cpu_to_be32(0x00) };
        qemu_fdt_setprop(mc->fdt, hog_name, "gpios", gpios, sizeof(gpios));
        qemu_fdt_setprop_string(mc->fdt, hog_name, "line-name", line_name);
        qemu_fdt_setprop(mc->fdt, hog_name, "output-high", NULL, 0);
        g_free(hog_name);
    }

    g_free(name);
}

static void create_gpio_key_node(MachineState *mc, const char *parent_path,
                                 const char *key_name, uint32_t gpio_phandle,
                                 uint32_t gpio_pin, const char *label,
                                 uint32_t linux_code)
{
    char *name = g_strdup_printf("%s/%s", parent_path, key_name);
    qemu_fdt_add_subnode(mc->fdt, name);

    uint32_t gpios[] = { cpu_to_be32(gpio_phandle), cpu_to_be32(gpio_pin),
                         cpu_to_be32(0x00) };
    qemu_fdt_setprop(mc->fdt, name, "gpios", gpios, sizeof(gpios));
    qemu_fdt_setprop_string(mc->fdt, name, "label", label);
    qemu_fdt_setprop_cell(mc->fdt, name, "linux,code", linux_code);

    g_free(name);
}

static void create_gpio_led_node(MachineState *mc, const char *parent_path,
                                 uint32_t gpio_phandle, int led_number,
                                 const char *trigger)
{
    char *name = g_strdup_printf("%s/led-%d", parent_path, led_number);
    char *label = g_strdup_printf("LD%d", led_number);

    qemu_fdt_add_subnode(mc->fdt, name);

    uint32_t gpios[] = { cpu_to_be32(gpio_phandle), cpu_to_be32(led_number),
                         cpu_to_be32(0x00) };
    qemu_fdt_setprop(mc->fdt, name, "gpios", gpios, sizeof(gpios));
    qemu_fdt_setprop_string(mc->fdt, name, "label", label);

    if (trigger) {
        qemu_fdt_setprop_string(mc->fdt, name, "linux,default-trigger",
                                trigger);
    }

    g_free(name);
    g_free(label);
}

static void create_fdt_gpio(HobgoblinState *s, const memmapEntry_t *memmap,
                            uint32_t irq_mmio_phandle, uint32_t gpio0_phandle,
                            uint32_t gpio1_phandle)
{
    MachineState *mc = MACHINE(s);
    const char *gpio_keys_path = "/gpio-keys";
    const char *gpio_restart_path = "/gpio-restart";
    const char *leds_path = "/leds";

    create_gpio_node(s, memmap, irq_mmio_phandle, HOBGOBLIN_GPIO0,
                     HOBGOBLIN_GPIO0_IRQ, gpio0_phandle, 0xfe, NULL, 0);

    create_gpio_node(s, memmap, irq_mmio_phandle, HOBGOBLIN_GPIO1,
                     HOBGOBLIN_GPIO1_IRQ, gpio1_phandle, 0x01,
                     "sdcard-fast-gpio", 1);

    qemu_fdt_add_subnode(mc->fdt, gpio_keys_path);
    qemu_fdt_setprop_string(mc->fdt, gpio_keys_path, "compatible", "gpio-keys");

    create_gpio_key_node(mc, gpio_keys_path, "key-down", gpio0_phandle, 0x11,
                         "BTND", 0x6c);
    create_gpio_key_node(mc, gpio_keys_path, "key-enter", gpio0_phandle, 0x10,
                         "BTNC", 0x1c);
    create_gpio_key_node(mc, gpio_keys_path, "key-left", gpio0_phandle, 0x12,
                         "BTNL", 0x69);
    create_gpio_key_node(mc, gpio_keys_path, "key-right", gpio0_phandle, 0x13,
                         "BTNR", 0x6a);
    create_gpio_key_node(mc, gpio_keys_path, "key-up", gpio0_phandle, 0x14,
                         "BTNU", 0x67);

    qemu_fdt_add_subnode(mc->fdt, gpio_restart_path);
    qemu_fdt_setprop_string(mc->fdt, gpio_restart_path, "compatible",
                            "gpio-restart");
    uint32_t restart_gpios[] = { cpu_to_be32(gpio1_phandle), cpu_to_be32(0x03),
                                 cpu_to_be32(0x00) };
    qemu_fdt_setprop(mc->fdt, gpio_restart_path, "gpios", restart_gpios,
                     sizeof(restart_gpios));

    qemu_fdt_add_subnode(mc->fdt, leds_path);
    qemu_fdt_setprop_string(mc->fdt, leds_path, "compatible", "gpio-leds");

    for (int i = 0; i < 8; i++) {
        const char *trigger = (i == 0) ? "heartbeat" : (i == 1) ? "mmc0" : NULL;
        create_gpio_led_node(mc, leds_path, gpio0_phandle, i, trigger);
    }
}

static void create_fdt(HobgoblinState *s, const memmapEntry_t *memmap,
                       uint64_t mem_size, const char *cmdline, bool is_32_bit)
{
    MachineState *mc = MACHINE(s);

    if (mc->dtb) {
        mc->fdt = load_device_tree(mc->dtb, &s->fdt_size);
        if (!mc->fdt) {
            error_report("load_device_tree() failed");
            exit(1);
        }
    } else {
        mc->fdt = create_device_tree(&s->fdt_size);
        if (!mc->fdt) {
            error_report("create_device_tree() failed");
            exit(1);
        }

        uint32_t phandle = 0;
        uint32_t *intc_phandles = g_new0(uint32_t, s->soc.num_harts);

        uint32_t l2cache_phandle = ++phandle;
        for (int i = 0; i < s->soc.num_harts; i++) {
            intc_phandles[i] = ++phandle;
        }

        uint32_t irq_mmio_phandle = ++phandle;
        uint32_t gpio1_phandle = ++phandle;
        uint32_t clock_phandle = ++phandle;
        uint32_t axi_phy_phandle = ++phandle;
        uint32_t axi_phandle = ++phandle;
        uint32_t i2c0_phandle __attribute__((unused)) = ++phandle;
        uint32_t fmc_phy_phandle = ++phandle;
        uint32_t fmc_axi_phandle = ++phandle;
        uint32_t i2c1_phandle __attribute__((unused)) = ++phandle;
        uint32_t pcie0_phandle = ++phandle;
        uint32_t pcie1_phandle = ++phandle;
        uint32_t gpio0_phandle = ++phandle;

        qemu_fdt_setprop_string(mc->fdt, "/", "model", "hobgoblin,qemu");
        qemu_fdt_setprop_string(mc->fdt, "/", "compatible", "codasip,a730");
        qemu_fdt_setprop_cell(mc->fdt, "/", "#size-cells", 0x2);
        qemu_fdt_setprop_cell(mc->fdt, "/", "#address-cells", 0x2);

        qemu_fdt_add_subnode(mc->fdt, "/soc@0");
        qemu_fdt_setprop_cell(mc->fdt, "/soc@0", "#address-cells", 0x01);
        qemu_fdt_setprop_cell(mc->fdt, "/soc@0", "#size-cells", 0x01);
        qemu_fdt_setprop_string(mc->fdt, "/soc@0", "compatible", "simple-bus");
        qemu_fdt_setprop_cells(mc->fdt, "/soc@0", "ranges", 0x00, 0x00, 0x00,
                               0x80000000);
        qemu_fdt_setprop(mc->fdt, "/soc@0", "dma-noncoherent", NULL, 0);

        qemu_fdt_add_subnode(mc->fdt, "/chosen");
        qemu_fdt_setprop_string(mc->fdt, "/chosen", "bootargs",
                                "earlycon console=ttyS0,115200n8");
        qemu_fdt_setprop_string(mc->fdt, "/chosen", "stdout-path",
                                "serial0:115200n8");

        create_fdt_aliases(s, memmap);
        create_fdt_sockets(s, memmap, is_32_bit, l2cache_phandle,
                           intc_phandles);
        create_fdt_l2cache(s, l2cache_phandle);
        create_fdt_virtio(s, memmap, irq_mmio_phandle);
        create_fdt_pcie(s, memmap, irq_mmio_phandle, pcie0_phandle,
                        pcie1_phandle);
        create_fdt_plic(s, memmap, irq_mmio_phandle, intc_phandles);
        create_fdt_axi(s, memmap, irq_mmio_phandle, fmc_axi_phandle,
                       axi_phandle);
        create_fdt_clock(s, memmap, clock_phandle);
        create_fdt_trng(s, memmap);
        create_fdt_timer(s, memmap, irq_mmio_phandle, intc_phandles);
        create_fdt_spi(s, memmap, irq_mmio_phandle, gpio1_phandle);
        create_fdt_uart(s, memmap, irq_mmio_phandle);
        create_fdt_ethernet(s, memmap, irq_mmio_phandle, fmc_axi_phandle,
                            fmc_phy_phandle, axi_phandle, axi_phy_phandle,
                            clock_phandle);
        create_fdt_gpio(s, memmap, irq_mmio_phandle, gpio0_phandle,
                        gpio1_phandle);

        g_free(intc_phandles);
    }
}

static void hobgoblin_machine_init(MachineState *machine)
{
    HobgoblinState *s = HOBGOBLIN_MACHINE(machine);
    HobgoblinClass *hc = HOBGOBLIN_MACHINE_GET_CLASS(s);
    MemoryRegion *system_memory = get_system_memory();
    MemoryRegion __attribute__((unused)) *sram, *ddr[MAX_DRAM];
    const memmapEntry_t *memmap = address_maps[MAPVERSION(s)];
    const int smp_cpus = machine->smp.cpus;
    const memmapEntry_t *dram;

    hobgoblin_add_soc(s, smp_cpus);

    /* add memory regions */
    hobgoblin_add_memory_area(system_memory, &memmap[HOBGOBLIN_MROM]);
    hobgoblin_add_memory_area(system_memory, &memmap[HOBGOBLIN_BOOT_ROM]);
    sram = hobgoblin_add_memory_area(system_memory, &memmap[HOBGOBLIN_SRAM]);

    for (int i = 0; i < hc->dram_banks; i++) {
        ddr[i] = hobgoblin_add_memory_area(system_memory, &hc->dram[i]);
    }
    dram = &hc->dram[0];

    /* add interrupt controller */
    hobgoblin_add_interrupt_controller(s, smp_cpus);

    /* add peripherals (requires having an interrupt controller) */
    hobgoblin_add_id_register(s, system_memory);
#ifdef TARGET_CHERI
    //Hobgoblin V2 has no CMU on SRAM
    if (MAPVERSION(s) == V1) {
        hobgoblin_add_cmu(s, &s->internal_cmu, &memmap[HOBGOBLIN_INTL_CMU], sram);
    }
    for (int i = 0; i < hc->dram_banks; i++) {
        hobgoblin_add_cmu(s, &s->ddr_cmu[i], &memmap[HOBGOBLIN_CMU_DDR0+i], ddr[i]);
    }
#endif
    hobgoblin_add_uart(s, system_memory);
    if (hc->board_type == BOARD_TYPE_VCU118)
        hobgoblin_add_uartlite(s, system_memory);
    hobgoblin_add_gpio(s);
    hobgoblin_add_spi(s);
    hobgoblin_add_sd(s);
    if (hc->board_type == BOARD_TYPE_VCU118)
        hobgoblin_add_axi_ethernet(s, 1, 1,
            HOBGOBLIN_FMC_AXI_ETH, HOBGOBLIN_FMC_AXI_DMA,
            HIRQ(s, HOBGOBLIN_FMC_ETH_IRQ),
            HIRQ(s, HOBGOBLIN_FMC_AXIDMA_IRQ0), HIRQ(s, HOBGOBLIN_FMC_AXIDMA_IRQ1));
    switch (s->eth_type) {
    case ETH_TYPE_ETHERNETLITE:
        hobgoblin_add_ethernetlite(s);
        break;
    case ETH_TYPE_AXI_ETHERNET:
        hobgoblin_add_axi_ethernet(s, 0,
            (hc->board_type == BOARD_TYPE_VCU118) ? 3 : 1,
            HOBGOBLIN_AXI_ETH, HOBGOBLIN_AXI_DMA,
            HIRQ(s, HOBGOBLIN_ETH_IRQ),
            HIRQ(s, HOBGOBLIN_AXIDMA_IRQ0), HIRQ(s, HOBGOBLIN_AXIDMA_IRQ1));
        break;
    }
    hobgoblin_add_trng(s);
    hobgoblin_add_nvemu(s);
    hobgoblin_add_timer(s);
    hobgoblin_add_virtio(s);

    if (hc->board_type == BOARD_TYPE_VCU118 && MAPVERSION(s) == V2) {
        hobgoblin_add_xilinx_pcie(s, system_memory, 0,
            HOBGOBLIN_PCIE0,
            (int[]){HOBGOBLIN_PCIE0_MMIO0, HOBGOBLIN_PCIE0_MMIO1}, 2,
            HIRQ(s, HOBGOBLIN2_PCIE0_IRQ),
            (int[]){HIRQ(s, HOBGOBLIN2_PCIE0_MSI0_IRQ), HIRQ(s, HOBGOBLIN2_PCIE0_MSI1_IRQ)},
            true);
        hobgoblin_add_xilinx_pcie(s, system_memory, 1,
            HOBGOBLIN_PCIE1,
            (int[]){HOBGOBLIN_PCIE1_MMIO0, HOBGOBLIN_PCIE1_MMIO1}, 2,
            HIRQ(s, HOBGOBLIN2_PCIE1_IRQ),
            (int[]){HIRQ(s, HOBGOBLIN2_PCIE1_MSI0_IRQ), HIRQ(s, HOBGOBLIN2_PCIE1_MSI1_IRQ)},
            true);
    }

    /* create device tree */
    create_fdt(s, memmap, machine->ram_size, machine->kernel_cmdline,
               riscv_is_32bit(&s->soc));

    /* load images into memory to boot the platform */
    int ret = hobgoblin_load_images(s, dram);
    if (ret != 0) {
        error_report("loading images failed (%d)", ret);
        exit(1);
    }
}

static bool hobgoblin_machine_get_boot_from_rom(Object *obj, Error **errp)
{
    HobgoblinState *s = HOBGOBLIN_MACHINE(obj);

    return s->boot_from_rom;
}

static void hobgoblin_machine_set_boot_from_rom(Object *obj, bool value,
                                                Error **errp)
{
    HobgoblinState *s = HOBGOBLIN_MACHINE(obj);

    s->boot_from_rom = value;
}

static char *hobgoblin_machine_get_eth_type(Object *obj, Error **errp)
{
    HobgoblinState *s = HOBGOBLIN_MACHINE(obj);
    const char *result;

    switch (s->eth_type) {
    case ETH_TYPE_AXI_ETHERNET:
        result = "axi-ethernet";
        break;
    case ETH_TYPE_ETHERNETLITE:
        result = "ethernetlite";
        break;
    default:
        result = "Unknown";
        break;
    }

    return g_strdup(result);
}

static void hobgoblin_machine_set_eth_type(Object *obj, const char *value,
                                           Error **errp)
{
    HobgoblinState *s = HOBGOBLIN_MACHINE(obj);

    if (!strcmp(value, "axi-ethernet"))
        s->eth_type = ETH_TYPE_AXI_ETHERNET;
    else if (!strcmp(value, "ethernetlite"))
        s->eth_type = ETH_TYPE_ETHERNETLITE;
    else
        error_setg(errp, "Unrecognised eth-type");
}

static void hobgoblin_machine_instance_init(Object *obj)
{
    HobgoblinState *s = HOBGOBLIN_MACHINE(obj);

    s->boot_from_rom = false;
    s->eth_type = ETH_TYPE_AXI_ETHERNET;
}

static void hobgoblin_machine_class_init(ObjectClass *oc, void *data)
{
    MachineClass *mc = MACHINE_CLASS(oc);

    mc->init = hobgoblin_machine_init;
#if defined(TARGET_RISCV64)
    mc->default_cpu_type = TYPE_RISCV_CPU_CODASIP_A730;
#elif defined(TARGET_RISCV32)
    mc->default_cpu_type = TYPE_RISCV_CPU_CODASIP_L730;
#endif

    /* mc->reset:   void reset(MachineState *state, ShutdownCause reason); */
    /* mc->wakeup:  void wakeup(MachineState *state); */

    object_class_property_add_bool(oc, "boot-from-rom",
                                   hobgoblin_machine_get_boot_from_rom,
                                   hobgoblin_machine_set_boot_from_rom);
    object_class_property_set_description(oc, "boot-from-rom",
        "Load BIOS (default fsbl_rom.xexe) into ROM and boot into it");

    object_class_property_add_str(oc, "eth-type",
                                  hobgoblin_machine_get_eth_type,
                                  hobgoblin_machine_set_eth_type);
    object_class_property_set_description(oc, "eth-type",
        "Set the Ethernet type (axi-ethernet (default) or ethernetlite)");
}

struct HobgoblinInitData {
    enum board_type board_type;
    const char *desc;
    unsigned int cpus;
    const memmapEntry_t *dram;
    int dram_banks;
    int map_version;
    int irq_map_version;
};

static void hobgoblin_concrete_machine_class_init(ObjectClass *oc, void *data)
{
    MachineClass *mc = MACHINE_CLASS(oc);
    HobgoblinClass *hc = HOBGOBLIN_MACHINE_CLASS(oc);
    struct HobgoblinInitData *hid = data;

    mc->desc = hid->desc;
    mc->max_cpus = hid->cpus;
    mc->min_cpus = 1;
    mc->default_cpus = hid->cpus;
    hc->board_type = hid->board_type;
    hc->dram = hid->dram;
    hc->dram_banks = hid->dram_banks;
    hc->map_version = hid->map_version;
    hc->irq_map_version = hid->irq_map_version;
}

#define HOBGOBLIN_MACHINE(_type, _desc, _cpus, _dram, _map, _irq_map) {         \
    .name          = TYPE_HOBGOBLIN_ ## _type ## _map ## _MACHINE,      \
    .parent        = TYPE_HOBGOBLIN_MACHINE,                    \
    .class_init    = hobgoblin_concrete_machine_class_init,      \
    .class_data    = &((struct HobgoblinInitData) {             \
        .board_type = BOARD_TYPE_ ## _type,                     \
        .desc = _desc,                                          \
        .cpus = _cpus,                                          \
        .dram = _dram,                                          \
        .dram_banks = ARRAY_SIZE(_dram),                        \
        .map_version = _map,                                    \
        .irq_map_version = _irq_map,                            \
    })                                                          \
}

#define HOBGOBLIN_MACHINE_NAMED(_name, _type, _desc, _cpus, _dram, _map, _irq_map) {         \
    .name          = _name,                                       \
    .parent        = TYPE_HOBGOBLIN_MACHINE,                    \
    .class_init    = hobgoblin_concrete_machine_class_init,      \
    .class_data    = &((struct HobgoblinInitData) {             \
        .board_type = BOARD_TYPE_ ## _type,                     \
        .desc = _desc,                                          \
        .cpus = _cpus,                                          \
        .dram = _dram,                                          \
        .dram_banks = ARRAY_SIZE(_dram),                        \
        .map_version = _map,                                    \
        .irq_map_version = _irq_map,                            \
    })                                                          \
}
static const TypeInfo hobgoblin_machines_typeinfo[] = {
    {
        .name          = TYPE_HOBGOBLIN_MACHINE,
        .parent        = TYPE_MACHINE,
        .abstract      = true,
        .instance_size = sizeof(HobgoblinState),
        .class_size    = sizeof(HobgoblinClass),
        .instance_init = hobgoblin_machine_instance_init,
        .class_init    = hobgoblin_machine_class_init,
    },
    HOBGOBLIN_MACHINE(GENESYS2,
                      "RISC-V Hobgoblin_v1 (Genesys2) board",
                      1, genesys2_dram_memmap, V1, V1),
    HOBGOBLIN_MACHINE(PROFPGA,
                      "RISC-V Hobgoblin_v1 (proFPGA) board",
                      4, profpga_dram_memmap, V1, V1),
    HOBGOBLIN_MACHINE(VCU118,
                      "RISC-V Hobgoblin_v1 (VCU118) board",
                      4, vcu118_dram_memmap, V1, V1),
    HOBGOBLIN_MACHINE(GENESYS2,
                      "RISC-V Hobgoblin_v2 (Genesys2) board",
                      1, genesys2_dram_memmap, V2, V2),
    HOBGOBLIN_MACHINE(PROFPGA,
                      "RISC-V Hobgoblin_v2 (proFPGA) board",
                      4, profpga_dram_memmap, V2, V2),
    HOBGOBLIN_MACHINE(VCU118,
                      "RISC-V Hobgoblin_v2 (VCU118) board",
                      4, vcu118_dram_memmap, V2, V2),
    HOBGOBLIN_MACHINE_NAMED(TYPE_HOBGOBLIN_CODASIP_PRIME_MACHINE, VCU118,
                            "Codasip Prime FPGA platform", 4,
                            vcu118_dram_memmap, V2, V2),
};

DEFINE_TYPES(hobgoblin_machines_typeinfo)
