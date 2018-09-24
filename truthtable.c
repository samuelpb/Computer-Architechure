/*TRUTHTABLE.C
	Takes in stdin or file readings as instructions for a truth table
	Stores instructions in nodes, and then solves all nodes (so they can be inserted in any order)
	Types:
		A(two inputs, one output): AND OR XOR NOT NAND
		B(one input, one output): NOT PASS
		MUX(1 output of 2^n bits, n selectors): MULTIPLEXER
		DEC(2^n outputs, n inputs): DECODER

	METHODS:
		createNodeA, createNodeB, createMuxNode, createDecNode
		freeNodes, freeTable, createTable, printTable, main
		solveNodeA, solveNodeB, solveMuxNode, solveDecNode, solveNode

	Prints:
		Truth Table corresponding to the instructions given in the file or stdin


*/





#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


/*struct that can hold the data from any line inputted
	AND, OR, XOR, NAND, NOR are type 1-5 (two arguments)
	NOT, PASS are type 6-7 (1 argument)
	MUX is type 8
	DECODER is type 9
	
	Therefore, depending on the type, I will have a method to initialize the node
	*/
/*adding some prototypes for functions that have to call another function below themselves*/
void solveNodeA();
void solveNodeB();
void solveMuxNode();
void solveDecNode();


struct node {

	char * outName;
	short function;
	char * inA;
	char * inB;
	
	/*arrays for decoders and multiplexers*/
	char ** inArray;

	
	char ** selectors;
	int n;
	
	short evaluated;

	struct node * next;

	short * output;

	/*used for decoders outputs*/
	short ** multOut;
	char ** decOut; /*names*/
};

/*method to create nodes that hold AND NOR NAND OR XOR */

struct node * createNodeA (char * name, short type, char * A, char * B, int rowNum) {

	struct node * ptr = (struct node * )malloc(sizeof(struct node));
	if (!ptr) return NULL;

	int size =0;

	/*copies in the name of what is produced (output/temp)*/
	for (size =0; name[size]!='\0'; size++) {
	}size++;

	ptr->outName = (char *)malloc(sizeof(char)*size);
	if (!ptr->outName) return NULL;
	strcpy(ptr->outName, name);


	/*copies in type*/
	ptr->function = type;

	/*copies in the first input name*/
	for (size =0; A[size]!='\0'; size++) {
	}size++;

	ptr->inA = (char *)malloc(sizeof(char)*size);
	if (!ptr->inA) return NULL;
	strcpy(ptr->inA, A);

	/*copies in the second input name*/
	for (size =0; B[size]!='\0'; size++) {
	}size++;

	ptr->inB = (char *)malloc(sizeof(char)*size);
	if (!ptr->inB) return NULL;
	strcpy(ptr->inB, B);

	/*sets all unneeded info to null*/
	ptr->inArray = NULL;
 	ptr->decOut = NULL;
 	ptr->selectors = NULL;
 	ptr->multOut=NULL;

 	/*sets that the node has not been evaluated*/
 	ptr->evaluated = 0;

 	/*sets the current output to 2, so know its wrong if it prints*/
 	ptr->output = (short *)malloc(sizeof(short) * rowNum);
 	if (!ptr->output) return NULL;
 	for (size=0; size<rowNum; size++) {
 		ptr->output[size] = 2;
 	}

 	ptr->next = NULL;
 	ptr->n=0;

 	return ptr;
}


/*method to create nodes 6 and 7 (NOT, PASS) */
struct node * createNodeB (char * name, short type, char * A, int rowNum) {

	struct node * ptr = (struct node * )malloc(sizeof(struct node));
	if (!ptr) return NULL;

	int size =0;

	/*copies in the name of what is produced (output/temp)*/
	for (size =0; name[size]!='\0'; size++) {
	}size++;

	ptr->outName = (char *)malloc(sizeof(char)*size);
	if (!ptr->outName) return NULL;
	strcpy(ptr->outName, name);


	/*copies in type*/
	ptr->function = type;

	/*copies in the first input name*/
	for (size =0; A[size]!='\0'; size++) {
	}size++;

	ptr->inA = (char *)malloc(sizeof(char)*size);
	if (!ptr->inA) return NULL;
	strcpy(ptr->inA, A);

	ptr->inB = NULL;
	ptr->inArray = NULL;
 	ptr->decOut = NULL;
 	ptr->selectors = NULL;
 	ptr->n=0;
 	ptr->multOut=NULL;

 	/*sets that the node has not been evaluated*/
 	ptr->evaluated = 0;

 	/*sets the current outputs to all 2s, so know its wrong if it prints*/
 	ptr->output = (short *)malloc(sizeof(short) * rowNum);
 	if (!ptr->output) return NULL;
 	for (size=0; size<rowNum; size++) {
 		ptr->output[size] = 2;
 	}

 	ptr->next = NULL;

 	return ptr;
}

/*method to create a node that hold MUX info*/
struct node * createMuxNode (char * out, char ** strings, char ** selectors, int rowNum, int n, int length) {

	struct node * ptr = (struct node *)malloc(sizeof(struct node));
	if (!ptr) return NULL;

	int i, j, size;

	for (i=0; out[i]!='\0'; i++) {
	}
	ptr->outName = (char *)malloc(sizeof(char)* i+1);
	if (!ptr->outName) return NULL;
	strcpy(ptr->outName, out);
	
	/*copies the selectors into the node*/
	ptr->selectors = (char **)malloc(sizeof(char *)* n);
	for (i=0; i<n; i++) {
		for (j=0; selectors[i][j]!='\0'; j++) {
		}
		ptr->selectors[i] = (char *)malloc(sizeof(char)* j +1);
		if (!ptr->selectors[i]) return NULL;
		strcpy(ptr->selectors[i], selectors[i]);
	}
	
