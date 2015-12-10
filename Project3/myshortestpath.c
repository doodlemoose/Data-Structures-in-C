#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "myshortestpath.h"

#define MAX_PQ_SIZE 100000

clock_t t;

int main(int argc,char** argv)
{
	
	//------------PARSE INPUT FILE------------------
	
	FILE* fp = fopen(argv[1],"r"); //declare file pointer, open input file
	FILE* fp2 = fopen(argv[2],"r"); //declare file pointer, open query file

	//local variables for this section 
	int vertices, edges,tmpX,tmpY,tmpUnused,i;
	
	fscanf(fp, "%d %d", &vertices, &edges); //get number of vertices and edges
	if(feof (fp)) return -1;
	
	//printf("\nNumber of vertices are %d, number of edges are %d\n",vertices, edges);
	
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
	
  

	//----------Create adjacency list-----------------
	
	//Local variable for this section
	int A, B; //vertices of an edge
	int x1,y1,x2,y2; //cords
	listNode* tmp;
	
	
	//ajaList[vertices+1] is a dummy node used later 
	listNode* ajaList = (listNode*)calloc(vertices+1, sizeof(listNode)); //use calloc to init pointers to NULL
	
	//init pointers adjacency list
	for(i= 0; i< vertices; i++)
	{
		ajaList[i].node = i; 
	}	
	
	
	//----------FILL adjacency list-----------------
	
	//create an array of pointers to point to the last listNode of each vertex's adjacency list
	//this way we dont have to iterate to the end of the list to add a node 
	listNode** pointers = (listNode**)malloc(vertices*sizeof(listNode*)); 	
	
	//init pointers to POINT TO the corresponding nodes in the adjacency list
	for(i= 0; i< vertices; i++)
	{
		pointers[i]= &ajaList[i]; //pointers[i] points to ajaList[i]
	}	
		
	for(i= 0; i< edges; i++)
	{
		//assign A and B from file
		fscanf(fp, "%d %d", &A, &B); //get connection

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

  	
	/*--------PRINT OUT ADJACENCY LIST--
	printf("\nAdjacency List:\n");
	int count;
	for(i= 0; i< vertices; i++)
	{
		//printf("%d: ",i);
		tmp = ajaList[i].next;
		count = 0;
		while(tmp!= NULL)
		{
			//printf("%d(%u)  ",tmp->node,tmp->weight);
			tmp = tmp->next;
			count++;
		}
		if(count==0) printf("Unused:%d\n",i);
		//printf("\n");
	}	
	*/

	//--------------------------------MILESTONE 2 COMPLETE------------------------------
	
	//local variables for this section 
	int* preceeding; //data structure used to help print the shortest path 
	int numQueries; //number of queries in query file
	unsigned char* visited; //data structure used to keep track of visited nodes
	int k,j; //loop counters
	int rootVertex, currentVertex; //used for to keep track of vertices when using Dijkstra's algorithm 
	unsigned int min; //minimum distance of all unvisited vertices
	int query,prev; //used to print out shortest path 
	queryNode* queryPtr, *start; //used to make a linked list of the nodes in a shortest path  
		
	//get number of queries from file (so we know how many times to run the following loop
	if((fscanf(fp2,"%d",&numQueries)) == 0) { fprintf(stderr,"ERROR! COULD NOT READ NUMQUERIES FROM FILE"); return 0;}
	
	//printf("\n Numqueries is %d\n", numQueries);
	
	
	//------declare 'PRECEEDING' array on heap
	preceeding = (int*)malloc(vertices*sizeof(int)); 
	
	//------declare 'visited' array on heap
	visited = (unsigned char*)malloc(vertices*sizeof(unsigned char)); 
	
	//init prority queue
	initPQ();
	
	//start loop
	for(k=0; k<numQueries; k++)
	{
	
		//get SOURCE vertex from query file
		if((fscanf(fp2,"%d",&rootVertex)) == 0) { fprintf(stderr,"ERROR! COULD NOT READ QUERY FROM FILE"); return 0;}

		//start at the source vertex 
		currentVertex = rootVertex;
	

		//init preceeding & visited arrays
		for(i=0;i<vertices; i++)
		{
			preceeding[i] = -1;
			visited[i] = 0;
		}

		//----INIT WEIGHTS
		initWeights(ajaList, vertices, rootVertex);

		//init 'dummy' node in adjacency list
		ajaList[vertices].node = vertices;
		ajaList[vertices].weight = -1;
		ajaList[vertices].next = NULL;
	
		//init min to dummy node
		min = vertices;

  	t = clock(); //start timer
  		
  	//get DEST vertex from query file 
		if((fscanf(fp2,"%d",&query)) == 0) fprintf(stderr,"ERROR! COULD NOT READ QUERY FROM FILE");
	
  		
		//---=-=-=-=-=-=-=-=-=-=------DIJKSTRA'S ALGORITHM-----------=-=-=-=-=-=-=-=-=-=-	

		for(j =0;j<vertices-1 ; j++)
		{			
			//break once ditance to target node is finalized
			if(visited[query] == 1) break;
		
			//update weights of neighbours of current vertex
			updateWeights(ajaList, currentVertex, visited, preceeding);
	
			//Mark current vertex as 'visited'
			visited[currentVertex] = 1;
			
			buildHeap();
			if(pQueue.size == 0) break;
			min = dequeue()->node;

			//UPDATE CURRENT VERTEX
			currentVertex = min;
		
			//Re-init Min
			min = vertices;
		}
		//--------=-=-=-=-=-=-=-=-=-=-=-=-------------------------=-=-=-=-=-=-=-=----------	
    t = clock() - t;
    //double time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds
		//printf("Time taken : %lf seconds\n",time_taken); 

	
		//--------------------------Now for queries--------------------------------


		//last vertex in path is the DEST vertex	
		prev = query;
	
		//first display distance
		printf("%u\n",ajaList[prev].weight);
	
		//----NOW DISPLAY SHORTEST PATH----------- 
	
		//FILL QUERYNODE ARRAY
		//init first node (start at the last node in path)
		start = createQueryNode(prev, NULL);
		queryPtr = start;

		//error check 
		if(query == rootVertex)
		{
			//printf("%d\n",query);
		}
		else
		{
			do 
			{ 
				//update prev
				prev = preceeding[prev];	

				//add node to list containing previous item in query path
				queryPtr->next = createQueryNode(prev, queryPtr);
			
				//update pointer (move to next node in list)...
				queryPtr = queryPtr->next; 
			
			}while(prev != rootVertex); //till we get to the root node (first node in the path)
		}
	
		//PRINT PATH IN REVERSE
		while(queryPtr != NULL)
		{
			printf("%d ",queryPtr->node);
			queryPtr = queryPtr->prev;
		}
	
		printf("\n");
	
		//delete list and free memory 
		deleteQueryList(start);
		
		resetPQ();
	}
	

	
	fclose(fp);
	fclose(fp2);

	return 0;
}//END OF MAIN


