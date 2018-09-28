project( PatternRecognition ) 
cmake_minimum_required( VERSION 2.8.7 )   



find_package(OpenCV REQUIRED)
INCLUDE_DIRECTORIES(${OpenCV_INCLUDE_DIRS}) 

add_library(PatternRecognition SHARED main.cpp PatternRecognition.h ) 
target_link_libraries(PatternRecognition rt dl m ${OpenCV_LIBRARIES} ) 


 

set_target_properties(PatternRecognition PROPERTIES 
                       #DEBUG_POSTFIX "D"
                       ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
                       LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
                       RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
                      )
