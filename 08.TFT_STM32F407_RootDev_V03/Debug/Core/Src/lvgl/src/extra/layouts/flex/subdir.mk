################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/lvgl/src/extra/layouts/flex/lv_flex.c 

C_DEPS += \
./Core/Src/lvgl/src/extra/layouts/flex/lv_flex.d 

OBJS += \
./Core/Src/lvgl/src/extra/layouts/flex/lv_flex.o 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/lvgl/src/extra/layouts/flex/%.o Core/Src/lvgl/src/extra/layouts/flex/%.su Core/Src/lvgl/src/extra/layouts/flex/%.cyclo: ../Core/Src/lvgl/src/extra/layouts/flex/%.c Core/Src/lvgl/src/extra/layouts/flex/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I"C:/Users/wawer/STM32CubeIDE/workspace_1.14.1/08.TFT_STM32F407_RootDev/Core/Src/lvgl" -I"C:/Users/wawer/STM32CubeIDE/workspace_1.14.1/08.TFT_STM32F407_RootDev/Core/Src/lvgl/src" -I"C:/Users/wawer/STM32CubeIDE/workspace_1.14.1/08.TFT_STM32F407_RootDev/Core/Src/lvgl/src/core" -I"C:/Users/wawer/STM32CubeIDE/workspace_1.14.1/08.TFT_STM32F407_RootDev/Core/Src" -I"C:/Users/wawer/STM32CubeIDE/workspace_1.14.1/08.TFT_STM32F407_RootDev/Core/Src/ui" -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../FATFS/Target -I../FATFS/App -I../Middlewares/Third_Party/FatFs/src -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-lvgl-2f-src-2f-extra-2f-layouts-2f-flex

clean-Core-2f-Src-2f-lvgl-2f-src-2f-extra-2f-layouts-2f-flex:
	-$(RM) ./Core/Src/lvgl/src/extra/layouts/flex/lv_flex.cyclo ./Core/Src/lvgl/src/extra/layouts/flex/lv_flex.d ./Core/Src/lvgl/src/extra/layouts/flex/lv_flex.o ./Core/Src/lvgl/src/extra/layouts/flex/lv_flex.su

.PHONY: clean-Core-2f-Src-2f-lvgl-2f-src-2f-extra-2f-layouts-2f-flex

