#include <stdint.h>
#include "ip4_addr.h"
#include "err.h"
#include "pbuf.h"
#include "netif.h"
#include "etharp.h"
#include "ethip6.h"
#include "netifapi.h"
#include "def.h"
#include "mem.h"
#include "opt.h"
#include "stats.h"
#include "sockets.h"
#include "wizchip_conf.h"
#include "wizchip_port.h"
#include "wizchip_ethnetif.h"
#include "lwip/snmp.h"
#include "ql_include.h"

//#include "quec_debug.h"

//#define wz_log(msg, ...)	os_printf("wz_drv", msg, ##__VA_ARGS__)
#define wz_log  bk_printf

static uint8_t g_mac[6] = {0};


err_t ethernet_netif_input(struct pbuf *p, struct netif *netif);
struct pbuf * ethernet_dl_pbuf_reshape(struct netif *netif, struct pbuf *p);
//extern GlobalParameter_Dialer2Duster gDialer2UIPara;
uint8_t lwip_init_flag;

static struct netif wz_netif = {0};
static int wz_netif_exist = 0;
//static statisticsData  *wz_netif_statistics  = &(gDialer2UIPara.USB);

static struct pbuf *wz_netif_copy_data(u8_t *data, u32_t len)
{
    struct pbuf *p = NULL;

   // LWIP_DIAG(DIAG_ON_DEBUG, wz_ethnetif, "lwipdbg: wz_netif_copy_data, len: %u", len);
    
    p = pbuf_alloc(PBUF_RAW, len, PBUF_RAM);

    if (p != NULL) {
        MEMCPY(p->payload, data, p->len);
        LINK_STATS_INC(link.recv);
    } else {
        LINK_STATS_INC(link.memerr);
        LINK_STATS_INC(link.drop);
     //   LWIP_DIAG(DIAG_ON_DEBUG, wz_ethnetif, "lwiperr: %s, pbuf_alloc failed!", __FUNCTION__);
    }

    return p;
}
int wz_netif_low_input(uint8_t *data, uint32_t len) {
    struct pbuf *p       = NULL;
    struct netif * netif = &wz_netif;
    int ret              = ERR_OK;

    if ((data == NULL )
       || (len == 0)) {
       // LWIP_DIAG(DIAG_ON_DEBUG, wz_ethnetif, "lwiperr: %s, param check error!", __FUNCTION__);
       bk_printf("lwiperr: %s, param check error!", __FUNCTION__);
        return ERR_MEM;
    }
#if 0
    ret = netif_ready(netif);
    if (ret != ERR_OK) {
       // LWIP_DIAG(DIAG_ON_DEBUG, wz_ethnetif, "lwiperr: %s, netif not ready", __FUNCTION__);
        goto _get_packet_in_ret;
    }


    /*do pre check, keep it*/
    ret = tcpip_input_eth_check(data, len, netif);
    if (ERR_OK != ret) {
        goto _get_packet_in_ret;
    }
#endif
    /*copy data to pbuf, then free data outside */
    p = wz_netif_copy_data(data, len);
	if (p) {
		ret = netif->input(p, netif);
		if( ret != ERR_OK ) {
			pbuf_free(p);
		}
	} else {
		ret = ERR_BUF;
	}

_get_packet_in_ret:

    if (ERR_OK == ret) {
        //netif_set_rx_info(wz_netif_statistics, len);
        return ret;
    } else {
        //netif_set_rx_error_info(wz_netif_statistics, len);
        return ret;
    }

    return ret;
}


