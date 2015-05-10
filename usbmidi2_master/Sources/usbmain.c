#include <string.h>

#include "USB1.h"
#include "WAIT1.h"
#include "USBMIDI1.h"
#include "COREUART.h"
#include "MIDIUART1.h"
#include "LED1.h"
#include "PTD.h"
#include "SlaveReset.h"
#include "KIN1.h"

#include "midiflash.h"

void SoftwareReset(void)
{
	KIN1_SoftwareReset();
}

extern int slave_initialized;
extern int sysex_flashing;
extern int sysex_started;

void FlashFailed(int warning)
{
	int infinite = 1;
	int repeats = 0;
	if (warning) {
		infinite = 0;
		repeats = 3;
	}

	while(infinite || repeats > 0) {
		LED1_On();
		WAIT1_Waitms(200);
		LED1_Off();
		WAIT1_Waitms(200);

		if (!infinite) {
			repeats--;
		}
	}
}

#if 0
int slaveresetticks = 0;

void SlaveResetMode(int mode)
{
	// PTD7: UART0_TX
	// Slave expects boot mode selection
	if (mode) {
		// normal boot, set pin
		GPIO_PDD_SetPortDataOutputMask(PTD_BASE_PTR, (1 << 7));
	}
	else {
		// stop in boot loader, clear pin
		GPIO_PDD_ClearPortDataOutputMask(PTD_BASE_PTR, (1 << 7));
	}
}

int IsSlaveReady()
{
	// PTD7: UART0_RX
	// Expect slave to pull pin high when ready.
	uint32_t result = GPIO_PDD_GetPortDataInput(PTD_BASE_PTR);
	return (result & (1 << 6)) != 0;
}

byte ReadCoreUartByte()
{
	byte databyte = 0;
	while (COREUART_RecvChar(&databyte) == ERR_RXEMPTY);
	return databyte;
}

void SlaveHello()
{
	byte b = 0;
	do {
		COREUART_ClearRxBuf();
		COREUART_SendChar(0x5A);
		b = ReadCoreUartByte();
	} while (b != 0x5A);
}

void SlaveToBootloader()
{
  COREUART_Init();
  /* PORTD_PCR6: ISF=0,PE=1,PS=0 */
  PORTD_PCR6 = (uint32_t)((PORTD_PCR6 & (uint32_t)~(uint32_t)(
                PORT_PCR_ISF_MASK |
                PORT_PCR_PS_MASK
               )) | (uint32_t)(
                PORT_PCR_PE_MASK
               ));
  /* PORTD_PCR7: ISF=0,PE=1,PS=0 */
  PORTD_PCR7 = (uint32_t)((PORTD_PCR7 & (uint32_t)~(uint32_t)(
                PORT_PCR_ISF_MASK |
                PORT_PCR_PS_MASK
               )) | (uint32_t)(
                PORT_PCR_PE_MASK
               ));
  PTD_Init();
  SlaveResetMode(0);
  SlaveReset_ClrVal(SlaveReset_DeviceData);
  WAIT1_Waitms(1);
  SlaveReset_SetVal(SlaveReset_DeviceData);
  slaveresetticks = 0;
  while (!IsSlaveReady()) {
	  slaveresetticks++;
  }

  COREUART_Enable();
  SlaveHello();
  LED1_On();
}

void SlaveToProgram()
{
  COREUART_Init();
  COREUART_Enable();

  SlaveReset_ClrVal(SlaveReset_DeviceData);
  WAIT1_Waitms(1);
  SlaveReset_SetVal(SlaveReset_DeviceData);

  slaveresetticks = 0;
  while (!IsSlaveReady()) {
	  slaveresetticks++;
  }
}

void SlaveRequest(void* req, size_t reqsize, void* res, size_t ressize)
{
	COREUART_ClearRxBuf();

	word xferred = 0;
	while (COREUART_SendBlock(req, reqsize, &xferred) == ERR_TXFULL) {
		req += xferred;
		reqsize -= xferred;
	}

	if (ressize > 0) {
		xferred = 0;
		while (COREUART_RecvBlock(res, ressize, &xferred) == ERR_RXEMPTY) {
			res += xferred;
			ressize -= xferred;
		}
	}
}

uint32_t GetSlaveBlockSize()
{
	uint32_t blocksize = 0;

	byte req[] = { 0x1, 0x1 };
	SlaveRequest(req, sizeof(req), &blocksize, sizeof(blocksize));

	return blocksize;
}

