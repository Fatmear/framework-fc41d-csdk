/*================================================================
  Copyright (c) 2024, Quectel Wireless Solutions Co., Ltd. All rights reserved.
  Quectel Wireless Solutions Proprietary and Confidential.
=================================================================*/
/**************************************************************************************************
* include
**************************************************************************************************/
#include "ql_include.h"
/**************************************************************************************************
* enum
**************************************************************************************************/
typedef enum
{
    QL_BLE_DEMO_START_SCAN,
    QL_BLE_DEMO_STOP_SCAN,    
    QL_BLE_DEMO_CREATE_CONN,
    QL_BLE_DEMO_CONN_PEER_DEVICE,
    QL_BLE_DEMO_CENTRAL_EXCHANGE_MTU,
    QL_BLE_DEMO_CEN_GATT_NTFCFG,
    QL_BLE_DEMO_CEN_GATT_WRCMD,
    QL_BLE_DEMO_CEN_GATT_WRREQ,
} ql_ble_demo_msg_id;
/**************************************************************************************************
* param
**************************************************************************************************/
typedef struct
{
    uint32_t msg_id;
}ql_ble_demo_msg;

#define QL_BLE_DEMO_QUEUE_MAX_NB      10

#if CFG_ENABLE_QUECTEL_BLE_PERIPHERA
static ql_task_t ble_periphera_hdl = NULL;
int ble_periphera_state = 0;
uint8_t adv_actv_idx = -1;
char ble_name[32] = {0};
struct adv_param g_adv_info = {0};

#define QL_DEMO_ATT_DECL_PRIMARY_SERVICE_128     {0x00,0x28,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
#define QL_DEMO_ATT_DECL_CHARACTERISTIC_128      {0x03,0x28,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
#define QL_DEMO_ATT_DESC_CLIENT_CHAR_CFG_128     {0x02,0x29,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
#define NOTIFY_CHARACTERISTIC_128           {0x15,0xFF,0,0,0x34,0x56,0,0,0,0,0x28,0x37,0,0,0,0}
static const uint8_t ql_demo_svc_uuid[16] = {0x36,0xF5,0,0,0x34,0x56,0,0,0,0,0x28,0x37,0,0,0,0};

