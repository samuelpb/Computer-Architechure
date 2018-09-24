/* sudoko file that checks to see if a given puzzle is correctly completed
	or if the file has a space or two missing it checks if it can be completed*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/*the puzzle as a global variable*/
int ** puzzle = NULL;

/*function to check if the given row of the puzzle is correct, returns 1 if it is, 0 if not*/
int crow(int row) {

	int check[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	int i;

	for (i=0; i<9; i++) {
		check[(puzzle[row][i])] ++;
		if (check[puzzle[row][i]] >1) {
			return 0;
		}
	}
	return 1;

}


/*function to check if the given column of the puzzle is correct, returns 1 if it is, 0 if not*/
int ccolumn(int column) {

	int check[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

	int i;
	for (i=0; i<9; i++) {
		check[puzzle[i][column]] ++;
		if (check[puzzle[i][column]] >1) {
			return 0;
		}
	}

	return 1;

}

/*checks each 3x3 box of the puzzle, based on int given
	returns 1 if correct, 0 if not*/
int cbox (int box) {

	int row, column;
	switch (box) {
		case 0: row = 0; column =0; break;
		case 1: row = 0; column =3; break;
		case 2: row = 0; column =6; break;
		case 3: row = 3; column =0; break;
		case 4: row = 3; column =3; break;
		case 5: row = 3; column =6; break;
		case 6: row = 6; column =0; break;
		case 7: row = 6; column =3; break;
		case 8: row = 6; column =6; break;
		default: return 0;
	}

	int check[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	int i, j;
	for (i=row; i<row+3; i++) {
		for (j = column; j<column+3; j++) {
			check[puzzle[i][j]] ++;
			if (check[puzzle[i][j]] >1) {
				return 0;
			}	
		}
	}
	return 1;
}




/*function to check if the sudoko puzzle is correct
	calls crow, ccolumn, and cbox*/

int check () {

	int total=0;
	int i;
	for (i = 0; i<9; i++) {
		total += crow(i);
		total += ccolumn(i);
		total += cbox(i);

		if (total!=3) {
			return 0;
		}
		total = 0;
	}

	return 1;

}

/*function to solve the puzzle with one space missing, then check it to see if it is right*/
	/*returns 1 if it can be solved, 0 if not*/
int solveone (int loc) {

	int row = loc/10;
	int column = loc%10;

	int count[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	int i;
	/*finds the digits in that row*/
	for (i=0; i<9; i++) {
		count[puzzle[row][i]] ++;
		if (count[puzzle[row][i]] >1) {
			return 0;
		}
	}

	/*puts the missing number into the puzzle*/
	for (i=1; i<10; i++) {
		if (count[i]==0) {
			puzzle[row][column] = i;
			break;
		}
	}

	/*checks the solved puzzle to see if it is right*/
	if (check()==1) {
		return 1;
	}

	return 0;

}


int solvetwo (int loc1, int loc2) {

	int rowone = loc1/10;
	int rowtwo = loc2/10;
	int columnone = loc1%10;
	int columntwo = loc2 %10;
	int countone[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	int counttwo[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	int i;

	/*if in the same row, fill the puzzle by the columns*/
	if (rowone == rowtwo) {

		/*counts the digits in each of the rows*/
		for (i=0; i<9; i++) {
			countone[puzzle[i][columnone]] ++;
			counttwo[puzzle[i][columntwo]] ++;

			if (countone[puzzle[i][columnone]] >1 || counttwo[puzzle[i][columntwo]]>1) {
				return 0;
			}

		}

		/*puts the missing digits into the puzzle*/
		for (i=1; i<10; i++) {
			if (countone[i]==0) {
				puzzle[rowone][columnone] = i;
			}
			if (counttwo[i]==0) {
				puzzle[rowtwo][columntwo] = i;
			}
		}

	} else { /*if not in the same row, then fill them by the rows, using code from solve one*/
		
		/*counts the digits in each of the rows*/
		for (i=0; i<9; i++) {
			countone[puzzle[rowone][i]] ++;
			counttwo[puzzle[rowtwo][i]] ++;

			if (countone[puzzle[rowone][i]] >1 || counttwo[puzzle[rowtwo][i]]>1) {
				return 0;
			}

		}

		/*puts the missing digits into the puzzle*/
		for (i=1; i<10; i++) {
			if (countone[i]==0) {
				puzzle[rowone][columnone] = i;
			}
			if (counttwo[i]==0) {
				puzzle[rowtwo][columntwo] = i;
			}
		}

	}

	/*checks the solved puzzle to see if it is right*/
	if (check()==1) {
		return 1;
	}

	return 0;

}

int main (int argc, char** argv) {

	FILE * fp;
	int size;
	for (size=0; argv[1][size] != '\0'; size++) {
	}
	size++;

	char * filename = (char *)malloc(sizeof(char) * size);
	strcpy(filename, argv[1]);

	puzzle = (int **)malloc(sizeof(int*) * 10);

	int i;
	for (i=0; i<10; i++) {
		puzzle[i] = (int *)malloc(sizeof(int) * 10);
	}

	fp =fopen(filename, "r");

	if (!fp) {
		printf("error\n");
		return 0;
	}

	char input=0;
	int row=0, column=0, zCount = 0;
	int loc1=0, loc2=0;

	/*loop to take in the input puzzle
		checks for non-digits and spaces, keeps track of empty spots
			prints error if the input is not correct*/

	while (fscanf(fp, "%c", &input)==1) {

		/*if inpu tis too large more than 9x9*/
		if (row==9) {
			if (input=='\n') continue;
			printf("error\n");
			return 0;
		}
		
		
		if (input == '\n') continue;

		if (isdigit(input)) {
			int x = input - '0';
			if (x==0) {
				printf("error\n");
				return 0;
			}
			puzzle[row][column] = x;
		}else {
			/*if there is a space, keep track of the spaces*/
			if (input == ' '){
				puzzle[row][column] = 0;
				zCount++;
				if (zCount==1) {
					loc1 = row*10 + column;
				}else if (zCount==2) {
					loc2 = row*10 + column;
				} else {
					printf("error\n");
					return 0;
				}

			} else {
				printf("error\n");
				return 0;
			}
		}

		
		column++;

		/*after 9 digits, go to next row*/
		if (column==9) {
			column=0;
			row++;
		}

		
	}

	/*if not a 9x9 filled up, print error*/
	if (row!=9) {
		printf("error\n");
		return 0;
	}

	/*if no blanks, check it.
		if one blank, solve it with solve one and check
		if two, solve it with solvetwo and check*/

	
	if (zCount==0) {
		if (check() != 1) {
			printf("incorrect\n");
			return 0;
		}else {
			printf("correct\n");
			return 0;
		}
	}else if (zCount==1) {
		if (solveone(loc1) ==1) {
			printf("solvable\n");
			return 0;
		} else {
			printf("unsolvable\n");
			return 0;
		}

	} else if (zCount==2) {
		if (solvetwo(loc1, loc2) ==1) {
			printf("solvable\n");
			return 0;
		} else {
			printf("unsolvable\n");
			return 0;
		}
	}


	
	fclose(fp);

	return 0;
}