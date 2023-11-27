#include "quicksort.h"

void Recordswap(Record* A, Record *B) {
    Record temp = *A;
    *A = *B;
    *B = temp;
    return;
}

void Quicksort( Record* A, int low, int high) {
    if (low < high) {
        int pivot = Partition(A, low, high);
        Quicksort(A, low, pivot); // Quicksort left partition
        Quicksort(A, pivot + 1, high); // Quicksort right partition
    }
    return;
}

int Partition(Record* A, int low, int high) {
    Record* pivot = &A[low];
    int leftwall = low;

    for (int i = low + 1; i < high; i++) {
        if (strcmp(A[i].lastName, pivot->lastName) < 0) {
            leftwall++;
            Recordswap(&A[leftwall], &A[i]);
        }
        if (strcmp(A[i].lastName, pivot->lastName) == 0) {
            if ( strcmp( A[i].firstname, pivot->firstname) < 0) {
                leftwall++;
                Recordswap(&A[leftwall], &A[i]);
            }
            if (strcmp (A[i].firstname , pivot->firstname) == 0) {
                if ( A[i].id > pivot->id) {
                    leftwall++;
                    Recordswap(&A[leftwall], &A[i]);
                }
            }
        }
    }
    Recordswap(&A[low], &A[leftwall]);
    return leftwall;
}
