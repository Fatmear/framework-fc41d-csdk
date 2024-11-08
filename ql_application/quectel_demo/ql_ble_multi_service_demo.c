#include "ql_include.h"


/***************ble test******************/
#if CFG_ENABLE_QUECTEL_BLE_MULTI_SERVICE
static ql_task_t ble_periphera_hdl = NULL;
int ble_periphera_state = 0;
uint8_t adv_actv_idx = -1;
char ble_name[32] = {0};
struct adv_param g_adv_info = {0};

#define QL_DEMO_ATT_DECL_PRIMARY_SERVICE0_128     {0x00,0x28,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
#define QL_DEMO_ATT_DECL_CHARACTERISTIC0_128      {0x03,0x28,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
#define QL_DEMO_ATT_DESC_CLIENT_CHAR_CFG0_128     {0x02,0x29,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
#define NOTIFY_CHARACTERISTIC0_128           {0x15,0xFF,0,0,0x34,0x56,0,0,0,0,0x28,0x37,0,0,0,0}

#define QL_DEMO_ATT_DECL_PRIMARY_SERVICE1_128     {0x00,0x28,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
#define QL_DEMO_ATT_DECL_CHARACTERISTIC1_128      {0x03,0x28,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
#define QL_DEMO_ATT_DESC_CLIENT_CHAR_CFG1_128     {0x02,0x29,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
#define NOTIFY_CHARACTERISTIC1_128           {0x16,0xFF,0,0,0x34,0x56,0,0,0,0,0x28,0x37,0,0,0,0}

#define QL_DEMO_ATT_DECL_PRIMARY_SERVICE2_128     {0x00,0x28,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
#define QL_DEMO_ATT_DECL_CHARACTERISTIC2_128      {0x03,0x28,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
#define QL_DEMO_ATT_DESC_CLIENT_CHAR_CFG2_128     {0x02,0x29,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
#define NOTIFY_CHARACTERISTIC2_128           {0x17,0xFF,0,0,0x34,0x56,0,0,0,0,0x28,0x37,0,0,0,0}

// static const uint8_t ql_demo_svc_uuid[16] = {0x36,0xF5,0,0,0x34,0x56,0,0,0,0,0x28,0x37,0,0,0,0};
static const uint8_t ql_demo_svc0_uuid[16] = {0x36,0xF5,0,0,0x34,0x56,0,0,0,0,0x28,0x37,0,0,0,0};
static const uint8_t ql_demo_svc1_uuid[16] = {0x37,0xF5,0,0,0x34,0x56,0,0,0,0,0x28,0x37,0,0,0,0};
static const uint8_t ql_demo_svc2_uuid[16] = {0x38,0xF5,0,0,0x34,0x56,0,0,0,0,0x28,0x37,0,0,0,0};
enum
{
	QL_DEMO_IDX_SVC0,
	QL_DEMO_IDX_FF03_VAL_CHAR0,
	QL_DEMO_IDX_FF03_VAL_VALUE0,
	QL_DEMO_IDX_FF03_VAL_NTF_CFG0,
	QL_DEMO_IDX_NB0,
};
enum
{
	QL_DEMO_IDX_SVC1,
	QL_DEMO_IDX_FF03_VAL_CHAR1,
	QL_DEMO_IDX_FF03_VAL_VALUE1,
	QL_DEMO_IDX_FF03_VAL_NTF_CFG1,
	QL_DEMO_IDX_NB1,
};
enum
{
	QL_DEMO_IDX_SVC2,
	QL_DEMO_IDX_FF03_VAL_CHAR2,
	QL_DEMO_IDX_FF03_VAL_VALUE2,
	QL_DEMO_IDX_FF03_VAL_NTF_CFG2,
	QL_DEMO_IDX_NB2,
};
ql_attm_desc_t ql_demo_att_db0[QL_DEMO_IDX_NB0] =
{
	//  Service Declaration
	[QL_DEMO_IDX_SVC0]              = {QL_DEMO_ATT_DECL_PRIMARY_SERVICE0_128, QL_PERM_SET(RD, ENABLE), 0, 0},

    [QL_DEMO_IDX_FF03_VAL_CHAR0]    = {QL_DEMO_ATT_DECL_CHARACTERISTIC0_128,  QL_PERM_SET(RD, ENABLE), 0, 0},
	//  Level Characteristic Value
	[QL_DEMO_IDX_FF03_VAL_VALUE0]   = {NOTIFY_CHARACTERISTIC0_128,   QL_PERM_SET(WRITE_REQ, ENABLE) |    QL_PERM_SET(NTF, ENABLE), QL_PERM_SET(RI, ENABLE)|QL_PERM_SET(UUID_LEN, UUID_16), 512},

	//  Level Characteristic - Client Characteristic Configuration Descriptor

	 [QL_DEMO_IDX_FF03_VAL_NTF_CFG0] = {QL_DEMO_ATT_DESC_CLIENT_CHAR_CFG0_128, QL_PERM_SET(RD, ENABLE)|QL_PERM_SET(WRITE_REQ, ENABLE), 0, 512},
};

