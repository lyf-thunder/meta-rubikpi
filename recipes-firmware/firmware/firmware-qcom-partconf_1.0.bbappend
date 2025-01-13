do_deploy:append() {
    install -m 0644 ${WORKDIR}/QCM6490_bootbinaries/*.img ${DEPLOYDIR}
}
