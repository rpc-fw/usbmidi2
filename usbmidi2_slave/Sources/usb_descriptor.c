/* USB descriptors for MIDIStreaming interfaces */

/******************************************************************************
 * Includes
 *****************************************************************************/
#include "derivative.h"
#include "types.h"
#include "usb_class.h"
#include "usb_descriptor.h"
#include "usb_midi.h"

#if (defined __MCF52xxx_H__)||(defined __MK_xxx_H__)
/* Put CFV2 descriptors in RAM */
#define USB_DESC_CONST
#else
#define USB_DESC_CONST	const
#endif

/*****************************************************************************
 * Constant and Macro's
 *****************************************************************************/
/* structure containing details of all the endpoints used by this device */
USB_DESC_CONST USB_ENDPOINTS usb_desc_ep =
{
		2, /* endpoint count */
		{
				{
						1,				/* endpoint number */
						USB_BULK_PIPE,	/* type of endpoint */
						USB_RECV,		/* direction */
						64				/* buffer size */
				},
				{
						2,				/* endpoint number */
						USB_BULK_PIPE,	/* type of endpoint */
						USB_SEND,		/* direction */
						64				/* buffer size */
				}
		}
};

uint_8 USB_DESC_CONST g_device_descriptor[DEVICE_DESCRIPTOR_SIZE] =
{
   DEVICE_DESCRIPTOR_SIZE,               /*  Device Descriptor Size         */
   USB_DEVICE_DESCRIPTOR,                /*  Device Type of descriptor      */
   0x00, 0x02,                           /*  BCD USB version                */
   0x00,                                 /*  Device class            		*/
   0x00,                                 /*  Device							*/
   0x00,                                 /*  Device Protocol                */
   CONTROL_MAX_PACKET_SIZE,              /*  Max Packet size                */
#if 0 /* << EST */
   0xA2,0x15,                            /*  Vendor ID                      */
   0x00,0x03,                            /*  0300 is our Product ID for CDC */
#else
#undef PID
#define PID 0x27de
   (VID&0xFF),((VID>>8)&0xFF),     /*  Vendor ID from properties      */
   (PID&0xFF),((PID>>8)&0xFF),     /*  Product ID from properties     */
#endif
   0x00,0x0a,                            /*  BCD Device version             */
   0x01,                                 /*  Manufacturer string index      */
   0x02,                                 /*  Product string index           */
   0x0,                                  /*  Serial number string index     */
   0x01                                  /*  Number of configurations       */
};

