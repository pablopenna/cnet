all: showip beejs server01 client01
	echo 'all done'
client01: client01.o
	gcc client01.o -o client01
client01.o: client01.c
	gcc -Wall -c client01.c
server01: server01.o
	gcc server01.o -o server01
server01.o: server01.c
	gcc -Wall -c server01.c
showip: showip.o
	gcc showip.o -o showip
showip.o: showip.c
	gcc -Wall -c showip.c
beejs: beejs.o
	gcc beejs.o -o beejs
beejs.o: beejs.c
	gcc -Wall -c beejs.c
clean:
	rm *.o
	rm beejs
	rm showip
	rm server01
	rm client01
