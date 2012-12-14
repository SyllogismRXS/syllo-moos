PROJECT(VideoRayControl)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)

SET(SRCS main.cpp VideoRayApp.h VideoRayApp.cpp  ${RESOURCES})

# Add executable called pSysMon
ADD_EXECUTABLE( pVideoRay ${SRCS})

# indicate how to link
TARGET_LINK_LIBRARIES(pVideoRay m pthread MOOS MOOSGen videoraycomm)
