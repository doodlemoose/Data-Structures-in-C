#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#define DEBUG 1
#define MAX_PQ_SIZE 260 //count when PQ is full  	
#define PATH_SIZE 257



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


/*----------------PRIORITY QUEUE-----------------------------
	Here for a starting point, i have implemented the PQ as an array,
	using a pass of selection sort to implement the priority.
	
	Properties:
	count: number of elements currently in queue 
		   - also is the index of the next enqueue operation 
		   - [count - 1] is the index of next dequeue operation
	buff: array when elements are stored
		  
	Operations:
	Enqueue: O(n) time - run pass of insertion sort 
	Dequeue: O(1) time - just remove last element of array
	
	-> Cannot dequeue when count = 0
	-> Cannot enqueue when count = MAX_PQ_SIZE
*/	
	
typedef struct pQueue{
	
	int count; //current number of elements
	huffNode** buff; //pointer to an array of huffNodes
}pQueue;
	
	
pQueue* createPqueue()
{
	pQueue* PQ = (pQueue*)malloc(sizeof(pQueue)); //allocate space for PQ on heap
	 
	PQ->count = 0; //init count
	PQ->buff = (huffNode**)malloc(MAX_PQ_SIZE * sizeof(huffNode)); //allocate space for array of nodePtrs on heap
	
	return PQ;
}

	
void enqueue(pQueue* pQueue, huffNode* nodePtr)
{
	//error check - abort if pQueue is full
	if(pQueue->count == MAX_PQ_SIZE)
	{
		fprintf(stderr,"ERROR! Cannot enqueue beacuse priority queue is full!");
		return;
	}
		
	int count = pQueue->count; //number of elements currently in pQueue
	int newVal = nodePtr->val; //the character count of the new nodePtr added to pQueue 
	int val; //count of an existing element in pQueue used to compare with pQueue
	
	int newIndex = count; //init newIndex 
	
	/*-----------------------------------------
	if(DEBUG) 
	{	
		printf("queue size  = %d\n",count);
		printf("newchar = %c  ",nodePtr->key);
		printf("newVal = %d  ",nodePtr->val);
	}
	//-------------------------------------------*/
	
	if(count > 0)
	{
		do
		{
			val= pQueue->buff[newIndex-1]->val;

			if(val< newVal) 
			{
				newIndex--;
			}
			//------------------------------------------
			//printf("val = %d, newval = %d\n",val,newVal);
			//------------------------------------------ 
		}while((val < newVal) && (newIndex > 0)); 
	
		//move elements in queue to right to make room for new nodePtr if needed to 
		int i;
		for(i = count - 1; i >= newIndex ; i--) //wont run at all if newIndex has not been decremented(which only happens when newval is lowest in pQueue)
		{
			pQueue->buff[i+1] = pQueue->buff[i];
		}
	}
	
	//copy new nodePtr to right place
	pQueue->buff[newIndex] = nodePtr;
	
	//incr count
	pQueue->count = (pQueue->count) + 1; 	
}
	

huffNode* dequeue(pQueue* pQueue)
{
	//error check
	if(pQueue->count == 0)
	{
		fprintf(stderr,"ERROR! cannot dequeue because pQueue is empty!");
		return NULL;
	}
	
	int index = (pQueue->count) - 1; //index in pQueue of element to be dequeued 
	
	//get node to be dequeued
	huffNode* tmp = pQueue->buff[index];
	
	//decrement count
	pQueue->count = pQueue->count - 1;

	return tmp;
}


void preOrder(huffNode* root)
{
	//base case
	if(root==NULL) return;

	//print current node
	if((root->left== NULL) && (root->right == NULL)) //leaf
	{
		printf("[%c, %d]\n", root->key, root->val);;
		return;
	}
	else printf("%d\n",root->val);

	//traverse left
	preOrder(root->left);
	//traverse right 
	preOrder(root->right);
}

