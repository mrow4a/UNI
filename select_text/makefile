OBJS = select.o 
CC = gcc
DEBUG = -g -pedantic -Werror 
CFLAGS = -Wall -c $(DEBUG)
LFLAGS = -Wall $(DEBUG)

select : $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) -o select

select.o: select.c select.h
	$(CC) $(CFLAGS) select.c
	
clean:
	rm *.o *~ select