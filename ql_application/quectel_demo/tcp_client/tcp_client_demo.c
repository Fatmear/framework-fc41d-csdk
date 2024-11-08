#include "tcp_client_demo.h"
#include "include.h"

#if TCP_CLIENT_DEMO
#include "include.h"
#include "rtos_pub.h"
#include "doubly_list.h"
#include "mem_pub.h"
#include "str_pub.h"
#include "lwip/sockets.h"
#include "BkDriverUart.h"
#include "error.h"
#include "uart_pub.h"
#include "lwip/sockets.h"
#include "lwip/ip_addr.h"
#include "lwip/inet.h"
#include <string.h>

struct ip_port_msg
{
    ip4_addr_t IP;
    in_port_t  sin_port;
};

int num_string2uint16num(char *Numstring)
{
    int len;
    unsigned int Num = 0;
    unsigned int j, temp;
    unsigned int i = 0;

    len = os_strlen(Numstring);
    if((len) && (len < 6))
    {
        for(i = 0; i < len; i++)
        {
            if((Numstring[i] >= '0') && (Numstring[i] <= '9'))
            {
                temp = Numstring[i] - '0';
                for(j = 0; j < (len - 1 - i); j++)
                    temp *= 10;
                Num += temp;
            }
            else
            {
                return -1;
            }
        }
    }
    return (int)Num;
}

void tcp_connect_server_thread( beken_thread_arg_t arg )
{
    int SkFd = -1;
    int len = 0;
    char *buf = NULL;
    OSStatus err = kNoErr;
    struct ip_port_msg *i_pMsg;
    struct sockaddr_in server_addr;

    i_pMsg = (struct ip_port_msg *)arg;
    if(i_pMsg == NULL)
    {
        os_printf( "arg err!\r\n" );
        goto exit;
    }

    SkFd = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = i_pMsg->IP.addr;    //0x0728a8c0 /*192.168.40.7*/
    server_addr.sin_port = htons(i_pMsg->sin_port);   //htons( 12345 );

    if(i_pMsg)
    {
        os_free(i_pMsg);
    }

    err = connect(SkFd, (struct sockaddr *) &server_addr, sizeof(server_addr));
    if(err < 0)
    {
        os_printf( "connect err: %d\r\n", err );
        goto exit;
    }

    buf = (char *) os_malloc( 1024 );
    if(buf == NULL)
    {
        os_printf( "buf == NULL\r\n");
        goto exit;
    }

    while ( 1 )
    {
        len = recv(SkFd, buf, 1024, 0 );

        if ( len <= 0 )
        {
            os_printf( "TCP Client is disconnected, fd: %d\r\n", SkFd );
            goto exit;
        }

        len = send( SkFd, buf, len, 0 );
    }

exit:

    if ( err != kNoErr )
    {
        os_printf( "tcp_connect_server_thread exit with err: %d\r\n", err );
    }

    if ( buf != NULL )
    {
        os_free( buf );
    }

    close( SkFd );
    rtos_delete_thread( NULL );
}

#define TCP_CONNECT_TIMEOUT_S 10
#define TCP_RECV_TIMEOUT_S 10
#define TCP_CLOSE_LINGER_TIME_S 10
#define TCP_CLIENT_SEND_STR "tcp client send string"
#define PROFILE_IDX 1

