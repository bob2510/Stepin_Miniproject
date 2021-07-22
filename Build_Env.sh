# Building Essential packages

apt-get install build-essential gcc make cmake cmake-gui cmake-curses-gui
apt-get install fakeroot fakeroot devscripts dh-make lsb-release
apt-get install libssl-dev

# Building source and libraries

git clone https://github.com/eclipse/paho.mqtt.c.git
cd paho.mqtt.c 
make
sudo make install
make html