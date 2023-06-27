#include <stdio.h>
#include <mpi.h>
#include<math.h>
#define N 1000000
int l,h,i,j;
int is_prime[N];
void Sieve(){

    for ( i = 2; i <N ; ++i) {
        is_prime[i]= 1;
    }
    for ( i = 2; i <= sqrt(N) ; ++i) {
        if(!is_prime[i])continue;
        for (  j = 2 * i; j < N ; j += i)
            is_prime[j] = 0;
    }
}
int count(int lo,int r){
    int cnt=0;
    for(i=lo;i<=r;i++){
        if(is_prime[i]==1){
                cnt++;
           // printf("%d ",i);
        }
    }
    return cnt;
}
int LOW ,High;

int main(int argc, char* argv[])
{

    int myRank;
    int numOfProccess;
    int numOfelemPerProcess;
    MPI_Status status;

    MPI_Init(&argc, &argv);
    Sieve();
    MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
    MPI_Comm_size(MPI_COMM_WORLD, &numOfProccess);

    float time = 0.0;
    MPI_Barrier(MPI_COMM_WORLD);
    time-= MPI_Wtime();
    if (myRank == 0) {

        printf("Please enter the low ...");
        fflush(stdout);
        scanf("%d",&LOW);
        printf("Please enter the High ...");
        fflush(stdout);
        scanf("%d",&High);

        numOfelemPerProcess = (High-LOW) / (numOfProccess - 1);
        i=1;
        while (i < numOfProccess) {

            l=LOW+(numOfelemPerProcess*(i-1));
            h=l+numOfelemPerProcess-1;
            if (i == numOfProccess - 1) {
              h=High;
            }

            MPI_Send(&is_prime[0], N, MPI_INT, i, 0, MPI_COMM_WORLD);
            MPI_Send(&l, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            MPI_Send(&h, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            i++;
        }
        int sum=0,tmp;
       j = 1;
        while (j < numOfProccess) {
            MPI_Recv(&tmp, 1, MPI_INT, j, 0, MPI_COMM_WORLD,& status);
            sum+=tmp;
            j++;
        }
        printf("After reduction, P0 will have Count %d ",sum);fflush(stdout);

    }
    else {
        MPI_Recv(&is_prime[0], N, MPI_INT, 0, 0, MPI_COMM_WORLD,& status);
        MPI_Recv(&l, 1, MPI_INT, 0, 0, MPI_COMM_WORLD,& status);
        MPI_Recv(&h, 1, MPI_INT, 0, 0, MPI_COMM_WORLD,& status);
        int c =count(l,h);

        MPI_Send(&c, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        printf("calculate partial count of prime numbers from %d %d %d ",l ,h , c );fflush(stdout);

    }
    MPI_Barrier(MPI_COMM_WORLD);
    time+= MPI_Wtime();
    if (myRank == 0) {
        printf("total time = %d \n", time);
        fflush(stdout);
    }
    MPI_Finalize();
    return 0;
}