void do_tcp_client_thread(beken_thread_arg_t arg )
{
	int ret = 0;
	int sock_fd = -1;
	int sock_error = 0;
	socklen_t optlen = 0;
	int sock_nbio = 0;

	fd_set read_fds, write_fds;
	struct timeval t;

	u8 recv_buf[128] = {0};


    OSStatus err = kNoErr;
    struct ip_port_msg *i_pMsg;
    struct sockaddr_in server_addr;
   

	struct sockaddr_in ip4_local_addr = {0};
	

    i_pMsg = (struct ip_port_msg *)arg;
    if(i_pMsg == NULL)
    {
        os_printf( "arg err!\r\n" );
        goto exit;
    }

    ret = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(ret < 0)
	{
	  os_printf("*** socket create fail ***\r\n");
	  goto exit;
	}
	sock_fd = ret;

	ioctl(sock_fd, FIONBIO, &sock_nbio); 

	ip4_local_addr.sin_family = AF_INET;
	ip4_local_addr.sin_port = htons(1234);
	ip4_local_addr.sin_addr.s_addr = INADDR_ANY;

	ret = bind(sock_fd, (struct sockaddr *)&ip4_local_addr, sizeof(ip4_local_addr));
	if(ret < 0)
	{
	  os_printf("*** bind fail ***\r\n");
	  goto exit;
	}

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = i_pMsg->IP.addr;    //0x0728a8c0 /*192.168.40.7*/
    server_addr.sin_port = htons(i_pMsg->sin_port);   //htons( 12345 );


    ret = connect(sock_fd, (struct sockaddr *) &server_addr, sizeof(server_addr));
    if(ret < 0)
    {
        os_printf( "connect err: %d\r\n", err );
        goto exit;
    }

	t.tv_sec = TCP_CONNECT_TIMEOUT_S;
	t.tv_usec = 0;
	
	FD_ZERO(&read_fds);
	FD_ZERO(&write_fds);
	
	FD_SET(sock_fd, &read_fds);
	FD_SET(sock_fd, &write_fds);
	
	ret = select(sock_fd + 1, &read_fds, &write_fds, NULL, &t);
	
	os_printf("select ret: %d\r\n", ret);

	if(ret <= 0)
	{
		os_printf("*** select timeout or error ***\r\n");
		goto exit;
	}
	
	if(!FD_ISSET(sock_fd, &read_fds) && !FD_ISSET(sock_fd, &write_fds)) {
	    os_printf("*** connect fail ***\r\n");
	    goto exit;
	}
	else if(FD_ISSET(sock_fd, &read_fds) && FD_ISSET(sock_fd, &write_fds))
	{
	    optlen = sizeof(sock_error);
	    ret = getsockopt(sock_fd, SOL_SOCKET, SO_ERROR, &sock_error, &optlen);
	    if(ret == 0 && sock_error == 0)
	    {
	       os_printf("connect success\r\n");
	    }
	     else
		{
		  os_printf("*** connect fail, sock_err = %d, errno = %u ***\r\n", sock_error, errno);
		  goto exit;
	    } 
	}
	else if(!FD_ISSET(sock_fd, &read_fds) && FD_ISSET(sock_fd, &write_fds))
	{
	    os_printf("connect success\r\n");
	}
	else if(FD_ISSET(sock_fd, &read_fds) && !FD_ISSET(sock_fd, &write_fds))
	{
	    os_printf("*** connect fail ***\r\n");
	    goto exit;
	}
	else
	{
	    os_printf("*** connect fail ***\r\n");
	    goto exit;
	}
	
	ret = send(sock_fd, (const void*)TCP_CLIENT_SEND_STR, strlen(TCP_CLIENT_SEND_STR), 0);
	
	if(ret < 0)
	{
	   os_printf("*** send fail ***\r\n");
	   goto exit;
	 }
	
_recv_:


	FD_ZERO(&read_fds);
	FD_SET(sock_fd, &read_fds);
	
	ret = select(sock_fd + 1, &read_fds, NULL, NULL, NULL);
	
	os_printf("select ret: %d\r\n", ret);
	
	if(ret <= 0)
	{
		os_printf("*** select timeout or error ***\r\n");
		goto exit;
	}
	if(FD_ISSET(sock_fd, &read_fds))
	{
		ret = recv(sock_fd, recv_buf, sizeof(recv_buf), 0);
	    if(ret > 0)
	    {
		  os_printf("recv data: [%d]%s\r\n", ret, recv_buf);
	    }
	    else if(ret == 0)
		{
			os_printf("*** peer closed ***\r\n");
			goto exit;
		}
		else
		{
			if(!(errno == EINTR || errno == EWOULDBLOCK || errno == EAGAIN))
			{
				os_printf("*** error occurs ***\r\n");
				goto exit;
			}
			else
			{
				os_printf("wait for a while\r\n");
				rtos_delay_milliseconds(20);
				goto _recv_;
			} 
		}
	}

exit:
		if(sock_fd >= 0) {
			struct linger linger = {0};
			
			linger.l_onoff = 1;
			linger.l_linger = TCP_CLOSE_LINGER_TIME_S;
			
			setsockopt(sock_fd, SOL_SOCKET, SO_LINGER, &linger, sizeof(linger));
			setsockopt(sock_fd, IPPROTO_TCP, TCP_KEEPALIVE, &linger.l_linger, sizeof(linger.l_linger));
			close(sock_fd);
		}

    rtos_delete_thread( NULL );
}

/*****************************************************************
please add this function on struct cli_command built_ins[] of wlan_cli.c files.
such as,
static const struct cli_command built_ins[] =
{
...
//example
    {"tcp_client", "tcp_client ip port", tcp_make_connect_server_command},
    .....
 };
******************************************************************/
void tcp_make_connect_server_command(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
    OSStatus err = kNoErr;
    struct ip_port_msg *i_pMsg = NULL;
    int port;
    i_pMsg = (struct ip_port_msg *) os_malloc( sizeof(struct ip_port_msg) );

    if(i_pMsg == NULL)
    {
        os_printf("i_pMsg Failed\r\n");
        return;
    }

    if(!inet_aton(argv[1], &i_pMsg->IP.addr))
    {
        os_printf("inet_aton Failed\r\n");
        goto exit;
    }

    port = num_string2uint16num(argv[2]);
    if(port < 0)
    {
        os_printf("port err:%d\r\n", port);
    }

    i_pMsg->sin_port = port;

    err = rtos_create_thread( NULL,
                              BEKEN_APPLICATION_PRIORITY,
                              "connt_Server",
                              do_tcp_client_thread,
                              0x600,
                              i_pMsg );
    if(kNoErr != err)
    {
        os_printf("rtos_create_thread Failed(%d)\r\n", err);
        goto exit;
    }

    return;

exit:

    if(i_pMsg)
    {
        os_free(i_pMsg);
    }
}

#endif
// eof

