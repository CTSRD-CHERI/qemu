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
def archiveBranches = ['qemu-cheri', 'dev']
if (!env.CHANGE_ID && archiveBranches.contains(env.BRANCH_NAME)) {
    GlobalVars.archiveArtifacts = true
    jobProperties.add(pipelineTriggers([triggers: [[$class: 'jenkins.triggers.ReverseBuildTrigger',
                                                    upstreamProjects: "BBL/cheri_purecap",
                                                    threshold: hudson.model.Result.SUCCESS]]]))
}

def paramsArray = []

// Add an OS selector for manual builds
def allConfigs = ["linux", "freebsd", "linux-debug"]
paramsArray.add(text(defaultValue: allConfigs.join('\n'),
        description: 'The configurations to build for (one per line)',
        name: 'Configs'))

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
def selectedConfigs = isManualBuild() && params.Configs != null ? params.Configs.split('\n') : allConfigs
echo("Selected Configs: ${selectedConfigs}")
selectedConfigs.each { config ->
    def os = config.split('-')[0]
    def nodeLabel = os
    if (os == 'linux') {
        // Build on the oldest supported Ubuntu version so the binaries also run there
        nodeLabel = "${nodeLabel}-baseline"
    }
    jobs[config] = { ->
        node(nodeLabel) {
            def extraQemuArgs = ''
            boolean isDebug = config.endsWith('-debug')
            if (isDebug) {
                extraQemuArgs = '--qemu/configure-options=--enable-rvfi-dii --qemu/build-type=Debug --qemu/use-asan'
            }
            if (os == "linux") {
               extraQemuArgs += ' --qemu/configure-options=--enable-perfetto-log-instr'
            }
            def qemuResult = cheribuildProject(target: 'qemu', cpu: 'native', skipArtifacts: true,
                    nodeLabel: null,
                    extraArgs: "--without-sdk --install-prefix=/usr $extraQemuArgs",
                    runTests: /* true */ false,
                    uniqueId: "qemu-build-${config}",
                    skipTarball: true,
                    afterBuild: { params ->
                        extraBuildSteps(params, os)
                        // Don't archive the Debug+ASAN artifacts
                        if (!isDebug) {
                            maybeArchiveArtifacts(params, os)
                        }
                    })

            // Run the generated Morello tests
            stage("Run Morello tests") {
                sh "qemu-${os}/bin/qemu-system-morello --version"
                dir('morello-generated-tests') {
                    cloneGitRepoWithReference(url: 'https://github.com/rems-project/morello-generated-tests.git',
                                              branch: 'main', credentialsId: 'ctsrd-jenkins-new-github-api-key')
                }
                sh """#!/usr/bin/env bash
set -xe
virtualenv -p python3 venv
source venv/bin/activate
pip install pytest-xdist
# The pytest.ini configuration ensures that pytest-xdist autodetects the number of CPUs used for running tests.
pytest qemu/tests/morello \
--morello-tests-dir=\$WORKSPACE/morello-generated-tests \
--qemu=\$WORKSPACE/qemu-${os}/bin/qemu-system-morello \
--junit-xml=\$WORKSPACE/morello-generated-tests-result.xml || echo "Some tests failed"
"""
                junit allowEmptyResults: false, keepLongStdio: true, testResults: 'morello-generated-tests-result.xml'
            }
            // Run the baremetal MIPS tests to check we didn't regress.
            if (os == 'linux') {
                cheribuildProject(target: 'cheritest-qemu', architecture: 'native',
                        customGitCheckoutDir: 'cheritest', scmOverride: gitRepoWithLocalReference(url: 'https://github.com/CTSRD-CHERI/cheritest.git'),
                        nodeLabel: null,
                        // Ensure that test failures don't prevent creation of the junit file
                        extraArgs: '--install-prefix=/ --cheritest-qemu/no-run-tests-with-build',
                        runTests: true,
                        // Set the status message on the QEMU repo not the cheritest one
                        gitHubStatusArgs: qemuResult.gitInfo,
                        sdkCompilerOnly: true, skipTarball: true,
                        uniqueId: "mips-baremetal-testsuite-${config}",
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
