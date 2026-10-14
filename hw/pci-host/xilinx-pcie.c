/*
 * Xilinx PCIe host controller emulation.
 *
 * Copyright (c) 2016 Imagination Technologies
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, see <http://www.gnu.org/licenses/>.
 */

#include "qemu/osdep.h"
#include "qemu/log.h"
#include "qemu/module.h"
#include "qemu/units.h"
#include "qapi/error.h"
#include "hw/pci/msi.h"
#include "hw/pci/pci_bridge.h"
#include "hw/pci/pci_host.h"
#include "hw/pci/pcie_port.h"
#include "hw/qdev-properties.h"
#include "hw/irq.h"
#include "hw/pci-host/xilinx-pcie.h"

enum root_cfg_reg {
    /* Interrupt Decode Register */
    ROOTCFG_INTDEC              = 0x138,

    /* Interrupt Mask Register */
    ROOTCFG_INTMASK             = 0x13c,
    /* INTx Interrupt Received */
#define ROOTCFG_INTMASK_INTX    (1 << 16)
    /* MSI Interrupt Received */
#define ROOTCFG_INTMASK_MSI     (1 << 17)

    /* PHY Status/Control Register */
    ROOTCFG_PSCR                = 0x144,
    /* Link Up */
#define ROOTCFG_PSCR_LINK_UP    (1 << 11)

    /* Root Port Status/Control Register */
    ROOTCFG_RPSCR               = 0x148,
    /* Bridge Enable */
#define ROOTCFG_RPSCR_BRIDGEEN  (1 << 0)
    /* Interrupt FIFO Not Empty */
#define ROOTCFG_RPSCR_INTNEMPTY (1 << 18)
    /* Interrupt FIFO Overflow */
#define ROOTCFG_RPSCR_INTOVF    (1 << 19)

    /* Root Port MSI Base Register 1 */
    ROOTCFG_RPMSIB1             = 0x14c,
    /* Root Port MSI Base Register 2 */
    ROOTCFG_RPMSIB2             = 0x150,

    /* Root Port Interrupt FIFO Read Register 1 */
    ROOTCFG_RPIFR1              = 0x158,
#define ROOTCFG_RPIFR1_INT_LANE_SHIFT   27
#define ROOTCFG_RPIFR1_INT_ASSERT_SHIFT 29
#define ROOTCFG_RPIFR1_INT_VALID_SHIFT  31
    /* Root Port Interrupt FIFO Read Register 2 */
    ROOTCFG_RPIFR2              = 0x15c,

    /* Root Port Interrupt Decode 2 Register */
    ROOTCFG_RPID2               = 0x160,
    /* Root Port Interrupt Decode 2 Register */
    ROOTCFG_RPID2M              = 0x164,

    /* Root Port MSI Interrupt Decode 1 Register */
    ROOTCFG_RPMSID1             = 0x170,
    /* Root Port MSI Interrupt Decode 2 Register */
    ROOTCFG_RPMSID2             = 0x174,
    /* Root Port MSI Interrupt Decode 1 Mask Register */
    ROOTCFG_RPMSID1M            = 0x178,
    /* Root Port MSI Interrupt Decode 2 Mask Register */
    ROOTCFG_RPMSID2M            = 0x17c,
};

static void xilinx_pcie_update_decode_msi_intr(XilinxPCIEHost *s)
{
    uint64_t intr = s->msi_intr_decode & s->msi_intr_decode_mask;
    int level;

    if (s->intr_fifo_mode)
        return;

    level = !!(intr & 0xffffffff);
    qemu_set_irq(s->irq_msi[0], level);
    level = !!(intr >> 32);
    qemu_set_irq(s->irq_msi[1], level);
}

static uint64_t xilinx_pcie_root_msi_read(void *opaque, hwaddr addr,
                                          unsigned size)
{
    /*
     * Attempts to read from the MSI address are undefined in the PCI
     * specifications. Similarly, the datasheet doesn't specify the
     * behaviour. Since well-behaved guests won't ever ask a PCI
     * device to DMA from this address we just log the missing
     * functionality.
     */
    qemu_log_mask(LOG_UNIMP, "%s not implemented\n", __func__);
    return 0;
}

static void xilinx_pcie_root_msi_write(void *opaque, hwaddr addr,
                                       uint64_t val, unsigned len)
{
    XilinxPCIEHost *s = XILINX_PCIE_HOST(opaque);

    if (s->intr_fifo_mode) {
        qemu_log_mask(LOG_UNIMP, "%s not implemented\n", __func__);
        return;
    }

    s->msi_intr_decode |= BIT_ULL(val);
    xilinx_pcie_update_decode_msi_intr(s);
}

