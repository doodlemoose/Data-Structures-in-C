#include<stdio.h>


int seq1(int *arr, int n)
{
	//intialize array
	*arr = 1; *(arr+1) = 2; *(arr+2) = 3;
   	int i=2, next; /* Keep track of output array */
   	int two = 4, three = 6;
    	int twop = 1, threep = 1; //2ptr and 3ptr

    	do{
		if(2 * arr[twop] <= arr[i]) 
		{
			twop++;
			two= 2 * arr[twop]; //make sure twop is pointing to a num whose double is larger than the biggest element alredy in the ouput array
		}
		if(3 * arr[threep] <= arr[i])
		{
			threep++;
			three= 3 * arr[threep];  //make sure three is pointing to a num whose triple is larger than the biggest element alredy in the ouput array
		}
		next = (two < three? two:three); //choose the smaller of the two values above as the next element in the sequence
		if(next <= n) arr[++i] = next;  //add it to our output array if it within desired range

	}while(next <= n);

	return i+1;
}

int main()
{

	int arr[25] = {0};
	int i;

	seq1(arr, 85);

	for(i = 0; i<25; i++)	
	{
		printf("%d ", arr[i]);
	}


  return(0);
}
