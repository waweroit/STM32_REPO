################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/lvgl/src/draw/sdl/lv_draw_sdl.c \
../Core/Src/lvgl/src/draw/sdl/lv_draw_sdl_arc.c \
../Core/Src/lvgl/src/draw/sdl/lv_draw_sdl_bg.c \
../Core/Src/lvgl/src/draw/sdl/lv_draw_sdl_composite.c \
../Core/Src/lvgl/src/draw/sdl/lv_draw_sdl_img.c \
../Core/Src/lvgl/src/draw/sdl/lv_draw_sdl_label.c \
../Core/Src/lvgl/src/draw/sdl/lv_draw_sdl_layer.c \
../Core/Src/lvgl/src/draw/sdl/lv_draw_sdl_line.c \
../Core/Src/lvgl/src/draw/sdl/lv_draw_sdl_mask.c \
../Core/Src/lvgl/src/draw/sdl/lv_draw_sdl_polygon.c \
../Core/Src/lvgl/src/draw/sdl/lv_draw_sdl_rect.c \
../Core/Src/lvgl/src/draw/sdl/lv_draw_sdl_stack_blur.c \
../Core/Src/lvgl/src/draw/sdl/lv_draw_sdl_texture_cache.c \
../Core/Src/lvgl/src/draw/sdl/lv_draw_sdl_utils.c 

C_DEPS += \
./Core/Src/lvgl/src/draw/sdl/lv_draw_sdl.d \
./Core/Src/lvgl/src/draw/sdl/lv_draw_sdl_arc.d \
./Core/Src/lvgl/src/draw/sdl/lv_draw_sdl_bg.d \
./Core/Src/lvgl/src/draw/sdl/lv_draw_sdl_composite.d \
./Core/Src/lvgl/src/draw/sdl/lv_draw_sdl_img.d \
./Core/Src/lvgl/src/draw/sdl/lv_draw_sdl_label.d \
./Core/Src/lvgl/src/draw/sdl/lv_draw_sdl_layer.d \
./Core/Src/lvgl/src/draw/sdl/lv_draw_sdl_line.d \
./Core/Src/lvgl/src/draw/sdl/lv_draw_sdl_mask.d \
./Core/Src/lvgl/src/draw/sdl/lv_draw_sdl_polygon.d \
./Core/Src/lvgl/src/draw/sdl/lv_draw_sdl_rect.d \
./Core/Src/lvgl/src/draw/sdl/lv_draw_sdl_stack_blur.d \
./Core/Src/lvgl/src/draw/sdl/lv_draw_sdl_texture_cache.d \
./Core/Src/lvgl/src/draw/sdl/lv_draw_sdl_utils.d 

OBJS += \
./Core/Src/lvgl/src/draw/sdl/lv_draw_sdl.o \
./Core/Src/lvgl/src/draw/sdl/lv_draw_sdl_arc.o \
./Core/Src/lvgl/src/draw/sdl/lv_draw_sdl_bg.o \
./Core/Src/lvgl/src/draw/sdl/lv_draw_sdl_composite.o \
./Core/Src/lvgl/src/draw/sdl/lv_draw_sdl_img.o \
./Core/Src/lvgl/src/draw/sdl/lv_draw_sdl_label.o \
./Core/Src/lvgl/src/draw/sdl/lv_draw_sdl_layer.o \
./Core/Src/lvgl/src/draw/sdl/lv_draw_sdl_line.o \
./Core/Src/lvgl/src/draw/sdl/lv_draw_sdl_mask.o \
./Core/Src/lvgl/src/draw/sdl/lv_draw_sdl_polygon.o \
./Core/Src/lvgl/src/draw/sdl/lv_draw_sdl_rect.o \
./Core/Src/lvgl/src/draw/sdl/lv_draw_sdl_stack_blur.o \
./Core/Src/lvgl/src/draw/sdl/lv_draw_sdl_texture_cache.o \
./Core/Src/lvgl/src/draw/sdl/lv_draw_sdl_utils.o 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/lvgl/src/draw/sdl/%.o Core/Src/lvgl/src/draw/sdl/%.su Core/Src/lvgl/src/draw/sdl/%.cyclo: ../Core/Src/lvgl/src/draw/sdl/%.c Core/Src/lvgl/src/draw/sdl/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I"C:/Users/wawer/STM32CubeIDE/workspace_1.14.1/08.TFT_STM32F407_RootDev/Core/Src/lvgl" -I"C:/Users/wawer/STM32CubeIDE/workspace_1.14.1/08.TFT_STM32F407_RootDev/Core/Src/lvgl/src" -I"C:/Users/wawer/STM32CubeIDE/workspace_1.14.1/08.TFT_STM32F407_RootDev/Core/Src/lvgl/src/core" -I"C:/Users/wawer/STM32CubeIDE/workspace_1.14.1/08.TFT_STM32F407_RootDev/Core/Src" -I"C:/Users/wawer/STM32CubeIDE/workspace_1.14.1/08.TFT_STM32F407_RootDev/Core/Src/ui" -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../FATFS/Target -I../FATFS/App -I../Middlewares/Third_Party/FatFs/src -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-lvgl-2f-src-2f-draw-2f-sdl

