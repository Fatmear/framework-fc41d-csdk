#ifndef _POWER_SAVE_PUB_H_
#define _POWER_SAVE_PUB_H_

#include "arch.h"
#include "rtos_pub.h"
#include "rw_pub.h"
#include "wlan_ui_pub.h"
#include "sys_ctrl_pub.h"
#include "drv_model_pub.h"

//#define PS_DEBUG

#ifdef PS_DEBUG
#define PS_PRT                 os_printf
#define PS_WPRT                os_printf
#define PS_DBG                 os_printf
#else
#define PS_PRT                 os_null_printf
#define PS_WPRT                os_null_printf
#define PS_DBG                 os_null_printf
#endif

#define PS_DTIM_COUNT           (1)
#define PS_LISTEN_INTERVAL_MARGIN           (10)

#define LOW_VOL_NULL_SEND_INTERVAL         10000000
#define LOW_VOL_ARP_SEND_INTERVAL         10//s

#define PS_USE_KEEP_TIMER       1
#define PS_USE_WAIT_TIMER       1
#define PS_WAKEUP_MOTHOD_RW     1

typedef enum {
    PS_BMSG_IOCTL_RF_ENABLE = 0,
    PS_BMSG_IOCTL_RF_DISANABLE = 1,
    PS_BMSG_IOCTL_MCU_ENABLE = 2,
    PS_BMSG_IOCTL_MCU_DISANABLE = 3,
    PS_BMSG_IOCTL_RF_USER_WKUP = 4,
    PS_BMSG_IOCTL_RF_KP_SET = 5,
    PS_BMSG_IOCTL_RF_TD_SET = 6,
    PS_BMSG_IOCTL_RF_KP_HANDLER = 7,
    PS_BMSG_IOCTL_RF_TD_HANDLER = 8,
    PS_BMSG_IOCTL_RF_KP_STOP = 9,
    PS_BMSG_IOCTL_WAIT_TM_SET = 10,
    PS_BMSG_IOCTL_WAIT_TM_HANDLER = 11,
    PS_BMSG_IOCTL_RF_PS_TIMER_INIT = 12,
    PS_BMSG_IOCTL_RF_PS_TIMER_DEINIT = 13,
    PS_BMSG_IOCTL_AP_PS_STOP = 14,
    PS_BMSG_IOCTL_AP_PS_START = 15,
    PS_BMSG_IOCTL_AP_PS_RUN = 16,

} PS_BMSG_IOCTL_CMD;

#define ICU_BASE                                     (0x00802000)
#define ICU_INTERRUPT_ENABLE                         (ICU_BASE + 16 * 4)
#define ICU_PERI_CLK_PWD                             (ICU_BASE + 2 * 4)
#define ICU_ARM_WAKEUP_EN                            (ICU_BASE + 20 * 4)

#define   PS_WAKEUP_MOTHOD_RW     1

enum {
    NEED_DISABLE = 0,
    NEED_REBOOT = 1,
};
#define NEED_DISABLE_BIT            CO_BIT(NEED_DISABLE)
#define NEED_REBOOT_BIT             CO_BIT(NEED_REBOOT)

typedef enum {
    PS_FORBID_NO_ON = 1,
    PS_FORBID_PREVENT = 2,
    PS_FORBID_VIF_PREVENT = 3,
    PS_FORBID_IN_DOZE = 4,
    PS_FORBID_KEEVT_ON = 5,
    PS_FORBID_BMSG_ON = 6,
    PS_FORBID_TXING = 7,
    PS_FORBID_HW_TIMER = 8,
    PS_FORBID_RXING = 9,
} PS_FORBID_STATUS;

typedef enum {
    PS_NO_PS_MODE = 0,
    PS_STANDBY_PS_MODE = 1,
    PS_MCU_PS_MODE = 2,
    PS_DTIM_PS_MODE = 3,
    PS_DTIM_PS_OPENING = 4,
    PS_DTIM_PS_CLOSING = 5,
} PS_MODE_STATUS;


#define  PRINT_LR_REGISTER()                \
	{                                           \
		\
		uint32_t value;                         \
		\
		__asm volatile(                         \
		                                        "MOV %0,lr\n"                       \
	                                        :"=r" (value)                       \
			                                        :                                   \
			                                        :"memory"                           \
			              );                                      \
		\
		os_printf("lr:%x\r\n", value);          \
	}

