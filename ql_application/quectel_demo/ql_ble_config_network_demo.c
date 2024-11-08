#include "string.h"
#include "ble_ui.h"
#include "app_ble.h"
#include "app_sdp.h"

#if (BLE_APP_PRESENT && (BLE_CENTRAL))
#include "app_ble_init.h"
#if BLE_SDP_CLIENT
#include "app_sdp.h"
#endif
#endif

#include "gattc.h"
#include "ql_ble.h"
#include "app_comm.h"
#include "ble_api_5_x.h"
#include "param_config.h"
#include "app_task.h"
#include "quectel_demo_config.h"
#include "ql_wlan.h"
#include "ql_api_osi.h"
#include "wlan_ui_pub.h"


/***************ble config network test******************/
#if CFG_ENABLE_QUECTEL_BLE_CONFIG_NETWORK

static ql_task_t ble_config_network_hdl = NULL;
static int ble_periphera_state = 0;
static uint8_t adv_actv_idx = -1;
static char ble_name[32] = {0};
struct adv_param g_adv_info = {0};
static uint16_t g_ucBleMtu = 23;
static char g_cApSsid[32] = {0};
static char g_cApPwd[64] = {0};

#define QL_RSSI_AP_POWER(rssi)     (((rssi + 100) > 100) ? 100 : (rssi + 100))

#define QUEC_ATT_DECL_PRIMARY_SERVICE_128     {0x00,0x28,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
#define QUEC_ATT_DECL_CHARACTERISTIC_128      {0x03,0x28,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
#define QUEC_ATT_DESC_CLIENT_CHAR_CFG_128     {0x02,0x29,0,0,0,0,0,0,0,0,0,0,0,0,0,0}

#define QUEC_WRITE_REQ_CHARACTERISTIC_128        {0x01,0xFF,0,0,0x34,0x56,0,0,0,0,0x28,0x37,0,0,0,0}
#define QUEC_INDICATE_CHARACTERISTIC_128         {0x02,0xFF,0,0,0x34,0x56,0,0,0,0,0x28,0x37,0,0,0,0}
#define QUEC_NOTIFY_CHARACTERISTIC_128           {0x03,0xFF,0,0,0x34,0x56,0,0,0,0,0x28,0x37,0,0,0,0}

enum
{
    TEST_IDX_SVC,
    TEST_IDX_FF01_VAL_CHAR,
    TEST_IDX_FF01_VAL_VALUE,
    TEST_IDX_FF01_VAL_NTF_CFG,
    TEST_IDX_NB,
};
static ql_attm_desc_t quec_att_db_default[TEST_IDX_NB] =
{
    //  Service Declaration
    [TEST_IDX_SVC]              = {QUEC_ATT_DECL_PRIMARY_SERVICE_128, QL_PERM_SET(RD, ENABLE), 0, 0},

    //  Level Characteristic Declaration
    [TEST_IDX_FF01_VAL_CHAR]    = {QUEC_ATT_DECL_CHARACTERISTIC_128,  QL_PERM_SET(RD, ENABLE), 0, 0},

    //  Level Characteristic Value
    [TEST_IDX_FF01_VAL_VALUE]   = {QUEC_WRITE_REQ_CHARACTERISTIC_128,    QL_PERM_SET(WRITE_REQ, ENABLE)|QL_PERM_SET(NTF, ENABLE), QL_PERM_SET(RI, ENABLE)|QL_PERM_SET(UUID_LEN, UUID_16), 512},

    //  Level Characteristic - Client Characteristic Configuration Descriptor
    [TEST_IDX_FF01_VAL_NTF_CFG] = {QUEC_ATT_DESC_CLIENT_CHAR_CFG_128, QL_PERM_SET(RD, ENABLE)|QL_PERM_SET(WRITE_REQ, ENABLE), 0, 0},
};

static const uint8_t quec_svc_uuid_default[16] = {0xFF,0xFF,0,0,0x34,0x56,0,0,0,0,0x28,0x37,0,0,0,0};
static void *  demo_scan_sema = NULL;
void ql_ble_send_net_info(char *msg,unsigned int msg_len);



