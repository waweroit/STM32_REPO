################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/lvgl/src/misc/lv_anim.c \
../Core/Src/lvgl/src/misc/lv_anim_timeline.c \
../Core/Src/lvgl/src/misc/lv_area.c \
../Core/Src/lvgl/src/misc/lv_async.c \
../Core/Src/lvgl/src/misc/lv_bidi.c \
../Core/Src/lvgl/src/misc/lv_color.c \
../Core/Src/lvgl/src/misc/lv_fs.c \
../Core/Src/lvgl/src/misc/lv_gc.c \
../Core/Src/lvgl/src/misc/lv_ll.c \
../Core/Src/lvgl/src/misc/lv_log.c \
../Core/Src/lvgl/src/misc/lv_lru.c \
../Core/Src/lvgl/src/misc/lv_math.c \
../Core/Src/lvgl/src/misc/lv_mem.c \
../Core/Src/lvgl/src/misc/lv_printf.c \
../Core/Src/lvgl/src/misc/lv_style.c \
../Core/Src/lvgl/src/misc/lv_style_gen.c \
../Core/Src/lvgl/src/misc/lv_templ.c \
../Core/Src/lvgl/src/misc/lv_timer.c \
../Core/Src/lvgl/src/misc/lv_tlsf.c \
../Core/Src/lvgl/src/misc/lv_txt.c \
../Core/Src/lvgl/src/misc/lv_txt_ap.c \
../Core/Src/lvgl/src/misc/lv_utils.c 

C_DEPS += \
./Core/Src/lvgl/src/misc/lv_anim.d \
./Core/Src/lvgl/src/misc/lv_anim_timeline.d \
./Core/Src/lvgl/src/misc/lv_area.d \
./Core/Src/lvgl/src/misc/lv_async.d \
./Core/Src/lvgl/src/misc/lv_bidi.d \
./Core/Src/lvgl/src/misc/lv_color.d \
./Core/Src/lvgl/src/misc/lv_fs.d \
./Core/Src/lvgl/src/misc/lv_gc.d \
./Core/Src/lvgl/src/misc/lv_ll.d \
./Core/Src/lvgl/src/misc/lv_log.d \
./Core/Src/lvgl/src/misc/lv_lru.d \
./Core/Src/lvgl/src/misc/lv_math.d \
./Core/Src/lvgl/src/misc/lv_mem.d \
./Core/Src/lvgl/src/misc/lv_printf.d \
./Core/Src/lvgl/src/misc/lv_style.d \
./Core/Src/lvgl/src/misc/lv_style_gen.d \
./Core/Src/lvgl/src/misc/lv_templ.d \
./Core/Src/lvgl/src/misc/lv_timer.d \
./Core/Src/lvgl/src/misc/lv_tlsf.d \
./Core/Src/lvgl/src/misc/lv_txt.d \
./Core/Src/lvgl/src/misc/lv_txt_ap.d \
./Core/Src/lvgl/src/misc/lv_utils.d 

OBJS += \
./Core/Src/lvgl/src/misc/lv_anim.o \
./Core/Src/lvgl/src/misc/lv_anim_timeline.o \
./Core/Src/lvgl/src/misc/lv_area.o \
./Core/Src/lvgl/src/misc/lv_async.o \
./Core/Src/lvgl/src/misc/lv_bidi.o \
./Core/Src/lvgl/src/misc/lv_color.o \
./Core/Src/lvgl/src/misc/lv_fs.o \
./Core/Src/lvgl/src/misc/lv_gc.o \
./Core/Src/lvgl/src/misc/lv_ll.o \
./Core/Src/lvgl/src/misc/lv_log.o \
./Core/Src/lvgl/src/misc/lv_lru.o \
./Core/Src/lvgl/src/misc/lv_math.o \
./Core/Src/lvgl/src/misc/lv_mem.o \
./Core/Src/lvgl/src/misc/lv_printf.o \
./Core/Src/lvgl/src/misc/lv_style.o \
./Core/Src/lvgl/src/misc/lv_style_gen.o \
./Core/Src/lvgl/src/misc/lv_templ.o \
./Core/Src/lvgl/src/misc/lv_timer.o \
./Core/Src/lvgl/src/misc/lv_tlsf.o \
./Core/Src/lvgl/src/misc/lv_txt.o \
./Core/Src/lvgl/src/misc/lv_txt_ap.o \
./Core/Src/lvgl/src/misc/lv_utils.o 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/lvgl/src/misc/%.o Core/Src/lvgl/src/misc/%.su Core/Src/lvgl/src/misc/%.cyclo: ../Core/Src/lvgl/src/misc/%.c Core/Src/lvgl/src/misc/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I"C:/Users/wawer/STM32CubeIDE/workspace_1.14.1/08.TFT_STM32F407_RootDev/Core/Src/lvgl" -I"C:/Users/wawer/STM32CubeIDE/workspace_1.14.1/08.TFT_STM32F407_RootDev/Core/Src/lvgl/src" -I"C:/Users/wawer/STM32CubeIDE/workspace_1.14.1/08.TFT_STM32F407_RootDev/Core/Src/lvgl/src/core" -I"C:/Users/wawer/STM32CubeIDE/workspace_1.14.1/08.TFT_STM32F407_RootDev/Core/Src" -I"C:/Users/wawer/STM32CubeIDE/workspace_1.14.1/08.TFT_STM32F407_RootDev/Core/Src/ui" -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../FATFS/Target -I../FATFS/App -I../Middlewares/Third_Party/FatFs/src -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-lvgl-2f-src-2f-misc

