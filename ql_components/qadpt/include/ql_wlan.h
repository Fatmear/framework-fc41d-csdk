#ifndef __QL_WLAN_H_
#define __QL_WLAN_H_

#define WLAN_TEST_DEBUG
#ifdef WLAN_TEST_DEBUG
#define ql_wlan_log               os_printf

#include "wlan_ui_pub.h"

#else
#define ql_wlan_log               os_null_printf
#endif


typedef struct
{
	int8_t rssi;
}ql_wifi_link_info_t;

/**
 *  @brief  wlan network interface enumeration definition.
 */
typedef enum
{
    QL_SOFT_AP ,  /**< Act as an access point, and other station can connect, 4 stations Max*/
    QL_STATION ,   /**< Act as a station which can connect to an access point*/
} ql_wlanInterface_Typedef_e;


/**
 *  @brief  Wi-Fi security type enumeration definition.
 */
typedef enum 
{
    QL_SECURITY_TYPE_NONE,        /**< Open system. */
    QL_SECURITY_TYPE_WEP,         /**< Wired Equivalent Privacy. WEP security. */
    QL_SECURITY_TYPE_WPA_TKIP,    /**< WPA /w TKIP */
    QL_SECURITY_TYPE_WPA_AES,     /**< WPA /w AES */
    QL_SECURITY_TYPE_WPA2_TKIP,   /**< WPA2 /w TKIP */
    QL_SECURITY_TYPE_WPA2_AES,    /**< WPA2 /w AES */
    QL_SECURITY_TYPE_WPA2_MIXED,  /**< WPA2 /w AES or TKIP */
    QL_SECURITY_TYPE_WPA3_SAE,	  /**< WPA3 SAE */
    QL_SECURITY_TYPE_WPA3_WPA2_MIXED, /** WPA3 SAE or WPA2 AES */
    QL_SECURITY_TYPE_EAP,
    QL_SECURITY_TYPE_OWE,
    QL_SECURITY_TYPE_AUTO,        /**< It is used when calling @ref bkWlanStartAdv, _BK_ read security type from scan result. */
}ql_wlan_sec_type_e;

/**
 *  @brief  Input network paras, used in bk_wlan_start function.
 */
typedef struct 
{
    char wifi_mode;               /**< DHCP mode: @ref wlanInterfaceTypedef.*/
    char wifi_ssid[33];           /**< SSID of the wlan needs to be connected.*/
    char wifi_key[64];            /**< Security key of the wlan needs to be connected, ignored in an open system.*/
    char local_ip_addr[16];       /**< Static IP configuration, Local IP address. */
    char net_mask[16];            /**< Static IP configuration, Netmask. */
    char gateway_ip_addr[16];     /**< Static IP configuration, Router IP address. */
    char dns_server_ip_addr[16];   /**< Static IP configuration, DNS server IP address. */
    char dhcp_mode;                /**< DHCP mode, @ref DHCP_Disable, @ref DHCP_Client and @ref DHCP_Server. */
    char wifi_bssid[6];
    char reserved[26];
    int  wifi_retry_interval;     /**< Retry interval if an error is occured when connecting an access point,
                                     time unit is millisecond. */
} ql_network_InitTypeDef_s;

typedef struct
{
    char    ssid[32];    /**< SSID of the wlan that needs to be connected. Example: "SSID String". */
    char    bssid[6];    /**< BSSID of the wlan needs to be connected. Example: {0xC8 0x93 0x46 0x11 0x22 0x33}. */
    uint8_t channel;     /**< Wlan's RF frequency, channel 0-13. 1-13 means a fixed channel
                            that can speed up a connection procedure, 0 is not a fixed input
                            means all channels are possible*/
    uint8_t security;
}   ql_apinfo_adv_s;


typedef struct ql_network_InitTypeDef_adv_st
{
    ql_apinfo_adv_s ap_info;         /**< @ref apinfo_adv_t. */
    char  key[64];                /**< Security key or PMK of the wlan. */
    int   key_len;                /**< The length of the key. */
    char  local_ip_addr[16];      /**< Static IP configuration, Local IP address. */
    char  net_mask[16];           /**< Static IP configuration, Netmask. */
    char  gateway_ip_addr[16];    /**< Static IP configuration, Router IP address. */
    char  dns_server_ip_addr[16];  /**< Static IP configuration, DNS server IP address. */
    char  dhcp_mode;               /**< DHCP mode, @ref DHCP_Disable, @ref DHCP_Client and @ref DHCP_Server. */
    char  reserved[32];
    int   wifi_retry_interval;    /**< Retry interval if an error is occured when connecting an access point,
                                  time unit is millisecond. */
} ql_network_InitTypeDef_adv_s;

typedef struct ql_network_InitTypeDef_ap_st
{
    char wifi_ssid[32];
    char wifi_key[64];
    uint8_t channel;
    wlan_sec_type_t security;
	uint8_t ssid_hidden;
	uint8_t max_con;
    char local_ip_addr[16];
    char net_mask[16];
    char gateway_ip_addr[16];
    char dns_server_ip_addr[16];
    char dhcp_mode;
    char reserved[32];
    int  wifi_retry_interval;
} ql_network_InitTypeDef_ap_s;

typedef  struct  ql_ScanResult_adv
{
	char ApNum; 	  /**< The number of access points found in scanning.*/
	struct ql_ApListStruct
	{
		char ssid[33];	
		char ApPower;	
		uint8_t bssid[6];
		char channel;	
		uint8_t security;	
	} *ql_ApList_s;
} ql_ScanResult_adv_s;

