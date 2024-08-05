# CMSIS Build CMakeLists generated on 2024-08-05T15:27:52

cmake_minimum_required(VERSION 3.22)

# Target options

set(TARGET STM32G474RETx_Demo_Blink)
set(CPU Cortex-M4)
set(PRJ_DIR "c:/Users/Michael_Chemic/Desktop/STM32G4-DRV8301-FOC/MDK-ARM")
set(OUT_DIR "C:/Users/Michael_Chemic/Desktop/STM32G4-DRV8301-FOC/MDK-ARM/out/STM32G474RETx_Demo_Blink/STM32G474RETxDemoBlink")
set(INT_DIR "C:/Users/Michael_Chemic/Desktop/STM32G4-DRV8301-FOC/MDK-ARM/tmp/STM32G474RETx_Demo_Blink/STM32G474RETxDemoBlink")
set(FPU SP_FPU)
set(BYTE_ORDER Little-endian)
set(OPTIMIZE debug)
set(DEBUG on)
set(CC_FLAGS_GLOBAL "-std=c99 -fno-rtti -funsigned-char -fshort-enums -fshort-wchar -ffunction-sections -Wno-packed -Wno-missing-variable-declarations -Wno-missing-prototypes -Wno-missing-noreturn -Wno-sign-conversion -Wno-nonportable-include-path -Wno-reserved-id-macro -Wno-unused-macros -Wno-documentation-unknown-command -Wno-documentation -Wno-license-management -Wno-parentheses-equality")
set(CXX_FLAGS_GLOBAL "-xc++ -std=c++03 -fno-rtti -funsigned-char -fshort-enums -fshort-wchar -ffunction-sections -Wno-packed -Wno-missing-variable-declarations -Wno-missing-prototypes -Wno-missing-noreturn -Wno-sign-conversion -Wno-nonportable-include-path -Wno-reserved-id-macro -Wno-unused-macros -Wno-documentation-unknown-command -Wno-documentation -Wno-license-management -Wno-parentheses-equality")
set(LD_FLAGS_GLOBAL "--library_type=microlib --map --load_addr_map_info --xref --callgraph --symbols --info sizes --info totals --info unused --info veneers --strict --summary_stderr --info summarysizes")
set(LD_SCRIPT "c:/Users/Michael_Chemic/Desktop/STM32G4-DRV8301-FOC/MDK-ARM/STM32G474RETx_Demo_Blink_STM32G474RETxDemoBlink.sct")

set(DEFINES
  STM32G474xx
  _RTE_
  USE_HAL_DRIVER
  __MICROLIB
)

set(INC_PATHS
  "c:/Users/Michael_Chemic/Desktop/STM32G4-DRV8301-FOC/Core/Inc"
  "c:/Users/Michael_Chemic/Desktop/STM32G4-DRV8301-FOC/Drivers/STM32G4xx_HAL_Driver/Inc"
  "c:/Users/Michael_Chemic/Desktop/STM32G4-DRV8301-FOC/Drivers/STM32G4xx_HAL_Driver/Inc/Legacy"
  "c:/Users/Michael_Chemic/Desktop/STM32G4-DRV8301-FOC/Drivers/CMSIS/Device/ST/STM32G4xx/Include"
  "c:/Users/Michael_Chemic/Desktop/STM32G4-DRV8301-FOC/Drivers/CMSIS/Include"
  "c:/Users/Michael_Chemic/Desktop/STM32G4-DRV8301-FOC/FOC_Related/CONTROL"
  "c:/Users/Michael_Chemic/Desktop/STM32G4-DRV8301-FOC/FOC_Related/AS5600"
  "c:/Users/Michael_Chemic/Desktop/STM32G4-DRV8301-FOC/FOC_Related/FOC"
  "c:/Users/Michael_Chemic/Desktop/STM32G4-DRV8301-FOC/FOC_Related/LOWPASS_FILTER"
  "c:/Users/Michael_Chemic/Desktop/STM32G4-DRV8301-FOC/FOC_Related/PID"
  "c:/Users/Michael_Chemic/Desktop/STM32G4-DRV8301-FOC/FOC_Related/ABZ"
  "c:/Users/Michael_Chemic/Desktop/STM32G4-DRV8301-FOC/FOC_Related/HALL"
  "c:/Users/Michael_Chemic/Desktop/STM32G4-DRV8301-FOC/MDK-ARM/RTE/_STM32G474RETxDemoBlink"
  "C:/Users/Michael_Chemic/AppData/Local/arm/packs/ARM/CMSIS/5.9.0/CMSIS/Core/Include"
  "C:/Users/Michael_Chemic/AppData/Local/arm/packs/Keil/STM32G4xx_DFP/1.6.0/Drivers/CMSIS/Device/ST/STM32G4xx/Include"
)

