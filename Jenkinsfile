// import the cheribuildProject() step
library 'ctsrd-jenkins-scripts'

def archiveQEMU(String target) {
	return {
		sh "rm -rf \$WORKSPACE/qemu-${target} && mv \$WORKSPACE/tarball/usr \$WORKSPACE/qemu-${target}"
		archiveArtifacts allowEmptyArchive: false, artifacts: "qemu-${target}/bin/qemu-system-*", fingerprint: true, onlyIfSuccessful: true
	}
}
  
cheribuildProject(target: 'qemu', cpu: 'native', skipArtifacts: true,
      buildStage: "Build Linux", nodeLabel: 'linux',
      extraArgs: '--unified-sdk --without-sdk --install-prefix=/usr',
      skipTarball: true, afterBuild: archiveQEMU('linux'))

cheribuildProject(target: 'qemu', cpu: 'native', skipArtifacts: true,
      buildStage: "Build FreeBSD", nodeLabel: 'freebsd',
      extraArgs: '--unified-sdk --without-sdk --install-prefix=/usr',
      skipTarball: true, afterBuild: archiveQEMU('freebsd'))
