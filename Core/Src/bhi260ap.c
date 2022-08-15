/*
 * bhi260ap.c
 *
 *  Created on: Aug 6, 2022
 *      Author: westr
 */
#include <BHI260AP_APP30.h>
#include "bhi260ap.h"

void bhi_hwrite(uint8_t addr, uint8_t *data, uint32_t data_len)
{
	//	Send dummy byte. Hacky solution to problem. When SPI is not used, usually SCK should be idle in CPOL.
	//	However for this architecture, SCK goes into Hi-Z state. Even if pulling down with internal/external
	//	resistor, the levelshifter will interpret the idle signal as either high or low. Thus not fulfilling
	//	the criteria that when SCK goes active, SCK NEEDS to be {LOW for CPHA=CPOL=0} or {HIGH for CPHA=CPOL=1}
	HAL_GPIO_WritePin(BHI_SPI_CS_BUS, BHI_SPI_CS_PIN, HIGH);
	HAL_SPI_Transmit(&BHI_SPI, &(uint8_t){0x00}, 1, 100);

	// Set SCK active
	HAL_GPIO_WritePin(BHI_SPI_SCK_BUS, BHI_SPI_SCK_PIN, BHI_SPI_CPOL);

//	HAL_Delay(1); // tmp

	// Set CS active
	HAL_GPIO_WritePin(BHI_SPI_CS_BUS, BHI_SPI_CS_PIN, LOW);

	// Transmit address
	HAL_SPI_Transmit(&BHI_SPI, &(uint8_t){addr}, 1, 100);

	//	Transmit data
//	HAL_SPI_Transmit(&BHI_SPI, &(uint8_t){0x02}, data_len, 200);


	uint32_t tmp_len = data_len;
	uint8_t *dataptr;
	dataptr = &data[0];
	while(tmp_len > (( 1 << 16 ) - 1) )
	{
		HAL_SPI_Transmit(&BHI_SPI, dataptr, 65535, 100000);
		dataptr = &dataptr[65535];
		tmp_len -= 65535;
	}
	HAL_SPI_Transmit(&BHI_SPI, dataptr, tmp_len, 100000);


//	HAL_SPI_Transmit(&BHI_SPI, data, data_len, 20000000); // Original



	HAL_GPIO_WritePin(BHI_SPI_SCK_BUS, BHI_SPI_SCK_PIN, BHI_SPI_CPOL); /////// TEST

	//	Set CS inactive
	HAL_GPIO_WritePin(BHI_SPI_CS_BUS, BHI_SPI_CS_PIN, HIGH);

	HAL_Delay(1);
}

// This is only for fw image loaded through host interface
void wip_verify_fw()
{
	// TODO: Add error handler, we shouldn't stay here forever if there was an error
	while( (tmpread(BHI_BOOT_STATUS) & ( BOOT_STATUS_FW_VERIFIED | BOOT_STATUS_FW_VERIFY_ERR ) ) == BOOT_STATUS_FW_VERIFIED  );
}

uint8_t tmpread(uint8_t addr)
{
	//	Send dummy byte. Hacky solution to problem. When SPI is not used, usually SCK should be idle in CPOL.
	//	However for this architecture, SCK goes into Hi-Z state. Even if pulling down with internal/external
	//	resistor, the levelshifter will interpret the idle signal as either high or low. Thus not fulfilling
	//	the criteria that when SCK goes active, SCK NEEDS to be {LOW for CPHA=CPOL=0} or {HIGH for CPHA=CPOL=1}
	HAL_GPIO_WritePin(BHI_SPI_CS_BUS, BHI_SPI_CS_PIN, HIGH);
	HAL_SPI_Transmit(&BHI_SPI, &(uint8_t){0x00}, 1, 100);

	// Set SCK active
	HAL_GPIO_WritePin(BHI_SPI_SCK_BUS, BHI_SPI_SCK_PIN, BHI_SPI_CPOL); // Remove?

	// Set CS active
	HAL_GPIO_WritePin(BHI_SPI_CS_BUS, BHI_SPI_CS_PIN, LOW);

	// Transmit address
	HAL_SPI_Transmit(&BHI_SPI, &(uint8_t){addr|(1<<7)}, 1, 100);

	uint8_t tmp[2];
//	HAL_SPI_Receive(&BHI_SPI, tmp, 2, 100);
//	HAL_SPI_TransmitReceive(&BHI_SPI, (uint8_t[2]){0}, tmp, 2, 100);
//	HAL_SPI_TransmitReceive(&BHI_SPI, (uint8_t[2]){0x00}, tmp, 2, 100);

	HAL_SPI_TransmitReceive(&BHI_SPI, (uint8_t[1]){0x00}, tmp, 1, 100);

	HAL_GPIO_WritePin(BHI_SPI_SCK_BUS, BHI_SPI_SCK_PIN, BHI_SPI_CPOL); /////// TEST

	// Set CS inactive
	HAL_GPIO_WritePin(BHI_SPI_CS_BUS, BHI_SPI_CS_PIN, HIGH);
	HAL_Delay(1);
	return tmp[1];
}

