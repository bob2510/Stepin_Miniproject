# Introduction

An encrypted communication system for remotely monitoring IoT based smart home environment.


# Encrypted Multi Client MQTT based communication system

This project uses MQTT for communication between client and server. The encryption techique used in the project is Advanced Encryption Standard (AES).

![MQTT](https://github.com/bob2510/Stepin_Miniproject/blob/008ac0e8f18359da6aa12ccb0996bd624b47fbcb/SDLC/1_Requirements/MQTT.jpg)

This project is capable of connecting to multiple clients at a time to the broker and sending data to the server in a sequence. The clients are internally synchronized within each other. Any particular client sends data after receiving signal from previous publisher that sent data. The server receives the data from the broker in the same sequence in which the data is sent.

The sequencing is managed by using multi threads and named semaphores in each file.

# Advantages

* Secuered Connection
* Multi Client System
* Reduced Data Packet Loss
* Remote Monitoring

# Defining the System

Any MQTT system consists of a publisher, broker and a subscriber. Publisher sends data to the broker and subscriber receives the data from the broker. In our case, we have an encrypted multi publisher MQTT system. 

We have 4 publishers that connect to the broker at a same time and send data in a sequence with synchronization. As soon as the first publisher is done publishing, it sends the signal to the second publisher and only then the second publisher publishes the data and then it sends signal to the third publisher and so on.

The subscriber recieves data as soon as any publisher publishes the data and so it receives data in the same sequence. After recieving the data, subscriber processes it a bit and then notifies the user after processing.

![System](https://github.com/bob2510/Stepin_Miniproject/blob/db44949635d0a912d830b83c5cd99d0bd0271003/SDLC/2_Architecture/Behavioral_Diagram/Behavioral.jpg)






