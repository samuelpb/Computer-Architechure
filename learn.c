/*program that takes in a solved data file and learns the variables that determine the price of a house
	it then uses those variables to predict the prices of houses in the next file*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/*METHODS:
	transpose
	invert
		-add row
		-multiply row
	multiply
	free matrix
*/


/*method to divide a row of a matrix all by a nuber given*/
void divideRow (double ** matrix, int row, double factor, int columns) {

	if (matrix == NULL) return;

	int i;
	for (i=0; i<columns; i++) {
		matrix[row][i] = matrix[row][i]/factor;
	}



}
 
 /*method that subtracts  row A (times the multiplier) from row B*/
void subtractRow (double ** matrix, int A, int B, double multiplier, int columns) {

	if (matrix==NULL) return;
	int i;
	for (i = 0; i<columns; i++) {
		matrix[B][i] = (matrix[B][i] - (multiplier * matrix[A][i]));
	}
}

/*method to invert a given square matrix*/
double ** invert (double ** matrix, int row, int column) {

	if (row!=column) return NULL;
	if (matrix==NULL) return NULL;

	/*malloc the memory for the inverse matrix*/
	double ** inverse = (double **)malloc(sizeof(double *)* row);
	if (!inverse) return NULL;
	int i, j;

	for(i=0; i<row; i++) {
		inverse[i] = (double *)malloc(sizeof(double) * column);
		if (!inverse[i]) return NULL;
	}

	/*initialize to identity matrix*/
	for (i=0; i<row; i++ ) {
		for (j=0; j<row; j++) {
			if (i==j) {
				inverse[i][j]=1;
			}else {
				inverse[i][j]=0;
			}
		}
	}



	double divider=1, subtractor=1;;

	/*go row by row, to make into an upper triangular matrix*/
		/*Do this by making the first element of the row 1 by dividing by inverse[i][i]
			then subtract from every row below
				Do it all to the inverse as well*/


	for (i=0; i<row; i++) {

		/*divide the row to make it start with 1*/
		divider = matrix[i][i];
		divideRow(matrix, i, divider, row);
		divideRow(inverse, i, divider, row);
		/*row i of matrixshould now start with 1*/
		

		/*subtract the row i from all the rows j beneath it, by a factor of matrix[j][i]*/
		for (j=i+1; j<row; j++) {
			subtractor = matrix[j][i];
			subtractRow(matrix, i, j, subtractor, row);
			subtractRow(inverse, i, j, subtractor, row);
		}

	}


	/*now go bottom up to turn the left matrix into an identity matrix*/
		/*do this by subtracting the row from all above it by a factor of matrix[j][i]*/
			/*then do the same exact math to the inverse matrix*/
	for (i=row-1; i>0; i--) {
		for (j=i-1; j>=0; j--) {
			subtractor = matrix[j][i];
			subtractRow(matrix, i, j, subtractor, row);
			subtractRow(inverse, i, j, subtractor, row);
		}
	}



	return inverse;
}


/*method that frees all the memory allocated for a given matrix pointer*/

void freeMatrix (double ** matrix, int row) {

	if (matrix==NULL) return;
	int i;
	/*frees all row pointers*/
	for (i=0; i<row; i++) {
		free(matrix[i]);
	}
	/*frees the outer pointer*/
	free(matrix);
	return;
}



/*function that takes in two matricies and returns the product in a double** */
	/*same code from pa1, just adapted*/
double ** multiply (double** A, double** B, int rowsA, int columnsA, int rowsB, int columnsB) {

	double total=0;

	if (columnsA != rowsB) return NULL;

	/*allocate a product matrix*/
	double ** product = (double **)malloc(sizeof(double *) * rowsA);
	if (!product) return NULL;
	int i;
	for (i =0; i<rowsA; i++) {
		product[i] = (double *)malloc(sizeof(double)* columnsB);
		if (!product[i]) return NULL;
	}

	int y, p, x;
	for (y=0; y< rowsA; y++) {//goes down first
		for (p=0; p< columnsB; p++) {//goes across second
			for (x=0; x< columnsA; x++)	{//goes across first
		
				total= total+ A [y][x] * B[x][p];
		
			}

			product[y][p] = total;
			total=0;
		
		}
	}


	return product;
}

/*funtion that takes in a double** matrix and returns a double** to its transponse
	mallocs space for the transpose*/
double ** tanspose (double** matrix, int rows, int columns) {

	if (!matrix) return NULL;

	int i, j;

	/*create the space for the transposed matrix*/
	double ** result = (double **)malloc(sizeof(double *) * columns);
	if (!result) return NULL;
	for (i=0; i<columns; i++) {
		result[i] = (double *)malloc(sizeof(double) * rows);
		if (!result[i])return NULL;
	}

	
	/*copy numbers into the transpose matrix, swtiching th espot of row and column*/
	for (i=0; i<rows; i++) {
		for (j=0; j<columns; j++) {
			result[j][i] = matrix [i][j];
		}
	}


	return result;
}




