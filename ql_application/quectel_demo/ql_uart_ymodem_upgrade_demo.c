


#include "include.h"
#include "rtos_pub.h"
#include "error.h"
#include "uart_pub.h"
#include "drv_model_pub.h"
#include "quectel_demo_config.h"
#include "BkDriverFlash.h"
#include "ql_uart.h"
#include "ql_flash.h"
#include "ql_api_osi.h"

#if CFG_ENABLE_QUECTEL_UART_UPGRADE

#define SOH 0x01  
#define STX 0x02  
#define EOT 0x04  
#define ACK 0x06  
#define NAK 0x15  


#define SEND_ACK        ql_uart_write(QL_UART_PORT_1,&ack,1)
#define SEND_NACK       ql_uart_write(QL_UART_PORT_1,&nack,1)
#define SEND_C          ql_uart_write(QL_UART_PORT_1,"C",1)

#define FLASH_ERASE(hd,parm)                 	GLOBAL_INT_DISABLE();\
	                                    ddev_control(hd, CMD_FLASH_ERASE_SECTOR, (void *)&parm);\
	                                    GLOBAL_INT_RESTORE();

#define FLASH_WRITE(hd,w_buffer,parm)                       GLOBAL_INT_DISABLE();\
                                          ddev_write(hd, (char *)w_buffer+3, 1024, parm);\
                                           ql_uart_log("Write flash[addr:0x%x len:0x%x]\r\n", parm, 1024);\
                                            GLOBAL_INT_RESTORE();

#define FALSH_READ(hd,buf,parm)      GLOBAL_INT_DISABLE();\
                                 ddev_read(hd, buf, 1024, parm);\
                                 GLOBAL_INT_RESTORE(); 
#define PACKET_DATA_SIZE      1024
#define PACKET_STX_SEZE      1029
#define PACKET_SOH_SEZE      133
                                


GLOBAL_INT_DECLARATION();
char ack=ACK;
char nack=NAK;

unsigned char buffer[1100]={0};

int uret=0;
DD_HANDLE f_hd=0;
bk_logic_partition_t *ota_info=NULL;
static void uart_rx_test_callback1(int uport, void *param)
{
   uret=bk_uart_get_length_in_buffer(QL_UART_PORT_1);
}
//uart init
static void uart_init1(void)
{

   ql_uart_config_s uart_test_cfg;
   uart_test_cfg.baudrate = QL_UART_BAUD_115200;
   uart_test_cfg.data_bit = QL_UART_DATABIT_8;
   uart_test_cfg.parity_bit = QL_UART_PARITY_NONE;
   uart_test_cfg.stop_bit = QL_UART_STOP_1;
   uart_test_cfg.flow_ctrl = QL_FC_NONE;
   ql_uart_set_dcbconfig(QL_UART_PORT_1,&uart_test_cfg);
   ql_uart_open(QL_UART_PORT_1);
   ql_uart_set_rx_cb(QL_UART_PORT_1, uart_rx_test_callback1);


}

unsigned short crc16_ccitt(const unsigned char *data, size_t length) {  
    int crc =0;
    int i,j;
    for(i=0;i<length;i++)
    {
        crc=crc^(data[i]<<8);
        for(j=0;j<8;j++)
        {
            if((crc &((int)0x8000))!=0)
            {
                crc = ((crc<<1) ^ 0x1021);
            }
            else
            {
                crc = crc << 1;
            }
        }
    }
    return (crc&0xFFFF); 
}

unsigned char sequence_num=1;
int ymodem_receive_packet(unsigned char *recvbuf)
{

   unsigned short p_crc;
   unsigned short r_crc;
   unsigned char temp_num;

   switch (*(recvbuf))
   {
   case EOT:   // Data transfer complete, restart the device
     { 
      ql_uart_log("EOT!!! \r\n");
      SEND_NACK;
      ql_rtos_task_sleep_ms(5);
      SEND_ACK;
      SEND_C;
      ql_rtos_task_sleep_ms(100);
      SEND_ACK;
      ddev_close(f_hd);

	   bk_flash_enable_security(FLASH_UNPROTECT_LAST_BLOCK);
      bk_reboot();
      break;
     }
   case SOH:    
     { 
      ql_uart_log("SOH!!! \r\n");
      ql_uart_log("begin data receive  success!!! \r\n");     
      break;
     }
   case STX:     //data packet
     { 
      ql_uart_log("STX!!! \r\n");
      temp_num=*(recvbuf+1);
      if(temp_num!=sequence_num)
      {
         ql_uart_log("error sequence number!!! \r\n");
         SEND_NACK;
         return -1;
      }
      sequence_num++;
      p_crc=crc16_ccitt(recvbuf+3,PACKET_DATA_SIZE);
      
      r_crc=*(recvbuf+PACKET_DATA_SIZE+3)<<8 | *(recvbuf+PACKET_DATA_SIZE+3+1);
      
         if(r_crc!=p_crc)
      {
         ql_uart_log("data packet crc error!!! \r\n");
         SEND_NACK;
         return -1;
      }
      else
      {
         ql_uart_log("data packet crc success!!! \r\n");
         return 0;
      }
      break;
     }

   default:
      ql_uart_log("unknown data packet  \r\n");
      SEND_NACK;
      return -1;
      break;
   }

   return 0;

}

