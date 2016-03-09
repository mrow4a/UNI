all: linked

linked: linked.o
	gcc -g $^ -o $@ -lm

.c.o: 
	gcc -g -Wall -pedantic -c $<

linked.o: linked.c 

clean:
	-rm linked.o linked