//////////////////////////////////////////////////////////wifi function///////////////////////////////////////////////////////////////
void ql_demo_sta_connect_ap(char *oob_ssid,char *connect_key)
{
	network_InitTypeDef_st wNetConfig;
    int len;
	os_memset(&wNetConfig, 0x0, sizeof(network_InitTypeDef_st));

    len = os_strlen(oob_ssid);
    if(32 < len)
    {
        os_printf("ssid name more than 32 Bytes\r\n");
        return;
    }

	os_strcpy((char *)wNetConfig.wifi_ssid, oob_ssid);
	os_strcpy((char *)wNetConfig.wifi_key, connect_key);

	wNetConfig.wifi_mode = QL_STATION;
	wNetConfig.dhcp_mode = DHCP_CLIENT;
	wNetConfig.wifi_retry_interval = 100;

	os_printf("ssid:%s key:%s\r\n", wNetConfig.wifi_ssid, wNetConfig.wifi_key);
	ql_wlan_start(&wNetConfig);
}


int scan_ap_cb(void)
{
	if (demo_scan_sema) {
		rtos_set_semaphore( &demo_scan_sema );
	}

	return 0;
}

static int ql_demo_scan_ap_cb(void)
{
	if (demo_scan_sema) {
		rtos_set_semaphore( &demo_scan_sema );
	}

	return 0;
}



static void ql_demo_status_callback(ql_wlan_evt_type *ctxt)
{
	os_printf("[WLAN]%s:%d,event(%x)\r\n",__FUNCTION__ , __LINE__ , *ctxt);
	switch(*ctxt){
	case QL_WLAN_EVT_STA_GOT_IP:
        ql_ble_send_net_info("+QSTASTAT:GOT_IP\r\n" , strlen("+QSTASTAT:GOT_IP\r\n"));
		break;
	case QL_WLAN_EVT_STA_CONNECTED:
        ql_ble_send_net_info("+QSTASTAT:WLAN_CONNECTED\r\n" , strlen("+QSTASTAT:WLAN_CONNECTED\r\n"));
		break;
	case QL_WLAN_EVT_STA_DISCONNECTED:
		break;
	case QL_WLAN_EVT_STA_NO_AP_FOUND:
		break;
	case QL_WLAN_EVT_AP_CONNECTED:
		break;
	case QL_WLAN_EVT_AP_DISCONNECTED:
	    break;
	case QL_WLAN_EVT_AP_CONNECT_FAILED:
    	break;
	default:
		break;
	}
}


static int ql_demo_wlan_scan_find_ssid_len(unsigned char *ssid)
{
	int i;

	for(i = 0;i < 32; i++){
		if(ssid[i] == '\0'){
			break;
		}
	}
	return i;
}



static int ql_demo_get_scan_result(void)
{
	char *resultbuf  = NULL;
	int i;
	int n = 0;
    #define WLAN_SCAN_BUF_SIZE     (5 * 1024)
	resultbuf = os_malloc(WLAN_SCAN_BUF_SIZE);
	if(resultbuf == NULL) {
		os_printf("resultbuf malloc failed!\r\n");
		return 1;
	}

    ql_ScanResult_adv_s apList;
    if (ql_wlan_sta_scan_result(&apList) == 0) {
        int ap_num = apList.ApNum;

        os_printf("Got ap count: %d\r\n", ap_num);
        resultbuf[0] = '\0';
        for (i = 0; i < ap_num; i++) {
            os_printf("scan_ssid:%s\r\n" , apList.ql_ApList_s[i].ssid);
            if(ql_demo_wlan_scan_find_ssid_len((unsigned char*)apList.ql_ApList_s[i].ssid)>= 32){
                char temp[33];
                memcpy(temp,apList.ql_ApList_s[i].ssid,32);
                temp[32] = '\0';
                n += sprintf(resultbuf+n,"+QWSCAN:\"%s\",",temp);
            }else{
                n += sprintf(resultbuf+n,"+QWSCAN:\"%s\",",apList.ql_ApList_s[i].ssid);
            }
            switch(apList.ql_ApList_s[i].security) {
            case QL_SECURITY_TYPE_NONE:
                n += sprintf(resultbuf+n,"%s","OPEN");
                break;
            case QL_SECURITY_TYPE_WEP:
                n += sprintf(resultbuf+n,"%s","WEP_SHARED");
                break;
            case QL_SECURITY_TYPE_WPA_TKIP:
                n += sprintf(resultbuf+n,"%s","WPA_TKIP_PSK");
                break;
            case QL_SECURITY_TYPE_WPA_AES:
                n += sprintf(resultbuf+n,"%s","WPA_AES_PSK");
                break;
            case QL_SECURITY_TYPE_WPA2_TKIP:
                n += sprintf(resultbuf+n,"%s","WPA2_TKIP_PSK");
                break;
            case QL_SECURITY_TYPE_WPA2_AES:
                n += sprintf(resultbuf+n,"%s","WPA2_AES_PSK");
                break;
            case QL_SECURITY_TYPE_WPA2_MIXED:
                n += sprintf(resultbuf+n,"%s","WPA2_MIXED_PSK");
                break;
            case QL_SECURITY_TYPE_WPA3_SAE:
                n += sprintf(resultbuf+n,"%s","WPA3_SAE");
                break;
            case QL_SECURITY_TYPE_WPA3_WPA2_MIXED:
                n += sprintf(resultbuf+n,"%s","WPA3_WPA2_MIXED");
                break;
            case QL_SECURITY_TYPE_AUTO:
                n += sprintf(resultbuf+n,"%s","AUTO_PSK");
                break;
            default:
                n += sprintf(resultbuf+n,"%s","UNKNOWN");
                break;
            }
            n += sprintf(resultbuf+n,",%d",QL_RSSI_AP_POWER(apList.ql_ApList_s[i].ApPower)); ///RSSI
            n += sprintf(resultbuf+n,",%02x:%02x:%02x:%02x:%02x:%02x",apList.ql_ApList_s[i].bssid[0],
                            apList.ql_ApList_s[i].bssid[1],apList.ql_ApList_s[i].bssid[2],
                            apList.ql_ApList_s[i].bssid[3],apList.ql_ApList_s[i].bssid[4],
                            apList.ql_ApList_s[i].bssid[5]);
            n += sprintf(resultbuf+n,",%d\r\n",apList.ql_ApList_s[i].channel);
            
            if(n > (WLAN_SCAN_BUF_SIZE - 512)){
                ql_ble_send_net_info(resultbuf , n);
                resultbuf[0] = '\0';
                n = 0;
            }
        }
        os_printf("scan_res:%d\r\n" , n);
        ql_ble_send_net_info(resultbuf , n);
        os_free(apList.ql_ApList_s);
    }
    
    ql_ble_send_net_info("OK\r\n", strlen("OK\r\n"));
    
	if(resultbuf) {
		free(resultbuf);
		resultbuf = NULL;
	}


	return 0;
}

