################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Middlewares/ST/threadx/utility/low_power/tx_low_power.c 

OBJS += \
./Middlewares/ST/threadx/utility/low_power/tx_low_power.o 

C_DEPS += \
./Middlewares/ST/threadx/utility/low_power/tx_low_power.d 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/ST/threadx/utility/low_power/%.o Middlewares/ST/threadx/utility/low_power/%.su Middlewares/ST/threadx/utility/low_power/%.cyclo: ../Middlewares/ST/threadx/utility/low_power/%.c Middlewares/ST/threadx/utility/low_power/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m33 -std=gnu11 -g3 -DDEBUG -DUSE_FULL_LL_DRIVER -DTX_SINGLE_MODE_NON_SECURE -DTX_INCLUDE_USER_DEFINE_FILE -DUSE_HAL_DRIVER -DSTM32L552xx -DSTM32_THREAD_SAFE_STRATEGY=2 -c -I../Core/Inc -I../AZURE_RTOS/App -I../Drivers/STM32L5xx_HAL_Driver/Inc -I../Drivers/STM32L5xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L5xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/ST/threadx/common/inc/ -I../Middlewares/ST/threadx/ports/cortex_m33/gnu/inc/ -I../Middlewares/ST/threadx/utility/low_power/ -I../Core/ThreadSafe -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Middlewares-2f-ST-2f-threadx-2f-utility-2f-low_power

clean-Middlewares-2f-ST-2f-threadx-2f-utility-2f-low_power:
	-$(RM) ./Middlewares/ST/threadx/utility/low_power/tx_low_power.cyclo ./Middlewares/ST/threadx/utility/low_power/tx_low_power.d ./Middlewares/ST/threadx/utility/low_power/tx_low_power.o ./Middlewares/ST/threadx/utility/low_power/tx_low_power.su

.PHONY: clean-Middlewares-2f-ST-2f-threadx-2f-utility-2f-low_power

