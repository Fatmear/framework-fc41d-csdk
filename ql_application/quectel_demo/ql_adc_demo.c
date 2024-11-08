#include "ql_include.h"


/***************adc test******************/
static ql_adc_obj_s ql_test_adc;
extern uint8_t adctest_flag ;
ql_task_t	adc_test_thread_handle = NULL;

static void ql_adc_detect_callback(int new_mv, void *user_data)
{
	static int cnt = 0;
	if (cnt++ >= 100)
	{
		cnt = 0;
		ql_adc_log("adc channel%d voltage:%d mV\r\n", ql_test_adc.channel, new_mv);
	}
}
void ql_adc_demo_thread(void *param)
{
	ql_rtos_task_sleep_ms(3000);

    ql_adc_log("adc test start\r\n");
	adctest_flag = 1;
	ql_adc_thread_init();
	ql_adc_channel_init(&ql_test_adc, ql_adc_detect_callback, ADC_TEST_CHANNEL, NULL);
	ql_adc_channel_start(&ql_test_adc);
    ql_rtos_task_delete(adc_test_thread_handle);
}


void ql_adc_demo_thread_creat(void)
{
    int ret;
	ret = ql_rtos_task_create(&adc_test_thread_handle,
					   (unsigned short)2048,
					   THD_EXTENDED_APP_PRIORITY,
					   "adc_test",
					   ql_adc_demo_thread,   
					   0);


	  if (ret != kNoErr) {
		os_printf("Error: Failed to create adc test thread: %d\r\n",ret);
		goto init_err;
	  }
   
	  return;
   
	  init_err:
	   if( adc_test_thread_handle != NULL ) {
		   ql_rtos_task_delete(adc_test_thread_handle);
	   }

}
