################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/broker.c \
../src/particionesDinamicas.c 

OBJS += \
./src/broker.o \
./src/particionesDinamicas.o 

C_DEPS += \
./src/broker.d \
./src/particionesDinamicas.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'

	gcc -I"/home/utnso/tp-2020-1c-BOMP/Utils" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"

	@echo 'Finished building: $<'
	@echo ' '


