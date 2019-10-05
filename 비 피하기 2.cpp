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

char*** PosArr(int x_pos, int y_pos, int z_pos, int rand); //공간 좌표 설정(feat.malloc) 
char*** Format_PosArr(int x_pos, int y_pos, int z_pos); //공간 좌표 0으로 초기화(feat.malloc)
void HumanArr(int x_pos, int y_pos, int z_pos); //사람 좌표 설정 
void Human_Moving(int x_pos, int y_pos, int z_pos); //사람 움직임 설정 
void Touch_Rain(int x_pos, int y_pos, int z_pos); //Human_Moving 함수에서 호출 
void Rain_Fall(int x_pos, int y_pos, int z_pos); //공간 좌표 전체에서 검색, 0과 1로 되어 있는 부분을 밑으로 떨어뜨림 
void Touch_Human(int x, int y, int z); //Rain_Fall 함수에서 호출 
void Rain_Create(int x_pos, int z_pos, int rand); //비 생성 

void FreeArr(int x_pos, int y_pos); // 동적으로 할당 된 메모리 해제
int rand_by_rand(int num); //확률 1~100% 대입하면 그에 맞게 당첨시 1 출력 꽝일시 0 출력 
void trial_print_pos(void); //시험용 프린트함수. 가로58 세로 30정도가 적당 
void Only_Print_Result(void); //오로지 결과만 프린트. 
void Only_Print_Average(void);
void print_pos(void); //망작 
void *t_function1(void *arg); //멀티쓰레드 함수1 
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
		dataCube = PosArr(X_POS, Y_POS, Z_POS, RAND); //구역 생성 
		HumanArr(MAN_length, MAN_height, MAN_width); //인간 생성 
		
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

char*** PosArr(int x_pos, int y_pos, int z_pos, int rand) //공간 좌표 설정(feat.malloc)
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

    // 모든 배열원소 초기화(중요하다)
    for (x = 0; x < x_pos; x++)
        for (y = 0; y < y_pos; y++)
            for (z = 0; z < z_pos; z++)
                ptr[x][y][z] = rand_by_rand(rand);

    //printf("\nMEMORY ALLOCATION(char) OK!\n");
    return ptr;
}
char*** Format_PosArr(int x_pos, int y_pos, int z_pos) //공간 좌표 설정(feat.malloc)
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

    // 모든 배열원소 초기화(중요하다)
    for (x = 0; x < x_pos; x++)
        for (y = 0; y < y_pos; y++)
            for (z = 0; z < z_pos; z++)
                ptr[x][y][z] = 0;

    //printf("\nMEMORY ALLOCATION(char) OK!\n");
    return ptr;
}
void HumanArr(int x_pos, int y_pos, int z_pos) //사람 좌표 설정 
{
    int x, y, z;
    // 모든 배열원소 초기화(중요하다)
    for (x = 0; x <= x_pos; x++)
        for (y = 0; y <= y_pos; y++)
            for (z = 0; z <= z_pos; z++)
                dataCube[x][y][z] = Human;
}
void Human_Moving(int x_pos, int y_pos, int z_pos) //사람 움직임 설정
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
void Touch_Rain(int x_pos, int y_pos, int z_pos) //Human_Moving 함수에서 호출
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
void Touch_Human(int x, int y, int z) //비가 인간한테 닿을 때 
{
	if(dataCube[x][y-1][z]==5)
	{
		dataCube[x][y][z]=Air;
		rain_cnt+=1;
		touch_cnt=1;
	}
}
void Rain_Create(int x_pos, int z_pos, int rand) //천장에 비 생성 
{
	for (int x = 0; x < x_pos; x++)
            for (int z = 0; z < z_pos; z++)
                dataCube[x][Y_POS-1][z] = rand_by_rand(rand);
}
// 동적으로 할당 된 메모리 해제
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

void trial_print_pos(void) //가로 58 세로 30정도 콘솔창에서 보기위한 프린트함수 
{
	for(int y=Y_POS-1 ; y>=0 ; y--)
	{
		for(int x=0 ; x<X_POS ; x++)
		{
			printf("%d ",dataCube[x][y][0]);
		}
	printf("\n");
	}
	printf("비 맞은 횟수 : %d", rain_cnt);
}
void Only_Print_Result(void) //오로지 결과만 보여주는 함수 
{
	printf("비 맞은 횟수 : %d\n", rain_cnt);
}
void Only_Print_Average(void) //오로지 결과만 보여주는 함수 
{
	printf("비 맞은 횟수 : %d\n", sum_rain_cnt/LOOP);
}
void print_pos(void) //필요없는 함수 
{
	for(int x=0 ; x<X_POS ; x++)
		for(int y=0 ; y<Y_POS ; y++)
			for(int z=0 ; z<Z_POS ; z++)
				printf("%d \n",dataCube[x][y][z]); 
}
void *t_function1(void* arg) //멀티쓰레드 함수 
{

	while(1)
    //for(int i=0 ; i<X_POS-MAN_length-1 ; i++)
    {
		Rain_Fall(X_POS, Y_POS, Z_POS); 
		if(finish_cnt == 1) break;
    }
}
void *t_function2(void* arg) //멀티쓰레드 22 
{
	while(1)
    {
		Rain_Create(X_POS, Z_POS, RAND);
		if(finish_cnt == 1) break;
    }
}
void *t_function3(void* arg) //멀쓰 33 
{
	for(int i=0 ; i<X_POS-MAN_length-1 ; i++)
	{
		Human_Moving(MAN_length+i, MAN_height, MAN_width); //인간 움직임
		
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

