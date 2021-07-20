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




# Features

## Reliability

Reliability is an important aspect in communication system, as faulty or dummy data can create severe problems within the system. Also snooping the data can potentially expose privacy of the user. To ensure reliability

1. AES (Advanced Encryption Standard) algorithm is implemented
2. 128 bit encryption also boosts the immunity of the system
3. MQTT(Message Queueing Telemetry Transport) protocol is used which uses TCP protocol internally(takes care of certain level of reliability)

## Scalability
As the MQTT broker is capable of connecting to multiple clients, the system is scalable upto a certain number, such that the real time data validity does not get affected(Depends on sensor sampling rate).

## Cost

It is a developing free of cost open source software.



## 4W's and 1H

### WHO
Helpful to every citizen availing remote household facilities.
### WHAT
An efficient 128 bit encrypted comunication system capable of offering smart home infrastucture.
### WHEN
It is an 24*7 service.
### WHERE
It can be applied to any house having proper infrastructure. 
### HOW
By installing required senosrs and the system, the features can be relished.

## SWOT Analysis

![SWOT Analysis](https://github.com/bob2510/Stepin_Miniproject/blob/7100d99b3b5c3bb1fe5d1253afb29a3c92525a4a/SDLC/1_Requirements/SWOT.jpg)

# Detail Requirements
## High Level Requirements
| ID | Description | Status |
|----|------------|---------|
|HR_01| Connection between client and Server| Implemented|
|HR_02| Synchronisation between Clients | Implemented|
|HR_03| Advanced Encryption Standard Implementation (128 bit)| Implemented|
|HR_04| Installing MQTT in mobile / Laptop to control system from end user | Future|

## Low Level Requirements

| ID | Description | Status |
|----|------------|---------|
|LR_01| Building and installing MQTT libraries and source code| Implemented|
|LR_02| Creating connections using MQTT APIs | Implemented|
|LR_03| AES source code implementaion using 128 bit encryption| Implemented|
|LR_04| Using named semaphores to internally synchronise the clients | Implemented|
|LR_05| Storing received data using file handling in the server side | Implemented|





