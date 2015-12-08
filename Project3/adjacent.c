#include <math.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct coordinates{
  int x;
  int y;
}coordinates;

struct listNode{
  unsigned int node;
  unsigned int weight;
 	struct listNode* next;
};

typedef struct listNode listNode;
/*
struct done{
	int vertex;
	short weight;
	unsigned done;
};

typedef struct minPath minPath;

void initMinPaths(minPath* minPaths, int numVertices, int rootVertex);
*/

void initWeights(listNode* ajaList, int vertices, int root);

listNode* createListNode(int nodeNum, int weight);

int euclidean(int x1, int y1, int x2, int y2);

void updateWeights(listNode* ajaList, int currentVertex, unsigned char* done);

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
	
	/*-----------Print Coordinates List----
	printf("\nCoordinates List: \n");
	for(i = 0;i <vertices; i++)
	{
		printf("%d: %d %d\n",i,(cords + i)->x,(cords + i)->y); 
	}
	
	//------------------------------------------------*/
	
	
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
  //---------CREATE minPATHS array-------------
  minPath* minPaths = (minPath*)malloc(vertices*sizeof(minPath));
  

  unsigned int a = -1;
  unsigned short b = -1;
  unsigned char c = -1;
  printf("%u,%u,%u",a,b,c);  

	int x1=0, x2=4,y1=0,y2=4;

	printf("\nThe distance is %d",euclidean(x1,y1,x2,y2));
*/
	int root = 0;
	initWeights(ajaList, vertices, root);

		//--------PRINT OUT INIT WEIGHTS--
	printf("\n\nInit Weights:\n");
	for(i= 0; i< vertices; i++)
	{
		printf("%d(%u)\n",i,ajaList[i].weight);
	}	
	
	//------create/init 'DONE' array on heap
	unsigned char* visited = (unsigned char*)calloc(vertices, sizeof(unsigned char)); //use calloc to init to zeros(not visited)
	  
	//--------PRINT OUT INIT VISITED--
	printf("\n\nInit Visited :\n");
	for(i= 0; i< vertices; i++)
	{
		printf("%d(%u)\n",i,visited[i]);
	}	
	
	unsigned int min = -1;
	unsigned int min2 = -1;
	
	int currentVertex= 0;
	updateWeights(ajaList, currentVertex, visited);
	
	//Mark current vertex as 'visited'
	visited[currentVertex] = 1;
	
	
		//--------PRINT OUT WEIGHTS--
	printf("\n\nUpdated Weights:\n");
	for(i= 0; i< vertices; i++)
	{
		printf("%d(%u)\n",i,ajaList[i].weight);
	}	
		
	//--------PRINT OUT VISITED--
	printf("\n\nUpdated Visited :\n");
	for(i= 0; i< vertices; i++)
	{
		printf("%d(%u)\n",i,visited[i]);
	}	
	
//------------------TEST OUT UPDATEWIEGHTS-----------	
	int verts[] = {1,2,3};
	int j;
	for(j =0;j<3; j++)
	{	
		currentVertex = verts[j];
		updateWeights(ajaList, currentVertex, visited);
	
		//Mark current vertex as 'visited'
		visited[currentVertex] = 1;
	
	
			//--------PRINT OUT WEIGHTS--
		printf("\n\nUpdated Weights:\n");
		for(i= 0; i< vertices; i++)
		{
			printf("%d(%u)\n",i,ajaList[i].weight);
		}	
		
		//--------PRINT OUT VISITED--
		printf("\n\nUpdated Visited :\n");
		for(i= 0; i< vertices; i++)
		{
			printf("%d(%u)\n",i,visited[i]);
		}	
		
	}
//-------------------------------------------------------	
	
	//printf("\n\n\n char-%lu, short-%lu, int-%lu, long-%lu\n",sizeof(char),sizeof(short),sizeof(int),sizeof(long));
	

	
	fclose(fp);

	return 0;
}//END OF MAIN





//function to compute the Euclidean distance between 2 coordinates
int euclidean(int x1, int y1, int x2, int y2)
{
	double distance = (pow((double)(x2 - x1),2) + pow((double)(y2 - y1),2));
	
	distance = sqrt((double)distance);
  return (int)distance;
}


listNode* createListNode(int nodeNum, int weight)
{
	listNode* tmp = (listNode*) malloc(sizeof(listNode));
	tmp->node = nodeNum;
	tmp->weight = weight;
	tmp->next = NULL;
	
	return tmp;
}

/*
void initMinPaths(minPath* minPaths, int numVertices, int rootVertex)
{
	int i;
	for(i= 0; i<numVertices; i++)
	{
		minPaths[i].vertex = i;
		minPaths[i].done = 0;
		
		if(i == rootVertex) minPaths[i].weight = 0;		
		else minPaths[i].weight = -1;
	}
}
*/

//This function initializes the ajacency list to prep for Dijkstra's shortest path algorithm from vertex 'root' (weight of root = 0, all other vertices = inf )
void initWeights(listNode* ajaList, int vertices, int root)
{
	int i;
	for(i= 0; i<vertices; i++)
	{
		if(i == root) ajaList[i].weight = 0;
		else ajaList[i].weight = -1;
	}
}

//Part 1 of my iterative implementation of Dijkstra's shortest path algorithm
//This function looks at all neighbours of a vertex and checks for shorter paths to them 
void updateWeights(listNode* ajaList, int currentVertex, unsigned char* done)
{
	listNode* ajaPtr = ajaList[currentVertex].next; //init to first neighbour
	unsigned int rootWeight = ajaList[currentVertex].weight; //save weight of current Vertex
	
	unsigned int newWeight, oldWeight;
	
	while(ajaPtr !=NULL)
	{
		printf("\n Iter");
		
		if(!(done[ajaPtr->node])) //Ignore nodes already visted 
		{
			printf("iter ");
			newWeight = ajaPtr->weight;
			oldWeight = ajaList[ajaPtr->node].weight;
		
			printf("\noldweight:%u  , newweight:%u",oldWeight,newWeight);
		
			if((rootWeight + newWeight) < (oldWeight)) //if shorter path is found
			{
				ajaList[ajaPtr->node].weight = rootWeight + newWeight; //update path length
			}
		}
		ajaPtr = ajaPtr->next; //update ajacency list pointer	
	} 
}
