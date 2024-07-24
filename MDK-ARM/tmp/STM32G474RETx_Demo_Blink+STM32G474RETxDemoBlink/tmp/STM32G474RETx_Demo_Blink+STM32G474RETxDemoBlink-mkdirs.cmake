# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "C:/Users/Michael_Chemic/Desktop/STM32G4-DRV8301-FOC/MDK-ARM/tmp/STM32G474RETx_Demo_Blink+STM32G474RETxDemoBlink"
  "C:/Users/Michael_Chemic/Desktop/STM32G4-DRV8301-FOC/MDK-ARM/tmp/1"
  "C:/Users/Michael_Chemic/Desktop/STM32G4-DRV8301-FOC/MDK-ARM/tmp/STM32G474RETx_Demo_Blink+STM32G474RETxDemoBlink"
  "C:/Users/Michael_Chemic/Desktop/STM32G4-DRV8301-FOC/MDK-ARM/tmp/STM32G474RETx_Demo_Blink+STM32G474RETxDemoBlink/tmp"
  "C:/Users/Michael_Chemic/Desktop/STM32G4-DRV8301-FOC/MDK-ARM/tmp/STM32G474RETx_Demo_Blink+STM32G474RETxDemoBlink/src/STM32G474RETx_Demo_Blink+STM32G474RETxDemoBlink-stamp"
  "C:/Users/Michael_Chemic/Desktop/STM32G4-DRV8301-FOC/MDK-ARM/tmp/STM32G474RETx_Demo_Blink+STM32G474RETxDemoBlink/src"
  "C:/Users/Michael_Chemic/Desktop/STM32G4-DRV8301-FOC/MDK-ARM/tmp/STM32G474RETx_Demo_Blink+STM32G474RETxDemoBlink/src/STM32G474RETx_Demo_Blink+STM32G474RETxDemoBlink-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "C:/Users/Michael_Chemic/Desktop/STM32G4-DRV8301-FOC/MDK-ARM/tmp/STM32G474RETx_Demo_Blink+STM32G474RETxDemoBlink/src/STM32G474RETx_Demo_Blink+STM32G474RETxDemoBlink-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "C:/Users/Michael_Chemic/Desktop/STM32G4-DRV8301-FOC/MDK-ARM/tmp/STM32G474RETx_Demo_Blink+STM32G474RETxDemoBlink/src/STM32G474RETx_Demo_Blink+STM32G474RETxDemoBlink-stamp${cfgdir}") # cfgdir has leading slash
endif()
