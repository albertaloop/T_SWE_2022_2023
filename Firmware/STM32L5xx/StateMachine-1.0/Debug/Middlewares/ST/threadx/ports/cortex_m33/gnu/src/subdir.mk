################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Middlewares/ST/threadx/ports/cortex_m33/gnu/src/tx_thread_stack_error_handler.c \
../Middlewares/ST/threadx/ports/cortex_m33/gnu/src/tx_thread_stack_error_notify.c 

S_UPPER_SRCS += \
../Middlewares/ST/threadx/ports/cortex_m33/gnu/src/tx_thread_context_restore.S \
../Middlewares/ST/threadx/ports/cortex_m33/gnu/src/tx_thread_context_save.S \
../Middlewares/ST/threadx/ports/cortex_m33/gnu/src/tx_thread_interrupt_control.S \
../Middlewares/ST/threadx/ports/cortex_m33/gnu/src/tx_thread_interrupt_disable.S \
../Middlewares/ST/threadx/ports/cortex_m33/gnu/src/tx_thread_interrupt_restore.S \
../Middlewares/ST/threadx/ports/cortex_m33/gnu/src/tx_thread_schedule.S \
../Middlewares/ST/threadx/ports/cortex_m33/gnu/src/tx_thread_stack_build.S \
../Middlewares/ST/threadx/ports/cortex_m33/gnu/src/tx_thread_system_return.S \
../Middlewares/ST/threadx/ports/cortex_m33/gnu/src/tx_timer_interrupt.S 

OBJS += \
./Middlewares/ST/threadx/ports/cortex_m33/gnu/src/tx_thread_context_restore.o \
./Middlewares/ST/threadx/ports/cortex_m33/gnu/src/tx_thread_context_save.o \
./Middlewares/ST/threadx/ports/cortex_m33/gnu/src/tx_thread_interrupt_control.o \
./Middlewares/ST/threadx/ports/cortex_m33/gnu/src/tx_thread_interrupt_disable.o \
./Middlewares/ST/threadx/ports/cortex_m33/gnu/src/tx_thread_interrupt_restore.o \
./Middlewares/ST/threadx/ports/cortex_m33/gnu/src/tx_thread_schedule.o \
./Middlewares/ST/threadx/ports/cortex_m33/gnu/src/tx_thread_stack_build.o \
./Middlewares/ST/threadx/ports/cortex_m33/gnu/src/tx_thread_stack_error_handler.o \
./Middlewares/ST/threadx/ports/cortex_m33/gnu/src/tx_thread_stack_error_notify.o \
./Middlewares/ST/threadx/ports/cortex_m33/gnu/src/tx_thread_system_return.o \
./Middlewares/ST/threadx/ports/cortex_m33/gnu/src/tx_timer_interrupt.o 

S_UPPER_DEPS += \
./Middlewares/ST/threadx/ports/cortex_m33/gnu/src/tx_thread_context_restore.d \
./Middlewares/ST/threadx/ports/cortex_m33/gnu/src/tx_thread_context_save.d \
./Middlewares/ST/threadx/ports/cortex_m33/gnu/src/tx_thread_interrupt_control.d \
./Middlewares/ST/threadx/ports/cortex_m33/gnu/src/tx_thread_interrupt_disable.d \
./Middlewares/ST/threadx/ports/cortex_m33/gnu/src/tx_thread_interrupt_restore.d \
./Middlewares/ST/threadx/ports/cortex_m33/gnu/src/tx_thread_schedule.d \
./Middlewares/ST/threadx/ports/cortex_m33/gnu/src/tx_thread_stack_build.d \
./Middlewares/ST/threadx/ports/cortex_m33/gnu/src/tx_thread_system_return.d \
./Middlewares/ST/threadx/ports/cortex_m33/gnu/src/tx_timer_interrupt.d 

C_DEPS += \
./Middlewares/ST/threadx/ports/cortex_m33/gnu/src/tx_thread_stack_error_handler.d \
./Middlewares/ST/threadx/ports/cortex_m33/gnu/src/tx_thread_stack_error_notify.d 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/ST/threadx/ports/cortex_m33/gnu/src/%.o: ../Middlewares/ST/threadx/ports/cortex_m33/gnu/src/%.S Middlewares/ST/threadx/ports/cortex_m33/gnu/src/subdir.mk
	arm-none-eabi-gcc -mcpu=cortex-m33 -g3 -DDEBUG -DTX_SINGLE_MODE_NON_SECURE -DTX_LOW_POWER=1 -c -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@" "$<"
