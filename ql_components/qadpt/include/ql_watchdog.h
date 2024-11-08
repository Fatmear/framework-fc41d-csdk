#ifndef _QL_WATCHDOG_H
#define _QL_WATCHDOG_H


typedef enum
{
	 QL_WDG_SUCCESS = 0,
	 QL_WDG_EXECUTE_ERR,
	 QL_WDG_INVALID_PARAM_ERR,
} ql_wdg_errcode_e;

ql_wdg_errcode_e ql_WdgInit(UINT32 timeout);
void ql_wdg_reload(void);
void ql_wdg_finalize(void);


#endif 

