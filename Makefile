all:
	gcc -pthread -g -Wall -std=gnu99 	mfind.c sfunc.c mlist.c -o mfind
test:
	gcc -pthread -g -Wall -std=gnu99 -DEBUG	mfind.c sfunc.c mlist.c -o mfind
