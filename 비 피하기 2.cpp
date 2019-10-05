#include <stdio.h>
#include <stdlib.h>
#include <malloc.h> 
#include <time.h>
#include <windows.h>
#include <pthread.h>
#include <unistd.h>

#define X_POS 1000
#define Y_POS 1000
#define Z_POS 200

#define RAND 95

#define MAN_length 60
#define MAN_height 900
#define MAN_width 150

#define LOOP 1
typedef enum syllable
{
	Air=0, Rain=1, Human=5
} Syllable; 

char*** dataCube;
int rain_cnt=0; 
int touch_cnt=0;   
int finish_cnt=0;
int sum_rain_cnt=0;

char*** PosArr(int x_pos, int y_pos, int z_pos, int rand); //���� ��ǥ ����(feat.malloc) 
char*** Format_PosArr(int x_pos, int y_pos, int z_pos); //���� ��ǥ 0���� �ʱ�ȭ(feat.malloc)
void HumanArr(int x_pos, int y_pos, int z_pos); //��� ��ǥ ���� 
void Human_Moving(int x_pos, int y_pos, int z_pos); //��� ������ ���� 
void Touch_Rain(int x_pos, int y_pos, int z_pos); //Human_Moving �Լ����� ȣ�� 
void Rain_Fall(int x_pos, int y_pos, int z_pos); //���� ��ǥ ��ü���� �˻�, 0�� 1�� �Ǿ� �ִ� �κ��� ������ ����߸� 
void Touch_Human(int x, int y, int z); //Rain_Fall �Լ����� ȣ�� 
void Rain_Create(int x_pos, int z_pos, int rand); //�� ���� 

void FreeArr(int x_pos, int y_pos); // �������� �Ҵ� �� �޸� ����
int rand_by_rand(int num); //Ȯ�� 1~100% �����ϸ� �׿� �°� ��÷�� 1 ��� ���Ͻ� 0 ��� 
void trial_print_pos(void); //����� ����Ʈ�Լ�. ����58 ���� 30������ ���� 
void Only_Print_Result(void); //������ ����� ����Ʈ. 
void Only_Print_Average(void);
void print_pos(void); //���� 
void *t_function1(void *arg); //��Ƽ������ �Լ�1 
void *t_function2(void *arg); //22
void *t_function3(void *arg); //33
void Confirm_thr_id(int num);

int main()
{
	Syllable tone;
	pthread_t p_thread[2];
    int thr_id;
    int status;
 
 	for(int i=0 ; i<LOOP ; i++)
 	{
		dataCube = PosArr(X_POS, Y_POS, Z_POS, RAND); //���� ���� 
		HumanArr(MAN_length, MAN_height, MAN_width); //�ΰ� ���� 
		
		pthread_create(&p_thread[0], NULL, t_function1, NULL);
	 	Confirm_thr_id(thr_id);
	
		pthread_create(&p_thread[1], NULL, t_function2, NULL);
	 	Confirm_thr_id(thr_id);
	 	
	    t_function3(NULL);
		 
	    pthread_join(p_thread[0], (void **)&status);
	    pthread_join(p_thread[1], (void **)&status);	

		//trial_print_pos();
		//Only_Print_Result();
		sum_rain_cnt+=rain_cnt;
		rain_cnt=0;
		finish_cnt=0;
		FreeArr(X_POS, Y_POS);
		//Format_PosArr(X_POS, Y_POS, Z_POS);
	}
	Only_Print_Average();
	//print_pos();
    
	return 0;
}

int rand_by_rand(int num)
{
	if(rand()%100<num)
		return Rain;
	else return Air;	
}

