def archiveQEMU(String target) {
	return {
		sh "rm -rf \$WORKSPACE/${target} && mv \$WORKSPACE/tarball/${target} \$WORKSPACE/${target}"
		archiveArtifacts allowEmptyArchive: false, artifacts: "${target}/bin/qemu-system-*", fingerprint: true, onlyIfSuccessful: true
	}
}
  
cheribuildProject(target: 'qemu', cpu: 'native', skipArtifacts: true,
      buildStage: "Build Linux", nodeLabel: 'linux',
      extraArgs: '--unified-sdk --without-sdk --install-prefix=/linux',
      skipTarball: true, afterBuild: archiveQEMU('linux'))

cheribuildProject(target: 'qemu', cpu: 'native', skipArtifacts: true,
      buildStage: "Build FreeBSD", nodeLabel: 'freebsd',
      extraArgs: '--unified-sdk --without-sdk --install-prefix=/freebsd',
      skipTarball: true, afterBuild: archiveQEMU('freebsd'))
