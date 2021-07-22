# Building Essential packages

sudo apt-get install build-essential gcc make cmake cmake-gui cmake-curses-gui
sudo apt-get install fakeroot fakeroot devscripts dh-make lsb-release
sudo apt-get install libssl-dev
sudo apt-get install -y doxygen

# Building source and libraries

sudo git clone https://github.com/eclipse/paho.mqtt.c.git
cd paho.mqtt.c 
sudo make
sudo make install
sudo make html
