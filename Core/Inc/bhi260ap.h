/*
 * bhi260ap.h
 *
 *  Created on: Aug 6, 2022
 *      Author: westr
 */

#ifndef INC_BHI260AP_H_
#define INC_BHI260AP_H_
#include "main.h"
#include <stdlib.h>

#define LOW			GPIO_PIN_RESET
#define HIGH		GPIO_PIN_SET
#define FALSE		0x00


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

/* Bit map, register 0x25 BOOT STATUS */
#define 	BOOT_STATUS_FW_VERIFIED			(0x01 << 5)			/*!<	Bit position 5 register 0x25 (BOOT STATUS). Indicates whether the verification of the firmware image loaded via the host interface is done (1) or not done (0)	*/
#define 	BOOT_STATUS_FW_VERIFY_ERR		(0x01 << 6) 		/*!<	Bit position 6 register 0x25 (BOOT STATUS). Indicates whether the verification of the firmware image loaded via the host interface was successful (0) or contained an error (1)	*/
#define 	BOOT_STATUS_FW_IDLE				(0x01 << 7) 		/*!<	Bit position 7 register 0x25 (BOOT STATUS). Indicates whether the firmware is idle (1) or running (0)	*/


/**********************************************************************
 *		 				HOST INTERFACE COMMANDS
 *			 					@HI_CMD
 * 		Set Parameter commands available in datasheet, section 13.3
 **********************************************************************/
#define		HI_CMD_POSTMORTEM_DATA			0x0001				/*!<	Download post mortem data. 	Available in Bootloader. Response packet on Status Channel (Host Channel 3):	Crash Dump Status Packet		*/
#define		HI_CMD_UPLOAD_PROG_RAM			0x0002				/*!<	Upload firmware to Program RAM.	Available in Bootloader. Response packet on Status Channel (Host Channel 3):	None		*/
#define		HI_CMD_BOOT_PROG_RAM			0x0003				/*!<	Boot firmware from program RAM.	Available in Bootloader. Response packet on Status Channel (Host Channel 3):	None		*/
#define		HI_CMD_ERASE_FLASH				0x0004				/*!<	Erase toexternal flash.	Available in Bootloader. Response packet on Status Channel (Host Channel 3):	Flash Erase Complete Status Packet		*/
#define		HI_CMD_WRITE_FLASH				0x0005				/*!<	Write to external flash.	Available in Bootloader. Response packet on Status Channel (Host Channel 3):	Flash Write Complete Status Packet		*/
#define		HI_CMD_BOOT_FLASH				0x0006				/*!<	Boot from external flash.	Available in Bootloader. Response packet on Status Channel (Host Channel 3):	None		*/
#define		HI_CMD_SENS_INJ_MODE			0x0007				/*!<	Set sensor data injection mode (Requires dedicated firmware version). Used for debug and testing.	Available in Framework. Response packet on Status Channel (Host Channel 3):		Injected Sensor Configuration Request Status Packet	*/
#define		HI_CMD_INJ_SENS_DATA			0x0008				/*!<	Inject sensor data.	Available in Framework. Response packet on Status Channel (Host Channel 3):		None	*/
#define		HI_CMD_FIFO_FLUSH				0x0009				/*!<	Flush FIFO. The host can request the BHI260AP at any time to initiate a data transfer of the content in one or more FIFOs from BHI to the host. Can also be used to discard data from any FIFO.	Available in Framework. Response packet on Status Channel (Host Channel 3):			*/
#define		HI_CMD_SOFTPASS					0x000A				/*!<	Soft pass-through can be used to read/write registers on devices attached to BHI260APs secondary master I2C or SPI busses.	Available in Framework. Response packet on Status Channel (Host Channel 3):		Soft Pass-Through Results Status Packet	*/
#define		HI_CMD_REQ_SELFTEST				0x000B				/*!<	Request sensor self-test.	Available in Framework. Response packet on Status Channel (Host Channel 3):		Sensor Self-Test Results Status Packet	*/
#define		HI_CMD_REQ_OFFSET_COMP			0x000C				/*!<	Request that a specific sensor runs fast offset compensation.	Available in Framework. Response packet on Status Channel (Host Channel 3):		Sensor FOC Results Status Packet	*/
#define		HI_CMD_CONF_SENS				0x000D				/*!<	Configure sensor. Used to activate/deactivate/change parameters of a specific sensor .	Available in Framework. Response packet on Status Channel (Host Channel 3):		None	*/
#define		HI_CMD_SENS_DYN_RANGE			0x000E				/*!<	Set dynamic range for a virtual sensor.	Available in Framework. Response packet on Status Channel (Host Channel 3):		None	*/
#define		HI_CMD_SET_CHANGE_SENSITIVITY	0x000F				/*!<	Set change sensitivity.	Available in Framework. Response packet on Status Channel (Host Channel 3):		None	*/
#define		HI_CMD_DEBUG_TEST				0x0010				/*!<	Debug Test.	Available in Framework. Response packet on Status Channel (Host Channel 3):		None	*/
#define		HI_CMD_DUT_CONT					0x0011				/*!<	DUT Continue.	Available in Framework. Response packet on Status Channel (Host Channel 3):		DUT Test Status Packet	*/
#define		HI_CMD_DUT_START_TEST			0x0012				/*!<	DUT Start Test.	Available in Framework. Response packet on Status Channel (Host Channel 3):		DUT Test Status Packet	*/
#define		HI_CMD_CTRL_FIFO_FORMAT			0x0015				/*!<	Change format and amount of timestamps placed in wake-up and non-wake-up FIFOs.	Available in Framework. Response packet on Status Channel (Host Channel 3):		None	*/
#define		HI_CMD_RAISE_HI_SPEED			0x0017				/*!<	Raise Host Interface Speed.	Available in Bootloader. Response packet on Status Channel (Host Channel 3): Raise Host Interface Speed Status Packet			*/

/**
 * 	FIFO EVENTS, to be used in set_fifo_event
 */

/*****************************************
 * 				FUNCTIONS
 ****************************************/
//void BHI_SPI(uint8_t data, uint8_t addr);
void bhi_hwrite(uint8_t addr, uint8_t *data, uint32_t data_len);			/*!<	BHI260AP Host write over SPI	*/
void set_firmware_ram();													/*!<	BHI260AP Uploads and boots predetermined firmware (BHI260AP_APP30) to the program RAM	*/

// Work in progress functions
void wip_verify_fw();
uint8_t tmpread(uint8_t addr);	// Almost done
void tmpreadmsg(uint8_t addr);
void wip_set_fifo_event(uint8_t SENSOR_ID, float_t freq, uint32_t delay);

/*
 * 		API
 */
void cmd_input(uint16_t command_id, uint16_t length, uint8_t* content);		/*!<	BHI260AP command input	*/

// External
extern SPI_HandleTypeDef hspi1;					/*!<	SPI channel for ADS1299	*/

#endif /* INC_BHI260AP_H_ */
