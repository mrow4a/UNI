#include "sort.h"

int sort(char* infile, char* outfile){
	void add_to_file();
	int readlines();
	int compare_fun();
	char** lines=NULL; /*define string lines which will be read out from the file*/
	size_t lines_number;
	FILE* fp;

	fp = fopen(infile,"r");
	if(fp == NULL) /*opening the file error */
	{
		perror("Error in opening file");
	    return(-1);
	}

	lines_number = readlines(fp,&lines); /*pass a pointer to the array of stringsto be modified there*/

	fclose(fp);

	printf("\n");

    qsort(lines, lines_number, sizeof(char *), compare_fun); /*sort using char comparisons*/

	fp = fopen(outfile,"w");
	if(fp == NULL) /*opening the file error */
	{
		perror("Error in opening file");
	    return(-1);
	}
    add_to_file(fp,&lines,lines_number);
	fclose(fp);
	return 0;
}

int compare_fun (const void * a, const void * b ) {
    const char *pa = *(const char**)a;
    const char *pb = *(const char**)b;

    return strcmp(pa,pb);
}

int readlines(FILE* fp,char*** ptr_lines){
	void add_char_lines(size_t,size_t,char***, char);
	char ** lines = (*ptr_lines);
	int c;
	int old_c=NULL;
	size_t bufsize_l = 0; /*buffer for lines*/
	size_t bufsize = 0; /*buffer for pointer to lines*/
	size_t size=0;

	while ((c = fgetc(fp))!=EOF){
		if (bufsize == 0 || old_c=='\n'){
			if (bufsize!=0 && old_c=='\n'){
				add_char_lines(++bufsize_l,size,&lines,'\0');
				bufsize_l=0;
			}
			bufsize=bufsize+sizeof(char*);
			size++;
			lines = realloc(lines,bufsize);
			lines[size-1] = NULL;
		}
		add_char_lines(++bufsize_l,size,&lines,(char) c);
		old_c = c;
	}
	if (c!=EOF){
		size++;
	}
	if (lines[size-1][bufsize_l-1]!='\n'){
		add_char_lines(++bufsize_l,size,&lines,'\n');
	}
	add_char_lines(++bufsize_l,size,&lines,'\0');
	(*ptr_lines) = lines;
	return size;
}

void add_char_lines(size_t bufsize_l,size_t size,char*** ptr_lines, char c){
	char ** lines = (*ptr_lines);
	lines[size-1] = realloc(lines[size-1],bufsize_l);
	lines[size-1][bufsize_l-1]=c;
}
void add_to_file(FILE* fp,char*** ptr_lines,size_t lines_number){
	char ** lines = (*ptr_lines);
	int i;

    /*free memory allocated in the lines*/
	for (i=0;i<lines_number;i++){
		fprintf(fp, "%s", lines[i]);
		free(lines[i]);
	}
	free(lines);
}

int main(int argc, char* argv[])
{
	int sort();

	if (argc!=3){
		fprintf(stderr, "Please specify: ./sort [infile] [outfile] !\n");
		return -1;
	}
	else{
		return sort(argv[1],argv[2]);
	}
}