static int ql_demo_wlan_scan_start(void)
{
	int err;

	if( demo_scan_sema == NULL ) {
        err = rtos_init_semaphore( &demo_scan_sema, 1 );
		if(err != 0){
			return -1;
		}
    }
	ql_wlan_scan_ap_reg_cb(ql_demo_scan_ap_cb);
    ql_wlan_status_register_cb(ql_demo_status_callback);
	ql_wlan_start_scan();
	if(demo_scan_sema != NULL) {
		err = rtos_get_semaphore(&demo_scan_sema, 4000);
		if(err == 0) {
			os_printf("scan wait semaphore ok!\r\n");
		} else {
			os_printf("scan wait semaphore error!\r\n");
		}
	} else {
		os_printf("waited semaphore!\r\n");
		return -1;
	}

	if(ql_demo_get_scan_result() != 0){
		return -1;
	}
	return 0;
}
//////////////////////////////////////////////////////////wifi function///////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////ble function///////////////////////////////////////////////////////////////
void ql_ble_send_net_info(char *msg,unsigned int msg_len)
{
    int send = msg_len;
    char *p = msg;
    int	mtu = g_ucBleMtu- 3;
    os_printf("%s:%d,mtu=%d + 3 , msg_len = %d\r\n", __FUNCTION__ , __LINE__ , mtu , msg_len);
    
    while(send > mtu)
    {
        if(0 != ql_ble_gatts_send_ntf_value(mtu, p, 0, TEST_IDX_FF01_VAL_VALUE))
        {
            os_printf("BLE SEND ERROR\r\n");
        }
        send = send-mtu;
        p = p+mtu;
    }

    if(0 != ql_ble_gatts_send_ntf_value(send, p, 0, TEST_IDX_FF01_VAL_VALUE))
    {
        os_printf("BLE SEND ERROR\r\n");
    }
}


static void ql_demo_ble_config_network_notice_cb(ble_notice_t notice, void *param)
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
#if 0
        // 回传测试
        ql_ble_gatts_send_ntf_value(w_req->len , w_req->value , QL_DEMO_IDX_FF01_VAL_VALUE);
