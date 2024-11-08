#ifndef _QL_TIMER_H_
#define _QL_TIMER_H_


#define TIMER_TEST_DEBUG
#ifdef TIMER_TEST_DEBUG
#define ql_timer_log               os_printf

#else
#define ql_timer_log               os_null_printf
#endif


#define TIMER_TEST_ID  QL_TIMER_0

typedef void (*ql_timer_callback)(UINT8 arg);

typedef enum
{
	 QL_TIMER_SUCCESS = 0,
	 QL_TIMER_EXECUTE_ERR,
	 QL_TIMER_INVALID_PARAM_ERR,
	 QL_TIMER_NOT_OPEN_ERR,
} ql_timer_errcode_e;


typedef enum 
{
	 QL_TIMER_0 =0,
	 QL_TIMER_1,
	 QL_TIMER_2,
	 QL_TIMER_3,
	 QL_TIMER_4,
	 QL_TIMER_5,
}ql_timer_number_e;

ql_timer_errcode_e ql_TimerInit(ql_timer_number_e timer_id,UINT32 time_ms,ql_timer_callback timer_cb);
ql_timer_errcode_e ql_TimerInit_us(ql_timer_number_e timer_id,UINT32 time_us,ql_timer_callback timer_cb);
ql_timer_errcode_e ql_get_timer_cnt(ql_timer_number_e timer_id,UINT32 *cont);
ql_timer_errcode_e ql_TimerStart(ql_timer_number_e timer_id);
ql_timer_errcode_e ql_TimerStop(ql_timer_number_e timer_id);
void ql_timer_demo_thread(void *param);
void ql_timer_demo_thread_creat(void);

#endif 

