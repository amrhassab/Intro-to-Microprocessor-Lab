#ifndef __MCP25AA080D_H__
#define __MCP25AA080D_H__

#include <stdint.h>
#include "spi.h"

#define SPI_FIFO_DEPTH                8

#define MCP25AA080D_READ			   0x0003	
#define MCP25AA080D_WRITE			   0x0002
#define MCP25AA080D_WRDI			   0x0004	
#define MCP25AA080D_WREN			   0x0006
#define MCP25AA080D_RDSR			   0x0005	
#define MCP25AA080D_WRSR			   0x0001

#define MCP25AA080D_WIP_M			   ( 1 << 0 )

typedef struct {
	uint8_t 	inst;
	uint8_t		addr_hi;
	uint8_t		addr_low;
	uint8_t		data;
} SPI_EEPROM_BYTE_CMD;

typedef struct {
	uint8_t 	inst;
	uint8_t		data;
} SPI_EEPROM_CFG_CMD;

typedef struct {
	uint8_t 	inst;
} SPI_EEPROM_EN_CMD;

typedef struct {
	uint8_t 	inst;
	uint8_t		addr_hi;
	uint8_t		addr_low;
	uint8_t		data[SPI_FIFO_DEPTH];
} SPI_EEPROM_PAGE_CMD;



uint8_t mcp25AA080D_read_byte(uint32_t base, uint16_t address);
void mcp25AA080D_write_byte(uint32_t base, uint16_t address, uint8_t value);
void mcp25AA080D_read_page(uint32_t base, uint16_t address, uint8_t *buf, int size);
void mcp25AA080D_write_page(uint32_t base, uint16_t address, uint8_t *buf, int size);
uint8_t mcp25AA080D_read_status(uint32_t base);
void mcp25AA080D_write_status(uint32_t base, uint8_t value);
void mcp25AA080D_write_enable(uint32_t base);
void mcp25AA080D_write_disable(uint32_t base);
void mcp25AA080D_wait_write_in_progress(uint32_t base);
void mcp25AA080D_writeArray( uint32_t base,uint16_t address, char *msg, int16_t msg_size);
void mcp25AA080D_readArray(uint32_t base,uint16_t address, char *msg, int16_t msg_size);

#endif