#endif
        if(NULL != strstr(w_req->value , "AT+QWSCAN"))
        {                        
            //start wifi scan
            os_printf("start wifi scan !\r\n");
            ql_demo_wlan_scan_start();
            
        }
        else if(NULL != strstr(w_req->value , "AT+QSTAAPINFO"))
        {
            memset(g_cApSsid , 0 , sizeof(g_cApSsid));
            memset(g_cApPwd , 0 , sizeof(g_cApPwd));
            char *_ssid = strchr(w_req->value , '=');
            if(NULL != _ssid)
            {
                _ssid++;
                for(uint8_t i = 0 ; i < sizeof(g_cApSsid) ; i++)
                {
                    if(_ssid[i] != ',')
                        g_cApSsid[i] = _ssid[i] ;
                    else
                        break;   
                }
            }
            char *pwd = strchr(_ssid, ',');
            if(NULL != pwd)
            {
                 pwd++;
                for(uint8_t i = 0 ; i < sizeof(g_cApPwd) ; i++)
                {
                    if(pwd[i] != '\r')
                        g_cApPwd[i] = pwd[i] ;
                    else
                        break;    
                }               
            }
            os_printf("ssid = %s , pwd = %s\r\n" ,g_cApSsid ,g_cApPwd);
            //connect ap
            ql_demo_sta_connect_ap(g_cApSsid , g_cApPwd);
        }
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
        g_ucBleMtu = m_ind->mtu_size;
        break;
    }
    case BLE_5_CONNECT_EVENT:
    {
        conn_ind_t *c_ind = (conn_ind_t *)param;
        os_printf("c_ind:conn_idx:%d, addr_type:%d, peer_addr:%02x:%02x:%02x:%02x:%02x:%02x\r\n",
                  c_ind->conn_idx, c_ind->peer_addr_type, c_ind->peer_addr[0], c_ind->peer_addr[1],
                  c_ind->peer_addr[2], c_ind->peer_addr[3], c_ind->peer_addr[4], c_ind->peer_addr[5]);
        
//        bk_ble_gatt_mtu_changes(c_ind->conn_idx, 512, NULL);

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


static void ql_ble_config_network_demo_entry(void *arg)
{
    unsigned char ble_mac[6] = {0};
    struct adv_param *adv_info = &g_adv_info;
    int ret = 0;
    
    while (1)
    {
        /* 等待协议栈初始化完成 */
        while (kernel_state_get(TASK_BLE_APP) != APPM_READY)
        {
            ql_rtos_task_sleep_ms(200);
        }

        if(ble_periphera_state == 0)
        {
            /* 设置ble事件callback */
            ql_ble_set_notice_cb(ql_demo_ble_config_network_notice_cb);

            /* 添加蓝牙服务 */
            struct ql_ble_db_cfg ble_db_cfg;
            ble_db_cfg.att_db = quec_att_db_default;
            ble_db_cfg.att_db_nb = TEST_IDX_NB;
            ble_db_cfg.prf_task_id = 0;
            ble_db_cfg.start_hdl = 0;
            ble_db_cfg.svc_perm = QL_PERM_SET(SVC_UUID_LEN, UUID_16);
            memcpy(&(ble_db_cfg.uuid[0]), &quec_svc_uuid_default[0], 16);
            ql_ble_create_db(&ble_db_cfg);

            /* 设置广播参数 */
            adv_info->interval_min = 160;
            adv_info->interval_max = 160;
            adv_info->channel_map = 7;
            adv_info->duration = 0;

            /* 设置蓝牙广播数据  */
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

            /* 获取空闲的actv_idx */
            adv_actv_idx = ql_ble_get_idle_actv_idx_handle();
            if (adv_actv_idx >= BLE_ACTIVITY_MAX)
            {
                return ;
            }
            /* 该actv_idx空闲时 */
            if (app_ble_actv_state_get(adv_actv_idx) == ACTV_IDLE)
            {
                ql_ble_adv_start(adv_actv_idx, adv_info, ble_periphera_cmd_cb);
            }
            else /*如果该actv_idx已经被创建，则直接开始广播*/
            {
                ql_ble_start_advertising(adv_actv_idx, 0, ble_periphera_cmd_cb);
            }

            ble_periphera_state = 1;
        }
        ql_rtos_task_sleep_ms(20);
    }

    ql_rtos_task_delete(NULL);
}
//////////////////////////////////////////////////////////ble function///////////////////////////////////////////////////////////////

void ql_ble_demo_config_network_thread_creat(void)
{
    OSStatus err = kNoErr;
    // todo  create new thread;
    err = ql_rtos_task_create(&ble_config_network_hdl,
                                8*1024,
                                THD_EXTENDED_APP_PRIORITY,
                                "ble_config_network_demo",
                                ql_ble_config_network_demo_entry,
                                0);

    if (err != kNoErr)
    {
        return ;
    }

    return;
}

#endif