Middlewares/ST/threadx/ports/cortex_m33/gnu/src/%.o Middlewares/ST/threadx/ports/cortex_m33/gnu/src/%.su: ../Middlewares/ST/threadx/ports/cortex_m33/gnu/src/%.c Middlewares/ST/threadx/ports/cortex_m33/gnu/src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m33 -std=gnu11 -g3 -DDEBUG -DUSE_FULL_LL_DRIVER -DTX_SINGLE_MODE_NON_SECURE -DTX_INCLUDE_USER_DEFINE_FILE -DUSE_HAL_DRIVER -DSTM32L552xx -DSTM32_THREAD_SAFE_STRATEGY=2 -c -I../Core/Inc -I../AZURE_RTOS/App -I../Drivers/STM32L5xx_HAL_Driver/Inc -I../Drivers/STM32L5xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L5xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/ST/threadx/common/inc/ -I../Middlewares/ST/threadx/ports/cortex_m33/gnu/inc/ -I../Middlewares/ST/threadx/utility/low_power/ -I../Core/ThreadSafe -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Middlewares-2f-ST-2f-threadx-2f-ports-2f-cortex_m33-2f-gnu-2f-src

clean-Middlewares-2f-ST-2f-threadx-2f-ports-2f-cortex_m33-2f-gnu-2f-src:
	-$(RM) ./Middlewares/ST/threadx/ports/cortex_m33/gnu/src/tx_thread_context_restore.d ./Middlewares/ST/threadx/ports/cortex_m33/gnu/src/tx_thread_context_restore.o ./Middlewares/ST/threadx/ports/cortex_m33/gnu/src/tx_thread_context_save.d ./Middlewares/ST/threadx/ports/cortex_m33/gnu/src/tx_thread_context_save.o ./Middlewares/ST/threadx/ports/cortex_m33/gnu/src/tx_thread_interrupt_control.d ./Middlewares/ST/threadx/ports/cortex_m33/gnu/src/tx_thread_interrupt_control.o ./Middlewares/ST/threadx/ports/cortex_m33/gnu/src/tx_thread_interrupt_disable.d ./Middlewares/ST/threadx/ports/cortex_m33/gnu/src/tx_thread_interrupt_disable.o ./Middlewares/ST/threadx/ports/cortex_m33/gnu/src/tx_thread_interrupt_restore.d ./Middlewares/ST/threadx/ports/cortex_m33/gnu/src/tx_thread_interrupt_restore.o ./Middlewares/ST/threadx/ports/cortex_m33/gnu/src/tx_thread_schedule.d ./Middlewares/ST/threadx/ports/cortex_m33/gnu/src/tx_thread_schedule.o ./Middlewares/ST/threadx/ports/cortex_m33/gnu/src/tx_thread_stack_build.d ./Middlewares/ST/threadx/ports/cortex_m33/gnu/src/tx_thread_stack_build.o ./Middlewares/ST/threadx/ports/cortex_m33/gnu/src/tx_thread_stack_error_handler.d ./Middlewares/ST/threadx/ports/cortex_m33/gnu/src/tx_thread_stack_error_handler.o ./Middlewares/ST/threadx/ports/cortex_m33/gnu/src/tx_thread_stack_error_handler.su ./Middlewares/ST/threadx/ports/cortex_m33/gnu/src/tx_thread_stack_error_notify.d ./Middlewares/ST/threadx/ports/cortex_m33/gnu/src/tx_thread_stack_error_notify.o ./Middlewares/ST/threadx/ports/cortex_m33/gnu/src/tx_thread_stack_error_notify.su ./Middlewares/ST/threadx/ports/cortex_m33/gnu/src/tx_thread_system_return.d ./Middlewares/ST/threadx/ports/cortex_m33/gnu/src/tx_thread_system_return.o ./Middlewares/ST/threadx/ports/cortex_m33/gnu/src/tx_timer_interrupt.d ./Middlewares/ST/threadx/ports/cortex_m33/gnu/src/tx_timer_interrupt.o

.PHONY: clean-Middlewares-2f-ST-2f-threadx-2f-ports-2f-cortex_m33-2f-gnu-2f-src

