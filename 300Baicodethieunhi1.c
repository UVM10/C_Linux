/*Given an array of integers nums and an integer target, return indices of the two numbers such that they add up to target.

You may assume that each input would have exactly one solution, and you may not use the same element twice.

You can return the answer in any order.*/
#include <stdio.h>
#include <stdlib.h>

int* InputandSolution(int* number, int sum, int size) {
    printf("Input your array of integers:\n");
    for (int i = 0; i < size; i++) {
        scanf("%d", &number[i]);
    }

    printf("Input your sum: ");
    scanf("%d", &sum);

    printf("Solution:\n");
    for (int i = 0; i < size; i++) {
        for (int k = i + 1; k < size; k++) {
            if (number[i] + number[k] == sum) {
                printf("Integer[%d] and Integer[%d] (values: %d + %d = %d)\n", 
                        i, k, number[i], number[k], sum);
                return;
            }
        }
    }

    printf("No solution found.\n");
}

int main() {
    int n;

    printf("Size of array is: ");
    scanf("%d", &n);

    if (n <= 0) {
        printf("Invalid array size.\n");
        return 1;
    }

    int* Integer = (int*) malloc(n * sizeof(int));
    if (Integer == NULL) {
        printf("Memory allocation failed!\n");
        return 1;
    }

    InputandSolution(Integer, 0, n);

    free(Integer);

    return 0;
}
//Done