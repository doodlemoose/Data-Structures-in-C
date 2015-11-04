#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#define DEBUG 1
#define MAX_PQ_SIZE 260 //count when PQ is full  	
#define PATH_SIZE 257

int main(int argc, char** argv)
{
	//open compressed file
	FILE* fp = fopen(argv[1], "rb");
	
	//read header size
	
	//size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream)
	int headerSize;
	//fseek(fp2, SEEK_SET, 0);
	if((fread((void*)&headerSize, sizeof(int), 1, fp)) != 1)
	{
		fprintf(stderr,"ERROR! Header not read!"); 
	}
	
	printf("header = %d bytes",headerSize);
	/*
	for(k = 0; k<charCount; k++)
	{
		
		fread((void*)&testSize, sizeof(int), 1, fp2);
		printf("%d, char is %d   ",numRead, testSize); 
	
		fread((void*)&testSize, sizeof(int), 1, fp2);
		printf("%d, count is is %d\n",numRead, testSize); 
	}
	printf("\n");
	//-----------------------------*/
	
	
	//parse through char-val pairs and recreate char counts table
	
	
	//add pseudo-eof char to counts table
	
	
	//build huffman tree  
	
	
	
	// -------parse through compressed file
	
	//open output file
	
	//declare and init mask and buffer
	
	//do:
	//read next byte and store in buffer
	
	//read next bit
	
	//move down tree accordingly
	
		
	//if we reach a leaf node, check if the node's key is the pseudo-eof char
	
	//stop parsing input file if pseudo-eof is reached
	
	//if not pseudo-eof, write the node's key to the output file as an ASCII value, and move back to root   
	
	//shift mask 
	//if mask is 0, rewrite buffer with next byte from compressed file

	
	//keep on doing this 
	
	
	
	return 0;
}
