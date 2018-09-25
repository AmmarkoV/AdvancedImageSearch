project( cmpimg ) 
cmake_minimum_required( VERSION 2.8.7 ) 
     
add_executable(cmpimg cmpimg.c ) 
target_link_libraries(cmpimg rt dl m  AdvancedImageSearchLib jpeg png )
set_target_properties(cmpimg PROPERTIES DEBUG_POSTFIX "D")
add_dependencies(cmpimg AdvancedImageSearchLib ${PatternRecognitionParts} )  
       
 
set_target_properties(cmpimg PROPERTIES 
                       DEBUG_POSTFIX "D"
                       ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
                       LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
                       RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
                      )
