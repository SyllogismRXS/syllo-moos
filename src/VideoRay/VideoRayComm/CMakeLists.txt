PROJECT(videoraycomm)

ADD_LIBRARY( videoraycomm
	./protocol_pro4.h
	./types.h
	./protocol_pro4_device_types.h
	./protocol_pro4.cpp
	./pro4_specific_response.h
  	./VideoRay.h
  	./VideoRay.cpp
)

target_link_libraries(videoraycomm sylloserial)
