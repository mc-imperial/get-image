
FROM ubuntu:14.04

RUN \
  apt-get update && \
  apt-get -y install \
    gcc-mingw-w64 python cmake libxi-dev

COPY . /data/get-image

CMD ["bash"]

  
