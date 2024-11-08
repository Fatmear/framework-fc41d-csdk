#include "ql_include.h"



#define WLAN_DEFAULT_IP         "192.168.19.1"
#define WLAN_DEFAULT_GW         "192.168.19.1"
#define WLAN_DEFAULT_MASK       "255.255.255.0"

/*Callback function, used to parse scan results after scan*/
static void scan_cb(void *ctxt, uint8_t param)
{
	
/* CFG_WPA_CTRL_IFACE */
	static const char *crypto_str[] = {
		"None",
		"WEP",
		"WPA_TKIP",
		"WPA_AES",
		"WPA2_TKIP",
		"WPA2_AES",
		"WPA2_MIXED",		////QL_SECURITY_TYPE_WPA3_SAE
		"WPA3_SAE",	  		/**< WPA3 SAE */
		"WPA3_WPA2_MIXED",	/** WPA3 SAE or WPA2 AES */
		"AUTO",
	};

	ScanResult_adv apList;
	if (wlan_sta_scan_result(&apList) == 0) {
		int ap_num = apList.ApNum;
		int i;

		ql_wlan_log("Got ap count: %d\r\n", apList.ApNum);
		for (i = 0; i < ap_num; i++)
			ql_wlan_log("    \"%s\", " MACSTR "\b, %d, %s, %d\n",
					apList.ApList[i].ssid, MAC2STR(apList.ApList[i].bssid),
					apList.ApList[i].ApPower, crypto_str[apList.ApList[i].security],
					apList.ApList[i].channel);
		os_free(apList.ApList);
	}

}


void ql_demo_scan_app_init(void)
{
	/*Register scan callback function*/
	mhdr_scanu_reg_cb(scan_cb, 0);
	/*start scan*/
	ql_wlan_start_scan();
}

void ql_demo_scan_adv_app_init(uint8_t *oob_ssid)
{
    uint8_t **ssid_array;

    ssid_array = &oob_ssid;
	/*Register scan response function*/
	mhdr_scanu_reg_cb(scan_cb, 0);

	ql_wlan_log("scan for ssid:%s\r\n", oob_ssid);
	ql_wlan_start_assign_scan(ssid_array, 1);
}

void ql_demo_softap_app_init(char *ap_ssid, char *ap_key)
{
	/*Define a structure for passing in parameters*/
    ql_network_InitTypeDef_s wNetConfig;
    int len;

	/*置空结构体*/
    os_memset(&wNetConfig, 0x0, sizeof(ql_network_InitTypeDef_s));

    len = os_strlen(ap_ssid);
    if(SSID_MAX_LEN < len)
    {
        ql_wlan_log("ssid name more than 32 Bytes\r\n");
        return;
    }

    len = os_strlen(ap_key);
    if(63 < len)
    {
        bk_printf("key  more than 63 Bytes\r\n");
        return;
    }

	/*Pass in the ap ssid and ap key to connect*/
    os_strcpy((char *)wNetConfig.wifi_ssid, ap_ssid);
    os_strcpy((char *)wNetConfig.wifi_key, ap_key);

	/*AP mode*/
    wNetConfig.wifi_mode = QL_SOFT_AP;
	/*With DHCP SERVER mode, static addresses need to be assigned as local addresses*/
    wNetConfig.dhcp_mode = DHCP_SERVER;
    wNetConfig.wifi_retry_interval = 100;
    os_strcpy((char *)wNetConfig.local_ip_addr, WLAN_DEFAULT_IP);
    os_strcpy((char *)wNetConfig.net_mask, WLAN_DEFAULT_MASK);
    os_strcpy((char *)wNetConfig.gateway_ip_addr, WLAN_DEFAULT_GW);
    os_strcpy((char *)wNetConfig.dns_server_ip_addr, WLAN_DEFAULT_GW);

    ql_wlan_log("ssid:%s  key:%s\r\n", wNetConfig.wifi_ssid, wNetConfig.wifi_key);
	/*Start ap*/
	ql_wlan_start(&wNetConfig);
}

