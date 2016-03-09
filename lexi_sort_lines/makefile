OBJS = sort.o 
CC = gcc
DEBUG = -g -pedantic -Werror 
CFLAGS = -Wall -c $(DEBUG)
LFLAGS = -Wall $(DEBUG)

sort : $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) -o sort

sort.o: sort.c sort.h
	$(CC) $(CFLAGS) sort.c
	
clean:
	rm *.o *~ sort