	/*copies the strings into the node*/
	ptr->inArray = (char **)malloc(sizeof(char *)* length);
	for (i=0; i<length; i++) {
		for (j=0; strings[i][j]!='\0'; j++) {
		}
		ptr->inArray[i] = (char *)malloc(sizeof(char)* j +1);
		if (!ptr->inArray[i]) return NULL;
		strcpy(ptr->inArray[i], strings[i]);
	}

	ptr->function = 8;
	ptr->inA = NULL;
	ptr->inB = NULL;
	ptr->evaluated = 0;
	ptr->next=NULL;
	ptr->n = n;
	ptr->decOut = NULL;
	ptr->multOut=NULL;

	ptr->output = (short *)malloc(sizeof(short) * rowNum);
 	if (!ptr->output) return NULL;
 	for (size=0; size<rowNum; size++) {
 		ptr->output[size] = 3;
 	}

	return ptr;
}


/*method to create a node for a decoder*/
struct node * createDecNode (char * out, char ** outputs, char ** selectors, int rowNum, int n, int length) {

	struct node * ptr = (struct node *)malloc(sizeof(struct node));
	if (!ptr) return NULL;

	int i, j, size;

	for (i=0; out[i]!='\0'; i++) {
	}
	ptr->outName = (char *)malloc(sizeof(char)* i+1);
	if (!ptr->outName) return NULL;
	strcpy(ptr->outName, out);
	
	
	/*copies the selectors into the node*/
	ptr->selectors = (char **)malloc(sizeof(char *)* n);
	for (i=0; i<n; i++) {
		for (j=0; selectors[i][j]!='\0'; j++) {
		}
		ptr->selectors[i] = (char *)malloc(sizeof(char)* j +1);
		if (!ptr->selectors[i]) return NULL;
		strcpy(ptr->selectors[i], selectors[i]);
	}
	
	
	/*copies the strings into the node*/
	ptr->decOut = (char **)malloc(sizeof(char *)* length);
	for (i=0; i<length; i++) {
		
		for (size=0; outputs[i][size]!='\0'; size++){
		};
		
		ptr->decOut[i] = (char *)malloc(sizeof(char )* size+1);
		if (!ptr->decOut[i]) return NULL;
		strcpy(ptr->decOut[i], outputs[i]);
		
	}


	ptr->multOut = (short **) malloc(sizeof(short *) * length);
	if (!ptr->multOut) return NULL;
	for (i=0; i<length; i++) {
		ptr->multOut[i] = (short *) malloc(sizeof(short ) * rowNum);
		if (!ptr->multOut[i])return NULL;
		for (j=0; j<rowNum; j++) {
			ptr->multOut[i][j] = 3;
		}
	}


	ptr->function = 9;
	ptr->inA = NULL;
	ptr->inB = NULL;
	ptr->evaluated = 0;
	ptr->next=NULL;
	ptr->n = n;
	ptr->inArray = NULL;

	ptr->output = NULL;

	return ptr;

}


/*creates the truth table and initializes the input columns in the table*/
short ** createT (int numI, int numO, int rows) {

	short ** table = (short **) malloc (sizeof(short *) * rows);

	if (!table) return NULL;

	int i, j;

	for (i =0; i< (rows); i++) {
		table[i] = (short *) malloc (sizeof(short) * (numI + numO));
		if (!table[i]) return NULL;
	}

	/*init table to all 0s*/
	for (i=0; i<rows; i++) {
		for (j = (numI + numO -1); j>=0; j--) {

			table[i][j]=0;

		}
	}

	table[1][numI-1] = 1;

	/*init input colums to the correct order of 1s and 0s*/
	for (i=2; i<rows; i++) {
		for (j = (numI + numO -1); j>=0; j--) {

			if (j>=numI) {
				continue;
			}

			if (j==numI-1) {
				table[i][j] = table[i-1][j] + 1;
			} else if (j<numI -1){
				table[i][j] = table[i][j] + table[i-1][j];
			}

			if (table[i][j]==2) {
				table [i][j] = 0;
				table[i][j-1] ++;
			}
		}
	}

	return table;

}

/*prints the table with or without headers headers*/
void printTable (short ** table, int rows, int columns, int numIn, char ** inputs, char ** outputs, int headers) {

	int i, j, k;

	if (headers==1) {
		/*prints headers if needed*/
		for (i=0; i<columns; i++) {
			if (i<numIn) {
				printf("%s", inputs[i]);
			}else {
				printf("%s", outputs[i-numIn]);
			}

			if (i==numIn-1) {
				printf(" | ");
			}else if (i<columns-1) {
				printf(" ");
			}

		}
		printf("\n");
	}

	

	/*prints the body of the table*/
	for (i=0; i<rows; i++) {
		for (j=0; j<columns; j++) {
			/*adds spacing for the length of the variable name if needed*/
			if (headers==1) {
				if (j<numIn) {
					for (k=1; inputs[j][k]!='\0'; k++) {
						printf(" ");
					}
				} else {

					for (k=1; outputs[j-numIn][k]!='\0'; k++) {
						printf(" ");
					}
				}
			}

			printf("%d", table[i][j]);

			if (j==numIn-1) {
				printf(" | ");
			}else if (j<columns-1) {
				printf(" ");
			}

		}
		printf("\n");
	}

}

