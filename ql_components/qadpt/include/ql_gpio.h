#ifndef _QL_GPIO_H
#define _QL_GPIO_H


#define GPIO_TEST_DEBUG
#ifdef GPIO_TEST_DEBUG
#define ql_gpio_log               os_printf

#else
#define ql_gpio_log               os_null_printf
#endif

typedef void (*ql_gpio_irq_callback)( void *arg );


typedef enum
{
	 QL_GPIO_SUCCESS = 0,
	 QL_GPIO_EXECUTE_ERR,
	 QL_GPIO_INVALID_PARAM_ERR,
} ql_gpio_errcode_e;

typedef enum 
{
    QL_GPIO0 = 0,
    QL_GPIO1,
    QL_GPIO6 = 6,
    QL_GPIO7,
    QL_GPIO8,
    QL_GPIO9,
    QL_GPIO10,
    QL_GPIO11,
    QL_GPIO14 = 14,
    QL_GPIO15,
    QL_GPIO16,
    QL_GPIO17,
    QL_GPIO20 = 20,
    QL_GPIO21,
    QL_GPIO22,
    QL_GPIO23,
    QL_GPIO24,
    QL_GPIO26 = 26,
    QL_GPIO28 = 28,   
} ql_gpio_num_e ;
	
typedef enum
{
    QL_GMODE_INPUT_PULLDOWN = 0,
    QL_GMODE_OUTPUT,
    QL_GMODE_SECOND_FUNC,
    QL_GMODE_INPUT_PULLUP,
    QL_GMODE_INPUT,
    QL_GMODE_SECOND_FUNC_PULL_UP,//Special for uart1
    QL_GMODE_OUTPUT_PULLUP,
    QL_GMODE_SET_HIGH_IMPENDANCE,
    QL_GMODE_DEEP_PS,
}ql_gpio_mode_e;	

typedef enum 
{
	 QL_GPIO_OUTPUT_LOW =0,
	 QL_GPIO_OUTPUT_HIGH,		 

}ql_gpio_output_level_e;

typedef enum
{
    QL_IRQ_TRIGGER_LOW_LEVEL = 0x0,
    QL_IRQ_TRIGGER_HGIH_LEVEL,	
    QL_IRQ_TRIGGER_RISING_EDGE, 
    QL_IRQ_TRIGGER_FALLING_EDGE, 
} ql_gpio_irq_trigger_e;


ql_gpio_errcode_e ql_gpio_init(ql_gpio_num_e gpio_num, ql_gpio_mode_e mode);
ql_gpio_errcode_e ql_gpio_deinit(ql_gpio_num_e gpio_num);
ql_gpio_errcode_e ql_gpio_set_level(ql_gpio_num_e gpio_num, ql_gpio_output_level_e output_level);
ql_gpio_errcode_e ql_gpio_set_level_reverse(ql_gpio_num_e gpio_num);
ql_gpio_errcode_e ql_gpio_get_level(ql_gpio_num_e gpio_num,UINT32 *input_level);
ql_gpio_errcode_e ql_gpio_int_init(ql_gpio_num_e gpio_num,ql_gpio_irq_trigger_e trigger,ql_gpio_irq_callback calback);
ql_gpio_errcode_e ql_gpio_int_disable(ql_gpio_num_e gpio_num);
void ql_gpio_demo_thread_creat(void);



#endif 

