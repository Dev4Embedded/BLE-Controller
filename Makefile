CC := arm-none-eabi-gcc
CCPP := arm-none-eabi-g++
OBJ := arm-none-eabi-objcopy

TARGET="ble_keyboard.elf"
FLASH_TOOL=STM32_Programmer_CLI

CC_COMMON_PARAMS := -mcpu=cortex-m4 \
                    -g3 \
                    -O0 \
                    -ffunction-sections \
                    -fdata-sections \
                    -Wall \
                    -MMD -MP \
                    -mfpu=fpv4-sp-d16 \
                    -mfloat-abi=hard \
                    -mthumb \
                    -fstack-usage \
                    --specs=nano.specs \

ASM_PARAMS := -mcpu=cortex-m4 \
              -g3 \
              -c -x assembler-with-cpp \
              -MMD -MP \
              --specs=nano.specs \
              -mfpu=fpv4-sp-d16 \
              -mfloat-abi=hard \
              -mthumb \

CC_PARAMS := ${CC_COMMON_PARAMS} \
             -std=gnu11 \

CC_CPP_PARAMS := ${CC_COMMON_PARAMS} \
                 -std=gnu++14 \
                 -fno-exceptions \
                 -fno-rtti \
                 -fno-use-cxa-atexit \

SYSTEM=System
DRIVERS=Drivers

LL_DRIVERS=${DRIVERS}/HAL_Driver
CMSIS=Drivers/CMSIS
CMSIS_ST_WB=${CMSIS}/Device/ST/STM32WBxx

SYSTEM_SRC=${SYSTEM}/Src
SYSTEM_INC=${SYSTEM}/Inc

CMSIS_INC=${CMSIS}/Include
CMSIS_ST_WB_INC=${CMSIS_ST_WB}/Include

LL_DRIVERS_SRC=${LL_DRIVERS}/Src
LL_DRIVERS_INC=${LL_DRIVERS}/Inc

DRIVER_CLOCK=${DRIVERS}/Clock
DRIVER_GPIO=${DRIVERS}/GPIO

LINKER_SCRIPT=-T "STM32WB55RGVX_FLASH.ld"

LINKER_FLAGS += -mcpu=cortex-m4 \
                $(LINKER_SCRIPT) \
                -specs=nosys.specs \
                -Wl,-Map="firmware.map" -Wl,--gc-sections \
                -static \
                -specs=nano.specs \
                -mfpu=fpv4-sp-d16 \
                -mfloat-abi=hard \
                -mthumb \
                -Wl,--start-group -lc -lm -lstdc++ -lsupc++ -Wl,--end-group

INCLUDES=-I ${LL_DRIVERS_INC} \
         -I ${CMSIS_INC} \
         -I ${CMSIS_ST_WB_INC} \
         -I ${SYSTEM_INC} \
         -I ${DRIVER_CLOCK} \
         -I ${DRIVER_GPIO} \

CPP_SOURCES= ${SYSTEM_SRC}/main.cpp \
             ${DRIVER_GPIO}/gpio.cpp \
             ${DRIVER_CLOCK}/clock.cpp \

C_SOURCES=${SYSTEM_SRC}/system_stm32wbxx.c \
          ${SYSTEM_SRC}/syscalls.c \
          ${SYSTEM_SRC}/sysmem.c \
          ${SYSTEM_SRC}/stm32wbxx_hal_msp.c \
          ${SYSTEM_SRC}/stm32wbxx_it.c \
          ${LL_DRIVERS_SRC}/stm32wbxx_hal.c \
          ${LL_DRIVERS_SRC}/stm32wbxx_hal_cortex.c \
          ${LL_DRIVERS_SRC}/stm32wbxx_hal_dma.c \
          ${LL_DRIVERS_SRC}/stm32wbxx_hal_dma_ex.c \
          ${LL_DRIVERS_SRC}/stm32wbxx_hal_exti.c \
          ${LL_DRIVERS_SRC}/stm32wbxx_hal_flash.c \
          ${LL_DRIVERS_SRC}/stm32wbxx_hal_flash_ex.c \
          ${LL_DRIVERS_SRC}/stm32wbxx_hal_gpio.c \
          ${LL_DRIVERS_SRC}/stm32wbxx_hal_hsem.c \
          ${LL_DRIVERS_SRC}/stm32wbxx_hal_pcd.c \
          ${LL_DRIVERS_SRC}/stm32wbxx_hal_pcd_ex.c \
          ${LL_DRIVERS_SRC}/stm32wbxx_hal_pwr.c \
          ${LL_DRIVERS_SRC}/stm32wbxx_hal_pwr_ex.c \
          ${LL_DRIVERS_SRC}/stm32wbxx_hal_rcc.c \
          ${LL_DRIVERS_SRC}/stm32wbxx_hal_rcc_ex.c \
          ${LL_DRIVERS_SRC}/stm32wbxx_hal_tim.c \
          ${LL_DRIVERS_SRC}/stm32wbxx_hal_tim_ex.c \

ASM_SOURCES=${SYSTEM_SRC}/startup_stm32wb55rgvx.s

DEFINES=-DSTM32WB55xx \
        -DUSE_HAL_DRIVER \
        -DDEBUG

OBJECTS =  $(notdir $(addsuffix .o, $(basename $(ASM_SOURCES))))
OBJECTS += $(notdir $(addsuffix .o, $(basename   $(C_SOURCES))))
OBJECTS += $(notdir $(addsuffix .o, $(basename $(CPP_SOURCES))))

binary: elf
	@echo Creating binary...
	@${OBJ} -O binary ${TARGET} firmware.bin


elf: c
	@echo Linking...
	@${CCPP} -o ${TARGET} ${OBJECTS} ${LINKER_FLAGS}

size: c
	arm-none-eabi-size ${OBJECTS}

cpp: asm
	@echo Compiling C++ sources...
	@for source in ${CPP_SOURCES} ; do \
	echo "    CC    $${source} "; \
	${CCPP} ${CC_CPP_PARAMS} ${INCLUDES} ${DEFINES} -c $${source}; \
	done

c: cpp
	@echo Compiling C sources...
	@for source in ${C_SOURCES} ; do \
	echo "    CC    $${source} "; \
	${CC} ${CC_PARAMS} ${INCLUDES} ${DEFINES} -c $${source}; \
	done

asm:
	@echo Compiling assembler sources...
	@for source in ${ASM_SOURCES} ; do \
	echo "    CC    $${source}"; \
	${CC} ${ASM_PARAMS} -c $${source}; \
	done

flash: elf
	@echo Start flashing...
	${FLASH_TOOL} -c port=swd -w ${TARGET} 0x8000000
	${FLASH_TOOL} -c port=swd --start 0x8000000

flash_raw:
	${FLASH_TOOL} -c port=swd -w nucleo-wb.elf 0x8000000
	${FLASH_TOOL} -c port=swd --start 0x8000000

clean:
	@echo Cleaning...
	@rm -f *.o *.d *.su *.map *.elf *.bin
	@echo done.
