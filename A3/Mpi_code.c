#include <stdio.h>
#include <stdlib.h>
#include<math.h>
#include <mpi.h>
float numbers[1000],final_sum=0,square_diff=0;
float mean=0.0,variance=0.0,standardDeviation=0.0;
int n,range;
int main(int argc, char * argv[])
{
    int rank,numberofprocess;
    MPI_Init( &argc, &argv );
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &numberofprocess);

    if(rank==0)
    {
        printf("Enter the size of Array \n");
        fflush(stdout);
        scanf("%d",&n);
        printf("Enter the elements of the Array : \n");
        fflush(stdout);
        for (int i = 0; i < n; ++i)
        {
            scanf("%f",&numbers[i]);
        }
        range=(n/(numberofprocess-1));
    }

    MPI_Bcast(&n,1,MPI_INT,0,MPI_COMM_WORLD);
    MPI_Bcast(&numbers,n,MPI_FLOAT,0,MPI_COMM_WORLD);
    MPI_Bcast(&range,1,MPI_INT,0,MPI_COMM_WORLD);

    float sum=0;

    int low=(rank)*range;
    int high=low+range;
    // reminder handle
    if(rank==numberofprocess-1)
    {
        high=n;
    }
    for (int i = low; i <high ; ++i)
    {
        sum+=numbers[i];
    }

    MPI_Reduce(&sum,&final_sum,1,MPI_FLOAT,MPI_SUM,0,MPI_COMM_WORLD);
    if(rank == 0)
    {
        mean=final_sum/n;
        printf("Mean : %lf\n",mean);
        fflush(stdout);
    }
    MPI_Bcast(&mean,1,MPI_FLOAT,0,MPI_COMM_WORLD);

    float sum2=0;
    low=(rank)*range;
    high=low+range;
    // reminder handle
    if(rank==numberofprocess-1)
    {
        high=n;
    }
    for (int i = low; i <high ; ++i)
    {
        float x=numbers[i]-mean;
        sum2+=(x*x);
    }

    MPI_Reduce(&sum2,&square_diff,1,MPI_FLOAT,MPI_SUM,0,MPI_COMM_WORLD);
    if(rank == 0)
    {
        variance=square_diff/n;
        printf("variance : %lf\n",variance);
        fflush(stdout);
        standardDeviation=sqrt(variance);
        printf("standardDeviation  : %lf\n",standardDeviation);
        fflush(stdout);
    }
    MPI_Finalize();
    return 0;
}
