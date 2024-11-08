/**************************************************************************************************
* include
**************************************************************************************************/
#include "ql_include.h"

#if CFG_ENABLE_QUECTEL_BLE_PAIR
/**************************************************************************************************
* define
**************************************************************************************************/
#define UART_TEST_PORT     QL_UART_PORT_1
#define BLE_RECV_SIZE_MAX  512

#define QL_DEMO_ATT_DECL_PRIMARY_SERVICE_128  {0x00,0x28,0,0,0,0,0,0,0,0,0,0,0,0,0,0}     
#define QL_DEMO_ATT_DECL_CHARACTERISTIC_128   {0x03,0x28,0,0,0,0,0,0,0,0,0,0,0,0,0,0} 
#define QL_DEMO_ATT_DESC_CLIENT_CHAR_CFG_128  {0x02,0x29,0,0,0,0,0,0,0,0,0,0,0,0,0,0}        
#define RX_CHARACTERISTIC_128                 {0x9E,0xCA,0xDC,0x24,0x0E,0xE5,0xA9,0xE0,0x93,0xF3,0xA3,0xB5,0x02,0x00,0x40,0x6E} 
#define TX_CHARACTERISTIC_128                 {0x9E,0xCA,0xDC,0x24,0x0E,0xE5,0xA9,0xE0,0x93,0xF3,0xA3,0xB5,0x03,0x00,0x40,0x6E} 
#define WR_CHARACTERISTIC_128                 {0x9e,0xca,0xdc,0x24,0x0e,0xe5,0xa9,0xe0,0x93,0xf3,0xa3,0xb5,0x3f,0x6c,0x61,0x45}  
static const uint8_t ql_demo_svc_uuid[16] =   {0x33,0XD8,0X7F,0xAB,0x0F,0xE3,0xA9,0XE0,0X93,0xF3,0xA3, 0xB5 ,0x01,0x00,0x40,0x6E};    
/**************************************************************************************************
* enum
**************************************************************************************************/
typedef enum
{
    QL_BLE_PAIR_DEMO_START_ADV,
} ql_ble_pair_demo_msg_id;

enum
{
	QL_DEMO_IDX_SVC_CREATE,
    QL_DEMO_IDX_RX_VAL_CHAR,
    QL_DEMO_IDX_RX_VAL_VALUE,
    QL_DEMO_IDX_RX_VAL_NTF_CFG,
    QL_DEMO_IDX_TX_VAL_CHAR,
    QL_DEMO_IDX_TX_VAL_VALUE,
    QL_DEMO_IDX_WR_VAL_CHAR,
    QL_DEMO_IDX_WR_VAL_VALUE,
    QL_DEMO_IDX_NB,
};   
/**************************************************************************************************
* param
**************************************************************************************************/
ql_queue_t ql_ble_pair_demo_msg_q;
static ql_task_t ble_periphera_hdl = NULL;
uint8_t adv_actv_idx = -1;
uint8_t dev = 0;

ql_attm_desc_t ql_demo_att_db[QL_DEMO_IDX_NB] =
{
	//  Service Declaration
	[QL_DEMO_IDX_SVC_CREATE]     = {QL_DEMO_ATT_DECL_PRIMARY_SERVICE_128, QL_PERM_SET(RD, ENABLE), 0, 0},

    [QL_DEMO_IDX_RX_VAL_CHAR]    = {QL_DEMO_ATT_DECL_CHARACTERISTIC_128,  QL_PERM_SET(RD, ENABLE), 0, 0},
	[QL_DEMO_IDX_RX_VAL_VALUE]   = {RX_CHARACTERISTIC_128,                QL_PERM_SET(NTF, ENABLE), QL_PERM_SET(RI, ENABLE)|QL_PERM_SET(UUID_LEN, UUID_128), 512},
	[QL_DEMO_IDX_RX_VAL_NTF_CFG] = {QL_DEMO_ATT_DESC_CLIENT_CHAR_CFG_128, QL_PERM_SET(RD, ENABLE)|QL_PERM_SET(WRITE_REQ, ENABLE), 0, 0},

    [QL_DEMO_IDX_TX_VAL_CHAR]    = {QL_DEMO_ATT_DECL_CHARACTERISTIC_128,  QL_PERM_SET(RD, ENABLE), 0, 0},
	[QL_DEMO_IDX_TX_VAL_VALUE]   = {TX_CHARACTERISTIC_128,                QL_PERM_SET(WRITE_COMMAND, ENABLE), QL_PERM_SET(RI, ENABLE)|QL_PERM_SET(UUID_LEN, UUID_128), 512},

    [QL_DEMO_IDX_WR_VAL_CHAR]    = {QL_DEMO_ATT_DECL_CHARACTERISTIC_128,  QL_PERM_SET(RD, ENABLE), 0, 0},
	[QL_DEMO_IDX_WR_VAL_VALUE]   = {WR_CHARACTERISTIC_128,                QL_PERM_SET(WRITE_REQ, ENABLE), QL_PERM_SET(RI, ENABLE)|QL_PERM_SET(UUID_LEN, UUID_128), 512},

};