/*method to free all the nodes of the instructions stored*/
void freeNode (struct node * ptr) {

	if (ptr==NULL) return;

	if (ptr->next!=NULL) {
		freeNode(ptr->next);
	}


	int i, twoN=1;
	if (ptr->n!=0) {
		for (i=0; i<ptr->n; i++) {
			twoN = twoN *2;
		}
	}	



	if (ptr->outName!=NULL) {
		free(ptr->outName);
	}	
	if (ptr->inA!=NULL) {
		free(ptr->inA);
	}
	if (ptr->inB!=NULL) {
		free(ptr->inB);
	}
	if (ptr->inArray!=NULL) {
		for (i=0; i<twoN; i++) {
			if (ptr->inArray[i]!=NULL){
				free(ptr->inArray[i]);
			}
		}
		free(ptr->inArray);
	}
	
	

	/*arrays for decoders and multiplexers*/
	if (ptr->decOut!=NULL){
		for (i=0; i<ptr->n; i++) {
			free(ptr->decOut[i]);
		}
		free(ptr->decOut);
	}
	
	if (ptr->selectors!=NULL) {
		for (i=0; i<ptr->n; i++) {
			if (ptr->selectors[i]!=NULL){
				free(ptr->selectors[i]);
			}
		}
		free(ptr->selectors);
	}
	
	if (ptr->output!=NULL) {
		free(ptr->output);
	}

	if (ptr->multOut!=NULL) {
		for (i=0; i<twoN; i++) {
			free(ptr->multOut[i]);
		}
		free(ptr->multOut);
	}



}

/*method to free the table*/
void freeTable (short ** table, int rows) {


	int i;
	for (i=0; i<rows; i++) {
		free(table[i]);
	}
	free(table);

}


/*method to do the calculations for every row for a articular node of tye 1-5 (AND OR XOR NAND NOR)*/
void solveNodeA (struct node * ptr, char ** inputs, char ** outputs, int nIn, int nOut, int rowNum, short ** table, struct node * start) {

	
	if (ptr==NULL ) return;

	int columnA=-1, columnB=-1, i;
	short * rowA=NULL; short * rowB=NULL;

	if (strcmp(ptr->inA, "1")==0) {
		rowA = (short *) malloc(sizeof(short)* rowNum);
		for (i=0; i<rowNum; i++) {
			rowA[i]=1;
		}
		
		columnA = -2;
	} else if (strcmp(ptr->inA, "0")==0) {
		rowA= (short *) malloc(sizeof(short)*rowNum);
		for (i=0; i<rowNum; i++) {
			rowA[i]=0;
		}
		columnA = -2;
	}

	if (strcmp(ptr->inB, "0")==0) {
		rowB= (short *) malloc(sizeof(short)*rowNum);
		for (i=0; i<rowNum; i++) {
			rowB[i]=0;
		}
		columnB = -2;
	}else if (strcmp(ptr->inB, "1")==0) {
		rowB= (short *) malloc(sizeof(short)*rowNum);
		for (i=0; i<rowNum; i++) {
			rowB[i]=1;
		}
		columnB = -2;
	}


	if (!rowA || !rowB){
		for (i=0; i<nIn; i++) {
			if (strcmp(ptr->inA, inputs[i])==0) {
				columnA = i;
			}
			if (strcmp(ptr->inB, inputs[i])==0) {
				columnB = i;
			}
		}
	}



	/*if the inputs for the instruction are not inputs, check to see if they are solves, if not solve them*/
	if (columnA == -1) {
		struct node * curr = start;
		while (curr != NULL) {
			/*if already solved, pull out its outputs*/
			if (strcmp(curr->outName, ptr->inA) == 0) {
				if (curr->evaluated==1) {
					rowA = curr->output;
					break;
				}else {
					if (curr->function <= 5) {
						solveNodeA(curr, inputs, outputs, nIn, nOut, rowNum, table, start);
						rowA = curr->output;
						break;
					}else if (curr->function <=7) {
						solveNodeB(curr, inputs, outputs, nIn, nOut, rowNum, table, start);
						rowA = curr->output;
						break;
					} else if (curr->function == 8) {
						solveMuxNode(ptr, inputs, outputs, nIn, nOut, rowNum, table, start);
						rowA = curr->output;
						break;
					}
				}

			/*check all ouputs of decoder nodes*/
			}else if (strcmp(curr->outName, "DECODER")==0) {
				if (curr == ptr) {
					curr=curr->next;
					continue;
				}
				int twoN = 1;
				for (i=0; i<curr->n; i++) {
					twoN= twoN*2;
				}
				for (i=0; i<twoN; i++) {
					if (strcmp(ptr->inA, curr->decOut[i])==0){
						solveDecNode(curr, inputs, outputs, nIn, nOut, rowNum, table, start);
						rowA = curr->multOut[i];
						break;
					}
				}

			}
			curr=curr->next;
		}
	} else if (columnA!=-2) {
		rowA = (short *)malloc(sizeof(short)* rowNum);
		for (i=0; i<rowNum; i++) {
			rowA[i]= table[i][columnA];
		}
	}


	/*checks the same for the second arguement*/
	if (columnB == -1) {
		struct node * curr = start;
		while (curr != NULL) {
			/*if already solved, pull out its outputs*/
			if (strcmp(curr->outName, ptr->inB) == 0) {
				if (curr->evaluated==1) {
					rowB = curr->output;
					for (i=0; i<rowNum; i++) {
					}
					break;
				}else {
					if (curr->function <= 5) {
						solveNodeA(curr, inputs, outputs, nIn, nOut, rowNum, table, start);
						rowB = curr->output;
						break;
					}else if (curr->function <=7) {
						solveNodeB(curr, inputs, outputs, nIn, nOut, rowNum, table, start);
						rowB = curr->output;
						break;
					} else if (curr->function == 8) {
						solveMuxNode(ptr, inputs, outputs, nIn, nOut, rowNum, table, start);
						rowB = curr->output;
						break;
					}
				}
				
			}else if (strcmp(curr->outName, "DECODER")==0) {
				if (curr == ptr) {
					curr=curr->next;
					continue;
				}
				int twoN = 1;
				for (i=0; i<curr->n; i++) {
					twoN= twoN*2;
				}
				for (i=0; i<twoN; i++) {
					if (strcmp(ptr->inB, curr->decOut[i])==0){
						solveDecNode(curr, inputs, outputs, nIn, nOut, rowNum, table, start);
						rowB = curr->multOut[i];
						break;
					}
				}

			}


			curr=curr->next;
		}
	} else if (columnB!=-2) {
		rowB = (short *)malloc(sizeof(short)* rowNum);
		for (i=0; i<rowNum; i++) {
			rowB[i]= table[i][columnB];
		}
	}

	/*NOW A SECTION TO ACTUALLY DO THE MATH AND CREATE THE OUTPUT ROW*/

	for (i =0; i<rowNum; i++) {
		if (ptr->function==1) {
			int y = rowA[i] + rowB[i];
			if (y==2) {
				ptr->output[i]=1;
			}else {
				ptr->output[i]=0;
			}
		} else if (ptr->function==2) {
			int y = rowA[i] + rowB[i];
			if (y>0) {
				ptr->output[i]=1;
			}else {
				ptr->output[i]=0;
			}
			
		} else if (ptr->function==3) {
			int y = rowA[i] + rowB[i];
			if (y==1) {
				ptr->output[i]=1;
			}else {
				ptr->output[i]=0;
			}
		} else if (ptr->function==4) {
			int y = rowA[i] + rowB[i];
			if (y==2) {
				ptr->output[i]=0;
			}else {
				ptr->output[i]=1;
			}
		} else {
			int y = rowA[i] + rowB[i];
			if (y>0) {
				ptr->output[i]=0;
			}else {
				ptr->output[i]=1;
			}
		}

	}
	ptr->evaluated = 1;

	int j=0;


	/*updates the table if the node was of an output column in the final table*/
	for (i=0; i<nOut; i++) {
		if (strcmp(ptr->outName, outputs[i])==0) {
			for (j=0; j<rowNum; j++) {
				table[j][nIn + i] = ptr->output[j];
			}
			break;
		}
	}


}


