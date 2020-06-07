// import the cheribuildProject() step
@Library('ctsrd-jenkins-scripts') _

properties([
		disableConcurrentBuilds(),
		disableResume(),
		[$class: 'GithubProjectProperty', projectUrlStr: 'https://github.com/CTSRD-CHERI/qemu'],
		copyArtifactPermission('*'),
		durabilityHint('PERFORMANCE_OPTIMIZED'),
		pipelineTriggers([githubPush(), pollSCM('@daily')])
])

def archiveQEMU(String target) {
	return {
		sh "rm -rf \$WORKSPACE/qemu-${target} && mv \$WORKSPACE/tarball/usr \$WORKSPACE/qemu-${target}"
		archiveArtifacts allowEmptyArchive: false, artifacts: "qemu-${target}/bin/qemu-system-*, qemu-${target}/share/qemu/efi-pcnet.rom, qemu-${target}/share/qemu/vgabios-cirrus.bin", fingerprint: true, onlyIfSuccessful: true
	}
}

def archiveBBL(List targets) {
	sh 'find \$WORKSPACE/tarball/'
	for (target in targets) {
		sh """
mkdir -p $WORKSPACE/qemu-${target}/share/qemu/
cp \$WORKSPACE/tarball/share/qemu/bbl-riscv64cheri-virt-fw_jump.bin \$WORKSPACE/qemu-${target}/share/qemu/
"""
		archiveArtifacts allowEmptyArchive: false, artifacts: "qemu-${target}/share/qemu/bbl-riscv64cheri-virt-fw_jump.bin", fingerprint: true, onlyIfSuccessful: true
	}
}

bblRepo = gitRepoWithLocalReference(url: 'https://github.com/CTSRD-CHERI/riscv-pk.git')
// Build the without-sysroot branch since otherwise we'd have to build CheriBSD first
bblRepo["branches"] = [[name: '*/cheri_purecap']]
cheribuildProject(target: 'bbl-baremetal-riscv64-purecap',
		customGitCheckoutDir: 'bbl', scmOverride: bblRepo,
		buildStage: "Build BBL BIOS", nodeLabel: 'linux',
		extraArgs: '--install-prefix=/',
		sdkCompilerOnly: true, skipTarball: true,
		afterBuild: { archiveBBL(['freebsd', 'linux']) }
)

cheribuildProject(target: 'qemu', cpu: 'native', skipArtifacts: true,
		buildStage: "Build Linux",
		nodeLabel: 'xenial', // build on the oldest supported ubuntu version so the binaries also run there
		extraArgs: '--without-sdk --install-prefix=/usr',
		runTests: /* true */ false,
		skipTarball: true, afterBuild: archiveQEMU('linux'))

cheribuildProject(target: 'qemu', cpu: 'native', skipArtifacts: true,
		buildStage: "Build FreeBSD", nodeLabel: 'freebsd',
		extraArgs: '--without-sdk --install-prefix=/usr',
		runTests: /* true */ false,
		skipTarball: true, afterBuild: archiveQEMU('freebsd'))
