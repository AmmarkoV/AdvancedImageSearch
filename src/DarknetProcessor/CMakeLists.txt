project( DarknetProcessor ) 
cmake_minimum_required( VERSION 2.8.7 )   
 

ADD_LIBRARY(Darknet SHARED IMPORTED)
SET_TARGET_PROPERTIES(Darknet PROPERTIES IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/3dparty/darknet/libdarknet.a") 


INCLUDE_DIRECTORIES("/usr/local/cuda/include") 
LINK_DIRECTORIES("/usr/local/cuda/lib64")

add_library(DarknetProcessor SHARED DarknetProcessor.h DarknetProcessor.c recordOutput.h recordOutput.c imageOps.c imageOps.h ) 
target_link_libraries(DarknetProcessor Darknet rt dl m pthread cuda cudart cublas curand stdc++)  #CUDA 9.0  ${CUDA_LIBRARIES}  ? 
 

#target_compile_definitions(DarknetProcessor PRIVATE FOO=1 BAR=1)

set_target_properties(DarknetProcessor PROPERTIES 
                       #DEBUG_POSTFIX "D"
                       ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
                       LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
                       RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
                      )
