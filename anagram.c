
/*program that prints out the letters in a given string in alphabetical order*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>





/*method that inserts a letter into a given place and returns a new array with the letter in the 
correct spot. moves all letters after the place down one spot*/

char * insert (char * old, int size, char let, int place) {

	char * new = (char*)malloc(sizeof(char) * size);
	
	int i;
	for (i=0; i<size; i++) {
		if (i<place) {
			/*if before place nothing changes*/
			new[i] = old[i];
		} else if (i==place) {
			/*if at the place, insert the new letter*/
			new[i] = let;
		} else {
			/*if after the place, insert the one from the place before in the old*/
			new[i] = old[i-1];
		}
	}
	new[size-1] = '\0';


	return new;
}

/*function that puts the  iven string into a new array in alphabetical order*/

char * arrange (char * str, int size) {


	char * result = (char *) malloc (sizeof(char)*size);
    
    int i, j;
	/*initialize result ot all '.'*/
	for (i=0; i<size; i++) {
		result[i] = '.';
	}
	result[size-1]='\0';

	for (i=0; i<size-1; i++) { /*goes thru str*/
		
		for (j=0; j<size-1; j++) { /* goes thru result*/
			if (result[j]=='.') {
				result[j]=str[i];
				break;
			} else if (result[j]>str[i]) {
				
				result = insert(result, size, str[i], j);
				
				break;
			} else {
				continue;
			}
		}
		/*if the next letter in the string is alphabetically before the letter in result, use insert function*/
	}

	return result;
}


int main (int argc, char** argv) {

	int size = 0;
	for (size=0; argv[1][size]!='\0'; size++) {
	};
	size++;

	char * target = (char *)malloc(sizeof(char)*size);
	strcpy(target, argv[1]);

	target = arrange (target, size);

	printf("%s\n", target);



	return 0;
}

