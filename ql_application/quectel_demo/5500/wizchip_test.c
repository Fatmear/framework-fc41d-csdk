
#include "includes.h"
#include "ql_include.h"

#include "sockets.h"
#include "netdb.h"
#include "wizchip_ethnetif.h"
#include "ethernet.h"
#include "etharp.h"

#define wz_log	bk_printf

#define USE_DHCP  0

#define W5500_MAC  {0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC}
#define W5500_MAC_1 {0x00, 0x0e, 0xc6, 0x25, 0x8a, 0x9e}

#if USE_DHCP
#define W5500_IP   "0.0.0.0"
#define W5500_MASK "0.0.0.0"
#define W5500_GW   "0.0.0.0"

#else
#define W5500_IP   "192.168.31.115"
//#define W5500_MASK NULL
#define W5500_MASK "255.255.255.0"

#define W5500_GW   "192.168.31.1"
#endif

#define TCP_SERVER_DOMAIN "192.168.31.221"
#define TCP_SERVER_PORT 8000
#define TCP_CONNECT_TIMEOUT_S 10
#define TCP_RECV_TIMEOUT_S 3
#define TCP_CLOSE_LINGER_TIME_S 10
#define TCP_CLIENT_SEND_STR "GET / HTTP/1.1\r\nHost: www.baidu.com:80\r\n\r\n"


static err_t
etharp_raw_2(struct netif *netif, const struct eth_addr *ethsrc_addr,
           const struct eth_addr *ethdst_addr,
           const struct eth_addr *hwsrc_addr, const ip4_addr_t *ipsrc_addr,
           const struct eth_addr *hwdst_addr, const ip4_addr_t *ipdst_addr,
           const u16_t opcode)
{
  struct pbuf *p;
  err_t result = ERR_OK;
  struct etharp_hdr *hdr;

  LWIP_ASSERT("netif != NULL", netif != NULL);

  /* allocate a pbuf for the outgoing ARP request packet */
  p = pbuf_alloc(PBUF_LINK, SIZEOF_ETHARP_HDR, PBUF_RAM);
  /* could allocate a pbuf for an ARP request? */
  if (p == NULL) {
    LWIP_DEBUGF(ETHARP_DEBUG | LWIP_DBG_TRACE | LWIP_DBG_LEVEL_SERIOUS,
      ("etharp_raw: could not allocate pbuf for ARP request.\n"));
    ETHARP_STATS_INC(etharp.memerr);
    return ERR_MEM;
  }
  LWIP_ASSERT("check that first pbuf can hold struct etharp_hdr",
              (p->len >= SIZEOF_ETHARP_HDR));

  hdr = (struct etharp_hdr *)p->payload;
  LWIP_DEBUGF(ETHARP_DEBUG | LWIP_DBG_TRACE, ("etharp_raw: sending raw ARP packet.\n"));
  hdr->opcode = lwip_htons(opcode);

  LWIP_ASSERT("netif->hwaddr_len must be the same as ETH_HWADDR_LEN for etharp!",
              (netif->hwaddr_len == ETH_HWADDR_LEN));

  /* Write the ARP MAC-Addresses */
  ETHADDR16_COPY(&hdr->shwaddr, hwsrc_addr);
  ETHADDR16_COPY(&hdr->dhwaddr, hwdst_addr);
  /* Copy struct ip4_addr2 to aligned ip4_addr, to support compilers without
   * structure packing. */
  IPADDR2_COPY(&hdr->sipaddr, ipsrc_addr);
  IPADDR2_COPY(&hdr->dipaddr, ipdst_addr);

  hdr->hwtype = PP_HTONS(HWTYPE_ETHERNET);
  hdr->proto = PP_HTONS(ETHTYPE_IP);
  /* set hwlen and protolen */
  hdr->hwlen = ETH_HWADDR_LEN;
  hdr->protolen = sizeof(ip4_addr_t);

  /* send ARP query */
#if LWIP_AUTOIP
  /* If we are using Link-Local, all ARP packets that contain a Link-Local
   * 'sender IP address' MUST be sent using link-layer broadcast instead of
   * link-layer unicast. (See RFC3927 Section 2.5, last paragraph) */
  if(ip4_addr_islinklocal(ipsrc_addr)) {
    ethernet_output(netif, p, ethsrc_addr, &ethbroadcast, ETHTYPE_ARP);
  } else
#endif /* LWIP_AUTOIP */
  {
    ethernet_output(netif, p, ethsrc_addr, ethdst_addr, ETHTYPE_ARP);
  }

  ETHARP_STATS_INC(etharp.xmit);
  /* free ARP query packet */
  pbuf_free(p);
  p = NULL;
  /* could not allocate pbuf for ARP request */

  return result;
}

