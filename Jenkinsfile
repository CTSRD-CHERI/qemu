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
mkdir -p \$WORKSPACE/qemu-${target}/share/qemu/
cp \$WORKSPACE/tarball/share/qemu/bbl-riscv64cheri-virt-fw_jump.bin \$WORKSPACE/qemu-${target}/share/qemu/
"""
		archiveArtifacts allowEmptyArchive: false, artifacts: "qemu-${target}/share/qemu/bbl-riscv64cheri-virt-fw_jump.bin", fingerprint: true, onlyIfSuccessful: true
	}
}

// build on the oldest supported ubuntu version so the binaries also run there
node('xenial') {
	bblRepo = gitRepoWithLocalReference(url: 'https://github.com/CTSRD-CHERI/riscv-pk.git')
	bblRepo["branches"] = [[name: '*/cheri_purecap']]
	cheribuildProject(target: 'bbl-baremetal-riscv64-purecap',
			customGitCheckoutDir: 'bbl', scmOverride: bblRepo,
			buildStage: "Build BBL BIOS",
			extraArgs: '--install-prefix=/',
			sdkCompilerOnly: true, skipTarball: true,
			afterBuild: { archiveBBL(['freebsd', 'linux']) }
	)

	def qemuResult = cheribuildProject(target: 'qemu', cpu: 'native', skipArtifacts: true,
			buildStage: "Build Linux",
			extraArgs: '--without-sdk --install-prefix=/usr',
			runTests: /* true */ false,
			skipTarball: true, afterBuild: archiveQEMU('linux')
	)

	// Run the baremetal MIPS tests to check we didn't regress
	cheribuildProject(target: 'cheritest-qemu', architecture: 'native',
			customGitCheckoutDir: 'cheritest', scmOverride: gitRepoWithLocalReference(url: 'https://github.com/CTSRD-CHERI/cheritest.git'),
			buildStage: "Run CHERI-MIPS tests",
			extraArgs: '--install-prefix=/',
			// Set the status message on the QEMU repo not the cheritest one
			gitHubStatusArgs: qemuResult.gitInfo,
			sdkCompilerOnly: true, skipTarball: true,
	)

	def summary = junit allowEmptyResults: false, keepLongStdio: true, testResults: 'cheritest/nosetests_qemu*.xml'
	echo("cheritest test summary: ${summary.totalCount}, Failures: ${summary.failCount}, Skipped: ${summary.skipCount}, Passed: ${summary.passCount}")
	if (summary.passCount == 0 || summary.totalCount == 0) {
		error("No tests successful?")
	}
}

cheribuildProject(target: 'qemu', cpu: 'native', skipArtifacts: true,
		buildStage: "Build FreeBSD", nodeLabel: 'freebsd',
		extraArgs: '--without-sdk --install-prefix=/usr',
		runTests: /* true */ false,
		skipTarball: true, afterBuild: archiveQEMU('freebsd'))
