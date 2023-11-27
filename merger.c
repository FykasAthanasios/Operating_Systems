#include "merge.h"

void merge(Record *array, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    // Create temp arrays
    Record *L = (Record *)malloc(n1 * sizeof(Record));
    Record *R = (Record *)malloc(n2 * sizeof(Record));

    // Copy data to temp arrays L[] and R[]
    for (int i = 0; i < n1; i++)
        L[i] = array[left + i];
    for (int j = 0; j < n2; j++)
        R[j] = array[mid + 1 + j];

    // Merge the temp arrays back into array[left..right]
    int i = 0; // Initial index of first subarray
    int j = 0; // Initial index of second subarray
    int k = left; // Initial index of merged subarray
    while (i < n1 && j < n2) {
        if (strcmp(L[i].lastName, R[j].lastName) < 0) {
            array[k] = L[i];
            i++;
        } 
        else if (strcmp(L[i].lastName, R[j].lastName) > 0) {
            array[k] = R[j];
            j++;
        } 
        else {
            // Last names are equal, compare first names
            if (strcmp(L[i].firstname, R[j].firstname) < 0) {
                array[k] = L[i];
                i++;
            } 
            else if (strcmp(L[i].firstname, R[j].firstname) > 0) {
                array[k] = R[j];
                j++;
            } 
            else {
                // Both last names and first names are equal, compare IDs
                if (L[i].id < R[j].id) {
                    array[k] = L[i];
                    i++;
                } else {
                    array[k] = R[j];
                    j++;
                }
            }
        }
        k++;
    }

    // Copy the remaining elements of L[], if there are any
    while (i < n1) {
        array[k] = L[i];
        i++;
        k++;
    }

    // Copy the remaining elements of R[], if there are any
    while (j < n2) {
        array[k] = R[j];
        j++;
        k++;
    }

    // Free temporary arrays
    free(L);
    free(R);
}
