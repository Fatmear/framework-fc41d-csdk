#include "ql_include.h"


/***************wlan config network test******************/
 uint8_t wifi_config_ssid[32] = {0};
 uint8_t wifi_config_pwd[32] = {0};
 uint8_t uart_band[32] = {0};
 uint8_t wifi_config = 0;
 uint8_t wifi_uart_config = 0;

 ql_uart_config_s web_uart_test_cfg={QL_UART_BAUD_115200,QL_UART_DATABIT_8,QL_UART_PARITY_NONE,QL_UART_STOP_1,QL_FC_NONE};
 
 static void *current_connection;


 static ql_sem_t g_test_web_sem = NULL;

 static uint8_t g_wait_body_finish = 0;


 err_t httpd_post_begin(void *connection, const char *uri, const char *http_request,
                        u16_t http_request_len, int content_len, char *response_uri,
                        u16_t response_uri_len, u8_t *post_auto_wnd)
 {
     LWIP_UNUSED_ARG(connection);
     LWIP_UNUSED_ARG(http_request);
     LWIP_UNUSED_ARG(http_request_len);
     LWIP_UNUSED_ARG(content_len);
     LWIP_UNUSED_ARG(post_auto_wnd);
 
     if (!memcmp(uri, "/index.html", strlen("/index.html")))
     {

        // if (current_connection != connection)
        {
             current_connection = connection;
             snprintf(response_uri, response_uri_len, "/");

             /* e.g. for large uploads to slow flash over a fast connection, you should
                manually update the rx window. That way, a sender can only send a full
                tcp window at a time. If this is required, set 'post_aut_wnd' to 0.
                We do not need to throttle upload speed here, so: */
             *post_auto_wnd = 1;
             return ERR_OK;
         }
     }
     return ERR_VAL;
 }



 static int httpd_htoi(char *s)
 {
	 int value;
	 int c;

	 c = ((unsigned char *)s)[0];
	 if (isupper(c))
		 c = tolower(c);
	 value = (c >= '0' && c <= '9' ? c - '0' : c - 'a' + 10) * 16;

	 c = ((unsigned char *)s)[1];
	 if (isupper(c))
		 c = tolower(c);
	 value += c >= '0' && c <= '9' ? c - '0' : c - 'a' + 10;

	 return (value);
 }

 static int httpd_url_decode(char *str, int len)
 {
	 char *dest = str;
	 char *data = str;

	 while (len--)
	 {
		 if (*data == '+')
		 {
			 *dest = ' ';
		 }
		 else if (*data == '%' && len >= 2 && isxdigit((int)*(data + 1)) && isxdigit((int)*(data + 2)))
		 {
			 *dest = (char)httpd_htoi(data + 1);
			 data += 2;
			 len -= 2;
		 }
		 else
		 {
			 *dest = *data;
		 }
		 data++;
		 dest++;
	 }
	 *dest = '\0';
	 return dest - str;
 }

			 
						
err_t httpd_post_receive_data(void *connection, struct pbuf *p)

