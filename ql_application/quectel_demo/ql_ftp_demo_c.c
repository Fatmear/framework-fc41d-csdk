#include "include.h"
#include "rtos_pub.h"
#include "error.h"
#include "uart_pub.h"
#include "drv_model_pub.h"
#include "quectel_demo_config.h"
#include "ql_api_osi.h"
#include "quectel_curl_io.h"
#include "ql_ftp_client_c.h"


ql_task_t	ftp_test_thread_handle = NULL;


size_t  ql_ftp_transmit_data_cb(void *ptr, size_t size, size_t nmemb, void *stream)
{
   uint16_t len = size * nmemb;
   char*   str = os_malloc(len) ;

   os_printf("len:%d\r\n", len);
   if (NULL != str)
   {
      memcpy(str, ptr, len);
      os_printf("%s\r\n", str);
      os_free(str);
   }
   return len;
}

size_t  ql_ftp_upload_data_cb(void *ptr, size_t size, size_t nmemb, void *stream)
{
   uint16_t len = size * nmemb;
   static uint8_t count = 0;
   char data[10] = {0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39};

   os_printf("==========>len:%d\r\n", len);
   memcpy(ptr, data, 10);
   len = 10;
   os_printf("%s\r\n", ptr);

   count++;
   if (count > 10)
   {
      len = 0;
      count = 0;
   }


   return len;
}


void ql_ftp_demo_thread(void *param)
{
   ql_ftp_context_ptr ctx_p = NULL;
   char data[128] = {0};
   uint16_t len = 0;
   double size = 0;
   int ret =0;
   int last_err_code = 0;
   int error_code = 0;
   ql_demo_sta_app_init("zxcvbnm", "12345678");
   do
   {
      ql_rtos_task_sleep_ms(1000);
      os_printf("connectting");
   } while (0 == sta_ip_is_start());

   os_printf("~~~~~~~~creat ftp client\r\n");
   ctx_p = ql_ftp_client_new_c();
   if (NULL == ctx_p)
   {
      os_printf("creat ftp client fail");
   }
   os_printf("~~~~~~~~open ftp client\r\n");
   ret = ql_ftp_client_open_c(ctx_p, "abc","12345678", "10.55.25.104", 21); // HOSTNAME should be set
   if (0 != ret)
   {
      os_printf("open fail\r\n");
   }
   os_printf("~~~~~~~~open ftp client  success\r\n");

   ql_ftp_client_mlsd_c(ctx_p, "include", ql_ftp_transmit_data_cb, &len);
   ql_ftp_client_nlst_c(ctx_p, "FC41D.txt", ql_ftp_transmit_data_cb, &len);
   ql_ftp_client_mkd_c(ctx_p, "fc41D");
   ql_ftp_client_rmd_c(ctx_p, "fc41D");
   ql_ftp_client_cwd_c(ctx_p, "include");

   ret = ql_ftp_client_put_c(ctx_p, "test.txt", ql_ftp_upload_data_cb, &len, 0);
   if (0 != ret)
   {
      last_err_code = ql_ftp_client_get_last_error_c(ctx_p);
      error_code = ql_ftp_client_get_error_code(ret);
      os_printf("last_err_code:%d,error_code:%d\r\n", last_err_code, error_code);
   }

   ret = ql_ftp_client_get_c(ctx_p, "test.txt", ql_ftp_transmit_data_cb, &len, 0);
   if (0 != ret)
   {
      last_err_code = ql_ftp_client_get_last_error_c(ctx_p);
      error_code = ql_ftp_client_get_error_code(ret);
      os_printf("last_err_code:%d,error_code:%d\r\n", last_err_code, error_code);
   }

   ql_ftp_client_rename_c(ctx_p, "test.txt", "FC41D.txt");
   ql_ftp_client_delete_c(ctx_p, "FC41D.txt");
   ql_ftp_client_cwd_c(ctx_p, "/");
   ql_ftp_client_mdtm_c(ctx_p, "FC41D.txt", data);
   os_printf("~~~~~~~~time:%s\r\n", data);
   memset(data, 0, 128);
   ql_ftp_client_pwd_c(ctx_p, data);
   os_printf("~~~~~~~~pwd:%s\r\n", data);

   ql_ftp_client_size_c(ctx_p, "FC41D.txt", &size);
   os_printf("~~~~~~~~size:%0.0f\r\n", size);

   ret = ql_ftp_client_list_c(ctx_p, "/", "FC41D.txt",ql_ftp_transmit_data_cb, &len);
   if (0 != ret)
   {
      last_err_code = ql_ftp_client_get_last_error_c(ctx_p);
      error_code = ql_ftp_client_get_error_code(ret);
      os_printf("last_err_code:%d,error_code:%d\r\n", last_err_code, error_code);
   }

   ql_rtos_task_sleep_ms(20000);
   ql_ftp_client_close_c(ctx_p);
   ql_ftp_client_cleanup_c(ctx_p);
   os_printf("~~~~~~~~close ftp client\r\n");
   ql_rtos_task_delete(ftp_test_thread_handle);
}


void ql_ftp_demo_thread_creat(void)
{
   int ret;
	ret = ql_rtos_task_create(&ftp_test_thread_handle,
					   (unsigned short)1024*20,
					   THD_EXTENDED_APP_PRIORITY,
					   "ftp_test",
					   ql_ftp_demo_thread,   
					   0);


   if (ret != kNoErr) {
      os_printf("Error: Failed to create adc test thread: %d\r\n",ret);
      goto init_err;
   }

   return;

   init_err:
   if( ftp_test_thread_handle != NULL ) {
      ql_rtos_task_delete(ftp_test_thread_handle);
   }
}

























