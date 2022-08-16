/*
 * bhi260ap.c
 *
 *  Created on: July, 2022 for dept. of Biomedical Engineering at Lunds University
 *      Author: Krippkrupp@github.com
 *      Version: n/a
 *      Required files:
 *      Description:
 *
 */
#include <BHI260AP_APP30.h>
#include "bhi260ap.h"

/**
 * 	WIP
 */
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

	// Set CS active
	HAL_GPIO_WritePin(BHI_SPI_CS_BUS, BHI_SPI_CS_PIN, LOW);

	// Transmit address
	HAL_SPI_Transmit(&BHI_SPI, &(uint8_t){addr}, 1, 100);

	//	Transmit data
	uint32_t tmp_len = data_len;
	uint8_t *dataptr;
	dataptr = &data[0];

	// Not possible to transmit more than 2^16-1 bit at once
	while(tmp_len > (( 1 << 16 ) - 1) )
	{
		HAL_SPI_Transmit(&BHI_SPI, dataptr, 65535, 100000);
		dataptr = &dataptr[65535];
		tmp_len -= 65535;
	}
	HAL_SPI_Transmit(&BHI_SPI, dataptr, tmp_len, 100000);


	HAL_GPIO_WritePin(BHI_SPI_SCK_BUS, BHI_SPI_SCK_PIN, BHI_SPI_CPOL); /////// TEST

	//	Set CS inactive
	HAL_GPIO_WritePin(BHI_SPI_CS_BUS, BHI_SPI_CS_PIN, HIGH);

//	HAL_Delay(1);	//0816
}

// This is only for fw image loaded through host interface
// TODO: Add error handler, we shouldn't stay here forever if there was an error
// Todo: This was used in upload_ram previously but will probably only be used once or twice. Remove function?
void wip_verify_fw()
{
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
//	HAL_Delay(1);	//0816
	return tmp[0];
}

/**
 * 	\brief	set_firmware_ram
 * 			Upload firmware to program RAM over Host interface on Host Channel 0 (Reg 0x00)
 * 	Once the uploaded RAM is verified, the Boot Program RAM command (0x0003 to Host Channel 0 Input Command) is issued.
 * 	Subsequent checks are done after uploading and booting, to ensure that the operations have been done successfully.
 * 	This function only uploads a specific firmware, with a predetermined length.
 * 	Usually the Length in the Command Packet refers to byte length, however for the Host Interface Command "Upload to Program RAM (0x0002)"
 * 	the Length field refers the total number of 32 bit words in the Content field. See section 13.1.2 in the Datasheet for more information.
 *
 *	The Firmware uploaded can be found in @file BHI260AP_APP30.h
 *
 *	TODO: Add
 *		- Meta Events i Wake-up och Non-wake-up (Currently reading Wake-Up and Non-Wake-Up, should we handle the values?)
 */
void set_firmware_ram()
{

//	28040 words

	//	Word length (32 bit words) of the firmware
	uint32_t length = 28040;
	length = sizeof(bhy2_firmware_image)/(sizeof(uint32_t));	// Divide by sizeof(uint32_t), i.e. 4, as length=total word length



//	uint8_t* full_cmd = malloc((length*4+3)*sizeof(uint8_t));	//Orig
	uint8_t* command_pkt = calloc( ( (length * 4) + 4 ), sizeof(uint8_t));

	//	Command ID 0x0002 = Upload to Program RAM
	uint16_t command_id = 0x0002;

	// Command ID LSB first
	command_pkt[0] = command_id & 0xff;
	command_pkt[1] = (command_id >> 8) & 0xff;

//	uint16_t length = 28040;

	//	Length, LSB first - who is the knob head that decides LSB first?
	command_pkt[2] = length & 0xff;
	command_pkt[3] = (length & 0xff00) >> 8;

	for(int i = 4; i <= length*4+3; i++){
//		full_cmd[i]=fw[i-4];
		command_pkt[i] = bhy2_firmware_image[i-4];
	}

	//	Send Host Reg 0x00 (Host ch0 Cmd Input) and Command Packet
	bhi_hwrite(BHI_CMDINPUT, command_pkt, ( ( length * 4 ) + 4) );

	/*		Note: The reason cmd_input isn't used for uploading RAM, is because length needs to changed to words instead of bytes	 */

	//	Command packet will not be used any more, free memory!
	free(command_pkt);

	//	Verify that the firmware has been uploaded by checking Host register Boot Status (0x25)
	//	TODO: Error handler, check X times before throwing error
	uint8_t tmp = 0;
	while( (tmpread(BHI_BOOT_STATUS) & ( BOOT_STATUS_FW_VERIFIED | BOOT_STATUS_FW_VERIFY_ERR ) ) != BOOT_STATUS_FW_VERIFIED  );

	// Boot firmware from RAM
	cmd_input(0x0003, 0, NULL);

	//	Wait for firmware to boot, by checking Host Interface Ready-bit in Boot Status (0x25)
	//	TODO: Error handler, check X times before throwing error
	while( (tmpread(BHI_BOOT_STATUS) & BOOT_STATUS_FW_IDLE ) == BOOT_STATUS_FW_IDLE );

	//	Clear Wake-up and Non-Wake-Up FIFO
	tmpreadmsg(0x01);
	tmpreadmsg(0x02);
}


