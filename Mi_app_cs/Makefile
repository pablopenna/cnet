all: server01 client01
	echo 'all done'
client01: client01.o
	gcc client01.o -o client01
client01.o: client01.c
	gcc -Wall -c client01.c
server01: server01.o
	gcc server01.o -o server01 -lpthread
server01.o: server01.c
	gcc -Wall -c server01.c
clean:
	rm *.o
	rm server01
	rm client01
