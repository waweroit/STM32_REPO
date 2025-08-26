################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/BMP280.c \
../Core/Src/CommunicationMessages.c \
../Core/Src/DataTime.c \
../Core/Src/ESP8266_WiFi.c \
../Core/Src/File_Handling.c \
../Core/Src/INI_FileHandling.c \
../Core/Src/LoRa.c \
../Core/Src/SPITouchScreen.c \
../Core/Src/cJSON.c \
../Core/Src/font12.c \
../Core/Src/font16.c \
../Core/Src/font20.c \
../Core/Src/font24.c \
../Core/Src/font8.c \
../Core/Src/fsmc.c \
../Core/Src/gpio.c \
../Core/Src/i2c.c \
../Core/Src/ili9341.c \
../Core/Src/lvgl_Controller.c \
../Core/Src/main.c \
../Core/Src/sdio.c \
../Core/Src/spi.c \
../Core/Src/stm32f4xx_hal_msp.c \
../Core/Src/stm32f4xx_it.c \
../Core/Src/syscalls.c \
../Core/Src/sysmem.c \
../Core/Src/system_stm32f4xx.c \
../Core/Src/tim.c \
../Core/Src/usart.c \
../Core/Src/waweroBasicFunction.c 

C_DEPS += \
./Core/Src/BMP280.d \
./Core/Src/CommunicationMessages.d \
./Core/Src/DataTime.d \
./Core/Src/ESP8266_WiFi.d \
./Core/Src/File_Handling.d \
./Core/Src/INI_FileHandling.d \
./Core/Src/LoRa.d \
./Core/Src/SPITouchScreen.d \
./Core/Src/cJSON.d \
./Core/Src/font12.d \
./Core/Src/font16.d \
./Core/Src/font20.d \
./Core/Src/font24.d \
./Core/Src/font8.d \
./Core/Src/fsmc.d \
./Core/Src/gpio.d \
./Core/Src/i2c.d \
./Core/Src/ili9341.d \
./Core/Src/lvgl_Controller.d \
./Core/Src/main.d \
./Core/Src/sdio.d \
./Core/Src/spi.d \
./Core/Src/stm32f4xx_hal_msp.d \
./Core/Src/stm32f4xx_it.d \
./Core/Src/syscalls.d \
./Core/Src/sysmem.d \
./Core/Src/system_stm32f4xx.d \
./Core/Src/tim.d \
./Core/Src/usart.d \
./Core/Src/waweroBasicFunction.d 