int getPath(char key, huffNode* root, char* path, int i)
{
	/* This function return the encoding(path) of a leaf node in a huffman tree as a string of 0's and 1's*/
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

int main(int argc, char** argv)
{
	//===============================================================
	//-------BUILD CHARACTER COUNTS TABLE---------------------------- 
	char tmp;
	int counts[257] = {0}; //declare counts array and init to 0's
	
	FILE* fp = fopen(argv[1],"r"); //declare file pointer, open input file

	do
	{
		tmp = (char)fgetc(fp); //read single char from file 
		if(tmp != EOF)
		{
			counts[(int)tmp] = counts[(int)tmp] + 1; //incr corresponding char count...
			
			/*----------------------------------------
			if(DEBUG)
			{
				printf("%c ",tmp);
				printf("\n\n");			
			}
			//----------------------------------------*/
		}
	}while(tmp != EOF); //...until EOF is encountered
	
	counts[256] = 1; //pseodo-EOF char - set count to 1
	//----------------------------------------------------------------
	//===============================================================
	//=============================================================
	//-------------BUILD FOREST OF TREES----------------------------
	
	pQueue* PQ = createPqueue(); //Declare Priority Queue
	huffNode* tmpPtr;
	int i,j;
	for(i = 0; i < 256; i++)
	{	
		if(counts[i] > 0)
		{
			//--------------------------------------------------
			if(DEBUG)
			{
				//printf("%c = %d \n",i, counts[i]);
			}
			//--------------------------------------------------
			
			//for every nonzero char count, create a huffman node and enqueue it 
			tmpPtr = createNode(i, counts[i]);  
			//printf("%d\n",tmpPtr->val);
			
			enqueue(PQ, tmpPtr);
			
		}//if
	}//for
	

	//create huffNode for pseudo EOF
	huffNode* eofNode = createNode(0,counts[256]); 	
		
	//enqueue pseudo EOF; 
	enqueue(PQ,eofNode);
	
	int charCount = PQ->count;
	
	//Now forest of one-node trees is complete and we can start to build huffMan tree
	
	//----------------------------------------------------------------------------
	//=============================================================================
	//======================================================================
	//-----------BUILD HUFFMAN TREE USING GREEDY ALGORITHM------------------

	/*--------------------------------
	if(DEBUG)
	{	
		printf("\nafter enqueing...");
		printf("\ncount = %d",PQ->count);
		for(j=0 ;j < (PQ->count);j++)
		{
			//printf("\nj = %d ",j);
			//printf("\n%p is at index %d, ",PQ->buff[j],j);
			printf("\n%c,%d is at index %d, ",PQ->buff[j]->key,PQ->buff[j]->val, j);
		}
		printf("\n\n");
	}
	//-------------------------------*/				
			
	huffNode* one,*two,*parent;
	int parentVal;

	while(PQ->count > 1)
	{
		//dequeue two nodes(dequeue gives us the node with lowest val in pQueue)
		one = dequeue(PQ);
		two = dequeue(PQ);

		// create 'parent' node whose val is the sum of the vals of the nodes dequeued
		parentVal = (one->val) + (two->val);
		parent = createNode(0,parentVal);

		//link parent to the two nodes that were dequeued
		parent->left = one;
		parent->right = two;

		//enqueue parent
		enqueue(PQ,parent);
	}		
 
	huffNode* huffRoot = dequeue(PQ);
	
	//---pre-order traversal
	//preOrder(huffRoot);
	
	//-----------------------------------------------------------------------
	//=======================================================================	
	//====================================================================
	//-------------BUILD HUFFMAN TABLE-----------------------------------
	
	//declare space on stack for huffman table, implemented as array of strings(array of pointers to char)
	char* huffTable[257];
	int k;
	//each pointer element in table points to a string on the heap
	for(k = 0;k<257;k++)
	{
		huffTable[k] = (char*)malloc(PATH_SIZE*sizeof(char));
	}
	
	//parse through char counts table and fill in the corresponding string in huffman table (for non-zero counts) 
	
	for(k = 0; k<257; k++)
	{
		if(counts[k] > 0)
		{
			getPath((char)k, huffRoot, huffTable[k], 0);
		}
	}

	/*-----------------------
	if(DEBUG)
		{
		for(k = 0; k<257; k++)
		{
			if(counts[k] > 0)
			{
				printf("path to %c is %s\n",k,huffTable[k]);
			}
		}
	}
	//----------------------*/
	
	//free huffman tree
	
	//--------------------------------------------------------------------
	//====================================================================
	//======================================================================
	//---------WRITE ENCODED OUTPUT TO COMPRESSED FILE------------------------
	
	//open output file
	
	FILE* fp2 = fopen("output.txt","wb+");
	
	//-----print header - nonzero char-val pairs from character-counts table 
	//first 4 bytes (int) is the number of char-val pairs
	//size of char-val pair = 4 bytes + 4bytes = 8ytes (char is stored as int)
	//size of header = (4 + count*8) bytes
	int headerSize = 4 + charCount*8;
	
	printf("header Size is %d\n",headerSize);
	//size_t fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream)
	if((fwrite((void*)&headerSize, sizeof(int), 1, fp2)) != 1)
	{
		fprintf(stderr,"ERROR! Header was not written to file!");
	}
	
	//store nonzero pairs from charCounts table
	for(k = 0; k<257; k++)
	{
		if(counts[k] > 0)
		{
					
			if((fwrite((void*)&k, sizeof(int), 1, fp2)) != 1)
			{
				fprintf(stderr,"ERROR! Char was not written to file!");
			};
			if((fwrite((void*)&counts[k], sizeof(int), 1, fp2)) != 1)
			{
				fprintf(stderr,"ERROR! Count was not written to file!");
			};
		}
	}
	
	//-----------------------------------
	//size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream)
	int testSize;
	fseek(fp2, SEEK_SET, 0);
	int numRead = fread((void*)&testSize, sizeof(int), 1, fp2);
	printf("numRead = %d, testSize is %d\n",numRead, testSize); 
	
	for(k = 0; k<charCount; k++)
	{
		
		fread((void*)&testSize, sizeof(int), 1, fp2);
		printf("%d, char is %d   ",numRead, testSize); 
	
		fread((void*)&testSize, sizeof(int), 1, fp2);
		printf("%d, count is is %d\n",numRead, testSize); 
	}
	printf("\n");
	//-----------------------------*/
	
	//---write encoded ouput to compressed file----
	//get next char from input file
	
	//get correspoding path from huffman table
	
	//parse path and write to byte buffer
	
	//when buffer is full, flush it to compressed file, reset counter and carry on
	
	//do this until EOF char is reached
	
	//write pseudo-eof char to file  
	
	//flush buffer and close file
	fclose(fp2);
	
	//close input file
	fclose(fp);
	//-----------------------------------------------------------------------
	//====================================================================

	 
	return 0;
}



	/*	
	printf("\nafter enqueing...");
	printf("\ncount = %d",PQ->count);
	for(j=0 ;j < (PQ->count);j++)
	{
		//printf("\nj = %d ",j);
		//printf("\n%p is at index %d, ",PQ->buff[j],j);
		printf("\n%c,%d is at index %d, ",PQ->buff[j]->key,PQ->buff[j]->val, j);
	}
	printf("\n\n");
		
	int size = PQ->count;	
		
	for(j = 0; j < size; j++)
	{
		dequeue(PQ);
	}	
		
	printf("\nafter dequeing...");
	printf("\ncount = %d",PQ->count);
	for(j=0 ;j < (PQ->count);j++)
	{
		//printf("\nj = %d ",j);
		//printf("\n%p is at index %d, ",PQ->buff[j],j);
		printf("\n%c,%d is at index %d, ",PQ->buff[j]->key,PQ->buff[j]->val, j);
	}
	printf("\n\n");
	
	*/	