static const MemoryRegionOps xilinx_pcie_host_msi_ops = {
    .read = xilinx_pcie_root_msi_read,
    .write = xilinx_pcie_root_msi_write,
    .endianness = DEVICE_LITTLE_ENDIAN,
    .valid = {
        .min_access_size = 4,
        .max_access_size = 4,
    },
};

static void xilinx_pcie_root_update_msi_mapping(XilinxPCIEHost *s)
{
    memory_region_set_address(&s->msi_iomem, s->msi_base);
}

static void xilinx_pcie_update_intr(XilinxPCIEHost *s,
                                    uint32_t set, uint32_t clear)
{
    int level;

    s->intr |= set;
    s->intr &= ~clear;

    if (s->intr_fifo_mode) {
        if (s->intr_fifo_r != s->intr_fifo_w) {
            s->intr |= ROOTCFG_INTMASK_INTX;
        }
        /* MSI interrupts should be handled here */
    } else {
        if (s->intr_decode & s->intr_decode_mask)
            s->intr |= ROOTCFG_INTMASK_INTX;
    }

    level = !!(s->intr & s->intr_mask);
    qemu_set_irq(s->irq, level);
}

static void xilinx_pcie_queue_intr(XilinxPCIEHost *s,
                                   uint32_t fifo_reg1, uint32_t fifo_reg2)
{
    XilinxPCIEInt *intr;
    unsigned int new_w;

    new_w = (s->intr_fifo_w + 1) % ARRAY_SIZE(s->intr_fifo);
    if (new_w == s->intr_fifo_r) {
        s->rpscr |= ROOTCFG_RPSCR_INTOVF;
        return;
    }

    intr = &s->intr_fifo[s->intr_fifo_w];
    s->intr_fifo_w = new_w;

    intr->fifo_reg1 = fifo_reg1;
    intr->fifo_reg2 = fifo_reg2;

    xilinx_pcie_update_intr(s, ROOTCFG_INTMASK_INTX, 0);
}

static void xilinx_pcie_decode_intr(XilinxPCIEHost *s,
                                   uint32_t irq, int level)
{
    assert(irq <= 3);

    if (level)
        s->intr_decode |= 1 << (irq + 16);
    else
        s->intr_decode &= ~(1 << (irq + 16));

    xilinx_pcie_update_intr(s, 0, 0);
}

static void xilinx_pcie_set_irq(void *opaque, int irq_num, int level)
{
    XilinxPCIEHost *s = XILINX_PCIE_HOST(opaque);

    if (s->intr_fifo_mode)
        xilinx_pcie_queue_intr(s,
           (irq_num << ROOTCFG_RPIFR1_INT_LANE_SHIFT) |
               (level << ROOTCFG_RPIFR1_INT_ASSERT_SHIFT) |
               (1 << ROOTCFG_RPIFR1_INT_VALID_SHIFT),
           0);
    else
        xilinx_pcie_decode_intr(s, irq_num, level);
}

static AddressSpace *xilinx_pcie_host_set_iommu(PCIBus *bus, void *opaque,
                                                int devfn)
{
    XilinxPCIEHost *s = XILINX_PCIE_HOST(opaque);

    return &s->address_space;
}

static void xilinx_pcie_host_realize(DeviceState *dev, Error **errp)
{
    PCIHostState *pci = PCI_HOST_BRIDGE(dev);
    XilinxPCIEHost *s = XILINX_PCIE_HOST(dev);
    SysBusDevice *sbd = SYS_BUS_DEVICE(dev);
    PCIExpressHost *pex = PCIE_HOST_BRIDGE(dev);

    snprintf(s->name, sizeof(s->name), "pcie%u", s->bus_nr);

    /* PCI configuration space */
    pcie_host_mmcfg_init(pex, s->cfg_size);
    sysbus_init_mmio(sbd, &pex->mmio);

    /* MMIO region */
    memory_region_init(&s->mmio, OBJECT(s), "mmio", UINT64_MAX);
    sysbus_init_mmio(sbd, &s->mmio);

    //memory_region_set_enabled(&s->mmio, false);

    /* dummy PCI I/O region (not visible to the CPU) */
    memory_region_init(&s->io, OBJECT(s), "io", 16);

    /* interrupt out */
    qdev_init_gpio_out_named(dev, &s->irq, "interrupt_out", 1);

    /* Decoded MSI interrupts */
    if (!s->intr_fifo_mode) {
        /* interrupt_out_msi_vec0to31 and interrupt_out_msi_vec32to63 */
        qdev_init_gpio_out_named(dev, s->irq_msi, "interrupt_out_msi", 2);
    }

#if 0
    sysbus_init_mmio(sbd, &s->pref_mmio);
#endif

    pci->bus = pci_register_root_bus(dev, s->name, xilinx_pcie_set_irq,
                                     pci_swizzle_map_irq_fn, s, &s->mmio,
                                     &s->io, 0, 4, TYPE_PCIE_BUS);

    {
        g_autofree char *as_root_name =
            g_strdup_printf("%s-bus-address-space-root", s->name);
        g_autofree char *as_name =
            g_strdup_printf("%s-bus-address-space", s->name);

        memory_region_init(&s->address_space_root, OBJECT(s),
                           as_root_name, UINT64_MAX);
        memory_region_add_subregion(&s->address_space_root,
                                    0x0, &s->mmio);
        address_space_init(&s->address_space,
                           &s->address_space_root,
                           as_name);
    }
    pci_setup_iommu(pci->bus, xilinx_pcie_host_set_iommu, s);

    qdev_realize(DEVICE(&s->root), BUS(pci->bus), &error_fatal);
}