set(AS_ARM_SRC_FILES
  "c:/Users/Michael_Chemic/Desktop/STM32G4-DRV8301-FOC/MDK-ARM/startup_stm32g474xx.s"
)

set(CC_SRC_FILES
  "c:/Users/Michael_Chemic/Desktop/STM32G4-DRV8301-FOC/Core/Src/adc.c"
  "c:/Users/Michael_Chemic/Desktop/STM32G4-DRV8301-FOC/Core/Src/dma.c"
  "c:/Users/Michael_Chemic/Desktop/STM32G4-DRV8301-FOC/Core/Src/fdcan.c"
  "c:/Users/Michael_Chemic/Desktop/STM32G4-DRV8301-FOC/Core/Src/gpio.c"
  "c:/Users/Michael_Chemic/Desktop/STM32G4-DRV8301-FOC/Core/Src/i2c.c"
  "c:/Users/Michael_Chemic/Desktop/STM32G4-DRV8301-FOC/Core/Src/main.c"
  "c:/Users/Michael_Chemic/Desktop/STM32G4-DRV8301-FOC/Core/Src/stm32g4xx_hal_msp.c"
  "c:/Users/Michael_Chemic/Desktop/STM32G4-DRV8301-FOC/Core/Src/stm32g4xx_it.c"
  "c:/Users/Michael_Chemic/Desktop/STM32G4-DRV8301-FOC/Core/Src/system_stm32g4xx.c"
  "c:/Users/Michael_Chemic/Desktop/STM32G4-DRV8301-FOC/Core/Src/tim.c"
  "c:/Users/Michael_Chemic/Desktop/STM32G4-DRV8301-FOC/Core/Src/usart.c"
  "c:/Users/Michael_Chemic/Desktop/STM32G4-DRV8301-FOC/Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal.c"
  "c:/Users/Michael_Chemic/Desktop/STM32G4-DRV8301-FOC/Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_adc.c"
  "c:/Users/Michael_Chemic/Desktop/STM32G4-DRV8301-FOC/Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_adc_ex.c"
  "c:/Users/Michael_Chemic/Desktop/STM32G4-DRV8301-FOC/Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_cortex.c"
  "c:/Users/Michael_Chemic/Desktop/STM32G4-DRV8301-FOC/Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_dma.c"
  "c:/Users/Michael_Chemic/Desktop/STM32G4-DRV8301-FOC/Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_dma_ex.c"
  "c:/Users/Michael_Chemic/Desktop/STM32G4-DRV8301-FOC/Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_exti.c"
  "c:/Users/Michael_Chemic/Desktop/STM32G4-DRV8301-FOC/Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_fdcan.c"
  "c:/Users/Michael_Chemic/Desktop/STM32G4-DRV8301-FOC/Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_flash.c"
  "c:/Users/Michael_Chemic/Desktop/STM32G4-DRV8301-FOC/Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_flash_ex.c"
  "c:/Users/Michael_Chemic/Desktop/STM32G4-DRV8301-FOC/Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_flash_ramfunc.c"
  "c:/Users/Michael_Chemic/Desktop/STM32G4-DRV8301-FOC/Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_gpio.c"
  "c:/Users/Michael_Chemic/Desktop/STM32G4-DRV8301-FOC/Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_i2c.c"
  "c:/Users/Michael_Chemic/Desktop/STM32G4-DRV8301-FOC/Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_i2c_ex.c"
  "c:/Users/Michael_Chemic/Desktop/STM32G4-DRV8301-FOC/Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_pwr.c"
  "c:/Users/Michael_Chemic/Desktop/STM32G4-DRV8301-FOC/Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_pwr_ex.c"
  "c:/Users/Michael_Chemic/Desktop/STM32G4-DRV8301-FOC/Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_rcc.c"
  "c:/Users/Michael_Chemic/Desktop/STM32G4-DRV8301-FOC/Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_rcc_ex.c"
  "c:/Users/Michael_Chemic/Desktop/STM32G4-DRV8301-FOC/Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_tim.c"
  "c:/Users/Michael_Chemic/Desktop/STM32G4-DRV8301-FOC/Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_tim_ex.c"
  "c:/Users/Michael_Chemic/Desktop/STM32G4-DRV8301-FOC/Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_uart.c"
  "c:/Users/Michael_Chemic/Desktop/STM32G4-DRV8301-FOC/Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_uart_ex.c"
  "c:/Users/Michael_Chemic/Desktop/STM32G4-DRV8301-FOC/Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_adc.c"
  "c:/Users/Michael_Chemic/Desktop/STM32G4-DRV8301-FOC/FOC_Related/ABZ/ABZ.c"
  "c:/Users/Michael_Chemic/Desktop/STM32G4-DRV8301-FOC/FOC_Related/AS5600/AS5600.c"
  "c:/Users/Michael_Chemic/Desktop/STM32G4-DRV8301-FOC/FOC_Related/CONTROL/control.c"
  "c:/Users/Michael_Chemic/Desktop/STM32G4-DRV8301-FOC/FOC_Related/FOC/FOC.c"
  "c:/Users/Michael_Chemic/Desktop/STM32G4-DRV8301-FOC/FOC_Related/HALL/HALL.c"
  "c:/Users/Michael_Chemic/Desktop/STM32G4-DRV8301-FOC/FOC_Related/LOWPASS_FILTER/LowPass_Filter.c"
  "c:/Users/Michael_Chemic/Desktop/STM32G4-DRV8301-FOC/FOC_Related/PID/pid.c"
)