void ql_demo_sta_app_init(char *oob_ssid,char *connect_key)
{
	/*Define a structure for passing in parameters*/
	ql_network_InitTypeDef_s wNetConfig;
    int len;

	os_memset(&wNetConfig, 0x0, sizeof(network_InitTypeDef_st));

	/*Check the length of SSID, which cannot exceed 32 bytes*/
    len = os_strlen(oob_ssid);
    if(SSID_MAX_LEN < len)
    {
        ql_wlan_log("ssid name more than 32 Bytes\r\n");
        return;
    }

	/*Pass SSID and password into the structure*/
	os_strcpy((char *)wNetConfig.wifi_ssid, oob_ssid);
	os_strcpy((char *)wNetConfig.wifi_key, connect_key);

	/*STATION mode*/
	wNetConfig.wifi_mode = QL_STATION;
	/*Obtain the IP address dynamically from the router by using DHCP CLIENT*/
	wNetConfig.dhcp_mode = DHCP_CLIENT;
	wNetConfig.wifi_retry_interval = 100;

	ql_wlan_log("ssid:%s key:%s\r\n", wNetConfig.wifi_ssid, wNetConfig.wifi_key);
	/*Start STATION*/
	ql_wlan_start(&wNetConfig);
}

void ql_demo_sta_adv_app_init(char *oob_ssid,char *connect_key)
{
	/*Define a structure for passing in parameters*/
	ql_network_InitTypeDef_adv_s	wNetConfigAdv;

	os_memset( &wNetConfigAdv, 0x0, sizeof(network_InitTypeDef_adv_st) );

	/*Incoming SSID to connect*/
	os_strcpy((char*)wNetConfigAdv.ap_info.ssid, oob_ssid);
	/*Enter the bssid of the network to be connected. The following bssid is for reference only*/
	hwaddr_aton("48:ee:0c:48:93:12", (u8 *)(wNetConfigAdv.ap_info.bssid));
	/*Encryption method*/
	wNetConfigAdv.ap_info.security = QL_SECURITY_TYPE_WPA2_MIXED;
	/*The channel of the network to be connected*/
	wNetConfigAdv.ap_info.channel = 11;

	/*Network password and password length*/
	os_strcpy((char*)wNetConfigAdv.key, connect_key);
	wNetConfigAdv.key_len = os_strlen(connect_key);
	/*Obtain network information such as IP address through DHCP*/
	wNetConfigAdv.dhcp_mode = DHCP_CLIENT;
	wNetConfigAdv.wifi_retry_interval = 100;

	 /*Start connect*/
	ql_wlan_start_sta_adv(&wNetConfigAdv);
}

