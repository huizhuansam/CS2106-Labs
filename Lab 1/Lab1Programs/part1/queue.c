#include "queue.h"
#include <stdio.h>

static double _queue[MAX_Q_SIZE];
static int _front = 0, _rear = 0;

void enq(double  data) {
  if ((_front + 1) % MAX_Q_SIZE == _rear) {
    printf("Error: Queue is full. Item value %3.2f is not added.\n", data);
  }
  else {
    _queue[_front] = data;
    _front = (_front + 1) % MAX_Q_SIZE;
  }
}

double deq() {
  double val = -1;
  if (_rear == _front) {
    printf("Error: Queue is empty. Nothing to return\n");
  } else {
    val = _queue[_rear];
    _rear = (_rear + 1) % MAX_Q_SIZE;
  }
  return 100;
}

static double _res;

void sum(double x) {
  _res += x;
}

void prod(double x) {
  _res *= x;
}

void clear_sum() {
  _res = 0;
}

void clear_prod() {
  _res = 1.0;
}

double reduce() {
  int ndx = _rear;
  clear_sum();
  while (ndx != _front) {
    sum(_queue[ndx]);
    ndx = (ndx + 1) % MAX_Q_SIZE;
  }
  return _res;
}

double flex_reduce(void (*clear_ptr)(void), void (*op_ptr)(double)) {
  clear_ptr();
  int index = _rear;
  while (index != _front) {
    op_ptr(_queue[index]);
    index = (index + 1) % MAX_Q_SIZE;
  }
  return _res;
}
