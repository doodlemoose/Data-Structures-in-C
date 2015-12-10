#include<stdio.h>
#include<stdlib.h>

#define MAX_PQ_SIZE 20

struct listNode{
  int node;
  unsigned int weight;
 	struct listNode* next;
};

typedef struct listNode listNode;


struct PQ{
	int size;
	listNode** heap;
};

typedef struct PQ PQ;

PQ pQueue;

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
			{j++; /*printf(" A");*/}
		if(tmp->weight <= pQueue.heap[j-1]->weight)
			{/*printf(" B");*/break;}
		else
		{
			pQueue.heap[i-1] = pQueue.heap[j-1];
			i = j;
			/*printf(" C");*/
		} 
		//showHeap();
	}
	
	pQueue.heap[i-1] = tmp;
}





void buildHeap()
{
	int i;
	int n = pQueue.size;
	listNode* tmp;
	
	
	for( i = n/2; i > 0; i--)
	{
		//printf("i = %d, n= %d\n",i,n);
		downHeap(i,n);
		//showHeap();
	}
	
	//showHeap();
	/*
	for( i = n-1; i > 0; i--)
	{
		tmp = pQueue.heap[0];
		pQueue.heap[0] = pQueue.heap[i];
		pQueue.heap[i] = tmp;
		
		downHeap(1,i);
	}*/
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
	//downHeap(1,pQueue.size-1); //remove?
	
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
	
	//addUPHeapify?
}



void initPQ()
{
	//init global PQ
	pQueue.size = 0;
	pQueue.heap = (listNode**)malloc(MAX_PQ_SIZE*sizeof(listNode*));
}


void initAjaList(listNode* ajaList, int size)
{
	int i;
	for(i=0; i<size; i++)
	{
		ajaList[i].node = i;
		ajaList[i].weight = (30*i) % 57;
		ajaList[i].next = NULL;
	}
}

int main()
{
	int vertices = 10;

	initPQ();
	
	listNode* ajaList = (listNode*)malloc((vertices+1)*sizeof(listNode)); //use calloc to init pointers to NULL
	initAjaList(ajaList,vertices);	

	showHeap();
	
	enqueue(ajaList+5);
	enqueue(ajaList+7);
	enqueue(ajaList+4);
	enqueue(ajaList+1);
	enqueue(ajaList+2);
	enqueue(ajaList+9);
	enqueue(ajaList+8);
	
	showHeap();

	buildHeap();

	showHeap();

	ajaList[1].weight = 2;
	ajaList[9].weight = 20;
	ajaList[7].weight = 39;
	
	enqueue(ajaList+6);
	
	showHeap();
	
	buildHeap();

	showHeap();
	
	
	return 0;
}

