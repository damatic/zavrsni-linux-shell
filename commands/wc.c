#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h> // potrebno za rmdir

#define BUFFER_SIZE_FOR_NAMES 50
#define BUFFER_LENGTH 1024


void print_error(const char *this, const char *filename)
{	// u slucaju da radnja ne uspije iz nekog razloga
	// this ce biti ime komande
	fprintf(stderr, "%s: cannot count chars/words/lines in '%s'\n"
					"ERROR: %s\n", this, filename, strerror(errno));
	
	exit(EXIT_FAILURE);
}

void print_usage(const char *this)
{	// u slucaju da nije sintaksno tocno
	fprintf(stderr, "SYNTAX ERROR: \n"
					"USAGE %s [OPTION] [FILE]\n", this);
	
	exit(EXIT_FAILURE);
}


int main(int argc, char* argv[])
{
	int c;
	FILE *file;
	struct stat stbuf1, stbuf2;
	int count = 0;
	char word[BUFFER_SIZE_FOR_NAMES];
	int flag = 0;
	
	if(argv[1][0] != '-'){
		print_usage(argv[0]);
	}
	
	if(fstat(3, &stbuf2) == 0 && argv[2] == NULL){ // provjera ako je fd otvoren, stdin spojen na read kraj od pipe-a...
		flag++;
		if((file = fopen("pipe_temp", "w")) == NULL){
			print_error(argv[0], "pipe_temp");
		}
		
		while((c = getc(stdin)) != EOF ){
			fprintf(file, "%c", c);
		}
		
		fclose(file);
		argv[2] = malloc(sizeof(char)*BUFFER_LENGTH);
		strcpy(argv[2], "pipe_temp");
	}

	if(stat(argv[2], &stbuf1) == -1){ // ako datoteka za citanje ne postoji
		print_error(argv[0], argv[2]);
		return EXIT_FAILURE;
	}

	if((file = fopen(argv[2], "r")) == NULL){
			print_error(argv[0], argv[2]);
			return EXIT_FAILURE;
	}
	
	if(strcmp(argv[1], "-c") == 0){
		while ((c = getc(file)) != EOF){
			if(c != ' ' && c != '\n' && c != '\t')
				count++;
		}
		printf("Broj znakova u datoteci je: %d\n", count);
		
		if(flag > 1){ // ciscenje programa u slucaju da je bio pipe
			free(argv[2]);
			fclose(file);
			if(remove("pipe_temp") == -1){
				perror("cannot remove file 'pipe_temp'");
			}
		}
		return 0;
	}

	if(strcmp(argv[1], "-w") == 0){
		while(fscanf(file, "%s", word) == 1){
			count++;
		}
		printf("Broj \"rijeci\" u datoteci je: %d\n", count);
		
		if(flag > 0){ // ciscenje programa u slucaju da je bio pipe
			free(argv[2]);
			fclose(file);
			if(remove("pipe_temp") == -1){
				perror("Cannot remove file 'pipe_temp'");
			}
		}
		return 0;
	}
	 
	if(strcmp(argv[1], "-l") == 0){
		while ((c = getc(file)) != EOF){
			if(c == '\n')
				count++;
		}
		printf("Broj linija u datoteci je: %d\n", count);
		
		if(flag > 0){ // ciscenje programa u slucaju da je bio pipe
			free(argv[2]);
			fclose(file);
		if(remove("pipe_temp") == -1){
				perror("cannot remove file 'pipe_temp'");
			}
		}	
		return 0;
	}
	fclose(file);
	
	/*if(flag == 1){ // ciscenje programa u slucaju da je bio pipe
		free(argv[2]);
		if(remove("pipe_temp") == -1){
			perror("ERROR: Cannot remove file 'pipe_temp'");
		}
	}*/
	
	return 0;
}
