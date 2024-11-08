#ifndef _QL_PWM_H
#define _QL_PWM_H


#define PWM_TEST_DEBUG
#ifdef PWM_TEST_DEBUG
#define ql_pwm_log               os_printf

#else
#define ql_pwm_log               os_null_printf
#endif

typedef void (*pwm_callbak)(UINT8);


#define  PWM_TEST_CHANNEL   QL_PWM_0
#define  PWM_TEST_PERIOD    26000
#define  PWM_TEST_DUTY      13000

typedef enum
{
	 QL_PWM_SUCCESS = 0,
	 QL_PWM_EXECUTE_ERR,
	 QL_PWM_INVALID_PARAM_ERR,
} ql_pwm_errcode_e;


typedef enum
{
    QL_PWM_0,
    QL_PWM_1,
    QL_PWM_2,
    QL_PWM_3,
    QL_PWM_4,
    QL_PWM_5,
} ql_pwm_channel_e;
	
typedef enum
{
    QL_PWM_CAP_POS_MODE = 4,
    QL_PWM_CAP_NEG_MODE = 5,
    QL_PWM_CAP_EDGE_MODE = 6,
} ql_pwm_cap_mode_e;
	



typedef enum 
{
	 QL_PWM_INIT_LEVEL_LOW =0,
	 QL_PWM_INIT_LEVEL_HIGH,		 

}ql_pwm_init_level_e;

ql_pwm_errcode_e ql_pwmInit(ql_pwm_channel_e pwm, UINT32 period, UINT32 duty_cycle);
ql_pwm_errcode_e ql_pwmInit_level(ql_pwm_channel_e pwm, ql_pwm_init_level_e level);
ql_pwm_errcode_e ql_pwm_capture_Init(ql_pwm_channel_e pwm, ql_pwm_cap_mode_e cap_mode,pwm_callbak calbak);
ql_pwm_errcode_e ql_pwm_enable(ql_pwm_channel_e pwm);
ql_pwm_errcode_e ql_pwm_disable(ql_pwm_channel_e pwm);
ql_pwm_errcode_e ql_pwm_update_param(ql_pwm_channel_e pwm, UINT32 period, UINT32 duty_cycle);
UINT32 ql_pwm_get_capvalue(ql_pwm_channel_e pwm);

void ql_pwm_demo_thread_creat(void);


#endif 

