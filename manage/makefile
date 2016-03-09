OBJS = employee.o main.o 
CC = g++
DEBUG = -g -pedantic -Werror
CFLAGS = -Wall -c $(DEBUG)
LFLAGS = -Wall $(DEBUG)

manage : $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) -o manage

main.o : employee.h map.h main.cpp  
	$(CC) $(CFLAGS) main.cpp

employee.o : employee.h employee.cpp 
	$(CC) $(CFLAGS) employee.cpp


clean:
	rm *.o *~ manage
