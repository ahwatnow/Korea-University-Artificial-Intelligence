#include <stdlib.h> // malloc, atoi, rand
#include <stdio.h>
#include <assert.h>
#include <time.h> // time

#define RANDOM_INPUT	1
#define FILE_INPUT		2

////////////////////////////////////////////////////////////////////////////////
// TREE type definition
typedef struct node
{
	int			data;
	struct node	*left;
	struct node	*right;
} NODE;

typedef struct
{
	NODE	*root;
} TREE;

////////////////////////////////////////////////////////////////////////////////
// Prototype declarations

/* Allocates dynamic memory for a tree head node and returns its address to caller
	return	head node pointer
			NULL if overflow
*/
TREE *BST_Create( void)
{
	TREE *head = (TREE*)malloc(sizeof(TREE));
	head -> root = NULL;
	return head;
}

static void _destroy( NODE *root)
{
	if(root==NULL)
		return;
	_destroy(root->left);
	_destroy(root->right);
	free(root);
}

void BST_Destroy( TREE *pTree)
{
	if(pTree)
		_destroy(pTree->root);
	free(pTree);
}

static void _insert( NODE *root, NODE *newPtr)
{
	if((root->data)<(newPtr->data))
	{
		if(root->left==NULL)
			root->left = newPtr;
		else
		_insert(root->left,newPtr);
	}
	else
		if(root->right == NULL)
			root->right = newPtr;
		else
			_insert(root->right, newPtr);	
}

NODE *_makeNode( int data)
{
	NODE* newnode=(NODE*)malloc(sizeof(NODE));
	newnode->data = data;
	newnode->right = NULL;
	newnode->left =NULL;
	return newnode;
}

int BST_Insert( TREE *pTree, int data)
{
	NODE*newnode = _makeNode(data);
	if(!newnode)
		return 0;
	if(pTree->root !=NULL)
		_insert(pTree->root,newnode);
	else
		pTree->root = newnode;
	return 1;
}

static NODE *_retrieve( NODE *root, int key)
{
	if(root==NULL)
		return NULL;
	if(root->data ==key)
		return root;
	else if(key<root->data)
		_retrieve(root->left,key);
	else if (key>root->data)
		_retrieve(root->right,key);
}

int *BST_Retrieve( TREE *pTree, int key)
{
	NODE * rnode=_retrieve(pTree->root,key);
	if(rnode==NULL)
		return 0;
}


static NODE *_delete( NODE *root, int dltKey, int *success)
{
	if(root==NULL)
	{
		*success = 0;
		return root;
	}
	
	if(root->data == dltKey)
	{
			if(root->left!=NULL && root->right !=NULL)
			{
				NODE *temp = root->right;
				while(temp ->left !=NULL)
					temp=temp->left;
				root->right = _delete(root->right,temp->data,success);
				root->data = temp->data;
			} 	
			else if(root->left ==NULL) 
			{
				NODE*dnode=root;
				root=root->right;
				free(dnode);
			}
			else if(root->right==NULL) 
			{
				NODE*dnode=root;
				root=root->left;
				free(dnode);
			}
			*success=1;	
	}
	else if (dltKey > root->data)
		root->right = _delete(root->right,dltKey,success);
	else 
		root->left=_delete(root->left, dltKey,success);
	return root;
}



int BST_Delete( TREE *pTree, int dltKey)
{
	NODE *dnode=_retrieve(pTree->root,dltKey);
	int success =0;
	if(!dnode)
	{
		return success;
	}
	else
	{
		pTree->root = _delete(pTree->root,dltKey,&success);
		return success;
	}	
}

/* internal function
	Retrieve node containing the requested key
	return	address of the node containing the key
			NULL not found
*/

static void _traverse( NODE *root)
{
	if(root != NULL)
	{
		_traverse(root->left);
		printf("%d",root->data);
		_traverse(root->right);
	}
}
void BST_Traverse( TREE *pTree)
{
	_traverse(pTree->root);
}


static void _inorder_print( NODE *root, int level)
{
	if(root!=NULL)
	{
		level++;
		_inorder_print(root->right,level);
		for(int i=1;i<level;i++)
		{
			printf("\t");
		}
		printf("%d\n",root->data);
		_inorder_print(root->left,level);
	}
}

void printTree( TREE *pTree)
{
	_inorder_print(pTree->root, 0);
	return;
}
/* internal traversal function
*/

/* 
	return 1 if the tree is empty; 0 if not
*/
int BST_Empty( TREE *pTree)
{
	if(pTree==NULL)
		return 1;
	else return 0;
}

////////////////////////////////////////////////////////////////////////////////
int main( int argc, char **argv)
{
	int mode; // input mode
	TREE *tree;
	int data;
	
	if (argc != 2)
	{
		fprintf( stderr, "usage: %s FILE or %s number\n", argv[0], argv[0]);
		return 1;
	}
	
	FILE *fp;
	
	if ((fp = fopen(argv[1], "rt")) == NULL)
	{
		mode = RANDOM_INPUT;
	}
	else mode = FILE_INPUT;
	
	// creates a null tree
	tree = BST_Create();
	
	if (!tree)
	{
		printf( "Cannot create a tree!\n");
		return 100;
	}

	if (mode == RANDOM_INPUT)
	{
		int numbers;
		numbers = atoi(argv[1]);
		assert( numbers > 0);

		fprintf( stdout, "Inserting: ");
		
		srand( time(NULL));
		for (int i = 0; i < numbers; i++)
		{
			data = rand() % (numbers*3) + 1; // random number (1 ~ numbers * 3)
			
			fprintf( stdout, "%d ", data);
			
			// insert function call
			int ret = BST_Insert( tree, data);
			if (!ret) break;
		}
	}
	else if (mode == FILE_INPUT)
	{
		fprintf( stdout, "Inserting: ");
		
		while (fscanf( fp, "%d", &data) != EOF)
		{
			fprintf( stdout, "%d ", data);
			
			// insert function call
			int ret = BST_Insert( tree, data);
			if (!ret) break;
		}
		fclose( fp);
	}
	
	fprintf( stdout, "\n");

	if (BST_Empty( tree))
	{
		fprintf( stdout, "Empty tree!\n");
		BST_Destroy( tree);
		return 0;
	}	

	// inorder traversal
	fprintf( stdout, "Inorder traversal: ");
	BST_Traverse( tree);
	fprintf( stdout, "\n");
	
	// print tree with right-to-left inorder traversal
	fprintf( stdout, "Tree representation:\n");
	printTree(tree);
	
	while (1)
	{
		fprintf( stdout, "Input a number to delete: "); 
		int num;
		if (scanf( "%d", &num) == EOF) break;
		
		int ret = BST_Delete( tree, num);
		if (!ret)
		{
			fprintf( stdout, "%d not found\n", num);
			continue;
		}
		
		// print tree with right-to-left inorder traversal
		fprintf( stdout, "Tree representation:\n");
		printTree(tree);
		
		if (BST_Empty( tree))
		{
			fprintf( stdout, "Empty tree!\n");
			break;
		}
	}
	
	BST_Destroy( tree);

	return 0;
}