ql_task_t wz_socket_test_ref = NULL;

void wz_socket_test(void *argv) {
	int				ret			= 0;
    int				sock_nbio	= 1;
	int				sock_fd     = -1;
	int				sock_error  = 0;
	socklen_t		optlen = 0;
	fd_set 			read_fds, write_fds;
	struct timeval	t;
    struct addrinfo		* res, hints;
	struct sockaddr_in	* ip4_svr_addr;
    int dns_success = 0;
	unsigned char recv_buf[1024] = {0};
	struct netif * netif = NULL;
	char ip[16];

	
	ip_addr_t ip_t , mask , gw ;

    ql_rtos_task_sleep_s(1);

    wz_log("wz_socket_test start\n");

    uint8_t mac[6] = W5500_MAC;

    if(wz_ethnetif_register(mac, W5500_IP, W5500_MASK, W5500_GW, WZ_ETHNETIF_NODE))
    {
        wz_log("W5500 NIC FAILLLLLLL");
        goto exit;
    }
    else
    {
        wz_log("W5500 NIC OKKKKKKKKK \n");
        wz_ethnetif_set_dns("8.8.8.8", "114.114.114.114");
    #if USE_DHCP
        wz_ethnetif_dhcp();
    #else
        wz_ethnetif_set_up();
    #endif
    }

	netif = netif_find("wz");
	if(netif)
	{
		os_strcpy(ip, ipaddr_ntoa(&netif->ip_addr));
		bk_printf("********find netif ip:%s\n", ip );
	}

	
#if 0	
	while(1)
	{
		ql_rtos_task_sleep_s(1);
	}
#endif

#if 1
	memset(&hints, 0, sizeof(struct addrinfo));
	//return;
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;

	if(getaddrinfo(TCP_SERVER_DOMAIN, NULL, &hints, &res) != 0)
	{
		printf("*** DNS fail ***\r\n");
		goto exit;
	}

	dns_success = 1;

	ret = socket(AF_INET, SOCK_STREAM, 0);
	if(ret < 0)
	{
		wz_log("*** socket create fail ***\r\n");
		goto exit;
	}

	sock_fd = ret;

    ioctlsocket(sock_fd, FIONBIO, &sock_nbio);

	ip4_svr_addr = (struct sockaddr_in *)res->ai_addr;
	ip4_svr_addr->sin_port = htons(TCP_SERVER_PORT);

	ret = connect(sock_fd, (struct sockaddr *)ip4_svr_addr, sizeof(struct sockaddr));

	wz_log("connect ret: %d, errno: %u\r\n", ret, errno);

	if(ret == -1 && errno != EINPROGRESS)
	{
		wz_log("*** connect fail ***\r\n");
		goto exit;
	}

	t.tv_sec = TCP_CONNECT_TIMEOUT_S;
	t.tv_usec = 0;

	FD_ZERO(&read_fds);
	FD_ZERO(&write_fds);

	FD_SET(sock_fd, &read_fds);
	FD_SET(sock_fd, &write_fds);

	ret = select(sock_fd + 1, &read_fds, &write_fds, NULL, &t);

	wz_log("select ret: %d\r\n", ret);

	if(ret <= 0)
	{
		wz_log("*** select timeout or error ***\r\n");
		//return;
		goto exit;
	}

	if(!FD_ISSET(sock_fd, &read_fds) && !FD_ISSET(sock_fd, &write_fds))
	{
		wz_log("*** connect fail ***\r\n");
		goto exit;
	}
	else if(FD_ISSET(sock_fd, &read_fds) && FD_ISSET(sock_fd, &write_fds))
	{
		optlen = sizeof(sock_error);
		ret = getsockopt(sock_fd, SOL_SOCKET, SO_ERROR, &sock_error, &optlen);
		if(ret == 0 && sock_error == 0)
		{
			wz_log("connect success\r\n");
		}
		else
		{
			wz_log("*** connect fail, sock_err = %d, errno = %u ***\r\n", sock_error, errno);
			goto exit;
		}
	}
	else if(!FD_ISSET(sock_fd, &read_fds) && FD_ISSET(sock_fd, &write_fds))
	{
		wz_log("connect success\r\n");
	}
	else if(FD_ISSET(sock_fd, &read_fds) && !FD_ISSET(sock_fd, &write_fds))
	{
		wz_log("*** connect fail ***\r\n");
		goto exit;
	}
	else
	{
		wz_log("*** connect fail ***\r\n");
		goto exit;
	}

	ret = send(sock_fd, (const void*)TCP_CLIENT_SEND_STR, strlen(TCP_CLIENT_SEND_STR), 0);
	if(ret < 0)
	{
		wz_log("*** send fail ***\r\n");
		goto exit;
	}

_recv_:

	//t.tv_sec = TCP_RECV_TIMEOUT_S;
	t.tv_sec = 30;
	t.tv_usec = 0;

	FD_ZERO(&read_fds);
	FD_SET(sock_fd, &read_fds);

	ret = select(sock_fd + 1, &read_fds, NULL, NULL, &t);

	wz_log("select ret: %d\r\n", ret);

	if(ret <= 0)
	{
		wz_log("*** select timeout or error ***\r\n");
		//return;
		goto exit;
	}

	if(FD_ISSET(sock_fd, &read_fds))
	{
		ret = recv(sock_fd, recv_buf, sizeof(recv_buf), 0);
		if(ret > 0)
		{
			wz_log("recv data: [%d]%s\r\n", ret, recv_buf);
            goto _recv_;
		}
		else if(ret == 0)
		{
			wz_log("*** peer closed ***\r\n");
			goto exit;
		}
		else
		{
			if(!(errno == EINTR || errno == EWOULDBLOCK || errno == EAGAIN))
			{
				wz_log("*** error occurs ***\r\n");
				goto exit;
			}
			else
			{
				wz_log("wait for a while\r\n");
				goto _recv_;
			}
		}
	}

exit:
    if(dns_success) freeaddrinfo(res);
	
	if(sock_fd >= 0)
	{
		struct linger linger = {0};

		linger.l_onoff = 1;
		linger.l_linger = TCP_CLOSE_LINGER_TIME_S;

		setsockopt(sock_fd, SOL_SOCKET, SO_LINGER, &linger, sizeof(linger));
      //  setsockopt(sock_fd, IPPROTO_TCP, TCP_CLOSE_TIMEROUT, &linger.l_linger, sizeof(linger.l_linger));
		
		close(sock_fd);
	}

    ql_rtos_task_delete(wz_socket_test_ref);
#endif
}

void w5500_ethernet_test(void) {

	os_printf("***********w5500 demo start \n");
	ql_rtos_task_sleep_ms(10);
    ql_rtos_task_create(&wz_socket_test_ref, 10240, THD_EXTENDED_APP_PRIORITY, "wz_socket_test", wz_socket_test, NULL);
}
