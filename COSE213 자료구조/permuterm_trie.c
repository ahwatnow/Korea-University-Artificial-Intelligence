#include <stdio.h>
#include <stdlib.h>	// malloc
#include <string.h>	// strdup
#include <ctype.h>	// isupper, tolower

#define MAX_DEGREE	27 // 'a' ~ 'z' and EOW
#define EOW			'$' // end of word

// used in the following functions: trieInsert, trieSearch, triePrefixList
#define getIndex(x)		(((x) == EOW) ? MAX_DEGREE-1 : ((x) - 'a'))

// TRIE type definition
typedef struct trieNode {
	int 			index; // -1 (non-word), 0, 1, 2, ...
	struct trieNode	*subtrees[MAX_DEGREE];
} TRIE;

////////////////////////////////////////////////////////////////////////////////
// Prototype declarations

/* Allocates dynamic memory for a trie node and returns its address to caller
	return	node pointer
			NULL if overflow
*/
TRIE *trieCreateNode(void)
{ 
	TRIE* newTrie = (TRIE*)malloc(sizeof(TRIE));
	if (newTrie == NULL) 
	{
		return NULL;
	}
	newTrie->index = -1;
	for (int i = 0; i < MAX_DEGREE; i++) 
	{
		newTrie->subtrees[i] = NULL;
	}
	return newTrie;
}


/* Deletes all data in trie and recycles memory
*/
void trieDestroy( TRIE *root);

void trieDestroy( TRIE* root) {

	for (int i = 0; i < MAX_DEGREE; i++) {
		if (root->subtrees[i]) {
			trieDestroy(root->subtrees[i]);
		}
	}
	free(root);
}

/* Inserts new entry into the trie
	return	1 success
			0 failure
*/
// 주의! 엔트리를 중복 삽입하지 않도록 체크해야 함
// 대소문자를 소문자로 통일하여 삽입
// 영문자와 EOW 외 문자를 포함하는 문자열은 삽입하지 않음
int trieInsert( TRIE *root, char *str, int dic_index);

int trieInsert( TRIE* root, char* str, int dic_index) 
{
	TRIE* first = root;
	while (*str) 
	{
		if (((*str > 64) && (*str < 91)) || (*str == EOW) || ((*str > 96) && (*str < 123))) 
		{
			if (*str == EOW) 
			{
				if (first->subtrees[getIndex(EOW)] == NULL) 
				{
					first->subtrees[getIndex(EOW)] = trieCreateNode();
				}
			}
			else if (isupper(*str)) 
			{
				*str = tolower(*str);
				if (first->subtrees[getIndex(*str)] == NULL) 
				{
					first->subtrees[getIndex(*str)] = trieCreateNode();
				}
			}
			else 
			{
				if (first->subtrees[getIndex(*str)] == NULL) 
				{
					first->subtrees[getIndex(*str)] = trieCreateNode();
				}
			}
			first = first->subtrees[getIndex(*str)];
			str++;
		}
		else return 0;
	}
	if (first->index == -1) 
	{
		first->index = dic_index;
		return 1;
	}
	else return 0;
}

/* Retrieve trie for the requested key
	return	index in dictionary (trie) if key found
			-1 key not found
*/
int trieSearch( TRIE *root, char *str);

int trieSearch( TRIE* root, char* str) 
{
	TRIE* r = root;
	while (*str) 
	{
		if (*str == '*')return -1;
		if (((*str > 64) && (*str < 91)) || (*str == EOW) || ((*str > 96) && (*str < 123)))
		{
			if (r->subtrees[getIndex(*str)] != NULL) 
			{
				r = r->subtrees[getIndex(*str)];
				str++;
			}
			else return -1;
		}
		else return -1;
	}

	if (r->index != -1) 
	{
		return r->index;
	}
	return -1;
}

/* prints all entries in trie using preorder traversal
*/
void trieList( TRIE *root, char *dic[]);

void trieList( TRIE* root, char* dic[]) 
{
	if (root->index != -1) 
	{
		printf("%s\n", dic[root->index]);
	}

	for (int i = 0; i < MAX_DEGREE; i++) 
	{
		if (root->subtrees[i] != NULL) 
		{
			trieList(root->subtrees[i], dic);
		}
	}
}


/* prints all entries starting with str (as prefix) in trie
	ex) "abb" -> "abbas", "abbasid", "abbess", ...
	this function uses trieList function
*/
void triePrefixList( TRIE *root, char *str, char *dic[]);

void triePrefixList(TRIE* root, char* str, char* dic[]) 
{
	TRIE* v = root;
	while (*str) 
	{
		if (*str == '*') 
		{
			trieList(v, dic);
			return;
		}
		if (v->subtrees[getIndex(*str)] != NULL) 
		{
			v = v->subtrees[getIndex(*str)];
			str++;
		}
		else return;
	}
}


/* makes permuterms for given str
	ex) "abc" -> "abc$", "bc$a", "c$ab", "$abc"
	return	number of permuterms
*/
int make_permuterms( char *str, char *permuterms[]);

