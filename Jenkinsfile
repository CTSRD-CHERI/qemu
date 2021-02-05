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

def paramsArray = []

// Add an OS selector for manual builds
def allOSes = ["linux", "freebsd"]
paramsArray.add(text(defaultValue: allOSes.join('\n'),
        description: 'The operating systems to build for (one per line)',
        name: 'OSes'))

jobProperties.add(parameters(paramsArray))

// Set the default job properties (work around properties() not being additive but replacing)
setDefaultJobProperties(jobProperties)

jobs = [:]

def archiveQEMU(String os) {
    return {
        stage("Archiving artifacts") {
            sh "rm -rf \$WORKSPACE/qemu-${os} && mv \$WORKSPACE/tarball/usr \$WORKSPACE/qemu-${os}"
            // Copy BBL binary for embedding
            copyArtifacts projectName: "BBL/cheri_purecap", filter: "bbl-riscv64cheri-virt-fw_jump.bin", target: "qemu-${os}/share/qemu", fingerprintArtifacts: true
            // Add all the firmwares that are needed to boot CheriBSD
            def firmwareFiles = [
                "efi-pcnet.rom", "vgabios-cirrus.bin", // MIPS
                "bbl-riscv64cheri-virt-fw_jump.bin", // RISC-V
                "bios-256k.bin", "efi-virtio.rom", "vgabios-stdvga.bin", // x86_64
                "edk2-aarch64-code.fd"  // AArch64
            ].collect { "qemu-${os}/share/qemu/$it" }.join(', ')
            archiveArtifacts allowEmptyArchive: false, artifacts: "qemu-${os}/bin/qemu-system-*, ${firmwareFiles}", fingerprint: true, onlyIfSuccessful: true
        }
    }
}

// Work around for https://issues.jenkins.io/browse/JENKINS-46941
// Jenkins appears to use the last selected manual override for automatically triggered builds.
// Therefore, only read the parameter value for manually-triggered builds.
def selectedOSes = isManualBuild() && params.OSes != null ? params.OSes.split('\n') : allOSes
echo("Selected OSes: ${selectedOSes}")
selectedOSes.each { os ->
    def nodeLabel = os
    if (os == "linux") {
        // Build on the oldest supported Ubuntu version so the binaries also run there
        nodeLabel = "${nodeLabel}-baseline"
    }
    jobs[os] = { ->
        node(nodeLabel) {
            def qemuResult = cheribuildProject(target: 'qemu', cpu: 'native', skipArtifacts: true,
                    nodeLabel: null,
                    extraArgs: '--without-sdk --install-prefix=/usr',
                    runTests: /* true */ false,
                    uniqueId: "qemu-build-${os}",
                    skipTarball: true,
                    afterBuild: archiveQEMU(os))

            // Run the baremetal MIPS tests to check we didn't regress.
            if (os == "linux") {
                cheribuildProject(target: 'cheritest-qemu', architecture: 'native',
                        customGitCheckoutDir: 'cheritest', scmOverride: gitRepoWithLocalReference(url: 'https://github.com/CTSRD-CHERI/cheritest.git'),
                        nodeLabel: null,
                        // Ensure we test failures don't prevent creation of the junit file
                        extraArgs: '--install-prefix=/ --cheritest-qemu/no-run-tests-with-build',
                        runTests: true,
                        // Set the status message on the QEMU repo not the cheritest one
                        gitHubStatusArgs: qemuResult.gitInfo,
                        sdkCompilerOnly: true, skipTarball: true,
                        uniqueId: 'mips-baremetal-testsuite',
                        junitXmlFiles: 'cheritest/nosetests_qemu*.xml')
            }
        }
    }
}

boolean runParallel = true
echo("Running jobs in parallel: ${runParallel}")
if (runParallel) {
    jobs.failFast = false
    parallel jobs
} else {
    jobs.each { key, value ->
        echo("RUNNING ${key}")
        value()
    }
}
