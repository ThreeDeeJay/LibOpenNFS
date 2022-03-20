cmake_minimum_required(VERSION 3.7)
project(OpenNFS)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    add_compile_options (-fdiagnostics-color=always)
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    add_compile_options (-fcolor-diagnostics)
endif ()

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

set(SOURCE_DIR ${CMAKE_SOURCE_DIR}/sources)

set(NFS2_SOURCES 
        ${SOURCE_DIR}/Common/Utils.cpp  
        ${SOURCE_DIR}/NFS2/COL/ColFile.cpp
        ${SOURCE_DIR}/NFS2/GEO/GeoFile.cpp
        ${SOURCE_DIR}/NFS2/PSH/PshFile.cpp
        ${SOURCE_DIR}/NFS2/TRK/ExtraObjectBlock.cpp
        ${SOURCE_DIR}/NFS2/TRK/StructureBlock.cpp
        ${SOURCE_DIR}/NFS2/TRK/StructureRefBlock.cpp
        ${SOURCE_DIR}/NFS2/TRK/SuperBlock.cpp
        ${SOURCE_DIR}/NFS2/TRK/TrackBlock.cpp
        ${SOURCE_DIR}/NFS2/TRK/TrkFile.cpp
        #${SOURCE_DIR}/NFS2/Loader.cpp
)

set(NFS5_SOURCES
        ${SOURCE_DIR}/NFS5/CRP/CRPFile.cpp
        ${SOURCE_DIR}/NFS5/VIV/VIVFile.cpp
)

add_library(${PROJECT_NAME}_obj OBJECT ${NFS2_SOURCES} ${NFS5_SOURCES})

set_property(TARGET ${PROJECT_NAME}_obj PROPERTY POSITION_INDEPENDENT_CODE 1)

target_include_directories(${PROJECT_NAME}_obj
    PUBLIC ${CMAKE_SOURCE_DIR}/include
    PUBLIC ${CMAKE_SOURCE_DIR}/vendor/glm/glm)

add_library(${PROJECT_NAME}_shared SHARED $<TARGET_OBJECTS:${PROJECT_NAME}_obj>)
add_library(${PROJECT_NAME}_static STATIC $<TARGET_OBJECTS:${PROJECT_NAME}_obj>)

add_executable(${PROJECT_NAME} main.cpp $<TARGET_OBJECTS:${PROJECT_NAME}_obj>)
target_include_directories(${PROJECT_NAME}
    PUBLIC ${CMAKE_SOURCE_DIR}/include
    PUBLIC ${CMAKE_SOURCE_DIR}/vendor/glm)