uint_8 USB_DESC_CONST g_config_descriptor[CONFIG_DESC_SIZE] =
{
    CONFIG_ONLY_DESC_SIZE,  /*  Configuration Descriptor Size */
    USB_CONFIG_DESCRIPTOR,  /* "Configuration" type of descriptor */
    CONFIG_DESC_SIZE, 0x00, /*  Total length of the Configuration descriptor */
    (uint_8)2,              /*NumInterfaces*/
    0x01,                      /*  Configuration Value */
    0x00,                      /*  Configuration Description String Index*/
    BUS_POWERED|SELF_POWERED,			   /*  Bus powered */
    0,                    		/*  Current draw from bus -- 0 mA*/

#if 1
        /* AUDIOCONTROL INTERFACE DESCRIPTOR */
        IFACE_ONLY_DESC_SIZE,
        USB_IFACE_DESCRIPTOR,
        (uint_8)(0), /* bInterfaceNumber */
        0x00, /* bAlternateSetting */
        0, /* no endpoints */
        AUDIO_CLASS,
        AUDIO_CONTROL_SUBCLASS,
        0x00, /* Interface Protocol, unused */
        0x00, /* Interface, unused */

        /* AudioControl Class-Specific descriptor */
        0x09,             /* size of Functional Desc in bytes */
        USB_CS_INTERFACE,  /* descriptor type*/
        0x01, /* HEADER subtype */
        0x00, 0x01,  /* USB Class Definitions for AC spec release number in BCD */
        0x09, 0x00, /* Total size of class-specific descriptors */
        0x01, /* Number of streaming interfaces */
        0x01, /* MIDIStreaming interface 1 belongs to this AC interface */
#endif

        /* MIDISTREAMING INTERFACE DESCRIPTOR */
        IFACE_ONLY_DESC_SIZE,
        USB_IFACE_DESCRIPTOR,
        (uint_8)(1), /* bInterfaceNumber */
        0x00, /* bAlternateSetting */
        0x02, /* 2 endpoints */
        AUDIO_CLASS,
        MIDISTREAMING_SUBCLASS,
        0x00, /* Interface Protocol, unused */
        0x00, /* Interface, unused */

        /* MIDIStreaming Class-Specific descriptor */
        0x07,             /* size of Functional Desc in bytes */
        USB_CS_INTERFACE,  /* descriptor type*/
        0x01, /* HEADER subtype */
        0x00, 0x01,  /* USB Class Definitions for AC spec release number in BCD */
        7+6+6+6+6+9+9+9+9, 0x00, /* Total size of class-specific descriptors */

        /* MIDI adapter MIDI IN Jack Descriptor (Embedded) */
        0x06,             /* size of Functional Desc in bytes */
        USB_CS_INTERFACE,  /* descriptor type*/
        MIDI_IN_JACK,  /* descriptor type*/
        EMBEDDED,	/* jack type */
        0x01, /* jack ID */
        0x06, /* unused */

        /* MIDI adapter MIDI IN Jack Descriptor (External) */
        0x06,             /* size of Functional Desc in bytes */
        USB_CS_INTERFACE,  /* descriptor type*/
        MIDI_IN_JACK,  /* descriptor type*/
        EXTERNAL,	/* jack type */
        0x02, /* jack ID */
        0x06, /* unused */

        /* MIDI adapter MIDI IN Jack Descriptor (Embedded) */
        0x06,             /* size of Functional Desc in bytes */
        USB_CS_INTERFACE,  /* descriptor type*/
        MIDI_IN_JACK,  /* descriptor type*/
        EMBEDDED,	/* jack type */
        0x03, /* jack ID */
        0x07, /* unused */

        /* MIDI adapter MIDI IN Jack Descriptor (External) */
        0x06,             /* size of Functional Desc in bytes */
        USB_CS_INTERFACE,  /* descriptor type*/
        MIDI_IN_JACK,  /* descriptor type*/
        EXTERNAL,	/* jack type */
        0x04, /* jack ID */
        0x07, /* unused */

        /* MIDI adapter MIDI OUT Jack Descriptor (Embedded) */
        0x09,             /* size of Functional Desc in bytes */
        USB_CS_INTERFACE,  /* descriptor type*/
        MIDI_OUT_JACK,  /* descriptor type*/
        EMBEDDED,	/* jack type */
        0x05, /* jack ID */
        0x01, /* number of input pins */
        0x01, /* BaSourceID(1) */
        0x01, /* BaSourcePin(1) */
        0x04, /* unused */

        /* MIDI adapter MIDI OUT Jack Descriptor (External) */
        0x09,             /* size of Functional Desc in bytes */
        USB_CS_INTERFACE,  /* descriptor type*/
        MIDI_OUT_JACK,  /* descriptor type*/
        EXTERNAL,	/* jack type */
        0x06, /* jack ID */
        0x01, /* number of input pins */
        0x01, /* BaSourceID(1) */
        0x01, /* BaSourcePin(1) */
        0x04, /* unused */

        /* MIDI adapter MIDI OUT Jack Descriptor (Embedded) */
        0x09,             /* size of Functional Desc in bytes */
        USB_CS_INTERFACE,  /* descriptor type*/
        MIDI_OUT_JACK,  /* descriptor type*/
        EMBEDDED,	/* jack type */
        0x07, /* jack ID */
        0x01, /* number of input pins */
        0x03, /* BaSourceID(1) */
        0x01, /* BaSourcePin(1) */
        0x05, /* unused */

        /* MIDI adapter MIDI OUT Jack Descriptor (External) */
        0x09,             /* size of Functional Desc in bytes */
        USB_CS_INTERFACE,  /* descriptor type*/
        MIDI_OUT_JACK,  /* descriptor type*/
        EXTERNAL,	/* jack type */
        0x08, /* jack ID */
        0x01, /* number of input pins */
        0x03, /* BaSourceID(1) */
        0x01, /* BaSourcePin(1) */
        0x05, /* unused */

        /* MIDI Adapter Standard Bulk OUT Endpoint Descriptor */
        0x09,
        USB_ENDPOINT_DESCRIPTOR,
        0x01, /* OUT endpoint 1 */
        USB_BULK_PIPE,
        64, 0x00, /* max packet size */
        0x00, /* interval */
        0x00, /* refresh */
        0x00, /* synch address */

        /* Class-specific MS Bulk OUT Endpoint Descriptor */
        0x06,
        USB_CS_ENDPOINT,
        MS_GENERAL,
        0x02, /* Number of embedded MIDI IN jacks */
        0x01, /* ID of the Embedded MIDI IN jack */
        0x03, /* ID of the Embedded MIDI IN jack */

        /* MIDI Adapter Standard Bulk IN Endpoint Descriptor */
        0x09,
        USB_ENDPOINT_DESCRIPTOR,
        0x82, /* IN endpoint 2 */
        USB_BULK_PIPE,
        64, 0x00, /* max packet size */
        0x00, /* interval */
        0x00, /* refresh */
        0x00, /* synch address */

        /* Class-specific MS Bulk IN Endpoint Descriptor */
        0x06,
        USB_CS_ENDPOINT,
        MS_GENERAL,
        0x02, /* Number of embedded MIDI OUT jacks */
        0x05, /* ID of the Embedded MIDI OUT jack */
        0x07 /* ID of the Embedded MIDI OUT jack */

};

