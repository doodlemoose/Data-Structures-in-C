//helper functions for myshortestpath.c


#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "myshortestpath.h"

#define MAX_PQ_SIZE 100000


//function to compute the Euclidean distance between 2 coordinates
int euclidean(int x1, int y1, int x2, int y2)
{
	double distance = (pow((double)(x2 - x1),2) + pow((double)(y2 - y1),2));
	
	distance = sqrt((double)distance);
  return (int)distance;
}

//creates and inits a node in the ajacency list data structure 
listNode* createListNode(int nodeNum, unsigned int weight)
{
	listNode* tmp = (listNode*) malloc(sizeof(listNode));
	tmp->node = nodeNum;
	tmp->weight = weight;
	tmp->next = NULL;
	
	return tmp;
}

//This function initializes the weights of all the vertices (first part of Dijkstra's algorithm
//The SOURCE vertex's weight is 0 and all others are infinity
void initWeights(listNode* ajaList, int vertices, int root)
{
	int i;
	for(i= 0; i<vertices; i++)
	{
		if(i == root) ajaList[i].weight = 0;
		else ajaList[i].weight = -1;
	}
}

//This function looks at all neighbours of a vertex and checks for shorter paths to them 
void updateWeights(listNode* ajaList, int currentVertex, unsigned char* done, int* preceding)
{
	listNode* ajaPtr = ajaList[currentVertex].next; //init to first neighbour
	unsigned int rootWeight = ajaList[currentVertex].weight; //save weight of current Vertex
	
	unsigned int newWeight, oldWeight, inf = -1; 
	
	while(ajaPtr !=NULL)
	{
		
		if(!(done[ajaPtr->node])) //Ignore nodes already visted 
		{
			newWeight = ajaPtr->weight;
			oldWeight = ajaList[ajaPtr->node].weight;
			
			if((rootWeight + newWeight) < (oldWeight)) //if shorter path is found
			{
				if(oldWeight == inf)
				{
					enqueue(&ajaList[ajaPtr->node]); //add to PQ
				}
				
				ajaList[ajaPtr->node].weight = rootWeight + newWeight; //update path length
				
				preceding[ajaPtr->node] = currentVertex; //update preceding node
			}
		}
		ajaPtr = ajaPtr->next; //update ajacency list pointer	
	} 
}

queryNode* createQueryNode(int node, queryNode* prev)
{
	//alocate space on stack
	queryNode* tmp = (queryNode*)malloc(sizeof(queryNode));
	
	tmp->node = node;
	tmp->prev = prev;
	tmp->next = NULL;
	
	return tmp; 
}

void deleteQueryList(queryNode* head)
{
	queryNode* curr = head, *next;
	
	while(curr!=NULL)
	{
		//save pointer to next node
		next = curr->next;
		
		//delete current node
		free(curr);
		
		//update current node
		curr = next;
	}
}


void showHeap()
{
	int i = 0;
	printf("\n");
	printf("PQsize: %d\n",pQueue.size);
	for(i = 0; i<pQueue.size; i++)
	{
		printf("%d(%u)  ", pQueue.heap[i]->node, pQueue.heap[i]->weight);
	}
	printf("\n\n");

}

void downHeap(int i, int n)
{
	listNode* tmp = pQueue.heap[i-1];
	int j;
	
	while((i-1) < (n/2)) //not a leaf
	{
		j = 2*i; //rightchild
		if((j < n) && (pQueue.heap[j-1]->weight > pQueue.heap[j]->weight)) //find min child
			j++;
		if(tmp->weight <= pQueue.heap[j-1]->weight)
			break;
		else
		{
			pQueue.heap[i-1] = pQueue.heap[j-1];
			i = j;
		} 
	}
	
	pQueue.heap[i-1] = tmp;
}





void buildHeap()
{
	int i;
	int n = pQueue.size;
	
	for( i = n/2; i > 0; i--)
	{
		downHeap(i,n);
	}
}


listNode* dequeue()
{	
	//error check
	if(pQueue.size == 0)
	{
		fprintf(stderr,"ERROR! cannot dequeue because pQueue is empty!");
		return NULL;
	}
	listNode* tmp = pQueue.heap[0];
	pQueue.heap[0] = pQueue.heap[pQueue.size-1];

	pQueue.size = pQueue.size - 1;
	return tmp;
}


void enqueue(listNode* item)
{
	//error check - abort if pQueue is full
	if(pQueue.size == MAX_PQ_SIZE)
	{
		fprintf(stderr,"ERROR! Cannot enqueue beacuse priority queue is full!");
		return;
	}
	pQueue.heap[pQueue.size] = item;
	pQueue.size = pQueue.size + 1;	
}

void initPQ()
{
	//init global PQ
	pQueue.size = 0;
	pQueue.heap = (listNode**)malloc(MAX_PQ_SIZE*sizeof(listNode*));
}

void resetPQ()
{
	//init global PQ
	pQueue.size = 0;
}