char*** PosArr(int x_pos, int y_pos, int z_pos, int rand) //���� ��ǥ ����(feat.malloc)
{
    int x, y, z;
    char*** ptr;
  	srand((int)time(NULL));

    if ( ( ptr = (char***)malloc(x_pos * sizeof(char**)) ) == NULL )
    {
        printf("\nMemory allocation failure\n");
        exit(1);
    }

    for (x = 0; x < x_pos; x++)
        if ( ( ptr[x] = (char**)malloc(y_pos * sizeof(char*)) ) == NULL )
        {
            printf("\nMemory allocation failure\n");
            exit(1);
        }

    for (x = 0; x < x_pos; x++)
        for (y = 0; y < y_pos; y++)
            if ( ( ptr[x][y] = (char*)malloc(z_pos * sizeof(char)) ) == NULL )
            {
                printf("\nMemory allocation failure\n");
                exit(1);
            }

    // ��� �迭���� �ʱ�ȭ(�߿��ϴ�)
    for (x = 0; x < x_pos; x++)
        for (y = 0; y < y_pos; y++)
            for (z = 0; z < z_pos; z++)
                ptr[x][y][z] = rand_by_rand(rand);

    //printf("\nMEMORY ALLOCATION(char) OK!\n");
    return ptr;
}
char*** Format_PosArr(int x_pos, int y_pos, int z_pos) //���� ��ǥ ����(feat.malloc)
{
    int x, y, z;
    char*** ptr;
  	srand((int)time(NULL));

    if ( ( ptr = (char***)malloc(x_pos * sizeof(char**)) ) == NULL )
    {
        printf("\nMemory allocation failure\n");
        exit(1);
    }

    for (x = 0; x < x_pos; x++)
        if ( ( ptr[x] = (char**)malloc(y_pos * sizeof(char*)) ) == NULL )
        {
            printf("\nMemory allocation failure\n");
            exit(1);
        }

    for (x = 0; x < x_pos; x++)
        for (y = 0; y < y_pos; y++)
            if ( ( ptr[x][y] = (char*)malloc(z_pos * sizeof(char)) ) == NULL )
            {
                printf("\nMemory allocation failure\n");
                exit(1);
            }

    // ��� �迭���� �ʱ�ȭ(�߿��ϴ�)
    for (x = 0; x < x_pos; x++)
        for (y = 0; y < y_pos; y++)
            for (z = 0; z < z_pos; z++)
                ptr[x][y][z] = 0;

    //printf("\nMEMORY ALLOCATION(char) OK!\n");
    return ptr;
}
void HumanArr(int x_pos, int y_pos, int z_pos) //��� ��ǥ ���� 
{
    int x, y, z;
    // ��� �迭���� �ʱ�ȭ(�߿��ϴ�)
    for (x = 0; x <= x_pos; x++)
        for (y = 0; y <= y_pos; y++)
            for (z = 0; z <= z_pos; z++)
                dataCube[x][y][z] = Human;
}
void Human_Moving(int x_pos, int y_pos, int z_pos) //��� ������ ����
{
	int x, y, z, tmp;
	for (x = x_pos; x >= 0; x--)
        for (y = y_pos; y >= 0; y--)
            for (z = z_pos; z >= 0; z--)
            {
            	Touch_Rain(x, y, z);
                dataCube[x+1][y][z]=dataCube[x][y][z];
                dataCube[x][y][z] = Air;
            }
}
void Touch_Rain(int x_pos, int y_pos, int z_pos) //Human_Moving �Լ����� ȣ��
{
	if(dataCube[x_pos+1][y_pos][z_pos] == Rain)
        rain_cnt += 1;
}
void Rain_Fall(int x_pos, int y_pos, int z_pos)
{
	for (int x = 1; x < x_pos; x++)
        for (int y = 1; y < y_pos; y++)
            for (int z = 1; z < z_pos; z++)
            {
            	Touch_Human(x,y,z);
            	if(touch_cnt==1)
            	{
            		touch_cnt=0;
            		continue;
            	}
				if(dataCube[x][y][z] == Rain || dataCube[x][y][z] == Air)
				{	
					dataCube[x][y-1][z]=dataCube[x][y][z];
				}
			}
}
void Touch_Human(int x, int y, int z) //�� �ΰ����� ���� �� 
{
	if(dataCube[x][y-1][z]==5)
	{
		dataCube[x][y][z]=Air;
		rain_cnt+=1;
		touch_cnt=1;
	}
}
void Rain_Create(int x_pos, int z_pos, int rand) //õ�忡 �� ���� 
{
	for (int x = 0; x < x_pos; x++)
            for (int z = 0; z < z_pos; z++)
                dataCube[x][Y_POS-1][z] = rand_by_rand(rand);
}
// �������� �Ҵ� �� �޸� ����
void FreeArr(int x_pos, int y_pos)
{
    int x,y;
    for(x=0; x<x_pos; x++)
    {
        for(y=0; y<y_pos; y++)
        {
            free(dataCube[x][y]);
        }
        free(dataCube[x]);
    }
    free(dataCube);
}

void trial_print_pos(void) //���� 58 ���� 30���� �ܼ�â���� �������� ����Ʈ�Լ� 
{
	for(int y=Y_POS-1 ; y>=0 ; y--)
	{
		for(int x=0 ; x<X_POS ; x++)
		{
			printf("%d ",dataCube[x][y][0]);
		}
	printf("\n");
	}
	printf("�� ���� Ƚ�� : %d", rain_cnt);
}
void Only_Print_Result(void) //������ ����� �����ִ� �Լ� 
{
	printf("�� ���� Ƚ�� : %d\n", rain_cnt);
}
void Only_Print_Average(void) //������ ����� �����ִ� �Լ� 
{
	printf("�� ���� Ƚ�� : %d\n", sum_rain_cnt/LOOP);
}
void print_pos(void) //�ʿ���� �Լ� 
{
	for(int x=0 ; x<X_POS ; x++)
		for(int y=0 ; y<Y_POS ; y++)
			for(int z=0 ; z<Z_POS ; z++)
				printf("%d \n",dataCube[x][y][z]); 
}
void *t_function1(void* arg) //��Ƽ������ �Լ� 
{

	while(1)
    //for(int i=0 ; i<X_POS-MAN_length-1 ; i++)
    {
		Rain_Fall(X_POS, Y_POS, Z_POS); 
		if(finish_cnt == 1) break;
    }
}
void *t_function2(void* arg) //��Ƽ������ 22 
{
	while(1)
    {
		Rain_Create(X_POS, Z_POS, RAND);
		if(finish_cnt == 1) break;
    }
}
void *t_function3(void* arg) //�־� 33 
{
	for(int i=0 ; i<X_POS-MAN_length-1 ; i++)
	{
		Human_Moving(MAN_length+i, MAN_height, MAN_width); //�ΰ� ������
		
		//Sleep(20);
	} 
	finish_cnt=1;
}
void Confirm_thr_id(int num)
{
	if (num < 0)
    {
        perror("thread create error : ");
        exit(0);
    }
}

