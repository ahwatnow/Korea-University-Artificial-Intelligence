#define SHOW_STEP 0 // 제출시 0
#define BALANCING 1 // 제출시 1

#include <stdlib.h>
#include <stdio.h>
#include <string.h> //strcmp, strdup

#define max(x, y)	(((x) > (y)) ? (x) : (y))

////////////////////////////////////////////////////////////////////////////////
// AVL_TREE type definition
typedef struct node
{
	char		*data;
	struct node	*left;
	struct node	*right;
	int			height;
} NODE;

typedef struct
{
	NODE	*root;
	int		count;  // number of nodes
} AVL_TREE;

////////////////////////////////////////////////////////////////////////////////
// Prototype declarations

/* Allocates dynamic memory for a AVL_TREE head node and returns its address to caller
	return	head node pointer
			NULL if overflow
*/
AVL_TREE *AVL_Create( void);
{
	AVL_TREE* nh = (AVL_TREE* )malloc (sizeof(AVL_TREE));
	nh->root = NULL;
	nh->count =0;
	return nh;
}


/* Deletes all data in tree and recycles memory
*/
void AVL_Destroy( AVL_TREE *pTree);
{
	if(pTree)_destroy(pTree->root);
	free(pTree);
}

static void _destroy( NODE *root);

static void _destroy( NODE *root)
{
	if(root==NULL)
		return;
	_destroy(root->right);
	_destroy(root->left);
	free(root->data);
	free(root);
}

/* Inserts new data into the tree
	return	1 success
			0 overflow
*/
int AVL_Insert( AVL_TREE *pTree, char *data);

int AVL_Insert( AVL_TREE *pTree, char *data)
{
	NODE *new = _makeNode(data);
	if(!new)
	{
		return 0;
	}
	else
	{
		pTree->root = _insert(pTree->root,new);
		(pTree->count)++;
		return 1;
	}
}


/* internal function
	This function uses recursion to insert the new data into a leaf node
	return	pointer to new root
*/
static NODE *_insert( NODE *root, NODE *newPtr);

static NODE *_insert( NODE *root, NODE *newPtr) 
{
	if(root==NULL)
	{
		newPtr->height =1;
		root=newPtr;
		return root;
	]
}


static NODE *_makeNode( char *data);

static NODE *_makeNode( char *data)
{
	NODE* new=(NODE*)malloc(sizeof(NODE));
	new->right = NULL;
	new->left =NULL;
	new->data = strdup(data);
	new->height =0;
	return new;
}

/* Retrieve tree for the node containing the requested key
	return	address of data of the node containing the key
			NULL not found
*/
char *AVL_Retrieve( AVL_TREE *pTree, char *key);

char *AVL_Retrieve( AVL_TREE *pTree, char *key)
{
	NODE* nn = _retrieve(pTree->root, key);
	if(nn) return nn->data;
	else return NULL;
}

/* internal function
	Retrieve node containing the requested key
	return	address of the node containing the key
			NULL not found
*/
static NODE *_retrieve( NODE *root, char *key);

static NODE *_retrieve( NODE *root, char *key)
{
	if (root == NULL) return NULL;
	if (strcmp(key, root->data) > 0) return _retrieve(root->right, key);
	else if (strcmp(key, root->data) < 0) return _retrieve(root->left, key);
	else return root;
}

/* Prints tree using inorder traversal
*/
void AVL_Traverse( AVL_TREE *pTree);

void AVL_Traverse( AVL_TREE *pTree)
{
	_traverse(pTree->root);
}

static void _traverse( NODE *root);

static void _traverse( NODE *root)
{
	if (root==NULL) return;
	_traverse(root->left);
	printf("%s ", root->data);
	_traverse(root->right);
}

/* Prints tree using inorder right-to-left traversal
*/
void printTree( AVL_TREE *pTree);

void printTree( AVL_TREE *pTree)
{
	_infix_print(pTree->root, 0);
}

/* internal traversal function
*/
static void _infix_print( NODE *root, int level);

static void _infix_print( NODE *root, int level)
{
	if(root != NULL)
	{			
		level++;
		_infix_print(root->right, level);			
		for(int i=1; i<level; i++)
			printf("\t");
		printf("%s\n", root->data);
		_infix_print(root->left, level);
	}
}

/* internal function
	return	height of the (sub)tree from the node (root)
*/
static int getHeight( NODE *root);

static int getHeight( NODE *root)
{
	int L,R;
	if(root==NULL)
		return 0;
	if(root->right==NULL)	R=0;
	else	R=root->right->height;
	if(root->left==NULL)	L=0;
	else	L=root->left->height;
	if(R>L)
		return R+1;
	else
		return L+1;
}

/* internal function
	Exchanges pointers to rotate the tree to the right
	updates heights of the nodes
	return	new root
*/
static NODE *rotateRight( NODE *root);

static NODE *rotateRight( NODE *root)
{
	NODE *nr = root;
	root=root->left;
	nr->left = root->right;
	root->right = nr;
	root->right->height = max(getHeight(root->right->left),getHeight(root->right->right))+1;
	root->height = max(getHeight(root->left),getHeight(root->right))+1;
	return root;
}

/* internal function
	Exchanges pointers to rotate the tree to the left
	updates heights of the nodes
	return	new root
*/
static NODE *rotateLeft( NODE *root);

static NODE *rotateLeft( NODE *root)
{
	NODE *nr = root;
	root=root->right;
	nr->right = root->left;
	root->left = nr;
	root->left->height = max(getHeight(root->left->right),getHeight(root->left->left))+1;
	root->height = max(getHeight(root->left),getHeight(root->right))+1;
	return root;
}

////////////////////////////////////////////////////////////////////////////////
int main( int argc, char **argv)
{
	AVL_TREE *tree;
	char str[1024];
	if (argc != 2)
	{
		fprintf( stderr, "Usage: %s FILE\n", argv[0]);
		return 0;
	}
	tree = AVL_Create();
	 if (!tree)
	{
		fprintf( stderr, "Cannot create tree!\n");
		return 100;
	} 
	FILE *fp = fopen( argv[1], "rt");
	if (fp == NULL)
	{
		fprintf( stderr, "Cannot open file! [%s]\n", argv[1]);
		return 200;
	}

	while(fscanf( fp, "%s", str) != EOF)
	{
#if SHOW_STEP
		fprintf( stdout, "Insert %s>\n", str);
#endif		
		AVL_Insert( tree, str);

#if SHOW_STEP
		fprintf( stdout, "Tree representation:\n");
		printTree( tree);
#endif
	}
	fclose( fp);
#if SHOW_STEP
	fprintf( stdout, "\n");
	fprintf( stdout, "Inorder traversal: ");
	AVL_Traverse( tree);
	fprintf( stdout, "\n");
	print tree with right-to-left infix traversal
	fprintf( stdout, "Tree representation:\n");
	printTree(tree);
#endif
	fprintf( stdout, "Height of tree: %d\n", tree->root->height);
	fprintf( stdout, "# of nodes: %d\n", tree->count);
	char *key;
	fprintf( stdout, "Query: ");
	while( fscanf( stdin, "%s", str) != EOF)
	{
		key = AVL_Retrieve( tree, str);
		if (key) fprintf( stdout, "%s found!\n", key);
		else fprintf( stdout, "%s NOT found!\n", str);
		fprintf( stdout, "Query: ");
	}
	AVL_Destroy( tree);
	return 0;
}

