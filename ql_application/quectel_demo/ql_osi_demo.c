#include "ql_include.h"


 /***************osi test******************/
 ql_task_t  osi_test1_thread_handle = NULL;
 ql_task_t  osi_test2_thread_handle = NULL;
 ql_sem_t   osi_test_semaphore = NULL;
 ql_mutex_t osi_test_mutex = NULL;
 ql_timer_t osi_test_timer ;

 int cnt = 0;
 void ql_osi_test1_thread(void *param)
 {
	 ql_rtos_task_sleep_s(10);
	 ql_rtos_task_suspend(osi_test2_thread_handle);
	 os_printf("osi tesk2 suspend\r\n");
	 ql_rtos_task_sleep_s(20);
	 ql_rtos_timer_change_period(osi_test_timer,2000);
	 ql_rtos_task_resume(osi_test2_thread_handle);
	 os_printf("osi tesk2 resume\r\n");		   

	 while(1)
	 {

	   ql_rtos_semaphore_wait(osi_test_semaphore,0xFFFFFFFF);
	   
	   ql_rtos_mutex_lock(osi_test_mutex);
	   cnt++;
	   ql_rtos_mutex_unlock(osi_test_mutex);

	   os_printf("cnt = %d\r\n",cnt);

	 }
	 
 }
 
 void ql_osi_test2_thread(void *param)
 {

	 while(1)
	 {
		 if(cnt ==10)
	     {

			 ql_rtos_task_change_priority(osi_test2_thread_handle,7);
		 }
	     else  if(cnt >= 20)
       	 {
	     	ql_rtos_mutex_lock(osi_test_mutex);
			cnt=0;
			ql_rtos_mutex_unlock(osi_test_mutex);
			ql_rtos_timer_stop(osi_test_timer);
        }

	   os_printf("osi task2 running\r\n");

	   ql_rtos_task_sleep_ms(1000);
	 }
	 
 }

 void osi_timer_alarm( void *arg )
{

	os_printf("rtos time %d\r\n",ql_rtos_up_time_ms());
	
	ql_rtos_semaphore_release(osi_test_semaphore);

}
 

 void ql_osi_demo_thread_creat(void)
 {
	 int ret;
	 ret = ql_rtos_task_create(&osi_test1_thread_handle,
	 	                (unsigned short)1024,
						THD_EXTENDED_APP_PRIORITY,
						"osi_test1",
						ql_osi_test1_thread,
						0);
 
     if (ret != kNoErr) {
	  os_printf("Error: Failed to create osi test1 thread: %d\r\n",ret);
	  goto init_err;
    }

	ret = ql_rtos_task_create(&osi_test2_thread_handle,
				        (unsigned short)1024,
						THD_EXTENDED_APP_PRIORITY,
						"osi_test2",
						ql_osi_test2_thread,
						0);

	if (ret != kNoErr) {
	  os_printf("Error: Failed to create osi test2 thread: %d\r\n",ret);
	  goto init_err;
    }


	ret = ql_rtos_semaphore_create(&osi_test_semaphore, 10);
	if (ret != kNoErr) {
	  os_printf("Error: Failed to create osi test semaphore: %d\r\n",ret);
	  goto init_err;
    }


	ret = ql_rtos_mutex_create(&osi_test_mutex);
	if (ret != kNoErr) {
	  os_printf("Error: Failed to create osi test mute: %d\r\n",ret);
	  goto init_err;
    }

    ret = ql_rtos_timer_create(&osi_test_timer, 1000, osi_timer_alarm, 0);  
    if(kNoErr != ret)
	{
	   os_printf("Error: Failed to create osi test timer: %d\r\n",ret);
       goto  init_err;
	}

	ret = ql_rtos_timer_start(osi_test_timer);
    if(kNoErr != ret)
	{
	   os_printf("Error: Failed to start osi test timer: %d\r\n",ret);
       goto  init_err;
	}else
		 os_printf(" osi test timer start\r\n");

	return;
	
 init_err:

	if( osi_test1_thread_handle != NULL ) {
		ql_rtos_task_delete(osi_test1_thread_handle);
	}

	if( osi_test2_thread_handle != NULL ) {
		ql_rtos_task_delete(osi_test2_thread_handle);
	}
	
	if(osi_test_semaphore != NULL){
		ql_rtos_semaphore_delete(osi_test_semaphore);
	}
	
	if(osi_test_mutex != NULL){
		ql_rtos_mutex_delete(osi_test_mutex);
	}
	
	ql_rtos_timer_delete(osi_test_timer);

 }

