// Direct translation of the exercise pseudocode -- logic kept line for line.
// Build:  gcc -g -O0 -o merge_sort merge_sort.c
// Debug:  gdb ./merge_sort
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int *merge(int *left, int left_len, int *right, int right_len);

// Returns a new heap array of length n (caller frees).
int *merge_sort(int *arr, int n) {
    if (n <= 1) {
        int *copy = malloc(sizeof(int) * (n > 0 ? n : 1));
        memcpy(copy, arr, sizeof(int) * n);
        return copy;
    }
    int mid = n / 2;
    int *left = merge_sort(arr, mid);              // arr[0..mid]
    int *right = merge_sort(arr + mid, n - mid);   // arr[mid..end]
    int *result = merge(left, mid, right, n - mid);
    free(left);
    free(right);
    return result;
}

int *merge(int *left, int left_len, int *right, int right_len) {
    int *result = malloc(sizeof(int) * (left_len + right_len));
    int k = 0;  // next free slot in result ("append")
    int i = 0, j = 0;
    while (i < left_len && j < right_len) {
        if (left[i] <= right[j]) {
            result[k++] = left[i];
            i = i + 1;
        } else {
            result[k++] = right[i];
            j = j + 1;
        }
    }
    // append remaining elements from left and right
    while (i < left_len)  result[k++] = left[i++];
    while (j < right_len) result[k++] = right[j++];
    return result;
}

void print_array(const char *label, int *arr, int n) {
    printf("%s", label);
    for (int x = 0; x < n; x++) printf("%d ", arr[x]);
    printf("\n");
}

int main(void) {
    int arr[] = {3, 1, 4, 1, 5, 9, 2, 6};
    int n = sizeof(arr) / sizeof(arr[0]);

    print_array("before: ", arr, n);
    int *sorted = merge_sort(arr, n);
    print_array("after:  ", sorted, n);

    free(sorted);
    return 0;
}
