/*
 * bhi260ap.h
 *
 *  Created on: Aug 6, 2022
 *      Author: westr
 */

#ifndef INC_BHI260AP_H_
#define INC_BHI260AP_H_
#include "main.h"

#define LOW			GPIO_PIN_RESET
#define HIGH		GPIO_PIN_SET

/*******************************************
 * 			User ??? definitions
 *******************************************/
//#define BHI_SPI_MISO_PIN
//#define BHI_SPI_MISO_BUS
#define BHI_SPI					hspi1			/*!<	SPI peripheral for BHI260AP. Needs to be configured according to user setup.	*/
#define BHI_SPI_CS_BUS			GPIOD			/*!<	SPI Chip select bus for BHI260AP. Needs to be configured according to user setup. 	*/
#define BHI_SPI_CS_PIN			GPIO_PIN_14		/*!<	SPI Chip select pin for BHI260AP. Needs to be configured according to user setup.	*/
#define BHI_SPI_NRST_BUS		GPIOF			/*!<	SPI RST bus for BHI260AP. Needs to be configured according to user setup. 	*/
#define BHI_SPI_NRST_PIN		GPIO_PIN_3		/*!<	SPI RST pin for BHI260AP. Needs to be configured according to user setup.	*/
#define BHI_SPI_SCK_BUS			GPIOA			/*!<	SPI clock bus for BHI260AP. Needs to be configured according to user setup. 	*/
#define BHI_SPI_SCK_PIN			GPIO_PIN_5		/*!<	SPI clock pin for BHI260AP. Needs to be configured according to user setup.	*/

#define BHI_SPI_CPOL			LOW				/*!<	SPI CPOL BHI260AP. Needs to be configured according to user setup.	Note that CPOl=CPHA=X. When seq is initiated with falling edge of CS, SCK has to be X! */


/************************************************
 * 				Host interface map
 ***********************************************/
#define		BHI_CMDINPUT					0x00		/*!< 	BHI260AP : Host Channel 0 - Command input. WRITE-ONLY			*/
#define		BHI_WAKEUP						0x01		/*!< 	BHI260AP : Host Channel 1 - Wake-up FIFO output. Host can read virtual sensor data stored in W-U FIFO through this DMA channel. READ-ONLY			*/
#define		BHI_NONWAKEUP					0x02		/*!< 	BHI260AP : Host Channel 2 - Non-Wake-up FIFO output.  Host can read virtual sensor data stored in NON-W-U FIFO through this DMA channel. READ-ONLY		*/
#define		BHI_STATUS						0x03		/*!< 	BHI260AP : Host can read Status and Debug packets from FIFO through this DMA channel. READ-ONLY			*/
#define		BHI_CHIP_CTRL					0x05		/*!< 	BHI260AP : Chip Control			READ-WRITE */
#define		BHI_INTERFACE_CTRL				0x06		/*!< 	BHI260AP : Host interface control. Control various actions regarding host interface. READ-WRITE			*/
#define		BHI_INTERRUPT_CTRL				0x07		/*!< 	BHI260AP : Host interrupt control.			*/
#define		BHI_RST_REQ						0x14		/*!< 	BHI260AP : 			*/
#define		BHI_TIMESTAMP					0x15		/*!< 	BHI260AP : 			*/
#define		BHI_HOST_CTRL					0x16		/*!< 	BHI260AP : 			*/
#define		BHI_HOST_STATUS					0x17		/*!< 	BHI260AP : 			*/
#define		BHI_FUSER_PROD_ID				0x1C		/*!< 	BHI260AP : 			*/
#define		BHI_FUSER_REV_ID				0x1D		/*!< 	BHI260AP : 			*/
#define		BHI_FEATURE_STATUS				0x24		/*!< 	BHI260AP : 			*/
#define		BHI_BOOT_STATUS					0x25		/*!< 	BHI260AP : 			*/
#define		BHI_CHIP_ID						0x2B		/*!< 	BHI260AP : 			*/
#define		BHI_INTERRUPT_STATUS			0x2D		/*!< 	BHI260AP : 			*/
#define		BHI_ERROR_VAL					0x2E		/*!< 	BHI260AP : 			*/
#define		BHI_ERROR_AUX					0x2F		/*!< 	BHI260AP : 			*/
#define		BHI_DEBUG_VALUE					0x30		/*!< 	BHI260AP : 			*/
#define		BHI_DEBUG_STATE					0x31		/*!< 	BHI260AP : 			*/


#define 	BOOT_STATUS_FW_VERIFIED			(0x01 << 5)			/*!<	Bit position 5 register 0x25 (BOOT STATUS). Indicates whether the verification of the firmware image loaded via the host interface is done (1) or not done (0)	*/
#define 	BOOT_STATUS_FW_VERIFY_ERR		(0x01 << 6) 		/*!<	Bit position 6 register 0x25 (BOOT STATUS). Indicates whether the verification of the firmware image loaded via the host interface was successful (0) or contained an error (1)	*/
/*****************************************
 * 				FUNCTIONS
 ****************************************/
//void BHI_SPI(uint8_t data, uint8_t addr);
void bhi_hwrite(uint8_t addr, uint8_t *data, uint32_t data_len);			/*!<	BHI260AP Host write over SPI	*/

// Work in progress functions
void wip_verify_fw();
uint8_t tmpread(uint8_t addr);	// Almost done

void tmp_upload_ram();

/*
 * 		API
 */
void cmd_input(uint16_t command_id, uint16_t length, uint8_t* content);		/*!<	BHI260AP command input	*/

// External
extern SPI_HandleTypeDef hspi1;					/*!<	SPI channel for ADS1299	*/

#endif /* INC_BHI260AP_H_ */
