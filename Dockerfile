
FROM ubuntu:14.04

RUN \
  apt-get update && \
  apt-get -y install \
    ruby-dev zip \
    gcc-mingw-w64 \
    g++ libxi-dev libgl1-mesa-dev libglu1-mesa-dev libxrandr-dev \
    cmake

#    automake autogen bzip2 ca-certificates curl file git gzip libcurl4-openssl-dev libssl-dev make ncurses-dev pkg-config python \
#    rsync sed tar vim emacs wget xz-utils zlib1g-dev \

COPY . /data/get-image

CMD ["bash"]

  