ql_attm_desc_t ql_demo_att_db1[QL_DEMO_IDX_NB1] =
{
	//  Service Declaration
	[QL_DEMO_IDX_SVC1]              = {QL_DEMO_ATT_DECL_PRIMARY_SERVICE1_128, QL_PERM_SET(RD, ENABLE), 0, 0},

    [QL_DEMO_IDX_FF03_VAL_CHAR1]    = {QL_DEMO_ATT_DECL_CHARACTERISTIC1_128,  QL_PERM_SET(RD, ENABLE), 0, 0},
	//  Level Characteristic Value
	[QL_DEMO_IDX_FF03_VAL_VALUE1]   = {NOTIFY_CHARACTERISTIC1_128,   QL_PERM_SET(WRITE_REQ, ENABLE) |    QL_PERM_SET(NTF, ENABLE), QL_PERM_SET(RI, ENABLE)|QL_PERM_SET(UUID_LEN, UUID_16), 512},

	//  Level Characteristic - Client Characteristic Configuration Descriptor

	 [QL_DEMO_IDX_FF03_VAL_NTF_CFG1] = {QL_DEMO_ATT_DESC_CLIENT_CHAR_CFG1_128, QL_PERM_SET(RD, ENABLE)|QL_PERM_SET(WRITE_REQ, ENABLE), 0, 512},
};

ql_attm_desc_t ql_demo_att_db2[QL_DEMO_IDX_NB2] =
{
	//  Service Declaration
	[QL_DEMO_IDX_SVC2]              = {QL_DEMO_ATT_DECL_PRIMARY_SERVICE2_128, QL_PERM_SET(RD, ENABLE), 0, 0},

    [QL_DEMO_IDX_FF03_VAL_CHAR2]    = {QL_DEMO_ATT_DECL_CHARACTERISTIC2_128,  QL_PERM_SET(RD, ENABLE), 0, 0},
	//  Level Characteristic Value
	[QL_DEMO_IDX_FF03_VAL_VALUE2]   = {NOTIFY_CHARACTERISTIC2_128,   QL_PERM_SET(WRITE_REQ, ENABLE) |    QL_PERM_SET(NTF, ENABLE), QL_PERM_SET(RI, ENABLE)|QL_PERM_SET(UUID_LEN, UUID_16), 512},

	//  Level Characteristic - Client Characteristic Configuration Descriptor

	 [QL_DEMO_IDX_FF03_VAL_NTF_CFG2] = {QL_DEMO_ATT_DESC_CLIENT_CHAR_CFG2_128, QL_PERM_SET(RD, ENABLE)|QL_PERM_SET(WRITE_REQ, ENABLE), 0, 512},
};

