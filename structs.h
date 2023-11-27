#include "lib.h"
#define buffername 20
#define bufferTK 6
#define READ 0
#define WRITE 1

typedef enum { No, Yes } bool;

//the struct is public thats why its declarated in structs.h file
typedef struct Record{
	int  id;
	char lastName[buffername];
	char firstname[buffername];
	char TK[bufferTK];
}Record;
