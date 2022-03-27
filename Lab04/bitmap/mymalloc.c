#include <stdio.h>
#include <stdlib.h>
#include "mymalloc.h"
#include "bitmap.h"
#include "llist.h"

char _heap[MEMSIZE] = {0};
unsigned char bitmap[MEMSIZE / 8] = {0};
TNode *linked_list = NULL;

// Do not change this. Used by the test harness.
// You may however use this function in your code if necessary.
long get_index(void *ptr) {
  if (ptr == NULL) {
    return -1;
  } else {
    return (long) ((char *) ptr - &_heap[0]);
  }
}

void print_memlist() {
  print_map(bitmap, MEMSIZE / 8);
}

// Allocates size bytes of memory and returns a pointer
// to the first byte.
void *mymalloc(size_t size) {
  long bitmap_index = search_map(bitmap, MEMSIZE / 8, size);
  if (bitmap_index == -1) {
    return NULL;
  }
  allocate_map(bitmap, bitmap_index, (long) size);
  TData *data = (TData*) malloc(sizeof(TData));
  data -> len = size;
  TNode *node = make_node(bitmap_index, data);
  insert_node(&linked_list, node, ASCENDING);
  return (void*) &_heap[bitmap_index];
}

// Frees memory pointer to by ptr.
void myfree(void *ptr) {
  if (ptr == NULL) {
    return;
  }
  long bitmap_index = get_index(ptr);
  TNode *node = find_node(linked_list, bitmap_index);
  if (node == NULL) {
    return;
  }
  TData *data = node -> pdata;
  long length = data -> len;
  free_map(bitmap, bitmap_index, length);
  free(data);
  delete_node(&linked_list, node);
}

