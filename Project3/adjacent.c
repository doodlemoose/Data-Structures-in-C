#include <math.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct coordinates{
  int x;
  int y;
}coordinates;

struct listNode{
  int node;
 	struct listNode* next;
};

typedef struct listNode listNode;

listNode* createListNode(int nodeNum)
{
	listNode* tmp = (listNode*) malloc(sizeof(listNode));
	tmp->node = nodeNum;
	tmp->next = NULL;
	
	return tmp;
}

//function to compute the Euclidean distance between 2 coordinates
int euclidean(int x1, int y1, int x2, int y2)
{
	double distance = (pow((double)(x2 - x1),2) + pow((double)(y2 - y1),2));
	
	distance = sqrt((double)distance);
  return (int)distance;
}

int main(int argc,char** argv)
{	
	FILE* fp = fopen(argv[1],"r"); //declare file pointer, open input file

	int vertices, edges,tmpX,tmpY,tmpUnused,i;
	
	fscanf(fp, "%d %d", &vertices, &edges); //get number of vertices and edges
	if(feof (fp)) return -1;
	
	printf("\nNumber of vertices are %d, number of edges are %d\n",vertices, edges);
	
	coordinates* cords = (coordinates*) malloc(vertices * sizeof(coordinates));
	
	
	//-----------TEST THIS PART!!---------------------
	for(i= 0; i< vertices; i++)
	{
		if(feof (fp)) break;
		
		//Read all coordinates from file in array on heap
		fscanf(fp, "%d %d %d", &tmpUnused, &tmpX, &tmpY);
		(cords + i)->x = tmpX;
		(cords + i)->y = tmpY; 
	}
	if(feof (fp)) return -1;
	//------------------------------------------------
	
	
	//----------Create adjacency list-----------------
	
	listNode* ajaList = (listNode*)malloc(vertices*sizeof(listNode)); 	
	
	//--------REMOVE THIS IF POSSIBLE--
	for(i= 0; i< vertices; i++)
	{
		ajaList[i].node = i;
		ajaList[i].next = NULL; //init pointers to NULL
	}	
	
	//-----------------------------------------------
	
	
	//----------FILL adjacency list-----------------
	
	//create an array of pointers to listNode
	listNode** pointers = (listNode**)malloc(vertices*sizeof(listNode*)); 	
	
	//--------REMOVE IF POSSIBLE--
	for(i= 0; i< vertices; i++)
	{
		pointers[i]= &ajaList[i]; //init pointers to point the corresponding nodes in the adjacency list
	}	
	
	int A, B; //vertices of an edge
	listNode* tmp;
	
	for(i= 0; i< edges; i++)
	{
		//assign A and B from file
		fscanf(fp, "%d %d", &A, &B); //get number of vertices and edges
		
		//printf("A = %d, B= %d\n",A,B);
		//add connection to adjacency list of appropriate node
		tmp = createListNode(B);
		pointers[A]->next = tmp;
		pointers[A] = tmp;
	
		tmp = createListNode(A);
		pointers[B]->next = tmp;
		pointers[B] = tmp;
	}	
  //----------------------------------------
  
  
  	
	//--------PRINT OUT ADJACENCY LIST--
	printf("\n");
	for(i= 0; i< vertices; i++)
	{
		printf("%d: ",i);
		tmp = ajaList[i].next;
		while(tmp!= NULL)
		{
			printf("%d ",tmp->node);
			tmp = tmp->next;
		}
		printf("\n");
	}	
  
  
  
/*
	int x1=0, x2=4,y1=0,y2=4;

	printf("\nThe distance is %d",euclidean(x1,y1,x2,y2));
*/
	fclose(fp);

	return 0;
}
