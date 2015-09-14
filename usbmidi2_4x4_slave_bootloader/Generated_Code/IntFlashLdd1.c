/* ###################################################################
**     THIS COMPONENT MODULE IS GENERATED BY THE TOOL. DO NOT MODIFY IT.
**     Filename    : IntFlashLdd1.c
**     Project     : usbmidi2_4x4_slave_bootloader
**     Processor   : MKL26Z64VFM4
**     Component   : FLASH_LDD
**     Version     : Component 01.106, Driver 01.15, CPU db: 3.00.000
**     Compiler    : GNU C Compiler
**     Date/Time   : 2015-08-10, 15:48, # CodeGen: 0
**     Abstract    :
**          This embedded component implements an access to an on-chip flash memory.
**          Using this component the flash memory could be written to, erased,
**          read from. Some other services would be provided, if they are supported
**          by hardware, e.g. changes of the flash memory protection state.
**     Settings    :
**          Component name                                 : IntFlashLdd1
**          Device                                         : FTFA
**          Use user memory areas                          : no
**          Interrupt service                              : Disabled
**            Write batch size                             : Minimal
**            Erase batch size                             : Minimal
**            Read batch size                              : Unlimited
**          Safe launch and wait                           : yes
**            Safe routine location                        : On stack
**            Interruptable wait loop                      : no
**          Initialization                                 : 
**            Enabled in init. code                        : yes
**            Auto initialization                          : no
**            Event mask                                   : 
**              OnOperationComplete                        : Enabled
**              OnError                                    : Enabled
**            CPU clock/configuration selection            : 
**              Clock configuration 0                      : This component enabled
**              Clock configuration 1                      : This component disabled
**              Clock configuration 2                      : This component disabled
**              Clock configuration 3                      : This component disabled
**              Clock configuration 4                      : This component disabled
**              Clock configuration 5                      : This component disabled
**              Clock configuration 6                      : This component disabled
**              Clock configuration 7                      : This component disabled
**     Contents    :
**         Init               - LDD_TDeviceData* IntFlashLdd1_Init(LDD_TUserData *UserDataPtr);
**         Write              - LDD_TError IntFlashLdd1_Write(LDD_TDeviceData *DeviceDataPtr, LDD_TData...
**         Erase              - LDD_TError IntFlashLdd1_Erase(LDD_TDeviceData *DeviceDataPtr,...
**         GetOperationStatus - LDD_FLASH_TOperationStatus IntFlashLdd1_GetOperationStatus(LDD_TDeviceData...
**         GetError           - void IntFlashLdd1_GetError(LDD_TDeviceData *DeviceDataPtr,...
**         Main               - void IntFlashLdd1_Main(LDD_TDeviceData *DeviceDataPtr);
**         GetDriverState     - LDD_TDriverState IntFlashLdd1_GetDriverState(LDD_TDeviceData *DeviceDataPtr);
**
**     Copyright : 1997 - 2014 Freescale Semiconductor, Inc. 
**     All Rights Reserved.
**     
**     Redistribution and use in source and binary forms, with or without modification,
**     are permitted provided that the following conditions are met:
**     
**     o Redistributions of source code must retain the above copyright notice, this list
**       of conditions and the following disclaimer.
**     
**     o Redistributions in binary form must reproduce the above copyright notice, this
**       list of conditions and the following disclaimer in the documentation and/or
**       other materials provided with the distribution.
**     
**     o Neither the name of Freescale Semiconductor, Inc. nor the names of its
**       contributors may be used to endorse or promote products derived from this
**       software without specific prior written permission.
**     
**     THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
**     ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
**     WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
**     DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
**     ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
**     (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
**     LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
**     ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
**     (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
**     SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**     
**     http: www.freescale.com
**     mail: support@freescale.com
** ###################################################################*/
/*!
** @file IntFlashLdd1.c
** @version 01.15
** @brief
**          This embedded component implements an access to an on-chip flash memory.
**          Using this component the flash memory could be written to, erased,
**          read from. Some other services would be provided, if they are supported
**          by hardware, e.g. changes of the flash memory protection state.
*/         
/*!
**  @addtogroup IntFlashLdd1_module IntFlashLdd1 module documentation
**  @{
*/         

/* MODULE IntFlashLdd1. */

#include "IFsh1.h"
#include "IntFlashLdd1.h"
/* {Default RTOS Adapter} No RTOS includes */

