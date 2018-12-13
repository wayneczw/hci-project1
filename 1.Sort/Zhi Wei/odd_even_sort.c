#include <omp.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>


void OddEvenSortOpt (int *arr, int N){
    /* flag0 is to track even phase */
    /* flag1 is to track odd phase */
    /* flag is to track if at least 1 iteration of scanning is done,
    flag is set as 0, because we want at least 1 iteration for off phase */
    int flag0, flag1 = 1, flag = 0, i;

    while(flag1){
        flag0 = 0;
        flag1 = 0;

        /* The omp parallel directive explicitly instructs the 
        compiler to parallelize the chosen block of code. */
        // #pragma omp parallel
        {
            int temp;

            /* even phase */
            /* The omp for directive instructs the compiler to 
            distribute loop iterations within the team of threads
            that encounters this work-sharing construct. */
            #pragma omp parallel for private(temp,i,flag0) shared(arr)
            for(i=0; i<N-1; i+=2){
                if(arr[i] > arr[i+1]){
                    temp = arr[i];
                    arr[i] = arr[i+1];
                    arr[i+1] = temp;
                    flag0 = 1;
                } //end if
            } //end for

            /* odd phase */
            /* if even phase had made exchange(s),
            re-examine odd phase too;
            else, no need to examine odd phase anymore */
            if(flag0 || !flag){
                #pragma omp parallel for private(temp,i,flag1) shared(arr)
                for(i=1; i<N-1; i+=2){
                    if(arr[i] > arr[i+1]){
                        temp = arr[i];
                        arr[i] = arr[i+1];
                        arr[i+1] = temp;
                        flag1 = 1;
                    } //end if
                } //end for
            } //if flag0
        } //end parallel

        /* flag is set to 1, indicating that at least
        1 iteration has been done */
        flag = 1;
    } //end while
}


void OddEvenSort (int *arr, int N){
    int flag = 1, start = 0, i;
    int temp;

    /* stop when no exchange is done
    at the particular iteration */
    while(flag || start){
        flag = 0;
        // #pragma omp parallel for private(temp,i,flag) shared(arr)
        for(i=start; i<N-1; i+=2){
            if(arr[i] > arr[i+1]){
                temp = arr[i];
                arr[i] = arr[i+1];
                arr[i+1] = temp;
                flag = 1;
            } //end if
        } //end for

        if(start == 0){
            start = 1;
        } else{
            start = 0;
        } //end if
    } //end while
}


// A utility function to swap two integers 
void swap (int *a, int *b) { 
    int temp = *a;
    *a = *b; 
    *b = temp; 
}


// A function to generate a random permutation of arr[] 
void shuffle (int arr[], int n) { 
    // Use a different seed value so that we don't get same 
    // result each time we run this program 
    srand (time(NULL)); 

    int i;
    // Start from the last element and swap one by one. We don't 
    // need to run for the first element that's why i > 0 
    for(i=n-1; i>0; i--){ 
        // Pick a random index from 0 to i 
        int j = rand() % (i+1); 

        // Swap arr[i] with the element at random index 
        swap(&arr[i], &arr[j]); 
    } 
}


double average (double a[], int n){ 
    /* Find sum of array element */ 
    double sum = 0;
    int i;
    for (i=0; i<n; i++) 
        sum += a[i]; 

    return sum/n; 
}


void copy (int *source, int *target, int N){
    int i;
    for (i=0; i<N; i++){
        target[i] = source[i];
    }
}


int main (int argc, char *argv[]){
    /* Some initializations */
    int n = 1000, c = 100, i;
    int a[n];
    for (i=0; i<n; i++)
        a[i] = i;

    /* experiment */
    double resultA[c], resultB[c];
    double begin, end;
    int tmp[n];
    for(i=0; i<c; i+=1){
        /* randomly shuffle a and b */
        shuffle(a, n);

        /* original algorithm */
        copy(a, tmp, n);
        begin = omp_get_wtime();
        OddEvenSort(tmp, n);
        end = omp_get_wtime();
        resultA[i] = (double) (end - begin);
        // printf("A: %e\n", resultA[i]);

        /* optimised algorithm */
        copy(a, tmp, n);
        begin = omp_get_wtime();
        OddEvenSortOpt(tmp, n);
        end = omp_get_wtime();
        resultB[i] = (double) (end - begin);
        // printf("B: %e\n", resultB[i]);
    }

    /* print result */
    double result_a, result_b;
    result_a = average(resultA, c);
    result_b = average(resultB, c);

    printf("Original OddEvenSort: %f\n", result_a);
    printf("Optimised OddEvenSort: %f\n", result_b);
}