clean-Core-2f-Src-2f-lvgl-2f-src-2f-draw-2f-sdl:
	-$(RM) ./Core/Src/lvgl/src/draw/sdl/lv_draw_sdl.cyclo ./Core/Src/lvgl/src/draw/sdl/lv_draw_sdl.d ./Core/Src/lvgl/src/draw/sdl/lv_draw_sdl.o ./Core/Src/lvgl/src/draw/sdl/lv_draw_sdl.su ./Core/Src/lvgl/src/draw/sdl/lv_draw_sdl_arc.cyclo ./Core/Src/lvgl/src/draw/sdl/lv_draw_sdl_arc.d ./Core/Src/lvgl/src/draw/sdl/lv_draw_sdl_arc.o ./Core/Src/lvgl/src/draw/sdl/lv_draw_sdl_arc.su ./Core/Src/lvgl/src/draw/sdl/lv_draw_sdl_bg.cyclo ./Core/Src/lvgl/src/draw/sdl/lv_draw_sdl_bg.d ./Core/Src/lvgl/src/draw/sdl/lv_draw_sdl_bg.o ./Core/Src/lvgl/src/draw/sdl/lv_draw_sdl_bg.su ./Core/Src/lvgl/src/draw/sdl/lv_draw_sdl_composite.cyclo ./Core/Src/lvgl/src/draw/sdl/lv_draw_sdl_composite.d ./Core/Src/lvgl/src/draw/sdl/lv_draw_sdl_composite.o ./Core/Src/lvgl/src/draw/sdl/lv_draw_sdl_composite.su ./Core/Src/lvgl/src/draw/sdl/lv_draw_sdl_img.cyclo ./Core/Src/lvgl/src/draw/sdl/lv_draw_sdl_img.d ./Core/Src/lvgl/src/draw/sdl/lv_draw_sdl_img.o ./Core/Src/lvgl/src/draw/sdl/lv_draw_sdl_img.su ./Core/Src/lvgl/src/draw/sdl/lv_draw_sdl_label.cyclo ./Core/Src/lvgl/src/draw/sdl/lv_draw_sdl_label.d ./Core/Src/lvgl/src/draw/sdl/lv_draw_sdl_label.o ./Core/Src/lvgl/src/draw/sdl/lv_draw_sdl_label.su ./Core/Src/lvgl/src/draw/sdl/lv_draw_sdl_layer.cyclo ./Core/Src/lvgl/src/draw/sdl/lv_draw_sdl_layer.d ./Core/Src/lvgl/src/draw/sdl/lv_draw_sdl_layer.o ./Core/Src/lvgl/src/draw/sdl/lv_draw_sdl_layer.su ./Core/Src/lvgl/src/draw/sdl/lv_draw_sdl_line.cyclo ./Core/Src/lvgl/src/draw/sdl/lv_draw_sdl_line.d ./Core/Src/lvgl/src/draw/sdl/lv_draw_sdl_line.o ./Core/Src/lvgl/src/draw/sdl/lv_draw_sdl_line.su ./Core/Src/lvgl/src/draw/sdl/lv_draw_sdl_mask.cyclo ./Core/Src/lvgl/src/draw/sdl/lv_draw_sdl_mask.d ./Core/Src/lvgl/src/draw/sdl/lv_draw_sdl_mask.o ./Core/Src/lvgl/src/draw/sdl/lv_draw_sdl_mask.su ./Core/Src/lvgl/src/draw/sdl/lv_draw_sdl_polygon.cyclo ./Core/Src/lvgl/src/draw/sdl/lv_draw_sdl_polygon.d ./Core/Src/lvgl/src/draw/sdl/lv_draw_sdl_polygon.o ./Core/Src/lvgl/src/draw/sdl/lv_draw_sdl_polygon.su ./Core/Src/lvgl/src/draw/sdl/lv_draw_sdl_rect.cyclo ./Core/Src/lvgl/src/draw/sdl/lv_draw_sdl_rect.d ./Core/Src/lvgl/src/draw/sdl/lv_draw_sdl_rect.o ./Core/Src/lvgl/src/draw/sdl/lv_draw_sdl_rect.su ./Core/Src/lvgl/src/draw/sdl/lv_draw_sdl_stack_blur.cyclo ./Core/Src/lvgl/src/draw/sdl/lv_draw_sdl_stack_blur.d ./Core/Src/lvgl/src/draw/sdl/lv_draw_sdl_stack_blur.o ./Core/Src/lvgl/src/draw/sdl/lv_draw_sdl_stack_blur.su ./Core/Src/lvgl/src/draw/sdl/lv_draw_sdl_texture_cache.cyclo ./Core/Src/lvgl/src/draw/sdl/lv_draw_sdl_texture_cache.d ./Core/Src/lvgl/src/draw/sdl/lv_draw_sdl_texture_cache.o ./Core/Src/lvgl/src/draw/sdl/lv_draw_sdl_texture_cache.su ./Core/Src/lvgl/src/draw/sdl/lv_draw_sdl_utils.cyclo ./Core/Src/lvgl/src/draw/sdl/lv_draw_sdl_utils.d ./Core/Src/lvgl/src/draw/sdl/lv_draw_sdl_utils.o ./Core/Src/lvgl/src/draw/sdl/lv_draw_sdl_utils.su

.PHONY: clean-Core-2f-Src-2f-lvgl-2f-src-2f-draw-2f-sdl

