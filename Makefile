CC=gcc
CFLAGS=-I.

clone: clone.c 
	$(CC) -o clone clone.c
