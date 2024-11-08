
#include "ql_include.h"


/***************ble test******************/
#if CFG_ENABLE_QUECTEL_BLE_AMS_ANCS

typedef enum
{
    QL_BLE_ANCS_DEMO_START_ADV,
    QL_BLE_ANCS_DEMO_SECURITY_REQ,
    QL_BLE_ANCS_DEMO_DIS_ALL_SRV,
    QL_BLE_ANCS_DEMO_GATTC_NTF_ENABLE, 
    QL_BLE_ANCS_DEMO_GATTC_WR,
} ql_ble_ancs_demo_msg_id;

typedef struct{
    uint32_t msg_id;
    
    union
    {
        struct
        {       
            uint8_t  conn_handle;
        }sec_req;
        
        struct
        {       
            uint16_t chara_hdl;
        }ntf_enable;
        
    }msg_param;

}ql_ble_ancs_demo_msg;

ql_queue_t ql_ble_ancs_demo_msg_q;

uint8_t conn_hdl = 0;
uint8_t wr_count = 0;
static ql_task_t ble_periphera_hdl = NULL;
//int ble_periphera_state = 0;
uint8_t adv_actv_idx = -1;
char ble_name[32] = {0};
struct adv_param g_adv_info = {0};
const char *rsp_data = "QUECTEL_AMS";
static uint16_t ams_remote_command_handle = 0, ams_entity_update_handle = 0, ams_entity_att = 0;
static uint16_t ancs_ntf_s_handle = 0, ancs_control_point_handle = 0, ancs_data_s_hanlde = 0;
static void ble_periphera_cmd_cb(ble_cmd_t cmd, ble_cmd_param_t *param);

#define CONN_IDX_UINIT  0xFF

///ANCS SERVICE
#define ANCS_SERVICE_UUID           {0xD0,0x00,0x2D,0x12,0x1E,0x4B,0x0F,0xA4,0x99,0x4E,0xCE,0xB5,0x31,0xF4,0x05,0x79}
#define ANCS_CHAR_CONTROL_POINT     {0xD9,0xD9,0xAA,0xFD,0xBD,0x9B,0x21,0x98,0xA8,0x49,0xE1,0x45,0xF3,0xD8,0xD1,0x69}
#define ANCS_CHAR_DATA_SOURCE       {0xFB,0x7B,0x7C,0xCE,0x6A,0xB3,0x44,0xBE,0xB5,0x4B,0xD6,0x24,0xE9,0xC6,0xEA,0x22}  //1
#define ANCS_CHAR_NTF_SOURCE        {0xBD,0x1D,0xA2,0x99,0xE6,0x25,0x58,0x8C,0xD9,0x42,0x01,0x63,0x0D,0x12,0xBF,0x9F}  //2
///AMS SERVICE
#define AMS_SERVICE_UUID           {0xDC,0xF8,0x55,0xAD,0x02,0xC5,0xF4,0x8E,0x3A,0x43,0x36,0x0F,0x2B,0x50,0xD3,0x89}
#define AMS_CHAR_REMOTE_COMMAND     {0xC2,0x51,0xCA,0xF7,0x56,0x0E,0xDF,0xB8,0x8A,0x4A,0xB1,0x57,0xD8,0x81,0x3C,0x9B}  //3
#define AMS_CHAR_ENTITY_UPDATE      {0x02,0xC1,0x96,0xBA,0x92,0xBB,0x0C,0x9A,0x1F,0x41,0x8D,0x80,0xCE,0xAB,0x7C,0x2F}  //4
#define AMS_CHAR_ENTITY_ATTRIBUTE   {0xD7,0xD5,0xBB,0x70,0xA8,0xA3,0xAB,0xA6,0xD8,0x46,0xAB,0x23,0x8C,0xF3,0xB2,0xC6}

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
	[QL_DEMO_IDX_FF03_VAL_VALUE]   = {NOTIFY_CHARACTERISTIC_128,   QL_PERM_SET(WRITE_REQ, ENABLE) |    QL_PERM_SET(NTF, ENABLE), QL_PERM_SET(RI, ENABLE)|QL_PERM_SET(UUID_LEN, UUID_128), 512},

	//  Level Characteristic - Client Characteristic Configuration Descriptor

	 [QL_DEMO_IDX_FF03_VAL_NTF_CFG] = {QL_DEMO_ATT_DESC_CLIENT_CHAR_CFG_128, QL_PERM_SET(RD, ENABLE)|QL_PERM_SET(WRITE_REQ, ENABLE), 0, 0},
};

