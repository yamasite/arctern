timeout(time: 20, unit: 'MINUTES') {
    dir ("docker/spark/${BINARY_VERSION}") {
        def channelPackage = "conda-bld.tar.gz"
        def downloadStatus = sh(returnStatus: true, script: "curl -C - -o arctern/${channelPackage} ${ARTFACTORY_URL}/${channelPackage}")

        if (downloadStatus != 0) {
            error("\" Download \" ${ARTFACTORY_URL}/${channelPackage} \" failed!")
        }

        sh "tar zxvf arctern/${channelPackage} -C ./arctern"

        def changeTargetImageName = ""
        if (CHANGE_TARGET) {
            changeTargetImageName = "${params.DOKCER_REGISTRY_URL}/${ARCTERN_REPO}:${CHANGE_TARGET}-${OS_NAME}-${LOWER_BUILD_TYPE}"
            sh "docker pull ${changeTargetImageName} || exit 0"
        }

        def imageName = "${ARCTERN_REPO}:${ARCTERN_TAG}"

        try {
            deleteImages("${imageName}", true)
            sh "docker build -t ${imageName} ."
            def customImage = docker.image("${imageName}")
            deleteImages("${params.DOKCER_REGISTRY_URL}/${imageName}", true)
            docker.withRegistry("https://${params.DOKCER_REGISTRY_URL}", "${params.DOCKER_CREDENTIALS_ID}") {
                customImage.push()
            }
        } catch (exc) {
            throw exc
        } finally {
            deleteImages("${imageName}", true)
            deleteImages("${params.DOKCER_REGISTRY_URL}/${imageName}", true)
            if (CHANGE_TARGET) {
                sh "docker rmi -f ${changeTargetImageName} || exit 0"
            }
        }
    }
}

boolean deleteImages(String imageName, boolean force) {
    def imageNameStr = imageName.trim()
    def isExistImage = sh(returnStatus: true, script: "docker inspect --type=image ${imageNameStr} 2>&1 > /dev/null")
    if (isExistImage == 0) {
        def deleteImageStatus = 0
        if (force) {
            def imageID = sh(returnStdout: true, script: "docker inspect --type=image --format \"{{.ID}}\" ${imageNameStr}")
            deleteImageStatus = sh(returnStatus: true, script: "docker rmi -f ${imageID}")
        } else {
            deleteImageStatus = sh(returnStatus: true, script: "docker rmi ${imageNameStr}")
        }

        if (deleteImageStatus != 0) {
            return false
        }
    }
    return true
}