uint_8 USB_DESC_CONST USB_STR_0[USB_STR_0_SIZE+USB_STR_DESC_SIZE] =
                                    {sizeof(USB_STR_0),
                                     USB_STRING_DESCRIPTOR,
                                      0x09,
                                      0x04/*equiavlent to 0x0409*/
                                    };

uint_8 USB_DESC_CONST USB_STR_1[USB_STR_1_SIZE+USB_STR_DESC_SIZE]
                          = {  sizeof(USB_STR_1),
                               USB_STRING_DESCRIPTOR,
                              'l',0,
                              'd',0,
                              '0',0,
                              'd',0,
                              '@',0,
                              'i',0,
                              'k',0,
                              'i',0,
                              '.',0,
                              'f',0,
                              'i',0
                          };


uint_8 USB_DESC_CONST USB_STR_2[USB_STR_2_SIZE+USB_STR_DESC_SIZE]
                          = {  sizeof(USB_STR_2),
                               USB_STRING_DESCRIPTOR,
                               'u',0,
                               's',0,
                               'b',0,
                               'm',0,
                               'i',0,
                               'd',0,
                               'i',0,
                               '2',0,
                               ' ',0,
                               's',0,
                               'l',0,
                               'a',0,
                               'v',0,
                               'e',0,
                               ' ',0
                          };

uint_8 USB_DESC_CONST USB_STR_3[USB_STR_3_SIZE+USB_STR_DESC_SIZE]
                          = {  sizeof(USB_STR_3),
                               USB_STRING_DESCRIPTOR,
                               '0',0
                          };

uint_8 USB_DESC_CONST USB_STR_4[USB_STR_4_SIZE+USB_STR_DESC_SIZE]
                          = {  sizeof(USB_STR_4),
                               USB_STRING_DESCRIPTOR,
                               'U',0,
                               'S',0,
                               'B',0,
                               ' ',0,
                               'O',0,
                               'u',0,
                               't',0
                          };

uint_8 USB_DESC_CONST USB_STR_5[USB_STR_5_SIZE+USB_STR_DESC_SIZE]
                          = {  sizeof(USB_STR_5),
                               USB_STRING_DESCRIPTOR,
                               'E',0,
                               'x',0,
                               't',0,
                               ' ',0,
                               'M',0,
                               'I',0,
                               'D',0,
                               'I',0,
                               ' ',0,
                               'O',0,
                               'u',0,
                               't',0
                          };
uint_8 USB_DESC_CONST USB_STR_6[USB_STR_6_SIZE+USB_STR_DESC_SIZE]
                          = {  sizeof(USB_STR_6),
                               USB_STRING_DESCRIPTOR,
                               'U',0,
                               'S',0,
                               'B',0,
                               ' ',0,
                               'I',0,
                               'n',0
                          };

