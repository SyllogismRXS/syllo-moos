PROJECT(videoraycomm)

ADD_LIBRARY( videoraycomm
	./protocol_pro4.h
	./protocol_pro4_device_types.h
	./pro4_specific_response.h  
  	./types.h
  	./protocol_pro4.cpp
  	./VideoRay.h
  	./VideoRay.cpp
)

target_link_libraries(videoraycomm sylloserial)