/**
 * 	\brief Sets FIFO event on either Non-Wake-Up or Wake-Up
 * 			In order to be performed properly, the firmware has to be uploaded and booted prior
 * 			to executing set_fifo_event.
 *	Writes to Configure Sensor (0x000D) to Host Channel 0 (Reg 0x00) and sets
 *	i.e Virtual Sensor to @p FIFO_EVENT (Use defines @?????), which result can be read
 *	in Host Channel 1 - Wake-Up FIFO Output or Host Channel 2 - Non-Wake-up FIFO Output, depending on
 *	@p FIFO_TYPE.
 * 	@param SENSOR_ID Should be the sensor ID, which can be seen on page 173 in the datasheet or by using defined @????
 * 	@param FIFO_TYPE indicates whether the event should be Wake-up (Host channel 1) or Non-Wake-Up (Host channel 2), use predefined @?????
 *
 */ // TODO: Rewrite description, changed and added parameters and the way the function works..
void wip_set_fifo_event(uint8_t SENSOR_ID, float_t freq, uint32_t delay) // TODO: Bad naming? Maybe conf_sensor would be more appropriate
{
	//	Fixed length, 8 bytes
	uint16_t len = 8;
	uint8_t content[len];
	uint32_t freq_bitwise;
	freq_bitwise = *((uint32_t *)&freq);
	content[0] = SENSOR_ID; // TODO: Check if sensor ID is valid?

	//	Set frequency in Hz, 32 bit float.
	// 	TODO: 	Is the endianness of frequency correct? Nothing is mentioned in the datasheet
	//			"The sample rate field is specified as a 32 bit float, allowing for a very wide range of rates.
	//			The host can turn off a sensor by setting the rate to 0"
//	content[1] = ( ( (uint32_t) freq & 0xff000000 ) >> 24 );
//	content[2] = ( ( (uint32_t) freq & 0x00ff0000 ) >> 16 );
//	content[3] = ( ( (uint32_t) freq & 0x0000ff00 ) >>  8 );
//	content[4] = ( ( (uint32_t) freq & 0x000000ff ) >>  0 );


			// WHICH ENDIAN? Below felt extremely quick, but it's impossible to know..
		// Den ger ju en längd, men allting ser ju ut att vara xff typ?
//	content[1] = ( ( freq_bitwise & 0xff000000 ) >> 24 );
//	content[2] = ( ( freq_bitwise & 0x00ff0000 ) >> 16 );
//	content[3] = ( ( freq_bitwise & 0x0000ff00 ) >>  8 );
//	content[4] = ( ( freq_bitwise & 0x000000ff ) >>  0 );

	content[4] = ( ( freq_bitwise & 0xff000000 ) >> 24 );
	content[3] = ( ( freq_bitwise & 0x00ff0000 ) >> 16 );
	content[2] = ( ( freq_bitwise & 0x0000ff00 ) >>  8 );
	content[1] = ( ( freq_bitwise & 0x000000ff ) >>  0 );

	// 	Set delay/latency in ms
	// 	TODO: Is the endianness of delay correct? Nothing is mentioned in the datasheet
		// Parameter delay is 32 bit, but only 24 bits are valid
	if(delay > (( 1 << 24 )-1)){
		// TODO: Error handler?
		content[5] = 0xff;
		content[6] = 0xff;
		content[7] = 0xff;
	}else{
		content[5] = ( ( delay & 0x00ff0000 ) >> 16 );
		content[5] = ( ( delay & 0x00ff0000 ) >>  8 );
		content[5] = ( ( delay & 0x00ff0000 ) >>  0 );
	}

	cmd_input(HI_CMD_CONF_SENS, len, content);

}

