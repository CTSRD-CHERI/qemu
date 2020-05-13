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
  
cheribuildProject(target: 'qemu', cpu: 'native', skipArtifacts: true,
      buildStage: "Build Linux",
      nodeLabel: 'xenial', // build on the oldest supported ubuntu version so the binaries also run there
      extraArgs: '--without-sdk --install-prefix=/usr',
      skipTarball: true, afterBuild: archiveQEMU('linux'))

cheribuildProject(target: 'qemu', cpu: 'native', skipArtifacts: true,
      buildStage: "Build FreeBSD", nodeLabel: 'freebsd',
      extraArgs: '--without-sdk --install-prefix=/usr',
      skipTarball: true, afterBuild: archiveQEMU('freebsd'))
