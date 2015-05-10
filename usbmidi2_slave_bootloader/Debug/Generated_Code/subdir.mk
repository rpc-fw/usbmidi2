################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Generated_Code/ASerialLdd1.c \
../Generated_Code/BitIoLdd1.c \
../Generated_Code/COREUART.c \
../Generated_Code/Cpu.c \
../Generated_Code/IFsh1.c \
../Generated_Code/IntFlashLdd1.c \
../Generated_Code/LED1.c \
../Generated_Code/LEDpin1.c \
../Generated_Code/PE_LDD.c \
../Generated_Code/PTC.c \
../Generated_Code/PTD.c \
../Generated_Code/PTE.c \
../Generated_Code/Vectors.c \
../Generated_Code/WAIT1.c 

OBJS += \
./Generated_Code/ASerialLdd1.o \
./Generated_Code/BitIoLdd1.o \
./Generated_Code/COREUART.o \
./Generated_Code/Cpu.o \
./Generated_Code/IFsh1.o \
./Generated_Code/IntFlashLdd1.o \
./Generated_Code/LED1.o \
./Generated_Code/LEDpin1.o \
./Generated_Code/PE_LDD.o \
./Generated_Code/PTC.o \
./Generated_Code/PTD.o \
./Generated_Code/PTE.o \
./Generated_Code/Vectors.o \
./Generated_Code/WAIT1.o 

C_DEPS += \
./Generated_Code/ASerialLdd1.d \
./Generated_Code/BitIoLdd1.d \
./Generated_Code/COREUART.d \
./Generated_Code/Cpu.d \
./Generated_Code/IFsh1.d \
./Generated_Code/IntFlashLdd1.d \
./Generated_Code/LED1.d \
./Generated_Code/LEDpin1.d \
./Generated_Code/PE_LDD.d \
./Generated_Code/PTC.d \
./Generated_Code/PTD.d \
./Generated_Code/PTE.d \
./Generated_Code/Vectors.d \
./Generated_Code/WAIT1.d 


# Each subdirectory must supply rules for building sources it contributes
Generated_Code/%.o: ../Generated_Code/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m0plus -mthumb -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g3 -DKDS -Dasm=__asm -DMCU_MKL25Z4 -DCPU_MKL26Z128VFM4 -I"/home/ld0d/work/kds_workspace/usbmidi2_slave_bootloader/Static_Code/PDD" -I"/home/ld0d/work/kds_workspace/usbmidilib/Sources" -I"/home/ld0d/work/kds_workspace/usbmidilib/Sources/usb" -I"/home/ld0d/work/kds_workspace/usbmidi2_slave_bootloader/Static_Code/IO_Map" -I"/home/ld0d/work/kds_workspace/usbmidi2_slave_bootloader/Sources" -I"/home/ld0d/work/kds_workspace/usbmidi2_slave_bootloader/Generated_Code" -std=c99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


