################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/conexionBroker.c \
../src/utilsBroker.c \
../src/utilsGeneral.c 

OBJS += \
./src/conexionBroker.o \
./src/utilsBroker.o \
./src/utilsGeneral.o 

C_DEPS += \
./src/conexionBroker.d \
./src/utilsBroker.d \
./src/utilsGeneral.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


