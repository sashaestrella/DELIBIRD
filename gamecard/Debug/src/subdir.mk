################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/adminMensajes.c \
../src/gamecard.c \
../src/gamecardParaPruebas.c 

OBJS += \
./src/adminMensajes.o \
./src/gamecard.o \
./src/gamecardParaPruebas.o 

C_DEPS += \
./src/adminMensajes.d \
./src/gamecard.d \
./src/gamecardParaPruebas.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/utnso/workspace/tp-2020-1c-BOMP/Utils" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


