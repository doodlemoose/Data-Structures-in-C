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


int main()
{
	char tmp;
	int counts[257] = {0}; //declare counts array and init to 0's
	
	FILE * fp = fopen("input.txt","r"); //declare file pointer

	
	
	do
	{
		tmp = (char)fgetc(fp); //read single char from file 
		if(tmp != EOF)
		{
			counts[(int)tmp] = counts[(int)tmp] + 1; //incr corresponding char count...
			
			//----------------------------------------
			if(DEBUG)
			{
				printf("%d ",tmp);
				printf("\n\n");			
			}
			//----------------------------------------
		}
	}while(tmp != EOF); //...until EOF is encountered
	
	counts[256] = 1; //pseodo-EOF char - set count to 1
	
	
	//---------------------------------------------------------
	int i;
	if (DEBUG)
	{
		for(i = 0; i < 256; i++)
		{	
			if(counts[i] > 0)
			{
				printf("%d = %d\n",i, counts[i]);
			}
		}
	printf("%c = %d\n",(char)i, counts[i]);
	}
	//---------------------------------------------------------
	
	
	fclose(fp);
	 
	return 0;
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
	
typedef struct priorityQueue{
	
	int count;
	huffNode* buff[MAX_PQ_SIZE];
}pQueue;
	
	/* 
void enqueue(pQueue PQ, huffNode* nodePtr)
{
	//error check - abort if PQ is full
	if(PQ->count == MAX_PQ_COUNT)
	{
		fprintf(stderr,"ERROR! Cannot enqueue beacuse priority queue is full!");
		return;
	}

}
	
*/	
}

