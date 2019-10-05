#include <stdio.h>
#include <string.h> 

int arr[100][100];

int main()
{
	memset(arr, -1, sizeof(arr));
	for(int i=0 ; i<10 ; i++)
	{
		for(int n=0 ; n<10 ; n++)
			printf("%d ", arr[i][n]);
		printf("\n");	
	}
}
