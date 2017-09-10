all: showip beejs ssserver
	echo 'all done'
showip: showip.o
	gcc showip.o -o showip
showip.o: showip.c
	gcc -Wall -c showip.c
beejs: beejs.o
	gcc beejs.o -o beejs
beejs.o: beejs.c
	gcc -Wall -c beejs.c
ssserver: ssserver.o
	gcc ssserver.o -o ssserver
ssserver.o: ssserver.c
	gcc -Wall -c ssserver.c
clean:
	rm *.o
	rm beejs
	rm showip
	rm ssserver