static err_t wz_netif_low_output(struct netif *netif, struct pbuf *p) {
    struct pbuf *q = NULL;
    u8_t *buf      = NULL;
    u8_t *payload  = NULL;
    u16_t len  = 0;
    u16_t size = 0;
    err_t ret  = ERR_OK;

    //LWIP_DIAG(DIAG_ON_DEBUG, wz_ethnetif, "lwipdbg: wz_netif_low_output enter");
    if (p) {
        size = p->tot_len;
    }

    if (NULL == netif || p == NULL)  {
       // LWIP_DIAG(DIAG_ON_DEBUG, wz_ethnetif, "lwiperr: %s, netif is null", __FUNCTION__);
       bk_printf("lyy debug %s %d \n", __FUNCTION__, __LINE__);
        ret = ERR_IF;
        goto _low_output_ret;
    }

    if (!netif_is_up(netif)) {
     //   LWIP_DIAG(DIAG_ON_DEBUG, wz_ethnetif, "lwiperr: %s, netif is down after retry", __FUNCTION__);
      bk_printf("lyy debug %s %d \n", __FUNCTION__, __LINE__);
        ret = ERR_IF;
        goto _low_output_ret;
    }

   // LWIP_DIAG(DIAG_ON_DEBUG, wz_ethnetif, "lwip: netif_%s low output,pbuf=%lx,payload=%lx,pkt_mac=%lx,tot_len=%d,len=%d,type=%d",
//                                               netif->hostname, p, p->payload, p->pkt_mac, p->tot_len, p->len, p->type);

    /* reshape pbuf for pc output, keep it here. */
//    p = ethernet_dl_pbuf_reshape(netif, p);
//    if (NULL == p) {
//        ret = ERR_ARG;
//        goto _low_output_ret;        
//    }

    /*now p->payload pointer to ethernet header pointer, need do copy, call low send api to send data*/
    len = p->tot_len + 0; //RNDIS_HEADER_SIZE just set length for rndis header size, need modify by customer
    buf = mem_malloc(len); //mem_free
    if (buf == NULL) {
        ret = ERR_MEM;
       // LWIP_DIAG(DIAG_ON_DEBUG, wz_ethnetif, "lwiperr: lwip -> usb buf null");
        goto _low_output_ret;
    }

    payload = buf + 0;
    for (q = p; q != NULL; q = q->next) {
        MEMCPY(payload, q->payload, q->len);
        payload += q->len;
    }

    /*linklayer send packet functon, customer define*/
    wizchip_send_data(buf, len);

    mem_free(buf);
    
    LINK_STATS_INC(link.xmit);

_low_output_ret:
    if (ret != ERR_OK) {
        //netif_set_tx_error_info(wz_netif_statistics, size);
        return ret;
    } else {
        //netif_set_tx_info(wz_netif_statistics, size);
        return ret;
    }

    //LWIP_DIAG(DIAG_ON_DEBUG, wz_ethnetif, "lwipdbg: wz_netif_low_output exit");
    
    return ret;
}

static err_t wz_netif_init(struct netif *netif) {
//    LWIP_ASSERT((netif != NULL));

#if LWIP_NETIF_HOSTNAME
    /* Initialize interface hostname */
    netif->hostname = (char *)malloc(sizeof(char)*64);;
	memset(netif->hostname, 0x00, sizeof(sizeof(char)*64));
	strcpy(netif->hostname, "w5500");
#endif /* LWIP_NETIF_HOSTNAME */

    /*
    * Initialize the snmp variables and counters inside the struct netif.
    * The last argument should be replaced with your link speed, in units
    * of bits per second.
    */
    NETIF_INIT_SNMP(netif, snmp_ifType_ethernet_csmacd, 100 * 1024 * 1024);

    netif->name[0] = 'w';
    netif->name[1] = 'z';
    /* We directly use etharp_output() here to save a function call.
    * You can instead declare your own function an call etharp_output()
    * from it if you have to do some checks before sending (e.g. if link
    * is available...) */
    netif->output     = etharp_output; //no modiify
    netif->linkoutput = wz_netif_low_output;
	// warn: not support ipv6 
//	netif->output_ip6 = ethip6_output; //no modiify

    /* initialize the hardware */
    netif->hwaddr_len = ETHARP_HWADDR_LEN;
    netif->hwaddr[0] = g_mac[0];
    netif->hwaddr[1] = g_mac[1];
    netif->hwaddr[2] = g_mac[2];
    netif->hwaddr[3] = g_mac[3];
    netif->hwaddr[4] = g_mac[4];
    netif->hwaddr[5] = g_mac[5];

    netif->mtu = 1500;
  //  netif->if_app_pkts = 0;
   // netif->if_in_pkts  = 0;
  //  netif->if_out_pkts = 0;
  //  netif->if_in_discards  = 0;
  //  netif->if_out_discards = 0;

 //   netif_set_ip6_linklocal(netif, 0, htonl(0x1234));

    /* don't set NETIF_FLAG_ETHARP if this device is not an ethernet one */
    netif->flags = NETIF_FLAG_BROADCAST |
                   NETIF_FLAG_ETHARP    |
                   NETIF_FLAG_LINK_UP   |
                   NETIF_FLAG_ETHERNET;
    return ERR_OK;
}

