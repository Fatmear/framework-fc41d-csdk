#ifndef _BK_SPI_FLASH_H_
#define _BK_SPI_FLASH_H_


extern UINT32 spi_flash_dma_read_id(void);   
extern int spi_flash_dma_read(UINT32 addr, UINT32 size, UINT8 * dst);
extern int spi_flash_dma_write(UINT32 addr, UINT32 size, UINT8 * src);
extern int spi_flash_dma_erase(UINT32 addr, UINT32 size);
extern int spi_flash_dma_erase_page(UINT32 addr, UINT32 mode);
#endif //_BK_SPI_FLASH_H_