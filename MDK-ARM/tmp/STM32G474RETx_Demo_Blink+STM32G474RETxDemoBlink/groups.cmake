# groups.cmake

# group Application/MDK-ARM
add_library(Group_Application_MDK-ARM OBJECT
  "${SOLUTION_ROOT}/startup_stm32g474xx.s"
)
target_include_directories(Group_Application_MDK-ARM PUBLIC
  $<TARGET_PROPERTY:${CONTEXT},INTERFACE_INCLUDE_DIRECTORIES>
)
target_compile_definitions(Group_Application_MDK-ARM PUBLIC
  $<TARGET_PROPERTY:${CONTEXT},INTERFACE_COMPILE_DEFINITIONS>
)
target_compile_options(Group_Application_MDK-ARM PUBLIC
  $<TARGET_PROPERTY:${CONTEXT},INTERFACE_COMPILE_OPTIONS>
)
target_link_libraries(Group_Application_MDK-ARM PUBLIC
  ${CONTEXT}_ABSTRACTIONS
)
set(COMPILE_DEFINITIONS
  STM32G474xx
  _RTE_
)
cbuild_set_defines(AS_ARM COMPILE_DEFINITIONS)
set_source_files_properties("${SOLUTION_ROOT}/startup_stm32g474xx.s" PROPERTIES
  COMPILE_FLAGS "${COMPILE_DEFINITIONS}"
)
set_source_files_properties("${SOLUTION_ROOT}/startup_stm32g474xx.s" PROPERTIES
  COMPILE_OPTIONS "-masm=auto"
)

# group Application/User/Core
add_library(Group_Application_User_Core OBJECT
  "${SOLUTION_ROOT}/../FOC_Related/AS5600/AS5600.c"
  "${SOLUTION_ROOT}/../FOC_Related/CONTROL/control.c"
  "${SOLUTION_ROOT}/../FOC_Related/FOC/FOC.c"
  "${SOLUTION_ROOT}/../FOC_Related/LOWPASS_FILTER/LowPass_Filter.c"
  "${SOLUTION_ROOT}/../FOC_Related/PID/pid.c"
  "${SOLUTION_ROOT}/../FOC_Related/HALL/HALL.c"
  "${SOLUTION_ROOT}/../FOC_Related/ABZ/ABZ.c"
  "${SOLUTION_ROOT}/../Core/Src/main.c"
  "${SOLUTION_ROOT}/../Core/Src/gpio.c"
  "${SOLUTION_ROOT}/../Core/Src/adc.c"
  "${SOLUTION_ROOT}/../Core/Src/dma.c"
  "${SOLUTION_ROOT}/../Core/Src/fdcan.c"
  "${SOLUTION_ROOT}/../Core/Src/i2c.c"
  "${SOLUTION_ROOT}/../Core/Src/usart.c"
  "${SOLUTION_ROOT}/../Core/Src/tim.c"
  "${SOLUTION_ROOT}/../Core/Src/stm32g4xx_it.c"
  "${SOLUTION_ROOT}/../Core/Src/stm32g4xx_hal_msp.c"
)
target_include_directories(Group_Application_User_Core PUBLIC
  $<TARGET_PROPERTY:${CONTEXT},INTERFACE_INCLUDE_DIRECTORIES>
)
target_compile_definitions(Group_Application_User_Core PUBLIC
  $<TARGET_PROPERTY:${CONTEXT},INTERFACE_COMPILE_DEFINITIONS>
)
target_compile_options(Group_Application_User_Core PUBLIC
  $<TARGET_PROPERTY:${CONTEXT},INTERFACE_COMPILE_OPTIONS>
)
target_link_libraries(Group_Application_User_Core PUBLIC
  ${CONTEXT}_ABSTRACTIONS
)
set_source_files_properties("${SOLUTION_ROOT}/../Core/Src/gpio.c" PROPERTIES
  COMPILE_OPTIONS ""
)
set_source_files_properties("${SOLUTION_ROOT}/../Core/Src/adc.c" PROPERTIES
  COMPILE_OPTIONS ""
)
set_source_files_properties("${SOLUTION_ROOT}/../Core/Src/dma.c" PROPERTIES
  COMPILE_OPTIONS ""
)
set_source_files_properties("${SOLUTION_ROOT}/../Core/Src/fdcan.c" PROPERTIES
  COMPILE_OPTIONS ""
)
set_source_files_properties("${SOLUTION_ROOT}/../Core/Src/i2c.c" PROPERTIES
  COMPILE_OPTIONS ""
)
set_source_files_properties("${SOLUTION_ROOT}/../Core/Src/usart.c" PROPERTIES
  COMPILE_OPTIONS ""
)
set_source_files_properties("${SOLUTION_ROOT}/../Core/Src/tim.c" PROPERTIES
  COMPILE_OPTIONS ""
)

