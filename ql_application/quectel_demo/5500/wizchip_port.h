#ifndef _WIZCHIP_PORT_H_
#define _WIZCHIP_PORT_H_

#include <stdint.h>

void wizchip_set_port(int spi_port, int cs_pin, int eint_pin, int reset_pin);
void wizchip_set_recvhandler(int(*recv_data_hdlr)(uint8_t *data, uint32_t len));
void wizchip_send_data(uint8_t *data, uint32_t len);
int wizchip_port_init(uint8_t mac[6], uint8_t ip[4], uint8_t sn[4], uint8_t gw[4]);
void wizchip_port_destroy();
void w5500_ethernet_test(void);

#endif
