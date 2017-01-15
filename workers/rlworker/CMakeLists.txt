cmake_minimum_required (VERSION 3.0)
project (RLWorker)

include_directories("${PROJECT_SOURCE_DIR}/worker_sdk/include")
include_directories("${PROJECT_SOURCE_DIR}/generated")

if(NOT MSVC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")
endif()

find_library(LIB_WORKER_SDK WorkerSdk "${PROJECT_SOURCE_DIR}/worker_sdk/lib" NO_DEFAULT_PATH)
find_library(LIB_PROTO NAMES protobuf libprotobuf PATHS "${PROJECT_SOURCE_DIR}/worker_sdk/lib" NO_DEFAULT_PATH)
find_library(LIB_SSL ssl "${PROJECT_SOURCE_DIR}/worker_sdk/lib" NO_DEFAULT_PATH)
find_library(LIB_RAKNET RakNetLibStatic "${PROJECT_SOURCE_DIR}/worker_sdk/lib" NO_DEFAULT_PATH)
find_library(LIB_CORE_SDK CoreSdk "${PROJECT_SOURCE_DIR}/worker_sdk/lib" NO_DEFAULT_PATH)
find_library(LIB_GPR gpr "${PROJECT_SOURCE_DIR}/worker_sdk/lib" NO_DEFAULT_PATH)
find_library(LIB_GPRC grpc "${PROJECT_SOURCE_DIR}/worker_sdk/lib" NO_DEFAULT_PATH)
find_library(LIB_GPRCPP grpc++ "${PROJECT_SOURCE_DIR}/worker_sdk/lib" NO_DEFAULT_PATH)
find_library(LIB_Z_STATIC zlibstatic "${PROJECT_SOURCE_DIR}/worker_sdk/lib" NO_DEFAULT_PATH)

FILE(GLOB_RECURSE SOURCES "${PROJECT_SOURCE_DIR}/*.cc")

# Build the worker
add_executable(RLWorker ${SOURCES})
target_link_libraries(RLWorker ${LIB_WORKER_SDK} ${LIB_PROTO} ${LIB_SSL} ${LIB_RAKNET} ${CMAKE_DL_LIBS} ${LIB_CORE_SDK} ${LIB_GPR} ${LIB_GPRC} ${LIB_GPRCPP} ${LIB_Z_STATIC})