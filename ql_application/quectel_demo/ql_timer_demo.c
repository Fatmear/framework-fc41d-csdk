#include "ql_include.h"



/***************timer test******************/

/********************/
//当测试定时器时间间隔大于1秒时，将TIMER_TEST_CHOSE设为1;时间间隔小于1秒时，将TIMER_TEST_CHOSE设为0�?
#define TIMER_TEST_CHOSE			1
/***************timer test******************/

static void ql_timer_test_isr_cb(UINT8 arg)
{
#if TIMER_TEST_CHOSE

	ql_timer_log("rtos-time: %d ms\r\n",rtos_get_time());

#else

	ql_gpio_set_level_reverse(QL_GPIO7); //QL_GPIO7为P7引脚，FC41DAHMD DK板子上为PWM2排针引脚�?
#endif

}

void ql_timer_demo_thread(void *param)
{

	UINT32 ret;
	ql_rtos_task_sleep_ms(3000);
#if TIMER_TEST_CHOSE

	ret = ql_TimerInit(TIMER_TEST_ID,1000,ql_timer_test_isr_cb);

#else

	ret = ql_gpio_init(QL_GPIO7,GMODE_OUTPUT);

	ql_gpio_set_level(QL_GPIO7,QL_GPIO_OUTPUT_HIGH);

	ret = ql_TimerInit(TIMER_TEST_ID,1,ql_timer_test_isr_cb);
#endif
    if(ret == QL_TIMER_SUCCESS )
	 ql_timer_log("timer init success\r\n");
	else
	 ql_timer_log("timer init fail\r\n");

	ql_TimerStart(TIMER_TEST_ID);

	ql_rtos_task_sleep_ms(20010);

	ql_TimerStop(TIMER_TEST_ID);

    if(ret == QL_TIMER_SUCCESS )
	 ql_timer_log("timer stop success\r\n");
	else
	 ql_timer_log("timer stop fail\r\n");

	while(1)
	{
	  ql_rtos_task_sleep_ms(1000);
	}

}

ql_task_t	timer_test_thread_handle = NULL;
void ql_timer_demo_thread_creat(void)
{
   int ret;
   ret = ql_rtos_task_create(&timer_test_thread_handle,
            	(unsigned short)2048,
				 THD_EXTENDED_APP_PRIORITY,
				 "timer_test",
				 ql_timer_demo_thread,
				 0);


				 if (ret != kNoErr) {
				   os_printf("Error: Failed to create timer test thread: %d\r\n",ret);
				   goto init_err;
				 }

	 return;

	 init_err:
	  if( timer_test_thread_handle != NULL ) {
		  ql_rtos_task_delete(timer_test_thread_handle);
	  }

}
