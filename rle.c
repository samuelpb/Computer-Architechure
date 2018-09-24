/*program that takes a string and condenses the repeating letters i.e aaa = a3;
if the condensed string is longer, then it just prints the original instead.
no numbers can be in the string to be condensed*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>






/*function that takes a strng and replaces sequential repeating letters with numbers
i.e aaa = a3, aaabbb = a3b3*/


char * condense (char * str, int size) {

	char * result = (char *)malloc(sizeof(char) * (size*2));
	char * target = (char *)malloc(sizeof(char) * (size));
	strcpy(target, str);
	
	

	

	char curr = target[0];



	int counter=1, pholder =0, tenCount=0, temp=0, i;

	for (i=1; i<size; i++) {
		if (target[i]==curr) {
			counter++;
		}else {
			/*if less than 10 repetitions, easy to insert as it is one digit*/
			if (counter<10) {
				result[pholder]=curr;
				pholder++;
				result[pholder] = (char) counter + 48;
				pholder++;
				curr=target[i];
				counter=1;
			}else {
				/*if more than 10 repetitions:
				inserts the number in reverse order, from end to front*/
				temp = counter;
				/*finds amount of places in number*/
				while (counter>0) {
					tenCount++;
					counter=counter/10;
				}
				counter = 1;
				result[pholder]= curr;
				curr=target[i];
				/*inserts the number starting at the one's place and working backwards*/
				int z;
				for (z = tenCount; z>0; z--) {
					result[pholder + z] = (char) temp%10 + 48;
					temp=temp/10;
				}
				pholder = pholder+ tenCount +1;
			}
		}
	}
	/*printf("%d: \n", pholder);*/
	result[pholder] = '\0';
	/*printf("%s\n", result); */

	if (pholder<size) {
		return result;
	}

	return str;

}



 
int main (int argc, char** argv) {

	int size = 0;
	/*finds size of argument and also checks for a digit in the string
		prints error if it contains a digit and returns*/


	for (size=0; argv[1][size]!= '\0'; size++) {
		if (isdigit(argv[1][size])) {
			printf("error\n");
			return 0;
		}
	}
	size++;
	

	char * ptr = (char *)malloc(sizeof(char) * (size));
	
	/*puts the argument into ptr*/
	strcpy(ptr, argv[1]);

	/*puts the condensed argument into ptr*/
	ptr = condense (ptr, size);


	printf("%s\n", ptr);





	return 0;
}