void wz_ethnetif_set_spi(int port, int cs)
{
    wizchip_set_port(port, cs, -1, -1);
}

void wz_ethnetif_set_eint_pin(int eint_pin)
{
    wizchip_set_port(-1, -1, eint_pin, -1);
}

void wz_ethnetif_set_reset_pin(int reset_pin)
{
    wizchip_set_port(-1, -1, -1, reset_pin);
}

static struct netif *get_lte_netif()
{
}

static int check_ip_format(char *ip)
{
	if( ip == NULL ) return 1;
	int i = 0;
	int cnt = 0;
	// check . count
	for(i = 0; i < strlen(ip); i++) {
		if( ip[i] == '.' ) {
			cnt++;
		}
	}
	if( cnt != 3 ) {
		wz_log("invalid . count = %d\n", cnt);
		return -1;
	}
	u32_t tmp_ip = inet_addr(ip);
	if( tmp_ip == 0 || tmp_ip == IPADDR_NONE ) {
		wz_log("invalid ip format\n");
		return 1;
	}
	return 0;
}





int wz_ethnetif_register(uint8_t mac[6], char *ip_str, char *mask_str, char *gw_str, enum wz_ethnetif_work_mode mode) {
    ip_addr_t ip , mask , gw ;

	memset(&ip, 0, sizeof(ip_addr_t) );
	memset(&mask, 0, sizeof(ip_addr_t));
	memset(&gw, 0, sizeof(ip_addr_t));

	bk_printf("lyy debug %s %d \n", __FUNCTION__, __LINE__);
    if(!ip_str) ip_str = "192.168.1.100";
    else if(!strlen(ip_str)) ip_str = "0.0.0.0";
	else {
	//	if( check_ip_format(ip_str) ) return -1;
	}

    if(!mask_str) mask_str = "255.255.255.0";
    else if(!strlen(mask_str)) mask_str = "0.0.0.0";
	else {
		//if( check_ip_format(mask_str) ) return -1;
	}

    if(!strlen(gw_str)) gw_str = "0.0.0.0";
	if( mode != WZ_ETHNETIF_NODE && mode != WZ_ETHNETIF_GATEWAY ) return -1;
	bk_printf("lyy debug %s %d \n", __FUNCTION__, __LINE__);

#if 0
    while(!lwip_init_flag)
        ql_rtos_task_sleep_ms(10);
#endif

    memcpy(g_mac, mac, 6);
	inet_pton(AF_INET, ip_str, &ip.addr);
	bk_printf("lyy debug %s %d \n", __FUNCTION__, __LINE__);
	//g_server_ip = ntohl(ip.addr);

 //   LWIP_DIAG(DIAG_ON_DEBUG, wz_ethnetif, "lwipinfo: %s, set ipaddr 0x08X", __FUNCTION__, ip.addr);

    inet_pton(AF_INET, mask_str, &mask.addr);

    if(gw_str) {
		//if( check_ip_format(gw_str) ) return -1;
        inet_pton(AF_INET, gw_str, &gw.addr);
    } else {
        gw.addr = ip.addr & 0x00FFFFFF | 0x01000000;
    }
	bk_printf("lyy debug %s %d \n", __FUNCTION__, __LINE__);
    wizchip_set_recvhandler(wz_netif_low_input);
    if(wizchip_port_init(mac, (uint8_t*)&ip.addr, (uint8_t*)&mask.addr, (uint8_t*)&gw.addr))
        return -1;

	bk_printf("lyy debug %s %d \n", __FUNCTION__, __LINE__);
    if(netifapi_netif_add(&wz_netif, &ip, &mask, &gw, NULL, wz_netif_init, tcpip_input)) {
		wz_log("netifapi_netif_add failed\n");
		wizchip_port_destroy();
		return -1;
    }
// add by lyy
#if 0
	if( mode == WZ_ETHNETIF_GATEWAY) {
		wz_netif.flags |= NETIF_FLAG_INTER;
	} else {
		wz_netif.flags |= NETIF_FLAG_EXTER;
		netif_set_default(&wz_netif);
	}
#endif 

    ip_addr_t pri, sec;
    inet_pton(AF_INET, "8.8.8.8", &pri.addr);
    inet_pton(AF_INET, "114.114.114.114", &sec.addr);
  //  netif_set_dns(&wz_netif, &pri, &sec);
    wz_netif_exist = 1;
    //if(netifapi_netif_set_down(&wz_netif)) return -1;
    return 0;
}

