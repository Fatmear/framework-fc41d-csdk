#ifndef __FLASH_BYPASS_H__
#define __FLASH_BYPASS_H__

#ifdef __cplusplus
extern "C" {
#endif

#define CURRENT_ROUTINE_TEXT_SIZE            (0x400)

/*Write Enable for Volatile Status Register*/
#define FLASH_CMD_WR_EN_VSR                 (0x50)
/*Write Status Register*/
#define FLASH_CMD_WR_SR                     (0x01)

#define SPI_VAL_TAKE_CS                     (0x02)
#define SPI_VAL_RELEASE_CS                  (0x03)

extern uint32_t flash_bypass_operate_sr_init(void);

#ifdef __cplusplus
}
#endif
#endif //__FLASH_BYPASS_H__
// eof

