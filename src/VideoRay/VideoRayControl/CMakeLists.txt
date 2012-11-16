PROJECT(VideoRayControl)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)

include_directories(${CMAKE_SOURCE_DIR}/../../moos-ivp/MOOS/Core)
link_directories(${CMAKE_SOURCE_DIR}/../../moos-ivp/MOOS/MOOSBin)

SET(SRCS main.cpp VideoRayControlApp.h VideoRayControlApp.cpp  ${RESOURCES})

# Add executable called pSysMon
ADD_EXECUTABLE( pVideoRayControl ${SRCS})

# indicate how to link
TARGET_LINK_LIBRARIES(pVideoRayControl m pthread MOOS MOOSGen sylloserial videoraycomm)