extern UINT8 power_save_if_ps_can_sleep ( void );
extern UINT16 power_save_forbid_trace ( PS_FORBID_STATUS forbid );
extern UINT16 power_save_beacon_len_get ( void );
extern void power_save_dump ( void );
extern UINT8 power_save_if_rf_sleep();
extern UINT16 power_save_radio_wkup_get ( void );
extern void power_save_radio_wkup_set ( UINT16 );
extern UINT8 power_save_set_dtim_multi ( UINT8 );
extern UINT8 power_save_sm_set_all_bcmc ( UINT8 );
extern void power_save_wkup_event_set ( UINT32 );
extern UINT8 power_save_if_ps_rf_dtim_enabled ( void );
extern int power_save_dtim_enable();
extern int power_save_dtim_disable();
extern void power_save_rf_dtim_manual_do_wakeup ( void );
extern void power_save_rf_ps_wkup_semlist_set ( void );
#if ( 1 == CFG_LOW_VOLTAGE_PS)
extern uint32_t power_save_rf_sleep_check ( void );
#else
extern bool power_save_rf_sleep_check ( void );
#endif
extern uint32_t ps_get_sleep_prevent(void);
extern void ps_set_key_prevent ( void );
extern void ps_clear_key_prevent ( void );
extern void ps_set_data_prevent ( void );
extern void ps_clear_prevent ( void );
extern void txl_cntrl_dec_pck_cnt ( void );
extern void txl_cntrl_inc_pck_cnt ( void );
extern int bmsg_is_empty ( void );
extern int net_if_is_up ( void );
extern void power_save_beacon_len_set ( UINT16 );
extern void power_save_beacon_state_update ( void );
extern void power_save_cal_bcn_liston_int ( UINT16 );
extern void power_save_delay_sleep_check ( void );
extern int power_save_dtim_disable_handler ( void );
extern int power_save_dtim_enable_handler ( void );
extern INT8 power_save_if_sleep_at_first ( void );
extern UINT8 power_save_if_sleep_first ( void );
extern PS_MODE_STATUS power_save_ps_mode_get ( void );
extern void power_save_ps_mode_set ( PS_MODE_STATUS );
extern void power_save_rf_ps_wkup_semlist_init ( void );
extern void *power_save_rf_ps_wkup_semlist_create ( void );
extern void *power_save_rf_ps_wkup_semlist_insert ( void );
extern void power_save_rf_ps_wkup_semlist_wait ( void * );
extern void power_save_rf_ps_wkup_semlist_destroy ( void * );
extern void power_save_rf_ps_wkup_semlist_get ( void * );
extern void power_save_set_temp_use_rf_flag(void);
extern void power_save_clr_temp_use_rf_flag(void);


extern void power_save_set_dtim_count ( UINT8 );
extern void power_save_set_dtim_period ( UINT8 );
extern void power_save_sleep_status_set ( void );
extern bool  power_save_sleep ( void );
extern void power_save_wkup_event_clear ( UINT32 );
extern void power_save_wkup_event_set ( UINT32 );
extern UINT32 power_save_wkup_event_get ( void );
extern UINT8 power_save_get_liston_int ( void );
extern int power_save_get_wkup_less_time();
extern void power_save_set_linger_time ( UINT32 );
extern void power_save_dtim_wake ( UINT32 );
extern UINT32 power_save_use_pwm0_isr ( void );
extern void power_save_td_ck_timer_set ( void );
extern void power_save_pwm0_isr ( UINT8 param );
extern void power_save_keep_timer_set ( void );
extern void power_save_keep_timer_real_handler();
extern void power_save_td_ck_timer_real_handler();
extern void power_save_keep_timer_stop ( void );
extern UINT32 power_save_get_sleep_count ( void );
extern void power_save_set_reseted_flag ( void );
extern UINT32 power_save_get_rf_ps_dtim_time ( void );
extern uint8_t ble_switch_mac_sleeped;
extern void power_save_set_keep_timer_time ( UINT32 );
extern void power_save_wake_mac_rf_end_clr_flag(void);

extern void ps_set_rf_prevent(void);
extern void ps_clear_rf_prevent(void);

void ps_set_td_timer(void);
UINT32 bk_unconditional_sleep_mode_get ( void );
void ps_set_mac_reset_prevent(void);
void ps_clear_mac_reset_prevent(void);
UINT8 power_save_low_latency_get ( void );
void power_save_set_low_latency ( UINT8 );
void power_save_set_liston_int(UINT16 listen_int);
extern void power_save_wait_timer_set(void );

void power_save_pwm_isr(UINT8 param);
UINT32 power_save_use_pwm_isr(void);


/***************************************************************************/
extern void power_save_wake_rf_if_in_sleep(void);
extern void power_save_check_clr_rf_prevent_flag(void);
extern void power_save_wake_mac_rf_if_in_sleep(void);
/***************************************************************************/


#endif // _POWER_SAVE_PUB_H_
// eof

