def archiveQEMU(String target) {
	return {
		sh "rm -rf \$WORKSPACE/${target} && mv \$WORKSPACE/tarball/${target} \$WORKSPACE/${target}"
		archiveArtifacts allowEmptyArchive: false, artifacts: "${target}/bin/qemu-system-*", fingerprint: true, onlyIfSuccessful: true
	}
}
  
node ('linux') {
  cheribuildProject(target: 'qemu', cpu: 'native', skipArtifacts: true,
      extraArgs: '--unified-sdk --without-sdk --install-prefix=/linux',
      nodeLabel: null, skipTarball: true, afterBuild: archiveQEMU('linux'))
}
node ('freebsd') {
  cheribuildProject(target: 'qemu', cpu: 'native', skipArtifacts: true,
      extraArgs: '--unified-sdk --without-sdk --install-prefix=/freebsd',
      nodeLabel: null, skipTarball: true, afterBuild: archiveQEMU('freebsd'))
}
