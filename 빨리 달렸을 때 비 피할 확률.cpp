#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#define NUM 100
/*
typedef struct point
{
	int xpos[100];
	int ypos[100];
	int zpos[100];
}Point;
*/
int pos[NUM][NUM][NUM];
void insert_pos(void)
{
	srand((int)time(NULL));
	for(int i=0 ; i<NUM ; i++)
		for(int j=0 ; j<NUM ; j++)
			for(int k=0 ; k<NUM ; k++)
				pos[i][j][k]=rand()%2;
}
void print_pos(void)
{
	for(int i=0 ; i<NUM ; i++)
		for(int j=0 ; j<NUM ; j++)
		{
			for(int k=0 ; k<NUM ; k++)
			{
				printf("%d \n",pos[i][j][k]); 
				Sleep(300);
			}
		}
}


int main()
{
	insert_pos();
	print_pos();
}
	/*if(i>0&&j>0&&k>0)
			{
				for(l=l ; l<i+1 ; l++)
				{
					for(m=1 ; m<
					if(ptr1[i-j]==ptr1[i])
					{
						sign = 1;
						break;
					}
				}
					if(sign == 1)
					{
						i=i-j;
						continue;	
					}
			}
		sign = 0;
	*/