enum
{
	QL_DEMO_IDX_SVC,
	QL_DEMO_IDX_FF03_VAL_CHAR,
	QL_DEMO_IDX_FF03_VAL_VALUE,
	QL_DEMO_IDX_FF03_VAL_NTF_CFG,
	QL_DEMO_IDX_NB,
};
ql_attm_desc_t ql_demo_att_db[QL_DEMO_IDX_NB] =
{
	//  Service Declaration
	[QL_DEMO_IDX_SVC]              = {QL_DEMO_ATT_DECL_PRIMARY_SERVICE_128, QL_PERM_SET(RD, ENABLE), 0, 0},

    [QL_DEMO_IDX_FF03_VAL_CHAR]    = {QL_DEMO_ATT_DECL_CHARACTERISTIC_128,  QL_PERM_SET(RD, ENABLE), 0, 0},
	//  Level Characteristic Value
	[QL_DEMO_IDX_FF03_VAL_VALUE]   = {NOTIFY_CHARACTERISTIC_128,   QL_PERM_SET(WRITE_REQ, ENABLE) |    QL_PERM_SET(NTF, ENABLE), QL_PERM_SET(RI, ENABLE)|QL_PERM_SET(UUID_LEN, UUID_16), 512},

	//  Level Characteristic - Client Characteristic Configuration Descriptor

	 [QL_DEMO_IDX_FF03_VAL_NTF_CFG] = {QL_DEMO_ATT_DESC_CLIENT_CHAR_CFG_128, QL_PERM_SET(RD, ENABLE)|QL_PERM_SET(WRITE_REQ, ENABLE), 0, 0},
};
#endif
/**************************************************************************************************
* Function ble test
**************************************************************************************************/
static void ql_ble_demo_notice_cb(ble_notice_t notice, void *param)
{
    switch (notice)
    {
        case BLE_5_STACK_OK:
            os_printf("ble stack ok");
            break;
               
        case BLE_5_WRITE_EVENT:
        {
            write_req_t *w_req = (write_req_t *)param;
            os_printf("write_cb:conn_idx:%d, prf_id:%d, add_id:%d, len:%d, data[0]:%02x\r\n",
                      w_req->conn_idx, w_req->prf_id, w_req->att_idx, w_req->len, w_req->value[0]);
            
            #if CFG_ENABLE_QUECTEL_BLE_PERIPHERA
            // Echo test
            ql_ble_gatts_send_ntf_value(w_req->len , w_req->value , w_req->prf_id , QL_DEMO_IDX_FF03_VAL_VALUE);
            #endif

            break;
        }
        
        case BLE_5_READ_EVENT:
        {
            read_req_t *r_req = (read_req_t *)param;
            os_printf("read_cb:conn_idx:%d, prf_id:%d, add_id:%d\r\n",
                      r_req->conn_idx, r_req->prf_id, r_req->att_idx);
            r_req->value[0] = 0x12;
            r_req->value[1] = 0x34;
            r_req->length = 2;
            break;
        }
        
        case BLE_5_REPORT_ADV:
        {
            recv_adv_t *r_ind = (recv_adv_t *)param;
            int8_t rssi = r_ind->rssi;
            os_printf("r_ind:actv_idx:%d, adv_addr:%02x:%02x:%02x:%02x:%02x:%02x,rssi:%d\r\n",
                      r_ind->actv_idx, r_ind->adv_addr[0], r_ind->adv_addr[1], r_ind->adv_addr[2],
                      r_ind->adv_addr[3], r_ind->adv_addr[4], r_ind->adv_addr[5],rssi);
            
            #if CFG_ENABLE_QUECTEL_BLE_CENTRAL
            extern void app_demo_ble_adv_report_deal(const recv_adv_t * p_adv_report);
            app_demo_ble_adv_report_deal(r_ind);
            #endif

            break;
        }
        
        case BLE_5_MTU_CHANGE:
        {
            mtu_change_t *m_ind = (mtu_change_t *)param;
            os_printf("m_ind:conn_idx:%d, mtu_size:%d\r\n", m_ind->conn_idx, m_ind->mtu_size);
            break;
        }
        
        case BLE_5_CONNECT_EVENT:
        {
            conn_ind_t *c_ind = (conn_ind_t *)param;
            os_printf("BLE_5_CONNECT_EVENT c_ind:conn_idx:%d, addr_type:%d, peer_addr:%02x:%02x:%02x:%02x:%02x:%02x\r\n",
                      c_ind->conn_idx, c_ind->peer_addr_type, c_ind->peer_addr[0], c_ind->peer_addr[1],
                      c_ind->peer_addr[2], c_ind->peer_addr[3], c_ind->peer_addr[4], c_ind->peer_addr[5]);

            break;
        }
        
        case BLE_5_DISCONNECT_EVENT:
        {
            discon_ind_t *d_ind = (discon_ind_t *)param;
            os_printf("BLE_5_DISCONNECT_EVENT, conn_idx:%d, reason:%d \r\n", d_ind->conn_idx, d_ind->reason);
            
            break;
        }
        
        case BLE_5_ATT_INFO_REQ:
        {
            att_info_req_t *a_ind = (att_info_req_t *)param;
            os_printf("a_ind:conn_idx:%d\r\n", a_ind->conn_idx);
            a_ind->length = 128;
            a_ind->status = ERR_SUCCESS;
            break;
        }
        
        case BLE_5_CREATE_DB:
        {
            create_db_t *cd_ind = (create_db_t *)param;
            os_printf("cd_ind:prf_id:%d, status:%d\r\n", cd_ind->prf_id, cd_ind->status);
            break;
        }
        
        case BLE_5_INIT_CONNECT_EVENT:
        {
            conn_ind_t *c_ind = (conn_ind_t *)param;
            os_printf("BLE_5_INIT_CONNECT_EVENT:conn_idx:%d, addr_type:%d, peer_addr:%02x:%02x:%02x:%02x:%02x:%02x\r\n",
                      c_ind->conn_idx, c_ind->peer_addr_type, c_ind->peer_addr[0], c_ind->peer_addr[1],
                      c_ind->peer_addr[2], c_ind->peer_addr[3], c_ind->peer_addr[4], c_ind->peer_addr[5]);

            #if CFG_ENABLE_QUECTEL_BLE_CENTRAL
            ql_ble_gattc_all_service_discovery(c_ind->conn_idx);
            #endif
            break;
        }
                
        case BLE_5_INIT_DISCONNECT_EVENT:
        {
            discon_ind_t *d_ind = (discon_ind_t *)param;
            os_printf("BLE_5_INIT_DISCONNECT_EVENT, conn_idx:%d, reason:%d\r\n", d_ind->conn_idx, d_ind->reason);
            
            #if CFG_ENABLE_QUECTEL_BLE_CENTRAL
            ql_ble_demo_msg ble_demo_msg = {0};
                             
            ble_demo_msg.msg_id = QL_BLE_DEMO_START_SCAN;
            ql_ble_demo_msg_send(&ble_demo_msg);
            #endif
            break;
        }
        
        case BLE_5_SDP_REGISTER_FAILED:
            os_printf("BLE_5_SDP_REGISTER_FAILED\r\n");
            break;
        
        default:
            break;
    }
}