OBJS += \
./Core/Src/BMP280.o \
./Core/Src/CommunicationMessages.o \
./Core/Src/DataTime.o \
./Core/Src/ESP8266_WiFi.o \
./Core/Src/File_Handling.o \
./Core/Src/INI_FileHandling.o \
./Core/Src/LoRa.o \
./Core/Src/SPITouchScreen.o \
./Core/Src/cJSON.o \
./Core/Src/font12.o \
./Core/Src/font16.o \
./Core/Src/font20.o \
./Core/Src/font24.o \
./Core/Src/font8.o \
./Core/Src/fsmc.o \
./Core/Src/gpio.o \
./Core/Src/i2c.o \
./Core/Src/ili9341.o \
./Core/Src/lvgl_Controller.o \
./Core/Src/main.o \
./Core/Src/sdio.o \
./Core/Src/spi.o \
./Core/Src/stm32f4xx_hal_msp.o \
./Core/Src/stm32f4xx_it.o \
./Core/Src/syscalls.o \
./Core/Src/sysmem.o \
./Core/Src/system_stm32f4xx.o \
./Core/Src/tim.o \
./Core/Src/usart.o \
./Core/Src/waweroBasicFunction.o 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o Core/Src/%.su Core/Src/%.cyclo: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I"C:/Users/wawer/STM32CubeIDE/workspace_1.14.1/08.TFT_STM32F407_RootDev/Core/Src/lvgl" -I"C:/Users/wawer/STM32CubeIDE/workspace_1.14.1/08.TFT_STM32F407_RootDev/Core/Src/lvgl/src" -I"C:/Users/wawer/STM32CubeIDE/workspace_1.14.1/08.TFT_STM32F407_RootDev/Core/Src/lvgl/src/core" -I"C:/Users/wawer/STM32CubeIDE/workspace_1.14.1/08.TFT_STM32F407_RootDev/Core/Src" -I"C:/Users/wawer/STM32CubeIDE/workspace_1.14.1/08.TFT_STM32F407_RootDev/Core/Src/ui" -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../FATFS/Target -I../FATFS/App -I../Middlewares/Third_Party/FatFs/src -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/BMP280.cyclo ./Core/Src/BMP280.d ./Core/Src/BMP280.o ./Core/Src/BMP280.su ./Core/Src/CommunicationMessages.cyclo ./Core/Src/CommunicationMessages.d ./Core/Src/CommunicationMessages.o ./Core/Src/CommunicationMessages.su ./Core/Src/DataTime.cyclo ./Core/Src/DataTime.d ./Core/Src/DataTime.o ./Core/Src/DataTime.su ./Core/Src/ESP8266_WiFi.cyclo ./Core/Src/ESP8266_WiFi.d ./Core/Src/ESP8266_WiFi.o ./Core/Src/ESP8266_WiFi.su ./Core/Src/File_Handling.cyclo ./Core/Src/File_Handling.d ./Core/Src/File_Handling.o ./Core/Src/File_Handling.su ./Core/Src/INI_FileHandling.cyclo ./Core/Src/INI_FileHandling.d ./Core/Src/INI_FileHandling.o ./Core/Src/INI_FileHandling.su ./Core/Src/LoRa.cyclo ./Core/Src/LoRa.d ./Core/Src/LoRa.o ./Core/Src/LoRa.su ./Core/Src/SPITouchScreen.cyclo ./Core/Src/SPITouchScreen.d ./Core/Src/SPITouchScreen.o ./Core/Src/SPITouchScreen.su ./Core/Src/cJSON.cyclo ./Core/Src/cJSON.d ./Core/Src/cJSON.o ./Core/Src/cJSON.su ./Core/Src/font12.cyclo ./Core/Src/font12.d ./Core/Src/font12.o ./Core/Src/font12.su ./Core/Src/font16.cyclo ./Core/Src/font16.d ./Core/Src/font16.o ./Core/Src/font16.su ./Core/Src/font20.cyclo ./Core/Src/font20.d ./Core/Src/font20.o ./Core/Src/font20.su ./Core/Src/font24.cyclo ./Core/Src/font24.d ./Core/Src/font24.o ./Core/Src/font24.su ./Core/Src/font8.cyclo ./Core/Src/font8.d ./Core/Src/font8.o ./Core/Src/font8.su ./Core/Src/fsmc.cyclo ./Core/Src/fsmc.d ./Core/Src/fsmc.o ./Core/Src/fsmc.su ./Core/Src/gpio.cyclo ./Core/Src/gpio.d ./Core/Src/gpio.o ./Core/Src/gpio.su ./Core/Src/i2c.cyclo ./Core/Src/i2c.d ./Core/Src/i2c.o ./Core/Src/i2c.su ./Core/Src/ili9341.cyclo ./Core/Src/ili9341.d ./Core/Src/ili9341.o ./Core/Src/ili9341.su ./Core/Src/lvgl_Controller.cyclo ./Core/Src/lvgl_Controller.d ./Core/Src/lvgl_Controller.o ./Core/Src/lvgl_Controller.su ./Core/Src/main.cyclo ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/main.su ./Core/Src/sdio.cyclo ./Core/Src/sdio.d ./Core/Src/sdio.o ./Core/Src/sdio.su ./Core/Src/spi.cyclo ./Core/Src/spi.d ./Core/Src/spi.o ./Core/Src/spi.su ./Core/Src/stm32f4xx_hal_msp.cyclo ./Core/Src/stm32f4xx_hal_msp.d ./Core/Src/stm32f4xx_hal_msp.o ./Core/Src/stm32f4xx_hal_msp.su ./Core/Src/stm32f4xx_it.cyclo ./Core/Src/stm32f4xx_it.d ./Core/Src/stm32f4xx_it.o ./Core/Src/stm32f4xx_it.su ./Core/Src/syscalls.cyclo ./Core/Src/syscalls.d ./Core/Src/syscalls.o ./Core/Src/syscalls.su ./Core/Src/sysmem.cyclo ./Core/Src/sysmem.d ./Core/Src/sysmem.o ./Core/Src/sysmem.su ./Core/Src/system_stm32f4xx.cyclo ./Core/Src/system_stm32f4xx.d ./Core/Src/system_stm32f4xx.o ./Core/Src/system_stm32f4xx.su ./Core/Src/tim.cyclo ./Core/Src/tim.d ./Core/Src/tim.o ./Core/Src/tim.su ./Core/Src/usart.cyclo ./Core/Src/usart.d ./Core/Src/usart.o ./Core/Src/usart.su ./Core/Src/waweroBasicFunction.cyclo ./Core/Src/waweroBasicFunction.d ./Core/Src/waweroBasicFunction.o ./Core/Src/waweroBasicFunction.su

.PHONY: clean-Core-2f-Src

