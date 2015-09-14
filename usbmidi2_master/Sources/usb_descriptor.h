/******************************************************************************
 *
 * Freescale Semiconductor Inc.
 * (c) Copyright 2004-2009 Freescale Semiconductor, Inc.
 * ALL RIGHTS RESERVED.
 *
 **************************************************************************//*!
 *
 * @file usb_descriptor.h
 *
 * @author
 *
 * @version
 *
 * @date May-28-2009
 *
 * @brief The file is a header file for USB Descriptors required for Virtual
 *        COM Loopback Application
 *****************************************************************************/

#ifndef _USB_DESCRIPTOR_H
#define _USB_DESCRIPTOR_H

/******************************************************************************
 * Includes
 *****************************************************************************/
#include "usb/types.h"
#include "usb/usb_class.h"
#include "usb/USB_Config.h"

/******************************************************************************
 * Constants - None
 *****************************************************************************/

/******************************************************************************
 * Macro's
 *****************************************************************************/

/* free VID/PID: https://github.com/obdev/v-usb/blob/master/usbdrv/USB-IDs-for-free.txt */
/*
 * Windows caches device data per VID/PID pair: cannot use the same PID for
 * both master and slave interfaces because they have different port names.
 */
#define VID 0x16c0
#define PID 0x05e4

#define REMOTE_WAKEUP_SHIFT              (5)

/* Various descriptor sizes */
#define DEVICE_DESCRIPTOR_SIZE            (18)
#define CONFIG_ONLY_DESC_SIZE             (9)
#define CONFIG_DESC_SIZE                  (CONFIG_ONLY_DESC_SIZE + \
										   IFACE_ONLY_DESC_SIZE + 9 + \
										   IFACE_ONLY_DESC_SIZE + \
										   7+2*(6+6+9+9) + \
										   9+6+9+6)
										   
#define DEVICE_QUALIFIER_DESCRIPTOR_SIZE  (10)
#define IFACE_ONLY_DESC_SIZE              (9)
#define ENDP_ONLY_DESC_SIZE               (7)

/* Max descriptors provided by the Application */
#define USB_MAX_STD_DESCRIPTORS           (7)

/* Max configuration supported by the Application */
#define USB_MAX_CONFIG_SUPPORTED          (1)

/* Max string descriptors supported by the Application */
#define USB_MAX_STRING_DESCRIPTORS        (8)

/* Max language codes supported by the USB */
#define USB_MAX_LANGUAGES_SUPPORTED       (1)

/* string descriptors sizes */
#define USB_STR_DESC_SIZE (2)
#define USB_STR_0_SIZE  (2)
#define USB_STR_1_SIZE  (22)
//#define USB_STR_2_SIZE  (30)
#define USB_STR_2_SIZE  (38)
#define USB_STR_3_SIZE  (2)
#define USB_STR_4_SIZE  (14)
#define USB_STR_5_SIZE  (24)
#define USB_STR_6_SIZE  (12)
#define USB_STR_7_SIZE  (22)
#define USB_STR_n_SIZE  (32)

/* descriptors codes */
#define USB_DEVICE_DESCRIPTOR     (1)
#define USB_CONFIG_DESCRIPTOR     (2)
#define USB_STRING_DESCRIPTOR     (3)
#define USB_IFACE_DESCRIPTOR      (4)
#define USB_ENDPOINT_DESCRIPTOR   (5)
#define USB_DEVQUAL_DESCRIPTOR    (6)
#define USB_CS_INTERFACE          (0x24)
#define USB_CS_ENDPOINT           (0x25)

#define USB_MAX_SUPPORTED_INTERFACES     (2)

#define USB_ENDPOINT_COUNT               (2)

/******************************************************************************
 * Types
 *****************************************************************************/
typedef const struct _USB_LANGUAGE
{
    uint_16 const language_id;      /* Language ID */
    uint_8 const ** lang_desc;      /* Language Descriptor String */
    uint_8 const * lang_desc_size;  /* Language Descriptor Size */
} USB_LANGUAGE;

typedef const struct _USB_ALL_LANGUAGES
{
    /* Pointer to Supported Language String */
    uint_8 const *languages_supported_string;
    /* Size of Supported Language String */
    uint_8 const languages_supported_size;
    /* Array of Supported Languages */
    USB_LANGUAGE usb_language[USB_MAX_SUPPORTED_INTERFACES];
}USB_ALL_LANGUAGES;

typedef const struct _USB_ENDPOINTS
{
    /* Number of non control Endpoints */
    uint_8 count;
    /* Array of Endpoints Structures */
    USB_EP_STRUCT ep[USB_ENDPOINT_COUNT];
}USB_ENDPOINTS;

/******************************************************************************
 * Global Functions
 *****************************************************************************/
extern uint_8 USB_Desc_Get_Descriptor(
    uint_8 controller_ID,
    uint_8 type,
    uint_8 str_num,
    uint_16 index,
    uint_8_ptr *descriptor,
    USB_PACKET_SIZE *size);

extern uint_8 USB_Desc_Get_Interface(
    uint_8 controller_ID,
    uint_8 interface,
    uint_8_ptr alt_interface);


extern uint_8 USB_Desc_Set_Interface(
    uint_8 controller_ID,
    uint_8 interface,
    uint_8 alt_interface);

extern boolean USB_Desc_Valid_Configation(
    uint_8 controller_ID,
    uint_16 config_val);

extern boolean USB_Desc_Valid_Interface(
    uint_8 controller_ID,
    uint_8 interface);

extern boolean USB_Desc_Remote_Wakeup(uint_8 controller_ID);

extern void* USB_Desc_Get_Endpoints(uint_8 controller_ID);

#endif


