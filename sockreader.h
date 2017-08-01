#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include <assert.h>
#include <float.h>

#define IP "192.168.1.1"
#define PORT 1338
#define BUFFSIZE 1024

typedef struct data{
	char * int_buffer;
	char * char_buffer;
	int fd;
	long answer;
	int i_offset;
	int c_offset;
	long answer_type;
} Data;

char* dynamic_reader(int fd, char *buffer, int *offset);
void init_data(Data *d);
long calculate_value(Data * d);
long find_calc_type(Data * d);
int get_answer_type(long prob);
int write_answer(Data * d);
int get_socket();
void free_mem(Data* d);
void increase_mem(char ** buf, int offset);
void init_buffer_memory(char ** buffer);
int check_finished(char * buffer);

/* compiler hopefully can inline these quick functions  */
static inline long maxval(long x, long y){
	return (x > y) ? x : y;
}
static inline long minval(long x, long y){
	return (x < y) ? x : y;
}
static inline long sumval(long x, long y){
	return (x + y);
}