int main (int argc, char** argv) {


	/*SECTION TO READ THE ARGUMENTS FOR FILE POINTERS*/
		/*copies file names into file1 and file2*/
	int size1, size2=0;
	if (argc!=3) return 1;
	if (!argv[2]) return 1;
	for (size1=0; argv[1][size1]!='\0'; size1++) {};
	for (size2=0; argv[2][size2]!='\0'; size2++) {};
	size1++; size2++;

	char * file1 = (char *)malloc(sizeof(char) * size1);
	if (!file1) {
		return 1;
	}
	strcpy(file1, argv[1]);

	char * file2 = (char *)malloc(sizeof(char) * size2);
	if (!file2) {
		return 1;
	}
	strcpy(file2, argv[2]);



	/*SECTION TO OPEN THE TRAIN FILE AND TAKE IN THE DATA INTO X AND Y*/
	FILE * fp;
	fp = fopen(file1, "r");
	if (!fp) return 1;

	char * type = (char *)malloc(sizeof(char)* 6);
	if (!type) return 1;
	if (fscanf(fp, "%s ", type)!=1) return 1;
	type[5]='\0';
	/*printf("%s\n", type);*/
	free(type);
	free(file1);

	int row, column;

	/*read rows and columns*/
	if (fscanf(fp, "%d %d", &column, &row) !=2) return 1;
	/*printf("%d %d\n", column, row);*/
	column++;

	/*allocates memory for X and Y*/
	int i, j;
	double ** X = (double **)malloc(sizeof(double *) * row);
	double ** Y = (double **)malloc(sizeof(double *) * row);
	if (!X || !Y) return 1;
	for (i=0; i<row; i++) {
		X[i] = (double *)malloc(sizeof(double) * column);
		Y[i] = (double *)malloc(sizeof(double));
		if (!X[i] || !Y[i])return 1;
	}

	int valid=0;

	/*reads the data from the file and puts it into X and Y*/
	for (i=0; i<row; i++)  {

		for (j=0; j<column+1; j++) {
			if (j==0) {
				X[i][j] = 1.0;
			}else if (j==column) {
				valid = fscanf(fp, "%lf ", &Y[i][0]);
				if (valid!=1) return 1;
			} else {
				valid = fscanf(fp, " %lf", &X[i][j]);
				if (valid!=1) return 1;
			}

			
		}

	}

	
	/*PRINTS X AS A CHECK
	for (i=0; i<row; i++) {
		for (j=0; j<column; j++) {
			printf("%.0f ", X[i][j]);
		}
		printf("\n");
	} 

	printf("\n");*/


	/*MATH SECTION, DOES W = (X^T * X)^-1 * X^T * Y*/

	double ** transposed = tanspose(X, row, column);
	if (transposed==NULL) return 1;

	double ** product = multiply(transposed, X, column, row, row, column);
	/*expect a result w dimensions of column X column */
	if (product == NULL) return 1;


	/*no longer need X, free it*/
	freeMatrix(X, row);

	double ** inverse = invert (product, column, column);
	if (inverse == NULL) return 1;

	/*no longer need transpose * X, free it*/
	freeMatrix(product, column);

	double ** product2 = multiply(inverse, transposed, column, column, column, row);
	if (product2==NULL) return 1;
	/*returns a column * row matrix*/

	/*no longer need the first inverse, nor the transposed X, free it*/
	freeMatrix(transposed, column);
	freeMatrix(inverse, column);

	
	double ** W = multiply(product2, Y, column, row, row, 1);
	if (W==NULL) return 1;
	/*W should be a column X 1 matrix*/

	/*no longer need Y or the second product, free them*/
	freeMatrix(Y, row);
	freeMatrix(product2, column);

	/*close the train file*/
	fclose(fp);

	/*SECTION TO OPEN THE DATA FILE AND INITIALIZE THE DATA MATRIX*/
	FILE * fp2;

	fp2 = fopen(file2, "r");
	if (!fp2) return 1;

	char * type2 = (char *)malloc(sizeof(char)* 5);
	
	if (!type2) return 1;
	if (fscanf(fp2, "%s ", type2)!=1) return 1;
	type2[4]='\0';
	/*printf("%s\n", type2);*/
	free(type2);
	free(file2);

	int row2, column2;
	

	if (fscanf(fp2, "%d %d", &column2, &row2) !=2) return 1;
	/*printf("%d %d\n", column2, row2);*/
	column2++;

	
	/*allocate memory for data*/
	double ** data = (double **)malloc(sizeof(double *) * row2);
	if (!data) return 1;



	for (i=0; i<row2; i++) {
		data[i] = (double *)malloc(sizeof(double) * column2);
		if (!data[i])return 1;
	}


	valid=0;

	/*reads the data from the file and puts it into data*/
	for (i=0; i<row2; i++)  {

		for (j=0; j<column2; j++) {
			if (j==0) {
				data[i][j] = 1.0;
			} else {
				valid = fscanf(fp2, " %lf", &data[i][j]);
				if (valid!=1) return 1;
			}

			
		}

	}
	
	



	/*PRINTS DATA AS A CHECK*/
	/*for (i=0; i<row2; i++) {
		for (j=0; j<column2; j++) {
			printf("%.0f ", data[i][j]);
		}
		printf("\n");
	}
	printf("\n");
	*/

	/*THE MATH TO FIND THE NEW PRICES*/
	double ** result = multiply(data, W, row2, column2, column, 1);
	if (result==NULL) return 1;

	/*PRINTS THE RESULTING PRICES*/
	for (i=0; i<row2; i++) {
		for (j=0; j<1; j++) {
			printf("%.0f", result[i][j]);
		}
		printf("\n");
	}
	

	freeMatrix(data, row2);
	freeMatrix(result, row2);
	freeMatrix(W, column);
	fclose(fp2);


	return 0;
}