int WriteSlaveBlock(uint32_t address, void* data, int datasize)
{
	byte req[] = { 0x3, 0, 0, 0, 0, 0, 0, 0 };
	*(uint16_t*)&req[2] = datasize;
	*(uint32_t*)&req[4] = address;
	SlaveRequest(&req, sizeof(req), NULL, 0);

	SlaveRequest(data, datasize, NULL, 0);

	/* erase started, wait for completion */

	byte ack = ReadCoreUartByte();
	if (ack != 1) {
		// erase failed or parameter error
		return 1;
	}

	/* write started, wait for completion */

	ack = ReadCoreUartByte();
	if (ack != 1) {
		// write failed
		return 1;
	}
}

void flash_write_page(uint8_t* buffer, uint32_t address, uint32_t blocksize)
{
	WriteSlaveBlock(address, buffer, blocksize);
}

uint32_t flash_slave_blocksize()
{
	return GetSlaveBlockSize();
}

void flash_slave_reset_bootloader()
{
	SlaveToBootloader();
}

void flash_slave_reset_application()
{
	SlaveToProgram();
}

int slave_initialized = 0;
uint32_t slave_blocksize = 4096;
uint32_t curblock_base = 0xFFFFFFFF;
uint8_t blockbuf[4096];

void flash_finish_page()
{
	if (curblock_base != 0xFFFFFFFF) {
		flash_write_page(blockbuf, curblock_base, slave_blocksize);
		curblock_base = 0xFFFFFFFF;
	}
}

void flash_64_bytes(uint8_t* block, uint32_t address)
{
	if (!slave_initialized) {
		flash_slave_reset_bootloader();
		slave_initialized = 1;
		slave_blocksize = flash_slave_blocksize();
	}

	uint32_t slave_blockmask = ~(slave_blocksize-1);

	if ((address & slave_blockmask) != curblock_base) {
		flash_finish_page();

		curblock_base = address & slave_blockmask;
		memset(blockbuf, 0xFF, slave_blocksize);
	}

	memcpy(&blockbuf[address & (~slave_blockmask)], block, 64);
}


const static uint32_t syxblocksize = 64;
const static uint32_t blockidstart = 64;
const static uint32_t addressstart = 66;
const static uint32_t checksumstart = 68;
const static uint32_t fullblocksize = 70;

int flash_expect_blockid = 0;

int block_index = 0;
uint8_t block[70];

uint16_t flash_fletcher_checksum(uint8_t* block, int count)
{
	uint32_t checksum1 = 0;
	uint32_t checksum2 = 0;

	for (int i = 0; i < count; i++) {
		checksum1 += block[i];
		if (checksum1 >= 255) {
			checksum1 -= 255;
		}
		checksum2 += checksum1;
		if (checksum2 >= 255) {
			checksum2 -= 255;
		}
	}

	return ((uint8_t)checksum2 << 8) | (uint8_t)checksum1;
}

int flash_7_bytes(uint8_t* buffer)
{
	for (int i = 0; i < 7; i++) {
		block[block_index + i] = buffer[i];
	}

	block_index += 7;

	if (block_index == fullblocksize) {
		block_index = 0;

		// verify checksum
		uint16_t checksum = flash_fletcher_checksum(block, fullblocksize);
		if (checksum != 0) {
			return 1;
		}

		// retrieve block id
		uint32_t blockid = (block[blockidstart] + (block[blockidstart + 1] << 8));
		if (blockid != flash_expect_blockid) {
			// didn't expect this block, fail
			return 1;
		}

		// retrieve write address
		uint32_t address = (block[addressstart] + (block[addressstart + 1] << 8)) << 7;

		// ready to flash
		flash_64_bytes(block, address);

		flash_expect_blockid++;
	}

	return 0;
}

int buf_7bit_index = 0;
uint8_t buf_7bit[7];

int flash_data_7bit(uint8_t b)
{
	if (buf_7bit_index < 7) {
		buf_7bit[buf_7bit_index] = b;
		buf_7bit_index++;
		return 0;
	}

	if (buf_7bit_index == 7) {
		for (int i = 0; i < 7; i++) {
			buf_7bit[i] |= ((b >> i) & 0x01) << 7;
		}
		buf_7bit_index = 0;
		return flash_7_bytes(buf_7bit);
	}

	// reached here -> fail
	return 1;
}

void flash_reset()
{
	if (slave_initialized) {
		flash_finish_page();
		flash_slave_reset_application();
		slave_initialized = 0;
		flash_expect_blockid = 0;
	}

	buf_7bit_index = 0;
	block_index = 0;
}

int sysex_ignore = 0;
int sysex_started = 0;
int sysex_flashing = 0;
int sysex_queue_used = 0;
uint8_t sysex_queue[4];

