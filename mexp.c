/*progrram that takes an input file with a matrix and
	multiplies that matrix to a given exponent*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int dimension=0; /*global variable for dimension of the matrix*/

/*a function that takes in 2 2D pointers, and multiplies them by each other. Works if the inputs follow
	the dimension restrictions of mtrix multiplcation*
			As this is a matrix exponentiation file, and the matrix is square, it should 
			always follow restrictions, and just multiplication should be done*/

int ** multiply (int ** matrixa, int** matrixb) {

	int total=0;

	/*allocate a product matrix*/
	int ** product = (int **)malloc(sizeof(int *) * dimension);
	int i;
	for (i =0; i<dimension; i++) {
		product[i] = (int *)malloc(sizeof(int)* dimension);
	}

	int y, p, x;
	for (y=0; y< dimension; y++) {//goes down first
		for (p=0; p< dimension; p++) {//goes across second
			for (x=0; x< dimension; x++)	{//goes across first
		
				total= total+ matrixa [y][x] * matrixb[x][p];
		
			}

			product[y][p] = total;
			total=0;
		
		}
	}

	free(matrixb);
		
	return product;
}





int main (int argc, char** argv) {

	/*opens a file from the argument*/
	FILE *fp;
	int size = 0;
	for (size=0; argv[1][size]!='\0'; size++) {
	};
	size++;

	char * filename = (char *)malloc(sizeof(char)*size);
	strcpy(filename, argv[1]);

	fp = fopen(filename, "r");



	/*if no file print error*/
	if (!fp) {
		printf("error\n");
		return 0;
	}

	/*put the dimension of the square matrix into the variable*/
	fscanf(fp, "%d\n", &dimension);
	

	/*allocate memory for a 2D matrix, both the original and current result*/
		/*while being multipled, the result holds the current answer, and matrix holds the original*/

	int ** matrix = (int ** )malloc(sizeof(int *) * dimension);
	int ** result = (int ** )malloc(sizeof(int *) * dimension);
	int i;
	for (i=0; i<dimension; i++) {
		result[i] = (int*)malloc(sizeof(int) * dimension);
		matrix[i] = (int*)malloc(sizeof(int) * dimension);
	}
	int j;
	/*read the file and put the data into the matrix*/
	for (i=0; i<dimension; i++){
		for (j=0; j<dimension; j++) {
			fscanf(fp, "%d", &matrix[i][j]);
			result[i][j] = matrix [i][j];
		}
	}

	/*read the power of the exponent*/
	int power =0;
	fscanf(fp, "%d", &power);

	/*if the zero power, return the identity matrix*/
	if (power==0) {
		for (i=0; i<dimension; i++){
			for (j=0; j<dimension; j++) {
				if (i==j) {
					result[i][j] = 1;
				}else {
					result[i][j] = 0;
				}
				
			}
		} 
	}

	/*call the multiply functon power-1 times*/
	for (i=1; i<power; i++) {

		result = multiply(matrix, result);	
	}
	free(matrix);



	/*print the result*/
	for (i=0; i<dimension; i++){
		for (j=0; j<dimension; j++) {
			printf("%d", result[i][j]);
			if (j<(dimension-1)) printf(" ");
		}
		printf("\n");
	}


	fclose(fp);

	return 0;
}