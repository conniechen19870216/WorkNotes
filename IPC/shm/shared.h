#define IPC_KEY 1234
#define SHARED_MEM_SIZE 4096
#define TEXT_SIZE 2048

typedef struct exchange
{
	int  flag;
	char text[TEXT_SIZE];
} Exchange;
