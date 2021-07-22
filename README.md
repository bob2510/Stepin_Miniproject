# Encrypted Multi Client MQTT based communication system

This project uses MQTT for communication between client and server. The encryption techique used in the project is Advanced Encryption Standard (AES). The Multi- Client is included in the project using sequencing. The second client sends data after the first client to the broker, third client sends data to the broker after second client and so on. The server receives the data from the broker in the same sequence in which data is sent.

The sequencing is managed by using multi threads and named semaphores.

|---------------|----------------|
|Build Status|[![Build_env](https://github.com/bob2510/Stepin_Miniproject/actions/workflows/c-cpp.yml/badge.svg)](https://github.com/bob2510/Stepin_Miniproject/actions/workflows/c-cpp.yml)|


## MQTT Requirements


### Libraries

The Paho C client comprises four variant libraries, shared or static:

 * paho-mqtt3a - asynchronous (MQTTAsync)
 * paho-mqtt3as - asynchronous with SSL (MQTTAsync)
 * paho-mqtt3c - "classic" / synchronous (MQTTClient)
 * paho-mqtt3cs - "classic" / synchronous with SSL (MQTTClient)

[Which Paho C API to use, with some history, for context](https://modelbasedtesting.co.uk/2013/10/13/which-paho-mqtt-c-api-to-use-and-some-history/)

### Usage and API

Detailed API documentation [is available online](https://www.eclipse.org/paho/files/mqttdoc/MQTTClient/html/index.html).  It is also available by building the Doxygen docs in the  ``doc`` directory. 

Samples are available in the Doxygen docs and also in ``src/samples`` for reference.  These are:

- paho_c_pub.c and paho_c_sub.c: command line utilities to publish and subscribe, -h will give help
- paho_cs_pub.c paho_cs_sub.c: command line utilities using MQTTClient to publish and subscribe
- MQTTClient_publish.c,	MQTTClient_subscribe.c and MQTTClient_publish_async.c: MQTTClient simple code examples
- MQTTAsync_publish.c and MQTTAsync_subscribe.c: MQTTAsync simple code examples						


### Build instructions for GNU Make

Ensure the OpenSSL development package is installed. (See [Build requirements](https://github.com/bob2510/Stepin_Miniproject/blob/main/README.md#build-instructions-for-gnu-make) if not installed)

First of all, download the client library folder in your system.
To download the folder, clone the following github repository in your system by following command

```
git clone https://github.com/eclipse/paho.mqtt.c.git

```
Then from the client library base directory run:

```
make
sudo make install
```

This will build and install the libraries.  To uninstall:

```
sudo make uninstall
```

To build the documentation requires doxygen and optionally graphviz.

```
make html
```

### Build requirements

The build process currently supports a number of Linux "flavors" including ARM and s390, OS X, AIX and Solaris as well as the Windows operating system. The build process requires the following tools:
  * CMake (http://cmake.org)
  * Ninja (https://martine.github.io/ninja/) or
    GNU Make (https://www.gnu.org/software/make/), and
  * gcc (https://gcc.gnu.org/).

On Debian based systems this would mean that the following packages have to be installed:

```
apt-get install build-essential gcc make cmake cmake-gui cmake-curses-gui
```

Also, in order to build a debian package from the source code, the following packages have to be installed

```
apt-get install fakeroot fakeroot devscripts dh-make lsb-release
```

Ninja can be downloaded from its github project page in the "releases" section. Optionally it is possible to build binaries with SSL support. This requires the OpenSSL libraries and includes to be available. E. g. on Debian:

```
apt-get install libssl-dev
```

The documentation requires doxygen and optionally graphviz:

```
apt-get install doxygen graphviz
```


## AES Requirements


Just download the 'AES' folder in your system






# References

Asynchronous Library: https://eclipse.github.io/paho.mqtt.c/MQTTAsync/html/index.html

Publisher:https://eclipse.github.io/paho.mqtt.c/MQTTAsync/html/publish.html

Subscriber:https://eclipse.github.io/paho.mqtt.c/MQTTAsync/html/subscribe.html

