SET(CMAKE_SYSTEM_NAME Linux)

SET(CMAKE_C_COMPILER arm-linux-gnueabihf-gcc)
SET(CMAKE_CXX_COMPILER arm-linux-gnueabihf-g++)
SET(CMAKE_ASM_COMPILER arm-linux-gnueabihf-gcc)
SET(CMAKE_SYSTEM_PROCESSOR arm)

SET(CMAKE_RANLIB arm-linux-gnueabihf-ranlib)

SET(CMAKE_FIND_ROOT_PATH "/usr/arm-linux-gnueabihf")
SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)


add_definitions("-march=armv6 -mfpu=vfp -mfloat-abi=hard -marm")
#add_definitions("-march=armv7")
#add_definitions("-march=armv7-a")

# rdynamic means the backtrace should work
#IF (CMAKE_BUILD_TYPE MATCHES "Debug")
#   add_definitions(-rdynamic)
#ENDIF()

# avoids annoying and pointless warnings from gcc
#SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -U_FORTIFY_SOURCE")
#SET(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} -c")