int receive_packet(unsigned char* data) { 
   int uret=0; 
   memset(buffer,0,1100);
   uret=bk_uart_get_length_in_buffer(QL_UART_PORT_1);//Gets the fifo readable data length
   if(uret==0)
   {
   rtos_delay_milliseconds(1000);
   uret=bk_uart_get_length_in_buffer(QL_UART_PORT_1);//Gets the fifo readable data length
   ql_uart_log("uret2 is %d \r\n",uret);
   }
   int ret=ql_uart_read(QL_UART_PORT_1, buffer,uret);
   ql_uart_log("uret is %d \r\n",uret);
   ql_uart_log("buffer[0][1][2] is : %x %x %x\r\n",buffer[0],buffer[1],buffer[2]);
   if(ret==-1||uret>1029)
   {
      return -1;
   }
  else
  {
      return 0;  
  }

} 

void ql_uart_upgrade_main()
{
   int status=0;
   int num1=0;
   UINT32 start_addr,param;
//Open the flash device
   GLOBAL_INT_DISABLE();
   f_hd=ddev_open(FLASH_DEV_NAME, &status, 0);
   GLOBAL_INT_RESTORE();
//Unlock flash   
   bk_flash_enable_security(FLASH_PROTECT_NONE);
   ota_info=bk_flash_get_info(BK_PARTITION_OTA);//
   start_addr=ota_info->partition_start_addr;//Get the first address of the ota partition
   ql_rtos_task_sleep_ms(3000);
   ql_uart_log("***********demo test start \n");
   uart_init1();

   SEND_C;
   ql_uart_log("send C to host and wait for response\r\n \n");
while(1)//Waiting soh package
{
   num1++;
   ql_rtos_task_sleep_ms(1000);   
if(receive_packet(buffer)==0)
{
   ymodem_receive_packet(buffer);
   break;
}
if(num1>10)
{
   ql_uart_log(" timerout\r\n");
   return;
}
   ql_uart_log(" waiting \r\n");

}

   ql_uart_write(QL_UART_PORT_1,&ack,1);
   ql_rtos_task_sleep_ms(10);
   SEND_C;
   int num=0;
while(1)//packet received
{
   char wr_buf[PACKET_DATA_SIZE]={0};
   while (uret!=PACKET_STX_SEZE)//Wait for enough packets to be received
   {
       ql_rtos_task_sleep_ms(5);
      uret=bk_uart_get_length_in_buffer(QL_UART_PORT_1);
      if(uret==1)
      {
         break;
      }
   }
   uret=0;
   if(receive_packet(buffer)!=0)
   {
      SEND_NACK;
      continue;
   }
   unsigned char result=0;
   param=start_addr;
   param=param+num*PACKET_DATA_SIZE;//Update flash address
   result=ymodem_receive_packet(buffer);
   if(result==-1)
   {
      continue;
   }
   if (param % 0x1000==0||num==0) {

   FLASH_ERASE(f_hd,param);//Erase flash
	}
   FLASH_WRITE(f_hd,buffer,param);//Write ota partition
   num++;

   FALSH_READ(f_hd,wr_buf,param);
//Check that the write is correct
   if (!os_memcmp(wr_buf, buffer+3, PACKET_DATA_SIZE)) {
     ql_uart_log("flash write success !!!\r\n");
     SEND_ACK;
     ql_rtos_task_sleep_ms(10);
   }
   else
   {
      ql_uart_log("flash write error !!!\r\n");
      return ;
   }
}

}


ql_task_t	uart_upgrade_test_thread_handle = NULL;
void ql_uart_upgrade_demo_thread_creat(void)
{
    int ret;
	ret = ql_rtos_task_create(&uart_upgrade_test_thread_handle,
		               (unsigned short)4096,
					   THD_EXTENDED_APP_PRIORITY,
					   "uart_upgrade_test",
					   ql_uart_upgrade_main,
					   0);


	  if (ret != kNoErr) {
		ql_uart_log("Error: Failed to create uart upgrade test thread: %d\r\n",ret);
		goto init_err;
	  }

	  return;

	  init_err:
	   if( uart_upgrade_test_thread_handle != NULL ) {
		   ql_rtos_task_delete(uart_upgrade_test_thread_handle);
	   }

}



#endif


