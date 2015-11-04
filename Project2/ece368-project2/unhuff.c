#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>
#include"huff.h"

#define DEBUG 1
#define MAX_PQ_SIZE 260 //count when PQ is full  	
#define PATH_SIZE 257


int main(int argc, char** argv)
{
	clock_t t;
    t = clock(); //start timer


	//open compressed file
	FILE* fp = fopen(argv[1], "rb");
	
	//read header size
	
	int headerSize;
	//fseek(fp2, SEEK_SET, 0);
	if((fread((void*)&headerSize, sizeof(int), 1, fp)) != 1)
	{
		fprintf(stderr,"ERROR! Header not read!"); 
	}
	
	//printf("header = %d bytes",headerSize);

	
	
	//parse through char-val pairs and recreate char counts table
	
	char tmpChar;
	int tmpCount;
	
	int counts[257] = {0}; //declare counts array and init to 0's
	
	int k;
	for(k = 0; k < ((headerSize-4)/5) -1; k++)
	{
		if((fread((void*)&tmpChar, sizeof(char), 1, fp)) != 1)
		{
			fprintf(stderr,"ERROR! Char was not read from file!");
		};
		if((fread((void*)&tmpCount, sizeof(int), 1, fp)) != 1)
		{
			fprintf(stderr,"ERROR! Count was not read from file!");
		};
		
		counts[(int)tmpChar] = tmpCount;
	}
	
	/*------------------------------
	for(k = 0; k < 256; k++)
	{
		printf("counts[%c] = %d\n",(char)k,counts[k]); 
	}
	printf("\n");
	//-----------------------------*/
	
	//add pseudo-eof char with count = 1 to counts table
	counts[256] = 1; 
	
	
	//build huffman tree  
	
	//=============================================================
	//-------------BUILD FOREST OF TREES----------------------------
	
	pQueue* PQ = createPqueue(); //Declare Priority Queue
	huffNode* tmpPtr;
	int i;
	//int j;
	for(i = 0; i < 256; i++)
	{	
		if(counts[i] > 0)
		{
			//--------------------------------------------------
			if(DEBUG)
			{
				//printf("%x = %x \n",i, counts[i]);
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
	
	//int charCount = PQ->count;
	
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
	
	
	//======================================================================
	// -------parse through compressed file-----------------------
	
	//open output file
	
	const char * postfix = ".unhuff";
	int needed = strlen(argv[1]) + strlen(postfix) + 1;

	char store[needed];

	strcpy(store, argv[1]);                   
	strcat(store, postfix);                 // append ".unhuff" to compressed file name

	
	FILE* fp2 = fopen(store,"w+");
	//declare and init mask and buffer
	unsigned char mask = 0x80;
	unsigned char buffer = 0x00;
	int nextBit;
	
	huffNode* curr = huffRoot;
	//printf("\nnow at %d", curr->val); 
	
	//read first byte
	if((fread((void*)&buffer, sizeof(char), 1, fp)) != 1)
	{
		fprintf(stderr,"ERROR! Byte not read into buffer!"); 
	}
	else
	{
		//printf("\nByte read: %x",buffer);	
	} 
	
	do
	{
		//error check
		if(curr == NULL) break;
		
		//clear, then read next bit
		nextBit = 0;
		nextBit = mask & buffer; 
		
		//printf("\nNext bit = %x & %x = %x",mask,buffer,nextBit);
		
		//move down tree accordingly
		if(nextBit)
		{
			curr = curr->right;
			//printf("\nnow at %d", curr->val); 
		}
		else 
		{
			curr = curr->left;
			//printf("\nnow at %d", curr->val);
		}
		
		
		//----------------
		if((curr->left == NULL) && (curr->right == NULL))
		{
			//printf("\nReached a leaf node with %c",curr->key);
		}
		//-------------------
		
		//if we reach a leaf node, 
		if((curr->left == NULL) && (curr->right == NULL))
		{
			//check if the node's key is the pseudo-eof char
			//printf("\nReached a leaf node with %c",curr->key);
			if(curr->key == 0)
			{
				//stop parsing input file if pseudo-eof is reached
				break;
			}
			else
			{
				//if not pseudo-eof, write the node's key to the output file as an ASCII value, and move back to root   
				if (fputc(curr->key,fp2) == EOF)
				{
					//printf("%c",curr->key);
					fprintf(stderr,"ERROR! Could not put char into output file");
				}
				else
				{
					//printf("\n put %c in output file",curr->key);
				}
				curr = huffRoot;
				//printf("\nnow BACK at %d", curr->val);
			}
		}
	
	
		//shift mask 
		mask = mask >> 1;
		//if mask is 0, reset it ,rewrite buffer with next byte from compressed file
		if(mask == 0)
		{
			if((fread((void*)&buffer, sizeof(char), 1, fp)) != 1)
			{
				fprintf(stderr,"ERROR! Byte not read into buffer!"); 
			}
		else
		{
			//printf("\nbyte read: %x",buffer);	
		} 
			mask = 0x80; 
		}
	
	}while(1);//keep on doing this 
	
	//close files
	fclose(fp);
	fclose(fp2);
	
	
	t = clock() - t;
    double time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds
	printf("File successfully unhuffed\n");
	printf("Time taken : %lf seconds\n",time_taken); 
	
	return 0;
}


	
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



