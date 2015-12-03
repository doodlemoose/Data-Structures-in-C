#include <math.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct coordinates{
  int x;
  int y;
}coordinates;

struct listNode{
  int node;
  int weight;
 	struct listNode* next;
};

typedef struct listNode listNode;

listNode* createListNode(int nodeNum, int weight)
{
	listNode* tmp = (listNode*) malloc(sizeof(listNode));
	tmp->node = nodeNum;
	tmp->weight = weight;
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
	
	
	//-----------Store coordinates from file in struct array!!---------------------
	for(i= 0; i< vertices; i++)
	{
		if(feof (fp)) break;
		
		//Read all coordinates from file in array on heap
		fscanf(fp, "%d %d %d", &tmpUnused, &tmpX, &tmpY);
		(cords + i)->x = tmpX;
		(cords + i)->y = tmpY; 
	}
	if(feof (fp)) return -1;
	
	//-----------Print Coordinates List----
	printf("\nCoordinates List: \n");
	for(i = 0;i <vertices; i++)
	{
		printf("%d: %d %d\n",i,(cords + i)->x,(cords + i)->y); 
	}
	
	//------------------------------------------------
	
	
	//----------Create adjacency list-----------------
	
	//listNode* ajaList = (listNode*)malloc(vertices*sizeof(listNode)); 	
	listNode* ajaList = (listNode*)calloc(vertices, sizeof(listNode)); //use calloc to init pointers to NULL
	/*--------Remove this is possible? ..Need for print statement?--
	for(i= 0; i< vertices; i++)
	{
		ajaList[i].node = i;
		ajaList[i].next = NULL; //init pointers to NULL
	}	
	
	//-----------------------------------------------*/
	
	
	//----------FILL adjacency list-----------------
	
	//create an array of pointers to point to the last listNode of each vertex's adjacency list
	//this way we dont have to iterate to the end of the list to add a node 
	listNode** pointers = (listNode**)malloc(vertices*sizeof(listNode*)); 	
	
	//init pointers to POINT TO the corresponding nodes in the adjacency list
	for(i= 0; i< vertices; i++)
	{
		pointers[i]= &ajaList[i]; //pointers[i] points to ajaList[i]
	}	
	
	int A, B; //vertices of an edge
	int x1,y1,x2,y2;
	listNode* tmp;
	
	for(i= 0; i< edges; i++)
	{
		//assign A and B from file
		fscanf(fp, "%d %d", &A, &B); //get connection
		
		//printf("A = %d, B= %d\n",A,B);
		//add connection to adjacency list of appropriate node
		x1=(cords+B)->x;
		x2=(cords+A)->x;
		y1=(cords+B)->y;
		y2=(cords+A)->y;
		tmp = createListNode(B,euclidean(x1, y1, x2, y2));
		pointers[A]->next = tmp;
		pointers[A] = tmp;
	
		tmp = createListNode(A,euclidean(x1, y1, x2, y2));
		pointers[B]->next = tmp;
		pointers[B] = tmp;
	}	
  //----------------------------------------
  
  
  	
	//--------PRINT OUT ADJACENCY LIST--
	printf("\nAdjacency List:\n");
	for(i= 0; i< vertices; i++)
	{
		printf("%d: ",i);
		tmp = ajaList[i].next;
		while(tmp!= NULL)
		{
			printf("%d(%d)  ",tmp->node,tmp->weight);
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
