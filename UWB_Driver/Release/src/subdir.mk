################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/COMHandler.cpp \
../src/COMPort.cpp \
../src/ConfigFW.cpp \
../src/CrossSleep.cpp \
../src/IniFiles.cpp \
../src/Logger.cpp \
../src/NamedPipe.cpp \
../src/TON.cpp \
../src/UserInterface.cpp \
../src/UserPackFW.cpp \
../src/UserPackHL.cpp \
../src/UserPacksConverter.cpp \
../src/main.cpp 

OBJS += \
./src/COMHandler.o \
./src/COMPort.o \
./src/ConfigFW.o \
./src/CrossSleep.o \
./src/IniFiles.o \
./src/Logger.o \
./src/NamedPipe.o \
./src/TON.o \
./src/UserInterface.o \
./src/UserPackFW.o \
./src/UserPackHL.o \
./src/UserPacksConverter.o \
./src/main.o 

CPP_DEPS += \
./src/COMHandler.d \
./src/COMPort.d \
./src/ConfigFW.d \
./src/CrossSleep.d \
./src/IniFiles.d \
./src/Logger.d \
./src/NamedPipe.d \
./src/TON.d \
./src/UserInterface.d \
./src/UserPackFW.d \
./src/UserPackHL.d \
./src/UserPacksConverter.d \
./src/main.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++  -std=c++14 -I"/home/roman/WORKSPACES/ECLIPSE_WS/UWB_Driver/UWB_Driver/header" -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


