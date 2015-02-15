#include "USB1.h"
#include "WAIT1.h"
#include "USBMIDI1.h"

__attribute__((__aligned__(512))) static uint8_t midi_buffer[USB1_DATA_BUFF_SIZE];
//static uint8_t cdc_buffer[USB1_DATA_BUFF_SIZE];
static uint8_t in_buffer[USB1_DATA_BUFF_SIZE];

void usb_run(void) {
    int i;

    for(;;) {
    	while (USBMIDI1_App_Task(midi_buffer, sizeof(midi_buffer)) == ERR_BUSOFF) {
    		WAIT1_Waitms(100);
    	}

    	if (UsbMidiRx_NofElements() >= 4) {
    		midicmd_t cmd;
    		UsbMidiRx_Get(&cmd.header);
    		UsbMidiRx_Get(&cmd.b1);
    		UsbMidiRx_Get(&cmd.b2);
    		UsbMidiRx_Get(&cmd.b3);
    		UsbMidiTx_Put(cmd.header);
    		UsbMidiTx_Put(cmd.b1);
    		UsbMidiTx_Put(cmd.b2);
    		UsbMidiTx_Put(cmd.b3);
    	}

#if 0
        while(CDC1_App_Task(cdc_buffer, sizeof(cdc_buffer))==ERR_BUSOFF) {
        /* device not enumerated */
            //LED1_Neg(); LED2_Off();
            WAIT1_Waitms(200);
        }
        while(USBMIDI1_App_Task(midi_buffer, sizeof(midi_buffer))==ERR_BUSOFF) {
        /* device not enumerated */
            //LED1_Neg(); LED2_Off();
            WAIT1_Waitms(200);
        }
        //LED1_Off(); LED2_Neg();
        if (USB1_GetCharsInRxBuf()!=0) {
            i = 0;
            while(   i<sizeof(in_buffer)-1
                )
            {
            i++;
            }
            in_buffer[i] = '\0';
            (void)USB1_SendString((unsigned char*)"echo: ");
            (void)USB1_SendString(in_buffer);
            (void)USB1_SendString((unsigned char*)"\r\n");
        } else {
            WAIT1_Waitms(50);
        }
#endif
    }
}