static const char *xilinx_pcie_host_root_bus_path(PCIHostState *host_bridge,
                                                  PCIBus *rootbus)
{
    return "0000:00";
}

static void xilinx_pcie_host_init(Object *obj)
{
    XilinxPCIEHost *s = XILINX_PCIE_HOST(obj);
    XilinxPCIERoot *root = &s->root;

    object_initialize_child(obj, "root", root, TYPE_XILINX_PCIE_ROOT);
    qdev_prop_set_int32(DEVICE(root), "addr", PCI_DEVFN(0, 0));
    qdev_prop_set_bit(DEVICE(root), "multifunction", false);
}

static Property xilinx_pcie_host_props[] = {
    DEFINE_PROP_UINT32("bus_nr", XilinxPCIEHost, bus_nr, 0),
    DEFINE_PROP_SIZE("cfg_base", XilinxPCIEHost, cfg_base, 0),
    DEFINE_PROP_SIZE("cfg_size", XilinxPCIEHost, cfg_size, 32 * MiB),
    DEFINE_PROP_SIZE("mmio_base", XilinxPCIEHost, mmio_base[0], 0),
    DEFINE_PROP_SIZE("mmio_size", XilinxPCIEHost, mmio_size[0], 1 * MiB),
    DEFINE_PROP_SIZE("mmio1_base", XilinxPCIEHost, mmio_base[1], 0),
    DEFINE_PROP_SIZE("mmio1_size", XilinxPCIEHost, mmio_size[1], 0),
    DEFINE_PROP_BOOL("link_up", XilinxPCIEHost, link_up, true),
    DEFINE_PROP_BOOL("intr_fifo_mode", XilinxPCIEHost, intr_fifo_mode, true),
    DEFINE_PROP_END_OF_LIST(),
};

static void xilinx_pcie_host_class_init(ObjectClass *klass, void *data)
{
    DeviceClass *dc = DEVICE_CLASS(klass);
    PCIHostBridgeClass *hc = PCI_HOST_BRIDGE_CLASS(klass);

    hc->root_bus_path = xilinx_pcie_host_root_bus_path;
    dc->realize = xilinx_pcie_host_realize;
    set_bit(DEVICE_CATEGORY_BRIDGE, dc->categories);
    dc->fw_name = "pci";
    device_class_set_props(dc, xilinx_pcie_host_props);
}

static const TypeInfo xilinx_pcie_host_info = {
    .name       = TYPE_XILINX_PCIE_HOST,
    .parent     = TYPE_PCIE_HOST_BRIDGE,
    .instance_size = sizeof(XilinxPCIEHost),
    .instance_init = xilinx_pcie_host_init,
    .class_init = xilinx_pcie_host_class_init,
};

