/* file that creates a binary search tree, with functions to add, delete, search, and print*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct node {
	int data;
	struct node* right;
	struct node* left;
};

struct node* start = NULL;

/*method to find the left most item in a suubtree
	used when deleting a node with two children*/

int find (struct node * root) {

	struct node * ptr = root;

	if (ptr->right==NULL) {
		int data = ptr->data;
		free(ptr);
		return data;
	} else {
		return find(ptr->right);
	}
}


/*method to delete a node from the tree*/

void delete (int data) {

	if (start==NULL) {
		printf("absent\n");
		return;
	}

	struct node * ptr=start;
	struct node * parent = NULL;

	while (ptr!=NULL) {

		if (ptr->data==data) {


			/*if no children*/
			if (ptr->right ==NULL && ptr->left == NULL) {
				/*if the start node*/
				if (parent==NULL) {
					start=NULL;
					free(ptr);
					return;
				} else {
					/*set either parent.left or right to NULL, free the curr node*/
					if (data < parent->data) {
						parent->left = NULL;
						free(ptr);
						printf("deleted\n");
						return;
					} else {
						parent->right = NULL;
						free(ptr);
						printf("deleted\n");
						return;
					}
				}
			}


				/*if only a right child*/
			if (ptr->right !=NULL && ptr->left == NULL) {
				if (parent==NULL) {
					start = ptr->right;
					free (ptr);
					printf("deleted\n");
					return;
				} else {

					if (parent->data > data) {
						parent->left = ptr->right;
					}else {
						parent->right = ptr->right;
					}

					free(ptr);
					printf("deleted\n");
					return;
				}
			}
				/*if only a left child*/
			if (ptr->left !=NULL && ptr->right == NULL) {
				if (parent==NULL) {
					start = ptr->left;
					free (ptr);
					printf("deleted\n");
					return;
				} else {
					if (parent->data > data) {
						parent->left = ptr->left;
					}else {
						parent->right = ptr->left;
					}
					free(ptr);
					printf("deleted\n");
					return;
				}
			}


			/*if two children*/
			if(ptr->right!=NULL && ptr->left!= NULL) {
				int replacement = find(ptr->left);
				ptr->data = replacement;
				printf("deleted\n");
				return;
			}


		}else if (ptr->data>data) {
			parent = ptr;
			ptr = ptr->left;
		}else {
			parent = ptr;
			ptr = ptr->right;
		}


	}

	/*if the item was not found*/

	printf("absent\n");
	return;


}

/*recursive method to print the binary search tree
	prints a ( followed by the left subtree, followed by the root, then the right subtree*/
void print (struct node* parent) {

	if (parent==NULL) {
		return;
	}

	printf("(");
	print(parent->left);
	printf("%d", parent->data);
	print(parent->right);
	printf(")");
	return;

}


/*method to search for a given value in the tree*/
void search (int data) {

	if (start == NULL) {
		printf("absent\n");
		return;
	}

	struct node * ptr = start;

	while (ptr!=NULL) {
		if (ptr->data == data) {
			printf("present\n");
			return;
		} else if (ptr->data < data) {
			ptr = ptr->right;
		} else {
			ptr = ptr->left;
		}
	}

	if (ptr==NULL) {
		printf("absent\n");
	}

	return;

}




/*method to insert a new node with the data given*/
void insert (int data) {

	if (start==NULL) {
		start = (struct node *)malloc(sizeof(struct node));
		start->right = NULL;
		start->left = NULL;
		start->data = data;
		printf("inserted\n");
		return;
	} 
	
	struct node * ptr = start;
	struct node * parent = NULL;
	

	/*trace through the tree, looking for the value already or inserting it into the correct spot*/
	while (ptr!=NULL) {

		if (ptr->data == data) {
			printf("duplicate\n"); return;
		} else if (data > ptr->data) {
			parent = ptr;
			ptr = ptr->right;
		} else {
			parent = ptr;
			ptr = ptr->left;
		}

	}
	
		/*insert the new node either to the left or the right of the bottom node of the tree*/
	if (ptr==NULL) {
		struct node* new = (struct node *)malloc(sizeof(struct node));
		new->right = NULL;
		new->left = NULL;
		new->data = data;

		if (data < parent->data) {
			parent->left = new;
		} else {
			parent->right = new;
		}

		printf("inserted\n");
	}

		return;

}


int main (int argc, char** argv) {

	FILE *fp;
	int size;
	for (size=0; argv[1][size]!='\0'; size++) {};
	char *filename = (char *)malloc(sizeof(char)*size+1);
	strcpy(filename, argv[1]);

	fp=fopen(filename, "r");

	if (!fp) {
		printf("error\n");
		return 0;
	}

	char letter; int data;

	while (fscanf(fp, "%c", &letter) == 1) {

		if (letter=='\n') {
			continue;
		}

		if (letter == 'p') {
			print(start);
			printf("\n");
			continue;
		}
		if (fscanf(fp, "%d", &data)==1) {

			switch (letter) {
				case 'i' : insert(data); break;
				case 's' : search(data); break;
				case 'd' : delete(data); break;
				default: printf("error\n"); return 0;
			}

		}else {
			printf("error\n"); return 0;
		}

	}


	fclose(fp);

	return 0;

}
