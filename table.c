/*program that makes a 10000 entry hash table for ints*/
	/*can insert or search for an item*/
	/*hash is n mod 10000*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>



/*the structure node to be held in the array*/

struct node {
	int data;
	struct node * next;
};

/*the main hash table*/
struct node * table [10000];


/*function that creates a new node to be added to the table*/

struct node * create (int data) {

	struct node * new = (struct node * )malloc(sizeof(struct node));
	new->data = data;
	new->next = NULL;
	return new;


}




/*function to search for a given number in the hash table to see if it is there
	Prints present if so, absent if not*/
void search (int data) {

	int place = data % 10000;

	if (table[place]==NULL) {
		printf("absent\n");
		return;
	}else {
		struct node * ptr = table[place];
		while (ptr!=NULL) {
			if (ptr->data == data) {
				printf("present\n");
				return;
			}
			ptr=ptr->next;
		}
		printf("absent\n");
		return;
	}
}

/*function that inserts a node into the hash table with the given data*/
	/*prints inserted if successful, and duplicate if the number is already present*/
void insert (int data) {

	int place = data % 10000;

	if (table[place]==NULL) {
		table[place] = create(data);
		printf("inserted\n");
		return;
	}else {
		struct node  * ptr = table[place];
		while (ptr!=NULL) {
			if (ptr->data == data) {
				printf("duplicate\n");
				return;
			}
			if (ptr->next==NULL) {
				ptr->next = create(data);
				printf("inserted\n");
				break;
			}
			ptr=ptr->next;
		}
		
		return;
	}
}


int main (int argc, char** argv) {

	int i;
	for (i =0; i<10000; i++) {
		table[i]=NULL;
	}
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



	int num, check; 
	char action;

	/*run through the file for each letter and int, create/delete nodes*/

	
	while (!feof(fp)){

		check = fscanf(fp, "%c %d\n", &action, &num);


		if (check==2) {

			if (action=='s') {
				search(num);
			}else if (action == 'i') {
				insert(num);
			}else {
				break;
			}
		
		}
	
	} 

	

	fclose(fp);
	
	return 0;
}