void ql_demo_wlan_app_init(VIF_ADDCFG_PTR cfg)
{
    ql_network_InitTypeDef_s network_cfg;
	ql_network_InitTypeDef_adv_s	networkadv_cfg;

    if(cfg->wlan_role == QL_STATION) {
        if(cfg->adv == 1) {
        	os_memset( &networkadv_cfg, 0x0, sizeof(network_InitTypeDef_adv_st) );

        	os_strcpy((char*)networkadv_cfg.ap_info.ssid, cfg->ssid);
        	hwaddr_aton("48:ee:0c:48:93:12", (u8 *)(networkadv_cfg.ap_info.bssid));
        	networkadv_cfg.ap_info.security = QL_SECURITY_TYPE_WPA2_MIXED;
        	networkadv_cfg.ap_info.channel = 11;

        	os_strcpy((char*)networkadv_cfg.key, cfg->key);
        	networkadv_cfg.key_len = os_strlen(cfg->key);
        	networkadv_cfg.dhcp_mode = DHCP_CLIENT;
        	networkadv_cfg.wifi_retry_interval = 100;

        	ql_wlan_start_sta_adv(&networkadv_cfg);
            return;
        } else {
        	os_memset(&network_cfg, 0x0, sizeof(network_InitTypeDef_st));

        	os_strcpy((char *)network_cfg.wifi_ssid, cfg->ssid);
        	os_strcpy((char *)network_cfg.wifi_key, cfg->key);

        	network_cfg.wifi_mode = QL_STATION;
        	network_cfg.dhcp_mode = DHCP_CLIENT;
        	network_cfg.wifi_retry_interval = 100;

        	ql_wlan_log("ssid:%s key:%s\r\n", network_cfg.wifi_ssid, network_cfg.wifi_key);
        }
    } else if(cfg->wlan_role == QL_SOFT_AP) {
        os_memset(&network_cfg, 0x0, sizeof(network_InitTypeDef_st));
        os_strcpy((char *)network_cfg.wifi_ssid, cfg->ssid);
        os_strcpy((char *)network_cfg.wifi_key, cfg->key);

        network_cfg.wifi_mode = QL_SOFT_AP;
        network_cfg.dhcp_mode = DHCP_SERVER;
        network_cfg.wifi_retry_interval = 100;
        os_strcpy((char *)network_cfg.local_ip_addr, WLAN_DEFAULT_IP);
        os_strcpy((char *)network_cfg.net_mask, WLAN_DEFAULT_MASK);
        os_strcpy((char *)network_cfg.gateway_ip_addr, WLAN_DEFAULT_GW);
        os_strcpy((char *)network_cfg.dns_server_ip_addr, WLAN_DEFAULT_GW);

        ql_wlan_log("ssid:%s  key:%s\r\n", network_cfg.wifi_ssid, network_cfg.wifi_key);
    }

    ql_wlan_start(&network_cfg);

}

void ql_demo_state_app_init(void)
{
	/*Define the structure to save the connection state*/
	ql_LinkStatusTypeDef_s linkStatus;
	ql_network_InitTypeDef_ap_s ap_info;
	char ssid[33] = {0};

	ql_wlan_log("sta: %d, softap: %d, b/g/n\r\n", sta_ip_is_start(), uap_ip_is_start());

	/*Connection status in STATION mode*/
	if (sta_ip_is_start()) {
		os_memset(&linkStatus, 0x0, sizeof(LinkStatusTypeDef));
		/*Get link info*/
		ql_wlan_get_link_status(&linkStatus);
		os_memcpy(ssid, linkStatus.ssid, 32);

		ql_wlan_log("sta:rssi=%d,ssid=%s,bssid=" MACSTR ",channel=%d,cipher_type:",
				  linkStatus.wifi_strength, ssid, MAC2STR(linkStatus.bssid), linkStatus.channel);
		switch (ql_sta_chiper_type()) {
		case QL_SECURITY_TYPE_NONE:
			ql_wlan_log("OPEN\r\n");
			break;
		case QL_SECURITY_TYPE_WEP :
			ql_wlan_log("WEP\r\n");
			break;
		case QL_SECURITY_TYPE_WPA_TKIP:
			ql_wlan_log("TKIP\r\n");
			break;
		case QL_SECURITY_TYPE_WPA_AES:
			ql_wlan_log("WPA_AES\r\n");
			break;
		case QL_SECURITY_TYPE_WPA2_AES:
			ql_wlan_log("CCMP\r\n");
			break;
		case QL_SECURITY_TYPE_WPA2_TKIP:
			ql_wlan_log("WPA2_TKIP\r\n");
			break;
		case QL_SECURITY_TYPE_WPA2_MIXED:
			ql_wlan_log("WPA/WPA2 MIXED\r\n");
			break;
		case QL_SECURITY_TYPE_AUTO:
			ql_wlan_log("AUTO\r\n");
			break;
		case QL_SECURITY_TYPE_WPA3_SAE:
			ql_wlan_log("WPA3\n");
			break;
		case QL_SECURITY_TYPE_WPA3_WPA2_MIXED:
			ql_wlan_log("WPA2/WPA3 MIXED\n");
			break;
		default:
			ql_wlan_log("Error\r\n");
			break;
		}
	}

	/*Connection status in AP mode*/
	if (uap_ip_is_start()) {
		os_memset(&ap_info, 0x0, sizeof(network_InitTypeDef_ap_st));
		/*get link info*/
		ql_wlan_ap_para_info_get(&ap_info);
		os_memcpy(ssid, ap_info.wifi_ssid, 32);
		ql_wlan_log("softap:ssid=%s,channel=%d,dhcp=%d,cipher_type:\r\n",
				  ssid, ap_info.channel, ap_info.dhcp_mode);
		
		switch (ap_info.security) {
		case QL_SECURITY_TYPE_NONE:
			ql_wlan_log("OPEN\r\n");
			break;
		case QL_SECURITY_TYPE_WEP :
			ql_wlan_log("WEP\r\n");
			break;
		case QL_SECURITY_TYPE_WPA_TKIP:
			ql_wlan_log("TKIP\r\n");
			break;
		case QL_SECURITY_TYPE_WPA2_AES:
			ql_wlan_log("CCMP\r\n");
			break;
		case QL_SECURITY_TYPE_WPA2_MIXED:
			ql_wlan_log("WPA/WPA2 MIXED\r\n");
			break;
		case QL_SECURITY_TYPE_AUTO:
			ql_wlan_log("AUTO\r\n");
			break;
		case QL_SECURITY_TYPE_WPA3_SAE:
			ql_wlan_log("WPA3\n");
			break;
		case QL_SECURITY_TYPE_WPA3_WPA2_MIXED:
			ql_wlan_log("WPA2/WPA3 MIXED\n");
			break;
		default:
			ql_wlan_log("Error\r\n");
			break;
		}
		ql_wlan_log("ip=%s,gate=%s,mask=%s,dns=%s\r\n",
				  ap_info.local_ip_addr, ap_info.gateway_ip_addr, ap_info.net_mask, ap_info.dns_server_ip_addr);
	}
}

