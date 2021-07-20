sub.out:subscriber.c test.c ../AES/aes.c
	gcc $^ -o $@ -lpaho-mqtt3a -lpthread -I../AES
clean:	
	rm -rf *.txt *.out
test:sub.out	
	./sub.out