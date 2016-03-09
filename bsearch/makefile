all: bsearch

bsearch: bsearch.o
	gcc -g $^ -o $@
.c.o:
	gcc -g -Wall -pedantic -c $<

bsearch.o: bsearch.c

clean:
	-rm bsearch.o bsearch
