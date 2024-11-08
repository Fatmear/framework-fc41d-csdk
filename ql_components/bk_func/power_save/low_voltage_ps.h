#ifndef _LOW_VOLTAGE_PS_H_
#define _LOW_VOLTAGE_PS_H_

#include "include.h"
#include "uart_pub.h"

#define LV_PS_DEBUG                1

#if LV_PS_DEBUG
#define LV_PS_PRT                 os_printf
#else
#define LV_PS_PRT                 os_null_printf
#endif

#define DURATION_AHEAD_OF_TBTT_US         (5 * 1000)  /*uS*/ /*obsoleting macro*/
#define DURATION_BEACON_INTERVAL_US       (100 * 1024)
#define DURATION_WAKEUP_STABILIZATION_US  (1 * 1000)

#define CELL_DURATION                     (1000)  /* US*/
#define DTIM_COUNT_WHEN_MISSING_BEACON    (1)

#define LEAD_FORCE_TIME                     (0000)  /* US*/
#define LV_PS_TBTT_TO_RXD_MAX		(30 * 1000) /* US */
#define LV_PS_NORMAL_BCN_RX_OFFSET	(400)	    /* US */	


#define WAIT_UNTIL_RECVED          (0)
#define WAIT_ONCE                  (1)
#define NO_WAIT                    (2)
#define WAIT_ONCE_ON_CONT_LOSS     (3)

#if(CFG_LV_PS_QUECTEL == 1)
#define AFTER_MISSING_STRATEGY    WAIT_ONCE
#else
#define AFTER_MISSING_STRATEGY    WAIT_UNTIL_RECVED
#endif

typedef struct 
{
    uint32_t ps_print_enable;
    uint32_t ps_print_period;
    uint32_t ps_statistical_period;
    uint32_t ps_print_flag;
    uint64_t ps_start_time;
    uint64_t running_time_in_total;//run time from first sleep 
    uint64_t wakeup_time_in_total;//in us
    uint64_t wakeup_time_in_total_without_receive_data;//in us
    uint64_t wakeup_to_beacon_time_in_total;//in us
    uint32_t beacon_missing_count_in_current_statistical_period;//if the fisrt wakeup miss beacon can be count
    uint32_t beacon_missing_count_in_statistical_period[10];//if the fisrt wakeup miss beacon can be count
    uint32_t beacon_missing_count_in_total;//if the fisrt wakeup miss beacon can be count
    uint32_t sleep_count_in_current_statistical_period;
    uint32_t sleep_count_in_total;
    uint32_t sleep_count_in_total_without_receive_data;
    uint32_t connection_loss_flag;
    uint32_t connection_loss_count;
    uint64_t connection_loss_time_start;
    uint64_t connection_loss_time_end;
    uint64_t ps_print_begin_time;
    uint64_t ps_last_print_time;
    uint64_t ps_last_statistical_time;
    uint32_t ps_arp_enable;
    uint32_t ps_arp_period;
}PS_INFO_T;

extern uint64_t lv_ps_wakeup_mac_timepoint;
extern PS_INFO_T ps_info;
extern uint32_t lv_ps_beacon_cnt_after_wakeup ;

extern void lv_ps_init(void);
extern uint32_t lv_ps_get_keep_timer_duration(void);
void lv_ps_set_bcn_data(uint64_t bcn_tsf, uint32_t bcn_int,
uint32_t duration_of_frame, uint32_t duration_to_timestamp);
void lv_ps_set_bcn_timing(uint64_t local_time, uint64_t duration_tbtt_to_rxd);
uint32_t lv_ps_get_sleep_duration(void);
void lv_ps_clear_anchor_point(void);
uint64_t lv_ps_wakeup_set_timepoint(void);
uint32_t lv_ps_calc_sleep_duration(void);
void sctrl_enable_lvps_rosc_timer(void);
uint32_t lv_ps_is_got_anchor_point(void);
uint32_t lv_ps_is_super_anchor_point(void);
uint32_t lv_ps_beacon_missing_handler(void);
uint32_t lv_ps_get_start_flag(void);
void lv_ps_clear_start_flag(void);
uint32_t lv_ps_set_start_flag(void);
uint32_t lv_ps_recv_beacon(void);
void lv_ps_set_anchor_point(void);
void us_to_readable_value(uint64_t us,uint32_t * h,uint32_t *m,uint32_t *s);




#endif
// eof

