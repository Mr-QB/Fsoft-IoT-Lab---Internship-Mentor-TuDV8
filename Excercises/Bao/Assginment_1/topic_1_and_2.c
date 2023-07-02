#include "stdio.h"
#include "stdlib.h"

int *create_array(int *size) {
    /*
     * Create an arrray and update its values from user input.
     *
     * @Param: Pointer address of size of array.
     * @Return: The created array with value update from user input.
     * */
    scanf("%d", size);
    char *string = (char *) malloc(*size * 2 * sizeof(char));
    int *array = (int *) malloc(*size * sizeof(int));
    scanf("%c", &string);

    for (int i = 0; i < *size; ++i) {
        char trash;
        scanf("%c", &string[i]);
        scanf("%c", &trash);
    }

    for (int i = 0; i < *size; i++) {
        array[i] = string[i] - 48;
    }
    return array;
}

int *insert(int *array, int *size) {
    /*
     * Add new value to array at a given position
     *
     * @Param: Array and size of array
     * @Return: New array
     * */
    int value, index;   // Value and position of value in array
    scanf("%d%d", &index, &value);
    (*size)++;  // Update size of the array

    int *new_array = (int *) malloc(*size * sizeof(int));
    for (int i = 0; i < *size; ++i) { //Update value for new array
        if (i < index) {
            new_array[i] = array[i];
        } else if (i == index) {
            new_array[i] = value;
        } else {
            new_array[i] = array[i - 1];
        }
    }
    return new_array;
}

int main() {
    int size = 5;
    int *array = create_array(&size);
    int *array_ = insert(array, &size);
    free(array);
    for (int i = 0; i < size; i++) {
        printf("%d ", array_[i]);
    }
    return 0;
}