#include "ql_include.h"


/***************uart test******************/

ql_sem_t uart_rx_test_sema = NULL;
ql_sem_t uart_tx_test_sema = NULL;

static void uart_rx_test_callback(int uport, void *param)
{
	if(uart_rx_test_sema)
    	ql_rtos_semaphore_release(uart_rx_test_sema);
}
static void uart_tx_test_callback(int uport, void *param)
{
	if(uart_tx_test_sema)
    	ql_rtos_semaphore_release(uart_tx_test_sema);
}

void ql_uart_demo_thread(void *param)
{
	int i,ret = 0,stop_cmd = 1;//stop_cmd；set 1 enable , set 0 disable
	UINT8 *recv_buf,*recv_buf_bak;
	UINT8 *send_buf;
	ql_uart_config_s uart_test_cfg;
	ql_uart_config_s uart_get_cfg;

	ql_rtos_task_sleep_ms(3000);

	recv_buf = os_malloc(UART_TEST_LEN * sizeof(UINT8));
	send_buf = os_malloc(UART_TEST_LEN * sizeof(UINT8));
	if(recv_buf == 0||send_buf == 0)
	{
		ql_uart_log("buf malloc failed\r\n");
		return;
	}
	recv_buf_bak = recv_buf;
	
	os_memset(recv_buf,0,UART_TEST_LEN);
	os_memset(send_buf,0,UART_TEST_LEN);

	uart_test_cfg.baudrate = QL_UART_BAUD_115200;
    uart_test_cfg.data_bit = QL_UART_DATABIT_8;
    uart_test_cfg.parity_bit = QL_UART_PARITY_NONE;
	uart_test_cfg.stop_bit = QL_UART_STOP_1;
    uart_test_cfg.flow_ctrl = QL_FC_NONE;

    ql_uart_set_dcbconfig(UART_TEST_PORT,&uart_test_cfg);

	ret =  ql_uart_open(UART_TEST_PORT);

	ret = ql_rtos_semaphore_create(&uart_tx_test_sema, 1);
	if(ret != kNoErr)
	{
		ql_uart_log("rtos_init_semaphore err:%d\r\n", ret);
		goto uart_test_exit;
	}
	ret = ql_rtos_semaphore_create(&uart_rx_test_sema, 1);
	if(ret != kNoErr)
	{
		ql_uart_log("rtos_init_semaphore err:%d\r\n", ret);
		goto uart_test_exit;
	}
	ret = ql_uart_set_tx_cb(UART_TEST_PORT, uart_tx_test_callback);
	if (ret != QL_UART_SUCCESS)
	{
		ql_uart_log("ql_uart_set_tx_cb failed\r\n");
		goto uart_test_exit;
	}
	ql_uart_set_rx_cb(UART_TEST_PORT, uart_rx_test_callback);
	ql_uart_log("uart tx test,out %d bytes using uart1 \r\n",UART_TEST_LEN);

	ql_uart_set_tx_int(UART_TEST_PORT,stop_cmd);

	for(i=0;i<UART_TEST_LEN;i++)
		send_buf[i] = i;

	ql_uart_write(UART_TEST_PORT,send_buf,20);
	ql_rtos_semaphore_wait(uart_tx_test_sema, BEKEN_NEVER_TIMEOUT);

	ql_uart_log("uart send succ\r\n");
	ql_uart_log("uart rx test,please enter %d bytes using uart1 \r\n",UART_TEST_LEN);

	ql_rtos_semaphore_wait(uart_rx_test_sema, BEKEN_NEVER_TIMEOUT);

	while(1)
	{

	  if(ql_uart_read(UART_TEST_PORT, recv_buf, UART_TEST_LEN) == 0)
	    {
			break;
		}
		ql_rtos_task_sleep_ms(10);
	}

	for(i=0; i<UART_TEST_LEN; i++)
	{
		ql_uart_log("recv_buf[%d] =0x%x\r\n",i,*recv_buf);
		recv_buf++;
	}

	ql_uart_get_dcbconfig(UART_TEST_PORT,&uart_get_cfg);

	ql_uart_log("uart get config:\r\n");
	ql_uart_log("uart baudrate:%d\r\n",uart_get_cfg.baudrate);
	ql_uart_log("uart data_bit:%d\r\n",uart_get_cfg.data_bit);
	ql_uart_log("uart stop_bit:%d\r\n",uart_get_cfg.stop_bit);
	ql_uart_log("uart parity_bit:%d\r\n",uart_get_cfg.parity_bit);
	ql_uart_log("uart flow_ctrl:%d\r\n",uart_get_cfg.flow_ctrl);
	ql_uart_log("\r\n");
	ql_uart_log("uart  test over \r\n");

	ql_uart_close(UART_TEST_PORT);
uart_test_exit:

	if (uart_rx_test_sema)
		ql_rtos_semaphore_delete(uart_rx_test_sema);
	if (uart_tx_test_sema)
		ql_rtos_semaphore_delete(uart_tx_test_sema);

	os_free(recv_buf_bak);
	os_free(send_buf);

	while(1)
	{
	  ql_rtos_task_sleep_ms(1000);
	}

}

ql_task_t	uart_test_thread_handle = NULL;
void ql_uart_demo_thread_creat(void)
{
    int ret;
	ret = ql_rtos_task_create(&uart_test_thread_handle,
		               (unsigned short)4096,
					   THD_EXTENDED_APP_PRIORITY,
					   "uart_test",
					   ql_uart_demo_thread,
					   0);


	  if (ret != kNoErr) {
		os_printf("Error: Failed to create uart test thread: %d\r\n",ret);
		goto init_err;
	  }

	  return;

	  init_err:
	   if( uart_test_thread_handle != NULL ) {
		   ql_rtos_task_delete(uart_test_thread_handle);
	   }

}