uint_8 USB_DESC_CONST USB_STR_7[USB_STR_7_SIZE+USB_STR_DESC_SIZE]
                          = {  sizeof(USB_STR_7),
                               USB_STRING_DESCRIPTOR,
                               'E',0,
                               'x',0,
                               't',0,
                               ' ',0,
                               'M',0,
                               'I',0,
                               'D',0,
                               'I',0,
                               ' ',0,
                               'I',0,
                               'n',0
                          };

uint_8 USB_DESC_CONST USB_STR_n[USB_STR_n_SIZE+USB_STR_DESC_SIZE]
                          = {  sizeof(USB_STR_n),
                               USB_STRING_DESCRIPTOR,
                               'B',0,
                               'A',0,
                               'D',0,
                               ' ',0,
                               'S',0,
                               'T',0,
                               'R',0,
                               'I',0,
                               'N',0,
                               'G',0,
                               ' ',0,
                               'I',0,
                               'N',0,
                               'D',0,
                               'E',0,
                               'X',0
                          };


USB_PACKET_SIZE const g_std_desc_size[USB_MAX_STD_DESCRIPTORS+1] =
                                    {0,
                                     DEVICE_DESCRIPTOR_SIZE,
                                     CONFIG_DESC_SIZE,
                                     0, /* string */
                                     0, /* Interface */
                                     0, /* Endpoint */
                                     0, /* Device Qualifier */
                                     0 /* other speed config */
                                    };

uint_8_ptr const g_std_descriptors[USB_MAX_STD_DESCRIPTORS+1] =
                            {
                                NULL,
                                (uint_8_ptr)g_device_descriptor,
                                (uint_8_ptr)g_config_descriptor,
                                NULL, /* string */
                                NULL, /* Interface */
                                NULL, /* Endpoint */
                                NULL, /* Device Qualifier */
                                NULL  /* other speed config*/
                            };

uint_8 const g_string_desc_size[USB_MAX_STRING_DESCRIPTORS+1] =
                            {
                                sizeof(USB_STR_0),
                                sizeof(USB_STR_1),
                                sizeof(USB_STR_2),
                                sizeof(USB_STR_3),
                                sizeof(USB_STR_4),
                                sizeof(USB_STR_5),
                                sizeof(USB_STR_6),
                                sizeof(USB_STR_7),
                                sizeof(USB_STR_n)
                            };

uint_8_ptr const g_string_descriptors[USB_MAX_STRING_DESCRIPTORS+1] =
                            {
                                (uint_8_ptr const)USB_STR_0,
                                (uint_8_ptr const)USB_STR_1,
                                (uint_8_ptr const)USB_STR_2,
                                (uint_8_ptr const)USB_STR_3,
                                (uint_8_ptr const)USB_STR_4,
                                (uint_8_ptr const)USB_STR_5,
                                (uint_8_ptr const)USB_STR_6,
                                (uint_8_ptr const)USB_STR_7,
                                (uint_8_ptr const)USB_STR_n
                            };

#ifdef __HC08__ /* << EST */
#pragma MESSAGE DISABLE C4800 /* implicit cast in assignment */
#endif
USB_ALL_LANGUAGES g_languages = { USB_STR_0, sizeof(USB_STR_0),
		                          { /* << EST: missing braces */
		                            {
                                        (uint_16 const)0x0409,
                                        (const uint_8 **)g_string_descriptors,
                                        g_string_desc_size
                                    }
		                          } /* << EST: missing braces */
                                };
#ifdef __HC08__ /* << EST */
#pragma MESSAGE DEFAULT C4800
#endif

uint_8 const g_valid_config_values[USB_MAX_CONFIG_SUPPORTED+1]={0,1};

/****************************************************************************
 * Global Variables
 ****************************************************************************/
#ifdef _MC9S08JS16_H
#pragma DATA_SEG APP_DATA
#endif