void sysex_stop()
{
	sysex_started = 0;
	sysex_queue_used = 0;
	sysex_flashing = 0;
	sysex_ignore = 0;
}

void sysex_start()
{
	if (sysex_started) {
		sysex_stop();
	}

	sysex_started = 1;
	sysex_queue_used = 0;
	sysex_flashing = 0;
	sysex_ignore = 0;
}

void sysex_start_request(uint8_t* requestheader)
{
	if (requestheader[0] == 0xF0
		&& requestheader[1] == 0x6F
		&& requestheader[2] == 0x6B
		&& requestheader[3] == 0x5F)
	{
		sysex_flashing = 1;
		return;
	}

	if (requestheader[0] == 0xF0
		&& requestheader[1] == 0x6F
		&& requestheader[2] == 0x6B
		&& requestheader[3] == 0x21)
	{
		flash_reset();
		KIN1_SoftwareReset();
		return;
	}

	sysex_ignore = 1;
}

void sysex_queue_push(uint8_t b)
{
	if (!sysex_started || sysex_ignore) {
		return;
	}

	if (sysex_flashing) {
		if (b == 0xF7) {
			// end
			sysex_stop();
			return;
		}

		if (b < 0x80) {
			if (flash_data_7bit(b)) {
				// fail!
				flash_reset();
				sysex_stop();
			}
		}
		return;
	}

	if (sysex_queue_used < 4) {
		sysex_queue[sysex_queue_used++] = b;

		if (sysex_queue_used == 4) {
			sysex_start_request(sysex_queue);
		}
	}
}
#endif
int sysex_process(const midicmd_t cmd)
{
	switch (cmd.header & 0x0F)
	{
	case 0x5:
		sysex_queue_push(cmd.b1);
		sysex_stop();
		break;
	case 0xF:
		if (cmd.b1 & 0x80) {
			switch (cmd.b1) {
			case 0xF0:
				sysex_start();
				sysex_queue_push(cmd.b1);
				break;
			case 0xF7:
				sysex_queue_push(cmd.b1);
				sysex_stop();
				break;
			default:
				sysex_stop();
			}
		}
		else if (sysex_started) {
			sysex_queue_push(cmd.b1);
		}
		break;
	case 0x6:
		sysex_queue_push(cmd.b1);
		sysex_queue_push(cmd.b2);
		sysex_stop();
		break;
	case 0x4:
		if (cmd.b1 == 0xF0) {
			sysex_start();
		}
		sysex_queue_push(cmd.b1);
		sysex_queue_push(cmd.b2);
		sysex_queue_push(cmd.b3);
		break;
	case 0x7:
		sysex_queue_push(cmd.b1);
		sysex_queue_push(cmd.b2);
		sysex_queue_push(cmd.b3);
		sysex_stop();
		break;
	}

	return sysex_flashing || slave_initialized;
}

__attribute__((__aligned__(512))) static uint8_t midi_buffer[USB1_DATA_BUFF_SIZE];
static uint8_t in_buffer[USB1_DATA_BUFF_SIZE];

int usbmidi_msglen(const midicmd_t cmd)
{
	switch (cmd.header & 0x0F) {
	case 0x5: // 1-byte sysex
	case 0xF: // single byte
		return 1; // 1 byte
	case 0x2: // 2-byte system common
	case 0x6: // 2-byte sysex end
	case 0xC: // program change
	case 0xD: // channel pressure
		return 2; // 2 bytes
	case 0x3: // 3-byte system common
	case 0x4: // 3-byte sysex start/continue
	case 0x7: // 3-byte sysex end
	case 0x8: // note off
	case 0x9: // note on
	case 0xA: // poly-keypress
	case 0xB: // control change
	case 0xE: // pitchbend
		return 3; // 3 bytes
		break;
	default:
		// unimplemented
		break;
	}

	return 0;
}

int intercore_receive(midicmd_t* cmd)
{
	while (COREUART_GetCharsInRxBuf() >= 4)
	{
		if (COREUART_RecvChar(&cmd->header) != ERR_OK) {
			return 0;
		}
		if (COREUART_RecvChar(&cmd->b1) != ERR_OK) {
			return 0;
		}
		if (COREUART_RecvChar(&cmd->b2) != ERR_OK) {
			return 0;
		}
		if (COREUART_RecvChar(&cmd->b3) != ERR_OK) {
			return 0;
		}
		return 1;
	}

	return 0;
}

int intercore_can_transmit(int numbytes)
{
	return (COREUART_OUT_BUF_SIZE - COREUART_GetCharsInTxBuf()) >= numbytes;
}

