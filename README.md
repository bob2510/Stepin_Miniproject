# Encrypted Multi Client MQTT based communication system

This project uses MQTT for communication between client and server. The encryption techique used in the project is Advanced Encryption Standard (AES). The Multi- Client is included in the project using sequencing. The second client sends data after the first client to the broker, third client sends data to the broker after second client and so on. The server receives the data from the broker in the same sequence in which data is sent.

The sequencing is managed by using multi threads and named semaphores.


## Requirements

1) MQTT Setup

## Libraries

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


## Build instructions for GNU Make

Ensure the OpenSSL development package is installed.  Then from the client library base directory run:

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

The provided GNU Makefile is intended to perform all build steps in the ```build``` directory within the source-tree of Eclipse Paho. Generated binares, libraries, and the documentation can be found in the ```build/output``` directory after completion. 

Options that are passed to the compiler/linker can be specified by typical Unix build variables:

Variable | Description
------------ | -------------
CC | Path to the C compiler
CFLAGS | Flags passed to compiler calls
LDFLAGS | Flags passed to linker calls


## Build requirements / compilation using CMake

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

Before compiling, determine the value of some variables in order to configure features, library locations, and other options:

Variable | Default Value | Description
------------ | ------------- | -------------
PAHO_BUILD_SHARED | TRUE | Build a shared version of the libraries
PAHO_BUILD_STATIC | FALSE | Build a static version of the libraries
PAHO_HIGH_PERFORMANCE | FALSE | When set to true, the debugging aids internal tracing and heap tracking are not included.
PAHO_WITH_SSL | FALSE | Flag that defines whether to build ssl-enabled binaries too. 
OPENSSL_ROOT_DIR | "" (system default) | Directory containing your OpenSSL installation (i.e. `/usr/local` when headers are in `/usr/local/include` and libraries are in `/usr/local/lib`)
PAHO_BUILD_DOCUMENTATION | FALSE | Create and install the HTML based API documentation (requires Doxygen)
PAHO_BUILD_SAMPLES | FALSE | Build sample programs
MQTT_TEST_BROKER | tcp://localhost:1883 | MQTT connection URL for a broker to use during test execution
MQTT_TEST_PROXY | tcp://localhost:1883 | Hostname of the test proxy to use
MQTT_SSL_HOSTNAME | localhost | Hostname of a test SSL MQTT broker to use
PAHO_BUILD_DEB_PACKAGE | FALSE | Build debian package

Using these variables CMake can be used to generate your Ninja or Make files. Using CMake, building out-of-source is the default. Therefore it is recommended to invoke all build commands inside your chosen build directory but outside of the source tree.

An example build session targeting the build platform could look like this:

```
mkdir /tmp/build.paho
cd /tmp/build.paho
cmake -GNinja -DPAHO_WITH_SSL=TRUE -DPAHO_BUILD_DOCUMENTATION=TRUE -DPAHO_BUILD_SAMPLES=TRUE ~/git/org.eclipse.paho.mqtt.c
```

Invoking cmake and specifying build options can also be performed using cmake-gui or ccmake (see https://cmake.org/runningcmake/). For example:

```
ccmake -GNinja ~/git/org.eclipse.paho.mqtt.c
```

To compile/link the binaries and to generate packages, simply invoke `ninja package` or `make -j <number-of-cores-to-use> package` after CMake. To simply compile/link invoke `ninja` or `make -j <number-of-cores-to-use>`.

### Debug builds

Debug builds can be performed by defining the value of the ```CMAKE_BUILD_TYPE``` option to ```Debug```. For example:

```
cmake -GNinja -DCMAKE_BUILD_TYPE=Debug git/org.eclipse.paho.mqtt.c
```


### Running the tests

Test code is available in the ``test`` directory. The tests can be built and executed with the CMake build system. The test execution requires a MQTT broker running. By default, the build system uses ```localhost```, however it is possible to configure the build to use an external broker. These parameters are documented in the Build Requirements section above.

After ensuring a MQTT broker is available, it is possible to execute the tests by starting the proxy and running `ctest` as described below:

```
python ../test/mqttsas2.py &
ctest -VV
```





# References

Asynchronous Library: https://eclipse.github.io/paho.mqtt.c/MQTTAsync/html/index.html

Publisher:https://eclipse.github.io/paho.mqtt.c/MQTTAsync/html/publish.html

Subscriber:https://eclipse.github.io/paho.mqtt.c/MQTTAsync/html/subscribe.html

