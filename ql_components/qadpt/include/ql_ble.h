#ifndef __QL_API_BLE_H__
#define __QL_API_BLE_H__

#include "app_ble.h"
#include "common_bt_defines.h"

#define QL_BT_ADV_INTERVAL_MIN 	32   //UNIT:0.625MS
#define QL_BT_ADV_INTERVAL_MAX 	0X4000  //UNIT:0.625MS

#define QL_BT_SCAN_INTERVAL_MIN 18   //UNIT:0.625MS
#define QL_BT_SCAN_INTERVAL_MAX 4096  //UNIT:0.625MS


#define QL_BT_SCAN_WINDOW_MIN 17   //UNIT:0.625MS
#define QL_BT_SCAN_WINDOW_MAX 4096  //UNIT:0.625MS

#define QL_BT_CONNECT_INTERVAL_MIN 6   //UNIT:1.25MS
#define QL_BT_CONNECT_INTERVAL_MAX 3200  //UNIT:1.25MS
#define QL_BT_CONNECT_LATENCY_MAX 499   
#define QL_BT_CONNECT_TIMEOUT_MIN 10   //UNIT:10MS
#define QL_BT_CONNECT_TIMEOUT_MAX 3200   //UNIT:10MS

typedef enum {
	QL_BT_SUCCESS = 0,
	QL_BT_ERR_PROFILE,
	QL_BT_ERR_CREATE_DB,
	QL_BT_ERR_CMD_NOT_SUPPORT,
	QL_BT_ERR_UNKNOW_IDX,
	QL_BT_ERR_BLE_STATUS,
	QL_BT_ERR_BLE_PARAM,
	QL_BT_ERR_ADV_DATA,
	QL_BT_ERR_CMD_RUN,
	QL_BT_ERR_NO_MEM,
	QL_BT_ERR_INIT_CREATE,
	QL_BT_ERR_INIT_STATE,

	QL_BT_ERR_ATTC_WRITE,
	QL_BT_ERR_ATTC_WRITE_UNREGISTER,
} ql_errcode_bt_e;

typedef struct
{
	/// 16 bits UUID LSB First
	uint8_t uuid[16];
	/// Attribute Permissions (@see enum attm_perm_mask)
	uint16_t perm;
	/// Attribute Extended Permissions (@see enum attm_value_perm_mask)
	uint16_t ext_perm;
	/// Attribute Max Size
	/// note: for characteristic declaration contains handle offset
	/// note: for included service, contains target service handle
	uint16_t max_size;
}ql_attm_desc_t;

struct ql_ble_db_cfg
{
	uint16_t prf_task_id;
	///Service uuid
	uint8_t uuid[16];
	///Number of db
	uint8_t att_db_nb;
	///Start handler, 0 means autoalloc
	uint16_t start_hdl;
	///Attribute database
	ql_attm_desc_t* att_db;
	///Service config
	uint8_t svc_perm;
};

typedef enum tag_scan_dup_filter_policy
{
    QL_SCAN_FILT_DUPLIC_DIS          = 0x00,
    QL_SCAN_FILT_DUPLIC_EN,
    QL_SCAN_FILT_DUPLIC_EN_PER_PERIOD,
}ql_scan_dup_filter_e;

struct ql_bd_addr
{
    uint8_t  addr[BD_ADDR_LEN];
};

typedef enum {
	QL_ACTV_IDLE,
	QL_ACTV_ADV_CREATED,
	QL_ACTV_ADV_STARTED,
	QL_ACTV_SCAN_CREATED,
	QL_ACTV_SCAN_STARTED,
	QL_ACTV_INIT_CREATED,
	QL_ACTV_PER_SYNC_CREATED,
} ql_actv_state_t;



