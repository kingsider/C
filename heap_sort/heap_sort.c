#include <stdio.h>
#include <stdlib.h>

#define left_child(node) ( (node) * 2 + 1 )
#define right_child(node) ( (node) * 2 + 2 )

void swap(int * array, int i, int j) {
    int tmp = array[i];
    array[i] = array[j];
    array[j] = tmp;
}

void heap_it(int * array, int length, int root) {
    int leftChild = left_child(root);
    int rightChild = right_child(root);
    int biggest = root;

    if ( leftChild < length && array[root] < array[leftChild] )
        biggest = leftChild;
    if ( rightChild < length && array[biggest] < array[rightChild] )
        biggest = rightChild;
    if ( biggest != root ) {
        swap(array, biggest, root);
        heap_it(array, length, biggest);
    }
}
void make_heap(int * array, int length) {
    int i = length / 2;

    for ( ; i >= 0; --i )
        heap_it(array, length, i);
}

void heap_sort(int * array, int count) {
    int last;

    make_heap(array, count);
    for ( last = count - 1; last > 0; --last ) {
        swap(array, 0, last);
        heap_it(array, last, 0);
    }
}

int main(void) {
    int * array, count, i;

    printf("Elements count: ");
    if ( scanf("%d", &count) != 1 || count < 1 ) {
        fprintf(stderr, "DOBAVTE\n");
                return 1;
    }
    if ( ! ( array = malloc(sizeof(int) * count) ) ) {
        fprintf(stderr, "KOSHMAR\n");
        return 1;
    }
    for ( i = 0; i < count; ++i ) {
        printf("number with index[%d] = ", i);
        if ( scanf("%d", &array[i]) != 1 ) {
            fprintf(stderr, "Error\n");
            free(array);
            fprintf(stderr, "Error\n");
            return 1;
        }
    }

    heap_sort(array, count);

    printf("----------------------------------------------\n");

    for ( i = 0; i < count; ++i )
        printf("number with index[%d] = %d\n", i, array[i]);

    free(array);
        return 0;
}