void intercore_transmit(const midicmd_t cmd)
{
	COREUART_SendChar(cmd.header);
	COREUART_SendChar(cmd.b1);
	COREUART_SendChar(cmd.b2);
	COREUART_SendChar(cmd.b3);
#if 0
	switch (usbmidi_msglen(cmd)) {
	case 1:
		// 1 byte
		COREUART_SendChar(cmd.b1);
		break;
	case 2:
		// 2 bytes
		COREUART_SendChar(cmd.b1);
		COREUART_SendChar(cmd.b2);
		break;
	case 3:
		// 3 bytes
		COREUART_SendChar(cmd.b1);
		COREUART_SendChar(cmd.b2);
		COREUART_SendChar(cmd.b3);
		break;
	default:
		// unimplemented
		break;
	}
#endif
}

enum {
	NoteOff,
	NoteOn,
	Aftertouch,
	ControlChange,
	PatchChange,
	ChannelPressure,
	PitchWheel,
	SysexBegin,
	MTCQuarterFrame,
	SongPosition,
	SongSelect,
	TuneRequest,
	SysexEnd
} modes;

byte midiin_status;
byte midiin_header;
byte midiin_bytes[3];
int midiin_count = 0;
int midiin_length = 0;

void midiin_sysex(midicmd_t* cmd)
{
	cmd->header = 0x4;
	cmd->b1 = midiin_bytes[0];
	cmd->b2 = midiin_bytes[1];
	cmd->b3 = midiin_bytes[2];
}

void midiin_sysex_end(midicmd_t* cmd)
{
	switch (midiin_count) {
	case 1:
		cmd->header = 0x5;
		break;
	case 2:
		cmd->header = 0x6;
		break;
	case 3:
		cmd->header = 0x7;
		break;
	default:
		return;
	}

	cmd->b1 = midiin_bytes[0];
	cmd->b2 = midiin_bytes[1];
	cmd->b3 = midiin_bytes[2];
}

void midiin_pack_cmd(midicmd_t* cmd)
{
	if (midiin_status < 0xF0) {
		cmd->header = midiin_status >> 4;
	}
	else if (midiin_status == 0xF0) {
		if (midiin_bytes[0] == 0xF0) {
			cmd->header = 0x4;
		}
		else {
			switch (midiin_count) {
			case 1:
				cmd->header = 0x5;
				break;
			case 2:
				cmd->header = 0x6;
				break;
			case 3:
				cmd->header = 0x7;
				break;
			default:
				return;
			}
		}
	}
	else {
		if (midiin_length == 1) {
			cmd->header = 0x2;
		}
		else if (midiin_length == 2) {
			cmd->header = 0x3;
		}
		else {
			// uh-oh
			cmd->header = 0xF;
		}
	}
	cmd->b1 = midiin_status;
	cmd->b2 = midiin_bytes[0];
	cmd->b3 = midiin_bytes[1];
}

int midiin_process(midicmd_t* cmd, byte b)
{
	if (b >= 0xF8) {
		// single byte system realtime messages
		cmd->header = 0xF;
		cmd->b1 = b;
		cmd->b2 = 0;
		cmd->b3 = 0;
		return 1;
	}

	if (b == 0xF7) {
		// SysEx End
		midiin_bytes[midiin_count] = b;
		midiin_count++;
		midiin_sysex_end(cmd);
		midiin_status = 0;
		midiin_count = 0;
		midiin_length = 0;
		return 1;
	}

	if ((b & 0x80) == 0) {
		if (midiin_status != 0) {
			midiin_bytes[midiin_count] = b;
			midiin_count++;

			if (midiin_count == midiin_length) {
				if (midiin_status == 0xF0) {
					midiin_sysex(cmd);
				}
				else {
					midiin_pack_cmd(cmd);
				}
				midiin_count = 0;

				return 1;
			}
		}

		return 0;
	}
	else {
		// (b & 0x80)

		switch (b & 0xF0) {
		case 0x80: // note off
		case 0x90: // note on
		case 0xA0: // aftertouch
		case 0xB0: // control change
		case 0xE0: // pitch bend
			midiin_status = b;
			midiin_count = 0;
			midiin_length = 2;
			return 0;
		case 0xC0: // program change
		case 0xD0: // channel pressure
			midiin_status = b;
			midiin_count = 0;
			midiin_length = 1;
			return 0;
		default:
			switch (b) {
			case 0xF0: // SysEx begin
				midiin_status = b;
				midiin_length = 3;
				midiin_bytes[0] = b;
				midiin_count = 1;
				return 0;
			case 0xF1: // MTC quarter frame
				midiin_status = b;
				midiin_count = 0;
				midiin_length = 1;
				return 0;
			case 0xF2: // Song position
				midiin_status = b;
				midiin_count = 0;
				midiin_length = 2;
				return 0;
			case 0xF3: // Song select
				midiin_status = b;
				midiin_count = 0;
				midiin_length = 1;
				return 0;
			case 0xF6: // Tune request
				midiin_status = 0;

				cmd->header = 0x5;
				cmd->b1 = b;
				cmd->b2 = 0;
				cmd->b3 = 0;
				return 1;
			}
		}
	}

	// ignored
	midiin_status = 0;
	return 0;
}

