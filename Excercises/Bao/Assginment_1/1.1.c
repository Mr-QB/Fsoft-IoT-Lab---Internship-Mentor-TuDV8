#include "stdio.h"

int *create_array(int size) {
    scanf("%d", &size);
    char *string = (char *) malloc(size * 2 * sizeof(char));
    int *array = (int *) malloc(size * sizeof(int));
    scanf("%c", &string);

    for (int i = 0; i < size; ++i) {
        char trash;
        scanf("%c", &string[i]);
        scanf("%c", &trash);
    }

    for (int i = 0; i < size; i++) {
        array[i] = string[i] - 48;
    }
    return array;
}

int *insert(int *array) {
    int value, index;
    scanf("%d%d", &value, &index);
    return array;
}

int main() {
    static int size;
    int *array = create_array(size);
    for (int i = 0; i < 5; i++) {
        printf("%d ", array[i]);
    }
    printf("%d", size);
//    int *array_ = insert(array);
    return 0;
}