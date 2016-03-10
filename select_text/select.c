#include "select.h"

int is_alph(char c){
	/*check if is alphanumeric*/
	if ((c>='a' && c<='z') || (c>='A' && c<= 'Z')){
		return 1;
	}
	return 0;
}

char* get_word(FILE** fpin){
	FILE* fp = (*fpin);
	char c;
	char old_c=NULL;
	size_t size = 0;
	size_t buff_size = 0;
	char* word = NULL;

	while ((c = fgetc(fp))!=EOF){
		if (is_alph(c)==0 && old_c=='t'){
			/*if last character was t and present is not alphanumeric,
			word with t is finished, just require addins special sign \0 !*/
			word = realloc(word, ++buff_size);
			word[size++]='\0';
			/*printf("save");*/
			return word;
		}
		else if (is_alph(c)==1 && is_alph(old_c)==0){
			/*last was not alpha numeric, new is, start word creation from zero!*/
			/*printf("start");*/
			size=0;
			buff_size=0;
			free(word);
			word = NULL;
		}
		/*else if (is_alph(c)==0 && is_alph(old_c)==1){
			printf("stop");
		}
		if (c=='\n'){
			printf("\n");
		}else if (c!='\0'){
			printf("%d",is_alph(c));
		}*/
		word = realloc(word, ++buff_size);
		word[size++]=(char) c;
		old_c=c;
	}
	/*ensure that word is cleared*/
	free(word);
	(*fpin)=fp;
	return NULL;
}

int select_text(char* infile, char* outfile){
	FILE* fpin;
	FILE* fpout;
	char* word;

	fpin = fopen(infile,"r");
	fpout = fopen(outfile,"w");
	if (fpin == NULL || fpout == NULL){
		perror("Error in opening file");
		return -1;
	}
	while((word=get_word(&fpin))!=NULL) {
		/*jump over the file contents with file pointer
		 and find words, free them after saving to new file */
		fprintf(fpout, "%s\n", word);
		free(word);
	}
	fclose(fpin);
	fclose(fpout);
	return 0;

}

int main(int argc, char* argv[]){
	int select_text(char*,char*);

	if (argc!=3){
		fprintf(stderr, "Please specify: ./sort [infile] [outfile] !\n");
		return -1;
	}
	else{
		return select_text(argv[1],argv[2]);
	}
}
