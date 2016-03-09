#include <stdio.h>
#include <stdlib.h>
#include <string.h>



int main(int argc, char* argv[])
{
	void *bsearch();
	int comparison();
    	char *string[10] = {"ALA","ELA","OLA","KAROLA","MARIOLA","WIOLA"};
    	char **result; 
    	char *find = "ELA";

    	result = bsearch(&find, string, sizeof(string)/sizeof(*string), sizeof(*string), comparison);
    	if(result)
	{
		printf("\nFound word %s, at position in string %d\n", *result, result-string+1);
	}
        else if(result==NULL)
        {
		printf("\nWord %s does not exists\n", *result);
	}

    	return 0;
}

void *bsearch(const void *key, const void *base, size_t nmemb, size_t size, int (*compar)(const void *, const void *))
{
	int low=0, high, middle; 
	void *result;
 	
	high=nmemb;
	while(high > low)
	{
		middle = low+(high-low)/2;
		result = (char*)base + middle*size;
		if((compar(result, key))==0)
		{
			return result;
		}
		else if((compar(result, key)) > 0)
		{
			high = middle;
		}
		else
		{
			low = middle + 1;
		}
	}
	return NULL;
}


int comparison(const void *a, const void *b)
{
	int result;
	char** string1;
	char** string2;

	string1 = (char**)a;
	string2 = (char**)b;
	result=strcmp(*string1, *string2);

	return result;
}

