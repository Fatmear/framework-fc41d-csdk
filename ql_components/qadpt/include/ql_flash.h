#ifndef _QL_FLASH_H
#define _QL_FLASH_H


#define FLASH_TEST_DEBUG
#ifdef FLASH_TEST_DEBUG
#define ql_flash_log               os_printf

#else
#define ql_flash_log               os_null_printf
#endif

#define TEST_FLASH_LEN  256
#define TEST_FLASH_ADDR  0x132000

typedef enum
{
	 QL_FLASH_SUCCESS = 0,
	 QL_FLASH_EXECUTE_ERR,
	 QL_FLASH_PARAM_ERROR,
} ql_errcode_flash_e;

typedef enum
{
    QL_FLASH_PROTECT_NONE = 0,
    QL_FLASH_PROTECT_ALL,
    QL_FLASH_PROTECT_HALF,
    QL_FLASH_UNPROTECT_LAST_BLOCK,
} ql_flash_protect_type_e;


typedef enum
{
    QL_PARTITION_BOOTLOADER = 0,
    QL_PARTITION_APPLICATION,
    QL_PARTITION_OTA,
    QL_PARTITION_RF_FIRMWARE,
    QL_PARTITION_NET_PARAM,
    QL_PARTITION_FAST_CONNECT,
    QL_PARTITION_USR_CONFIG,
    QL_PARTITION_BLE_BONDING_FLASH,
    QL_PARTITION_USR_RESERVE,
    QL_PARTITION_MAX,
} ql_partition_t;


ql_errcode_flash_e ql_flash_set_security(ql_flash_protect_type_e type);
ql_errcode_flash_e ql_flash_write(unsigned char *data, unsigned int addr, unsigned int len);
ql_errcode_flash_e ql_flash_read(unsigned char *data, unsigned int addr, unsigned int len);
ql_errcode_flash_e ql_flash_erase(unsigned int addr, unsigned int len);

ql_errcode_flash_e ql_flash_parttion_erase(ql_partition_t inPartition, uint32_t off_set, uint32_t size);
ql_errcode_flash_e ql_flash_parttion_write( ql_partition_t inPartition, volatile uint32_t off_set, uint8_t *inBuffer , uint32_t inBufferLength);
ql_errcode_flash_e ql_flash_parttion_read( ql_partition_t inPartition, volatile uint32_t off_set, uint8_t *outBuffer, uint32_t inBufferLength);

void ql_flash_demo_thread_creat(void);



#endif 