set(AS_FLAGS_c:/Users/Michael_Chemic/Desktop/STM32G4-DRV8301-FOC/MDK-ARM/startup_stm32g474xx.s "-masm=auto")

# Toolchain config map

set(REGISTERED_TOOLCHAIN_ROOT "c:/Users/Michael_Chemic/AppData/Roaming/Code/User/globalStorage/ms-vscode.vscode-embedded-tools/vcpkg/root/downloads/artifacts/2139c4c6/compilers.arm.armclang/6.22.0/bin")
set(REGISTERED_TOOLCHAIN_VERSION "6.22.0")
set(TOOLCHAIN_VERSION_MIN "6.22.0")
include ("C:/Users/Michael_Chemic/AppData/Roaming/Code/User/globalStorage/ms-vscode.vscode-embedded-tools/vcpkg/root/downloads/artifacts/2139c4c6/tools.open.cmsis.pack.cmsis.toolbox/2.4.0/etc/AC6.6.16.2.cmake")
include ("C:/Users/Michael_Chemic/AppData/Roaming/Code/User/globalStorage/ms-vscode.vscode-embedded-tools/vcpkg/root/downloads/artifacts/2139c4c6/tools.open.cmsis.pack.cmsis.toolbox/2.4.0/etc/CMSIS-Build-Utils.cmake")

# Setup project

project(${TARGET} LANGUAGES AS_ARM C)

cbuild_get_running_toolchain(TOOLCHAIN_ROOT TOOLCHAIN_VERSION C)

# Global Flags

set(CMAKE_AS_ARM_FLAGS "${AS_ARM_CPU} ${AS_ARM_BYTE_ORDER} ${AS_ARM_DEFINES} ${AS_ARM_OPTIONS_FLAGS} ${AS_ARM_FLAGS}")
cbuild_get_system_includes(CC_SYS_INC_PATHS_LIST CC_SYS_INC_PATHS)
set(CMAKE_C_FLAGS "${CC_CPU} ${CC_BYTE_ORDER} ${CC_DEFINES} ${CC_OPTIONS_FLAGS} ${CC_FLAGS} ${CC_FLAGS_GLOBAL} ${CC_SYS_INC_PATHS}")
set(CMAKE_C_LINK_FLAGS "${LD_CPU} ${_LS}\"${LD_SCRIPT}\" ${LD_FLAGS_GLOBAL} ${LD_OPTIONS_FLAGS} ${LD_FLAGS}")

# Local Flags

foreach(SRC ${AS_ARM_SRC_FILES})
  string(REPLACE " " "?" S ${SRC})
  if(DEFINED AS_FLAGS_${S})
    set(AS_FLAGS_LOCAL "${AS_FLAGS_${S}}")
  else()
    set(AS_FLAGS_LOCAL "${AS_FLAGS_GLOBAL}")
  endif()
  set_source_files_properties(${SRC} PROPERTIES COMPILE_FLAGS "${AS_FLAGS_LOCAL}")
  set_source_files_properties(${SRC} PROPERTIES LANGUAGE AS_ARM)
endforeach()

# Compilation Database

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_custom_target(database COMMAND ${CMAKE_COMMAND} -E copy_if_different "${INT_DIR}/compile_commands.json" "${OUT_DIR}")

# Setup Target

add_executable(${TARGET} ${AS_ARM_SRC_FILES} ${CC_SRC_FILES})
set_target_properties(${TARGET} PROPERTIES PREFIX "" SUFFIX ".axf" OUTPUT_NAME "STM32G474RETx_Demo_Blink")
set_target_properties(${TARGET} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${OUT_DIR} LINK_DEPENDS ${LD_SCRIPT})
target_include_directories(${TARGET} PUBLIC ${INC_PATHS})
