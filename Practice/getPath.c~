#include <stdio.h>
#include <stdlib.h>


/**
 * A Huffman Tree Node structure to store an ASCII char and its frequency.
 */
typedef struct hnode
{
    char key; 
    int val; //frequency
    struct hnode * left;
    struct hnode * right;
} huffNode;

huffNode * createNode(char key, int val)
{
    //error check
	if (val < 0)
	{
		fprintf(stderr, "\nERROR!! Negative count !\n");
		return NULL;
	}

	//allocate space for new node
	huffNode* node = (huffNode*)malloc(sizeof(huffNode));

    if (NULL == node)
    {
    	fprintf(stderr, "\nERROR !!! Malloc failed !\n");
    	return node;
    } 

    //initialize newly allocated node
	node->key = key;
	node->val = val;
	node->left = NULL;
	node->right = NULL;

	return node;
}

int getPath(char key, huffNode* root, char* path, int i)
{
	//printf("Call\n");
	if(root == NULL) return 0;
	if((root->left == NULL) && (root->right == NULL) && (root->key == key)) return 1;
	if ((root->left == NULL) && (root->right == NULL)) return 0;

	path[i] = '0'; 
	path[i+1] = '\0';
	
	if(getPath(key,root->left,path,i+1)) 
	{
		return 1;
	}
	else 
	{
		path[i] = '1';
		path[i+1] = '\0';
	
		return (getPath(key,root->right,path,i+1));	
	}

} 

int main()
{
	huffNode * root = NULL;
	
	root = createNode((char)0, 8);
	root->left = createNode('g', 3);
	root->right = createNode((char)0, 5);
	
	root->right->left = createNode((char)0, 3);
		root->right->left->left = createNode('o', 2);
		root->right->left->right = createNode(' ', 1);
		
	root->right->right = createNode((char)0, 2);
		root->right->right->left = createNode('n', 1);
		root->right->right->right = createNode('i', 1);
		
	char path[10] = {'\0'};
	int i;
	
	char keys[5] = {'g','o',' ','n','i'};
	
	for(i=0; i<5; i++)
	{
		getPath(keys[i], root, path, 0);	
		printf("\nPath to %c is: %s\n",keys[i],path); 
	}




	/*-----------huffnode create test----------------------
	huffNode * root = NULL;
	
	root = createNode('a', 2);
	root->left = createNode('b', 4);
	root->right = createNode('e', 5);
	root->left->left = createNode('c', 7);
	root->left->right = createNode('d', 1);
	

	printf("root = %c, %d\n",root->key,root->val);	
	printf("root->right = %c, %d\n",root->right->key,root->right->val);
	printf("root->left = %c, %d\n",root->left->key,root->left->val);
	printf("root->left->left = %c, %d\n",root->left->left->key,root->left->left->val);	
	printf("root->left->right = %c, %d\n",root->left->right->key,root->left->right->val);
	printf("root->left->right 's children  = %d, %d\n",root->left->right->right,root->left->right->left);
	printf("root->left->left 's children  = %d, %d\n",root->left->left->right,root->left->left->left);
	
	*/
	return 0;
}
