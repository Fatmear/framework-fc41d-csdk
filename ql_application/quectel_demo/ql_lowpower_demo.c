#include "ql_include.h"


#define lowpoer_test_mode 1
/***************lowvol test******************/
#if (lowpoer_test_mode == 1)
void ql_lowpower_demo_thread(void *param)
{
	ql_sleep_ctrl_parm_s lowvol_sleep_param;


	ql_rtos_task_sleep_ms(3000);
#if 0
    ql_lowpower_log("lowvol RTC wake up test \r\n");
	
	lowvol_sleep_param.gpio_index_map 		= 0;
	lowvol_sleep_param.gpio_edge_map		= 0;
	lowvol_sleep_param.gpio_last_index_map	= 0;
	lowvol_sleep_param.gpio_last_edge_map 	= 0;
	lowvol_sleep_param.sleep_time 			= 0x64;
	lowvol_sleep_param.wake_up_way			= 2;
	ql_lowpower_log("---lowvol sleep test param : 0x%0X 0x%0X 0x%0X 0x%0X %d %d\r\n", 
					lowvol_sleep_param.gpio_index_map, 
					lowvol_sleep_param.gpio_edge_map,
					lowvol_sleep_param.gpio_last_index_map, 
					lowvol_sleep_param.gpio_last_edge_map,
					lowvol_sleep_param.sleep_time,
					lowvol_sleep_param.wake_up_way);

	
#else

   ql_lowpower_log("lowvol GPIO wake up test \r\n");
   
   lowvol_sleep_param.gpio_index_map	   = 0x00400000;//gpio22
   lowvol_sleep_param.gpio_edge_map		   = 0x00400000;
   lowvol_sleep_param.gpio_last_index_map  = 0;
   lowvol_sleep_param.gpio_last_edge_map   = 0;
   lowvol_sleep_param.sleep_time		   = 0;
   lowvol_sleep_param.wake_up_way 		   = 1;
   ql_lowpower_log("---lowvol sleep test param : 0x%X 0x%X 0x%X 0x%X %d %d\r\n", 
				   lowvol_sleep_param.gpio_index_map, 
				   lowvol_sleep_param.gpio_edge_map,
				   lowvol_sleep_param.gpio_last_index_map, 
				   lowvol_sleep_param.gpio_last_edge_map,
				   lowvol_sleep_param.sleep_time,
				   lowvol_sleep_param.wake_up_way);

#endif
	ql_lowvol_sleep_enter(lowvol_sleep_param);

	ql_lowpower_log("lowvol sleep wake up\r\n");

	while(1)
	{
	  ql_rtos_task_sleep_ms(1000);
	}
	
}

#elif (lowpoer_test_mode == 2)

/***************deep sleep test******************/

void ql_lowpower_demo_thread(void *param)
{
	ql_sleep_ctrl_parm_s deep_sleep_param;

	GLOBAL_INT_DECLARATION();

	ql_rtos_task_sleep_ms(3000);
	
	ql_lowpower_log("deep sleep GPIO wake up test \r\n");
	  

    deep_sleep_param.gpio_index_map         = 0x00400000;//gpio22
    deep_sleep_param.gpio_edge_map          = 0x00400000;
    deep_sleep_param.gpio_last_index_map    = 0;
    deep_sleep_param.gpio_last_edge_map     = 0;
    deep_sleep_param.sleep_time             = 0;

	deep_sleep_param.sleep_time             = 0x64;
	
    deep_sleep_param.wake_up_way            = QL_DEEP_WAKEUP_GPIO|QL_DEEP_WAKEUP_RTC;
    deep_sleep_param.gpio_stay_lo_map       = 0x00400000;
    deep_sleep_param.gpio_stay_hi_map       = 0;
		

	
	ql_lowpower_log("---deep sleep test param : 0x%X 0x%X 0x%X 0x%X %d %d\r\n",
				deep_sleep_param.gpio_index_map,
				deep_sleep_param.gpio_edge_map,
				deep_sleep_param.gpio_last_index_map,
				deep_sleep_param.gpio_last_edge_map,
				deep_sleep_param.sleep_time,
				deep_sleep_param.wake_up_way);
	GLOBAL_INT_DISABLE();

	ql_deep_sleep_enter(deep_sleep_param);

	GLOBAL_INT_RESTORE();


	while(1)
	{
	  ql_rtos_task_sleep_ms(1000);
	}
	
}


#else
/***************Wifi connection alive sleep test******************/
//ps_clear_app_prevent()
//ps_clear_gpio_prevent()
void ql_lowpower_demo_thread(void *param)
{

	ql_rtos_task_sleep_ms(30000);
	
	//first connect the wifi
	
	ql_lowpower_log("Wifi connection alive wake up test \r\n");


	ql_deep_rfdtim_enter(1,0);
	


	ql_deep_mcudtim_enter(1,30);	
	ql_rtos_task_sleep_ms(10000);

	
	ql_lowpower_log("Wifi gpio alive wake up \r\n");



	while(1)
	{
	  ql_lowpower_log(" wake up\r\n");
	  ql_rtos_task_sleep_ms(1000);
	}
	
}
#endif

ql_task_t	lowpower_test_thread_handle = NULL;
void ql_lowpower_demo_thread_creat(void)
{
    int ret;
	ret = ql_rtos_task_create(&lowpower_test_thread_handle,
		               (unsigned short)2048,
					   THD_EXTENDED_APP_PRIORITY,
					   "lowpower_test",
					   ql_lowpower_demo_thread,  
					   0);


	  if (ret != kNoErr) {
		os_printf("Error: Failed to create lowpower test thread: %d\r\n",ret);
		goto init_err;
	  }
   
	  return;
   
	  init_err:
	   if( lowpower_test_thread_handle != NULL ) {
		   ql_rtos_task_delete(lowpower_test_thread_handle);
	   }

}
