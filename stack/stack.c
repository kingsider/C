#include <stdio.h>
#include <stdlib.h>

#define MAX 10

int count = 0;
struct stack {
  int items[MAX];
  int top;
};

typedef struct stack st;

void createEmptyStack(st *s) {
  s->top = -1;
}

int isfull(st *s) {
  if (s->top == MAX - 1)
    return 1;
  else
    return 0;
}

int isempty(st *s) {
  if (s->top == -1)
    return 1;
  else
    return 0;
}

void push(st *s, int newitem) {
  if (isfull(s)) {
    printf("���� ��������");
  } else {
    s->top++;
    s->items[s->top] = newitem;
  }
  count++;
}

void pop(st *s) {
  if (isempty(s)) {
    printf("\n ���� ���� \n");
  } else {
    printf("������ �������= %d", s->items[s->top]);
    s->top--;
  }
  count--;
  printf("\n");
}

void printStack(st *s) {
  printf("����: ");
  for (int i = 0; i < count; i++) {
    printf("%d ", s->items[i]);
  }
  printf("\n");
}

int main() {
  int ch;
  st *s = (st *)malloc(sizeof(st));

  createEmptyStack(s);

  push(s, 1);
  push(s, 2);
  push(s, 3);
  push(s, 4);

  printStack(s);
  pop(s);

  printf("\n����� ��������\n");
  printStack(s);
}
