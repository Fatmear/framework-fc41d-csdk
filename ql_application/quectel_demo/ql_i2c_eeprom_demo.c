#include "ql_include.h"


/***************i2c test******************/
ql_task_t	i2c_test_thread_handle = NULL;
void ql_i2c1_eeprom_demo_thread(void *param)
{
   DD_HANDLE i2c_hdl1 = DD_HANDLE_UNVALID;
   uint8_t i;
   unsigned char writebuf[I2C1_TEST_LEGNTH] = {0};
   unsigned char readbuf[I2C1_TEST_LEGNTH] = {0};
   uint8_t page = 0;
   uint8_t remian_bytes = 0;
   uint8_t page_i = 0;
   ql_rtos_task_sleep_ms(3000);

   ql_i2c_log("i2c1 write eeprom start \r\n");
   if(I2C1_TEST_LEGNTH > 255)
   {
		ql_i2c_log("I2C1_TEST_LEGNTH is error \r\n");
		ql_rtos_task_delete(i2c_test_thread_handle);
		return;
   }
   for (i = 0; i < I2C1_TEST_LEGNTH; i++)
	{
		writebuf[i] = i ;
		ql_i2c_log("0x%x  ", writebuf[i]);
	}
	remian_bytes = I2C1_TEST_LEGNTH;

	ql_I2cInit(&i2c_hdl1,FAST_MODE);

	if(I2C1_TEST_LEGNTH < 64 || I2C1_TEST_LEGNTH == 64)
	{
		ql_I2cWrite(i2c_hdl1,I2C1_TEST_SALVE_ID,I2C1_TEST_ADDR,(char*)writebuf,I2C1_TEST_LEGNTH,ADDR_WIDTH_16);
	}
	else if(I2C1_TEST_LEGNTH > 64)
	{

		page = I2C1_TEST_LEGNTH / 64 + 1;
		while(page--)
		{
			if(remian_bytes >64 || remian_bytes == 64)
			{
				ql_I2cWrite(i2c_hdl1,I2C1_TEST_SALVE_ID,I2C1_TEST_ADDR + page_i *64,(char*)writebuf + 64 * page_i,64,ADDR_WIDTH_16);
				page_i++;
				remian_bytes = remian_bytes - 64;
			}
			else
			{
				ql_I2cWrite(i2c_hdl1,I2C1_TEST_SALVE_ID,I2C1_TEST_ADDR + page_i *64,(char*)writebuf + 64 * page_i,remian_bytes,ADDR_WIDTH_16);
			}
		}
	}

   ql_i2c_log("\r\ni2c1 read eeprom start \r\n");
   ql_rtos_task_sleep_ms(100);

   ql_I2cRead(i2c_hdl1,I2C1_TEST_SALVE_ID,I2C1_TEST_ADDR,(char*)readbuf,I2C1_TEST_LEGNTH,ADDR_WIDTH_16);

   for (i = 0; i < I2C1_TEST_LEGNTH; i++)
	 ql_i2c_log("0x%x  ", readbuf[i]);

   ql_i2c_log("\r\ni2c1 eeprom test over \r\n");

   ql_I2cRelease(i2c_hdl1);

	while(1)
  	{
      ql_rtos_task_sleep_ms(1000);
	}

}

void ql_i2c_demo_thread_creat(void)
{
    int ret;
	ret = ql_rtos_task_create(&i2c_test_thread_handle,
	            	(unsigned short)2048,
					 THD_EXTENDED_APP_PRIORITY,
					 "i2c1_test",
					 ql_i2c1_eeprom_demo_thread,
					 0);



	 if (ret != kNoErr) {
	   os_printf("Error: Failed to create i2c test thread: %d\r\n",ret);
	   goto init_err;
	 }

     return;

	 init_err:
	  if( i2c_test_thread_handle != NULL ) {
		  ql_rtos_task_delete(i2c_test_thread_handle);
	  }

}
