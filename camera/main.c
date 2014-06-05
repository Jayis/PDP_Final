#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<math.h>
#include<mpi.h>
#include<assert.h>
#include<time.h>
#include <unistd.h>


#define ROWS 2
#define COLS 3
#define SCRWIDTH	920
#define SCRHEIGHT	1080

//ranks and sizes
int w_rank, w_size; 
int r_rank,c_rank,r_size,c_size;

double p(int,int);
void sendVal(double);
void recvVal(double*);
void doImageProcessing();
void trgtByVal(int*,int*,int,double*);

int main(int argc,char* argv[]){
	int row,col;
	MPI_Group rowGroups[ROWS];
	MPI_Group colGroups[COLS];
	MPI_Comm rowComm,colComm;
	MPI_Init(&argc, &argv);

	//determine rank,row, and column
	MPI_Comm_rank(MPI_COMM_WORLD, &w_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &w_size);
	//abort if # of processes not right
	if(w_size!=ROWS*COLS){
		if(w_rank==0){printf("!=%d\n",ROWS*COLS);}
		MPI_Finalize();
		exit(1);
	}
	//rank = row*COLS +col
	row = w_rank/COLS;
	col = w_rank%COLS;

	//split Comm
	//
	MPI_Comm_split(MPI_COMM_WORLD,row,w_rank,&rowComm);
	MPI_Comm_split(MPI_COMM_WORLD,col,w_rank,&colComm);
	MPI_Comm_rank(rowComm,&r_rank);
	MPI_Comm_rank(colComm,&c_rank);
	MPI_Comm_size(rowComm,&r_size);
	MPI_Comm_size(colComm,&c_size);

	assert(r_rank==col);
	assert(c_rank==row);
	assert(r_size==COLS);
	assert(c_size==ROWS);
	
	double value;
	double rel_value[4];//up,down,right,left
	int x,y;
	int stop=0;
	int cnt=0;
	//
	x = SCRWIDTH*(2*r_rank+1)/r_size;
	y = SCRHEIGHT*(2*c_rank+1)/c_size;

		time_t ltime;
	//routine
	while(stop==0){
		value = p(x,y);// the importance function
		sendVal(value);
		recvVal(rel_value);

		doImageProcessing();
		printf("{[%d,%d]|(%d,%d)|%d,%f}\n",r_rank,c_rank,x,y,cnt,MPI_Wtime());
		trgtByVal(&x,&y,value,rel_value);

		//Barrier with Stop Signal
		cnt++;
		if(w_rank==0){
			if(cnt >1)
				stop=1;
		}
		MPI_Bcast(&stop,1,MPI_INT,0,MPI_COMM_WORLD);
	}
	
	MPI_Finalize();	
	exit(0);
}

double p(int x,int y){
	return 1.0;
}

void sendVal(double val){
	if(r_rank!=0)
}
void recvVal(double* rel_val){
}
void doImageProcessing(){}
void trgtByVal(int* x,int* y,int value,double* rel_value){}


