PROJECT(VideoRaySim)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)

include_directories(${CMAKE_SOURCE_DIR}/../../moos-ivp/MOOS/Core)
link_directories(${CMAKE_SOURCE_DIR}/../../moos-ivp/MOOS/MOOSBin)

SET(SRCS main.cpp VideoRaySimApp.h VideoRaySimApp.cpp  ${RESOURCES})

# Add executable called pSysMon
ADD_EXECUTABLE( pVideoRaySim ${SRCS})

# indicate how to link
TARGET_LINK_LIBRARIES(pVideoRaySim m pthread MOOS)
