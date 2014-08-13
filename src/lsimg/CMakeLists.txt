project( lsimg ) 
cmake_minimum_required( VERSION 2.8.7 ) 
     
add_executable(lsimg main.c ) 
target_link_libraries(lsimg rt dl m  AdvancedImageSearchLib jpeg png )
set_target_properties(lsimg PROPERTIES DEBUG_POSTFIX "D")
add_dependencies(lsimg AdvancedImageSearchLib )  
       
 
set_target_properties(lsimg PROPERTIES 
                       DEBUG_POSTFIX "D"
                       ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
                       LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
                       RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
                      )