/*method to do the calculations for every row for a articular node of tye 1-5 (AND OR XOR NAND NOR)*/
void solveNodeB (struct node * ptr, char ** inputs, char ** outputs, int nIn, int nOut, int rowNum, short ** table, struct node * start) {

	if (ptr==NULL ) return;

	int columnA=-1;
	short * rowA=NULL; 
	int i;

	if (strcmp(ptr->inA, "1")==0) {
		rowA= (short *) malloc(sizeof(short)*rowNum);
		for (i=0; i<rowNum; i++) {
			rowA[i]=1;
		}
		columnA = -2;
	} else if (strcmp(ptr->inA, "0")==0) {
		rowA= (short *) malloc(sizeof(short)*rowNum);
		for (i=0; i<rowNum; i++) {
			rowA[i]=0;
		}
		columnA = -2;
	}
	if (!rowA) {
	for (i=0; i<nIn; i++) {
			if (strcmp(ptr->inA, inputs[i])==0) {
				columnA = i;
			}
		}
	}

	/*if the inputs for the instruction are not inputs, check to see if they are solves, if not solve them*/
	if (columnA == -1) {
		struct node * curr = start;
		while (curr != NULL) {
			/*if already solved, pull out its outputs*/
			if (strcmp(curr->outName, ptr->inA) == 0) {
				if (curr->evaluated==1) {
					rowA = curr->output;
					break;
				}else {
					if (curr->function <= 5) {
						solveNodeA(curr, inputs, outputs, nIn, nOut, rowNum, table, start);
						rowA = curr->output;
						break;
					}else if (curr->function <=7) {
						solveNodeB(curr, inputs, outputs, nIn, nOut, rowNum, table, start);
						rowA = curr->output;
						break;
					} else if (curr->function == 8) {
						solveMuxNode(ptr, inputs, outputs, nIn, nOut, rowNum, table, start);
						rowA = curr->output;
						break;
					}
					
				}
				
			}else if (strcmp(curr->outName, "DECODER")==0) {
				if (curr == ptr) {
					curr=curr->next;
					continue;
				}
				int twoN = 1;
				for (i=0; i<curr->n; i++) {
					twoN= twoN*2;
				}
				for (i=0; i<twoN; i++) {
					if (strcmp(ptr->inA, curr->decOut[i])==0){
						solveDecNode(curr, inputs, outputs, nIn, nOut, rowNum, table, start);
						rowA = curr->multOut[i];
					}
				}

			}


			curr=curr->next;
		}
	} else if (columnA != -2) {
		rowA = (short *)malloc(sizeof(short)* rowNum);
		for (i=0; i<rowNum; i++) {
			rowA[i]= table[i][columnA];
		}
	}


	
	/*NOW A SECTION TO ACTUALLY DO THE MATH AND CREATE THE OUTPUT ROW*/

	for (i =0; i<rowNum; i++) {
		if (ptr->function==6) {
			if (rowA[i]==1) {
				ptr->output[i]=0;
			}else {
				ptr->output[i]=1;
			}
		} else  {
			if (rowA[i]==1) {
				ptr->output[i]=1;
			}else {
				ptr->output[i]=0;
			}
		}
	}
	ptr->evaluated = 1;

	int j=0;


	/*updates the table if the node was of an output column in the final table*/
	for (i=0; i<nOut; i++) {
		if (strcmp(ptr->outName, outputs[i])==0) {
			for (j=0; j<rowNum; j++) {
				table[j][nIn + i] = ptr->output[j];
				
			}
			break;
		}
	}

}