static uint_8 g_line_coding[USB_MAX_SUPPORTED_INTERFACES][LINE_CODING_SIZE] =
{
        { (LINE_CODE_DTERATE_IFACE0>> 0) & 0x000000FF,
          (LINE_CODE_DTERATE_IFACE0>> 8) & 0x000000FF,
          (LINE_CODE_DTERATE_IFACE0>>16) & 0x000000FF,
          (LINE_CODE_DTERATE_IFACE0>>24) & 0x000000FF,
          /*e.g. 0x00,0xC2,0x01,0x00 : 0x0001C200 is 115200 bits per second */
           LINE_CODE_CHARFORMAT_IFACE0,
           LINE_CODE_PARITYTYPE_IFACE0,
           LINE_CODE_DATABITS_IFACE0
        }
};

static uint_8 g_abstract_state[USB_MAX_SUPPORTED_INTERFACES][COMM_FEATURE_DATA_SIZE] =
{
        { (STATUS_ABSTRACT_STATE_IFACE0>>0) & 0x00FF,
          (STATUS_ABSTRACT_STATE_IFACE0>>8) & 0x00FF
        }
};

static uint_8 g_country_code[USB_MAX_SUPPORTED_INTERFACES][COMM_FEATURE_DATA_SIZE] =
{
        { (COUNTRY_SETTING_IFACE0>>0) & 0x00FF,
          (COUNTRY_SETTING_IFACE0>>8) & 0x00FF
        }
};

static uint_8 g_alternate_interface[USB_MAX_SUPPORTED_INTERFACES];

/*****************************************************************************
 * Local Types - None
 *****************************************************************************/

/*****************************************************************************
 * Local Functions Prototypes
 *****************************************************************************/

/*****************************************************************************
 * Local Variables - None
 *****************************************************************************/


 /*****************************************************************************
 * Local Functions - None
 *****************************************************************************/

/*****************************************************************************
 * Global Functions
 *****************************************************************************/
/**************************************************************************//*!
 *
 * @name  USB_Desc_Get_Descriptor
 *
 * @brief The function returns the correponding descriptor
 *
 * @param controller_ID : Controller ID
 * @param type          : type of descriptor requested
 * @param sub_type      : string index for string descriptor
 * @param index         : string descriptor language Id
 * @param descriptor    : output descriptor pointer
 * @param size          : size of descriptor returned
 *
 * @return USB_OK                      When Successfull
 *         USBERR_INVALID_REQ_TYPE     when Error
 *****************************************************************************
 * This function is used to pass the pointer to the requested descriptor
 *****************************************************************************/
uint_8 USB_Desc_Get_Descriptor (
     uint_8 controller_ID,   /* [IN] Controller ID */
     uint_8 type,            /* [IN] type of descriptor requested */
     uint_8 str_num,         /* [IN] string index for string descriptor */
     uint_16 index,          /* [IN] string descriptor language Id */
     uint_8_ptr *descriptor, /* [OUT] output descriptor pointer */
     USB_PACKET_SIZE *size   /* [OUT] size of descriptor returned */
)
{
     UNUSED (controller_ID)

    /* string descriptors are handled saperately */
    if (type == USB_STRING_DESCRIPTOR)
    {
        if(index == 0)
        {
            /* return the string and size of all languages */
            *descriptor = (uint_8_ptr)g_languages.languages_supported_string;
            *size = g_languages.languages_supported_size;
        } else
        {
            uint_8 lang_id=0;
            uint_8 lang_index=USB_MAX_LANGUAGES_SUPPORTED;

            for(;lang_id< USB_MAX_LANGUAGES_SUPPORTED;lang_id++)
            {
                /* check whether we have a string for this language */
                if(index == g_languages.usb_language[lang_id].language_id)
                {   /* check for max descriptors */
                    if(str_num < USB_MAX_STRING_DESCRIPTORS)
                    {   /* setup index for the string to be returned */
                        lang_index=str_num;
                    }

                    break;
                }

            }

            /* set return val for descriptor and size */
            *descriptor = (uint_8_ptr)g_languages.usb_language[lang_id].
                                                lang_desc[lang_index];
            *size = g_languages.usb_language[lang_id].
                                                lang_desc_size[lang_index];
        }

    }
    else if (type < USB_MAX_STD_DESCRIPTORS+1)
    {
        /* Set return val for descriptor and size */
        *descriptor = (uint_8_ptr)g_std_descriptors [type];

        /* if there is no descriptor then return error */
        if(*descriptor == NULL)
        {
            return USBERR_INVALID_REQ_TYPE;
        }

        *size = g_std_desc_size[type];
    }
    else /* invalid descriptor */
    {
        return USBERR_INVALID_REQ_TYPE;
    }

    return USB_OK;
}