/**************************************************************************************************
* CFG_ENABLE_QUECTEL_BLE_PERIPHERA param
**************************************************************************************************/
#if CFG_ENABLE_QUECTEL_BLE_PERIPHERA

#define QUEC_IBEACON_ENABLE 0

static void ble_periphera_cmd_cb(ble_cmd_t cmd, ble_cmd_param_t *param)
{
    os_printf("cmd:%d idx:%d status:%d\r\n", cmd, param->cmd_idx, param->status);
}

#if (QUEC_IBEACON_ENABLE == 1)
int sec_adv_actv_idx = -1;
struct adv_param g_second_adv_info = {0};
void start_ibeacon_adv(void)
{    
    struct adv_param *sec_adv_info = &g_second_adv_info;
   //second advertising , beacon    
   uint8_t second_adv_data[] =  {        
       0x02,0x01,0x06,        
       0x1A, //length        
       0xFF, //        
       0x4C,0x00, //company : apple,inc.        
       0x02, //type : beacon        
       0x15, // length of data:21bytes        
       0xe2,0xc5,0x6d,0xb5,0xdf,0xfb,0x48,0xd2,0xb0,0x60,0xd0,0xf5,0xa7,0x10,0x96,0xe0, //uuid        
       0x00,0x00, //major        
       0x00,0x00, //minor        
       0xcc, //rssi at 1m    
    };    
       sec_adv_info->interval_min = 160;    
       sec_adv_info->interval_max = 160;    
       sec_adv_info->channel_map = 7;    
       sec_adv_info->duration = 0;    
       sec_adv_info->mode = QL_BLE_ADV_MODE_NON_CONN_SCAN;    
       /******************* *********************/    
       memcpy(sec_adv_info->advData , second_adv_data , sizeof(second_adv_data));    
       sec_adv_info->advDataLen = sizeof(second_adv_data);    
       if(sec_adv_actv_idx == -1)    
       {      
           /* Get idle actv_idx */      
           sec_adv_actv_idx = ql_ble_get_idle_actv_idx_handle();      
           if (sec_adv_actv_idx >= BLE_ACTIVITY_MAX)      
           {          
               return ;      
           }      
           /* If this actv_ Idx is used for the first time , create it */      
           if (app_ble_actv_state_get(sec_adv_actv_idx) == ACTV_IDLE)      
           {          
               ql_ble_adv_start(sec_adv_actv_idx, sec_adv_info,ble_periphera_cmd_cb);      
           }   
        }    
        else    
        {        
            ql_ble_start_advertising(sec_adv_actv_idx, 0, ble_periphera_cmd_cb);    
        }
}

void stop_ibeacon_adv(void)
{  
    ql_ble_stop_advertising(sec_adv_actv_idx, ble_periphera_cmd_cb);
}
#endif