#ifdef __cplusplus
extern "C" {
#endif

#define FTFE_FLEXNVM_START_ADDR          0x10000000U
/* bit 23 selects between pFLASH and FLEXNVM regions */
#define FTFE_FLEXNVM_CCOB_START_ADDR     0x00800000U

#define SAFE_ROUTINE_SIZE 25U

typedef void (* LDD_FLASH_TSafeRoutinePtr)(void); /* Safe routine pointer type */

typedef struct{
  uint8_t SafeRoutine[SAFE_ROUTINE_SIZE]; /* Safe routine buffer */
} LDD_FLASH_TSafeRoutine;              /* Safe routine buffer type */

/*
Safe launch and wait routine code. This code launch a command and
waits until command completion or command violation. Routine is
written as array of bytes to be constant size and compiler independent.
C language implementation could look like:

void SafeRoutine(void) {
  FTFA_PDD_LaunchCommand(FTFA_BASE_PTR);
  while ((FTFA_PDD_GetFlags(FTFA_BASE_PTR) & FTFA_PDD_COMMAND_COMPLETE) == 0U ) {};
}

*/
const static uint8_t SafeRoutine[SAFE_ROUTINE_SIZE - 1U] = {
  0x00U,0xb5U,             /*  push	{lr}                                 */
  0x04U,0x4aU,             /*  ldr	r2, [pc, #16]	; (bc <SafeRoutine___+0x14>) */
  0x80U,0x23U,             /*  movs	r3, #128	; 0x80               */
  0x13U,0x70U,             /*  strb	r3, [r2, #0]                         */
  0x11U,0x78U,             /*  ldrb	r1, [r2, #0]                         */
  0x48U,0xb2U,             /*  sxtb	r0, r1                               */
  0x00U,0x28U,             /*  cmp	r0, #0                                       */
  0xfbU,0xdaU,             /*  bge.n	b0 <SafeRoutine___+0x8>              */
  0x00U,0xbdU,             /*  pop	{pc}                                         */
  0xc0U,0x46U,             /*  nop			; (mov r8, r8)               */
  0x00U,0x00U,0x02U,0x40U  /*  dword	0x40020000                           */
};


typedef struct {
  LDD_FLASH_TOperationType    CurrentOperation; /* Current operation type */
  LDD_FLASH_TOperationStatus  CurrentOperationStatus; /* Current operation status */
  LDD_FLASH_TCommand          CurrentCommand; /* Last flash device command */
  LDD_FLASH_TErrorFlags       CurrentErrorFlags; /* Current error flags */
  LDD_FLASH_TDataSize         CurrentDataSize; /* Data size of the current operation step */
  uint32_t                    DataCounter; /* Counter of data to be proceeded in the current operation */
  uint8_t                    *CurrentDataPtr; /* Pointer to the source/destination data for the current operation step */
  LDD_FLASH_TAddress          CurrentFlashAddress; /* Address of destination/source in the flash memory for the operation step */
  LDD_TUserData              *UserDataPtr; /* Pointer to user data */
} IntFlashLdd1_TDeviceData;

typedef IntFlashLdd1_TDeviceData *IntFlashLdd1_TDeviceDataPtr; /* Pointer to the device data structure. */
/* {Default RTOS Adapter} Static object used for simulation of dynamic driver memory allocation */
static IntFlashLdd1_TDeviceData DeviceDataPrv__DEFAULT_RTOS_ALLOC;

#define AVAILABLE_EVENTS_MASK (LDD_FLASH_ON_OPERATION_COMPLETE | LDD_FLASH_ON_ERROR)

/* Internal method prototypes */
static LDD_TError RangeCheck(LDD_FLASH_TAddress Address, LDD_FLASH_TDataSize Size);
static void SafeRoutineCaller(void);

/*
** ===================================================================
**     Method      :  IntFlashLdd1_Init (component FLASH_LDD)
*/
/*!
**     @brief
**         Initializes the device. Allocates memory for the device data
**         structure, allocates interrupt vectors and sets interrupt
**         priority, sets pin routing, sets timing, etc. If the "Enable
**         in init. code" property is set to "yes" ([Enabled in init.
**         code]) value then the device is also enabled(see the
**         description of the Enable() method). In this case the
**         Enable() method is not necessary and needn't to be generated.
**         This method can be called only once. Before the second call
**         of Init() the Deinit() must be called first.
**     @param
**         UserDataPtr     - Pointer to the user or
**                           RTOS specific data. This pointer will be
**                           passed as an event or callback parameter.
**     @return
**                         - Device data structure pointer.
*/
/* ===================================================================*/
LDD_TDeviceData* IntFlashLdd1_Init(LDD_TUserData *UserDataPtr)
{
  IntFlashLdd1_TDeviceDataPtr DeviceDataPrv;

  /* Allocate LDD device structure */
  /* {Default RTOS Adapter} Driver memory allocation: Dynamic allocation is simulated by a pointer to the static object */
  DeviceDataPrv = &DeviceDataPrv__DEFAULT_RTOS_ALLOC;
  DeviceDataPrv->CurrentOperationStatus = LDD_FLASH_IDLE; /* Initialization of the Current operation status item */
  DeviceDataPrv->CurrentOperation = LDD_FLASH_NO_OPERATION; /* Initialization of the Current operation type item */
  DeviceDataPrv->CurrentErrorFlags = 0U; /* Initialization of the Current error flags item */
  DeviceDataPrv->CurrentDataPtr = NULL; /* Initialization of the Current data pointer item */
  /* Copy of the safe routine to a buffer for execution */
  DeviceDataPrv->UserDataPtr = UserDataPtr; /* Store a pointer to the User data structure */
  /* SIM_SCGC6: FTF=1 */
  SIM_SCGC6 |= SIM_SCGC6_FTF_MASK;
  /* Registration of the device structure */
  PE_LDD_RegisterDeviceStructure(PE_LDD_COMPONENT_IntFlashLdd1_ID,DeviceDataPrv);
  return DeviceDataPrv;
}

/*
** ===================================================================
**     Method      :  IntFlashLdd1_Write (component FLASH_LDD)
*/
/*!
**     @brief
**         This method sets up a flash memory write operation. The
**         operation itself is performing by defined batches (property
**         [Write batch size]) by periodical calling the component�s
**         Main method in the user application (higher level OS service)
**         or by the component�s ISR, if an component�s interrupt
**         service is enabled.
**     @param
**         DeviceDataPtr   - Device data structure
**                           pointer returned by [Init] method.
**     @param
**         FromPtr         - Pointer to data to be written to
**                           the flash memory.
**     @param
**         ToAddress       - Address to the flash memory
**                           the input data would be written to.
**     @param
**         Size            - Size of the input data (in bytes) to be
**                           written to the flash memory.
**     @return
**                         - Error code
**                           ERR_OK - OK
**                           ERR_DISABLED - Component is disabled
**                           ERR_SPEED - This device does not work in
**                           the active clock configuration
**                           ERR_BUSY - Some flash memory operation is
**                           already in progress 
**                           ERR_PARAM_ADDRESS - Desired flash memory
**                           area is out of allowed range
**                           ERR_NOTAVAIL - When Safe launch and wait
**                           mode is enabled (property Safe launch and
**                           wait) and safe routine location is defined
**                           in runtime (property Safe routine location)
**                           and the safe routine location has not been
**                           specified yet (the SetSafeRoutineLocation
**                           method has not been used to define the
**                           location the safe routine will be copied to).
*/
/* ===================================================================*/
LDD_TError IntFlashLdd1_Write(LDD_TDeviceData *DeviceDataPtr, LDD_TData *FromPtr, LDD_FLASH_TAddress ToAddress, LDD_FLASH_TDataSize Size)
{
  IntFlashLdd1_TDeviceDataPtr DeviceDataPrv = (IntFlashLdd1_TDeviceDataPtr)DeviceDataPtr; /* Auxiliary variable - pointer to an internal state structure */

  if (!(DeviceDataPrv->CurrentOperationStatus == LDD_FLASH_IDLE) && /* If some operation is already in progress return error */\
     !(DeviceDataPrv->CurrentOperationStatus == LDD_FLASH_STOP) && \
     !(DeviceDataPrv->CurrentOperationStatus == LDD_FLASH_FAILED)) {
    return ERR_BUSY;
  }
  if (RangeCheck(ToAddress, Size) != (LDD_TError)ERR_OK) { /* Is the given flash memory area in the allowed range? */
    return ERR_PARAM_ADDRESS;          /* If no, return error */
  }
  /* Filling of the internal state structure */
  DeviceDataPrv->CurrentOperation = LDD_FLASH_WRITE; /* Set the current operation type */
  DeviceDataPrv->CurrentOperationStatus = LDD_FLASH_START; /* Set the current operation status to START */
  DeviceDataPrv->CurrentCommand = LDD_FLASH_WRITE_LONG_WORD; /* Set the flash command to be proceed for the operation */
  DeviceDataPrv->DataCounter = Size;   /* Reset Data counter */
  DeviceDataPrv->CurrentDataPtr = (uint8_t *)FromPtr; /* Reset the "From pointer" for the operation */
  DeviceDataPrv->CurrentFlashAddress = ToAddress; /* Reset the "To address" for the operation */
  DeviceDataPrv->CurrentDataSize = 0U; /* Reset Current data size */
  return ERR_OK;                       /* Return with no error */
}

/*
** ===================================================================
**     Method      :  IntFlashLdd1_Erase (component FLASH_LDD)
*/
/*!
**     @brief
**         This method sets up a flash memory erase operation. The
**         operation itself is performing by defined batches (property
**         [Erase batch size]) by periodical calling the component�s
**         Main method in the user application (higher level OS service)
**         or by the component�s ISR, if an component�s interrupt
**         service is enabled.
**     @param
**         DeviceDataPtr   - Device data structure
**                           pointer returned by [Init] method.
**     @param
**         FromAddress     - Address of the flash
**                           memory area (the first erase sector is the
**                           sector the given address belongs to) to be
**                           erased.
**     @param
**         Size            - Size of the flash memory area (in bytes)
**                           to be erased. The flash memory is erased by
**                           the erase sectors. The first erased sector
**                           is a sector the address specified by the
**                           input parameter Address belongs to. The
**                           last erased sector is a sector the address
**                           calculated like an addition of the address
**                           specified by the input parameter Address
**                           and the size specified by the input
**                           parameter Size belongs to.
**     @return
**                         - Error code
**                           ERR_OK - OK
**                           ERR_DISABLED - Component is disabled
**                           ERR_SPEED - This device does not work in
**                           the active clock configuration
**                           ERR_BUSY - Some flash memory operation is
**                           already in progress 
**                           ERR_PARAM_ADDRESS - Desired flash memory
**                           area is out of allowed range or is not
**                           aligned to erasable units' bounderies
**                           ERR_NOTAVAIL - When Safe launch and wait
**                           mode is enabled (property Safe launch and
**                           wait) and safe routine location is defined
**                           in runtime (property Safe routine location)
**                           and the safe routine location has not been
**                           specified yet (the SetSafeRoutineLocation
**                           method has not been used to define the
**                           location the safe routine will be copied to).
*/
/* ===================================================================*/

LDD_TError IntFlashLdd1_Erase(LDD_TDeviceData *DeviceDataPtr, LDD_FLASH_TAddress FromAddress, LDD_FLASH_TDataSize Size)
{
  IntFlashLdd1_TDeviceDataPtr DeviceDataPrv = (IntFlashLdd1_TDeviceDataPtr)DeviceDataPtr; /* Auxiliary variable - pointer to an internal state structure */

  if (!(DeviceDataPrv->CurrentOperationStatus == LDD_FLASH_IDLE) && /* If some operation is already in progress return error. */\
     !(DeviceDataPrv->CurrentOperationStatus == LDD_FLASH_STOP) && \
     !(DeviceDataPrv->CurrentOperationStatus == LDD_FLASH_FAILED)) {
    return ERR_BUSY;
  }
  if (RangeCheck(FromAddress, Size) != (LDD_TError)ERR_OK) { /* Is an address range of desired operation out of used flash memory areas? */
    return ERR_PARAM_ADDRESS;          /* If yes, return error. */
  }
  if (((FromAddress & IntFlashLdd1_ERASABLE_UNIT_MASK) != 0U) || ((Size & IntFlashLdd1_ERASABLE_UNIT_MASK) != 0U)){ /* Is the desired area misaligned to erasable unit borders? */
    return ERR_PARAM_ADDRESS;          /* If yes, return error. */
  }
  /* Filling of the internal state structure */
  DeviceDataPrv->CurrentOperation = LDD_FLASH_ERASE; /* Set the current operation type */
  DeviceDataPrv->CurrentOperationStatus = LDD_FLASH_START; /* Set the current operation status to START */
  DeviceDataPrv->DataCounter = Size;   /* Reset Data counter */
  DeviceDataPrv->CurrentDataPtr = NULL; /* Reset the "Data pointer" for the operation */
  DeviceDataPrv->CurrentFlashAddress = FromAddress; /* Reset the "From address" for the operation */
  DeviceDataPrv->CurrentDataSize = IntFlashLdd1_ERASABLE_UNIT_SIZE; /* Set the current data size */
  DeviceDataPrv->CurrentCommand = LDD_FLASH_ERASE_SECTOR; /* Set the current operation flash command */
  return ERR_OK;                       /* Return with no error */
}

/*
** ===================================================================
**     Method      :  IntFlashLdd1_GetOperationStatus (component FLASH_LDD)
*/
/*!
**     @brief
**         Returns a status of current (last) flash memory operation.
**     @param
**         DeviceDataPtr   - Device data structure
**                           pointer returned by [Init] method.
**     @return
**                         - Current flash memory operation status. See
**                           the LDD_FLASH_TOperationStatus type's
**                           description for details.
*/
/* ===================================================================*/
LDD_FLASH_TOperationStatus IntFlashLdd1_GetOperationStatus(LDD_TDeviceData *DeviceDataPtr)
{
  return ((IntFlashLdd1_TDeviceDataPtr)(DeviceDataPtr))->CurrentOperationStatus; /* Return current operation status */
}

/*
** ===================================================================
**     Method      :  IntFlashLdd1_Main (component FLASH_LDD)
*/
/*!
**     @brief
**         This method is used to perform one batch of a flash memory
**         operation. This method is used to perform batches of all
**         flash memory operations (Write, Read, Erase, EraseBlock,
**         VerifyErasedBlock) when the component works in the polled
**         mode (interrupt service is disabled - property [Interrupt
**         service]). This method performs batches of the read flash
**         memory operation, when the component works in the interrupt
**         triggered mode (interrupt service is enabled). This method
**         is enabled only if the component works in the polled mode or
**         if the Read method is enabled.
**     @param
**         DeviceDataPtr   - Device data structure
**                           pointer returned by [Init] method.
*/
/* ===================================================================*/
#define WRITE_BATCH_SIZE 1U
#define ERASE_BATCH_SIZE 1U
#define WRITABLE_UNIT_MASK 3U

void IntFlashLdd1_Main(LDD_TDeviceData *DeviceDataPtr)
{
  IntFlashLdd1_TDeviceDataPtr DeviceDataPrv = (IntFlashLdd1_TDeviceDataPtr)DeviceDataPtr; /* Auxiliary variable - pointer to an internal state structure */
  uint32_t DataToPrg = 0xFFFFFFFFLU;   /* Auxiliary variable - current data to be proceeded */
  uint32_t DataToPrgMask = 0U;
  uint32_t CurrentFlashPrgUnitData;    /* Current flash location content */
  uint8_t PrgBytesCount;               /* Auxiliary variable - Current data counter - number of data to be proceeded (in bytes) */
  uint8_t MaxPossiblePrgBytes = 0U;
  uint8_t DstAddrOffset = 0U;          /* Offset of the desired flash location to be written from the begging of the smallest writable unit the desired location belongs to */
  uint8_t i;
  LDD_FLASH_TAddress FlashPrgUnitAddr;
  uint16_t StepsOfBatch = 0U;          /* Number of batch cycles to be proceeded */
  uint8_t CurrentFlags;                /* Auxiliary variable - current hw flags */

  if ((DeviceDataPrv->CurrentOperationStatus != LDD_FLASH_RUNNING) && /* If there is not an operation in progress or pending then end */\
     (DeviceDataPrv->CurrentOperationStatus != LDD_FLASH_START) && \
     (DeviceDataPrv->CurrentOperationStatus != LDD_FLASH_STOP_REQ)) {
    return;
  }
  if ((FTFA_PDD_GetFlags(FTFA_BASE_PTR) & FTFA_PDD_COMMAND_COMPLETE) == 0U){ /* If there is some flash operation in progress then end */
    return;
  }
  if (DeviceDataPrv->CurrentOperationStatus == LDD_FLASH_START) { /* Is this run the first run of the Main method in this operation? */
    FTFA_PDD_ClearFlags(FTFA_BASE_PTR, /* If yes, clear hw flags */
                        FTFA_PDD_READ_COLLISION_ERROR | \
                        FTFA_PDD_ACCESS_ERROR | \
                        FTFA_PDD_PROTECTION_VIOLATION);
    DeviceDataPrv->CurrentErrorFlags = 0U; /* Clear Current SW error flags */
  } else {                             /* If this is not the first run of the operation, checks the error flags */
    CurrentFlags = FTFA_PDD_GetFlags(FTFA_BASE_PTR);
    DeviceDataPrv->CurrentErrorFlags |= CurrentFlags & /* Save current hw flags */\
    (FTFA_PDD_READ_COLLISION_ERROR | \
     FTFA_PDD_ACCESS_ERROR | \
     FTFA_PDD_PROTECTION_VIOLATION);
    if (DeviceDataPrv->CurrentErrorFlags != 0U) { /* Has some error occurred? */
      DeviceDataPrv->CurrentOperationStatus = LDD_FLASH_FAILED; /* If yes, set the operation state to FAILED */
      if (DeviceDataPrv->CurrentOperation != LDD_FLASH_READ) {
        MCM_PDD_InvalidateFlashCache(MCM_BASE_PTR);
        PE_NOP();
        PE_NOP();
        PE_NOP();
      }
      IntFlashLdd1_OnError(DeviceDataPrv->UserDataPtr); /* Invoke the OnError event */
      return;                          /* End */
    }
    if (DeviceDataPrv->DataCounter == 0U) { /* If all the data has been successfully proceeded, finish the operation */
      DeviceDataPrv->CurrentOperationStatus = LDD_FLASH_IDLE; /* If yes, change the operation state to IDLE */
      if (DeviceDataPrv->CurrentOperation != LDD_FLASH_READ) {
        MCM_PDD_InvalidateFlashCache(MCM_BASE_PTR);
        PE_NOP();
        PE_NOP();
        PE_NOP();
      }
      IntFlashLdd1_OnOperationComplete(DeviceDataPrv->UserDataPtr); /* Invoke the OnOperationComplete event */
      return;                          /* End */
    }
  }
  switch (DeviceDataPrv->CurrentOperation) { /* Define the batch counter's initial value according to the current operation */
    case LDD_FLASH_WRITE:              /* The current operation is Write */
      StepsOfBatch = WRITE_BATCH_SIZE; /* Initialize the batch counter */
      break;                           /* Break of the case */
    case LDD_FLASH_ERASE:              /* The current operation is Erase */
      StepsOfBatch = ERASE_BATCH_SIZE; /* Initialize the batch counter */
      break;                           /* Break of the case */
    default:
      break;
  }
  while (StepsOfBatch > 0U) {          /* Should another batch of the current operation be proceeded in this Main method processing? */
    CurrentFlags = FTFA_PDD_GetFlags(FTFA_BASE_PTR); /* Read current hardware flags */
    if ((CurrentFlags & (FTFA_PDD_READ_COLLISION_ERROR /* Check the error flags */\
                     | FTFA_PDD_ACCESS_ERROR \
                     | FTFA_PDD_PROTECTION_VIOLATION)) != 0U) {
      return;                          /* If there has some error occurred then end. The error will be analyzed in the next call of the Main method */
    }
    if (DeviceDataPrv->CurrentOperationStatus == LDD_FLASH_START) { /* Is this step the first step of the operation? */
      DeviceDataPrv->CurrentOperationStatus = LDD_FLASH_RUNNING; /* If yes, change Current operation status to RUNNING */
    } else {
      DeviceDataPrv->CurrentFlashAddress += DeviceDataPrv->CurrentDataSize; /* If no, define of an address of a step of the current operation (address of the first step of the operation is defined by a operational method, which defines the operation */
      if ((DeviceDataPrv->CurrentOperation == LDD_FLASH_READ) || (DeviceDataPrv->CurrentOperation == LDD_FLASH_WRITE)) {
        DeviceDataPrv->CurrentDataPtr += DeviceDataPrv->CurrentDataSize; /* Update of the Current data pointer */
      }
    }
    switch(DeviceDataPrv->CurrentOperation) { /* Perform needed actions for the next step of the operation (for the next flash operation command) according to the current operation type */
      case LDD_FLASH_ERASE:            /* Erase operation */
        DeviceDataPrv->DataCounter -= (uint32_t)(IntFlashLdd1_ERASABLE_UNIT_SIZE); /* Update of the Data counter */
        break;
      case LDD_FLASH_WRITE:
        DstAddrOffset = (uint8_t)DeviceDataPrv->CurrentFlashAddress & WRITABLE_UNIT_MASK; /* Compute the address of the writable unit */
        MaxPossiblePrgBytes = (WRITABLE_UNIT_MASK - DstAddrOffset) + 1U; /* Compute number of bytes from the destination address to the end of the writable unit */
        if (DeviceDataPrv->DataCounter < MaxPossiblePrgBytes) {
          PrgBytesCount = (uint8_t)DeviceDataPrv->DataCounter;
        } else {
          PrgBytesCount = MaxPossiblePrgBytes;
        }
        for (i = 0U; i < PrgBytesCount; i++)  {
          ((uint8_t *)(void *)&DataToPrg)[DstAddrOffset + i] = ((uint8_t *)(void *)(DeviceDataPrv->CurrentDataPtr))[i];
          ((uint8_t *)(void *)&DataToPrgMask)[DstAddrOffset + i] = 0xFFu;
        }
        FlashPrgUnitAddr = DeviceDataPrv->CurrentFlashAddress - DstAddrOffset;
        CurrentFlashPrgUnitData = *(uint32_t *)FlashPrgUnitAddr;
        if(((DataToPrg & DataToPrgMask) & (~CurrentFlashPrgUnitData)) > 0U) {
          DeviceDataPrv->CurrentErrorFlags |= LDD_FLASH_MULTIPLE_WRITE_ERROR;
          return;
        }
        DataToPrg = DataToPrg ^ (~CurrentFlashPrgUnitData);
        DeviceDataPrv->CurrentDataSize = PrgBytesCount;
        DeviceDataPrv->DataCounter -= PrgBytesCount;
        FTFA_PDD_WriteFCCOBLongWordData(FTFA_BASE_PTR, DataToPrg);
        break;
      default:
        break;
    } /* switch(DeviceDataPrv->CurrentOperation) */
    StepsOfBatch -= 1U;
    if (DeviceDataPrv->DataCounter == 0U) {
      StepsOfBatch = 0U;
    }
    if (DeviceDataPrv->CurrentOperation != LDD_FLASH_READ) {
      FTFA_PDD_SetFCCOBCommand(FTFA_BASE_PTR, DeviceDataPrv->CurrentCommand); /* Set the desired flash operation command */
      FTFA_PDD_SetFCCOBAddress(FTFA_BASE_PTR, ((uint32_t)(DeviceDataPrv->CurrentFlashAddress - DstAddrOffset))); /* Set an address of the flash memory location for the current flash operation command */
      SafeRoutineCaller();             /* Call of the safe routine caller - the safe routine's code will be placed to stack and run */
    }
  }
}

/*
** ===================================================================
**     Method      :  IntFlashLdd1_GetError (component FLASH_LDD)
*/
/*!
**     @brief
**         Returns an error state of a current (or last) flash
**         operation. 
**     @param
**         DeviceDataPtr   - Device data structure
**                           pointer returned by [Init] method.
**     @param
**         OperationStatus - Pointer to an
**                           output structure with the error status
**                           details. See the LDD_FLASH_TErrorStatus
**                           type's description for details.
*/
/* ===================================================================*/
void IntFlashLdd1_GetError(LDD_TDeviceData *DeviceDataPtr, LDD_FLASH_TErrorStatus *OperationStatus)
{
  IntFlashLdd1_TDeviceDataPtr DeviceDataPrv = (IntFlashLdd1_TDeviceDataPtr)DeviceDataPtr; /* Auxiliary variable - pointer to an internal state structure */

  /* copy of information from internal status structure to the output error status structure */
  /* {Default RTOS Adapter} Critical section begin, general PE function is used */
  EnterCritical();
  OperationStatus->CurrentOperation = DeviceDataPrv->CurrentOperation; /* Copy the Current operation type to the output structure */
  OperationStatus->CurrentCommand = DeviceDataPrv->CurrentCommand; /* Copy the Current command type to the output structure */
  OperationStatus->CurrentErrorFlags = DeviceDataPrv->CurrentErrorFlags; /* Copy the Current error flags to the output structure */
  OperationStatus->CurrentAddress = DeviceDataPrv->CurrentFlashAddress; /* Copy the Current flash address to the output structure */
  OperationStatus->CurrentDataPtr = DeviceDataPrv->CurrentDataPtr; /* Copy the Current input/output data pointer to the output structure */
  OperationStatus->CurrentDataSize = DeviceDataPrv->CurrentDataSize; /* Copy the Current input/output data size to the output structure */
  /* {Default RTOS Adapter} Critical section end, general PE function is used */
  ExitCritical();
}

/*
** ===================================================================
**     Method      :  RangeCheck (component FLASH_LDD)
**
**     Description :
**         Range check internal routine. This routine is used by the 
**         component's method to test the input address range.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
static LDD_TError RangeCheck(LDD_FLASH_TAddress Address, LDD_FLASH_TDataSize Size)
{
  if ((Size > IntFlashLdd1_PFLASH_SIZE) || (Size == 0U) || (Address > (IntFlashLdd1_PFLASH_SIZE - Size))) {
    return ERR_PARAM_ADDRESS;
  }
  else {
    return ERR_OK;
  }
}

/*
** ===================================================================
**     Method      :  SafeRoutineCaller (component FLASH_LDD)
**
**     Description :
**         Safe routine caller internal routine. The routine is used to 
**         copy the safe routine code to stack a run it.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
static void SafeRoutineCaller(void)
{
  LDD_FLASH_TSafeRoutine SaveRoutineStackSpace;
  LDD_FLASH_TSafeRoutinePtr SafeRoutinePtr;

  /* The safe routine must be aligned to 16bit word */
  SafeRoutinePtr = (LDD_FLASH_TSafeRoutinePtr)(((uint32_t)&SaveRoutineStackSpace + 1U) & ~(uint32_t)1U);
  /* Copy the safe routine's code to a buffer on the stack */
  *(LDD_FLASH_TSafeRoutine *)(void *)SafeRoutinePtr = *(LDD_FLASH_TSafeRoutine *)(void *)&SafeRoutine;
  /* {Default RTOS Adapter} Critical section begin, general PE function is used */
  EnterCritical();
  ((LDD_FLASH_TSafeRoutinePtr)((uint32_t)(SafeRoutinePtr) | 1U))(); /* Run the Safe routine */
  /* {Default RTOS Adapter} Critical section end, general PE function is used */
  ExitCritical();
}