enum {
    RemoteCommandIDPlay,
    RemoteCommandIDPause,
    RemoteCommandIDTogglePlayPause,
    RemoteCommandIDNextTrack,
    RemoteCommandIDPreviousTrack,
    RemoteCommandIDVolumeUp,
    RemoteCommandIDVolumeDown,
    RemoteCommandIDAdvanceRepeatMode,
    RemoteCommandIDAdvanceShuffleMode,
    RemoteCommandIDSkipForward,
    RemoteCommandIDSkipBackward,
    RemoteCommandIDLikeTrack,
    RemoteCommandIDDislikeTrack,
    RemoteCommandIDBookmarkTrack,
};

void ql_ble_ancs_demo_msg_send(ql_ble_ancs_demo_msg *ble_msg)
{
    if(ql_rtos_queue_release(ql_ble_ancs_demo_msg_q, sizeof(ql_ble_ancs_demo_msg), (void *)ble_msg, QL_NO_WAIT) != 0)
    {
        os_printf("BLE: %s, error \r\n", __func__);
    }
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
        
        for(uint16_t i = 0 ; i < w_req->len ; i++)
        {
          os_printf("%02x " , w_req->value[i]);
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
        
        conn_hdl = c_ind->conn_idx;

        break;
    }
    case BLE_5_DISCONNECT_EVENT:
    {
        discon_ind_t *d_ind = (discon_ind_t *)param;
        os_printf("d_ind:conn_idx:%d,reason:%d\r\n", d_ind->conn_idx,d_ind->reason);
        wr_count = 0;

        ql_ble_ancs_demo_msg ble_ancs_demo_msg = {0};
        ble_ancs_demo_msg.msg_id = QL_BLE_ANCS_DEMO_START_ADV;
        ql_ble_ancs_demo_msg_send(&ble_ancs_demo_msg);

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

#if 0   //
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

    case BLE_5_PAIRING_REQ_IND:
    {
        os_printf("BLE_5_PAIRING_REQ_IND \r\n");
        
        uint8_t *conn_idx = (uint8_t *)param;
        
        ql_ble_pairing_rsp(*conn_idx, true);
            
        break;
    }

    case BLE_5_PAIRED:
    {
        os_printf("BLE_5_PAIRED\r\n");
            
        ql_ble_ancs_demo_msg ble_ancs_demo_msg = {0};
        ble_ancs_demo_msg.msg_id = QL_BLE_ANCS_DEMO_DIS_ALL_SRV;
        ql_ble_ancs_demo_msg_send(&ble_ancs_demo_msg);
    }break;

    case BLE_5_RAIR_FAILURE:
    {
        pair_fail_ind_t *d_ind = (pair_fail_ind_t *)param;

        os_printf("BLE_5_RAIR_FAILURE , reason = %x\r\n" , d_ind->reason);
    }break;
    
    case BLE_5_SEC_BONDLIST_COMPARISON_CMP_IND:
    {
        uint8_t *conn_idx = (uint8_t *)param;
        
        os_printf("BLE BLE_5_SEC_BONDLIST_COMPARISON_CMP_IND, conn_idx=%d\r\n", *conn_idx);
    
        ql_ble_security_req(*conn_idx, 30000); 
        
        break;
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

void ble_ams_NextTrack(void)
{
    uint8_t data[] = {
            RemoteCommandIDNextTrack,
    };

    ql_ble_gattc_write_service_data_req(0,ams_remote_command_handle,sizeof(data),data,NULL);
}

void ble_ams_PreviousTrack(void)
{
    uint8_t data[] = {
            RemoteCommandIDPreviousTrack,
    };

    ql_ble_gattc_write_service_data_req(0,ams_remote_command_handle,sizeof(data),data,NULL);
}

static void quec_ble_app_sdp_characteristic_cb(unsigned char conidx,uint16_t chars_val_hdl,unsigned char uuid_len,unsigned char *uuid)
{
    os_printf("[APP]characteristic conidx:%d,handle:0x%02x(%d),UUID:0x",conidx,chars_val_hdl,chars_val_hdl);
    for(int i = 0; i< uuid_len; i++)
    {
        os_printf("%02x ",uuid[i]);
    }
    os_printf("\r\n");

    ///Get Handle
    {
        uint8_t att_uuid[16] = AMS_CHAR_REMOTE_COMMAND;
        if (!memcmp(uuid,att_uuid,16)) 
        {
            ams_remote_command_handle = chars_val_hdl;
            os_printf("[cx]get ams_remote_command_handle success , %d\r\n" , ams_remote_command_handle);
            
            ql_ble_ancs_demo_msg ble_ancs_demo_msg = {0};
            ble_ancs_demo_msg.msg_id = QL_BLE_ANCS_DEMO_GATTC_NTF_ENABLE;
            ble_ancs_demo_msg.msg_param.ntf_enable.chara_hdl = chars_val_hdl+1;
            ql_ble_ancs_demo_msg_send(&ble_ancs_demo_msg);
            
        }
    }

    {
        uint8_t att_uuid[16] = AMS_CHAR_ENTITY_UPDATE;
        if (!memcmp(uuid,att_uuid,16)) {
            ams_entity_update_handle = chars_val_hdl;
            os_printf("[cx]get ams_entity_update_handle success %d\r\n" , ams_entity_update_handle);
        
            ql_ble_ancs_demo_msg ble_ancs_demo_msg = {0};
            ble_ancs_demo_msg.msg_id = QL_BLE_ANCS_DEMO_GATTC_NTF_ENABLE;
            ble_ancs_demo_msg.msg_param.ntf_enable.chara_hdl = chars_val_hdl+1;
            ql_ble_ancs_demo_msg_send(&ble_ancs_demo_msg);
            
            memset(&ble_ancs_demo_msg, 0x00, sizeof(ql_ble_ancs_demo_msg));
            ble_ancs_demo_msg.msg_id = QL_BLE_ANCS_DEMO_GATTC_WR;
            ql_ble_ancs_demo_msg_send(&ble_ancs_demo_msg);
        }
    }

    {
        uint8_t att_uuid[16] = AMS_CHAR_ENTITY_ATTRIBUTE;
        if (!memcmp(uuid,att_uuid,16)) {
            ams_entity_att = chars_val_hdl;
            os_printf("[cx]get ams_entity_att success\r\n" , ams_entity_att);
        }
    }

    {
        uint8_t att_uuid[16] = ANCS_CHAR_CONTROL_POINT;
        if (!memcmp(uuid,att_uuid,16)) {
            ancs_control_point_handle = chars_val_hdl;
            os_printf("[cx]get ancs_control_point_handle success\r\n" , ancs_control_point_handle);
        }
    }

    {
        uint8_t att_uuid[16] = ANCS_CHAR_NTF_SOURCE;
        if (!memcmp(uuid,att_uuid,16)) {
            ancs_ntf_s_handle = chars_val_hdl;
            os_printf("[cx]get ancs_ntf_s_handle success\r\n" , ancs_ntf_s_handle);
        
            ql_ble_ancs_demo_msg ble_ancs_demo_msg = {0};
            ble_ancs_demo_msg.msg_id = QL_BLE_ANCS_DEMO_GATTC_NTF_ENABLE;
            ble_ancs_demo_msg.msg_param.ntf_enable.chara_hdl = chars_val_hdl;
            ql_ble_ancs_demo_msg_send(&ble_ancs_demo_msg);
        }
    }
    {
        uint8_t att_uuid[16] = ANCS_CHAR_DATA_SOURCE;
        if (!memcmp(uuid,att_uuid,16)) {
            ancs_data_s_hanlde = chars_val_hdl;
            os_printf("[cx]get ancs_data_s_hanlde success\r\n" , ancs_data_s_hanlde);
        
            ql_ble_ancs_demo_msg ble_ancs_demo_msg = {0};
            ble_ancs_demo_msg.msg_id = QL_BLE_ANCS_DEMO_GATTC_NTF_ENABLE;
            ble_ancs_demo_msg.msg_param.ntf_enable.chara_hdl = chars_val_hdl;
            ql_ble_ancs_demo_msg_send(&ble_ancs_demo_msg);
        }
    }
}

//receive peer device data
void quec_app_sdp_charac_cb(CHAR_TYPE type,uint8 conidx,uint16_t hdl,uint16_t len,uint8 *data)
{
    os_printf("BLE: %s, type: %d \r\n", __func__, type);

#if 0  //Printf raw data 
    os_printf("[APP]type:%x conidx:%d,handle:0x%02x(%d),len:%d,0x",type,conidx,hdl,hdl,len);
    for(int i = 0; i< len; i++)
    {
        os_printf("%02x ",data[i]);
    }
    os_printf("\r\n");
#endif

    if (hdl == ams_entity_update_handle)
    {
        os_printf("Entity ID:               0x%x\r\n",data[0]);
        os_printf("Attribute ID:            0x%x\r\n",data[1]);
        os_printf("EntityUpdateFlag ID:     0x%x\r\n",data[2]);
        os_printf("Value(ASKII):            ");
        for (uint16_t i=3 ;i < len;i++)
        {
            os_printf("%x ", data[i]);
        }
        os_printf("\r\n");
    } 
    else 
    if (hdl == ancs_ntf_s_handle) 
    {
        os_printf("EventID:                 0x%x\r\n",data[0]);
        os_printf("EventFlags:              0x%x\r\n",data[1]);
        os_printf("CategoryID:              0x%x\r\n",data[2]);
        os_printf("CategoryCount:           0x%x\r\n",data[3]);

        uint32_t NotificationUID;
        memcpy(&NotificationUID,&(data[4]),4);
        os_printf("NotificationUID:         0x%x\r\n",NotificationUID);
    } 
    else 
    if (hdl == ancs_data_s_hanlde) 
    {
        os_printf("Data Source 0x%x 0x%x\r\n",hdl,data);
    }

    if(type == CHARAC_WRITE_DONE)
    {
        if(wr_count < 5)
        {
            ql_ble_ancs_demo_msg ble_ancs_demo_msg = {0};
            ble_ancs_demo_msg.msg_id = QL_BLE_ANCS_DEMO_GATTC_WR;
            ql_ble_ancs_demo_msg_send(&ble_ancs_demo_msg);
        }
    }
}

static void ql_ble_ancs_demo_msg_process(ql_ble_ancs_demo_msg *ble_msg)
{
    switch (ble_msg->msg_id)
    {        
        case QL_BLE_ANCS_DEMO_START_ADV:
        {
            os_printf("QL_BLE_PAIR_DEMO_START_ADV \r\n");
            
            ql_ble_start_advertising(adv_actv_idx, 0, ble_periphera_cmd_cb);
            break;
        }

        case QL_BLE_ANCS_DEMO_DIS_ALL_SRV:
        {
            os_printf("QL_BLE_ANCS_DEMO_DIS_ALL_SRV, conn_hdl: [%d] \r\n", conn_hdl); 
            
            ql_ble_gattc_all_service_discovery(conn_hdl);          
            break;
        }

        case QL_BLE_ANCS_DEMO_GATTC_NTF_ENABLE:
        {
//            os_printf("ams_remote_command_handle = %d\r\n" , ams_remote_command_handle);
//            os_printf("ams_entity_update_handle  = %d\r\n" , ams_entity_update_handle);
//            os_printf("ancs_ntf_s_handle         = %d\r\n" , ancs_ntf_s_handle);
//            os_printf("ancs_data_s_hanlde        = %d\r\n" , ancs_data_s_hanlde);
//            
//            ql_ble_gattc_ntf_enable(conn_hdl, ancs_ntf_s_handle,1);
//            ql_ble_gattc_ntf_enable(conn_hdl, ancs_data_s_hanlde,1);
//            ql_ble_gattc_ntf_enable(conn_hdl, ams_remote_command_handle+1,1);
//            ql_ble_gattc_ntf_enable(conn_hdl, ams_entity_update_handle+1,1);

            os_printf("QL_BLE_ANCS_DEMO_GATTC_NTF_ENABLE, chara_hdl = %d \r\n", ble_msg->msg_param.ntf_enable.chara_hdl);

            ql_ble_gattc_ntf_enable(conn_hdl, ble_msg->msg_param.ntf_enable.chara_hdl+1, 1);
            break;
        }

        case QL_BLE_ANCS_DEMO_GATTC_WR:
        {
            os_printf("QL_BLE_ANCS_DEMO_GATTC_WR, wr_count = %d \r\n", wr_count);

            if(wr_count == 0)
            {
                uint8_t data[] = {
                        0x00,
                        0x00,0x01,0x02
                };
                ql_ble_gattc_write_service_data_req(conn_hdl, ams_entity_update_handle, sizeof(data), data, ble_periphera_cmd_cb);
            }
            else
            if(wr_count == 1)
            {
                uint8_t data[] = {
                        0x01,
                        0x00,0x01,0x02,0x03
                };
                ql_ble_gattc_write_service_data_req(conn_hdl, ams_entity_update_handle, sizeof(data), data, ble_periphera_cmd_cb);
            }
            else
            if(wr_count == 2)
            {
                uint8_t data[] = {
                        0x02,
                        0x00,0x01,0x02,0x03
                };
                ql_ble_gattc_write_service_data_req(conn_hdl, ams_entity_update_handle, sizeof(data), data, ble_periphera_cmd_cb);
            }
            else
            if(wr_count == 3)
            {
                uint8_t data[] = {
                        RemoteCommandIDVolumeUp,
                };
                
                ql_ble_gattc_write_service_data_req(conn_hdl, ams_remote_command_handle, sizeof(data), data, ble_periphera_cmd_cb);
                os_printf("volumeUp , ams_remote_command_handle = %d\r\n", ams_remote_command_handle);
            }
            else
            if(wr_count == 4)
            {
                uint8_t data[] = {
                        RemoteCommandIDVolumeDown,
                };

                ql_ble_gattc_write_service_data_req(conn_hdl, ams_remote_command_handle, sizeof(data), data, ble_periphera_cmd_cb);
                os_printf("volumeDown\r\n");
            }
            
            wr_count++;
            break;
        }
                
        default:
            break;
    }
}

static void ql_ble_ams_ancs_demo_entry(void *arg)
{
    unsigned char ble_mac[6] = {0};
    struct adv_param *adv_info = &g_adv_info;
    int ret = 0;

    /* Wait BLE stack initialization complete */
    while (kernel_state_get(TASK_BLE_APP) != APPM_READY)
    {
        ql_rtos_task_sleep_ms(200);
    }

    /* Set BLE event callback */
    ql_ble_set_notice_cb(ql_demo_periphera_notice_cb);

    ql_ble_bond_delete_all();  //Delete all device pair information

    //Set security param
#if 1 //PIN code
    ql_ble_security_param_t sec_param = {0};
    sec_param.io_cap = QL_BLE_IO_CAP_DISPLAY_ONLY;
    sec_param.password = 123456;
    sec_param.ble_secure_conn = true;
    sec_param.bond_auth = true;
    ql_ble_security_param_init(&sec_param);
#else //just-work
    ql_ble_security_param_t sec_param;
    sec_param.io_cap = QL_BLE_IO_CAP_NO_INPUT_NO_OUTPUT;
    sec_param.password = 123456;
    sec_param.ble_secure_conn = false;
    sec_param.bond_auth = true;
    ql_ble_security_param_init(&sec_param);
#endif

#if 1 //It is not necessary to include service creation
    /* Add BLE profile */
    struct ql_ble_db_cfg ble_db_cfg;
    ble_db_cfg.att_db = ql_demo_att_db;
    ble_db_cfg.att_db_nb = QL_DEMO_IDX_NB;
    ble_db_cfg.prf_task_id = 0;
    ble_db_cfg.start_hdl = 0;
    ble_db_cfg.svc_perm = QL_PERM_SET(SVC_UUID_LEN, UUID_16);
    memcpy(&(ble_db_cfg.uuid[0]), &ql_demo_svc_uuid[0], 16);
    ql_ble_create_db(&ble_db_cfg);
#endif

    /* Set advertising param */
    adv_info->interval_min = 160;
    adv_info->interval_max = 160;
    adv_info->channel_map = 7;
    adv_info->duration = 0;
    /* Set advertising data */
    uint8_t ancs[16] = ANCS_SERVICE_UUID;
    
    adv_info->advData[0] = 0x02; /* Must be set */
    adv_info->advData[1] = 0x01; /* Must be set */
    adv_info->advData[2] = 0x06; /* Must be set */
    adv_info->advData[3] = sizeof(ancs) + 1;
    adv_info->advData[4] = GAP_AD_TYPE_RQRD_128_BIT_SVC_UUID;
    memcpy(&adv_info->advData[5], ancs, sizeof(ancs));
    adv_info->advDataLen = sizeof(ancs) + 5;

    adv_info->respData[0] = strlen(rsp_data) + 1;
    adv_info->respData[1] = GAP_AD_TYPE_COMPLETE_NAME;
    memcpy(&adv_info->respData[2], rsp_data, strlen(rsp_data));
    adv_info->respDataLen = strlen(rsp_data) + 2;

    /*Set discovery callback*/
    ql_ble_gattc_register_discovery_callback(quec_ble_app_sdp_characteristic_cb);
    ql_ble_gattc_register_event_recv_callback(quec_app_sdp_charac_cb);

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
    
//    if(ams_volume_ctrl_toggle )
//    {
//        ams_volume_ctrl_toggle = 0;
//        ble_ams_volumeUp();
//    }
//    else
//    {
//        ams_volume_ctrl_toggle = 1;
//        ble_ams_volumeDown();
//    }


    ql_ble_ancs_demo_msg ble_demo_ancs_msg;
    
    while (1)
    {
        os_memset((void *)&ble_demo_ancs_msg, 0x00, sizeof(ql_ble_ancs_demo_msg));
        
        if (ql_rtos_queue_wait(ql_ble_ancs_demo_msg_q, (uint8 *)&ble_demo_ancs_msg, sizeof(ql_ble_ancs_demo_msg), QL_WAIT_FOREVER) == 0)
        {
            ql_ble_ancs_demo_msg_process(&ble_demo_ancs_msg);
        }
    }
}

void ql_ble_demo_ams_ancs_thread_creat(void)
{
    OSStatus err = kNoErr;

    err = ql_rtos_queue_create(&ql_ble_ancs_demo_msg_q, sizeof(ql_ble_ancs_demo_msg), 10);
    if (err != kNoErr)
    {
        return ;
    }
    
    // todo  create new thread;
    err = ql_rtos_task_create(&ble_periphera_hdl,
                                8*1024,
                                THD_EXTENDED_APP_PRIORITY,
                                "ble_demo",
                                ql_ble_ams_ancs_demo_entry,
                                0);

    if (err != kNoErr)
    {
        return ;
    }
    
    return;
}

#endif

