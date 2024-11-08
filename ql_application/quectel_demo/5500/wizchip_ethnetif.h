
#ifndef _WIZCHIP_ETHNETIF_H_
#define _WIZCHIP_ETHNETIF_H_

#include "ip4_addr.h"

typedef struct
{
    uint8_t *mac;
    char *hostname;
    struct
    {
        int iptype;
        ip_addr_t ipaddr;
        ip_addr_t netmask;
        ip_addr_t gw;
        ip_addr_t dns_server[2];
    } ipv4_info;
} wz_ethnetif_ipconfig_t;

typedef enum wz_ethnetif_work_mode {
	WZ_ETHNETIF_NODE = 0,  // 网卡终端模式
	WZ_ETHNETIF_GATEWAY,   // 网卡网关模式
};

extern void wz_ethnetif_set_spi(int port, int cs);
extern void wz_ethnetif_set_eint_pin(int eint_pin);
extern void wz_ethnetif_set_reset_pin(int reset_pin);
extern int wz_ethnetif_register(uint8_t mac[6], char *ip_str, char *mask_str, char *gw_str, enum wz_ethnetif_work_mode mode);
extern int wz_ethnetif_set_addr(char *ip_str, char *mask_str, char *gw_str);
extern int wz_ethnetif_set_dns(char *pri_str, char *sec_str);
extern int wz_ethnetif_set_up(void);
extern int wz_ethnetif_set_down(void);
extern int wz_ethnetif_dhcp(void);
extern void wz_ethnetif_ipconfig(wz_ethnetif_ipconfig_t *ipconfig);
extern int wz_ethnetif_set_defult_network_card(char *ip);
extern void wz_ethnetif_destroy(void);



#endif