/*method to do the math for a multiplexer instuction*/
void solveMuxNode (struct node * ptr, char ** inputs, char ** outputs, int nIn, int nOut, int rows, short ** table, struct node * start) {

	if (ptr==NULL) return;


	int twoN = 1, i, j, k, check=0;;
	for (i=0; i<ptr->n; i++) {
		twoN = twoN *2;
	}


	short ** selectorColumns = (short **)malloc(sizeof(short *) * ptr->n);
	if (!selectorColumns) return;

	short** stringColumns = (short **)malloc(sizeof(short *) * twoN);
	if (!stringColumns) return;
	for (i=0; i<twoN; i++) {

		if (i<ptr->n) {
			selectorColumns[i] = (short * )malloc(sizeof(short) * rows);
			if (!selectorColumns[i]) return;
		}

		stringColumns[i] = (short *) malloc(sizeof(short) * rows);
		if (!stringColumns[i]) return;
	}
	for (i=0; i<twoN; i++) {
		for (j=0; j<rows; j++) {
			if (i<ptr->n) {
				selectorColumns[i][j]=2;
			}
			stringColumns[i][j]=2;
		}
	}
	
	/*checks to make sure each of the string output determiners are solved*/
		/*also copies the values for each item at each row into arrays (one for selectors, one for input string)*/
	for (i=0; i<twoN; i++ ) {
		check=0;
		if (strcmp(ptr->inArray[i], "1")==0) {
			for (j=0; j<rows; j++) {
				stringColumns[i][j]= 1;
			}
			continue;
		} else if (strcmp(ptr->inArray[i], "0")==0) {
			for (j=0; j<rows; j++) {
				stringColumns[i][j]= 0;
			}
			continue;
		} 
		for (j=0; j<nIn; j++) {
			if (strcmp(inputs[j], ptr->inArray[i])==0) {
				check = 1;
				for (k=0; k<rows; k++) {
					stringColumns[i][k]= table[k][j];
				}
			}
		}
		if (check==1) continue;

		
		/*if not a 1, 0, o input, check to make sure it is solved. if not, solve it*/
		struct node * curr = start;

		while (curr != NULL) {
			/*if already solved, pull out its outputs*/
			if (strcmp(curr->outName, ptr->inArray[i]) == 0) {
				if (curr->evaluated==1) {
					for (j=0; j<rows; j++) {
						stringColumns[i][j]= curr->output[j];
					}
					break;
				}else {
					if (curr->function <= 5) {
						solveNodeA(curr, inputs, outputs, nIn, nOut, rows, table, start);
						for (j=0; j<rows; j++) {
							stringColumns[i][j]= curr->output[j];
						}
						break;
						
					}else if (curr->function <=7) {
						solveNodeB(curr, inputs, outputs, nIn, nOut, rows, table, start);
						for (j=0; j<rows; j++) {
							stringColumns[i][j]= curr->output[j];
						}
						break;
					} else if (curr->function == 8) {
						solveMuxNode(curr, inputs, outputs, nIn, nOut, rows, table, start);
						for (j=0; j<rows; j++) {
							stringColumns[i][j]= curr->output[j];
						}
						break;
					}	 
				}
			}else if (strcmp(curr->outName, "DECODER")==0) {
				if (curr == ptr) {
					curr=curr->next;
					continue;
				}
				int square = 1;
				for (j=0; j<curr->n; j++) {
					square = square*2;
				}
				for (j=0; j<square; j++) {
					if (strcmp(ptr->inArray[i], curr->decOut[j])==0){
						solveDecNode(curr, inputs, outputs, nIn, nOut, rows, table, start);
						stringColumns[i] = curr->multOut[j];
					}
				}

			}
			curr=curr->next;
		}
	}


	
	/*does the same thing again for the selectors*/
	for (i=0; i<ptr->n; i++ ) {
		check=0;
		if (strcmp(ptr->selectors[i], "1")==0) {
			for (j=0; j<rows; j++) {
				
				selectorColumns[i][j]= 1;
			}
			continue;
		} else if (strcmp(ptr->selectors[i], "0")==0) {
			for (j=0; j<rows; j++) {
				
				selectorColumns[i][j]= 0;
			}
			continue;
		} 
		for (j=0; j<nIn; j++) {
			if (strcmp(inputs[j], ptr->selectors[i])==0) {
				for (k=0; k<rows; k++) {
					selectorColumns[i][k]= table[k][j];
				}
			continue;
			}
		}

		

		/*if not a 1, 0, o input, check to make sure it is solved. if not, solve it*/
		struct node * curr = start;
		while (curr != NULL) {
			
			/*if already solved, pull out its outputs*/
			if (strcmp(curr->outName, ptr->selectors[i]) == 0) {
				if (curr->evaluated==1) {
					for (j=0; j<rows; j++) {
						selectorColumns[i][j]= curr->output[j];
					}
					break;
				}else {
					if (curr->function <= 5) {
						solveNodeA(curr, inputs, outputs, nIn, nOut, rows, table, start);
						for (j=0; j<rows; j++) {
							selectorColumns[i][j]= curr->output[j];
						}
						break;
						
					}else if (curr->function <=7) {
						solveNodeB(curr, inputs, outputs, nIn, nOut, rows, table, start);
						for (j=0; j<rows; j++) {
							selectorColumns[i][j]= curr->output[j];
						}
						break;
					} else if (curr->function == 8) {
						solveMuxNode(curr, inputs, outputs, nIn, nOut, rows, table, start);
						for (j=0; j<rows; j++) {
							selectorColumns[i][j]= curr->output[j];
						}
						break;
					}	
					
				}
				
			}else if (strcmp(curr->outName, "DECODER")==0) {
				if (curr == ptr) {
					curr=curr->next;
					continue;
				}
				int square= 1;
				for (j=0; j<curr->n; j++) {
					square = square*2;
				}
				for (j=0; j<square; j++) {
					if (strcmp(ptr->selectors[i], curr->decOut[j])==0){
						solveDecNode(curr, inputs, outputs, nIn, nOut, rows, table, start);
						selectorColumns[i] = curr->multOut[j];
					}
				}

			}

			curr=curr->next;
		}
	}
	

	/*actual math for the multiplexer
		goes through, finds which entry to select based on the selectors, and then returns that input*/
	int entry=0;
	for (i=0; i<rows; i++) {
		entry = 0;
		/*picks entry from selector*/
		for (j=0; j<ptr->n; j++) {
			
			entry = entry*2;
			if (selectorColumns[j][i] == 1) {
				entry += 1;
			}
		}
		/*evaluates entry*/
		
		ptr->output[i] = stringColumns[entry][i];
	}

	ptr->evaluated = 1;


	for (i=0; i<nOut; i++) {
		if (strcmp(ptr->outName, outputs[i])==0) {
			for (j=0; j<rows; j++) {
				table[j][nIn + i] = ptr->output[j];
				
			}
			break;
		}
	} 
	
	

}

