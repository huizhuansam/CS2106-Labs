#include <stdio.h>
#include <stdlib.h>
#include "mymalloc.h"
#include "llist.h"

char _heap[MEMSIZE] = {0};
TNode *_memlist = NULL; // To maintain information about length

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
  if (_memlist == NULL) {
    printf("Status: FREE Start index: 0 Length: %d\n", MEMSIZE);
    return;
  }
  reset_traverser(_memlist, FRONT);
  TNode *trav = _memlist;
  while (trav) {
    TData *data = trav -> pdata;
    printf("Status: %s Start index: %d Length: %ld\n", 
        data -> status == OCCUPIED ? "ALLOCATED" : "FREE", 
        trav -> key, 
        data -> size);
    trav = trav -> next;
  }
}

// Allocates size bytes of memory and returns a pointer
// to the first byte.
void *mymalloc(size_t size) {
  TData *data = (TData*) malloc(sizeof(TData));
  data -> size = size;
  data -> status = OCCUPIED;

  if (_memlist == NULL) {
    TData *hole_data = (TData*) malloc(sizeof(TData));
    hole_data -> size = MEMSIZE;
    hole_data -> status = HOLE;
    TNode *hole_node = make_node(0, hole_data);
    insert_node(&_memlist, hole_node, ASCENDING);
  }

  TNode *trav = _memlist;

  while (trav) {
    TData *trav_data = trav -> pdata;
    if ((trav_data -> status) == HOLE && (trav_data -> size) >= size) {
      unsigned int index = trav -> key;
      TNode *node = make_node(index, data);
      if ((trav_data -> size) > size) {
        TData *hole_data = (TData*) malloc(sizeof(TData));
        hole_data -> size = (trav_data -> size) - size;
        hole_data -> status = HOLE;
        unsigned int hole_index = index + size;
        TNode *hole_node = make_node(hole_index, hole_data);
        insert_node(&_memlist, hole_node, ASCENDING);
      }
      free(trav -> pdata);
      trav -> pdata = NULL;
      delete_node(&_memlist, trav);
      insert_node(&_memlist, node, ASCENDING);
      return (void*) &_heap[index];
    }
    trav = trav -> next;
  }
  return NULL;
}

// Frees memory pointer to by ptr.
void myfree(void *ptr) {
  if (ptr == NULL) {
    return;
  }

  long index = get_index(ptr);
  if (index == -1) {
    return;
  }

  TNode *node = find_node(_memlist, index);
  if (node == NULL) {
    return;
  }
  TData *data = node -> pdata;
  data -> status = HOLE;
  TNode *next = node -> next;
  if (next) {
    TData *next_data = next -> pdata;
    if (next_data -> status == HOLE) {
      data -> size = (data -> size) + (next_data -> size);
      free(next -> pdata);
      next -> pdata = NULL;
      merge_node(_memlist, node, 1);
    }
  }

  TNode *prev = node -> prev;
  if (prev) {
    TData *prev_data = prev -> pdata;
    if (prev_data -> status == HOLE) {
      prev_data -> size = (prev_data -> size) + (data -> size);
      free(node -> pdata);
      node -> pdata = NULL;
      merge_node(_memlist, prev, 1);
    }
  }
  if (_memlist -> pdata -> size == MEMSIZE) {
    free(_memlist -> pdata);
    _memlist -> pdata = NULL;
    purge_list(&_memlist);
  } 
}

