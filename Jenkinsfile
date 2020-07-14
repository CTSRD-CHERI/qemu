// import the cheribuildProject() step
@Library('ctsrd-jenkins-scripts') _

// Set the default job properties (work around properties() not being additive but replacing)
setDefaultJobProperties([rateLimitBuilds([count: 2, durationName: 'hour', userBoost: true]),
                         [$class: 'GithubProjectProperty', projectUrlStr: 'https://github.com/CTSRD-CHERI/qemu'],
                         copyArtifactPermission('*'), // Downstream jobs need QEMU
])

def archiveQEMU(String target) {
    return {
        sh "rm -rf \$WORKSPACE/qemu-${target} && mv \$WORKSPACE/tarball/usr \$WORKSPACE/qemu-${target}"
        // Add all the firmwares that are needed to boot CheriBSD
        // Note: The RISC-V BIOS is added in archiveBBL()
        def firmwareFiles = [
            "efi-pcnet.rom", "vgabios-cirrus.bin", // MIPS
            "bios-256k.bin", "efi-virtio.rom", "vgabios-stdvga.bin", // x86_64
            "edk2-aarch64-code.fd"  // AArch64
        ].collect { "qemu-${target}/share/qemu/$it" }.join(', ')
        archiveArtifacts allowEmptyArchive: false, artifacts: "qemu-${target}/bin/qemu-system-*, ${firmwareFiles}" fingerprint: true, onlyIfSuccessful: true
    }
}

def archiveBBL(List targets) {
    sh 'find \$WORKSPACE/tarball/'
    for (target in targets) {
        sh """
mkdir -p \$WORKSPACE/qemu-${target}/share/qemu/
cp \$WORKSPACE/tarball/share/qemu/bbl-riscv64cheri-virt-fw_jump.bin \$WORKSPACE/qemu-${target}/share/qemu/
"""
        archiveArtifacts allowEmptyArchive: false, artifacts: "qemu-${target}/share/qemu/bbl-riscv64cheri-virt-fw_jump.bin", fingerprint: true, onlyIfSuccessful: true
    }
}

// build on the oldest supported ubuntu version so the binaries also run there
node('linux-baseline') {
    bblRepo = gitRepoWithLocalReference(url: 'https://github.com/CTSRD-CHERI/riscv-pk.git')
    bblRepo["branches"] = [[name: '*/cheri_purecap']]
    cheribuildProject(target: 'bbl-baremetal-riscv64-purecap',
            customGitCheckoutDir: 'bbl', scmOverride: bblRepo,
            nodeLabel: null, buildStage: "Build BBL BIOS",
            extraArgs: '--install-prefix=/',
            sdkCompilerOnly: true, skipTarball: true,
            uniqueId: "bbl-build",
            afterBuild: { archiveBBL(['freebsd', 'linux']) })

    def qemuResult = cheribuildProject(target: 'qemu', cpu: 'native', skipArtifacts: true,
            nodeLabel: null, buildStage: "Build Linux",
            extraArgs: '--without-sdk --install-prefix=/usr',
            runTests: /* true */ false,
            uniqueId: "qemu-build-linux",
            skipTarball: true, afterBuild: archiveQEMU('linux'))

    // Run the baremetal MIPS tests to check we didn't regress
    cheribuildProject(target: 'cheritest-qemu', architecture: 'native',
            customGitCheckoutDir: 'cheritest', scmOverride: gitRepoWithLocalReference(url: 'https://github.com/CTSRD-CHERI/cheritest.git'),
            nodeLabel: null, buildStage: "Run CHERI-MIPS tests",
            // Ensure we test failures don't prevent creation of the junit file
            extraArgs: '--install-prefix=/ --cheritest-qemu/no-run-tests-with-build',
            runTests: true,
            // Set the status message on the QEMU repo not the cheritest one
            gitHubStatusArgs: qemuResult.gitInfo,
            sdkCompilerOnly: true, skipTarball: true,
            uniqueId: 'mips-baremetal-testsuite',
            junitXmlFiles: 'cheritest/nosetests_qemu*.xml')
}

cheribuildProject(target: 'qemu', cpu: 'native', skipArtifacts: true,
        buildStage: "Build FreeBSD", nodeLabel: 'freebsd',
        extraArgs: '--without-sdk --install-prefix=/usr',
        runTests: /* true */ false,
        uniqueId: "qemu-build-freebsd",
        skipTarball: true, afterBuild: archiveQEMU('freebsd'))
