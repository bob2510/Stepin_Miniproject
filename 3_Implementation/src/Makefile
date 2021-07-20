sub.out:subscriber.c AES/aes.c
	gcc $^ -o $@ -lpaho-mqtt3a -lpthread
clean:	
	rm -rf *.txt *.out
test:sub.out	
	./sub.out