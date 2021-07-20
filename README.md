# Encrypted Multi Client MQTT based communication system

This project uses MQTT for communication between client and server. The encryption techique used in the project is Advanced Encryption Standard (AES). The Multi- Client is included in the project using sequencing. The second client sends data after the first client to the broker, third client sends data to the broker after second client and so on. The server receives the data from the broker in the same sequence in which data is sent.

The sequencing is managed by using multi threads and named semaphores.


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

## AES Requirements









# References

Asynchronous Library: https://eclipse.github.io/paho.mqtt.c/MQTTAsync/html/index.html

Publisher:https://eclipse.github.io/paho.mqtt.c/MQTTAsync/html/publish.html

Subscriber:https://eclipse.github.io/paho.mqtt.c/MQTTAsync/html/subscribe.html

