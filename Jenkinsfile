// import the cheribuildProject() step
@Library('ctsrd-jenkins-scripts') _

def jobProperties = [
    rateLimitBuilds(throttle: [count: 2, durationName: 'hour', userBoost: true]),
    [$class: 'GithubProjectProperty', projectUrlStr: 'https://github.com/CTSRD-CHERI/qemu'],
    copyArtifactPermission('*'), // Downstream jobs need QEMU
]

// Don't trigger for pull requests and non-default branches:
def triggerBranches = ['qemu-cheri', 'dev']
if (!env.CHANGE_ID && triggerBranches.contains(env.BRANCH_NAME)) {
    jobProperties.add(pipelineTriggers([triggers: [[$class: 'jenkins.triggers.ReverseBuildTrigger',
                                                    upstreamProjects: "BBL/cheri_purecap",
                                                    threshold: hudson.model.Result.SUCCESS]]]))
}
// Set the default job properties (work around properties() not being additive but replacing)
setDefaultJobProperties(jobProperties)

def archiveQEMU(String target) {
    return {
        stage("Archiving artifacts") {
            sh "rm -rf \$WORKSPACE/qemu-${target} && mv \$WORKSPACE/tarball/usr \$WORKSPACE/qemu-${target}"
            // Copy BBL binary for embedding
            copyArtifacts projectName: "BBL/cheri_purecap", filter: "bbl-riscv64cheri-virt-fw_jump.bin", target: "qemu-${target}/share/qemu", fingerprintArtifacts: true
            // Add all the firmwares that are needed to boot CheriBSD
            def firmwareFiles = [
                "efi-pcnet.rom", "vgabios-cirrus.bin", // MIPS
                "bbl-riscv64cheri-virt-fw_jump.bin", // RISC-V
                "bios-256k.bin", "efi-virtio.rom", "vgabios-stdvga.bin", // x86_64
                "edk2-aarch64-code.fd"  // AArch64
            ].collect { "qemu-${target}/share/qemu/$it" }.join(', ')
            archiveArtifacts allowEmptyArchive: false, artifacts: "qemu-${target}/bin/qemu-system-*, ${firmwareFiles}", fingerprint: true, onlyIfSuccessful: true
        }
    }
}

// build on the oldest supported ubuntu version so the binaries also run there
node('linux-baseline') {
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
