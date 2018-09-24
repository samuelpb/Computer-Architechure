

/* program that takes an input text file and creates a linked list of ints:
	can insert (i) or delete (d)
		will not insert a duplicate */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/* node structure for the linked list, contains the data and the pointer to the next one*/



struct node {

	int data;
	struct node* next;
};

/*counter for the number of nodes, global;  start node for the linked list*/
int counter=0;
struct node * start = NULL;


void nodefree(struct node * start) {

	struct node * ptr = start;

	while (ptr!=NULL) {
		start = start->next;
		free(ptr);
		ptr = start;
	}
}


/*method to create a new node, returns that node with the data
	mallocs space for the node*/
struct node * create (data) {

	struct node* new = (struct node *)malloc (sizeof(struct node));
	new->data = data;
	new->next = NULL;
	counter++;
	return new;
}


/*method to delete a node from the list
	if the node is not there, then it does nothing
	returns the start of the list*/

struct node* delete (int data) {

	if (start==NULL) {
		return start;
	}

	struct node* ptr = start;
	struct node* prev = NULL;

	/*loop thougho check for the number*/
	while (ptr !=NULL) {
		if (ptr->data == data) {
			if (prev==NULL) {
				counter = counter-1;
				prev = ptr;
				ptr = ptr->next;
				start=ptr;
				free(prev);
				return start;
			}
			/*if there, delete the link and free the space*/
			prev->next = ptr->next;
			free(ptr);
			counter = counter -1;
			return start;
		}
		prev = ptr;
		ptr = ptr->next;
	}

	return start;

}



/*method to insert a new node into the linked list, will not insert a duplicate
	returns 1 for a success, 0 for a failure*/
struct node* insert (int data) {

	if (start==NULL) {
		start = create(data);
		return start;
	}

	struct node * ptr = start;
	struct node * prev = NULL;

	
	/*loop through to check if item is there already*/
	while (ptr!=NULL) {
		if (ptr->data == data) {
			return start;
		}else if (ptr->data > data) {
			if (prev==NULL) {
				prev = create(data);
				prev->next = ptr;
				start=prev;
				return start;
			}else{
				prev->next = create(data);
				prev = prev->next;
				prev->next=ptr;
				return start;
			}

		}
		
		prev = ptr;
		ptr = ptr->next;
		
	}

	/*if it wasnt, create a new node and add to end*/

	prev->next = create(data);
	

	return start;
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



	int num, check; 
	char action;
	struct node* start;

	/*run through the file for each letter and int, create/delete nodes*/	
	while (!feof(fp)){

		
		check = fscanf(fp, "%c %d\n", &action, &num);


		if (check==2) {

			if (action=='d') {
				start = delete(num);
			}else if (action == 'i') {
				start = insert(num);
			}else {
				break;
			}
			

		}
	
	

	} 
	fclose(fp);

	/*printing result*/
	printf("%d\n", counter);
	if (counter==0) return 0;
	while (start!=NULL) {
		printf("%d ", start->data);
		start = start->next;
	}
	
	printf("\n");
	
	nodefree(start);

	return 0;
}

