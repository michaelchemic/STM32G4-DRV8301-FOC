cmake_minimum_required(VERSION 3.22)

set(CONTEXT STM32G474RETx_Demo_Blink+STM32G474RETxDemoBlink)
set(TARGET ${CONTEXT})
set(OUT_DIR "${SOLUTION_ROOT}/out/STM32G474RETx_Demo_Blink/STM32G474RETxDemoBlink")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(LD_SCRIPT "${SOLUTION_ROOT}/STM32G474RETx_Demo_Blink_STM32G474RETxDemoBlink.sct")
set(LD_SCRIPT_PP ${LD_SCRIPT})

# Processor Options
set(CPU Cortex-M4)
set(FPU SP_FPU)
set(BYTE_ORDER Little-endian)

# Toolchain config map
include("toolchain.cmake")

# Setup project
project(${CONTEXT} LANGUAGES ASM C)

# Compilation database
add_custom_target(database COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_CURRENT_BINARY_DIR}/compile_commands.json" "${OUT_DIR}")
set(CMAKE_C_STANDARD_INCLUDE_DIRECTORIES ${CMAKE_C_IMPLICIT_INCLUDE_DIRECTORIES})

# Setup context
add_executable(${CONTEXT})
set_target_properties(${CONTEXT} PROPERTIES PREFIX "" SUFFIX ".axf" OUTPUT_NAME "STM32G474RETx_Demo_Blink")
set_target_properties(${CONTEXT} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${OUT_DIR})
add_library(${CONTEXT}_GLOBAL INTERFACE)

# Includes
target_include_directories(${CONTEXT} PUBLIC
  $<$<COMPILE_LANGUAGE:C,CXX>:
    ${SOLUTION_ROOT}/../Core/Inc
    ${SOLUTION_ROOT}/../Drivers/STM32G4xx_HAL_Driver/Inc
    ${SOLUTION_ROOT}/../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy
    ${SOLUTION_ROOT}/../Drivers/CMSIS/Device/ST/STM32G4xx/Include
    ${SOLUTION_ROOT}/../Drivers/CMSIS/Include
    ${SOLUTION_ROOT}/../FOC_Related/CONTROL
    ${SOLUTION_ROOT}/../FOC_Related/AS5600
    ${SOLUTION_ROOT}/../FOC_Related/FOC
    ${SOLUTION_ROOT}/../FOC_Related/LOWPASS_FILTER
    ${SOLUTION_ROOT}/../FOC_Related/PID
    ${SOLUTION_ROOT}/../FOC_Related/ABZ
    ${SOLUTION_ROOT}/../FOC_Related/HALL
  >
  ${SOLUTION_ROOT}/RTE/_STM32G474RETxDemoBlink
  ${CMSIS_PACK_ROOT}/ARM/CMSIS/5.9.0/CMSIS/Core/Include
  ${CMSIS_PACK_ROOT}/Keil/STM32G4xx_DFP/1.6.0/Drivers/CMSIS/Device/ST/STM32G4xx/Include
)

# Defines
target_compile_definitions(${CONTEXT} PUBLIC
  $<$<COMPILE_LANGUAGE:C,CXX>:
    USE_HAL_DRIVER
    STM32G474xx
    __MICROLIB
    STM32G474xx
    _RTE_
  >
)

# Compile options
target_compile_options(${CONTEXT} PUBLIC
  $<$<COMPILE_LANGUAGE:ASM>:
    "SHELL:${ASM_CPU}"
    "SHELL:${ASM_FLAGS}"
    "SHELL:${ASM_BYTE_ORDER}"
  >
  $<$<COMPILE_LANGUAGE:C>:
    "SHELL:${CC_CPU}"
    "SHELL:${CC_FLAGS}"
    "SHELL:${CC_BYTE_ORDER}"
    "SHELL:-std=c99"
    "SHELL:-fno-rtti"
    "SHELL:-funsigned-char"
    "SHELL:-fshort-enums"
    "SHELL:-fshort-wchar"
    "SHELL:-ffunction-sections"
    "SHELL:-Wno-packed"
    "SHELL:-Wno-missing-variable-declarations"
    "SHELL:-Wno-missing-prototypes"
    "SHELL:-Wno-missing-noreturn"
    "SHELL:-Wno-sign-conversion"
    "SHELL:-Wno-nonportable-include-path"
    "SHELL:-Wno-reserved-id-macro"
    "SHELL:-Wno-unused-macros"
    "SHELL:-Wno-documentation-unknown-command"
    "SHELL:-Wno-documentation"
    "SHELL:-Wno-license-management"
    "SHELL:-Wno-parentheses-equality"
  >
)

# Compile Options Abstractions
add_library(${CONTEXT}_ABSTRACTIONS INTERFACE)
cbuild_set_options_flags(ASM "debug" "on" "" "" ASM_OPTIONS_FLAGS_${CONTEXT})
cbuild_set_options_flags(CC "debug" "on" "" "" CC_OPTIONS_FLAGS_${CONTEXT})
target_compile_options(${CONTEXT}_ABSTRACTIONS INTERFACE
  $<$<COMPILE_LANGUAGE:ASM>:
    "SHELL:${ASM_OPTIONS_FLAGS_${CONTEXT}}"
  >
  $<$<COMPILE_LANGUAGE:C>:
    "SHELL:${CC_OPTIONS_FLAGS_${CONTEXT}}"
  >
)

# Add groups and components
include("groups.cmake")
include("components.cmake")

target_link_libraries(${CONTEXT} PUBLIC
  Group_Application_MDK-ARM
  Group_Application_User_Core
  Group_Drivers_STM32G4xx_HAL_Driver
  Group_Drivers_CMSIS
  ARM_CMSIS_CORE_5_6_0
)

# Linker options
target_link_options(${CONTEXT} PUBLIC
  "SHELL:${LD_CPU}"
  "SHELL:${_LS}\"${LD_SCRIPT_PP}\""
  "SHELL:--library_type=microlib"
  "SHELL:--map"
  "SHELL:--load_addr_map_info"
  "SHELL:--xref"
  "SHELL:--callgraph"
  "SHELL:--symbols"
  "SHELL:--info sizes"
  "SHELL:--info totals"
  "SHELL:--info unused"
  "SHELL:--info veneers"
  "SHELL:--strict"
  "SHELL:--summary_stderr"
  "SHELL:--info summarysizes"
)
set_target_properties(${CONTEXT} PROPERTIES LINK_DEPENDS ${LD_SCRIPT})