void tmp_upload_ram()
{
	// test
//	uint8_t fw_single = bhy2_firmware_image[1];
//	uint8_t *fw = bhy2_firmware_image;
//	28040 words

	uint32_t length = 28040;

	uint8_t* full_cmd = malloc((length*4+3)*sizeof(uint8_t));

	uint16_t command_id = 0x0002;

	// Command ID LSB first
	full_cmd[0] = command_id & 0xff;
	full_cmd[1] = (command_id >> 8) & 0xff;

//	uint16_t length = 28040;

	//	Length, LSB first - who is the knob head that decides LSB first?
	full_cmd[2] = length & 0xff;
	full_cmd[3] = (length & 0xff00) >> 8;

	for(int i = 4; i <= length*4+3; i++){
//		full_cmd[i]=fw[i-4];
		full_cmd[i] = bhy2_firmware_image[i-4];
	}
//	bhi_hwrite(BHI_CMDINPUT, full_cmd, ( ( ( (length & 0xff) << 8) | (length & 0xff00) >> 8 ) + 4) );
	bhi_hwrite(BHI_CMDINPUT, full_cmd, ( ( length * 4 ) + 4) );


}

void tmpreadmsg(uint8_t addr)
{

	//	Send dummy byte. Hacky solution to problem. When SPI is not used, usually SCK should be idle in CPOL.
	//	However for this architecture, SCK goes into Hi-Z state. Even if pulling down with internal/external
	//	resistor, the levelshifter will interpret the idle signal as either high or low. Thus not fulfilling
	//	the criteria that when SCK goes active, SCK NEEDS to be {LOW for CPHA=CPOL=0} or {HIGH for CPHA=CPOL=1}
	HAL_GPIO_WritePin(BHI_SPI_CS_BUS, BHI_SPI_CS_PIN, HIGH);
	HAL_SPI_Transmit(&BHI_SPI, &(uint8_t){0x00}, 1, 100);

	// Set SCK active
	HAL_GPIO_WritePin(BHI_SPI_SCK_BUS, BHI_SPI_SCK_PIN, BHI_SPI_CPOL); // Remove?

	// Set CS active
	HAL_GPIO_WritePin(BHI_SPI_CS_BUS, BHI_SPI_CS_PIN, LOW);

	// Transmit address
	HAL_SPI_Transmit(&BHI_SPI, &(uint8_t){addr|(1<<7)}, 1, 100);
//	HAL_SPI_Transmit(&BHI_SPI, &(uint8_t){addr}, 1, 100);

	uint8_t len[2];
//	HAL_SPI_Receive(&BHI_SPI, tmp, 2, 100);
//	HAL_SPI_TransmitReceive(&BHI_SPI, (uint8_t[2]){0}, tmp, 2, 100);
	HAL_SPI_TransmitReceive(&BHI_SPI, (uint8_t[2]){0x00}, len, 2, 100);
	uint16_t tmplength;
	if(len[0] != 0 ||len[1] != 0){
		tmplength = (len[0] << 8) | ( len[1] );
//		uint8_t *testStorage = malloc(tmplength*sizeof(uint8_t));
		uint8_t testStorage[100];
		HAL_SPI_TransmitReceive(&BHI_SPI, (uint8_t[100]){0x00}, testStorage, 100, 90000);

	}

	HAL_GPIO_WritePin(BHI_SPI_SCK_BUS, BHI_SPI_SCK_PIN, BHI_SPI_CPOL); /////// TEST

	// Set CS inactive
	HAL_GPIO_WritePin(BHI_SPI_CS_BUS, BHI_SPI_CS_PIN, HIGH);
	HAL_Delay(1);
}


void cmd_input(uint16_t command_id, uint16_t length, uint8_t* content)
{
	// Commmand packet
	//	Bytes	[[	0-1		][	2-3	][	4-	..]]
	//	Name	[[COMMAND_ID][LENGTH][CONTENTS]]
//	uint8_t cid[2] = ((command_id&0xff)<<8)|((command_id&0xff00)>>8);
//	uint8_t len[2] = ((length&0xff)<<8)|((length&0xff00)>>8);
	uint8_t* full_cmd = malloc((length+3)*sizeof(uint8_t));

	// Command ID LSB first
	full_cmd[0] = command_id & 0xff;
	full_cmd[1] = (command_id >> 8) & 0xff;

	//	Length, LSB first - who is the knob head that decides LSB first?
	full_cmd[2] = length & 0xff;
	full_cmd[3] = (length & 0xff00) >> 8;

	for(int i = 4; i <= length+3; i++){
		full_cmd[i]=content[i-4];
	}
	bhi_hwrite(BHI_CMDINPUT, full_cmd, ((length & 0xff) + 4) );
}