/**************************************************************************//*!
 *
 * @name  USB_Desc_Get_Interface
 *
 * @brief The function returns the alternate interface
 *
 * @param controller_ID : Controller Id
 * @param interface     : Interface number
 * @param alt_interface : Output alternate interface
 *
 * @return USB_OK                     When Successfull
 *         USBERR_INVALID_REQ_TYPE    when Error
 *****************************************************************************
 * This function is called by the framework module to get the current interface
 *****************************************************************************/
uint_8 USB_Desc_Get_Interface (
      uint_8 controller_ID,     /* [IN] Controller ID */
      uint_8 interface,         /* [IN] interface number */
      uint_8_ptr alt_interface  /* [OUT] output alternate interface */
)
{
    UNUSED (controller_ID)
    /* if interface valid */
    if(interface < USB_MAX_SUPPORTED_INTERFACES)
    {
        /* get alternate interface*/
        *alt_interface = g_alternate_interface[interface];
        return USB_OK;
    }

    return USBERR_INVALID_REQ_TYPE;
}

/**************************************************************************//*!
 *
 * @name  USB_Desc_Set_Interface
 *
 * @brief The function sets the alternate interface
 *
 * @param controller_ID : Controller Id
 * @param interface     : Interface number
 * @param alt_interface : Input alternate interface
 *
 * @return USB_OK                              When Successfull
 *         USBERR_INVALID_REQ_TYPE             when Error
 *****************************************************************************
 * This function is called by the framework module to set the interface
 *****************************************************************************/
uint_8 USB_Desc_Set_Interface (
      uint_8 controller_ID, /* [IN] Controller ID */
      uint_8 interface,     /* [IN] interface number */
      uint_8 alt_interface  /* [IN] input alternate interface */
)
{
    UNUSED (controller_ID)
    /* if interface valid */
    if(interface < USB_MAX_SUPPORTED_INTERFACES)
    {
        /* set alternate interface*/
        g_alternate_interface[interface]=alt_interface;
            return USB_OK;
    }

    return USBERR_INVALID_REQ_TYPE;
}

/**************************************************************************//*!
 *
 * @name  USB_Desc_Valid_Configation
 *
 * @brief The function checks whether the configuration parameter
 *        input is valid or not
 *
 * @param controller_ID : Controller Id
 * @param config_val    : Configuration value
 *
 * @return TRUE           When Valid
 *         FALSE          When Error
 *****************************************************************************
 * This function checks whether the configuration is valid or not
 *****************************************************************************/
boolean USB_Desc_Valid_Configation (
      uint_8 controller_ID,/*[IN] Controller ID */
      uint_16 config_val   /*[IN] configuration value */
)
{
    uint_8 loop_index=0;
    UNUSED (controller_ID)

    /* check with only supported val right now */
    while(loop_index < (USB_MAX_CONFIG_SUPPORTED+1))
    {
        if(config_val == g_valid_config_values[loop_index])
        {
            return TRUE;
        }
        loop_index++;
    }

    return FALSE;
}
/**************************************************************************//*!
 *
 * @name  USB_Desc_Valid_Interface
 *
 * @brief The function checks whether the interface parameter
 *        input is valid or not
 *
 * @param controller_ID : Controller Id
 * @param interface     : Target interface
 *
 * @return TRUE           When Valid
 *         FALSE          When Error
 *****************************************************************************
 * This function checks whether the interface is valid or not
 *****************************************************************************/
boolean USB_Desc_Valid_Interface (
      uint_8 controller_ID, /*[IN] Controller ID */
      uint_8 interface      /*[IN] target interface */
)
{
    uint_8 loop_index=0;
    UNUSED (controller_ID)

    /* check with only supported val right now */
    while(loop_index < USB_MAX_SUPPORTED_INTERFACES)
    {
        if(interface == g_alternate_interface[loop_index])
        {
            return TRUE;
        }
        loop_index++;
    }

    return FALSE;
}

