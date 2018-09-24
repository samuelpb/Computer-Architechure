/*FILE THAT TAKES IN A TRACE FILE AND SIMULATES A CACHE OR AN INDICATED SIZE, THEN USES EITHER FIFO
	OR LRU FOR REPLACEMENTS WITHIN A SET

	SIMULATES THE CACHE IN TWO WAYS: WITH AND WITHOUT PREFETCH
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/*data that is held in the sets
	linked lists are used*/
struct node {
	int num;
	long int tag;
	struct node * next;
	struct node * prev;
};

/*method to create a node to store a memory address within a set*/
struct node * createNode (int number, long int t, struct node * n, struct node * p) {

	struct node * ptr = (struct node *)malloc(sizeof(struct node));
	if (!ptr) return NULL;

	ptr->num = number;
	ptr->tag = t;
	ptr->next = n;
	ptr->prev = p;

	return ptr;

}

/*function that takes a number and then returns its base 2. Returns -1 if not a power of 2
	used to check cache and block size*/
int logTwo (int num) {

	int i=0;

	while (num>1) {
		if (num%2!=0) return -1;
		num = num/2;
		i++;
	}


	return i;
}


/*function that converts a string of a decimal number into an int
	used for the arguments of cache size and block size*/
int toString (char * string) {

	int i =0, j=0;
	int result =0;
	for (i=0; string[i]!='\0'; i++) {
	}

	for (j=0; j<i; j++) {
		result = result * 10;
		result +=  (int)(string[j]) - 48;
	}

	return result;

}

/*method to check if the memory address is already in the cahce.
	If it is it returns 0;
	If it is not, it then loads that node into the cache with that memory address, returns 1;
*/

int check (struct node ** cache, long int address, int policy, int bSize, int iSize, int lines, int pre) {

	if (cache == NULL) return 1;


	int res=1;
	long int tag, index, blockOffset;
	

	blockOffset = address%bSize;
	

	long int maybe = (address - blockOffset)%(iSize*bSize);
	index = maybe/bSize;

	tag = address-blockOffset-(index*iSize*bSize);

	int number = 0;
	int a = 0;

	/*index contains the set number, tag contains the size*/
	struct node * start = cache[index];
	struct node * ptr = cache[index];
	struct node * prev = NULL;
	struct node * trash;


	while (ptr!=NULL) {
		if (a!=1) {
			if (ptr->tag == tag) {
				if (policy==1) {
					return 0;
				}else if (pre!=1) {
					prev->next = ptr->next;
					if (ptr->next!=NULL) {
						struct node * t = ptr->next;
						t->prev = prev;
					}
					trash = ptr;
					a=1;
					trash->next = NULL;
				/*save it so it can be moved to the end of the list since it is the most recently used*/
				} else {
					return 0;
				}
			}
		}
		if (ptr->num>number) {
			number=ptr->num;
		}
		prev = ptr;
		ptr = ptr->next;
	}
	
	
	if (a==1 && pre!=1) {
		prev->next = trash;
		trash->prev = prev;
		trash->next = NULL;
		trash->num = number;
		return 0;
	}

	/*if it gets here, the address was not in the cache*/

	number++;

	struct node * temp = createNode(number, tag, NULL, prev);
	prev->next = temp;

	/*if the set is full, get rid of first item in the list*/
	if (number>lines) {
		trash = start->next;
		ptr = trash->next;
		start->next = ptr;
		ptr->prev = start;
		free(trash);
	}


	return res;

}

/*method to free the nodes in the cache in a set*/
void freeNode(struct node * ptr) {


	struct node * prev = ptr;

	/*go to end of list*/
	while(ptr!=NULL) {
		prev = ptr;
		ptr=ptr->next;
	}

	/*free the list backwards, so not too much in stack*/
	ptr = prev;
	prev = ptr->prev;

	while (ptr!=NULL) {
		free(ptr);
		ptr = prev;
		if (ptr!=NULL) {
			prev = ptr->prev;
		}
	}

}

/*driving method for freeNode, makes sure each set in the cache is hit*/

void freeAll(struct node ** cache, int size) {

	if (cache==NULL) return;

	int i=0;

	for (i=0; i<size; i++) {
		freeNode(cache[i]);
	}

	free(cache);


}