/*function to evaluate a dec node for each row*/
void solveDecNode (struct node * ptr, char ** inputs, char ** outputs, int nIn, int nOut, int rows, short ** table, struct node * start) {

	if (ptr==NULL) return;


	int twoN = 1, i, j, k, stop=1;;
	for (i=0; i<ptr->n; i++) {
		twoN = twoN *2;
		stop=stop*2;
	}

	




	short ** selectorColumns = (short **)malloc(sizeof(short *) * ptr->n);
	if (!selectorColumns) return;

	for (i=0; i<ptr->n; i++) {
		selectorColumns[i] = (short * )malloc(sizeof(short) * rows);
		if (!selectorColumns[i]) return;
	}
	for (i=0; i<ptr->n; i++) {
		for (j=0; j<rows; j++) {
			selectorColumns[i][j]=17;
		}
	}

	
	/*checks to make sure if the inputs for the decoder are already solved*/
	for (i=0; i<ptr->n; i++ ) {
		
		if (strcmp(ptr->selectors[i], "1")==0) {
			for (j=0; j<rows; j++) {
				
				selectorColumns[i][j]= 1;
			}
			continue;
		} else if (strcmp(ptr->selectors[i], "0")==0) {
			for (j=0; j<rows; j++) {
				
				selectorColumns[i][j]= 0;
			}
			continue;
		} 
		for (j=0; j<nIn; j++) {
			if (strcmp(inputs[j], ptr->selectors[i])==0) {
				for (k=0; k<rows; k++) {
					selectorColumns[i][k]= table[k][j];
				}
			continue;
			}
		}

		

		/*if not a 1, 0, o input, check to make sure it is solved. if not, solve it*/
		struct node * curr = start;
		while (curr != NULL) {
			
			/*if already solved, pull out its outputs*/
			if (strcmp(curr->outName, ptr->selectors[i]) == 0) {
				if (curr->evaluated==1) {
					for (j=0; j<rows; j++) {
						selectorColumns[i][j]= curr->output[j];
					}
					break;
				}else {
					if (curr->function <= 5) {
						solveNodeA(curr, inputs, outputs, nIn, nOut, rows, table, start);
						for (j=0; j<rows; j++) {
							selectorColumns[i][j]= curr->output[j];
						}
						break;
						
					}else if (curr->function <=7) {
						solveNodeB(curr, inputs, outputs, nIn, nOut, rows, table, start);
						for (j=0; j<rows; j++) {
							selectorColumns[i][j]= curr->output[j];
						}
						break;
					} else if (curr->function == 8) {
						solveMuxNode(curr, inputs, outputs, nIn, nOut, rows, table, start);
						for (j=0; j<rows; j++) {
							selectorColumns[i][j]= curr->output[j];
						}
						break;
					}
				}
				
			}else if (strcmp(curr->outName, "DECODER")==0) {
				if (curr == ptr) {
					curr=curr->next;
					continue;
				} 
				int square = 1;
				for (j=0; j<curr->n; j++) {
					square= square*2;
				}
				for (j=0; j<square; j++) {
					if (strcmp(ptr->selectors[i], curr->decOut[j])==0){
						solveDecNode(curr, inputs, outputs, nIn, nOut, rows, table, start);
						selectorColumns[i] = curr->multOut[j];
					}
				}

			}

			curr=curr->next;
		}
	}


	

	/*the math for the decoder*/
	
	int entry=0;

	for (i=0; i<rows; i++) {
		entry = 0;
		
		/*picks entry from selector*/
		for (j=0; j<ptr->n; j++) {
			
			entry = entry*2;
			if (selectorColumns[j][i] == 1) {
				entry += 1;
			}
		}
		/*evaluates entry*/
		
		/*ptr->output[i] = ptr->decOut[entry];*/


		for (k=0; k<stop; k++) {
			
			ptr->multOut[k][i] = 0;
			
			if (k==entry) {
				ptr->multOut[k][i]=1;
			}
			
		}
	
	}

	

	ptr->evaluated = 1;

	/*copies the outputs into the table if they are final outputs*/
	for (i=0; i<nOut; i++) {
		for (k=0; k<twoN; k++) {
			if (strcmp(ptr->decOut[k], outputs[i])==0) {
				for (j=0; j<rows; j++) {
					table[j][nIn + i] = ptr->multOut[k][j];
				}
				break;
			}
		}
	} 


}

