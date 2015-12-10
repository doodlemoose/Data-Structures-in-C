//header file for myshortestpath.c 

typedef struct coordinates{
  int x;
  int y;
}coordinates;

struct listNode{
  int node;
  unsigned int weight;
 	struct listNode* next;
};

typedef struct listNode listNode;

struct queryNode{
	int node;
	struct queryNode* next;
	struct queryNode* prev;
}; 

typedef struct queryNode queryNode;

struct PQ{
	int size;
	listNode** heap;
};

typedef struct PQ PQ;

//declare global priority queue
PQ pQueue;

void initPQ();
void enqueue(listNode* item);
listNode* dequeue();
void buildHeap();
void downHeap(int i, int n);
void showHeap();

void initWeights(listNode* ajaList, int vertices, int root);

listNode* createListNode(int nodeNum, unsigned int weight);

int euclidean(int x1, int y1, int x2, int y2);

void updateWeights(listNode* ajaList, int currentVertex, unsigned char* done, int* preceding);

queryNode* createQueryNode(int node, queryNode* prev);

void deleteQueryList(queryNode* head);

void resetPQ();

