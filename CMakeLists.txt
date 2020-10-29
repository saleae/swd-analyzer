cmake_minimum_required (VERSION 3.11)
project(swd_analyzer)



# enable generation of compile_commands.json, helpful for IDEs to locate include files.
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# custom CMake Modules are located in the cmake directory.
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

include(ExternalAnalyzerSDK)

set(SOURCES 
src/SWDAnalyzer.cpp
src/SWDAnalyzer.h
src/SWDAnalyzerResults.cpp
src/SWDAnalyzerResults.h
src/SWDAnalyzerSettings.cpp
src/SWDAnalyzerSettings.h
src/SWDSimulationDataGenerator.cpp
src/SWDSimulationDataGenerator.h
src/SWDTypes.cpp
src/SWDTypes.h
src/SWDUtils.cpp
src/SWDUtils.h
)

add_analyzer_plugin(swd_analyzer SOURCES ${SOURCES})
