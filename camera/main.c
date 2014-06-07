#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<math.h>
#include<mpi.h>
#include<assert.h>
#include<time.h>
#include <unistd.h>
#include<float.h>


#define ROWS 2
#define COLS 3
#define SCRWIDTH	920
#define SCRHEIGHT	1080

//ranks and sizes
int w_rank, w_size; 
int r_rank,c_rank,r_size,c_size;
MPI_Comm rowComm,colComm;

double p(int,int);
void sendVal(double,int,int);
void recvVal(double*,int*,int*);
void doImageProcessing();
void trgtByVal(int*,int*,int,double*);
int getwrank(int,int);

int main(int argc,char* argv[]){
	int row,col;
	MPI_Group rowGroups[ROWS];
	MPI_Group colGroups[COLS];
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

	assert(w_rank == getwrank(r_rank,c_rank));

	assert(r_rank==col);
	assert(c_rank==row);
	assert(r_size==COLS);
	assert(c_size==ROWS);
	
	double value;
	double rel_value[4];//up,down,right,left
	int x,y,i;
	int x_pos[4],y_pos[4];
	int stop=0;
	int cnt=0;
	//
	x = SCRWIDTH*(2*r_rank+1)/(2*r_size);
	y = SCRHEIGHT*(2*c_rank+1)/(2*c_size);

		time_t ltime;
	//routine
	while(stop==0){
		value = p(x,y);// the importance function
		sendVal(value,x,y);
		//printf("%d\n",w_rank);
		recvVal(rel_value,x_pos,y_pos);

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

void sendVal(double val,int x,int y){
	double sendbuf[3];
	MPI_Request req;
	sendbuf[0]=val;
	sendbuf[1]=(double) x;
	sendbuf[2]=(double) y;
	int source[4]={-1,-1,-1,-1};
	if(c_rank!=c_size-1){//from up
		source[0]=getwrank(r_rank,c_rank+1);
	}
	if(c_rank!=0){//from down
		source[1]=getwrank(r_rank,c_rank-1);
	}
	if(r_rank!=r_size-1){//from right
		source[2]=getwrank(r_rank+1,c_rank);
	}
	if(r_rank!=0){//from left
		source[3]=getwrank(r_rank-1,c_rank);
	}
	int i;
	for(i=0;i<4;i++){
		if(source[i]!=-1){
			MPI_Isend(sendbuf,3,MPI_DOUBLE,source[i],0,MPI_COMM_WORLD,&req);
			MPI_Request_free(&req);
		}
	}
}
void recvVal(double* rel_val,int* x_pos,int* y_pos){
	MPI_Status stat[4];
	MPI_Request req[4];
	double recvbuf[4][3];
	int source[4]={-1,-1,-1,-1};
	if(c_rank!=c_size-1){//from up
		source[0]=getwrank(r_rank,c_rank+1);
	}
	if(c_rank!=0){//from down
		source[1]=getwrank(r_rank,c_rank-1);
	}
	if(r_rank!=r_size-1){//from right
		source[2]=getwrank(r_rank+1,c_rank);
	}
	if(r_rank!=0){//from left
		source[3]=getwrank(r_rank-1,c_rank);
	}
	int i;
	for(i=0;i<4;i++)
		if(source[i]>=0)
			MPI_Irecv(recvbuf+i,3,MPI_DOUBLE,source[i],0,MPI_COMM_WORLD,req+i);
	for(i=0;i<4;i++)
		if(source[i]>=0){
			MPI_Wait(req+i,stat+i);
			rel_val[i]=recvbuf[i][0];
			x_pos[i]=(int)recvbuf[i][1];
			y_pos[i]=(int)recvbuf[i][2];
		}else{
			rel_val[i]=-DBL_MIN;
			x_pos[i]=-1;
			y_pos[i]=-1;
		}
}

void doImageProcessing(){}
void trgtByVal(int* x,int* y,int value,double* rel_value){}
int getwrank(int r,int c){
	return c*COLS+r;
}


