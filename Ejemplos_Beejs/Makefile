all: showip beejs
	echo 'all done'
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