int midiin_receive(midicmd_t* cmd)
{
	while (MIDIUART1_GetCharsInRxBuf() > 0)
	{
		byte b;
		if (MIDIUART1_RecvChar(&b) == ERR_OK) {
			if (midiin_process(cmd, b)) {
				return 1;
			}
		}
	}

	return 0;
}

int midiout_can_transmit(int numbytes)
{
	return (MIDIUART1_OUT_BUF_SIZE - MIDIUART1_GetCharsInTxBuf()) >= numbytes;
}

void midiout_transmit(const midicmd_t cmd)
{
	switch (usbmidi_msglen(cmd)) {
	case 1:
		// 1 byte
		MIDIUART1_SendChar(cmd.b1);
		break;
	case 2:
		// 2 bytes
		MIDIUART1_SendChar(cmd.b1);
		MIDIUART1_SendChar(cmd.b2);
		break;
	case 3:
		// 3 bytes
		MIDIUART1_SendChar(cmd.b1);
		MIDIUART1_SendChar(cmd.b2);
		MIDIUART1_SendChar(cmd.b3);
		break;
	default:
		// unimplemented
		break;
	}
}

void usbmidi_transmit(const midicmd_t cmd)
{
	if (UsbMidiTx_NofFreeElements() >= 4) {
		UsbMidiTx_Put(cmd.header);
		UsbMidiTx_Put(cmd.b1);
		UsbMidiTx_Put(cmd.b2);
		UsbMidiTx_Put(cmd.b3);
	}
}

void usb_run(void)
{
    int i;
	midicmd_t cmd;
	int have_peek_byte = 0;
	byte peek_byte = 0;

	COREUART_Init();
	COREUART_Enable();

    for(;;) {
    	if (USBMIDI1_App_Task(midi_buffer, sizeof(midi_buffer)) == ERR_BUSOFF) {
    		LED1_Off();
    		//WAIT1_Waitms(100);
    	}
    	else {
    		LED1_On();

    		int cannot_transmit = 0;
    		if (have_peek_byte) {
				int cableid = (cmd.header >> 4);
				if (cableid == 0) {
					cannot_transmit = !intercore_can_transmit(4);
				}
				if (cableid == 1) {
					cannot_transmit = !midiout_can_transmit(4);
				}
    		}

			while (!cannot_transmit && UsbMidiRx_NofElements()+have_peek_byte >= 4) {
				if (!have_peek_byte) {
					UsbMidiRx_Get(&peek_byte);
					have_peek_byte = 1;

					int cableid = (cmd.header >> 4);
					if (cableid == 0) {
						cannot_transmit = !intercore_can_transmit(4);
					}
					if (cableid == 1) {
						cannot_transmit = !midiout_can_transmit(4);
					}
				}

				if (!cannot_transmit) {
					if (!have_peek_byte) {
						UsbMidiRx_Get(&cmd.header);
					}
					else {
						cmd.header = peek_byte;
						have_peek_byte = 0;
					}
					UsbMidiRx_Get(&cmd.b1);
					UsbMidiRx_Get(&cmd.b2);
					UsbMidiRx_Get(&cmd.b3);

					int cableid = (cmd.header >> 4);
					if (cableid == 0) {
						if (!sysex_process(cmd)) {
							intercore_transmit(cmd);
						}
					}
					if (cableid == 1) {
						midiout_transmit(cmd);
					}
				}
			}
    	}

    	if (!(sysex_flashing || slave_initialized)) {
			int ready = intercore_receive(&cmd);
			if (ready) {
				switch (cmd.header & 0xf0) {
				case 0x00:
					usbmidi_transmit(cmd);
					break;
				case 0x10:
					midiout_transmit(cmd);
					break;
				}
			}
    	}

		int ready = midiin_receive(&cmd);
		if (ready) {
			cmd.header &= 0x0F;
			cmd.header |= (0x1 << 4);
			usbmidi_transmit(cmd);
			intercore_transmit(cmd);
		}
    }
}
