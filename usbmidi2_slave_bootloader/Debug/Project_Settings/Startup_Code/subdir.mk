################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Project_Settings/Startup_Code/startup.c 

OBJS += \
./Project_Settings/Startup_Code/startup.o 

C_DEPS += \
./Project_Settings/Startup_Code/startup.d 


# Each subdirectory must supply rules for building sources it contributes
Project_Settings/Startup_Code/%.o: ../Project_Settings/Startup_Code/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m0plus -mthumb -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g3 -DKDS -Dasm=__asm -DMCU_MKL25Z4 -DCPU_MKL26Z128VFM4 -I"/home/ld0d/work/kds_workspace/usbmidi2_slave_bootloader/Static_Code/PDD" -I"/home/ld0d/work/kds_workspace/usbmidilib/Sources" -I"/home/ld0d/work/kds_workspace/usbmidilib/Sources/usb" -I"/home/ld0d/work/kds_workspace/usbmidi2_slave_bootloader/Static_Code/IO_Map" -I"/home/ld0d/work/kds_workspace/usbmidi2_slave_bootloader/Sources" -I"/home/ld0d/work/kds_workspace/usbmidi2_slave_bootloader/Generated_Code" -std=c99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


