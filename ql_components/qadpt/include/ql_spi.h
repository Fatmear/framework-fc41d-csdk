#ifndef _QL_SPI_H
#define _QL_SPI_H


#define SPI_TEST_DEBUG
#ifdef SPI_TEST_DEBUG
#define ql_spi_log               os_printf

#else
#define ql_spi_log               os_null_printf
#endif

#define SPI_TEST_LEN  100

#define spi_test_speed 10000000

typedef struct
{
    UINT8 *send_buf;
    UINT32 send_len;

    UINT8 *recv_buf;
    UINT32 recv_len;
}ql_spi_messag_s;

typedef enum
{
	 QL_SPI_SUCCESS = 0,
	 QL_SPI_EXECUTE_ERR,
} ql_spi_errcode_e;


typedef enum 
{
	 QL_SPI_MASTER =0,
	 QL_SPI_SLAVE,		 
}ql_spi_master_slave_mode_e;

typedef enum 
{
	 QL_SPI_MSB =0,
	 QL_SPI_LSB,		 
}ql_spi_transfer_mode_e;
	 

typedef enum
{
 QL_SPI_CPOL_LOW = 0,
 QL_SPI_CPOL_HIGH,
} ql_spi_cpol_pol_e;

typedef enum
{
 QL_SPI_CPHA_1Edge,
 QL_SPI_CPHA_2Edge,
}ql_spi_cpha_pol_e;


typedef struct
{
 UINT32 spiclk;
 ql_spi_cpol_pol_e cpol;
 ql_spi_cpha_pol_e cpha;
 ql_spi_transfer_mode_e transmode;
 ql_spi_master_slave_mode_e masterorslave;
} ql_spi_config_s;

ql_spi_errcode_e ql_spi_init(ql_spi_config_s spi_cfg,ql_spi_messag_s * spi_msg);
ql_spi_errcode_e ql_spi_transfer(ql_spi_messag_s * spi_msg);
void ql_spi_demo_thread_creat(void);




#endif 