/*takes the list of math instructions and evaluates each*/
void solveNode (struct node * start, char ** inputs, char ** outputs, int nIN, int nOut, int rowNum, short ** table) {

	
	struct node * ptr = start;
	if (ptr==NULL) return;

	while (ptr!=NULL) {

		

		if (ptr->evaluated == 1) {
			ptr = ptr->next;
			continue;
		} else if (ptr->function <=5 ) {
			solveNodeA(ptr, inputs, outputs, nIN, nOut, rowNum, table, start);
		} else if (ptr->function <=7) {
			solveNodeB(ptr, inputs, outputs, nIN, nOut, rowNum, table, start);
		}else if (ptr->function==8) {
			solveMuxNode(ptr, inputs, outputs, nIN, nOut, rowNum, table, start);
		} else {
			solveDecNode(ptr, inputs, outputs, nIN, nOut, rowNum, table, start);
		}

		
	
		ptr = ptr->next;


	}


}







int main (int argc, char ** argv) {

	int headers = 0, readFile = 0; 
	short ** table =NULL;
	int numIn = 0, numOut = 0, rows = 1; 
	int i, j, rCount;
	char ** inputs = NULL;
	char ** outputs = NULL;
	struct node * start = NULL;
	struct node * curr = start;
	char * read = (char *) malloc (sizeof(char) * 300);
	if (!read) return 1;




	/*figures out if you read from stdin, or if you read from file
		figures out if there are headers */

	if (argc==1) {
	
	} else if (argc==2) {
		if (strcmp("-h", argv[1])==0) {
			headers=1;
		} else {
			readFile=1;
		}
	
	} else {
		readFile=1;
		headers=1;
	}

	FILE * fp;
	/*section to read the file*/
	if (readFile==1) {
			if (argc==2) {
				fp = fopen(argv[1], "r");
			}else {
				fp = fopen(argv[2], "r");
			}
	}else {
		fp = stdin;
	}

			
			if (!fp) return 1;

	

			/* Section to read input line and make an array of input names*/
			if (fscanf(fp, "%s %d", read, &numIn)!=2) return 1;
			if (strcmp(read, "INPUT")!=0) return 1;


			/*puts input names into array*/
			inputs = (char ** )malloc(sizeof(char *) * numIn);


			for (i=0; i<numIn; i++) {
				if (fscanf(fp, "%s ", read) !=1) return 1;
				if (strcmp(read, ":")==0) {
					if (fscanf(fp, "%s ", read) !=1) return 1;
				}
				for (j=0; read[j]!='\0'; j++) {
					continue;
				}
				inputs[i] = (char *)malloc(sizeof(char *) * j+1 );
				strcpy(inputs[i], read);
			}


			/*Section to read Output names*/
			if (fscanf(fp, "%s %d", read, &numOut)!=2) return 1;
			if (strcmp(read, "OUTPUT")!=0) return 1;


			/*puts input names into array*/
			outputs = (char **)malloc(sizeof(char *) * numOut);
			for (i=0; i<numOut; i++) {
				if (fscanf(fp, "%s ", read) !=1) return 1;
				if (strcmp(read, ":")==0) {
					if (fscanf(fp, "%s ", read) !=1) return 1;
				}
				for (j=0; read[j]!='\0'; j++) {
					continue;
				}
				outputs[i] = (char *)malloc(sizeof(char *) * j+1 );
				strcpy(outputs[i], read);

			}

			for (rCount=0; rCount<numIn; rCount++) {
				rows = rows*2;
			}

			table = createT(numIn, numOut, rows);



			/*reads line by line creating a node for each line (each math instruction)*/
			while (fscanf(fp, "%s ", read)==1) {

				/*SECTION IF AND OR NAND NOR XOR are read*/
				/*if type1-5, create the node*/
				if (strcmp(read, "AND")==0 || strcmp(read, "OR")==0 || 
					strcmp(read, "XOR")==0 || strcmp(read, "NOR")==0 || 
					strcmp (read, "NAND") ==0) {


					int type; char * B = (char *)malloc(sizeof(char) * 300);
					char * out = (char *) malloc (sizeof(char) * 300);

					if (!B || !out) return 1;


					if (strcmp(read, "AND")==0) {
						type=1;
					} else if (strcmp(read, "OR")==0) {
						type = 2;
					} else if (strcmp(read, "XOR")==0) {
						type=3;
					}else if (strcmp(read, "NAND")==0) {
						type=4;
					}else {
						type=5;
					}


					if (fscanf(fp, "%s %s %s", read, B, out)!=3) return 1;
					if (strcmp(out, ":")==0) {
						if (fscanf(fp, "%s ", out)!=1) return 1;
					}

					struct node * ptr = createNodeA(out, type, read, B, rows);
					if (!ptr) return 1;
					if (start==NULL) {
						start = ptr;
						curr = start;
					} else {
						curr->next =ptr;
						curr = curr->next;
					}

					free(out);
					free(B);


				/*SECTION IF NOT PASS are read*/
				/*if type 6 or 7*/
				} else if (strcmp (read, "NOT")==0 || strcmp(read, "PASS")==0) {

					int type; char * out = (char *)malloc(sizeof(char) * 300);
					if (!out) return 1;

					

					if (strcmp(read, "NOT")==0) {
						type= 6;
					} else {
						type = 7;
					}


					if (fscanf(fp, "%s %s", read, out)!=2) return 1;
					if (strcmp(out, ":")==0) {
						if (fscanf(fp, "%s ", out)!=1) return 1;
					}

					struct node * ptr = createNodeB(out, type, read, rows);
					if (!ptr) return 1;
					if (start==NULL) {
						start = ptr;
						curr = start;
					} else {
						curr->next = ptr;
						curr = curr->next;
					}

					free(out);


				/*SECTION IF A MULTIPLEXER LINE IS READ*/
				/*if type 8*/
				} else if (strcmp(read, "MULTIPLEXER")==0) {

				
					char * out = (char *)malloc(sizeof(char) * 300);
					int n, length=1, j;
					
					if (fscanf(fp, " %d ", &n) != 1) return 1;

					for (j=0; j<n; j++) {
						length = length*2;
					};
				
					/*create something to hold the string of outputs*/
					char ** strings = (char ** ) malloc(sizeof(char *) * length);
					if (!strings)return 1;
					for (j=0; j<length; j++) {
						strings[j] = (char *)malloc(sizeof(char) * 100);
						if (!strings[j])return 1;
					}

				
					/*copy the output options into strings*/
					if (fscanf(fp, "%s ", read) != 1) return 1;
					if (strcmp(read, ":")==0) {
						if (fscanf(fp, "%s ", read) != 1) return 1;
					}
					strcpy(strings[0], read);
					for (j=1; j<length; j++) {
					if (fscanf(fp, "%s ", read) != 1) return 1;
						strcpy(strings[j], read);
					}

					/*create something to hold the selectors*/
					char ** selectors = (char ** ) malloc(sizeof(char * )*n);
					if (!selectors)return 1;
					for (j=0; j<n; j++) {
						selectors[j] = (char *)malloc(sizeof(char) * 100);
						if (!selectors[j])return 1;
					}
				
					/*copy the selctors into selectors*/
					if (fscanf(fp, "%s ", read) != 1) return 1;
					if (strcmp(read, ":")==0) {
						if (fscanf(fp, "%s ", read) != 1) return 1;
					}
					strcpy(selectors[0], read);
					for (j=1; j<n; j++) {
						if (fscanf(fp, "%s ", read) != 1) return 1;
						strcpy(selectors[j], read);
					}
					
				/*copy the output name into out*/
					if (fscanf(fp, "%s ", read) != 1) return 1;
					if (strcmp(read, ":")==0) {
						if (fscanf(fp, "%s ", read) != 1) return 1;
					}
					strcpy(out, read);


					struct node * ptr = createMuxNode(out, strings, selectors, rows, n, length);
					if (!ptr) return 1;

					for (j=0; j<length; j++) {
						if (j<n) {
							free(selectors[j]);
						}
						free(strings[j]);
					}
					free(selectors);
					free(out);
					free(strings);

					if (start==NULL) {
						start = ptr;
						curr = start;
					} else {
						curr->next =ptr;
						curr = curr->next;
					}

				/*if type 9 (decoder)*/
				} else {


					/*char * out = (char *)malloc(sizeof(char) * 300);*/
					int n, length=1, j;
					
					if (fscanf(fp, "%d ", &n) != 1) return 1;
					

					for (j=0; j<n; j++) {
						length = length*2;
					};
			
				
					/*create something to hold the array of outputs as shorts*/
					char ** output = (char ** ) malloc(sizeof(char *) * length);
					if (!output)return 1;
					for (j=0; j<length; j++) {
						output[j] = (char * ) malloc(sizeof(char *) * 100);
						if (!output[j] )return 1;
					}

				
				
				
					/*create something to hold the selectors(inputs)*/
					char ** selectors = (char ** ) malloc(sizeof(char * ) * n);
					if (!selectors)return 1;
					for (j=0; j<n; j++) {
						selectors[j] = (char *)malloc(sizeof(char) * 100);
						if (!selectors[j])return 1;
					}
				
					/*copy the selctors into selectors*/
					if (fscanf(fp, "%s ", read) != 1) return 1;
					if (strcmp(read, ":")==0) {
						if (fscanf(fp, "%s ", read) != 1) return 1;
					}
					strcpy(selectors[0], read);
					for (j=1; j<n; j++) {
						if (fscanf(fp, "%s ", read) != 1) return 1;
						strcpy(selectors[j], read);
					}

					

					/*copy the output options into output*/
					if (fscanf(fp, "%s ", read) != 1) return 1;
					if (strcmp(read, ":")==0) {
						if (fscanf(fp, "%s ", read) != 1) return 1;
					}
					strcpy(output[0], read);
					for (j=1; j<length; j++) {
						if (fscanf(fp, "%s ", read) != 1) return 1;
						strcpy(output[j], read);
					}
				
					/*copy the output name into out
					if (fscanf(fp, "%s ", read) != 1) return 1;
					if (strcmp(read, ":")==0) {
						if (fscanf(fp, "%s ", read) != 1) return 1;
					}
					strcpy(out, read);*/

					/*isn't an output name so I am confused here but I'll print the output of the decoder
					for now*/

					

					struct node * ptr = createDecNode("DECODER", output, selectors, rows, n, length);
					if (!ptr) return 1;

					for (j=0; j<n; j++) {
						free(selectors[j]);
					}
					free(selectors);
					free(output);

					if (start==NULL) {
						start = ptr;
						curr = start;
					} else {
						curr->next =ptr;
						curr = curr->next;
					}




				}
			}


	if (curr!=NULL){
		curr->next = NULL;
	}


	solveNode(start, inputs, outputs, numIn, numOut, rows, table);
	

	printTable(table, rows, (numIn+numOut), numIn, inputs, outputs, headers);

	freeNode(start);
	freeTable(table, rows);


	return 0;
}