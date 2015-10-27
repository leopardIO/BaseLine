#include <arpa/inet.h>
#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>//加上才能够找到 socaddrin
#include <netdb.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <malloc.h>
#include <string>
using namespace std;

//#include <time.h>

#define CLI_NUM 100
#define ECHO_TIMES 3000
#define SERV_PORT 6003
#define RECV_BUF_LEN 2000
#define copy_file_name "connect_time_record.txt"
#define ANNOUNCE_SUCCESS 0
#define ANNOUNCE_DEGUB 0
#define NONE 0
#define IN 1
#define OUT 2

typedef struct
{
	int id;
	int sock_fd;
    const	char * write_ptr;
    string write_str;
    char write_buf[50000];
	char * read_ptr;
	char  read_buf[RECV_BUF_LEN];
    int writed_length;
    int write_times;
	int echo_times;
    int epoll_state;
}PACK ;
extern PACK (* g_pack_ptr)[CLI_NUM];

extern void sig_fun(int signo);
extern int make_socket_non_blocking (int sfd);
extern int clean (int efd ,PACK *ptr);
//客户端的发送结构体



