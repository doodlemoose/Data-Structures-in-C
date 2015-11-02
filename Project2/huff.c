#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#define DEBUG 1
#define MAX_PQ_SIZE 260 //count when PQ is full  	




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

	
void enqueue(pQueue * pQueue, huffNode* nodePtr)
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
	
	//-----------------------------------------
	if(DEBUG) 
	{
		printf("newVal = %d  ",nodePtr->val);
		printf("count = %d\n",count);
	}
	//-------------------------------------------
	
	if(count > 0)
	{
		//printf("newIndex = %d ",newIndex);
		
		//find right place for new nodePtr in pQueue
		//val= pQueue->buff[newIndex-1]->val; //init to last element in queue
		//huffNode* tmp = pQueue->buff[newIndex-1];
		//if(tmp == NULL)
		//{
		//	fprintf(stderr,"ERROR! pQueue->buff[newIndex-1] = NULL ! ");  
		//}
		
		//val = tmp->val;
	
		
		
		do
		{
			printf("hello from inside while loop\n");
			if(count==4) printf("[newIndex-1] = %d , buff[newIndex-1] = %p\n",newIndex-1,pQueue->buff[newIndex-1]);
			
			val= pQueue->buff[newIndex-1]->val;
			newIndex--;

			//------------------------------------------
			//printf("val = %d, newval = %d\n",val,newVal);
			//------------------------------------------ 
		}while((val < newVal) && (newIndex > 0)); 
	

		//move elements in queue to right to make room for new nodePtr  
		int i;
		for(i = newIndex; i < count; i++) //wont run at all if newIndex has not been decremented(which only happens when newval is lowest in pQueue)
		{
			pQueue->buff[newIndex+1] = pQueue->buff[newIndex];
		}
	}
	
	//copy new nodePtr to right place
	pQueue->buff[newIndex] = nodePtr;
	
	//incr count
	pQueue->count = (pQueue->count) + 1; 

	
}
	





int main()
{
	char tmp;
	int counts[257] = {0}; //declare counts array and init to 0's
	
	FILE* fp = fopen("input.txt","r"); //declare file pointer

	do
	{
		tmp = (char)fgetc(fp); //read single char from file 
		if(tmp != EOF)
		{
			counts[(int)tmp] = counts[(int)tmp] + 1; //incr corresponding char count...
			
			/*----------------------------------------
			if(DEBUG)
			{
				printf("%d ",tmp);
				printf("\n\n");			
			}
			//----------------------------------------*/
		}
	}while(tmp != EOF); //...until EOF is encountered
	
	counts[256] = 1; //pseodo-EOF char - set count to 1
	
	huffNode* tmpPtr;
	
	//Declare Priority Queue
	pQueue* PQ = createPqueue();
	
	
	int i,j;
	for(i = 0; i < 256; i++)
	{	
		if(counts[i] > 0)
		{
			/*--------------------------------------------------
			if(DEBUG)
			{
				printf("%c = %d \n",i, counts[i]);
			}
			//--------------------------------------------------*/
			
			//for every nonzero char count, create a huffman node and enqueue it 
			tmpPtr = createNode(i, counts[i]);  
			//printf("%d\n",tmpPtr->val);
			
			enqueue(PQ, tmpPtr);
			
			printf("count = %d",PQ->count);
			for(j=0 ;j < (PQ->count);j++)
			{
				printf("\nj = %d ",j);
				//printf("%c,%d is at index %d, ",PQ->buff[j]->key,PQ->buff[j]->val, j);
			}
			printf("\n");
	}

	}
		
		
		printf("pseudo-EOF = %d\n", counts[i]);
		//create huffNode for pseudo EOF
		
		//enqueue pseudo EOF; 
		
		
		
	
	fclose(fp);
	 
	return 0;
}