static uint32_t xilinx_pcie_root_config_read(PCIDevice *d,
                                             uint32_t address, int len)
{
    XilinxPCIEHost *s = XILINX_PCIE_HOST(OBJECT(d)->parent);
    uint32_t val;

    switch (address) {
    case ROOTCFG_INTDEC:
        val = s->intr;
        break;
    case ROOTCFG_INTMASK:
        val = s->intr_mask;
        break;
    case ROOTCFG_PSCR:
        val = s->link_up ? ROOTCFG_PSCR_LINK_UP : 0;
        break;
    case ROOTCFG_RPSCR:
        if (s->intr_fifo_r != s->intr_fifo_w) {
            s->rpscr &= ~ROOTCFG_RPSCR_INTNEMPTY;
        } else {
            s->rpscr |= ROOTCFG_RPSCR_INTNEMPTY;
        }
        val = s->rpscr;
        break;
    case ROOTCFG_RPMSIB1:
        val = s->msi_base >> 32;
        break;
    case ROOTCFG_RPMSIB2:
        val = s->msi_base & 0xfffffffful;
        break;
    case ROOTCFG_RPIFR1:
        if (s->intr_fifo_w == s->intr_fifo_r) {
            /* FIFO empty */
            val = 0;
        } else {
            val = s->intr_fifo[s->intr_fifo_r].fifo_reg1;
        }
        break;
    case ROOTCFG_RPIFR2:
        if (s->intr_fifo_w == s->intr_fifo_r) {
            /* FIFO empty */
            val = 0;
        } else {
            val = s->intr_fifo[s->intr_fifo_r].fifo_reg2;
        }
        break;
    case ROOTCFG_RPID2:
        val = s->intr_decode;
        break;
    case ROOTCFG_RPID2M:
        val = s->intr_decode_mask;
        break;
    case ROOTCFG_RPMSID1:
        val = s->msi_intr_decode & 0xffffffff;
        break;
    case ROOTCFG_RPMSID2:
        val = s->msi_intr_decode >> 32;
        break;
    case ROOTCFG_RPMSID1M:
        val = s->msi_intr_decode_mask & 0xffffffff;
        break;
    case ROOTCFG_RPMSID2M:
        val = s->msi_intr_decode_mask >> 32;
        break;
    default:
        val = pci_default_read_config(d, address, len);
        break;
    }
    return val;
}

static void xilinx_pcie_root_config_write(PCIDevice *d, uint32_t address,
                                          uint32_t val, int len)
{
    XilinxPCIEHost *s = XILINX_PCIE_HOST(OBJECT(d)->parent);
    switch (address) {
    case ROOTCFG_INTDEC:
        xilinx_pcie_update_intr(s, 0, val);
        break;
    case ROOTCFG_INTMASK:
        s->intr_mask = val;
        xilinx_pcie_update_intr(s, 0, 0);
        break;
    case ROOTCFG_RPSCR:
        s->rpscr &= ~ROOTCFG_RPSCR_BRIDGEEN;
        s->rpscr |= val & ROOTCFG_RPSCR_BRIDGEEN;
        memory_region_set_enabled(&s->mmio, val & ROOTCFG_RPSCR_BRIDGEEN);

        if (val & ROOTCFG_INTMASK_INTX) {
            s->rpscr &= ~ROOTCFG_INTMASK_INTX;
        }
        break;
    case ROOTCFG_RPMSIB1:
        s->msi_base &= 0x00000000ffffffffull;
        s->msi_base |= (uint64_t)val << 32;
        xilinx_pcie_root_update_msi_mapping(s);
        break;
    case ROOTCFG_RPMSIB2:
        s->msi_base &= 0xffffffff00000000ull;
        s->msi_base |= (val & ~0xfff);
        xilinx_pcie_root_update_msi_mapping(s);
        break;
    case ROOTCFG_RPIFR1:
    case ROOTCFG_RPIFR2:
        if (s->intr_fifo_w == s->intr_fifo_r) {
            /* FIFO empty */
            return;
        } else {
            s->intr_fifo_r = (s->intr_fifo_r + 1) % ARRAY_SIZE(s->intr_fifo);
        }
        break;
    case ROOTCFG_RPID2:
        break;
    case ROOTCFG_RPID2M:
        s->intr_decode_mask = val;
        xilinx_pcie_update_intr(s, 0, 0);
        break;
    case ROOTCFG_RPMSID1:
        s->msi_intr_decode &= ~((uint64_t)val);
        xilinx_pcie_update_decode_msi_intr(s);
        break;
    case ROOTCFG_RPMSID2:
        s->msi_intr_decode &= ~((uint64_t)val << 32);
        xilinx_pcie_update_decode_msi_intr(s);
        break;
    case ROOTCFG_RPMSID1M:
        s->msi_intr_decode_mask &= 0xffffffff00000000ull;
        s->msi_intr_decode_mask |= val;
        xilinx_pcie_update_decode_msi_intr(s);
        break;
    case ROOTCFG_RPMSID2M:
        s->msi_intr_decode_mask &= 0xffffffffull;
        s->msi_intr_decode_mask |= (uint64_t)val << 32;
        xilinx_pcie_update_decode_msi_intr(s);
        break;
    default:
#if 0
        pci_default_write_config(d, address, val, len);
#else
        pci_bridge_write_config(d, address, val, len);
#endif
        break;
    }
}

