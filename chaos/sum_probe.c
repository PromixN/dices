#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>

int main(int argc, char *argv[]){
    int i,rank,nprocs, N,Nloc,MAX_NUMBERS;
    double *array;
    double sum,psum;
    MPI_Status status;
    
    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&nprocs);
    MAX_NUMBERS=100;
    srand(time(NULL));
    // Pick a random amount of integers to send to process one
    N = (rand() / (float)RAND_MAX) * MAX_NUMBERS;

    if(rank==0){
        Nloc=N/3;
        array=(double *)malloc(sizeof(double)*MAX_NUMBERS);
        for(i=0;i<N;i++) array[i]=1.0;
        // Send the amount of doubles to process one
        MPI_Send(array+Nloc,N-Nloc,MPI_DOUBLE,1,10,MPI_COMM_WORLD);
                fprintf(stderr,"Task 0 sent %d numbers to Task 1\n", N-Nloc);
    }
    else{
        MPI_Probe(0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
         MPI_Get_count(&status,MPI_DOUBLE,&Nloc);
         fprintf(stderr,"Task 1 dynamically received %d numbers from Task 0.\n", Nloc);
         array=(double *)malloc(sizeof(double)*Nloc);
         MPI_Recv(array,Nloc,MPI_DOUBLE,0,10,MPI_COMM_WORLD,&status);
//      MPI_Probe(0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
    }
    psum=0;
    sum =0;
    for(i=0;i<Nloc;i++){
        psum+=array[i];
    }
    
    if(rank==0) {
        MPI_Recv(&sum,1,MPI_DOUBLE,1,11,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
        sum+=psum;
        printf("Sum is %g, partial sum %g\n",sum,psum);
    }
    else {
        MPI_Send(&psum,1,MPI_DOUBLE,0,11,MPI_COMM_WORLD);
        printf("Sum is %g, partial sum %g\n",sum,psum);
    }
    MPI_Finalize();
}