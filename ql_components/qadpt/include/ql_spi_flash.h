#ifndef _QL_SPI_FLASH_H
#define _QL_SPI_FLASH_H


#define SPI_FLASH_TEST_DEBUG
#ifdef SPI_FLASH_TEST_DEBUG
#define ql_spi_flash_log               os_printf

#else
#define ql_spi_flash_log               os_null_printf
#endif

#define SPI_TEST_FLASH_LEN  1024
#define SPI_TEST_FLASH_ADDR  0

typedef enum
{
	 QL_SPI_FLASH_SUCCESS = 0,
	 QL_SPI_FLASH_EXECUTE_ERR,
	 QL_SPI_FLASH_PARAM_ERROR,
} ql_errcode_spi_nor_e;




ql_errcode_spi_nor_e ql_spi_flash_read( unsigned char* data, unsigned int addr, unsigned int len);
ql_errcode_spi_nor_e ql_spi_flash_write(unsigned char *data, unsigned int addr, unsigned int len);
ql_errcode_spi_nor_e ql_spi_flash_erase_chip(void);
ql_errcode_spi_nor_e ql_spi_flash_erase_sector(unsigned int addr);
ql_errcode_spi_nor_e ql_spi_flash_erase_64k_block(unsigned int addr);

void ql_spi_flash_demo_thread_creat(void);


#endif 

