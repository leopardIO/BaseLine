#include "globle.h"
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
using namespace std;
int count = 0;
PACK( *g_pack_ptr)[CLI_NUM] ;
string uint_to_string(uint32_t in)
{
    char buf[32] = {0};
    sprintf(buf,"%u",in);
    string str=buf;
    return str;
}
string read_picture( string path)
{
    fstream fs( path.c_str() ) ; // 创建个文件流对象,并打开"file.txt"
    stringstream ss ;        // 创建字符串流对象
    ss << fs.rdbuf()     ;     // 把文件流中的字符输入到字符串流中
    string str = ss.str() ;    // 获取流中的字符串
    cout<<" len file "<<str.length()<<endl;
    return str;
}
int
main(/*int argc, char **argv*/)
{
    string file = read_picture("./test.jpg");
	string send_body = 
        "{\"Content-Length\":\"36124\",\"picture_length\":\"36124\",\"picture_name\":\"test\"}";
    string head = "0000000000";
    string body_len = uint_to_string(send_body.length()); 
    string send_str = head+body_len+send_body+file;
    cout<<"send_str length : "<<send_str.length()<<endl;
    uint32_t nleft;
	int epoll_fd;  
	FILE * copy_fd; 
	int cnt_fail = 0;
#ifdef TIME
    float  time_use[ CLI_NUM ][ECHO_TIMES];
    struct timeval start[CLI_NUM][ECHO_TIMES];
    struct timeval end[CLI_NUM][ECHO_TIMES];
    struct timeval con_start[CLI_NUM];
    struct timeval con_end[CLI_NUM];
    float con_time[CLI_NUM]; 
#endif

	struct sockaddr_in	servaddr;
	struct epoll_event events[CLI_NUM];
    struct epoll_event event;
    PACK * ptr;
	PACK pack[CLI_NUM];
    g_pack_ptr = &pack;
    
    memset(&event ,0,sizeof(struct epoll_event));	
    if(signal(SIGINT , sig_fun ) == SIG_ERR)
    {
        printf("sig_func err\n");
    }
    if(signal(SIGPIPE,SIG_IGN) == SIG_ERR );
    {
        printf("SIGPIPE ignore err\n");
    }
    epoll_fd = epoll_create(CLI_NUM);
	for (int i = 0; i < CLI_NUM; i++) 
	{
#ifdef TIME
        gettimeofday(&con_start[i],NULL);
#endif
		pack[i].sock_fd = socket(AF_INET, SOCK_STREAM, 0);
		pack[i].id = i;

	    pack[i].write_str = send_str;
		pack[i].write_ptr = pack[i].write_str.c_str();		
		pack[i].read_ptr = pack[i].read_buf;
        pack[i].writed_length = 0;

        pack[i].write_times = 0;
		pack[i].echo_times = 0;
		bzero(&servaddr, sizeof(servaddr));
		servaddr.sin_family = AF_INET;
		servaddr.sin_port = htons(SERV_PORT);
//		inet_pton(AF_INET, argv[1], &servaddr.sin_addr);		
        inet_pton(AF_INET, "127.0.0.1", &servaddr.sin_addr); 
		int s = connect(pack[i].sock_fd, (struct sockaddr *) &servaddr, sizeof(servaddr));
		if(s == 0)
        {
			printf("socket : %d connect success\n",pack[i].sock_fd);
		}       
		if(s == -1)
		{
			printf("connect 返回 -1 sock_buf : %d , i: %d \n",pack[i].sock_fd,i);
            perror("");
			close (pack[i].sock_fd);
            printf("close sock_buf : %d \n",pack[i].sock_fd );
			cnt_fail ++;
			continue;
		}
#ifdef TIME
        gettimeofday(&con_end[i],NULL);
        con_time[i]=(con_end[i].tv_sec-con_start[i].tv_sec)*1000000+(con_end[i].tv_usec-con_start[i].tv_usec);
#endif
            
		if(0 != make_socket_non_blocking (pack[i].sock_fd)) 
			printf("make_socket_non_blocking error\n");
		event.events = EPOLLOUT | EPOLLIN;
        event.data.ptr = &pack[i];
        epoll_ctl(epoll_fd , EPOLL_CTL_ADD , pack[i].sock_fd , &event);
	}
    int epoll_ready_num;
	while(count < 100)
	{
		epoll_ready_num = epoll_wait(epoll_fd , events, CLI_NUM ,20);
		for(int i = 0; i < epoll_ready_num ;i++)
		{
			if(events[i].events & EPOLLOUT)			
            {
				ptr = (PACK *)events[i].data.ptr;
                nleft = ptr->write_str.length() - ptr->writed_length;
#ifdef ANNOUNCE_DEGUB
				if(nleft <= 0 )
				{
					printf("temp_buf 居然<= 0 这是不该出现的\n");
				}
#endif
#ifdef TIME
                if(nleft == ptr->write_str.length())
                    gettimeofday(&start[ptr->id][ptr->write_times],NULL);
#endif
				//将数据写出去
                int nwritten;
				nwritten = write (ptr->sock_fd , ptr->write_ptr ,nleft);//+++++++++++++这里可以判断一下
				if(nwritten <= 0)
				{
					if(errno == EAGAIN || errno == EWOULDBLOCK || errno == EINTR)
					{
						nwritten = 0;
                        continue;
					}
					else 
					{
						printf("epoll write error ,socket : %d \n " ,((PACK *)events[i].data.ptr)-> sock_fd);
						perror("");
                        abort();
					}
				}
				//修改PACK数据结构里面的数据
				else
				{
                    cout<< " total "<< ptr->write_str.length() <<":" <<nwritten<<endl;
					ptr->write_ptr += nwritten;
                    ptr->writed_length += nwritten;
					if((uint32_t)nwritten == nleft)
					{
                        count++;
                        ptr->write_times++;
						ptr->write_ptr = ptr->write_str.c_str();
                        ptr->writed_length = 0;
#ifdef TIME
						event.events = EPOLLIN;
						event.data.ptr = ptr;
						if(-1 == epoll_ctl(epoll_fd , EPOLL_CTL_MOD ,ptr->sock_fd ,&event))
						{
							printf("chang mod err %d\n",ptr->sock_fd);
							clean(epoll_fd , ptr);
						}
#endif
					}
				}
			}
			//如果是可读
			if(events[i].events & EPOLLIN)
			{
				//将数据全部读了
				ptr = (PACK *)events[i].data.ptr;
                int nread;
				while(( nread = read ( ptr -> sock_fd ,ptr ->read_ptr, 
                                RECV_BUF_LEN) )> 0 )
                {
					ptr ->read_ptr += nread;
				}
				if(nread <= 0)
                {
                    if(errno ==  EAGAIN || errno == EWOULDBLOCK ||  errno == EINTR)
				    {
					//  perror("资源暂时不可用\n");                   
				    }
                    else
                    {
                        perror("nread return -1\n");
						clean (epoll_fd , ptr);
                        abort();
                    }
                }
				else 
                {
                   // count++;
             //       cout<<"#################recv #################### len "<<nread<<endl;
                    ptr->read_ptr = ptr -> read_buf;
#ifdef TIME
                    gettimeofday(&end[ptr->id][ptr->echo_times],NULL);
                    time_use[ptr->id][ptr->echo_times]
                        =(end[ptr->id][ptr->echo_times].tv_sec-start[ptr->id][ptr->echo_times].tv_sec)
                        *1000000+(end[ptr->id][ptr->echo_times].tv_usec-start[ptr->id][ptr->echo_times].tv_usec)
                            ;//微秒         
					ptr->echo_times ++;
                    if(ptr -> echo_times == ECHO_TIMES/2)
                    {
                        printf("%d echo %d times \n",ptr->sock_fd,ptr->echo_times);
                    }
					if(ptr -> echo_times == ECHO_TIMES)
					{
						count ++;
                        printf("%d echo over (%d times)\n",ptr->sock_fd,ptr->echo_times);
						clean(epoll_fd ,ptr);
                        continue;
					}
                    
					event.events = EPOLLOUT;
					event.data.ptr = ptr;
					if(-1 == epoll_ctl (epoll_fd , EPOLL_CTL_MOD , ptr->sock_fd , &event))
					{
						printf("chang mod err%d\n",ptr->sock_fd);
						clean (epoll_fd , ptr);
					}
#endif
				}
			}
		}
	}
    sleep(10);
    printf("*****write file*****\n");
	if((copy_fd = fopen(copy_file_name,"w") ) == NULL)
	{
		printf("creat file :%s fail.\n",copy_file_name);
		return 1;
	}
#ifdef TIME
	float total =0 ;
    float con_total =0 ;
    float min = time_use[0][0];
    float max = time_use[0][0];
    float avrg = 0;
    float min_r = con_time[0];
    float max_r = con_time[0];
    float avrg_r = 0;
	for (i = 0; i < CLI_NUM; i++) 
	{
        fprintf(copy_fd ,"id : %d us,con_time : %f us \ntime_use :\n",i,con_time[i]);
        for(j = 0 ; j < ECHO_TIMES ; j++)
        {
            fprintf(copy_fd ," %f ",time_use[i][j]);
            total += time_use[i][j];
            if(min > time_use[i][j])
                min = time_use[i][j];
            if(max < time_use[i][j])
                max = time_use[i][j];
        }
        if(min_r > con_time[i])
            min_r = con_time[i];
        if(max_r < con_time[i])
            max_r = con_time[i];
        con_total += con_time[i];
	}
    avrg = total / (CLI_NUM-cnt_fail) / ECHO_TIMES;
    avrg_r = con_total /CLI_NUM;
    printf("CLI : %d ,DES_NUM : %d min %.2f max %.2f average \
            : %.2f \n",CLI_NUM , DESIGN_NUM , min/1000 ,max/1000 ,avrg/1000); 
    printf("con min_r %.2f max_r %.2f avrg_r %.2f \n",min_r/1000,max_r/1000 , avrg_r/1000);
	fclose (copy_fd);
#endif        
	return 0;
}
