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

huffNode * createNode(char key, int val); //creates and inits a huffman tree node

int getPath(char key, huffNode* root, char* path, int i); //gets path to node from huffamn tree

/*----------------PRIORITY QUEUE-----------------------------
	I have implemented the PQ as an array,
	using a pass of insertion sort to implement the priority.
	
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

pQueue* createPqueue();
huffNode* dequeue(pQueue* pQueue);
void enqueue(pQueue* pQueue, huffNode* nodePtr);
