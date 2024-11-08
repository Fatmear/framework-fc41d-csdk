/*

quectel curl io api function

by: larson.li@quectl.com

*/

#include "curl/quectel_curl_io.h"
#include "uart_pub.h"
#include "curl/curl.h"
// #include "ql_api_datacall.h"
#define curl_print(msg, ...)     bk_printf(msg, ##__VA_ARGS__)

int quectel_curl_getaddrinfo
(
uint32_t simcid,
const char *nodename,
const char *servname,
const struct addrinfo *hints,
struct addrinfo **res
)
{
	return lwip_getaddrinfo(nodename,servname,hints,res);
}


// void quectel_nCid_local_addr(uint32_t simcid, char *addr, int af, const char *func_name)
// {
// 	const char *local_addr = NULL;
// 	uint8_t nSimId;
// 	int nCid;
// 	ql_data_call_info_s info;
//     memset(&info, 0, sizeof(ql_data_call_info_s));

// 	if(QL_DATACALL_SUCCESS == ql_get_sim_and_profile(simcid, &nSimId, &nCid) \
// 		&& QL_DATACALL_SUCCESS == ql_get_data_call_info(nSimId,nCid,&info))
// 	{
// 		curl_print("af=%d v4=%d v6=%d version=%d", af, info.v4.state, info.v6.state, info.ip_version);
// 		if(AF_INET6 == af && QL_PDP_ACTIVED == info.v6.state)
// 		{
// 			local_addr = ip6addr_ntoa(&info.v6.addr.ip);
// 		}
// 		if(AF_INET == af && QL_PDP_ACTIVED == info.v4.state)
// 		{
// 			local_addr = ip4addr_ntoa(&info.v4.addr.ip);
// 		}
// 	}
    

// 	if(local_addr == NULL)
// 	{
// 		if(info.ip_version == QL_PDP_TYPE_IPV6){
// 			memcpy(addr,"::1",strlen("::1"));
// 		}
// 		else
// 		{
// 			memcpy(addr,"127.0.0.1",strlen("127.0.0.1"));
// 		}
// 	}
// 	else
// 	{
// 		strcpy(addr, local_addr);
// 	}
// 	curl_print("func=%s simcid=%d nCid=%d nSimId=%d local addr=%s",func_name, simcid, nCid, nSimId, local_addr);
// }

static int curl_global_init_count = 0;
void quectel_curl_global_init(void)
{
	
	if(curl_global_init_count <= 0)
	{
		curl_global_init(CURL_GLOBAL_ALL);
		curl_global_init_count = 1;
	}
	else
		curl_global_init_count++;

	//curl_print("global_init_count =%d ",curl_global_init_count);	
}

void quectel_curl_global_cleanup(void)
{
	if(curl_global_init_count == 1)
	{
		curl_global_cleanup();
		curl_global_init_count = 0;
	}
	else
		curl_global_init_count--;
		
	//curl_print("global_init_count =%d ",curl_global_init_count);
}