int make_permuterms( char* str, char* permuterms[]) 
{
	int leng = 0;
	int count = 0;

	if (str) 
	{
		leng = strlen(str);
	}
	else return 0;
	int lens = leng;
	str[lens] = '\0';	
	str[lens++] = '$';
	char* ptr = str;
	char* s;
	int i = 0;
	while (i <= leng) 
	{
		str[lens++] = *ptr;
		str[lens] = '\0';
		ptr++;
		s = ptr;
		permuterms[count++] = strdup(s);
		i++;
	}
	str[length] = '\0';
	return cnt;
}

/* recycles memory for permuterms
*/
void clear_permuterms( char *permuterms[], int size);

void clear_permuterms( char* permuterms[], int size) 
{
	for (int i = 0; i < size; i++) 
	{
		if (permuterms[i]) 
		{
			free(permuterms[i]);
		}
	}
}

/* wildcard search
	ex) "ab*", "*ab", "a*b", "*ab*"
	this function uses triePrefixList function
*/
void trieSearchWildcard( TRIE *root, char *str, char *dic[]);

void trieSearchWildcard( TRIE* root, char* str, char* dic[]) 
{
	char* p = str;
	int count = 0;
	TRIE* a = root;

	while (*p) 
	{
		if (((*p > 64) && (*p < 91)) || (*p == '*') || ((*p > 96) && (*p < 123))) 
		{
			if (*p == '*') 
			{
				count++;
			}	
			p++;
		}
		else return;
	}
	if (count == 1) 
	{
		if (str[strlen(str) - 1] == '*' && str[0] != '*') 
		{
			if (a->subtrees[getIndex(EOW)]) 
			{
				a = a->subtrees[getIndex(EOW)];
				triePrefixList(a, str, dic);
			}
			else return;
		}
	}
	if (count == 1) 
	{
		 if (str[strlen(str) - 1] != '*' && str[0] == '*') 
		 { 
			str++;
			while (*str) 
			{
				if (a->subtrees[getIndex(*str)]) 
				{
					a = a->subtrees[getIndex(*str)];
					str++;
				}
				else return;
			}
			if (a->subtrees[getIndex(EOW)]) 
			{
				a = a->subtrees[getIndex(EOW)];
				trieList(a, dic);
			}
			else return;
		}
	}
	if (count == 2) 
	{
		if (str[strlen(str) - 1]  == '*' && str[0] == '*') 
		{
			str++;
			triePrefixList(a, str, dic);
		}
	}
	if (count == 1) 
	{
		 if (str[strlen(str) - 1]  == '*' && str[0] == '*') 
		 { 
			int countt = 0;
			char* ptr = str;
			while (*str) 
			{
				str++;
				if (*str == '*') 
				{
					countt++;
					break;
				}
			}
			if (countt == 1) 
			{
				str++;
				while (*str) 
				{
					if (*str == '*')return;
					if (a->subtrees[getIndex(*str)]) 
					{
						a = a->subtrees[getIndex(*str)];
						str++;
					}
					else return;
				}
				if (a->subtrees[getIndex(EOW)]) 
				{
					a = a->subtrees[getIndex(EOW)];
				}
				else return;
				triePrefixList(a, ptr, dic);
			}
		}
	}
	return;
}

////////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
	TRIE *trie;
	TRIE *permute_trie;
	char *dic[100000];

	int ret;
	char str[100];
	FILE *fp;
	char *permuterms[100];
	int num_p;
	int index = 0;
	
	if (argc != 2)
	{
		fprintf( stderr, "Usage: %s FILE\n", argv[0]);
		return 1;
	}
	
	fp = fopen( argv[1], "rt");
	if (fp == NULL)
	{
		fprintf( stderr, "File open error: %s\n", argv[1]);
		return 1;
	}
	
	trie = trieCreateNode(); // original trie
	permute_trie = trieCreateNode(); // trie for permuterm index
	
	while (fscanf( fp, "%s", str) != EOF)
	{	
		ret = trieInsert( trie, str, index);
		
		if (ret)
		{
			num_p = make_permuterms( str, permuterms);
			
			for (int i = 0; i < num_p; i++)
				trieInsert( permute_trie, permuterms[i], index);
			
			clear_permuterms( permuterms, num_p);
			
			dic[index++] = strdup( str);
		}
	}
	
	fclose( fp);
	
	printf( "\nQuery: ");
	while (fscanf( stdin, "%s", str) != EOF)
	{
		// wildcard search term
		if (strchr( str, '*')) 
		{
			trieSearchWildcard( permute_trie, str, dic);
		}
		// keyword search
		else 
		{
			ret = trieSearch( trie, str);
			if (ret == -1) printf( "[%s] not found!\n", str);
			else printf( "[%s] found!\n", dic[ret]);
		}
		printf( "\nQuery: ");
	}

	for (int i = 0; i < index; i++)
		free( dic[i]);
	
	trieDestroy( trie);
	trieDestroy( permute_trie);
	
	return 0;
}

