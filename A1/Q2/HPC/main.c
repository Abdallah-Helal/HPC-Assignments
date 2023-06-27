#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "mpi.h"


// Define matrices
int** matrix1;
int** matrix2;
int** res;

int r1, c1, r2, c2; // dimensions of the matrices
int i, j, k; // Counter variables

int ch; //console or file

int** alloc_2d_int_array(int r, int c) {
    int* data = (int*)malloc(r * c * sizeof(int));
    int** arr = (int**)malloc(r * sizeof(int*));
    for (i = 0; i < r; i++) arr[i] = &(data[c * i]);
    for(i=0;i<r;i++)
        for(j=0;j<c;j++)
            arr[i][j]=0;

    return arr;
}
void console() {
    //first matrix
    printf("Please enter dimensions of the first matrix:"); fflush(stdout);
    scanf("%d %d", &r1, &c1);
    matrix1 = alloc_2d_int_array(r1, c1);

    printf("\nPlease enter its elements:\n");    fflush(stdout);
    for (i = 0; i < r1; i++)
        for (j = 0; j < c1; j++)
            scanf("%d", &(matrix1[i][j]));

    // second matrix
    printf("\nPlease enter dimensions of the second matrix: ");   fflush(stdout);
    scanf("%d %d", &r2, &c2);
    matrix2 = alloc_2d_int_array(r2, c2);

    printf("\nPlease enter its elements:\n");   fflush(stdout);
    for (i = 0; i < r2; i++)
        for (j = 0; j < c2; j++)
            scanf("%d", &(matrix2[i][j]));

    res = alloc_2d_int_array(r1, c2);
    for (i = 0; i < r1; i++)
        for (j = 0; j < c2; j++)
            res[i][j] = 0;


}
void files(){
    FILE *f;
    f=fopen("in.txt","r");

    fscanf (f,"%d %d",&r1, &c1);
    matrix1 = alloc_2d_int_array(r1, c1);
     for ( i = 0; i < r1; i++)
        for ( j = 0; j < c1; j++)
            fscanf(f,"%d", &(matrix1[i][j]));

    fscanf (f,"%d %d",&r2, &c2);
    matrix2 = alloc_2d_int_array(r2, c2);
     for ( i = 0; i < r2; i++)
        for ( j = 0; j < c2; j++)
            fscanf(f,"%d", &(matrix2[i][j]));

     res = alloc_2d_int_array(r1, c2);
    for (i = 0; i < r1; i++)
        for (j = 0; j < c2; j++)
            res[i][j] = 0;


}
void display() {
    printf("\nResult Matrix is ( %d x %d):\n", r1, c2); fflush(stdout);
    for (i = 0; i < r1; ++i) {
        for (j = 0; j < c2; ++j)
            printf("%d  ", res[i][j]);

        printf("\n");
    }
}
int main(int argc, char* argv[])
{
    int myRank;
    int numofCores;
    int orderOfRow;
    int numOfRowsForEachProcess;

    MPI_Init(&argc, &argv);

    MPI_Status status;

    MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
    MPI_Comm_size(MPI_COMM_WORLD, &numofCores);

    if (myRank == 0) {
        printf("Welcome to vector Matrix multiplication program!\n");
        printf(" To read dimensions and values from file press 1\n");
        printf("To read dimensions and values from console press 2\n");
        fflush(stdout);
        scanf_s("%d",&ch);
        if(ch==1){
            files();
        }
        else{
            console();
        }

        numOfRowsForEachProcess = r1 / (numofCores - 1);
        orderOfRow = 0;


        for (i = 1; i < numofCores; i++) {
            MPI_Send(&r1, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            MPI_Send(&c1, 1, MPI_INT, i, 0, MPI_COMM_WORLD);

            MPI_Send(&r2, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            MPI_Send(&c2, 1, MPI_INT, i, 0, MPI_COMM_WORLD);

            MPI_Send(&orderOfRow, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            MPI_Send(&numOfRowsForEachProcess, 1, MPI_INT, i, 0, MPI_COMM_WORLD);

            MPI_Send(&matrix1[orderOfRow][0], numOfRowsForEachProcess * c1, MPI_INT, i, 0, MPI_COMM_WORLD);
            MPI_Send(&matrix2[0][0], r2 * c2, MPI_INT, i, 0, MPI_COMM_WORLD);


            orderOfRow += numOfRowsForEachProcess;

        }

        for (i = 1; i < numofCores; i++) {
            //int **c = alloc_2d_int_array(r1, c2);
            MPI_Recv(&orderOfRow, 1, MPI_INT, i, 0, MPI_COMM_WORLD, &status);
            MPI_Recv(&numOfRowsForEachProcess, 1, MPI_INT, i, 0, MPI_COMM_WORLD, &status);
            MPI_Recv(&res[orderOfRow][0], numOfRowsForEachProcess * c2, MPI_INT, i, 0, MPI_COMM_WORLD, &status);

            /*for (j = 0; j < numOfRowsForEachProcess; j++) {
                for (k = 0; k < c2; k++) {
                    res[orderOfRow + j][k] = c[j][k];
                }
            }*/
        }
        //Printing matrix Result
        display();
    }
    else
    {
        MPI_Recv(&r1, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        MPI_Recv(&c1, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);

        MPI_Recv(&r2, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        MPI_Recv(&c2, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);

        MPI_Recv(&orderOfRow, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        MPI_Recv(&numOfRowsForEachProcess, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);

        int **tmatrix1 = alloc_2d_int_array(numOfRowsForEachProcess,c1);
        int **tmatrix2 = alloc_2d_int_array(r2,c2);

        MPI_Recv(&tmatrix1[0][0], numOfRowsForEachProcess * c1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        MPI_Recv(&tmatrix2[0][0], r2 * c2, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);

        int **c = alloc_2d_int_array(numOfRowsForEachProcess, c2);

        for (i = 0; i < c2; ++i) {
            for (j = 0; j < numOfRowsForEachProcess; ++j) {
                c[j][i] = 0;
                for (k = 0; k < c1; ++k) {
                    c[j][i] += (tmatrix1[j][k] * tmatrix2[k][i]);
                }
            }
        }
        MPI_Send(&orderOfRow, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        MPI_Send(&numOfRowsForEachProcess, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        MPI_Send(&c[0][0], numOfRowsForEachProcess * c2, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }
    MPI_Finalize();
    return 0;
}