static void ble_periphera_cmd_cb(ble_cmd_t cmd, ble_cmd_param_t *param);
/**************************************************************************************************
* struct
**************************************************************************************************/
typedef struct{
    uint32_t msg_id;
    
    union
    {
        struct
        {       
            uint8_t  conn_handle;
            uint16_t data_len;
            uint8_t  data[BLE_RECV_SIZE_MAX];
        }wr;
        
    }msg_param;

}ql_ble_pair_demo_msg;
/**************************************************************************************************
* Function
**************************************************************************************************/
void ql_ble_pair_demo_msg_send(ql_ble_pair_demo_msg *ble_msg)
{
    if(ql_rtos_queue_release(ql_ble_pair_demo_msg_q, sizeof(ql_ble_pair_demo_msg), (void *)ble_msg, QL_NO_WAIT) != 0)
    {
        os_printf("BLE: %s, error \r\n", __func__);
    }
}

static void ql_demo_periphera_notice_cb(ble_notice_t notice, void *param)
{
    switch (notice)
    {
        case BLE_5_STACK_OK:
        {
            os_printf("ble stack ok");
            break;
        }
        
        case BLE_5_WRITE_EVENT:
        {
            write_req_t *w_req = (write_req_t *)param;
            
            os_printf("write_cb:conn_idx:%d, prf_id:%d, add_id:%d, len:%d, data: \r\n", w_req->conn_idx, w_req->prf_id, w_req->att_idx, w_req->len);
            
            for(uint16_t i=0;i<w_req->len;i++)
            {
                os_printf("%c",w_req->value[i]);                    
            }
            os_printf("\r\n");

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
            os_printf("r_ind:actv_idx:%d, adv_addr:%02x:%02x:%02x:%02x:%02x:%02x\r\n",
                      r_ind->actv_idx, r_ind->adv_addr[0], r_ind->adv_addr[1], r_ind->adv_addr[2],
                      r_ind->adv_addr[3], r_ind->adv_addr[4], r_ind->adv_addr[5]);
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
            os_printf("c_ind:conn_idx:%d, addr_type:%d, peer_addr:%02x:%02x:%02x:%02x:%02x:%02x\r\n",
                      c_ind->conn_idx, c_ind->peer_addr_type, c_ind->peer_addr[0], c_ind->peer_addr[1],
                      c_ind->peer_addr[2], c_ind->peer_addr[3], c_ind->peer_addr[4], c_ind->peer_addr[5]);

            break;
        }
        
        case BLE_5_DISCONNECT_EVENT:
        {
            discon_ind_t *d_ind = (discon_ind_t *)param;
            os_printf("d_ind:conn_idx:%d,reason:%d\r\n", d_ind->conn_idx,d_ind->reason);
            
            ql_ble_pair_demo_msg ble_pair_demo_msg = {0};
            ble_pair_demo_msg.msg_id = QL_BLE_PAIR_DEMO_START_ADV;
            ql_ble_pair_demo_msg_send(&ble_pair_demo_msg);

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

            #if 0
            static uint8_t hid_profile_added = 0;
            if(0 == hid_profile_added)
            {
                app_hid_add_hids();
                hid_profile_added = 1;
            }
            #endif
            
            break;
        }
        
        case BLE_5_INIT_CONNECT_EVENT:
        {
            conn_ind_t *c_ind = (conn_ind_t *)param;
            os_printf("BLE_5_INIT_CONNECT_EVENT:conn_idx:%d, addr_type:%d, peer_addr:%02x:%02x:%02x:%02x:%02x:%02x\r\n",
                      c_ind->conn_idx, c_ind->peer_addr_type, c_ind->peer_addr[0], c_ind->peer_addr[1],
                      c_ind->peer_addr[2], c_ind->peer_addr[3], c_ind->peer_addr[4], c_ind->peer_addr[5]);

            os_printf("ql_ble_get_random_number: %d \r\n", ql_ble_get_random_number());
            
            break;
        }
        
        case BLE_5_INIT_DISCONNECT_EVENT:
        {
            discon_ind_t *d_ind = (discon_ind_t *)param;
            os_printf("BLE_5_INIT_DISCONNECT_EVENT:conn_idx:%d,reason:%d\r\n", d_ind->conn_idx,d_ind->reason);
            break;
        }
        
        case BLE_5_SDP_REGISTER_FAILED:
        {
            os_printf("BLE_5_SDP_REGISTER_FAILED\r\n");
            break;
        }

        case BLE_5_PAIRING_REQ_IND:
        {
            os_printf("BLE_5_PAIRING_REQ_IND \r\n");
            
            uint8_t *conn_idx = (uint8_t *)param;
            
            ql_ble_pairing_rsp(*conn_idx, true);
                
            break;
        }

        case BLE_5_PAIRED:
        {
            pair_ind_t *pair_ind = (pair_ind_t *)param;
            
            os_printf("BLE_5_PAIRED SUCCESS, idx: %d, mac: %02x:%02x:%02x:%02x:%02x:%02x \r\n", pair_ind->idx, pair_ind->mac[5], 
                pair_ind->mac[4], pair_ind->mac[3], pair_ind->mac[2], pair_ind->mac[1], pair_ind->mac[0]);
        
            break;  
        }

        case BLE_5_HIDC_ENC_EVENT:
        {
			bk_printf("BLE_5_HIDC_ENC_EVENT \r\n");

            break;  
        }

        case BLE_5_RAIR_FAILURE:
        {
            pair_fail_ind_t *d_ind = (pair_fail_ind_t *)param;

            os_printf("BLE_5_RAIR_FAILURE , reason = %x\r\n" , d_ind->reason);
            
            break;
        }

        case BLE_5_SEC_NUMERIC_EXCHANGE:
        {
            sec_numeric_exchange_t *num_par = (sec_numeric_exchange_t *)param;
            
    		os_printf("Exchange of numeric value:%d, conn_idx: %d \r\n", num_par->num_value, num_par->conn_idx);

            ql_ble_confirm_reply(num_par->conn_idx, true);
            
            break;
        }
        
        case BLE_5_SEC_BONDLIST_COMPARISON_CMP_IND:
        {
            uint8_t *conn_idx = (uint8_t *)param;
            
			os_printf("BLE BLE_5_SEC_BONDLIST_COMPARISON_CMP_IND, conn_idx=%d\r\n", *conn_idx);

            /* The to_duration parameter range is 1000-30000, in milliseconds */
            ql_ble_security_req(*conn_idx, 30000); //30s
        }
        
        default:
            break;
    }
}

