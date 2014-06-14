#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<math.h>
#include<mpi.h>
#include<assert.h>
#include<time.h>
#include <unistd.h>
#include<float.h>
#include"eva.h"
#include"combine.h"

#define ROWS 6
#define COLS 6
#define SCRWIDTH	1000
#define SCRHEIGHT	1000

//ranks and sizes
int w_rank, w_size; 
int r_rank,c_rank,r_size,c_size;
int nbr[4]={-1,-1,-1,-1};
MPI_Comm rowComm,colComm;

double p(int,int,Mat*);
void sendVal(double,int,int);
void recvVal(double*,int*,int*);
void doImageProcessing();
void trgtByVal(int*,int*,double,double*,int*,int*);
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
    int x0,y0;
	x0 = SCRWIDTH*(2*r_rank+1)/(2*r_size);
	y0 = SCRHEIGHT*(2*c_rank+1)/(2*c_size);
    default_w=SCRWIDTH/r_size-5;
    default_h=SCRHEIGHT/c_size-5;
    //
    if(c_rank!=c_size-1){//from up
        nbr[0]=getwrank(r_rank,c_rank+1);
    }
    if(c_rank!=0){//from down
        nbr[1]=getwrank(r_rank,c_rank-1);
    }
    if(r_rank!=r_size-1){//from right
        nbr[2]=getwrank(r_rank+1,c_rank);
    }
    if(r_rank!=0){//from left
        nbr[3]=getwrank(r_rank-1,c_rank);
    }
	time_t ltime;
    x=x0;
    y=y0;
    double alpha=0.9;
    Mat img;
	//routine
	while(stop==0){
		value = p(x,y,&img);// the importance function
		sendVal(value,x,y);
		//printf("%d\n",w_rank);
		recvVal(rel_value,x_pos,y_pos);

		doImageProcessing();
    //    if(w_rank==5)
	    printf("%d,%d|%d,%d|%d,%f\n",r_rank,c_rank,x,y,cnt,MPI_Wtime());
		trgtByVal(&x,&y,value,rel_value,x_pos,y_pos);

		//Barrier with Stop Signal
		cnt++;
       /* 
        if(cnt%12==0){
            x= alpha*x + (1-alpha)*x0;
            y= alpha*y + (1-alpha)*y0;
        }
        */
		if(w_rank==0){
			if(cnt >100)
				stop=1;
		}
		MPI_Bcast(&stop,1,MPI_INT,0,MPI_COMM_WORLD);
	}
	
	MPI_Finalize();	
	exit(0);
}

double p(int x,int y,Mat* img){
    camera cam;
    cam.center_x=x;
    cam.center_y=y;
    double res = eva(cam,0,img);
	return -res;
}

void sendVal(double val,int x,int y){
	double sendbuf[3];
	MPI_Request req;
	sendbuf[0]=val;
	sendbuf[1]=(double) x;
	sendbuf[2]=(double) y;
	int i;
	for(i=0;i<4;i++){
		if(nbr[i]!=-1){
			MPI_Isend(sendbuf,3,MPI_DOUBLE,nbr[i],0,MPI_COMM_WORLD,&req);
			MPI_Request_free(&req);
		}
	}
}
void recvVal(double* rel_val,int* x_pos,int* y_pos){
	MPI_Status stat[4];
	MPI_Request req[4];
	double recvbuf[4][3];
	int i;
	for(i=0;i<4;i++)
		if(nbr[i]!=-1)
			MPI_Irecv(recvbuf+i,3,MPI_DOUBLE,nbr[i],0,MPI_COMM_WORLD,req+i);
	for(i=0;i<4;i++)
		if(nbr[i]!=-1){
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

void doImageProcessing(){


}
void trgtByVal(int* x,int* y,double value,double* rel_value,int* x_pos,int* y_pos){
    int i;
    double dy=0,dx=0;
    double Hy=0,Hx=0;
    double k =500,xsq,ysq;
	double xd,yd;
    for(i=0;i<4;i++){
        xd=((*x-x_pos[i])/(double)default_w);
        yd=((*y-y_pos[i])/(double)default_h);
        xsq = xsq + k*xd*xd;
        ysq = ysq + k*yd*yd;
    }
    if(nbr[0]!=-1 && nbr[1]!=-1){
        Hy += (value +ysq - rel_value[0])/(*y-y_pos[0]);
        Hy += (value +ysq- rel_value[1])/(*y-y_pos[1]);
		Hy = Hy/(y_pos[1]-y_pos[0]);
        dy = -10*(rel_value[0]-rel_value[1])/(y_pos[0]-y_pos[1])/Hy;
        if(dy > default_h/4)
            dy = default_h/4;
        else if(dy < -default_h/4)
            dy=-default_h/4;
        *y= *y +dy;
        if(*y>y_pos[0]-default_h/2)
            *y=y_pos[0]-default_h/2;
        else if(*y<y_pos[1]+default_h/2)
            *y=y_pos[1]+default_h/2;
    }
    //if(w_rank==5){printf("%f\n",dy);}
    if(nbr[2]!=-1 && nbr[3]!=-1){
        Hx += (value +xsq- rel_value[2])/(*x-x_pos[2]);
        Hx += (value +xsq- rel_value[3])/(*x-x_pos[3]);
		Hx = Hx/(x_pos[1]-x_pos[0]);
        dx = -10*(rel_value[2]-rel_value[3])/(x_pos[2]-x_pos[3])/Hx;
        if(dx > default_w/4)
            dx = default_w/4;
        else if(dx < -default_w/4)
            dx=-default_w/4;
        *x+=dx;
        if(*x>x_pos[2]-default_w/2)
            *x=x_pos[2]-default_w/2;
        else if(*x<x_pos[3]+default_w/2)
            *x=x_pos[3]+default_w/2;
    }
    
 //   printf("%d,%d,%d,%d dx=%f,dy=%f\n", nbr[0],nbr[1],nbr[2],nbr[3],dx,dy);
}
int getwrank(int r,int c){
	return c*COLS+r;
}