/**************************************************************************//*!
 *
 * @name  USB_Desc_Remote_Wakeup
 *
 * @brief The function checks whether the remote wakeup is supported or not
 *
 * @param controller_ID : Controller ID
 *
 * @return REMOTE_WAKEUP_SUPPORT (TRUE) - if remote wakeup supported
 *****************************************************************************
 * This function returns remote wakeup is supported or not
 *****************************************************************************/
boolean USB_Desc_Remote_Wakeup (
      uint_8 controller_ID  /* [IN] Controller ID */
)
{
    UNUSED (controller_ID)
    return REMOTE_WAKEUP_SUPPORT;
}

/**************************************************************************//*!
 *
 * @name  USB_Desc_Get_Endpoints
 *
 * @brief The function returns with the list of all non control endpoints used
 *
 * @param controller_ID : Controller ID
 *
 * @return pointer to USB_ENDPOINTS
 *****************************************************************************
 * This function returns the information about all the non control endpoints
 * implemented
 *****************************************************************************/
void* USB_Desc_Get_Endpoints (
      uint_8 controller_ID  /* [IN] Controller ID */
)
{
    UNUSED (controller_ID)
    return (void*)&usb_desc_ep;
}

/**************************************************************************//*!
 *
 * @name  USB_Desc_Get_Line_Coding
 *
 * @brief The function returns the Line Coding/Configuraion
 *
 * @param controller_ID : Controller ID
 * @param interface     : Interface number
 * @param coding_data   : Output line coding data
 *
 * @return USB_OK                              When Successfull
 *         USBERR_INVALID_REQ_TYPE             when Error
 *****************************************************************************
 * Returns current Line Coding Parameters
 *****************************************************************************/
uint_8 USB_Desc_Get_Line_Coding (
    uint_8 controller_ID,       /* [IN] Controller ID */
    uint_8 interface,           /* [IN] Interface Number */
    uint_8_ptr *coding_data     /* [OUT] Line Coding Data */
)
{
    UNUSED (controller_ID)
    /* if interface valid */
    if(interface < USB_MAX_SUPPORTED_INTERFACES)
    {
        /* get line coding data*/
        *coding_data = g_line_coding[interface];
        return USB_OK;
    }

    return USBERR_INVALID_REQ_TYPE;
}

/**************************************************************************//*!
 *
 * @name  USB_Desc_Set_Line_Coding
 *
 * @brief The function sets the Line Coding/Configuraion
 *
 * @param controller_ID : Controller ID
 * @param interface     : Interface number
 * @param coding_data   : Output line coding data
 *
 * @return USB_OK                              When Successfull
 *         USBERR_INVALID_REQ_TYPE             when Error
 *****************************************************************************
 * Sets Line Coding Structure with the HOST specified values
 *****************************************************************************/
uint_8 USB_Desc_Set_Line_Coding (
    uint_8 controller_ID,       /* [IN] Controller ID */
    uint_8 interface,           /* [IN] Interface Number */
    uint_8_ptr *coding_data     /* [IN] Line Coding Data */
)
{
    uint_8 count;
    UNUSED (controller_ID)

    /* if interface valid */
    if(interface < USB_MAX_SUPPORTED_INTERFACES)
    {
        /* set line coding data*/
        for (count = 0; count < LINE_CODING_SIZE; count++)
        {
            g_line_coding[interface][count] = *((*coding_data +
                                                USB_SETUP_PKT_SIZE) + count);
        }
        return USB_OK;
    }

    return USBERR_INVALID_REQ_TYPE;
}

/**************************************************************************//*!
 *
 * @name  USB_Desc_Get_Abstract_State
 *
 * @brief The function gets the current setting for communication feature
 *                                                  (ABSTRACT_STATE)
 * @param controller_ID : Controller ID
 * @param interface     : Interface number
 * @param feature_data  : Output comm feature data
 *
 * @return USB_OK                              When Successfull
 *         USBERR_INVALID_REQ_TYPE             when Error
 *****************************************************************************
 * Returns ABSTRACT STATE Communication Feature to the Host
 *****************************************************************************/
