#ifndef _QL_LOWPOWER_H
#define _QL_LOWPOWER_H


#define LOWPOWER_TEST_DEBUG
#ifdef LOWPOWER_TEST_DEBUG
#define ql_lowpower_log               os_printf

#else
#define ql_lowpower_log               os_null_printf
#endif



typedef enum {
    QL_DEEP_WAKEUP_GPIO = 1,
    QL_DEEP_WAKEUP_RTC = 2,
} ql_deep_wakeup_way;


typedef struct  ql_ps_sleep_ctrl {

	/*deep_sleep wakeup mode */
	ql_deep_wakeup_way wake_up_way;
	
	/** @brief	Request deep sleep,and wakeup by gpio.
	 *
	 *	@param	gpio_index_map:The gpio bitmap which set 1 enable wakeup deep sleep.
	 *				gpio_index_map is hex and every bits is map to gpio0-gpio31.
	 *				ps:gpio1 as uart RX pin must be wake up from falling
	 *			gpio_edge_map:The gpio edge bitmap for wakeup gpios,
	 *				gpio_edge_map is hex and every bits is map to gpio0-gpio31.
	 *				0:rising,1:falling.
	 *		gpio_stay_lo_map:The gpio bitmap which need stay ,not change in deep sleep.
	 * 			 gpio_index_lo_map is hex and every bits is map to gpio0-gpio31.
				gpio_last_index_map:The gpio bitmap which set 1 enable wakeup deep sleep.
	  * 			 gpio_index_map is hex and every bits is map to gpio32-gpio39.
	  * 		gpio_last_edge_map:The gpio edge bitmap for wakeup gpios,
	  * 			 gpio_edge_map is hex and every bits is map to gpio32-gpio39.
	  * 			 0:rising,1:falling.
	 *		gpio_stay_hi_map:The gpio bitmap which need stay ,not change in deep sleep.
	 * 			 gpio_index_lo_map is hex and every bits is map to gpio32-gpio39.
	 *
	 *      sleep_time:the time secound when use PS_DEEP_WAKEUP_RTC wakeup.
	 *      lpo_32k_src:the RTC wakeup source.LPO_SELECT_ROSC or LPO_SELECT_32K_XTAL.
	 */
	
	UINT32 gpio_index_map;
	UINT32 gpio_edge_map;
	UINT32 gpio_stay_lo_map;
	UINT32 gpio_last_index_map;
	UINT32 gpio_last_edge_map;
	UINT32 gpio_stay_hi_map;
	
	UINT32 sleep_time;
	UINT32 lpo_32k_src;
} ql_sleep_ctrl_parm_s;


typedef enum
{
	 QL_LOWPOWER_SUCCESS = 0,
	 QL_LOWPOWER_EXECUTE_ERR,
	 QL_LOWPOWER_INVALID_PARAM_ERR,
} ql_lowpower_errcode_e;





ql_lowpower_errcode_e ql_deep_sleep_enter(ql_sleep_ctrl_parm_s deep_sleep_param);
ql_lowpower_errcode_e ql_lowvol_sleep_enter(ql_sleep_ctrl_parm_s lowvol_sleep_param);
ql_lowpower_errcode_e ql_deep_mcudtim_enter(UINT32 enable, UINT32 dtimnum);
ql_lowpower_errcode_e ql_deep_rfdtim_enter(UINT32 enable,UINT32 arpenable);
ql_lowpower_errcode_e ql_set_gpio_wakeup_index(UINT8 gpio);
ql_lowpower_errcode_e ql_ps_clear_gpio(void);
ql_lowpower_errcode_e ql_ps_clear_app(void);
ql_lowpower_errcode_e ql_ps_set_waite_timer_time(UINT32 time);

void ql_lowpower_demo_thread(void *param);
void ql_lowpower_demo_thread_creat(void);

#endif 