int wz_ethnetif_set_addr(char *ip_str, char *mask_str, char *gw_str)
{
    ip_addr_t ip, mask, gw;

    if(!ip_str) ip_str = "192.168.1.100";
    else if(!strlen(ip_str)) ip_str = "0.0.0.0";
	else {
		if( check_ip_format(ip_str) ) return -1;
	}

    if(!mask_str) mask_str = "255.255.255.0";
    else if(!strlen(mask_str)) mask_str = "0.0.0.0";
	else {
		if( check_ip_format(mask_str) ) return -1;
	}

    if(!strlen(gw_str)) gw_str = "0.0.0.0";

    inet_pton(AF_INET, ip_str, &ip.addr);

    inet_pton(AF_INET, mask_str, &mask.addr);

    if(gw_str) {
		if( check_ip_format(gw_str) ) return -1;
        inet_pton(AF_INET, gw_str, &gw.addr);
    } else {
        gw.addr = ip.addr & 0x00FFFFFF | 0x01000000;
    }

    setGAR((uint8_t*)&gw.addr);
    setSUBR((uint8_t*)&mask.addr);
    setSIPR((uint8_t*)&ip.addr);

	dhcp_stop(&wz_netif);

    return netifapi_netif_set_addr(&wz_netif, &ip, &mask, &gw);
}

int wz_ethnetif_set_dns(char *pri_str, char *sec_str)
{
    ip_addr_t pri, sec;

    if(!pri_str) pri_str = "8.8.8.8";
    else if(!strlen(pri_str)) pri_str = "0.0.0.0";
	else {
		if( check_ip_format(pri_str) ) return -1;
	}

    if(!sec_str) sec_str = "114.114.114.114";
    else if(!strlen(sec_str)) sec_str = "0.0.0.0";
	else {
		if( check_ip_format(sec_str) ) return -1;
	}

    inet_pton(AF_INET, pri_str, &pri.addr);
    inet_pton(AF_INET, sec_str, &sec.addr);
//    netif_set_dns(&wz_netif, &pri, &sec);
	return 0;
}

int wz_ethnetif_set_up(void)
{
    return netifapi_netif_set_up(&wz_netif);
}

int wz_ethnetif_set_down(void)
{
    return netifapi_netif_set_down(&wz_netif);
}

