/**
 ****************************************************************************************
 *
 * @file rwapp_config.h
 *
 * @brief Application configuration definition
 *
 * Copyright (C) RivieraWaves 2009-2016
 *
 ****************************************************************************************
 */


#ifndef _RWAPP_CONFIG_H_
#define _RWAPP_CONFIG_H_

/**
 ****************************************************************************************
 * @addtogroup app
 * @brief Application configuration definition
 *
 * @{
 ****************************************************************************************
 */


/*
 * INCLUDE FILES
 ****************************************************************************************
 */

/*
 * DEFINES
 ****************************************************************************************
 */
// 	<e> BLE_APP_COMM
// 	<i> BLE_APP_COMM enable /disable 
//  </e>
#if ( 1 )
#define CFG_APP_COMM
#endif

#if ( 1 )
#define CFG_PRF_HID
#endif


#define BLE_HID_DEVICE            1

#define BLE_APP_HID          1

/// COMM Service Application
#if defined(CFG_APP_COMM)
#define BLE_APP_COMM               1
#else
#define BLE_APP_COMM               0
#endif // defined(BLE_APP_COMM)
/******************************************************************************************/
/* -------------------------   BLE APPLICATION SETTINGS      -----------------------------*/
/******************************************************************************************/

#endif /* _RWAPP_CONFIG_H_ */
