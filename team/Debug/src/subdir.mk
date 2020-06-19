################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/adminMensajes.c \
../src/funcionesEntrenador.c \
../src/funcionesTeam.c \
../src/planificacion.c \
../src/team.c 

OBJS += \
./src/adminMensajes.o \
./src/funcionesEntrenador.o \
./src/funcionesTeam.o \
./src/planificacion.o \
./src/team.o 

C_DEPS += \
./src/adminMensajes.d \
./src/funcionesEntrenador.d \
./src/funcionesTeam.d \
./src/planificacion.d \
./src/team.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
<<<<<<< HEAD
	gcc -I"/home/utnso/Documentos/TP/tp-2020-1c-BOMP/Utils" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
=======
	gcc -I"/home/utnso/workspace/tp-2020-1c-BOMP/Utils" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
>>>>>>> 7d5c2a525a444daf778e89b607988ca983f1356f
	@echo 'Finished building: $<'
	@echo ' '