# group Drivers/STM32G4xx_HAL_Driver
add_library(Group_Drivers_STM32G4xx_HAL_Driver OBJECT
  "${SOLUTION_ROOT}/../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_adc.c"
  "${SOLUTION_ROOT}/../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_adc_ex.c"
  "${SOLUTION_ROOT}/../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_adc.c"
  "${SOLUTION_ROOT}/../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal.c"
  "${SOLUTION_ROOT}/../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_rcc.c"
  "${SOLUTION_ROOT}/../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_rcc_ex.c"
  "${SOLUTION_ROOT}/../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_flash.c"
  "${SOLUTION_ROOT}/../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_flash_ex.c"
  "${SOLUTION_ROOT}/../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_flash_ramfunc.c"
  "${SOLUTION_ROOT}/../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_gpio.c"
  "${SOLUTION_ROOT}/../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_exti.c"
  "${SOLUTION_ROOT}/../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_dma.c"
  "${SOLUTION_ROOT}/../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_dma_ex.c"
  "${SOLUTION_ROOT}/../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_pwr.c"
  "${SOLUTION_ROOT}/../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_pwr_ex.c"
  "${SOLUTION_ROOT}/../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_cortex.c"
  "${SOLUTION_ROOT}/../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_fdcan.c"
  "${SOLUTION_ROOT}/../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_i2c.c"
  "${SOLUTION_ROOT}/../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_i2c_ex.c"
  "${SOLUTION_ROOT}/../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_uart.c"
  "${SOLUTION_ROOT}/../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_uart_ex.c"
  "${SOLUTION_ROOT}/../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_tim.c"
  "${SOLUTION_ROOT}/../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_tim_ex.c"
)
target_include_directories(Group_Drivers_STM32G4xx_HAL_Driver PUBLIC
  $<TARGET_PROPERTY:${CONTEXT},INTERFACE_INCLUDE_DIRECTORIES>
)
target_compile_definitions(Group_Drivers_STM32G4xx_HAL_Driver PUBLIC
  $<TARGET_PROPERTY:${CONTEXT},INTERFACE_COMPILE_DEFINITIONS>
)
target_compile_options(Group_Drivers_STM32G4xx_HAL_Driver PUBLIC
  $<TARGET_PROPERTY:${CONTEXT},INTERFACE_COMPILE_OPTIONS>
)
target_link_libraries(Group_Drivers_STM32G4xx_HAL_Driver PUBLIC
  ${CONTEXT}_ABSTRACTIONS
)
set_source_files_properties("${SOLUTION_ROOT}/../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_adc.c" PROPERTIES
  COMPILE_OPTIONS ""
)
set_source_files_properties("${SOLUTION_ROOT}/../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_adc_ex.c" PROPERTIES
  COMPILE_OPTIONS ""
)
set_source_files_properties("${SOLUTION_ROOT}/../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_adc.c" PROPERTIES
  COMPILE_OPTIONS ""
)
set_source_files_properties("${SOLUTION_ROOT}/../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_fdcan.c" PROPERTIES
  COMPILE_OPTIONS ""
)
set_source_files_properties("${SOLUTION_ROOT}/../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_i2c.c" PROPERTIES
  COMPILE_OPTIONS ""
)
set_source_files_properties("${SOLUTION_ROOT}/../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_i2c_ex.c" PROPERTIES
  COMPILE_OPTIONS ""
)

# group Drivers/CMSIS
add_library(Group_Drivers_CMSIS OBJECT
  "${SOLUTION_ROOT}/../Core/Src/system_stm32g4xx.c"
)
target_include_directories(Group_Drivers_CMSIS PUBLIC
  $<TARGET_PROPERTY:${CONTEXT},INTERFACE_INCLUDE_DIRECTORIES>
)
target_compile_definitions(Group_Drivers_CMSIS PUBLIC
  $<TARGET_PROPERTY:${CONTEXT},INTERFACE_COMPILE_DEFINITIONS>
)
target_compile_options(Group_Drivers_CMSIS PUBLIC
  $<TARGET_PROPERTY:${CONTEXT},INTERFACE_COMPILE_OPTIONS>
)
target_link_libraries(Group_Drivers_CMSIS PUBLIC
  ${CONTEXT}_ABSTRACTIONS
)