/**
 * 	\brief WORK IN PROGRESS
 *
 */
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

//	uint8_t testStorage[100];	// Fix!
//	if(len[0] != 0 ||len[1] != 0){
//		tmplength = (len[0] << 8) | ( len[1] );
////		uint8_t *testStorage = malloc(tmplength*sizeof(uint8_t));
//
//		HAL_SPI_TransmitReceive(&BHI_SPI, (uint8_t[100]){0x00}, testStorage, 100, 90000);
//
//	}

	uint16_t FIFO_length = ( len[1] << 8 ) | ( len[0] );

	uint8_t *FIFO_data = calloc(FIFO_length, sizeof(uint8_t));

	HAL_SPI_Receive(&BHI_SPI, FIFO_data, FIFO_length, 500000);

//	uint8_t* command_packet = calloc( (cmd_pkt_len), sizeof(uint8_t) );

	HAL_GPIO_WritePin(BHI_SPI_SCK_BUS, BHI_SPI_SCK_PIN, BHI_SPI_CPOL); /////// TEST

	// Set CS inactive
	HAL_GPIO_WritePin(BHI_SPI_CS_BUS, BHI_SPI_CS_PIN, HIGH);
//	HAL_Delay(1);	//0816

	free(FIFO_data);
}


/*!
 * 	\brief Command input for Host Channel 0, (register 0x00)
 * 			Transmits the command including content and length in correct endianness to BHI.
 *
 * 	Host Channel 0 allows write-only access. The structure of a Command Packet contains
 * 	{ [Command ID, 2 bytes - LSByte first] |  [ Length, 2 bytes - LSByte first ] | [ Content ] }
 * 	The Command Packet is to be transmitted to Host Channel 0 register (0x00), this is added automatically
 *	 as a prefix to the command packet.
 *	 Note that the Command packet has to be a multiple of 4 bytes. If the command length does not end on a
 *	 4 byte boundary, the command must be padded out with zeroes to fulfill the structure. This is done automatically.
 *
 *	 The endianness of the Command ID and Length is important, the endianness is switched automatically.
 *	 @param command_id Command ID in normal endianness (i.e MSByte first), exactly as mentioned in the datasheet.
 *	 See Section 13 in datasheet for Command ID and their descriptions.
 *	 @param length The length of @p content. Note that it is not equal to the Command Packets length! This has been done
 *	 in order to simplify the coding for the end user. For @p content with length X, @p length should be X.
 */
void cmd_input(uint16_t command_id, uint16_t length, uint8_t* content)
{

	/*	 _______________________________________________________
	 * 	|		Host Channel 0 Command Packet incl Host Reg		|
	 * 	|_______________________________________________________|
	 *	|		  		|	command_packet (local pointer)		|
	 *	|_______________|_______________________________________|
	 * 	|	Host Reg	|	Command ID	|	Length	|	Content	|
	 * 	 -------------------------------------------------------
	 */


	//	Total Command Packet length in Bytes (Command ID + Length + Content)
	uint32_t cmd_pkt_len = length+3;

	//	Command Packet has to be a multiple of 4
	while( (cmd_pkt_len % 4) !=0){cmd_pkt_len++;}

	//	Allocate memory for the entire (except for Host Interface Register) command
	uint8_t* command_packet = calloc( (cmd_pkt_len), sizeof(uint8_t) );
//	uint8_t* command_packet = malloc((length+3)*sizeof(uint8_t)); // Old, remove

	// Command ID LSByte first
	command_packet[0] = command_id & 0xff;
	command_packet[1] = (command_id >> 8) & 0xff;

	//	Length, LSByte first - who is the knob head that decides LSByte first?
	command_packet[2] = length & 0xff;
	command_packet[3] = (length & 0xff00) >> 8;

	//	Fill full_cmd with content. Note that content should be in MSByte first ()
	for(int i = 4; i <= (length + 3); i++){
		command_packet[i] = content[i-4];
	}

	bhi_hwrite(BHI_CMDINPUT, command_packet, (length  + 4) );
//	bhi_hwrite(BHI_CMDINPUT, full_cmd, ((length & 0xff) + 4) );

	free(command_packet);
}
