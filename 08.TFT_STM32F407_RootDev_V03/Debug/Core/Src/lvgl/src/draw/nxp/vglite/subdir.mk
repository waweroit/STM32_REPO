################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/lvgl/src/draw/nxp/vglite/lv_draw_vglite.c \
../Core/Src/lvgl/src/draw/nxp/vglite/lv_draw_vglite_arc.c \
../Core/Src/lvgl/src/draw/nxp/vglite/lv_draw_vglite_blend.c \
../Core/Src/lvgl/src/draw/nxp/vglite/lv_draw_vglite_line.c \
../Core/Src/lvgl/src/draw/nxp/vglite/lv_draw_vglite_rect.c \
../Core/Src/lvgl/src/draw/nxp/vglite/lv_vglite_buf.c \
../Core/Src/lvgl/src/draw/nxp/vglite/lv_vglite_utils.c 

C_DEPS += \
./Core/Src/lvgl/src/draw/nxp/vglite/lv_draw_vglite.d \
./Core/Src/lvgl/src/draw/nxp/vglite/lv_draw_vglite_arc.d \
./Core/Src/lvgl/src/draw/nxp/vglite/lv_draw_vglite_blend.d \
./Core/Src/lvgl/src/draw/nxp/vglite/lv_draw_vglite_line.d \
./Core/Src/lvgl/src/draw/nxp/vglite/lv_draw_vglite_rect.d \
./Core/Src/lvgl/src/draw/nxp/vglite/lv_vglite_buf.d \
./Core/Src/lvgl/src/draw/nxp/vglite/lv_vglite_utils.d 

OBJS += \
./Core/Src/lvgl/src/draw/nxp/vglite/lv_draw_vglite.o \
./Core/Src/lvgl/src/draw/nxp/vglite/lv_draw_vglite_arc.o \
./Core/Src/lvgl/src/draw/nxp/vglite/lv_draw_vglite_blend.o \
./Core/Src/lvgl/src/draw/nxp/vglite/lv_draw_vglite_line.o \
./Core/Src/lvgl/src/draw/nxp/vglite/lv_draw_vglite_rect.o \
./Core/Src/lvgl/src/draw/nxp/vglite/lv_vglite_buf.o \
./Core/Src/lvgl/src/draw/nxp/vglite/lv_vglite_utils.o 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/lvgl/src/draw/nxp/vglite/%.o Core/Src/lvgl/src/draw/nxp/vglite/%.su Core/Src/lvgl/src/draw/nxp/vglite/%.cyclo: ../Core/Src/lvgl/src/draw/nxp/vglite/%.c Core/Src/lvgl/src/draw/nxp/vglite/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I"C:/Users/wawer/STM32CubeIDE/workspace_1.14.1/08.TFT_STM32F407_RootDev/Core/Src/lvgl" -I"C:/Users/wawer/STM32CubeIDE/workspace_1.14.1/08.TFT_STM32F407_RootDev/Core/Src/lvgl/src" -I"C:/Users/wawer/STM32CubeIDE/workspace_1.14.1/08.TFT_STM32F407_RootDev/Core/Src/lvgl/src/core" -I"C:/Users/wawer/STM32CubeIDE/workspace_1.14.1/08.TFT_STM32F407_RootDev/Core/Src" -I"C:/Users/wawer/STM32CubeIDE/workspace_1.14.1/08.TFT_STM32F407_RootDev/Core/Src/ui" -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../FATFS/Target -I../FATFS/App -I../Middlewares/Third_Party/FatFs/src -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-lvgl-2f-src-2f-draw-2f-nxp-2f-vglite

clean-Core-2f-Src-2f-lvgl-2f-src-2f-draw-2f-nxp-2f-vglite:
	-$(RM) ./Core/Src/lvgl/src/draw/nxp/vglite/lv_draw_vglite.cyclo ./Core/Src/lvgl/src/draw/nxp/vglite/lv_draw_vglite.d ./Core/Src/lvgl/src/draw/nxp/vglite/lv_draw_vglite.o ./Core/Src/lvgl/src/draw/nxp/vglite/lv_draw_vglite.su ./Core/Src/lvgl/src/draw/nxp/vglite/lv_draw_vglite_arc.cyclo ./Core/Src/lvgl/src/draw/nxp/vglite/lv_draw_vglite_arc.d ./Core/Src/lvgl/src/draw/nxp/vglite/lv_draw_vglite_arc.o ./Core/Src/lvgl/src/draw/nxp/vglite/lv_draw_vglite_arc.su ./Core/Src/lvgl/src/draw/nxp/vglite/lv_draw_vglite_blend.cyclo ./Core/Src/lvgl/src/draw/nxp/vglite/lv_draw_vglite_blend.d ./Core/Src/lvgl/src/draw/nxp/vglite/lv_draw_vglite_blend.o ./Core/Src/lvgl/src/draw/nxp/vglite/lv_draw_vglite_blend.su ./Core/Src/lvgl/src/draw/nxp/vglite/lv_draw_vglite_line.cyclo ./Core/Src/lvgl/src/draw/nxp/vglite/lv_draw_vglite_line.d ./Core/Src/lvgl/src/draw/nxp/vglite/lv_draw_vglite_line.o ./Core/Src/lvgl/src/draw/nxp/vglite/lv_draw_vglite_line.su ./Core/Src/lvgl/src/draw/nxp/vglite/lv_draw_vglite_rect.cyclo ./Core/Src/lvgl/src/draw/nxp/vglite/lv_draw_vglite_rect.d ./Core/Src/lvgl/src/draw/nxp/vglite/lv_draw_vglite_rect.o ./Core/Src/lvgl/src/draw/nxp/vglite/lv_draw_vglite_rect.su ./Core/Src/lvgl/src/draw/nxp/vglite/lv_vglite_buf.cyclo ./Core/Src/lvgl/src/draw/nxp/vglite/lv_vglite_buf.d ./Core/Src/lvgl/src/draw/nxp/vglite/lv_vglite_buf.o ./Core/Src/lvgl/src/draw/nxp/vglite/lv_vglite_buf.su ./Core/Src/lvgl/src/draw/nxp/vglite/lv_vglite_utils.cyclo ./Core/Src/lvgl/src/draw/nxp/vglite/lv_vglite_utils.d ./Core/Src/lvgl/src/draw/nxp/vglite/lv_vglite_utils.o ./Core/Src/lvgl/src/draw/nxp/vglite/lv_vglite_utils.su

.PHONY: clean-Core-2f-Src-2f-lvgl-2f-src-2f-draw-2f-nxp-2f-vglite

