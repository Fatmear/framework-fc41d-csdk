#ifndef _QL_ADC_H
#define _QL_ADC_H


#define ADC_TEST_DEBUG
#ifdef ADC_TEST_DEBUG
#define ql_adc_log               os_printf

#else
#define ql_adc_log               os_null_printf
#endif

#define ADC_TEST_CHANNEL   QL_ADC_CHANNEL_1


typedef void (*ql_adc_obj_callback)(int new_mv, void *user_data);


typedef enum
{
	 QL_ADC_SUCCESS = 0,
	 QL_ADC_EXECUTE_ERR,
	 QL_ADC_INVALID_PARAM_ERR,
} ql_adc_errcode_e;


typedef enum
{
	QL_ADC_CHANNEL_0 = 0, 
	QL_ADC_CHANNEL_1,
	QL_ADC_CHANNEL_2,
	QL_ADC_CHANNEL_3,
	QL_ADC_CHANNEL_4,
	QL_ADC_CHANNEL_5,
	QL_ADC_CHANNEL_6,
}ql_adc_channel_e;




typedef struct adc_obj_ {
	void *user_data;
    ql_adc_channel_e channel;
	ql_adc_obj_callback cb;
	struct adc_obj_ *next;
}ql_adc_obj_s;



ql_adc_errcode_e ql_adc_thread_init(void);
ql_adc_errcode_e ql_adc_channel_init(ql_adc_obj_s *handle, ql_adc_obj_callback cb, ql_adc_channel_e channel, void *user_data);
ql_adc_errcode_e ql_adc_channel_start(ql_adc_obj_s *handle);
ql_adc_errcode_e ql_adc_channel_stop(ql_adc_obj_s *handle);

void ql_adc_demo_thread_creat(void);



#endif 