static void ql_ble_demo_get_mac_address(char *ble_mac)
{
    char tmp_mac[6] = {0};
    if(ble_mac == NULL)
    {
        return;
    }
    ql_ble_address_get((char *)&tmp_mac[0]);
    memcpy(ble_mac, tmp_mac, 6);
}

static void ql_ble_periphera_demo_entry(void *arg)
{
    unsigned char ble_mac[6] = {0};
    struct adv_param *adv_info = &g_adv_info;
    int ret = 0;
    
    while (1)
    {
        /* Wait BLE stack initialization complete */
        while (kernel_state_get(TASK_BLE_APP) != APPM_READY)
        {
            ql_rtos_task_sleep_ms(200);
        }

        if(ble_periphera_state == 0)
        {
            /* Set BLE event callback */
            ql_ble_set_notice_cb(ql_ble_demo_notice_cb);

            /* Add BLE profile */
            struct ql_ble_db_cfg ble_db_cfg;
            ble_db_cfg.att_db = ql_demo_att_db;
            ble_db_cfg.att_db_nb = QL_DEMO_IDX_NB;
            ble_db_cfg.prf_task_id = 0;
            ble_db_cfg.start_hdl = 0;
            ble_db_cfg.svc_perm = QL_PERM_SET(SVC_UUID_LEN, UUID_16);
            memcpy(&(ble_db_cfg.uuid[0]), &ql_demo_svc_uuid[0], 16);
            ql_ble_create_db(&ble_db_cfg);

            /* Set advertising param */
            adv_info->interval_min = 160;
            adv_info->interval_max = 160;
            adv_info->channel_map = 7;
            adv_info->duration = 0;

            /* Set advertising data */
            ql_ble_demo_get_mac_address((char *)ble_mac);
            memset(ble_name, 0, sizeof(ble_name));
            ret = snprintf(ble_name, sizeof(ble_name) - 1,
                        "FC41D_%02x:%02x:%02x:%02x:%02x:%02x",
                        ble_mac[0], ble_mac[1], ble_mac[2], ble_mac[3], ble_mac[4], ble_mac[5]);
            adv_info->advData[0] = 0x02;
            adv_info->advData[1] = 0x01;
            adv_info->advData[2] = 0x06;
            adv_info->advData[3] = ret + 1;
            adv_info->advData[4] = 0x09;
            memcpy(&adv_info->advData[5], ble_name, ret);
            adv_info->advDataLen = 5 + ret;

            /* Get idle actv_idx */
            adv_actv_idx = ql_ble_get_idle_actv_idx_handle();
            if (adv_actv_idx >= BLE_ACTIVITY_MAX)
            {
                return ;
            }
            /* If this actv_ Idx is used for the first time , create it */
            if (app_ble_actv_state_get(adv_actv_idx) == ACTV_IDLE)
            {
                ql_ble_adv_start(adv_actv_idx, adv_info, ble_periphera_cmd_cb);
            }
            else /*else,start advertising*/
            {
                ql_ble_start_advertising(adv_actv_idx, 0, ble_periphera_cmd_cb);
            }

#if (QUEC_IBEACON_ENABLE == 1)
            ql_rtos_task_sleep_ms(1000);
            start_ibeacon_adv();
#endif

            ble_periphera_state = 1;
        }
        ql_rtos_task_sleep_ms(20);
    }

    ql_rtos_task_delete(NULL);
}

void ql_ble_demo_periphera_thread_creat(void)
{
    QlOSStatus err = 0;
    // todo  create new thread;
    err = ql_rtos_task_create(&ble_periphera_hdl,
                              8*1024,
                              THD_EXTENDED_APP_PRIORITY,
                              "ble_demo",
                              ql_ble_periphera_demo_entry,
                              0);

    if (err != 0)
    {
        return ;
    }

    return;
}
#endif

/**************************************************************************************************
* CFG_ENABLE_QUECTEL_BLE_CENTRAL include
**************************************************************************************************/
#if CFG_ENABLE_QUECTEL_BLE_CENTRAL

