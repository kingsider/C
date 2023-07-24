#include <stdio.h>

void bubble_sort(int *arr, int n);

int main() {
    int n;
    int arr[100];
    int i, input_value;

    printf("������� ���������� ����� ��� ���������� (�������� 100): ");
    scanf("%d", &n);

    if (n > 100) {
        printf("������! �� ����� ���������� ����� ������ 100.\n");
        return 1;
    }

    printf("������� ����� ��� ����������: \n");
    for (i = 0; i < n; i++) {
        scanf("%d", &input_value);
        if (input_value < 10000 && input_value > -10000) {
            arr[i] = input_value;
        } else {
            printf("������! ������� �����, ����������� 4 �����.\n");
            return 1;
        }
    }

    bubble_sort(arr, n);

    printf("��������������� ������:\n");
    for (i = 0; i < n; i++) {
        printf("%d\n", arr[i]);
    }

    return 0;
}
void bubble_sort(int *arr, int n) {
    int i, j, temp;
    for (i = 0; i < n; i++) {
        for (j = 0; j < n - 1 - i; j++) {
            if (arr[j] > arr[j + 1]) {
                temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}
