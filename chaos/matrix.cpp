#include "stdio.h"
#include "mpi.h"

int main(int argc, char**argv)
{
    float raw [16]=
    {
        1 ,2 ,3 ,4 ,
        5 ,6 ,7 ,8 ,
        9 ,10,11,12,
        13,14,15,16
    };
    float t [16];
    int i = 0;
    // MPI_Init(&argc,&argv);
    MPI_Init(NULL,NULL);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&size);

    MPI_Datatype C,R;
    MPI_Type_vector(4,1,4,MPI_FLOAT,&C);
    MPI_Type_vector(1,4,1,MPI_FLOAT,&R);
    MPI_Type_commit(&C);
    MPI_Type_commit(&R);
    if(rank==0)
    {
        for(i = 0 ; i < 4; i ++)
        MPI_Send(raw+i,1,C,1,i,MPI_COMM_WORLD);
    }
    else
    {
        for(i = 0 ; i < 4; i ++)
        MPI_Recv(t+4*i,1,R,0,i,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
    }
    for(i = 0 ; rank && i < 16 ; i++)
    {
        printf("%f\t",t[i]);
    }
    MPI_Finalize();
}