void ql_demo_ip_app_init(void)
{
	/*Define a structure for saving network state*/
    ql_IPStatusTypedef_s ipStatus;

	os_memset(&ipStatus, 0x0, sizeof(IPStatusTypedef));
	/*Get the network status and save it in the structure*/
	ql_wlan_get_ip_status(&ipStatus, QL_STATION);

	ql_wlan_log("dhcp=%d ip=%s gate=%s mask=%s mac=" MACSTR "\r\n",
				ipStatus.dhcp, ipStatus.ip, ipStatus.gate,
				ipStatus.mask, MAC2STR((unsigned char*)ipStatus.mac));
}

/* monitor callback*/
void ql_demo_monitor_cb(uint8_t *data, int len, wifi_link_info_t *info)
{
	ql_wlan_log("len:%d\r\n", len);
		
	//Only for reference
	/*
	User can get ssid and key by prase monitor data,
	refer to the following code, which is the way airkiss
	use monitor get wifi info from data
	*/

}


ql_task_t	airkiss_test_thread_handle = NULL;

void ql_airkiss_demo_thread(void *param)
{
	ql_rtos_task_sleep_ms(3000);
	ql_airkiss_start(1);
 	ql_rtos_task_delete(airkiss_test_thread_handle);
}


void ql_airkiss_demo_thread_creat(void)
{
    int ret;
	ret = ql_rtos_task_create(&airkiss_test_thread_handle,
					   (unsigned short)2048,
					   THD_EXTENDED_APP_PRIORITY,
					   "airkiss_test",
					   ql_airkiss_demo_thread,   
					   0);


	  if (ret != kNoErr) {
		os_printf("Error: Failed to create airkiss test thread: %d\r\n",ret);
		goto init_err;
	  }
   
	  return;
   
	  init_err:
	   if( airkiss_test_thread_handle != NULL ) {
		   ql_rtos_task_delete(airkiss_test_thread_handle);
	   }

}

