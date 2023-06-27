#include <stdio.h>
#include <mpi.h>
int n ;
int arr[10000];
int a[1000];
int main(int argc, char* argv[])
{
    int myRank;
    int numOfProccess;
    int numOfelemPerProcess;
    MPI_Status status;

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
    MPI_Comm_size(MPI_COMM_WORLD, &numOfProccess);

    numOfelemPerProcess = n / (numOfProccess - 1);
    if (myRank == 0) {
        printf("Please enter size of array...");
        fflush(stdout);
        scanf_s("%d",&n);
        printf("Please enter array elements ...");
        fflush(stdout);
        for(int i=0;i<n;i++){
            scanf("%d", &(arr[i]));
        }
          numOfelemPerProcess = n / (numOfProccess - 1);
        int idx,i=1;
        while (i < numOfProccess) {
               idx = (i - 1) * numOfelemPerProcess;
            if (i == numOfProccess - 1) {
                int rem = n - idx;
                MPI_Send(&rem, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
                MPI_Send(&arr[idx], rem, MPI_INT, i, 0, MPI_COMM_WORLD);
            }
            else {
                MPI_Send(&numOfelemPerProcess, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
                MPI_Send(&arr[idx], numOfelemPerProcess, MPI_INT, i, 0, MPI_COMM_WORLD);
            }
             MPI_Send(&idx, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            i++;
        }
        int j = 1,MaxNum=-1,tmp,index, index1;
        while (j < numOfProccess) {
            MPI_Recv(&index1, 1, MPI_INT, j, 0, MPI_COMM_WORLD, &status);
            MPI_Recv(&tmp, 1, MPI_INT, j, 0, MPI_COMM_WORLD,& status);
            if (tmp > MaxNum) MaxNum = tmp,index = index1;
            j++;
        }
        printf("Master process announce the final max which is %d and its index is %d\n ", MaxNum, index);
    }
    else {
        int maxNum = -1e9, i=0 ,idxOfmaxNum,skip,numOfRecElem;
        MPI_Recv(&numOfRecElem, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        MPI_Recv(&a, numOfRecElem, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        MPI_Recv(&skip, 1, MPI_INT, i, 0, MPI_COMM_WORLD, &status);
        while(i < numOfRecElem){
            if (a[i] > maxNum) {
                maxNum = a[i];
                idxOfmaxNum = i+skip;
            }
            i++;
        }
        printf("Hello From Slave# %d",myRank);
        printf(" Max number in my partition is %d", maxNum);
        printf(" and index is %d\n", idxOfmaxNum);
        MPI_Send(&idxOfmaxNum, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        MPI_Send(&maxNum, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }
    MPI_Finalize();
    return 0;
}