/*
** ===================================================================
**     Method      :  IntFlashLdd1_GetDriverState (component FLASH_LDD)
*/
/*!
**     @brief
**         This method returns the current driver status.
**     @param
**         DeviceDataPtr   - Device data structure
**                           pointer returned by [Init] method.
**     @return
**                         - The current driver status mask.
**                           Following status masks defined in PE_Types.
**                           h can be used to check the current driver
**                           status.
**                           PE_LDD_DRIVER_DISABLED_IN_CLOCK_CONFIGURATIO
**                           N - 1 - Driver is disabled in the current
**                           mode; 0 - Driver is enabled in the current
**                           mode.  
**                           PE_LDD_DRIVER_DISABLED_BY_USER - 1 - Driver
**                           is disabled by the user; 0 - Driver is
**                           enabled by the user.        
**                           PE_LDD_DRIVER_BUSY - 1 - Driver is the BUSY
**                           state; 0 - Driver is in the IDLE state.
*/
/* ===================================================================*/
LDD_TDriverState IntFlashLdd1_GetDriverState(LDD_TDeviceData *DeviceDataPtr)
{
  IntFlashLdd1_TDeviceData *DeviceDataPrv = (IntFlashLdd1_TDeviceData *)DeviceDataPtr;
  LDD_TDriverState DriverState = 0x00U;

  DriverState |= ((DeviceDataPrv->CurrentOperationStatus == LDD_FLASH_RUNNING) || /* Is driver busy? */\
                  (DeviceDataPrv->CurrentOperationStatus == LDD_FLASH_START) || \
                  (DeviceDataPrv->CurrentOperationStatus == LDD_FLASH_STOP_REQ))? PE_LDD_DRIVER_BUSY : 0x00U;
  return DriverState;
}

/* END IntFlashLdd1. */

#ifdef __cplusplus
}  /* extern "C" */
#endif

/*!
** @}
*/
/*
** ###################################################################
**
**     This file was created by Processor Expert 10.4 [05.11]
**     for the Freescale Kinetis series of microcontrollers.
**
** ###################################################################
*/
