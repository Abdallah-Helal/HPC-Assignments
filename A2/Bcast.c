#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>
#define N 1000000
int is_prime[N];
void Sieve(){

    for ( int i = 2; i <N ; ++i) {
        is_prime[i]= 1;
    }
    for (int  i = 2; i <= sqrt(N) ; ++i) {
        if(!is_prime[i])continue;
        for (int  j = 2 * i; j < N ; j += i)
            is_prime[j] = 0;
    }
}
int count(int lo,int r){
    int cnt=0;
    for(int i=lo;i< r;i++){
        if(is_prime[i]==1){
                cnt++;
       //     printf("%d ",i);
        }
    }
    return cnt;
}

//int c = 0, partial_count = 0;
int main(int argc , char * argv[]){
    Sieve();
    int myRank, numOfProcess;
    int sz;
    int LOW ,High;
    int partial_count=0,answer=0;

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
    MPI_Comm_size(MPI_COMM_WORLD, &numOfProcess);

    if (myRank == 0) {
        printf("Please enter the low ..");
        fflush(stdout);
        scanf("%d",&LOW);
        printf("Please enter the High ..");
        fflush(stdout);
        scanf("%d",&High);
        sz = (High - LOW + 1) / (numOfProcess-1);

    }
    float time = 0.0;
    MPI_Barrier(MPI_COMM_WORLD);
    time-= MPI_Wtime();

    MPI_Bcast(&LOW, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&High, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&sz, 1, MPI_INT, 0, MPI_COMM_WORLD);

    int start=0, endd=0;
    start = LOW + (myRank - 1) * sz;
    endd = start + sz;

    //printf("%d %d %d ",rank,start,end);

    if (myRank == numOfProcess - 1)endd = High + 1;

    partial_count=count(start,endd);
    MPI_Reduce(&partial_count, &answer, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if(myRank!=0){
        printf("calculate partial count of prime numbers from %d %d %d ",start,endd-1, partial_count);

    }

    if (myRank == 0) {
        printf("After reduction, P0 will have Count %d \n", answer);
        fflush(stdout);
    }
    MPI_Barrier(MPI_COMM_WORLD);
    time+= MPI_Wtime();
    if (myRank == 0) {
        printf("total time = %d \n", time);
        fflush(stdout);
    }


    MPI_Finalize();
}
