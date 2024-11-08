#include "ql_include.h"




/***************spi test******************/

void ql_spi_demo_thread(void *param)
{
	int i,ret = 0;
	ql_spi_messag_s spi_test_mess;
	ql_spi_config_s spi_test_cfg;
    UINT8 *buf;
	
	ql_rtos_task_sleep_ms(3000);
	buf = os_malloc(SPI_TEST_LEN * sizeof(UINT8));
	if(buf == 0)
	{
		ql_spi_log("buf malloc failed\r\n");
		return;
	}
	os_memset(buf,0,SPI_TEST_LEN);

	for(i=0;i<SPI_TEST_LEN;i++)
		buf[i] = i;
	spi_test_cfg.cpha = QL_SPI_CPHA_1Edge;
	spi_test_cfg.cpol = QL_SPI_CPOL_LOW;
	spi_test_cfg.masterorslave = QL_SPI_MASTER;
	spi_test_cfg.spiclk = spi_test_speed;
	spi_test_cfg.transmode = QL_SPI_MSB;

	spi_test_mess.recv_buf = NULL;
	spi_test_mess.recv_len = 0;
	spi_test_mess.send_buf = buf;
	spi_test_mess.send_len = SPI_TEST_LEN;
	ret = ql_spi_init(spi_test_cfg,&spi_test_mess);
	
	if (ret != QL_SPI_SUCCESS)
	{
		ql_spi_log("spi init failed\r\n");
		goto spi_test_exit;
	}

	ql_spi_log("spi tx test,out %d bytes \r\n",SPI_TEST_LEN);
	ql_spi_transfer(&spi_test_mess);
	ql_spi_log("spi test over \r\n");

spi_test_exit:
	os_free(buf);	
	while(1)
	{
	  ql_rtos_task_sleep_ms(1000);
	}
	
}

ql_task_t	spi_test_thread_handle = NULL;
void ql_spi_demo_thread_creat(void)
{
    int ret;
	ret = ql_rtos_task_create(&spi_test_thread_handle,
	                   (unsigned short)2048,
					   THD_EXTENDED_APP_PRIORITY,
					   "spi_test",
					   ql_spi_demo_thread,
					   0);


	  if (ret != kNoErr) {
		os_printf("Error: Failed to create spi test thread: %d\r\n",ret);
		goto init_err;
	  }
   
	  return;
   
	  init_err:
	   if( spi_test_thread_handle != NULL ) {
		   ql_rtos_task_delete(spi_test_thread_handle);
	   }

}
