################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../source/COMHandler.cpp \
../source/COMPort.cpp \
../source/ConfigFW.cpp \
../source/CrossSleep.cpp \
../source/IniFiles.cpp \
../source/Logger.cpp \
../source/NamedPipe.cpp \
../source/TON.cpp \
../source/UserInterface.cpp \
../source/UserPackFW.cpp \
../source/UserPackHL.cpp \
../source/UserPacksConverter.cpp 

OBJS += \
./source/COMHandler.o \
./source/COMPort.o \
./source/ConfigFW.o \
./source/CrossSleep.o \
./source/IniFiles.o \
./source/Logger.o \
./source/NamedPipe.o \
./source/TON.o \
./source/UserInterface.o \
./source/UserPackFW.o \
./source/UserPackHL.o \
./source/UserPacksConverter.o 

CPP_DEPS += \
./source/COMHandler.d \
./source/COMPort.d \
./source/ConfigFW.d \
./source/CrossSleep.d \
./source/IniFiles.d \
./source/Logger.d \
./source/NamedPipe.d \
./source/TON.d \
./source/UserInterface.d \
./source/UserPackFW.d \
./source/UserPackHL.d \
./source/UserPacksConverter.d 


# Each subdirectory must supply rules for building sources it contributes
source/%.o: ../source/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++  -std=c++14 -I"/home/roman/WORKSPACES/ECLIPSE_WS/UWB_Driver/UWB_Driver/header" -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


