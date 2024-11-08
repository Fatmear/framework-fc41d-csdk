#ifndef _QL_UART_H
#define _QL_UART_H

#include "uart_pub.h"

#define UART_TEST_DEBUG
#ifdef UART_TEST_DEBUG
#define ql_uart_log               os_printf

#else
#define ql_uart_log               os_null_printf
#endif


#define UART_TEST_LEN   20
#define UART_TEST_PORT     QL_UART_PORT_1

typedef void (*ql_uart_callback)(int uport, void *param);

typedef enum
{
	 QL_UART_SUCCESS = 0,
	 QL_UART_EXECUTE_ERR,
	 QL_UART_MEM_ADDR_NULL_ERR,
	 QL_UART_INVALID_PARAM_ERR,
	 QL_UART_NOT_OPEN_ERR,
} ql_uart_errcode_e;


typedef enum 
{
	 QL_UART_PORT_1 =0,
	 QL_UART_PORT_2,		 

}ql_uart_port_number_e;

typedef enum
{
	QL_FC_NONE = 0, 
	QL_FC_HW_CTS,
	QL_FC_HW_RTS,
	QL_FC_HW_RTS_CTS,
}ql_uart_flowctrl_e;

typedef enum
{
	 QL_UART_BAUD_1200 = 1200,
	 QL_UART_BAUD_2400 = 2400,
	 QL_UART_BAUD_4800 = 4800,
	 QL_UART_BAUD_9600 = 9600,
	 QL_UART_BAUD_14400 = 14400,
	 QL_UART_BAUD_19200 = 19200,
	 QL_UART_BAUD_28800 = 28800,
	 QL_UART_BAUD_33600 = 33600,
	 QL_UART_BAUD_38400 = 38400,
	 QL_UART_BAUD_57600 = 57600,
	 QL_UART_BAUD_115200 = 115200,
	 QL_UART_BAUD_230400 = 230400,
	 QL_UART_BAUD_460800 = 460800,
	 QL_UART_BAUD_921600 = 921600,
	 QL_UART_BAUD_1000000 = 1000000,
	 QL_UART_BAUD_2000000 = 2000000,
}ql_uart_baud_e;


typedef enum
{
	QL_UART_DATABIT_5 = 0,
	QL_UART_DATABIT_6 ,
	QL_UART_DATABIT_7 ,
	QL_UART_DATABIT_8 ,
}ql_uart_databit_e;


typedef enum
{
	QL_UART_STOP_1 = 0, 
	QL_UART_STOP_2 ,
}ql_uart_stopbit_e;

typedef enum
{
	QL_UART_PARITY_NONE = 0,
	QL_UART_PARITY_ODD,
	QL_UART_PARITY_EVEN,
}ql_uart_parityit_e;

typedef struct
{
	ql_uart_baud_e baudrate;
	ql_uart_databit_e data_bit;
	ql_uart_stopbit_e stop_bit;
	ql_uart_parityit_e parity_bit;
	ql_uart_flowctrl_e flow_ctrl;
}ql_uart_config_s;


ql_uart_errcode_e ql_uart_set_dcbconfig(ql_uart_port_number_e port, ql_uart_config_s *dcb);
ql_uart_errcode_e ql_uart_get_dcbconfig(ql_uart_port_number_e port, ql_uart_config_s *dcb);
ql_uart_errcode_e ql_uart_open(ql_uart_port_number_e port);
ql_uart_errcode_e ql_uart_close(ql_uart_port_number_e port);
int ql_uart_write(ql_uart_port_number_e port, unsigned char *data, unsigned int data_len);
int ql_uart_read(ql_uart_port_number_e port,unsigned  char *data, unsigned int data_len);
ql_uart_errcode_e ql_uart_set_rx_cb(ql_uart_port_number_e port, ql_uart_callback uart_cb);
ql_uart_errcode_e ql_uart_set_tx_int(ql_uart_port_number_e port, unsigned int set);
ql_uart_errcode_e ql_uart_set_tx_cb(ql_uart_port_number_e port, ql_uart_callback uart_cb);
int ql_uart_recv(ql_uart_port_number_e port, unsigned char *data, unsigned int data_len);

void ql_uart_demo_thread(void *param);
void ql_uart_demo_thread_creat(void);

#endif 

