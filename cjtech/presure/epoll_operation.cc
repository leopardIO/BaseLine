#include "globle.h"

/*********************
����
����:����socketΪ��������
*********************/
int make_socket_non_blocking (int sfd)
{
	int flags, s;

	//�õ��ļ�״̬��־
	flags = fcntl (sfd, F_GETFL, 0);
	if (flags == -1)
	{
	  perror ("error from funtion  : fcntl ����");
	  return -1;
	}

	//�����ļ�״̬��־
	flags |= O_NONBLOCK;
	s = fcntl (sfd, F_SETFL, flags);
	if (s == -1)
	{
	  perror ("error from funtion : fcntl ����");
	  return -1;
	}
	return 0;
}


void sig_fun(int signo)
{
    signo =0;
    int i;
    for(i = 0 ; i < CLI_NUM; i++)
    {
        printf("free num : %d success\n",i);
    }
    raise(SIGQUIT);
}
int clean (int efd ,PACK *ptr)
{
	struct epoll_event event;
	memset (&event , 0 , sizeof(struct epoll_event));
	epoll_ctl(efd , EPOLL_CTL_DEL , ptr->sock_fd , &event);
	return 0;

}
