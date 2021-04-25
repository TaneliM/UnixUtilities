#include <stdlib.h>
#include <string.h>
#include <stdio.h>


// List node struct
struct lines
{
	char* string;
	struct lines * prev;
	struct lines * next;
};


// Reading input to the list
int read_lines(struct lines** line, FILE* stream) {
	
	char* buffer = NULL;
	size_t n = 0;
	struct lines* lp;
	
	// reading input stream line by line
	while (getline(&buffer, &n , stream) >= 0) {
		if (buffer == NULL) {
			fprintf(stderr, "getline failed\n");
		}
		
		// Allocating memory for the first node
		if (*line == NULL) { 
			if ((*line = lp = malloc(sizeof(struct lines))) == NULL) {
				fprintf(stderr, "malloc failed\n");
				return -1;
			}
			
			lp->prev = lp->next = NULL;
		}
		// Allocating memory for a new node
		else { 
			if ( (lp->next = malloc(sizeof(struct lines))) == NULL) {
				fprintf(stderr, "malloc failed\n");
				return -1;
			}
			
			lp->next->next = lp->next->prev = NULL;
			lp->next->prev = lp;
			lp = lp->next;
		}
		
		// Allocating memory for the line to store it in the list
		if ( (lp->string = malloc(strlen(buffer) + 1)) == NULL) {
			fprintf(stderr, "malloc failed\n");
			return -1;
		}
		strcpy(lp->string, buffer);
	}
	free(buffer);
	return 0;
}


// Printing the list
void print_lines(struct lines * line, FILE * stream) {
	struct lines * lp;
	lp = line;
	
	while (lp != NULL) {
		fprintf(stream, "%s", lp->string);
		lp = lp->next;
	}
}


// Deleting the linked list
void delete_lines(struct lines * line) {
	struct lines * lp;
	lp = line;
	
	// Freeing allocated memory
	while (lp != NULL) {
		line = lp->next;
		free(lp->string);
		free(lp);
		lp = line;
	}
}


// Handling the arguments
int main(int argc, char** argv) {
	struct lines * line = NULL;
	FILE* pFile;
	
	if (argc > 1) {	
		// Going through all defined files
		for (int i = 1; i < argc; i++) {
			if ((pFile = fopen(argv[i], "r")) == NULL) {
				fprintf(stdout, "wcat: cannot open file\n");
				exit(1);
			}
			
			if (read_lines(&line, pFile) == -1) {
				exit(1);
			}
			
			print_lines(line, stdout);
			delete_lines(line);
			
			fclose(pFile);
		}
	}
	return 0;
}