typedef struct
{
	uint8_t dhcp;       /**< DHCP mode: @ref DHCP_Disable, @ref DHCP_Client, @ref DHCP_Server.*/
	char    ip[16];     /**< Local IP address on the target wlan interface: @ref wlanInterfaceTypedef.*/
    char    gate[16];   /**< Router IP address on the target wlan interface: @ref wlanInterfaceTypedef.*/
    char    mask[16];   /**< Netmask on the target wlan interface: @ref wlanInterfaceTypedef.*/
    char    dns[16];    /**< DNS server IP address.*/
    char    mac[16];    /**< MAC address, example: "C89346112233".*/
    char    broadcastip[16];
} ql_IPStatusTypedef_s;

typedef struct ql_linkStatus_t
{
    int conn_state;       /**< The link to wlan is established or not, reference to ql_wlan_evt_type */
    int wifi_strength;      /**< Signal strength of the current connected AP */
    uint8_t  ssid[32];      /**< SSID of the current connected wlan */
    uint8_t  bssid[6];      /**< BSSID of the current connected wlan */
    int      channel;       /**< Channel of the current connected wlan */
    uint8_t security;
} ql_LinkStatusTypeDef_s;



typedef enum {
    /* for station mode */
	QL_WLAN_EVT_STA_IDLE = 0,
	QL_WLAN_EVT_STA_CONNECTING = RW_EVT_STA_CONNECTING,
    QL_WLAN_EVT_STA_BEACON_LOSE = RW_EVT_STA_BEACON_LOSE,
    QL_WLAN_EVT_STA_PASSWORD_WRONG = RW_EVT_STA_PASSWORD_WRONG,
    QL_WLAN_EVT_STA_NO_AP_FOUND = RW_EVT_STA_NO_AP_FOUND,
    QL_WLAN_EVT_STA_ASSOC_FULL = RW_EVT_STA_ASSOC_FULL,
    QL_WLAN_EVT_STA_DISCONNECTED = RW_EVT_STA_DISCONNECTED,    /* disconnect with server */
    QL_WLAN_EVT_STA_CONNECT_FAILED = RW_EVT_STA_CONNECT_FAILED, /* authentication failed */
	QL_WLAN_EVT_STA_CONNECTED = RW_EVT_STA_CONNECTED,	 /* authentication success */
	QL_WLAN_EVT_STA_GOT_IP = RW_EVT_STA_GOT_IP,
    /* for softap mode */
    QL_WLAN_EVT_AP_CONNECTED = RW_EVT_AP_CONNECTED,          /* a client association success */
    QL_WLAN_EVT_AP_DISCONNECTED = RW_EVT_AP_DISCONNECTED,    /* a client disconnect */
    QL_WLAN_EVT_AP_CONNECT_FAILED = RW_EVT_AP_CONNECT_FAILED, /* a client association failed */
    QL_WLAN_EVT_MAX
}ql_wlan_evt_type;



typedef void (*ql_func_status_callback)(ql_wlan_evt_type *ctxt);
typedef void (*ql_func_scan_callback)(void *arg, unsigned char vif_idx);
typedef void (*ql_monitor_cb_t)(uint8_t *data, int len, ql_wifi_link_info_t *info);


int ql_wlan_start(ql_network_InitTypeDef_s *inNetworkInitPara);
int ql_wlan_start_sta_adv(ql_network_InitTypeDef_adv_s *inNetworkInitParaAdv);
int ql_wlan_stop(ql_wlanInterface_Typedef_e mode);
int ql_wlan_start_scan(void);
int ql_wlan_scan_ap_reg_cb(ql_func_scan_callback ind_cb);
int ql_wlan_sta_scan_result(ql_ScanResult_adv_s *results);
int ql_wlan_start_assign_scan(UINT8 **ssid_ary, UINT8 ssid_num);
int ql_wlan_status_register_cb(ql_func_status_callback cb);
int ql_wlan_start_monitor(void);
int ql_wlan_stop_monitor(void);
int ql_wlan_register_monitor_cb(ql_monitor_cb_t fn);
int ql_wlan_ap_para_info_get(ql_network_InitTypeDef_ap_s *ap_info);
int ql_wlan_get_ip_status(ql_IPStatusTypedef_s *outNetpara, ql_wlanInterface_Typedef_e inInterface);
int ql_wlan_get_link_status(ql_LinkStatusTypeDef_s *outStatus);
int ql_wlan_get_channel(void );
int ql_wlan_set_channel(int channel);
int ql_wlan_set_channel_sync(int channel);
int ql_sta_chiper_type(void);
int ql_wlan_ota_download(const char *uri,int port);
uint32_t ql_uap_ip_is_start(void);
uint32_t ql_sta_ip_is_start(void);


void ql_demo_scan_app_init(void);
void ql_demo_scan_adv_app_init(uint8_t * oob_ssid);
void ql_demo_softap_app_init(char *ap_ssid, char *ap_key);
void ql_demo_sta_app_init(char *oob_ssid,char *connect_key);
void ql_demo_sta_adv_app_init(char *oob_ssid,char *connect_key);
void ql_demo_wlan_app_init(VIF_ADDCFG_PTR cfg);
void ql_demo_state_app_init(void);
void ql_demo_ip_app_init(void);

void ql_wlan_cfgnet_demo_thread_creat(void);
int ql_airkiss_start(u8 start);
uint32 ql_airkiss_is_at_its_context(void);
void ql_airkiss_demo_thread_creat(void);

#endif

