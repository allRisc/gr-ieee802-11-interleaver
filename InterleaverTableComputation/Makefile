CC=gcc
CFLAGS=-Wall -ansi -g -std=c99

MAIN=interleaverTableGen

all : $(MAIN)

$(MAIN) : interleaver_table_gen.o hash_drgb.o sha256.o
	$(CC) $(CFLAGS) -o $@ $^

interleaver_table_gen.o : interleaver_table_gen.c
	$(CC) $(CFLAGS) -c $<

hash_drgb.o : hash_drgb.c hash_drgb.h
	$(CC) $(CFLAGS) -c $<

sha256.o : sha256.c sha256.h
	$(CC) $(CFLAGS) -c $<

clean : 
	rm *.o