#include "app_sdp.h"
/**************************************************************************************************
* param
**************************************************************************************************/
static ql_task_t ble_central_hdl = NULL;
ql_queue_t ql_ble_demo_msg_q;
uint8_t scan_actv_idx = 0xff;
uint8_t conn_actv_idx = 0xff;
uint8_t conn_handle = 0;
uint16_t chara_handle = 0;
struct ql_bd_addr conn_peer_addr = {0};
uint8_t conn_peer_addr_type = 0; 

/*! Definition of an AD Structure as contained in Advertising and Scan Response packets. An Advertising or Scan Response packet contains several AD Structures. */
typedef struct gapAdStructure_tag
{
    uint8_t  length;     /*!< Total length of the [adType + aData] fields. Equal to 1 + lengthOf(aData). */
    uint8_t  adType;     /*!< AD Type of this AD Structure. */
    uint8_t* aData;      /*!< Data contained in this AD Structure; length of this array is equal to (gapAdStructure_t.length - 1). */
} gapAdStructure_t;

void app_demo_ble_adv_report_deal(const recv_adv_t * p_adv_report)
{
    uint8_t index = 0;
    uint8_t loop_cnt = 0;
  
    while(index < p_adv_report->data_len)
    {
        gapAdStructure_t adElement;
       
        adElement.length = p_adv_report->data[index];
        adElement.adType = p_adv_report->data[index + 1];
        adElement.aData = (uint8_t *)&p_adv_report->data[index + 2];
    
        if(!memcmp(adElement.aData , "phone_server" , strlen("phone_server")))  //check adv name
        {            
            os_printf("Found Device NAME, RSSI:%ddBm, mac:", p_adv_report->rssi);
            
            for(uint8_t i = 0; i < 6; i++)
            {
                os_printf("%02X ", p_adv_report->adv_addr[i]);
            }
            os_printf("\r\n");

            memcpy(&conn_peer_addr.addr[0], p_adv_report->adv_addr, 6);
            conn_peer_addr_type = p_adv_report->adv_addr_type;

            ql_ble_demo_msg ble_demo_msg = {0};
                
            ble_demo_msg.msg_id = QL_BLE_DEMO_STOP_SCAN;
            ql_ble_demo_msg_send(&ble_demo_msg);
            
            return;
        }
        
        /* Move on to the next AD elemnt type */
        index += adElement.length + sizeof(uint8_t);
  
        loop_cnt++;
        if(loop_cnt >= 250)
        break;
    }  
}

void ql_ble_demo_central_cmd_cb(ble_cmd_t cmd, ble_cmd_param_t *param)
{
    os_printf("%s, cmd: [%d], cmd_idx: [%d], status: [%d] \r\n", __func__, cmd, param->cmd_idx, param->status);
    
    if(cmd == BLE_INIT_CREATE)   
    {
        ql_ble_demo_msg ble_demo_msg = {0};
        ble_demo_msg.msg_id = QL_BLE_DEMO_CONN_PEER_DEVICE;
        ql_ble_demo_msg_send(&ble_demo_msg);
    }
    else
    if(cmd == BLE_STOP_SCAN)
    {
        ql_ble_demo_msg ble_demo_msg = {0};
            
        ble_demo_msg.msg_id = QL_BLE_DEMO_CREATE_CONN;
        ql_ble_demo_msg_send(&ble_demo_msg);
    }
}

static void quec_ble_app_sdp_characteristic_cb(unsigned char conidx, uint16_t chars_val_hdl, unsigned char uuid_len, unsigned char *uuid)
{
    uint8_t temp_uuid[16] = {0};
    uint8_t demo_comm_uuid[2] = {0xff, 0x01};
    
    os_printf("[APP]characteristic conidx:%d, handle:0x%02x(%d), uuid_len: %d, UUID:0x", conidx, chars_val_hdl, chars_val_hdl, uuid_len);
    
    for(int i = 0; i < uuid_len; i++)
    {
        temp_uuid[i] = uuid[uuid_len-1-i];
        os_printf("%02x ", uuid[i]);
    }
    
    os_printf("\r\n");

    if((uuid_len == 2) && !os_memcmp((const void *)demo_comm_uuid, (const void *)temp_uuid, 2))
    {
        chara_handle = chars_val_hdl;
        conn_handle  = conidx;
        
        os_printf("start QL_BLE_DEMO_CEN_GATT_NTFCFG \r\n");
        
        ql_ble_demo_msg ble_demo_msg = {0};
            
        ble_demo_msg.msg_id = QL_BLE_DEMO_CEN_GATT_NTFCFG;
        ql_ble_demo_msg_send(&ble_demo_msg);
    }
}

