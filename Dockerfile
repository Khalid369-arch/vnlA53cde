FROM amazoncorretto:11

ADD https://detect.synopsys.com/detect9.sh /

RUN chmod a+x /detect9.sh

WORKDIR /workspace

ENTRYPOINT [ "/detect9.sh" ]

CMD [ "--blackduck.url=https://johndeere.app.blackduck.com", \
      "--blackduck.trust.cert=true", \
      "--blackduck.api.token=${API_TOKEN}", \
      "--detect.project.name=${PROJECT_NAME}", \
      "--detect.project.version.name=${BRANCH_NAME}-latest", \
      "--spring.config.location=/workspace/${CONFIG_FILE}" ]