#define QL_PERM_SET(access, right) \
    (((BK_PERM_RIGHT_ ## right) << (access ## _POS)) & (access ## _MASK))

#define QL_PERM_GET(perm, access)\
    (((perm) & (access ## _MASK)) >> (access ## _POS))

typedef enum
{
  QL_BLE_IO_CAP_DISPLAY_ONLY        = 0x00,  //!< Display Only Device
  QL_BLE_IO_CAP_DISPLAY_YES_NO      = 0x01,  //!< Display and Yes and No Capable
  QL_BLE_IO_CAP_KEYBOARD_ONLY       = 0x02,  //!< Keyboard Only
  QL_BLE_IO_CAP_NO_INPUT_NO_OUTPUT  = 0x03,  //!< No Display or Input Device
  QL_BLE_IO_CAP_KEYBOARD_DISPLAY    = 0x04,  //!< Both Keyboard and Display Capable
}ql_ble_io_cap_e;

typedef struct
{
    bool     ble_secure_conn;   //!< BLE Secure Simple Pairing, also called Secure Connection mode.
    ql_ble_io_cap_e  io_cap;    //!< IO capbilities
    bool     bond_auth;         //!< Bond_auth enable/disable,if true, then will distribute encryption key,and will check this key_req when bonding.
    uint32_t password;          //!< Password.range:[000,000 , 999,999]. The keboard device shall ensure that all 6 digits are inputed 锟斤拷C including zeros.
}ql_ble_security_param_t;


typedef struct
{
    uint8_t idx;
    uint8_t peer_addr_type;
    uint8_t peer_addr[6]; 
}ql_ble_bonded_device;

typedef enum
{
  QL_BLE_ADV_MODE_UNDIRECT               = 0x01,   /// scannalbe & connectable  -- support
  QL_BLE_ADV_MODE_DIRECT                 = 0x02,    /// non-scannalbe & connectable  -- not support
  QL_BLE_ADV_MODE_NON_CONN_NON_SCAN      = 0x03,    /// non-scannalbe & non-connectable  -- not support
  QL_BLE_ADV_MODE_NON_CONN_SCAN          = 0x04,    /// scannalbe & non-connectable -- support
  QL_BLE_ADV_MODE_HDC_DIRECT             = 0x05,    /// non-scannalbe & connectable -- not support
}ql_ble_adv_mode_e;

uint8_t ql_ble_set_device_name(uint8_t* name , uint8_t len);
uint8_t ql_ble_get_device_name(uint8_t* name, uint32_t buf_len);
ql_errcode_bt_e ql_ble_set_tx_power(int8_t tx_power);
ql_errcode_bt_e ql_ble_set_notice_cb(ble_notice_cb_t func);
ql_errcode_bt_e ql_ble_init(void);
ql_errcode_bt_e ql_ble_create_db (struct ql_ble_db_cfg* ble_db_cfg);
uint8_t ql_ble_get_idle_actv_idx_handle(void);
uint8_t ql_ble_get_idle_conn_idx_handle(void);
ql_actv_state_t ql_ble_actv_state_get(uint8_t actv_idx);
ql_errcode_bt_e ql_ble_create_advertising(uint8_t actv_idx,unsigned char chnl_map,uint32_t intv_min,uint32_t intv_max,ble_cmd_cb_t callback);
ql_errcode_bt_e ql_ble_set_adv_data(uint8_t actv_idx, unsigned char* adv_buff, unsigned char adv_len, ble_cmd_cb_t callback);
ql_errcode_bt_e ql_ble_set_scan_rsp_data(uint8_t actv_idx, unsigned char* scan_buff, unsigned char scan_len, ble_cmd_cb_t callback);
ql_errcode_bt_e ql_ble_start_advertising(uint8_t actv_idx, uint16 duration, ble_cmd_cb_t callback);
ql_errcode_bt_e ql_ble_stop_advertising(uint8_t actv_idx, ble_cmd_cb_t callback);
ql_errcode_bt_e ql_ble_adv_start(uint8_t actv_idx, struct adv_param *adv, ble_cmd_cb_t callback);
ql_errcode_bt_e ql_ble_delete_advertising(uint8_t actv_idx, ble_cmd_cb_t callback);
ql_errcode_bt_e ql_ble_create_scaning(uint8_t actv_idx, ble_cmd_cb_t callback);
ql_errcode_bt_e ql_ble_start_scaning(uint8_t actv_idx, uint16_t scan_intv, uint16_t scan_wd, ble_cmd_cb_t callback);
ql_errcode_bt_e ql_ble_scan_start(uint8_t actv_idx, struct scan_param *scan, ble_cmd_cb_t callback);
ql_errcode_bt_e ql_ble_stop_scaning(uint8_t actv_idx, ble_cmd_cb_t callback);
ql_errcode_bt_e ql_ble_delete_scaning(uint8_t actv_idx, ble_cmd_cb_t callback);
ql_errcode_bt_e ql_ble_update_param(uint8_t conn_idx, uint16_t intv_min, uint16_t intv_max,uint16_t latency, uint16_t sup_to, ble_cmd_cb_t callback);
ql_errcode_bt_e ql_ble_gatts_disconnect(uint8_t conn_idx, ble_cmd_cb_t callback);
ql_errcode_bt_e ql_ble_gatt_mtu_changes(uint8_t conn_idx,uint16_t mtu_size,ble_cmd_cb_t callback);
uint16_t ql_ble_gatt_mtu_get(uint8_t conn_idx);
ql_errcode_bt_e ql_ble_create_conn(uint8_t actv_idx,unsigned short con_interval,unsigned short con_latency,unsigned short sup_to,ble_cmd_cb_t callback);
ql_errcode_bt_e ql_ble_set_connect_dev_addr(unsigned char actv_idx,struct ql_bd_addr *bdaddr,unsigned char addr_type);
ql_errcode_bt_e ql_ble_gattc_register_discovery_callback(app_sdp_callback cb);
ql_errcode_bt_e ql_ble_gattc_register_event_recv_callback(app_sdp_charac_callback cb);
ql_errcode_bt_e ql_ble_start_conn(uint8_t actv_idx,ble_cmd_cb_t callback);
ql_errcode_bt_e ql_ble_stop_conn(uint8_t actv_idx,ble_cmd_cb_t callback);
ql_errcode_bt_e ql_ble_gattc_disconnect(uint8_t conn_idx , uint8_t reason);
#if (BLE_APP_PRESENT && (BLE_CENTRAL) && (BLE_SDP_CLIENT))
ql_errcode_bt_e ql_ble_gattc_read_service_data_by_handle_req(uint8_t conidx,uint16_t handle,ble_cmd_cb_t callback);
ql_errcode_bt_e ql_ble_gattc_write_service_data_req(uint8_t conidx,uint16_t handle,uint16_t data_len,uint8_t *data,ble_cmd_cb_t callback);
#endif
ql_errcode_bt_e ql_ble_gatts_send_ntf_value(uint32_t len, uint8_t *buf, uint16_t prf_id, uint16_t att_idx);
ql_errcode_bt_e ql_ble_gatts_send_ind_value(uint32_t len, uint8_t *buf, uint16_t prf_id, uint16_t att_idx);
ql_errcode_bt_e ql_ble_address_get(uint8_t *mac_addr);
ql_errcode_bt_e ql_ble_gattc_ntf_enable(uint8_t conidx,uint16_t handle,bool ntf);
ql_errcode_bt_e ql_ble_gattc_all_service_discovery(uint8_t conidx);
ql_errcode_bt_e ql_ble_security_param_init(ql_ble_security_param_t *param);
/* The to_duration parameter range is 1000-30000, in milliseconds */
ql_errcode_bt_e ql_ble_security_req(uint8_t conidx, uint16_t to_duration);
ql_errcode_bt_e ql_ble_bond_delete_all(void);
ql_errcode_bt_e ql_ble_delete_specified_bound_device(uint8_t idx);
int ql_ble_get_random_number(void);
ql_errcode_bt_e ql_ble_get_bonded_devices(uint8_t *bonded_total, ql_ble_bonded_device *bonded_device);
ql_errcode_bt_e ql_ble_is_device_bonded(uint8_t peer_addr_type, uint8_t *peer_addr);
ql_errcode_bt_e ql_ble_pairing_rsp(uint8_t conn_idx, bool accept);
ql_errcode_bt_e ql_ble_confirm_reply(uint8_t conn_idx, bool accept);

#endif