//receive slave data
void quec_app_sdp_charac_cb(CHAR_TYPE type, uint8 conidx, uint16_t hdl,uint16_t len, uint8 *data)
{
    os_printf("[APP]type:%x, conidx:%d, handle:0x%02x(%d), len:%d, 0x", type, conidx, hdl, hdl, len);
    
    for(int i = 0; i< len; i++)
    {
        os_printf("%02x ",data[i]);
    }
    
    os_printf("\r\n");

    if(hdl == chara_handle)
    {
        ql_ble_demo_msg ble_demo_msg = {0};
            
        ble_demo_msg.msg_id = QL_BLE_DEMO_CEN_GATT_WRREQ;
        ql_ble_demo_msg_send(&ble_demo_msg);
    }
}
  
static void ql_ble_demo_msg_process(ql_ble_demo_msg *ble_msg)
{
    switch (ble_msg->msg_id)
    {        
        case QL_BLE_DEMO_START_SCAN:
        {
            struct scan_param scan_info = {0};
                
            /* Set scan param */
            scan_info.channel_map = 0x07;
            scan_info.interval = 0x50;
            scan_info.window = 0x20;
            
            /* Get idle actv_idx */
            if(scan_actv_idx == 0xff)
            {
                scan_actv_idx = ql_ble_get_idle_actv_idx_handle();
                if (scan_actv_idx >= BLE_ACTIVITY_MAX)
                {
                    return ;
                }
            }
            
            /* If this actv_Idx is used for the first time . Start after creating it*/
            if (app_ble_actv_state_get(scan_actv_idx) == ACTV_IDLE)
            {
                ql_ble_scan_start(scan_actv_idx, &scan_info, NULL);
            }
            else /*else,start advertising*/
            {
                ql_ble_start_scaning(scan_actv_idx, scan_info.interval, scan_info.window, NULL);
            }
            
            break;
        }

        case QL_BLE_DEMO_STOP_SCAN:
        {
            ql_ble_stop_scaning(scan_actv_idx, ql_ble_demo_central_cmd_cb);
            
            break;
        }

        case QL_BLE_DEMO_CREATE_CONN:
        {         
            os_printf("QL_BLE_DEMO_CREATE_CONN, conn_actv_idx = %d\r\n" , conn_actv_idx);
            
            if(conn_actv_idx == 0xFF)
            {
                ql_ble_gattc_register_discovery_callback(quec_ble_app_sdp_characteristic_cb);
                ql_ble_gattc_register_event_recv_callback(quec_app_sdp_charac_cb);
                
                conn_actv_idx = ql_ble_get_idle_conn_idx_handle();
                
                os_printf("conn_actv_idx = %d\r\n" , conn_actv_idx);
                
                if (conn_actv_idx >= BLE_ACTIVITY_MAX)
                {
                    return;
                }
                            
                /* actv_Idx is used for the first time , create it */
                ql_ble_create_conn(conn_actv_idx, 0X30, 0,500, ql_ble_demo_central_cmd_cb);
            }
            else
            {                
                ql_ble_demo_msg ble_demo_msg = {0};
                ble_demo_msg.msg_id = QL_BLE_DEMO_CONN_PEER_DEVICE;
                ql_ble_demo_msg_send(&ble_demo_msg);
            }

            break;
        }

        case QL_BLE_DEMO_CONN_PEER_DEVICE:
        {       
            ql_errcode_bt_e ret = 0;

            os_printf("set connect address, conn_actv_idx:%d.%d \r\n", conn_actv_idx, BLE_APP_MASTER_GET_IDX_STATE(conn_actv_idx));
            
            ret = ql_ble_set_connect_dev_addr(conn_actv_idx, &conn_peer_addr, conn_peer_addr_type); 
            
            os_printf("set connect address, ret: %d \r\n", ret);
            
            
            os_printf("start connect.... , conn_actv_idx:%d.%d \r\n", conn_actv_idx, BLE_APP_MASTER_GET_IDX_STATE(conn_actv_idx));
            ret = ql_ble_start_conn(conn_actv_idx, NULL);
            
            os_printf("start connect .... ret: %d \r\n", ret);
                        
            break;
        }

        case QL_BLE_DEMO_CENTRAL_EXCHANGE_MTU:
        {
            break;
        }

        case QL_BLE_DEMO_CEN_GATT_NTFCFG:
        {            
            os_printf("QL_BLE_DEMO_CEN_GATT_NTFCFG, conn_handle: [%d], chara_handle: [%d] \r\n", conn_handle, chara_handle);
            ql_rtos_task_sleep_ms(200);            
            ql_ble_gattc_ntf_enable(conn_handle, chara_handle, true);

            break;
        }
        
        case QL_BLE_DEMO_CEN_GATT_WRREQ:
        {
            os_printf("QL_BLE_DEMO_CEN_GATT_WRREQ, conn_handle: [%d], chara_handle: [%d] \r\n", conn_handle, chara_handle);
            
            uint16_t wr_demo_data_len = 6;
            uint8_t  wr_demo_data[7] = "123456";
            
            ql_ble_gattc_write_service_data_req(conn_handle, chara_handle, wr_demo_data_len, wr_demo_data, NULL);

            break;
        }
                
        default:
            break;
    }
}

