/*yell.c*/
/*program that echos the string given to it, in all caps*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>




int main(int argc, char **argv) {


	

	int size=0;
	
	
	/* finds the size of the argument*/
	int i;
	for (i=0; argv[1][i]!='\0'; i++) {
		size++;
	}

	char target[size+1];
	/*copies the argument into an array to be manipulated*/
	int k;
	for (k=0; k<=size; k++) {
		target[k] = argv[1][k];
	}

	/*a line to check to make sure the string was copied correctly, commented out
	printf("%s\n", target);*/

	

	/*changes a letter if it is lowercase into an uppercase*/
	for (k=0; k<size; k++) {
		if (target[k] <='z' && target [k]>='a') {
			target[k] = toupper(target[k]);
		}
	}
	
	printf("%s\n", target);

	return 0;
}

	
