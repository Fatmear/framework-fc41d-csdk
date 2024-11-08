#include "ql_include.h"



/***************wdg test******************/

void ql_wdg_demo_thread(void *param)
{

	ql_WdgInit(2000);


	while(1)
	{
	    ql_wdg_reload();
		os_printf("wdg feed\r\n");
		ql_rtos_task_sleep_ms(1000);
	}

}

ql_task_t	wdg_test_thread_handle = NULL;
void ql_wdg_demo_thread_creat(void)
{
    int ret;
	ret = ql_rtos_task_create(&wdg_test_thread_handle,
		               (unsigned short)1024,
					   THD_EXTENDED_APP_PRIORITY,
					   "wdg_test",
					   ql_wdg_demo_thread,
					   0);


	  if (ret != kNoErr) {
		os_printf("Error: Failed to create wdg test thread: %d\r\n",ret);
		goto init_err;
	  }
   
	  return;
   
	  init_err:
	   if( wdg_test_thread_handle != NULL ) {
		   ql_rtos_task_delete(wdg_test_thread_handle);
	   }

}

