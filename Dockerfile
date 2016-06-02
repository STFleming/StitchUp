FROM ubuntu:14.04
MAINTAINER Shane Fleming <sf306@ic.ac.uk>

#	 ________  _________  ___  _________  ________  ___  ___  ___  ___  ________   
#	|\   ____\|\___   ___\\  \|\___   ___\\   ____\|\  \|\  \|\  \|\  \|\   __  \  
#	\ \  \___|\|___ \  \_\ \  \|___ \  \_\ \  \___|\ \  \\\  \ \  \\\  \ \  \|\  \ 
#	 \ \_____  \   \ \  \ \ \  \   \ \  \ \ \  \    \ \   __  \ \  \\\  \ \   ____\
#	  \|____|\  \   \ \  \ \ \  \   \ \  \ \ \  \____\ \  \ \  \ \  \\\  \ \  \___|
#	    ____\_\  \   \ \__\ \ \__\   \ \__\ \ \_______\ \__\ \__\ \_______\ \__\   
#	   |\_________\   \|__|  \|__|    \|__|  \|_______|\|__|\|__|\|_______|\|__|   
#	   \|_________|                                                                
#	

RUN dpkg --add-architecture i386

RUN apt-get -y update


RUN echo 'mysql-server mysql-server/root_password password shiny' | debconf-set-selections 
RUN echo 'mysql-server mysql-server/root_password_again password shiny' | debconf-set-selections 

RUN apt-get -y install git tcl8.5-dev dejagnu expect texinfo build-essential \
    liblpsolve55-dev libgmp3-dev automake libtool clang-3.5 libmysqlclient-dev \
    qemu-system-arm qemu-system-mips gcc-4.8-plugin-dev libc6-dev-i386 meld \
    libqt4-dev libgraphviz-dev libfreetype6-dev buildbot-slave vim gitk kdiff3 \
    clang-3.5 openssh-server mysql-server python3-mysql.connector python3-serial \
    python3-pyqt5 libxft2

RUN apt-get -y install libc6-i386 libXi6:i386 \
    libpng12-dev:i386 libfreetype6-dev:i386 libfontconfig1:i386 libxft2:i386 \
    libncurses5:i386 libsm6:i386 libxtst6:i386

RUN apt-get -y install git tcl8.5-dev dejagnu expect texinfo build-essential \
    liblpsolve55-dev libgmp3-dev automake libtool clang-3.5 libmysqlclient-dev \
    qemu-system-arm qemu-system-mips gcc-4.8-plugin-dev libc6-dev-i386 libqt4-dev \
    libfreetype6-dev

RUN wget http://archive.ubuntu.com/ubuntu/pool/universe/g/gxemul/gxemul-doc_0.4.7.2-1_all.deb
RUN dpkg -i gxemul-doc_0.4.7.2-1_all.deb

#Create the stitchup user and give it root access
RUN useradd -m stitchup && \
    echo stitchup:stitchup | chpasswd && \
    cp /etc/sudoers /etc/sudoers.bak && \
    echo 'klee  ALL=(root) NOPASSWD: ALL' >> /etc/sudoers

ADD . /home/stitchup/
RUN wget http://legup.eecg.utoronto.ca/releases/legup-4.0.tar.gz
RUN tar -xvf legup-4.0.tar.gz -C /home/stitchup/ 
RUN chown -R stitchup:stitchup /home/stitchup/* 

USER stitchup
WORKDIR /home/stitchup

RUN sed -i -r -e "s/CONFIG\s+\+= debug_and_release display_graphs/CONFIG \+= debug_and_release #display_graphs/g" legup-4.0/gui/scheduleviewer/scheduleviewer.pro

RUN ./patch /home/stitchup/legup-4.0

#Edit the startup message that is displayed
RUN echo "echo \" ________  _________  ___  _________  ________  ___  ___  ___  ___  ________    \"" >> /home/stitchup/.bashrc
RUN echo "echo \"|\   ____\|\___   ___\\\\  \|\___   ___\\\\   ____\|\  \|\  \|\  \|\  \|\   __  \   \"" >> /home/stitchup/.bashrc
RUN echo "echo \"\ \  \___|\|___ \  \_\ \  \|___ \  \_\ \  \___|\ \  \\\\\\  \ \  \\\\\\  \ \  \|\  \  \"" >> /home/stitchup/.bashrc
RUN echo "echo \" \ \_____  \   \ \  \ \ \  \   \ \  \ \ \  \    \ \   __  \ \  \\\\\\  \ \   ____\ \"" >> /home/stitchup/.bashrc
RUN echo "echo \"  \|____|\  \   \ \  \ \ \  \   \ \  \ \ \  \____\ \  \ \  \ \  \\\\\\  \ \  \___| \"" >> /home/stitchup/.bashrc
RUN echo "echo \"    ____\_\  \   \ \__\ \ \__\   \ \__\ \ \_______\ \__\ \__\ \_______\ \__\    \"" >> /home/stitchup/.bashrc
RUN echo "echo \"   |\_________\   \|__|  \|__|    \|__|  \|_______|\|__|\|__|\|_______|\|__|    \"" >> /home/stitchup/.bashrc
RUN echo "echo \"   \|_________|                                                                 \"" >> /home/stitchup/.bashrc

