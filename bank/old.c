int processFile() {
	FILE *fp;
	int i,j,size;
	
	char filename[30]="./test-matrix/mat3.txt";
	
	/* TO BE FIXED */
	char c;
	do {
		printf("which file to open? (3-7) ");
		c=getchar();
	} while (c<'3' || c>'7');
	filename[17]=c;
	/* /TO BE FIXED */
		
	/* open file for reading */
	printf("\nopening file:\n%s\n", filename);
	fp=fopen(filename, "rt");
	
	/* check if file opened properly: */
	if (!fp) {
		printf ("can\'t read the file, quitting\n");
		return 1;
	}
		
	/* read size of the matrix */
	if ((fscanf(fp, "%d", &size))<=0) {
		printf("\nerror while reading file, probably encountered a NaN character, quitting\n");
		fclose(fp);
		return 1;
	}
	
	
	
	
	/* check if we reached end of file because we should if size was correct */
	/* if NOT EOF and NOT \r and NOT \n then there is something that should not be there*/
	if (!feof(fp) && !(fgetc(fp)=='\r') && !(fgetc(fp)=='\n')) {
		printf("\nerror while reading file, probably contained too much or to few numbers, quitting\n");
		freeMatrix(matrix,size);
		fclose(fp);
		return 1;
	}
	
	/* close file, we are done reading */
	fclose (fp); fp=NULL;
	
	
	
	
	return 0;
}

int processInput() {