uint_8 USB_Desc_Get_Abstract_State (
    uint_8 controller_ID,       /* [IN] Controller ID */
    uint_8 interface,           /* [IN] Interface Number */
    uint_8_ptr *feature_data    /* [OUT] Output Comm Feature Data */
)
{
    UNUSED (controller_ID)
    /* if interface valid */
    if(interface < USB_MAX_SUPPORTED_INTERFACES)
    {
        /* get line coding data*/
        *feature_data = g_abstract_state[interface];
        return USB_OK;
    }

    return USBERR_INVALID_REQ_TYPE;
}

/**************************************************************************//*!
 *
 * @name  USB_Desc_Get_Country_Setting
 *
 * @brief The function gets the current setting for communication feature
 *                                                  (COUNTRY_CODE)
 * @param controller_ID : Controller ID
 * @param interface     : Interface number
 * @param feature_data  : Output comm feature data
 *
 * @return USB_OK                              When Successfull
 *         USBERR_INVALID_REQ_TYPE             when Error
 *****************************************************************************
 * Returns the country Code to the Host
 *****************************************************************************/
uint_8 USB_Desc_Get_Country_Setting (
    uint_8 controller_ID,       /* [IN] Controller ID */
    uint_8 interface,           /* [IN] Interface Number */
    uint_8_ptr *feature_data    /* [OUT] Output Comm Feature Data */
)
{
    UNUSED (controller_ID)
    /* if interface valid */
    if(interface < USB_MAX_SUPPORTED_INTERFACES)
    {
        /* get line coding data*/
        *feature_data = g_country_code[interface];
        return USB_OK;
    }

    return USBERR_INVALID_REQ_TYPE;
}

/**************************************************************************//*!
 *
 * @name  USB_Desc_Set_Abstract_State
 *
 * @brief The function gets the current setting for communication feature
 *                                                  (ABSTRACT_STATE)
 * @param controller_ID : Controller ID
 * @param interface     : Interface number
 * @param feature_data  : Output comm feature data
 *
 * @return USB_OK                              When Successfull
 *         USBERR_INVALID_REQ_TYPE             when Error
 *****************************************************************************
 * Sets the ABSTRACT State specified by the Host
 *****************************************************************************/
uint_8 USB_Desc_Set_Abstract_State (
    uint_8 controller_ID,       /* [IN] Controller ID */
    uint_8 interface,           /* [IN] Interface Number */
    uint_8_ptr *feature_data    /* [OUT] Output Comm Feature Data */
)
{
    uint_8 count;
    UNUSED (controller_ID)

    /* if interface valid */
    if(interface < USB_MAX_SUPPORTED_INTERFACES)
    {
        /* set Abstract State Feature*/
        for (count = 0; count < COMM_FEATURE_DATA_SIZE; count++)
        {
            g_abstract_state[interface][count] = *(*feature_data + count);
        }
        return USB_OK;
    }

    return USBERR_INVALID_REQ_TYPE;
}

/**************************************************************************//*!
 *
 * @name  USB_Desc_Set_Country_Setting
 *
 * @brief The function gets the current setting for communication feature
 *                                                  (COUNTRY_CODE)
 * @param controller_ID : Controller ID
 * @param interface     : Interface number
 * @param feature_data  : Output comm feature data
 *
 * @return USB_OK                              When Successfull
 *         USBERR_INVALID_REQ_TYPE             when Error
 *****************************************************************************
 * Sets the country code specified by the HOST
 *****************************************************************************/
uint_8 USB_Desc_Set_Country_Setting(
    uint_8 controller_ID,       /* [IN] Controller ID */
    uint_8 interface,           /* [IN] Interface Number */
    uint_8_ptr *feature_data    /* [OUT] Output Comm Feature Data */
)
{
    uint_8 count;
    UNUSED (controller_ID)

    /* if interface valid */
    if(interface < USB_MAX_SUPPORTED_INTERFACES)
    {
        for (count = 0; count < COMM_FEATURE_DATA_SIZE; count++)
        {
            g_country_code[interface][count] = *(*feature_data + count);
        }
        return USB_OK;
    }

    return USBERR_INVALID_REQ_TYPE;
}
