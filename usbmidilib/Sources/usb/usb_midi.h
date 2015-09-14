/* MIDI Streaming interface definitions */

#ifndef _USB_MIDI_H
#define _USB_MIDI_H

/******************************************************************************
 * Includes
 *****************************************************************************/
#include "types.h"
#include "usb_descriptor_common.h"
#include "usb_class.h"
#include "usb_devapi.h"
#ifdef COMPOSITE_DEV
#include "usb_composite.h"
#endif

/******************************************************************************
 * Constants - None
 *****************************************************************************/

/******************************************************************************
 * Macro's
 *****************************************************************************/
/* Class specific request Codes */
#define REQUEST_CODE_UNDEFINED			(0x00)
#define SET_CUR							(0x01)
#define GET_CUR							(0x81)
#define SET_MIN							(0x02)
#define GET_MIN							(0x82)
#define SET_MAX							(0x03)
#define GET_MAX							(0x83)
#define SET_RES							(0x04)
#define GET_RES							(0x84)
#define SET_MEM							(0x05)
#define GET_MEM							(0x85)
#define GET_STAT						(0xFF)

/* Endpoint Control Selectors */
#define EP_CONTROL_UNDEFINED			(0x00)
#define ASSOCIATION_CONTROL				(0x01)

/* Element descriptor capability fields */
#define CAPS_CUSTOM_UNDEFINED			(1 << 0)
#define CAPS_MIDI_CLOCK					(1 << 1)
#define CAPS_MIDI_TIME_CODE				(1 << 2)
#define CAPS_MIDI_MACHINE_CONTROL		(1 << 3)
#define CAPS_GM1						(1 << 4)
#define CAPS_GM2						(1 << 5)
#define CAPS_GS							(1 << 6)
#define CAPS_XG							(1 << 7)
#define CAPS_EFX						(1 << 8)
#define CAPS_MIDI_PATCH_BAY				(1 << 9)
#define CAPS_DLS1						(1 << 10)
#define CAPS_DLS2						(1 << 11)

/* MS Class-Specfic Interface Descriptor Subtypes */
#define MS_DESCRIPTOR_UNDEFINED			(0x00)
#define MS_HEADER						(0x01)
#define MIDI_IN_JACK					(0x02)
#define MIDI_OUT_JACK					(0x03)
#define ELEMENT							(0x04)

/* MS Class-Specific Endpoint Descriptor Subtypes */
#define DESCRIPTOR_UNDEFINED			(0x00)
#define MS_GENERAL						(0x01)

/* MS MIDI IN and OUT Jack types */
#define	JACK_TYPE_UNDEFINED				(0x00)
#define	EMBEDDED						(0x01)
#define	EXTERNAL						(0x02)

/* other macros */
//#define NOTIF_PACKET_SIZE             (0x08)
//#define NOTIF_REQUEST_TYPE            (0xA1)

#define AUDIO_CLASS						(0x01)
#define AUDIO_CONTROL_SUBCLASS			(0x01)
#define MIDISTREAMING_SUBCLASS			(0x03)
#define AUDIO_CLASS_REVISION			(0x0100)


typedef struct _app_data_struct
{
    uint_8_ptr      data_ptr;       /* pointer to buffer */
    USB_PACKET_SIZE data_size;      /* buffer size of endpoint */
} APP_DATA_STRUCT;

uint_8 USB_Class_MIDI_Init (
		uint_8    		   		controller_ID,			/* [IN] Controller ID */
	USB_CLASS_CALLBACK 		midi_class_callback,	/* [IN] CDC Class Callback */
	USB_REQ_FUNC       		vendor_req_callback,	/* [IN] Vendor Request Callback */
	uint_8            		bVregEn					/* Enables or disables internal regulator */
);

uint_8 USB_Class_MIDI_Send_Data (
	uint_8 controller_ID,   /* [IN] Controller ID */
	uint_8 ep_num,          /* [IN] Endpoint Number */
	uint_8_ptr app_buff,    /* Pointer to Application Buffer */
	USB_PACKET_SIZE size    /* Size of Application Buffer */
);

#if 0
/******************************************************************************
 * Types
 *****************************************************************************/
#ifndef COMPOSITE_DEV
typedef struct _app_data_struct
{
    uint_8_ptr      data_ptr;       /* pointer to buffer */
    USB_PACKET_SIZE data_size;      /* buffer size of endpoint */
}APP_DATA_STRUCT;
#endif

/* structure to hold a request in the endpoint queue */
typedef struct _usb_class_cdc_queue
{
    uint_8 controller_ID;       /* Controller ID */
    uint_8 channel;             /* Endpoint Number */
    APP_DATA_STRUCT app_data;   /* Application Data Structure */
}USB_CLASS_CDC_QUEUE, *PTR_USB_CLASS_CDC_QUEUE;

/* USB class cdc endpoint data */
typedef struct _usb_class_cdc_endpoint
{
    uint_8 endpoint; /* endpoint num */
    uint_8 type;     /* type of endpoint (interrupt, bulk or isochronous) */
    uint_8 bin_consumer;/* the num of queued elements */
    uint_8 bin_producer;/* the num of de-queued elements */
    USB_CLASS_CDC_QUEUE queue[MAX_QUEUE_ELEMS]; /* queue data */
}USB_CLASS_CDC_ENDPOINT;

/******************************************************************************
 * Global Functions
 *****************************************************************************/
extern uint_8 USB_Class_CDC_Init (
	uint_8    		   		controller_ID,       /* [IN] Controller ID */
	USB_CLASS_CALLBACK 		cdc_class_callback,  /* [IN] CDC Class Callback */
	USB_REQ_FUNC       		vendor_req_callback, /* [IN] Vendor Request Callback */
	USB_CLASS_CALLBACK 		pstn_callback,       /* [IN] PSTN Callback */
	uint_8            		bVregEn              /* Enables or disables internal regulator */
);

#ifdef COMPOSITE_DEV
extern uint_8 USB_CDC_Other_Requests(uint_8 controller_ID,
                          USB_SETUP_STRUCT * setup_packet,
                          uint_8_ptr *data,
                          USB_PACKET_SIZE *size);
#endif

extern uint_8 USB_Class_CDC_DeInit 
(
    uint_8 controller_ID              
); 

extern uint_8 USB_Class_CDC_Send_Data (
    uint_8           controller_ID,
    uint_8           ep_num,
    uint_8_ptr       buff_ptr,
    USB_PACKET_SIZE  size
);

#if CIC_NOTIF_ELEM_SUPPORT
#define USB_Class_CDC_Interface_CIC_Send_Data(a,b,c)  \
    USB_Class_CDC_Send_Data(a,CIC_SEND_ENDPOINT,b,c)
#endif

#define USB_Class_CDC_Interface_DIC_Send_Data(a,b,c)  \
    USB_Class_CDC_Send_Data(a,DIC_SEND_ENDPOINT,b,c)
#define USB_Class_CDC_Interface_DIC_Recv_Data(a,b,c)  \
	_usb_device_recv_data(a,DIC_RECV_ENDPOINT,b,c)
#define USB_Class_CDC_Interface_DIC_Get_Send_Buffer(a,b,c)  \
	_usb_device_get_send_buffer(a,DIC_SEND_ENDPOINT,b,c)

#define USB_Class_CDC_Periodic_Task     USB_Class_Periodic_Task

#endif

#endif
