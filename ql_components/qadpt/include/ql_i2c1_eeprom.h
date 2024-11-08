#ifndef _QL_I2C1_EEPROM_H_
#define _QL_I2C1_EEPROM_H_


#define I2C1_TEST_DEBUG

#ifdef I2C1_TEST_DEBUG
#define ql_i2c_log               os_printf

#else
#define ql_i2c_log               os_null_printf
#endif


typedef UINT32                       QL_DD_HANDLE;

#define I2C1_TEST_LEGNTH         200
#define I2C1_TEST_SALVE_ID       0x50
#define I2C1_TEST_ADDR           0x00


typedef enum 
{ 
   STANDARD_MODE = 0, // Standard mode (100K) 
   FAST_MODE = 1, // fast mode (400K) 
} ql_i2c_mode_e; 


typedef enum 
{ 
	QL_I2C_SUCCESS = 0, 
	QL_I2C_INIT_ERR = 1, 
	QL_I2C_WRITE_ERR, 
	QL_I2C_READ_ERR, 
	QL_I2C_RELEASE_ERR, 
}ql_errcode_i2c_e;


int ql_I2cInit(QL_DD_HANDLE* i2c_hdl,ql_i2c_mode_e Mode);
int ql_I2cWrite(QL_DD_HANDLE i2c_hdl, UINT8 slave, UINT16 addr, char *data, UINT32 length,UINT8 addr_width);
int ql_I2cRead(QL_DD_HANDLE i2c_hdl, UINT8 slave, UINT16 addr, char *buf, UINT32 length,UINT8 addr_width);
int ql_I2cRelease(QL_DD_HANDLE i2c_hdl);
void ql_i2c1_eeprom_demo_thread(void *param);
void ql_i2c_demo_thread_creat(void);

#endif  // _I2C1_H_