clean-Core-2f-Src-2f-lvgl-2f-src-2f-misc:
	-$(RM) ./Core/Src/lvgl/src/misc/lv_anim.cyclo ./Core/Src/lvgl/src/misc/lv_anim.d ./Core/Src/lvgl/src/misc/lv_anim.o ./Core/Src/lvgl/src/misc/lv_anim.su ./Core/Src/lvgl/src/misc/lv_anim_timeline.cyclo ./Core/Src/lvgl/src/misc/lv_anim_timeline.d ./Core/Src/lvgl/src/misc/lv_anim_timeline.o ./Core/Src/lvgl/src/misc/lv_anim_timeline.su ./Core/Src/lvgl/src/misc/lv_area.cyclo ./Core/Src/lvgl/src/misc/lv_area.d ./Core/Src/lvgl/src/misc/lv_area.o ./Core/Src/lvgl/src/misc/lv_area.su ./Core/Src/lvgl/src/misc/lv_async.cyclo ./Core/Src/lvgl/src/misc/lv_async.d ./Core/Src/lvgl/src/misc/lv_async.o ./Core/Src/lvgl/src/misc/lv_async.su ./Core/Src/lvgl/src/misc/lv_bidi.cyclo ./Core/Src/lvgl/src/misc/lv_bidi.d ./Core/Src/lvgl/src/misc/lv_bidi.o ./Core/Src/lvgl/src/misc/lv_bidi.su ./Core/Src/lvgl/src/misc/lv_color.cyclo ./Core/Src/lvgl/src/misc/lv_color.d ./Core/Src/lvgl/src/misc/lv_color.o ./Core/Src/lvgl/src/misc/lv_color.su ./Core/Src/lvgl/src/misc/lv_fs.cyclo ./Core/Src/lvgl/src/misc/lv_fs.d ./Core/Src/lvgl/src/misc/lv_fs.o ./Core/Src/lvgl/src/misc/lv_fs.su ./Core/Src/lvgl/src/misc/lv_gc.cyclo ./Core/Src/lvgl/src/misc/lv_gc.d ./Core/Src/lvgl/src/misc/lv_gc.o ./Core/Src/lvgl/src/misc/lv_gc.su ./Core/Src/lvgl/src/misc/lv_ll.cyclo ./Core/Src/lvgl/src/misc/lv_ll.d ./Core/Src/lvgl/src/misc/lv_ll.o ./Core/Src/lvgl/src/misc/lv_ll.su ./Core/Src/lvgl/src/misc/lv_log.cyclo ./Core/Src/lvgl/src/misc/lv_log.d ./Core/Src/lvgl/src/misc/lv_log.o ./Core/Src/lvgl/src/misc/lv_log.su ./Core/Src/lvgl/src/misc/lv_lru.cyclo ./Core/Src/lvgl/src/misc/lv_lru.d ./Core/Src/lvgl/src/misc/lv_lru.o ./Core/Src/lvgl/src/misc/lv_lru.su ./Core/Src/lvgl/src/misc/lv_math.cyclo ./Core/Src/lvgl/src/misc/lv_math.d ./Core/Src/lvgl/src/misc/lv_math.o ./Core/Src/lvgl/src/misc/lv_math.su ./Core/Src/lvgl/src/misc/lv_mem.cyclo ./Core/Src/lvgl/src/misc/lv_mem.d ./Core/Src/lvgl/src/misc/lv_mem.o ./Core/Src/lvgl/src/misc/lv_mem.su ./Core/Src/lvgl/src/misc/lv_printf.cyclo ./Core/Src/lvgl/src/misc/lv_printf.d ./Core/Src/lvgl/src/misc/lv_printf.o ./Core/Src/lvgl/src/misc/lv_printf.su ./Core/Src/lvgl/src/misc/lv_style.cyclo ./Core/Src/lvgl/src/misc/lv_style.d ./Core/Src/lvgl/src/misc/lv_style.o ./Core/Src/lvgl/src/misc/lv_style.su ./Core/Src/lvgl/src/misc/lv_style_gen.cyclo ./Core/Src/lvgl/src/misc/lv_style_gen.d ./Core/Src/lvgl/src/misc/lv_style_gen.o ./Core/Src/lvgl/src/misc/lv_style_gen.su ./Core/Src/lvgl/src/misc/lv_templ.cyclo ./Core/Src/lvgl/src/misc/lv_templ.d ./Core/Src/lvgl/src/misc/lv_templ.o ./Core/Src/lvgl/src/misc/lv_templ.su ./Core/Src/lvgl/src/misc/lv_timer.cyclo ./Core/Src/lvgl/src/misc/lv_timer.d ./Core/Src/lvgl/src/misc/lv_timer.o ./Core/Src/lvgl/src/misc/lv_timer.su ./Core/Src/lvgl/src/misc/lv_tlsf.cyclo ./Core/Src/lvgl/src/misc/lv_tlsf.d ./Core/Src/lvgl/src/misc/lv_tlsf.o ./Core/Src/lvgl/src/misc/lv_tlsf.su ./Core/Src/lvgl/src/misc/lv_txt.cyclo ./Core/Src/lvgl/src/misc/lv_txt.d ./Core/Src/lvgl/src/misc/lv_txt.o ./Core/Src/lvgl/src/misc/lv_txt.su ./Core/Src/lvgl/src/misc/lv_txt_ap.cyclo ./Core/Src/lvgl/src/misc/lv_txt_ap.d ./Core/Src/lvgl/src/misc/lv_txt_ap.o ./Core/Src/lvgl/src/misc/lv_txt_ap.su ./Core/Src/lvgl/src/misc/lv_utils.cyclo ./Core/Src/lvgl/src/misc/lv_utils.d ./Core/Src/lvgl/src/misc/lv_utils.o ./Core/Src/lvgl/src/misc/lv_utils.su

.PHONY: clean-Core-2f-Src-2f-lvgl-2f-src-2f-misc