int wz_ethnetif_dhcp(void)
{
    netifapi_netif_set_up(&wz_netif);

    //LWIP_DIAG(DIAG_ON_DEBUG, wz_ethnetif, "lwipinfo: %s, start DHCP ...", __FUNCTION__);
    int ret = 0;
	int try = 0;
	dhcp_stop(&wz_netif);
	ret = dhcp_start(&wz_netif);
	if( ret != ERR_OK ) return -1;

	struct dhcp *dhcp = netif_dhcp_data(&wz_netif);
	wz_log("lwipinfo:  DHCP OK\n", __FUNCTION__);
			   wz_log("lwipinfo:  get ip: 0x%08X\n",  dhcp->offered_ip_addr);
			   wz_log("lwipinfo:  get mask: 0x%08X\n", dhcp->offered_sn_mask.addr);
			   wz_log("lwipinfo:  get gw: 0x%08X\n",  dhcp->offered_gw_addr.addr);
    while(1)
    {
		try++;
    	wz_log("wz_ethnetif_dhcp tries=%d, try=%d, ret = %d\n", dhcp->tries, try, ret);
        if(dhcp->offered_ip_addr.addr)
        {
           
           // g_server_ip = dhcp->offered_ip_addr.addr;
            if(dhcp->offered_sn_mask.addr == 0) dhcp->offered_sn_mask.addr = 0x00FFFFFF;
            if(dhcp->offered_gw_addr.addr == 0) dhcp->offered_gw_addr.addr = dhcp->offered_ip_addr.addr & 0x00FFFFFF | 0x01000000;
            netifapi_netif_set_addr(&wz_netif, &dhcp->offered_ip_addr, &dhcp->offered_sn_mask, &dhcp->offered_gw_addr);
			//uint8_t* value_gw = (uint8_t*)dhcp->offered_gw_addr;
			setGAR((uint8_t*)(&dhcp->offered_gw_addr));
			//uint8_t* value_sn_mask = (uint8_t*)dhcp->offered_gw_addr;
			setSUBR((uint8_t*)(&dhcp->offered_sn_mask));
            setSIPR((uint8_t*)(&dhcp->offered_ip_addr));
			wz_log("lwipinfo:  get ip: 0x%08X\n",  dhcp->offered_ip_addr);
		    wz_log("lwipinfo:  get mask: 0x%08X\n", dhcp->offered_sn_mask.addr);
			wz_log("lwipinfo:  get gw: 0x%08X\n",  dhcp->offered_gw_addr.addr);

            return 0;
        }
        if(try > 30)
        {
            break;
        }
        ql_rtos_task_sleep_ms(500);
    }
	return -1;
}

void wz_ethnetif_ipconfig(wz_ethnetif_ipconfig_t *ipconfig)
{
    if(ipconfig)
    {
        ipconfig->mac = g_mac;
        ipconfig->hostname = wz_netif.hostname;
        ipconfig->ipv4_info.iptype = 4;
        ipconfig->ipv4_info.ipaddr = wz_netif.ip_addr;
        ipconfig->ipv4_info.netmask = wz_netif.netmask;
        ipconfig->ipv4_info.gw = wz_netif.gw;
      //  ipconfig->ipv4_info.dns_server[0] = wz_netif.dns_server[0];
       // ipconfig->ipv4_info.dns_server[1] = wz_netif.dns_server[1];
    }
}

int wz_ethnetif_set_defult_network_card(char *ip)
{
	if( ip == NULL ) return -1;
    struct netif *temp = NULL;
	ip_addr_t ip_addr;
	inet_pton(AF_INET, ip, &ip_addr.addr);
	for(temp = netif_list; temp != NULL; temp = temp->next) {
		if( ip_addr.addr == temp->ip_addr.addr ) {
			netif_set_default(temp);
			return 0;
		}
	}
	return -1;
}

void wz_ethnetif_destroy(void)
{
	if( wz_netif_exist != 0 ) {
		dhcp_stop(&wz_netif);
		wizchip_port_destroy();
		netif_remove(&wz_netif);
		wz_netif_exist = 0;
	}
}



