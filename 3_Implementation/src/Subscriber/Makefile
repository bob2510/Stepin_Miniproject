sub.out:subscriber.c test.c ../AES/aes.c
	gcc $^ -o $@ -lpaho-mqtt3a -lpthread -I../AES
clean:	
	rm -rf *.txt *.out

mem_leak:
	valgrind -s --leak-check=full --show-leak-kinds=all ./sub.out
	
test:sub.out	
	./sub.out