int main (int argc, char ** argv) {


	if (argc!=6) {
		printf("Error: invalid number or arguments.");
		return 1;
	} 

	


	int cSize=0, cPower =0, i=0, j=0, size=0;;
	int type; /* 0 for direct, 1 for full assoc, and n for assoc:n*/
	int policy; /* 1 for fifo, 2 for LRU*/
	int bSize=0, bPower = 0;
	int numSets, lines;

	for (size=0; argv[5][size]!='\0'; size++) {
	}size++;

	char * filename = (char *)malloc(sizeof(char *) * size);
	strcpy(filename, argv[5]);

	FILE * fp;
	fp = fopen(filename, "r");
	if (!fp) return 1;


	/*checks cache size and to make sure it is power of 2*/
	cSize = toString(argv[1]);
	cPower = logTwo(cSize);
	if (cPower == -1) return 1;

	/*checks block size and to make sure it is a power of 2*/
	bSize = toString(argv[4]);
	bPower = logTwo(bSize);
	if (bPower == -1) return 1;


	/*figures out the replacement policy*/
	if (strcmp("fifo", argv[3])==0) {
		policy =1;
	} else {
		policy=2;
	}

	/*figures out the type (direct/assoc)*/
	for (i=0; argv[2][i]!='\0'; i++) {}
	if (i==6) {
		type=0;
		numSets = cSize/bSize;
		lines = 1;
	}else if (i==5) {
		type=1;
		numSets = 1;
		lines = cSize/bSize;
	} else {
		int a = i-6;
		char * temp = (char *)malloc(sizeof(char) * a+1);
		for (j=6; argv[2][j]!='\0'; j++) {
			temp[j-6] = argv[2][j];
		}
		temp[j-6] = '\0';
		type = toString(temp);
		free(temp);
		if (logTwo(type)==-1) return -1;

		numSets = cSize/(bSize * type);
		lines = type;
	}

	/*int pIndex = logTwo(numSets);*/

	

	/*initializes all of the sets to have a null item at the beginning*/
	struct node ** regular = (struct node **) malloc (sizeof(struct node *) * numSets);
	struct node ** prefetch = (struct node **) malloc (sizeof(struct node *) * numSets);
	if (!regular || !prefetch) return 1;
	for (i=0; i<numSets; i++) {

		struct node * a = createNode(0, -1, NULL, NULL);
		struct node * b = createNode(0, -1, NULL, NULL);
		regular[i] = a;
		prefetch[i] = b;

	}

	int regHits=0, regMisses =0, regReads =0, regWrites = 0;
	int preHits =0, preMisses =0, preReads =0, preWrites =0;
	char * trash = (char *) malloc (sizeof(char)* 100);
	char instruction='a';
	long int address=0;


	

	/*reads the file and calls the regular cache and prefetch cache*/
	while (fscanf(fp, "%s ", trash)==1) {
		if (strcmp("#eof", trash)==0) break;
		if (fscanf(fp, "%c %lx", &instruction, &address)!=2) return 1;
		

		/*runs the address through the non prefetch cache*/
		if (check(regular, address, policy, bSize, numSets, lines, 0)==0) {
			regHits ++;
			if (instruction == 'W') {
				regWrites ++;
			}
		}else {
			regMisses ++;
			if (instruction == 'R') {
				regReads ++;
			}else {
				regReads ++;
				regWrites ++;
			}
		}

		/*runs it through the cache with prefetch*/
		if (check(prefetch, address, policy, bSize, numSets, lines, 0)==0) {
			preHits++;
			if (instruction == 'W') {
				preWrites ++;
			}
		}else {
			preMisses++;
			if (instruction == 'R') {
				preReads ++;
			}else {
				preReads++;
				preWrites ++;
			}
			if (check(prefetch, address+bSize, policy, bSize, numSets, lines, 1)==0) {
			}else {
				preReads ++;
			}
		}

	}

	printf("no-prefetch\n");
	printf("Cache hits: %d\n", regHits);
	printf("Cache misses: %d\n", regMisses);
	printf("Memory reads: %d\n", regReads);
	printf("Memory writes: %d\n", regWrites);
	printf("with-prefetch\n");
	printf("Cache hits: %d\n", preHits);
	printf("Cache misses: %d\n", preMisses);
	printf("Memory reads: %d\n", preReads);
	printf("Memory writes: %d\n", preWrites);


	freeAll(regular, numSets);
	freeAll(prefetch, numSets);
	fclose(fp);


	return 0;
}