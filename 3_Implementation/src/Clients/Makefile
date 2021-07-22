CC = gcc
LIBPATH = ../AES
LIBTHREAD = pthread
SRC = ../AES/aes.c
LIBMQTT = paho-mqtt3c
all:	client1 client2 client3

client1:	client1.c 
	$(CC) $^ -I$(LIBPATH) $(SRC) -l$(LIBMQTT) -l$(LIBTHREAD) -o client1
client2:	client2.c	
	$(CC) $^ -I$(LIBPATH) $(SRC) -l$(LIBMQTT) -l$(LIBTHREAD) -o client2
client3:	client3.c 
	$(CC) $^ -I$(LIBPATH) $(SRC) -l$(LIBMQTT) -l$(LIBTHREAD) -o client3

clean: 
	rm -rf *.txt *.o *.out client1 client2 client3

mem_leak:	client1 client2 client3
	valgrind ./client1
	valgrind ./client2
	valgrind ./client3	
