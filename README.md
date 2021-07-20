## Stepin_Miniproject

## Encrypted Multi Client MQTT based communication system

This project uses MQTT for communication between client and server. The encryption techique used in the project is Advanced Encryption Standard (AES). The Multi- Client is included in the project using sequencing. The second client sends data after the first client to the broker, third client sends data to the broker after second client and so on. The server receives the data from the broker in the same sequence in which data is sent.

The sequencing is managed by using multi threads and named semaphores.


# Requirements



# References

Asynchronous Library: https://eclipse.github.io/paho.mqtt.c/MQTTAsync/html/index.html

Publisher:https://eclipse.github.io/paho.mqtt.c/MQTTAsync/html/publish.html

Subscriber:https://eclipse.github.io/paho.mqtt.c/MQTTAsync/html/subscribe.html

