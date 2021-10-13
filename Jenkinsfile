// import the cheribuildProject() step
@Library('ctsrd-jenkins-scripts') _

class GlobalVars { // "Groovy"
    public static boolean archiveArtifacts = false
}

def jobProperties = [
    rateLimitBuilds(throttle: [count: 2, durationName: 'hour', userBoost: true]),
    [$class: 'GithubProjectProperty', projectUrlStr: 'https://github.com/CTSRD-CHERI/qemu'],
    copyArtifactPermission('*'), // Downstream jobs need QEMU
]

// Don't archive binaries for pull requests and non-default branches:
def archiveBranches = ['qemu-cheri', 'dev', 'qemu-morello-merged']
if (!env.CHANGE_ID && archiveBranches.contains(env.BRANCH_NAME)) {
    GlobalVars.archiveArtifacts = true
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

def extraBuildSteps(params, String os) {
    sh "rm -rf \$WORKSPACE/qemu-${os} && mv \$WORKSPACE/tarball/usr \$WORKSPACE/qemu-${os}"
    // Embed BBL binary (currently only needed when archiving, but could be used for build-time testing in future).
    copyArtifacts projectName: "BBL/cheri_purecap", filter: "bbl-riscv64cheri-virt-fw_jump.bin", target: "qemu-${os}/share/qemu", fingerprintArtifacts: true
}

def maybeArchiveArtifacts(params, String os) {
    if (GlobalVars.archiveArtifacts) {
        stage("Archiving artifacts") {
            // Add all the firmwares that are needed to boot CheriBSD
            def firmwareFiles = [
                "efi-pcnet.rom", "vgabios-cirrus.bin", // MIPS
                "bbl-riscv64cheri-virt-fw_jump.bin", // RISC-V
                "bios-256k.bin", "efi-virtio.rom", "vgabios-stdvga.bin", // x86_64
                "edk2-aarch64-code.fd"  // AArch64
            ].collect { "qemu-${os}/share/qemu/$it" }.join(', ')
            archiveArtifacts allowEmptyArchive: false, artifacts: "qemu-${os}/bin/qemu-system-*, qemu-${os}/bin/qemu-img, ${firmwareFiles}", fingerprint: true, onlyIfSuccessful: true
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
                    extraArgs: '--without-sdk --install-prefix=/usr --qemu/targets morello-softmmu,aarch64-softmmu',
                    runTests: /* true */ false,
                    uniqueId: "qemu-build-${os}",
                    skipTarball: true,
                    afterBuild: { params ->
                        extraBuildSteps(params, os)
                        maybeArchiveArtifacts(params, os)
                    })
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