static void xilinx_pcie_root_realize(PCIDevice *pci_dev, Error **errp)
{
    BusState *bus = qdev_get_parent_bus(DEVICE(pci_dev));
    XilinxPCIEHost *s = XILINX_PCIE_HOST(bus->parent);
    PCIBridge *br = PCI_BRIDGE(pci_dev);

#if 0
    pci_set_word(pci_dev->config + PCI_COMMAND,
                 PCI_COMMAND_MEMORY | PCI_COMMAND_MASTER);
    pci_set_word(pci_dev->config + PCI_MEMORY_BASE, s->mmio_base[0] >> 16);
    pci_set_word(pci_dev->config + PCI_MEMORY_LIMIT,
                 ((s->mmio_base[0] + s->mmio_size[0] - 1) >> 16) & 0xfff0);
#endif

    br->bus_name  = "xilinx-pcie";
    pci_bridge_initfn(pci_dev, TYPE_PCIE_BUS);

    pcie_port_init_reg(pci_dev);

    if (pcie_cap_init(pci_dev, 0x70, PCI_EXP_TYPE_ROOT_PORT,
                      0, &error_fatal) < 0) {
        error_setg(errp, "Failed to initialize PCIe capability");
    }

    msi_nonbroken = true;
    msi_init(pci_dev, 0x48, 1, true, false, &error_fatal);

    /*
     * Configure MemoryRegion implementing PCI -> CPU memory
     * access.
     */
    memory_region_init_alias(&s->inbound_alias, OBJECT(s), "inbound alias",
                             get_system_memory(), 0, UINT64_MAX);
    memory_region_add_subregion_overlap(&s->address_space_root, 0,
                                        &s->inbound_alias, -1);

    /*
     * Configure MemoryRegion implementing CPU -> PCI memory
     * access
     */
    memory_region_init_alias(&s->outbound_alias[0], OBJECT(s),
                             "outbound_alias0",
                             &s->mmio, s->mmio_base[0], s->mmio_size[0]);
    memory_region_add_subregion(get_system_memory(), s->mmio_base[0],
                                &s->outbound_alias[0]);

    if (s->mmio_size[1]) {
        memory_region_init_alias(&s->outbound_alias[1], OBJECT(s),
                                 "outbound_alias1",
                                 &s->mmio, s->mmio_base[1], s->mmio_size[1]);
        memory_region_add_subregion(get_system_memory(), s->mmio_base[1],
                                    &s->outbound_alias[1]);
    }

    /* MSI block */
    memory_region_init_io(&s->msi_iomem, OBJECT(s),
                          &xilinx_pcie_host_msi_ops,
                          s, "pcie-msi", 0x1000);
    memory_region_add_subregion(&s->mmio, 0, &s->msi_iomem);
}

static void xilinx_pcie_root_class_init(ObjectClass *klass, void *data)
{
    PCIDeviceClass *k = PCI_DEVICE_CLASS(klass);
    DeviceClass *dc = DEVICE_CLASS(klass);

    set_bit(DEVICE_CATEGORY_BRIDGE, dc->categories);
    dc->desc = "Xilinx AXI-PCIe Host Bridge";
    k->vendor_id = PCI_VENDOR_ID_XILINX;
    k->device_id = 0x9124;
    k->revision = 0;
    k->class_id = PCI_CLASS_BRIDGE_HOST; // DW is PCI_CLASS_BRIDGE_PCI
    k->is_bridge = true;
    k->realize = xilinx_pcie_root_realize;
    k->exit = pci_bridge_exitfn;
    dc->reset = pci_bridge_reset;
    k->config_read = xilinx_pcie_root_config_read;
    k->config_write = xilinx_pcie_root_config_write;
    /*
     * PCI-facing part of the host bridge, not usable without the
     * host-facing part, which can't be device_add'ed, yet.
     */
    dc->user_creatable = false;
}

static const TypeInfo xilinx_pcie_root_info = {
    .name = TYPE_XILINX_PCIE_ROOT,
    .parent = TYPE_PCI_BRIDGE,
    .instance_size = sizeof(XilinxPCIERoot),
    .class_init = xilinx_pcie_root_class_init,
    .interfaces = (InterfaceInfo[]) {
        { INTERFACE_PCIE_DEVICE },
        { }
    },
};

static void xilinx_pcie_register(void)
{
    type_register_static(&xilinx_pcie_root_info);
    type_register_static(&xilinx_pcie_host_info);
}

type_init(xilinx_pcie_register)
