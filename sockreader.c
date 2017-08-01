#include "sockreader.h"

int main(){

    int finished = 0;
    
    /* main structure of the program  */
    Data * info = malloc(sizeof(Data)) ;
    init_data(info);
    /* open up a socket connection */
    info->fd = get_socket();
    /* malloc initial data for buffers  */
    init_buffer_memory(&info->int_buffer);    
    init_buffer_memory(&info->char_buffer);
    
    /* run program through finish "good" or "nope"  */
    while(!finished){
	/* read into dynamic int_buffer  */
	dynamic_reader(info->fd, info->int_buffer, &info->i_offset);
	/* if we are done, cleanup and go home  */
	if( (finished = check_finished(info->int_buffer) == 1 )){
	    break;
	}
	/* read into dynamic char_buffer  */
	dynamic_reader(info->fd, info->char_buffer, &info->c_offset);
	/* if we are done, cleanup and go home  */
	if( (finished = check_finished(info->char_buffer) == 1 )){
	    break;
	}
	/* parse char_buffer to find if we want the max, min or sum  */
	find_calc_type(info);
	/* parse int_buffer to find desired value  */
	calculate_value(info);
	/* write back to socket */
	write_answer(info);
	/* wipe out what was in the buffers for next iteration */
	memset(info->int_buffer, 0, info->i_offset);
	memset(info->char_buffer, 0, info->c_offset);
    }
    
    /* return the memory  */
    free_mem(info);
    free(info);

    /* shut down socket */
    close(info->fd);
    return 0;
}

/* set some initial vars for buffer offsets and answer  */
void init_data(Data *d){
	d->i_offset = BUFFSIZE;
	d->c_offset = BUFFSIZE;
	d->answer = 0;
}

/* function will check a buffer for "good" or "nope" stopping criteria   */
int check_finished(char * buffer){
    char * good = "good";
    char * nope = "nope";
    if( (strstr(buffer, good)) != NULL || (strstr(buffer, nope)) != NULL){
	printf("%s",buffer);
	return 1;
    }
    return 0;
}

/*  malloc memory for buffer of char * type  */
void init_buffer_memory(char ** buffer){
    *buffer = malloc(BUFFSIZE);
    if (buffer == NULL){
	perror("malloc failed!");
    }
}

/* read from socket -> buffer as input, dynamically reallocates memory if needed  */
char * dynamic_reader(int fd, char *buffer, int *offset){
    int bytes = 0;
    /* set up buffers for reading int list and char list->offset for increasing memory  */
    if( (bytes = read(fd, buffer, *offset) <=0)) {
	perror("Something went wrong with the read!");   
    }
    /* in case we need to increase the memory in buf */
    while( bytes > 0 ){
	increase_mem(&buffer, *offset*2);
	if( (bytes = read(fd, buffer+*offset,*offset) <=0 )){
	    perror("Something went wrong with the read!");   
	}
	*offset*=2;
    }
    return buffer;
}

void increase_mem(char ** buf, int offset){
    /* grow like a vector */
    char * mem = realloc(*buf, offset);
    if (mem != NULL){
	*buf = mem;
    }
    else{
	perror("Something happened with realloc");
    }
}

/* free malloc'd memory from Data * d  */
void free_mem(Data* d){
    free(d->int_buffer);
    free(d->char_buffer);
}


/* return a file descriptor for socket */
int get_socket(){
    int socket_fd;
    /*socket file descriptor */
    /* open TCP socket handle failure */
    if( (socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0 ){
	perror("socket failed");
	exit(errno);
    }
    /* struct configs -> hard coded connection details */
    struct in_addr inaddress;
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr(IP);
    server_address.sin_port = htons(PORT);
    /* open connection with new file descriptor for reading, handle failure */
    if( (connect(socket_fd, (struct sockaddr *) &server_address, sizeof(server_address)) ) != 0 ){
	perror("connection failed");
	exit(errno);
    };
    return socket_fd;	
}

/* function take a long, returns the solution type { MAX, MIN, SUM } -> eh enums  */
int get_answer_type(long prob){
    if(prob < 0){
	return 0;
    }
    else if(prob > 0){
	return 1;
    }
    else{
	return 2;
    }
}

/* parse the char -> int array to find the answer  */
long calculate_value(Data * d){
    char *space = " ";
    char * temp = d->int_buffer;
    long x;
    char *token = strtok(temp,space);
    /* breaking up string buffer by space delimeter is easy work for strtok  */
    while(token != NULL){
	/* using longs in case of number size  */
	x = atol(token);
	/* have to see each number once anyway->with these 3 functions
	   might as well keep running tally of correct answer-> n compares max */
	switch(d->answer_type){
	    case 0:
		d->answer = maxval(d->answer, x);
		break;
	    case 1:
		d->answer = minval(d->answer, x);
		break;
	    case 2:
		d->answer = sumval(d->answer, x);
		break;
	}
	token = strtok(NULL,space);
    }
    return d->answer;
}

/* take in the char buffer->easy pickin's for strstr to find calc type  */
long find_calc_type(Data * d){
    char *maximum = "max";
    char *minimum = "min";
    char *summation = "sum";
    if( strstr (d->char_buffer,maximum) != NULL ){
	d->answer = LONG_MIN;
	d->answer_type = 0;
	return 0;
    }
    else if( strstr (d->char_buffer,minimum) != NULL ){
	d->answer = LONG_MAX;
	d->answer_type = 1;
	return 0;
    }
    else if( strstr (d->char_buffer,summation) != NULL ){
	d->answer = 0;
	d->answer_type = 2;
	return 0;
    }
    else{
	/* something went wrong  */
	return -1;
    }
    return 0;
}

/* write back to file descriptor -> taking care for format & long->bytes  */
int write_answer(Data * d){
    const int n = snprintf(NULL,0,"%lu",d->answer);
    assert(n>0);
    char buf[n+1];
    int s = snprintf(buf,n+1,"%lu",d->answer);
    assert(buf[n] == '\0');
    assert(s==n);
    /* htonl back to network  */
    int convert = htonl(d->answer);
    if ( (write(d->fd,buf,n) > 0) ){
	return 1;
    }
    return -1;
}

