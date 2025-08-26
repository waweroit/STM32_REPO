################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/lvgl/src/hal/lv_hal_disp.c \
../Core/Src/lvgl/src/hal/lv_hal_indev.c \
../Core/Src/lvgl/src/hal/lv_hal_tick.c 

C_DEPS += \
./Core/Src/lvgl/src/hal/lv_hal_disp.d \
./Core/Src/lvgl/src/hal/lv_hal_indev.d \
./Core/Src/lvgl/src/hal/lv_hal_tick.d 

OBJS += \
./Core/Src/lvgl/src/hal/lv_hal_disp.o \
./Core/Src/lvgl/src/hal/lv_hal_indev.o \
./Core/Src/lvgl/src/hal/lv_hal_tick.o 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/lvgl/src/hal/%.o Core/Src/lvgl/src/hal/%.su Core/Src/lvgl/src/hal/%.cyclo: ../Core/Src/lvgl/src/hal/%.c Core/Src/lvgl/src/hal/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I"C:/Users/wawer/STM32CubeIDE/workspace_1.14.1/08.TFT_STM32F407_RootDev/Core/Src/lvgl" -I"C:/Users/wawer/STM32CubeIDE/workspace_1.14.1/08.TFT_STM32F407_RootDev/Core/Src/lvgl/src" -I"C:/Users/wawer/STM32CubeIDE/workspace_1.14.1/08.TFT_STM32F407_RootDev/Core/Src/lvgl/src/core" -I"C:/Users/wawer/STM32CubeIDE/workspace_1.14.1/08.TFT_STM32F407_RootDev/Core/Src" -I"C:/Users/wawer/STM32CubeIDE/workspace_1.14.1/08.TFT_STM32F407_RootDev/Core/Src/ui" -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../FATFS/Target -I../FATFS/App -I../Middlewares/Third_Party/FatFs/src -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-lvgl-2f-src-2f-hal

clean-Core-2f-Src-2f-lvgl-2f-src-2f-hal:
	-$(RM) ./Core/Src/lvgl/src/hal/lv_hal_disp.cyclo ./Core/Src/lvgl/src/hal/lv_hal_disp.d ./Core/Src/lvgl/src/hal/lv_hal_disp.o ./Core/Src/lvgl/src/hal/lv_hal_disp.su ./Core/Src/lvgl/src/hal/lv_hal_indev.cyclo ./Core/Src/lvgl/src/hal/lv_hal_indev.d ./Core/Src/lvgl/src/hal/lv_hal_indev.o ./Core/Src/lvgl/src/hal/lv_hal_indev.su ./Core/Src/lvgl/src/hal/lv_hal_tick.cyclo ./Core/Src/lvgl/src/hal/lv_hal_tick.d ./Core/Src/lvgl/src/hal/lv_hal_tick.o ./Core/Src/lvgl/src/hal/lv_hal_tick.su

.PHONY: clean-Core-2f-Src-2f-lvgl-2f-src-2f-hal

