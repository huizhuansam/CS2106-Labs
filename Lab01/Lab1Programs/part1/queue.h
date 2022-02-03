// Maximum number of elements in a queue
#define MAX_Q_SIZE	10

// Function pointers
void (*clear_ptr)(void);
void (*op_ptr)(double);

// Function prototypes
void enq(double);
double deq(void);
void sum(double);
void prod(double);
void clear_sum(void);
void clear_prod(void);
double reduce(void);
double flex_reduce(void (*clear_ptr)(void), void (*op_ptr)(double));
