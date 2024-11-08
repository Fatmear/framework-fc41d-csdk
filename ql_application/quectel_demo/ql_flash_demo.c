#include "ql_include.h"



/***************flash test******************/
#if 0
void ql_flash_demo_thread(void *param)
{
	int i,ret = 0;
    UINT8 *buf;
	
	ql_rtos_task_sleep_ms(3000);
		
	buf = os_malloc(TEST_FLASH_LEN * sizeof(UINT8));
	if(buf == 0)
	{
		ql_flash_log("buf malloc failed\r\n");
		goto flash_test_exit;
	}
	os_memset(buf,0,TEST_FLASH_LEN);
	
	for(i=0;i<TEST_FLASH_LEN;i++)
		buf[i] = i%255;

	ql_flash_log(" flash write start\r\n");
	for (int i = 0; i < TEST_FLASH_LEN; i++) {
	  ql_flash_log("%02x,", buf[i]);
	  if ((i + 1) % 32 == 0)
		  ql_flash_log("\r\n");
    }
	
	ql_flash_set_security(QL_FLASH_PROTECT_NONE);
	ret  = ql_flash_write(buf, TEST_FLASH_ADDR, TEST_FLASH_LEN);
	if (ret == QL_FLASH_SUCCESS)
	  ql_flash_log("\r\nflash write success\r\n");
	else
	  ql_flash_log("\r\nflash write fail \r\n");
	ql_flash_set_security(QL_FLASH_UNPROTECT_LAST_BLOCK);

	os_memset(buf,0,TEST_FLASH_LEN);
	ql_flash_log("spi flash read start\r\n");
	ret =  ql_flash_read( buf, TEST_FLASH_ADDR, TEST_FLASH_LEN);
	if (ret == QL_FLASH_SUCCESS)
		{
		  for (int i = 0; i < TEST_FLASH_LEN; i++) {
			  ql_flash_log("%02x,", buf[i]);
			  if ((i + 1) % 32 == 0)
				  ql_flash_log("\r\n");
		   }
		  ql_flash_log("\r\nflash read success\r\n");

		}
	else
	  ql_flash_log("flash read fail \r\n");

	ql_flash_log("flash erase start\r\n");
	ql_flash_set_security(QL_FLASH_PROTECT_NONE);
	ret = ql_flash_erase(TEST_FLASH_ADDR,TEST_FLASH_LEN);
	if (ret == QL_FLASH_SUCCESS)
	  ql_flash_log("flash erase success\r\n");
	else
	  ql_flash_log("flash erase fail\r\n \r\n");
	ql_flash_set_security(QL_FLASH_UNPROTECT_LAST_BLOCK);

	os_memset(buf,0,TEST_FLASH_LEN);
	ret =  ql_flash_read(buf, TEST_FLASH_ADDR, TEST_FLASH_LEN);
		if (ret == QL_FLASH_SUCCESS)
			{
			  ql_flash_log("flash read success\r\n");
			  for (int i = 0; i < TEST_FLASH_LEN; i++) {
				  ql_flash_log("%02x,", buf[i]);
				  if ((i + 1) % 32 == 0)
					  ql_flash_log("\r\n");
			   }
	
			}
		else
		  ql_flash_log("flash read fail\r\n \r\n");

	ql_flash_log("\r\nflash test over \r\n");
	
flash_test_exit:

	os_free(buf);	
	while(1)
	{
	  ql_rtos_task_sleep_ms(1000);
	}
	
}
#else

/***************flash_parttion test******************/

void ql_flash_demo_thread(void *param)
{
	int i,ret = 0;
    UINT8 *buf;
	
	ql_rtos_task_sleep_ms(3000);
		
	buf = os_malloc(TEST_FLASH_LEN * sizeof(UINT8));
	if(buf == 0)
	{
		ql_flash_log("buf malloc failed\r\n");
		goto flash_test_exit;
	}
	os_memset(buf,0,TEST_FLASH_LEN);
	
	for(i=0;i<TEST_FLASH_LEN;i++)
		buf[i] = i%255;

	ql_flash_log(" flash write start\r\n");
	for (int i = 0; i < TEST_FLASH_LEN; i++) {
	  ql_flash_log("%02x,", buf[i]);
	  if ((i + 1) % 32 == 0)
		  ql_flash_log("\r\n");
    }
	
	ql_flash_set_security(QL_FLASH_PROTECT_NONE);
	ret  = ql_flash_parttion_write(QL_PARTITION_USR_RESERVE,0,buf,TEST_FLASH_LEN);
	if (ret == QL_FLASH_SUCCESS)
	  ql_flash_log("\r\nflash write success\r\n");
	else
	  ql_flash_log("\r\nflash write fail \r\n");
	ql_flash_set_security(QL_FLASH_UNPROTECT_LAST_BLOCK);

	os_memset(buf,0,TEST_FLASH_LEN);
	ql_flash_log("spi flash read start\r\n");
	ret =  ql_flash_parttion_read(QL_PARTITION_USR_RESERVE,0,buf,TEST_FLASH_LEN);
	if (ret == QL_FLASH_SUCCESS)
		{
		  for (int i = 0; i < TEST_FLASH_LEN; i++) {
			  ql_flash_log("%02x,", buf[i]);
			  if ((i + 1) % 32 == 0)
				  ql_flash_log("\r\n");
		   }
		  ql_flash_log("\r\nflash read success\r\n");

		}
	else
	  ql_flash_log("flash read fail \r\n");

	ql_flash_log("flash erase start\r\n");
	ql_flash_set_security(QL_FLASH_PROTECT_NONE);
	ret = ql_flash_parttion_erase(QL_PARTITION_USR_RESERVE,0,TEST_FLASH_LEN);
	if (ret == QL_FLASH_SUCCESS)
	  ql_flash_log("flash erase success\r\n");
	else
	  ql_flash_log("flash erase fail\r\n \r\n");
	ql_flash_set_security(QL_FLASH_UNPROTECT_LAST_BLOCK);

	os_memset(buf,0,TEST_FLASH_LEN);
	ret = ql_flash_parttion_read(QL_PARTITION_USR_RESERVE,0,buf,TEST_FLASH_LEN);
		if (ret == QL_FLASH_SUCCESS)
			{
			  ql_flash_log("flash read success\r\n");
			  for (int i = 0; i < TEST_FLASH_LEN; i++) {
				  ql_flash_log("%02x,", buf[i]);
				  if ((i + 1) % 32 == 0)
					  ql_flash_log("\r\n");
			   }
	
			}
		else
		  ql_flash_log("flash read fail\r\n \r\n");

	ql_flash_log("\r\nflash test over \r\n");
	
flash_test_exit:

	os_free(buf);	
	while(1)
	{
	  ql_rtos_task_sleep_ms(1000);
	}
	
}

#endif

ql_task_t	flash_test_thread_handle = NULL;
void ql_flash_demo_thread_creat(void)
{
    int ret;
	ret = ql_rtos_task_create(&flash_test_thread_handle,
		               (unsigned short)4096,
					   THD_EXTENDED_APP_PRIORITY,
					   "flash_test",
					   ql_flash_demo_thread,	   
					   0);


	  if (ret != kNoErr) {
		os_printf("Error: Failed to create flash test thread: %d\r\n",ret);
		goto init_err;
	  }
   
	  return;
   
	  init_err:
	   if( flash_test_thread_handle != NULL ) {
		   ql_rtos_task_delete(flash_test_thread_handle);
	   }

}