void ql_ble_demo_msg_send(ql_ble_demo_msg *ble_msg)
{
    if(ql_rtos_queue_release(ql_ble_demo_msg_q, sizeof(ql_ble_demo_msg), (void *)ble_msg, QL_NO_WAIT) != 0)
    {
        os_printf("BLE: %s, error \r\n", __func__);
    }
}

static void ql_ble_central_demo_entry(void *arg)
{    
    ql_ble_demo_msg ble_demo_msg;

    /* Wait BLE stack initialization complete */
     while (kernel_state_get(TASK_BLE_APP) != APPM_READY)
     {
         ql_rtos_task_sleep_ms(200);
     }
     
     /* Set ble event callback */
     ql_ble_set_notice_cb(ql_ble_demo_notice_cb);
     
     /* ble initialization*/
     ql_ble_init();
         
    ble_demo_msg.msg_id = QL_BLE_DEMO_START_SCAN;
    ql_ble_demo_msg_send(&ble_demo_msg);
        
    while (1)
    {
        os_memset((void *)&ble_demo_msg, 0x00, sizeof(ql_ble_demo_msg));
        
        if (ql_rtos_queue_wait(ql_ble_demo_msg_q, (uint8 *)&ble_demo_msg, sizeof(ble_demo_msg), QL_WAIT_FOREVER) == 0)
        {
            ql_ble_demo_msg_process(&ble_demo_msg);
        }
    }
}

void ql_ble_demo_central_thread_creat(void)
{
    QlOSStatus ret = 0;

    ret = ql_rtos_queue_create(&ql_ble_demo_msg_q, sizeof(ql_ble_demo_msg),  QL_BLE_DEMO_QUEUE_MAX_NB);
	
	if(ret != 0)
    {
		os_printf("Failed to Create BLE msg queue\n");
		return;
	}
    
    // todo  create new thread;
    ret = ql_rtos_task_create(&ble_central_hdl,
                              8*1024,
                              THD_EXTENDED_APP_PRIORITY,
                              "ble_central_demo",
                              ql_ble_central_demo_entry,
                              0);

    if (ret != 0)
    {
        os_printf("Error: Failed to create ble thread:%d\n");
        
        if (ql_ble_demo_msg_q != NULL)
        {
            ql_rtos_queue_delete(ql_ble_demo_msg_q);
            ql_ble_demo_msg_q = NULL;
        }
        
        if (ble_central_hdl != NULL)
        {
            ql_rtos_task_delete(ble_central_hdl);
            ble_central_hdl = NULL;
        }
        return ;
    }

    return;
}

#endif