uint8_t g_ucProfileId = 0;
void app_ble_service_add(void)
{

    if(g_ucProfileId >= 3)
        return ;
    
    if(g_ucProfileId == 0)
    {
        struct ql_ble_db_cfg ble_db_cfg0;
        ble_db_cfg0.att_db = ql_demo_att_db0;
        ble_db_cfg0.att_db_nb = QL_DEMO_IDX_NB0;
        ble_db_cfg0.prf_task_id = 0;
        ble_db_cfg0.start_hdl = 0;
        ble_db_cfg0.svc_perm = QL_PERM_SET(SVC_UUID_LEN, UUID_16);
        memcpy(&(ble_db_cfg0.uuid[0]), &ql_demo_svc0_uuid[0], 16);
        ql_ble_create_db(&ble_db_cfg0);
    }
    else if(g_ucProfileId == 1)
    {
        struct ql_ble_db_cfg ble_db_cfg1;
        ble_db_cfg1.att_db = ql_demo_att_db1;
        ble_db_cfg1.att_db_nb = QL_DEMO_IDX_NB1;
        ble_db_cfg1.prf_task_id = 1;
        ble_db_cfg1.start_hdl = 0;
        ble_db_cfg1.svc_perm = QL_PERM_SET(SVC_UUID_LEN, UUID_16);
        memcpy(&(ble_db_cfg1.uuid[0]), &ql_demo_svc1_uuid[0], 16);
        ql_ble_create_db(&ble_db_cfg1);
    }
    else if(g_ucProfileId == 2)
    {
        struct ql_ble_db_cfg ble_db_cfg2;
        ble_db_cfg2.att_db = ql_demo_att_db2;
        ble_db_cfg2.att_db_nb = QL_DEMO_IDX_NB2;
        ble_db_cfg2.prf_task_id = 2;
        ble_db_cfg2.start_hdl = 0;
        ble_db_cfg2.svc_perm = QL_PERM_SET(SVC_UUID_LEN, UUID_16);
        memcpy(&(ble_db_cfg2.uuid[0]), &ql_demo_svc2_uuid[0], 16);
        ql_ble_create_db(&ble_db_cfg2);
    }

    g_ucProfileId++; 

}

static void ql_demo_periphera_notice_cb(ble_notice_t notice, void *param)
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
#if 1
        // Echo test
        if(w_req->prf_id == 0)
        {
            ql_ble_gatts_send_ntf_value(w_req->len , w_req->value , w_req->prf_id , QL_DEMO_IDX_FF03_VAL_VALUE0);
        }
        else if(w_req->prf_id == 1)
        {
            ql_ble_gatts_send_ntf_value(w_req->len , w_req->value , w_req->prf_id , QL_DEMO_IDX_FF03_VAL_VALUE1);
        }
        else if(w_req->prf_id == 2)
        {
            ql_ble_gatts_send_ntf_value(w_req->len , w_req->value , w_req->prf_id , QL_DEMO_IDX_FF03_VAL_VALUE2);
        }
                
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
        app_ble_service_add();
        break;
    }
    case BLE_5_INIT_CONNECT_EVENT:
    {
        conn_ind_t *c_ind = (conn_ind_t *)param;
        os_printf("BLE_5_INIT_CONNECT_EVENT:conn_idx:%d, addr_type:%d, peer_addr:%02x:%02x:%02x:%02x:%02x:%02x\r\n",
                  c_ind->conn_idx, c_ind->peer_addr_type, c_ind->peer_addr[0], c_ind->peer_addr[1],
                  c_ind->peer_addr[2], c_ind->peer_addr[3], c_ind->peer_addr[4], c_ind->peer_addr[5]);
        break;
    }
    case BLE_5_INIT_DISCONNECT_EVENT:
    {
        discon_ind_t *d_ind = (discon_ind_t *)param;
        os_printf("BLE_5_INIT_DISCONNECT_EVENT:conn_idx:%d,reason:%d\r\n", d_ind->conn_idx,d_ind->reason);
        break;
    }
    case BLE_5_SDP_REGISTER_FAILED:
        os_printf("BLE_5_SDP_REGISTER_FAILED\r\n");
        break;
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




static void ql_ble_periphera_demo_entry(void *arg)
{
    uint8_t ble_mac[6] = {0};
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
            ql_ble_set_notice_cb(ql_demo_periphera_notice_cb);

            /* Add BLE profile */
            app_ble_service_add();

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

            ble_periphera_state = 1;
        }
        ql_rtos_task_sleep_ms(20);
    }

    ql_rtos_task_delete(NULL);
}

void ql_ble_demo_multi_service_thread_creat(void)
{
    OSStatus err = kNoErr;
    // todo  create new thread;
    err = ql_rtos_task_create(&ble_periphera_hdl,
                                8*1024,
                                THD_EXTENDED_APP_PRIORITY,
                                "ble_multi_service_demo",
                                ql_ble_periphera_demo_entry,
                                0);

    if (err != kNoErr)
    {
        return ;
    }

    return;
}


#endif