{
	#define LWIP_HTTPD_POST_MAX_PAYLOAD_LEN     512

	struct pbuf *q = p;


	u32_t http_post_payload_full_flag = 0;

						
	static char http_post_payload[LWIP_HTTPD_POST_MAX_PAYLOAD_LEN];
						
	static u16_t http_post_payload_len = 0;
	
	memset(http_post_payload, 0, LWIP_HTTPD_POST_MAX_PAYLOAD_LEN);
     memset(wifi_config_ssid,0,32);
     memset(wifi_config_pwd,0,32);
	 memset(uart_band, 0, 32);
	 
	http_post_payload_len = 0;
	while(q != NULL)  

	{
	 

	  if(http_post_payload_len + q->len <= LWIP_HTTPD_POST_MAX_PAYLOAD_LEN) {
	  	os_printf("lyy q_len:%d payload_len: %d \n", q->len, http_post_payload_len);
		  MEMCPY(http_post_payload+http_post_payload_len, q->payload, q->len);

		  http_post_payload_len += q->len;

	  }

	  else {  

		http_post_payload_full_flag = 1;

		break;

	  }

	  q = q->next;

	}

 

	pbuf_free(p); 
 

	if(http_post_payload_full_flag) 

	{
		http_post_payload_full_flag = 0;

		http_post_payload_len = 0;

	}

	char *ssid_start = strstr(http_post_payload,"ssid=") + strlen("ssid=");
    char *ssid_end = strstr((char *)ssid_start,"&");
	char *pwd_start = strstr(http_post_payload,"pwd=") + strlen("pwd=") ;
	char *str_uart_band = strstr(http_post_payload,"uart_band=") + strlen("uart_band=") ;
	 
	 if(strstr(p->payload,"ssid="))
	 {
		memcpy(wifi_config_ssid,ssid_start,ssid_end-ssid_start);
	 }

	 if(strstr(p->payload,"pwd="))
	 {
		 memcpy(wifi_config_pwd,pwd_start, strlen(pwd_start));
	 }

	if(strstr(p->payload,"uart_band="))
	{
		memcpy(uart_band, str_uart_band, strlen(str_uart_band));
	}

    if(strlen(wifi_config_ssid))
	{

	    httpd_url_decode(wifi_config_ssid, strlen(wifi_config_ssid));
	    httpd_url_decode(wifi_config_pwd, strlen(wifi_config_pwd));

		wifi_config = 1;
		os_printf("wifi_config_ssid:%s\n",wifi_config_ssid);
		os_printf("wifi_config_pwd:%s\n",wifi_config_pwd);

		if(g_test_web_sem)
		{
			ql_rtos_semaphore_release(g_test_web_sem);
		}
		g_wait_body_finish = 1;

	}
 
	if(strlen(uart_band))
	{

	    httpd_url_decode(uart_band, strlen(uart_band));

		wifi_uart_config = 1;
		os_printf("uart_band:%s\n",uart_band);

		if(g_test_web_sem)
		{
			ql_rtos_semaphore_release(g_test_web_sem);
		}
		g_wait_body_finish = 1;

	}	 

	return ERR_OK;

}


 void
 httpd_post_finished(void *connection, char *response_uri, u16_t response_uri_len)
 {

	int cnt = 0;
	while (g_wait_body_finish == 1)
	{
		ql_rtos_task_sleep_ms(100);
		cnt++;
		if(cnt > 20)
		{
			os_printf("timeout:%d\r\n", cnt);
			break;
		}
	}

    snprintf(response_uri, response_uri_len, "/");


 }

  

  
 void ql_wlan_cfgnet_demo(void)
 {
 	uint32 band, test;
	uint8  test_band[4];

	httpd_init();

	int ret = ql_rtos_semaphore_create(&g_test_web_sem, 10);



	if(ret != kNoErr)
	{
		os_printf("rtos_init_semaphore err:%d\r\n", ret);
	}

 	 while(1)
 	 {


		  if(g_test_web_sem)
		  {
			  ql_rtos_semaphore_wait(g_test_web_sem, BEKEN_WAIT_FOREVER);
			   os_printf("cbq get wifi config");
		  }

 		 if (wifi_config)
 		 {
			 
			 os_printf("cbq wifi_config");

			 ql_demo_sta_app_init(wifi_config_ssid, wifi_config_pwd);
	

 			  wifi_config = 0;

			   if(!wifi_uart_config)
			   {
				   g_wait_body_finish = 0;
			   }

 		 }

		 if (wifi_uart_config)
		 {
		 	memset(test_band, 0, sizeof(test_band));
			band = atoi(uart_band);
			if (((band > 100) && (band <= 2000000) ) )
			{
				web_uart_test_cfg.baudrate = band;

			    ql_uart_set_dcbconfig(UART_TEST_PORT,&web_uart_test_cfg);

	            ql_uart_open(UART_TEST_PORT);
				
				wifi_uart_config = 0;
			}

			g_wait_body_finish = 0;

		 }

 	 }
 }

 
ql_task_t	 wlan_cfgnet_thread_handle = NULL;
void ql_wlan_cfgnet_demo_thread_creat(void)
{
    int ret; 

	ql_rtos_task_sleep_ms(3000);

	ret = ql_rtos_task_create(&wlan_cfgnet_thread_handle,
						 (unsigned short)1024,
						 BEKEN_DEFAULT_WORKER_PRIORITY,
						 "wlan_cfgnet_test",
						 ql_wlan_cfgnet_demo,
						 0);
	
	
	if (ret != kNoErr) {
	  os_printf("Error: Failed to create wlan config net test thread: %d\r\n",ret);
	  goto init_err;
	}

  
		
	return;
	 
	init_err:
	 if( wlan_cfgnet_thread_handle != NULL ) {
		 ql_rtos_task_delete(wlan_cfgnet_thread_handle);
	 }

	

}


