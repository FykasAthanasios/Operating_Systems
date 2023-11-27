#include "bubblesort.h"

void Recordswap( Record* A, Record* B) {
    Record temp = *A;
    *A = *B;
    *B = temp;
    return;
}

void Bubblesort( Record* A , int size ) {
    int temp;
    for( int i=0; i < size-1 ; i++) {
        for ( int j=0; j < size-i-1; j++) {
            if( strcmp(A[j].lastName , A[j+1].lastName) > 0 ) {
                Recordswap( &A[j], &A[j+1]);
            }
            if (strcmp(A[j].lastName, A[j+1].lastName) == 0) {
                if ( strcmp( A[j].firstname, A[j+1].firstname) > 0) {
                    Recordswap(&A[j], &A[j+1]);
                }
                if (strcmp (A[j].firstname , A[j+1].firstname) == 0) {
                    if ( A[j].id > A[j+1].id) {
                        Recordswap(&A[j], &A[j+1]);
                    }
                }
            }
        }
    }
    return;
}
