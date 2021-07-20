# Introduction

An encrypted communication system for remotely monitoring IoT based smart home environment.


# Encrypted Multi Client MQTT based communication system

This project uses MQTT for communication between client and server. The encryption techique used in the project is Advanced Encryption Standard (AES).
<img src = "MQTT.jpg" alt = "MQTT">
This project is capable of connecting to multiple clients at a time to the broker and sending data to the server in a sequence. The clients are internally synchronized within each other. Any particular client sends data after receiving signal from previous publisher that sent data. The server receives the data from the broker in the same sequence in which the data is sent.

The sequencing is managed by using multi threads and named semaphores in each file.


