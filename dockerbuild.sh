#!/bin/sh
#
#

DIR="`pwd`"

cat <<EOF > build/Dockerfile
FROM debian:jessie
ENV DEBIAN_FRONTEND noninteractive
RUN sed -i "s|^deb-src|#deb-src|g" /etc/apt/sources.list
RUN echo "deb http://http.debian.net/debian jessie-backports main" >> /etc/apt/sources.list
RUN apt-get update && apt-get -y upgrade
RUN apt-get install -y git make gcc-arm-none-eabi dfu-util stm32flash
EOF
if test -d ./src/
then
	echo "RUN mkdir -p /usr/src/quadfork" >> build/Dockerfile
	for PART in Makefile Libraries src
	do
		echo "COPY "$DIR/$PART" /usr/src/quadfork/$PART" >> build/Dockerfile
	done
else
	echo "RUN (cd /usr/src/ ; git clone https://github.com/multigcs/quadfork.git)" >> build/Dockerfile
fi

mkdir -p build
echo "#!/bin/sh" > build/build.sh
echo "(cd /usr/src/quadfork ; make \$@)" >> build/build.sh
chmod 755 build/build.sh

docker build -t quadfork build
docker run --privileged=true -i -t -v "$DIR/build":/usr/src/quadfork/build quadfork /bin/bash /usr/src/quadfork/build/build.sh $@

rm -rf build/Dockerfile
rm -rf build/build.sh