static void ble_periphera_cmd_cb(ble_cmd_t cmd, ble_cmd_param_t *param)
{
    os_printf("cmd:%d idx:%d status:%d\r\n", cmd, param->cmd_idx, param->status);
}

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

static void ql_ble_pair_demo_msg_process(ql_ble_pair_demo_msg *ble_msg)
{
    switch (ble_msg->msg_id)
    {        
        case QL_BLE_PAIR_DEMO_START_ADV:
        {
            os_printf("QL_BLE_PAIR_DEMO_START_ADV \r\n");
            
            ql_ble_start_advertising(adv_actv_idx, 0, ble_periphera_cmd_cb);
            
            break;
        }
        
        default:
            break;
    }
}

static void ql_ble_pair_demo_entry(void *arg)
{
    unsigned char ble_mac[6] = {0};
    unsigned char rev_mac[6] = {0};
    char ble_name[32] = {0};
    uint8_t ble_name_len = 0;
    uint8_t solicitation_uuid[16] = {0xD0,0x00,0x2D,0x12,0x1E,0x4B,0x0F,0xA4,0x99,0x4E,0xCE,0xB5,0x31,0xF4,0x05,0x79};
    ql_ble_pair_demo_msg ble_demo_pair_msg = {0};
    uint8_t peer_addr[6]={0x30,0xbb,0x7d,0x97,0x27,0x0c};
    ql_ble_bonded_device bonded_device[6] = {0};
    
    /* Wait BLE stack initialization complete */
    while (kernel_state_get(TASK_BLE_APP) != APPM_READY)
    {
        ql_rtos_task_sleep_ms(200);
    }
    
    ql_ble_demo_get_mac_address((char *)ble_mac);

    os_printf("BLE MAC PRINT :0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x \n",ble_mac[0],ble_mac[1],ble_mac[2],ble_mac[3],ble_mac[4],ble_mac[5]);
    for (int i = 0; i < 6 ; i++)
    {
        rev_mac[(5 - i)] = ble_mac[i];
        ql_uart_log("BLE MAC :0x%02x \n",ble_mac[5-i]);
    }
    os_printf("BLE MAC REV PRINT :0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x \n",rev_mac[0],rev_mac[1],rev_mac[2],rev_mac[3],rev_mac[4],rev_mac[5]);

    /* Set BLE event callback */
    ql_ble_set_notice_cb(ql_demo_periphera_notice_cb);
    
    ql_ble_bond_delete_all();  //Delete all device pair information

    if(QL_BT_SUCCESS == ql_ble_is_device_bonded(0, peer_addr))
    {
        os_printf("Device is bonded\r\n");
    }
    
    ql_ble_get_bonded_devices(&dev,&bonded_device[0]);
    
    os_printf("Number of bonded devices are %d\r\n",dev);
    for(uint8_t i = 0; i < 6; i++)
    {
        os_printf("Device%d id: %d", i,bonded_device[i].idx);
        os_printf("Address :0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x \r\n",bonded_device[i].peer_addr[0],bonded_device[i].peer_addr[1],bonded_device[i].peer_addr[2],bonded_device[i].peer_addr[3],bonded_device[i].peer_addr[4],bonded_device[i].peer_addr[5]);
    }
    
    ql_ble_security_param_t sec_param = {0};
    sec_param.io_cap = QL_BLE_IO_CAP_NO_INPUT_NO_OUTPUT;
    sec_param.password = 123456;
    sec_param.ble_secure_conn = false;
    sec_param.bond_auth = false;
    ql_ble_security_param_init(&sec_param);

    /* Add BLE profile */
    struct ql_ble_db_cfg ble_db_cfg = {0};
    ble_db_cfg.att_db = ql_demo_att_db;
    ble_db_cfg.att_db_nb = QL_DEMO_IDX_NB;
    ble_db_cfg.prf_task_id = 0;
    ble_db_cfg.start_hdl = 0;
    ble_db_cfg.svc_perm = QL_PERM_SET(SVC_UUID_LEN, UUID_128);
    memcpy(&(ble_db_cfg.uuid[0]), &ql_demo_svc_uuid[0], 16);
    ql_ble_create_db(&ble_db_cfg);

    struct adv_param adv_info = {0};  
    /* Set advertising param */
    adv_info.interval_min = 160;
    adv_info.interval_max = 160;
    adv_info.channel_map = 7;
    adv_info.duration = 0;
    /* Set advertising data */
    memset(ble_name, 0, sizeof(ble_name));
    ble_name_len = snprintf(ble_name, sizeof(ble_name) - 1,  "FC41D_HID");
    adv_info.advData[0] = 0x02; /* Must be set */
    adv_info.advData[1] = 0x01; /* Must be set */
    adv_info.advData[2] = 0x06; /* Must be set */
    adv_info.advData[3] = ble_name_len + 1;
    adv_info.advData[4] = 0x09;
    memcpy(&adv_info.advData[5], ble_name, ble_name_len);
    adv_info.advDataLen = 5 + ble_name_len;
    //TX Power
    adv_info.respData[0] = 0x02;
    adv_info.respData[1] = 0x0A;
    adv_info.respData[2] = 0x08;
    adv_info.respData[3] = 0x11;
    adv_info.respData[4] = 0x15;
    memcpy(&adv_info.respData[5], solicitation_uuid , 16);
    adv_info.respDataLen = 5 + 16;

    /* Get idle actv_idx */
    adv_actv_idx = ql_ble_get_idle_actv_idx_handle();
    if (adv_actv_idx >= BLE_ACTIVITY_MAX)
    {
        return ;
    }
    /* If this actv_ Idx is used for the first time , create it */
    if (app_ble_actv_state_get(adv_actv_idx) == ACTV_IDLE)
    {
        ql_ble_adv_start(adv_actv_idx, &adv_info, ble_periphera_cmd_cb);
    }
    else /*else,start advertising*/
    {
        ql_ble_start_advertising(adv_actv_idx, 0, ble_periphera_cmd_cb);
    }
    
    while (1)
    {
        os_memset((void *)&ble_demo_pair_msg, 0x00, sizeof(ql_ble_pair_demo_msg));
        
        if (ql_rtos_queue_wait(ql_ble_pair_demo_msg_q, (uint8 *)&ble_demo_pair_msg, sizeof(ql_ble_pair_demo_msg), QL_WAIT_FOREVER) == 0)
        {
            ql_ble_pair_demo_msg_process(&ble_demo_pair_msg);
        }
    }
}

void ql_ble_demo_pair_thread_creat(void)
{
    OSStatus err = kNoErr;

    err = ql_rtos_queue_create(&ql_ble_pair_demo_msg_q, sizeof(ql_ble_pair_demo_msg), 10);
    if (err != kNoErr)
    {
        return ;
    }

    err = ql_rtos_task_create(&ble_periphera_hdl,
                                8*1024,
                                THD_EXTENDED_APP_PRIORITY,
                                "ble_demo",
                                ql_ble_pair_demo_entry,
                                0);

    if (err != kNoErr)
    {
        return ;
    }

    return;
}

#endif
