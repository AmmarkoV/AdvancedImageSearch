project( AdvancedImageSearch ) 
cmake_minimum_required( VERSION 2.8.7 )

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/3dparty ${CMAKE_MODULE_PATH})

set(CMAKE_CXX_FLAGS "-fPIC -march=native -mtune=native -O3 -fexpensive-optimizations -s") 
set(CMAKE_C_FLAGS "-fPIC -march=native -mtune=native -O3 -fexpensive-optimizations -s") 
      

OPTION (ENABLE_FACEDETECTION OFF)
OPTION (ENABLE_PATTERNRECOGNITION OFF)


#----
find_package(OpenCV)
if (OpenCV_FOUND)
 OPTION (ENABLE_OPENCV ON) 
ELSE (OpenCV_FOUND)
 OPTION (ENABLE_OPENCV OFF)
 MESSAGE("OpenCV Not found , some parts like viewer etc will not be compiled")
ENDIF(OpenCV_FOUND)
#----


if (ENABLE_PATTERNRECOGNITION)
 add_definitions(-DUSE_PATTERN_RECOGNITION)
ENDIF(ENABLE_PATTERNRECOGNITION)

IF( ENABLE_OPENCV )
 add_subdirectory (src/PatternRecognition) 
 #add_definitions(-DUSE_OPENCV_FACEDETECTION)
ENDIF( ENABLE_OPENCV )


add_subdirectory (src/AdvancedImageSearchLib)
add_subdirectory (src/lsimg)
add_subdirectory (src/cmpimg)

INSTALL(PROGRAMS "src/lsimg/lsimg" DESTINATION "/usr/bin" ) 
INSTALL(PROGRAMS "src/cmpimg/cmpimg" DESTINATION "/usr/bin" ) 


IF( ENABLE_OPENCV )
INSTALL(PROGRAMS "src/PatternRecognition/libPatternRecognition.so" DESTINATION "/usr/lib/AdvancedImageSearch" )
ENDIF( ENABLE_OPENCV )

INSTALL(PROGRAMS "src/AdvancedImageSearchLib/libAdvancedImageSearchLib.so" DESTINATION "/usr/lib/AdvancedImageSearch" ) 

INSTALL(FILES "data/haarcascade_frontalface_alt.xml" DESTINATION "/usr/lib/AdvancedImageSearch" ) 


# TODO FIX INSTALLATION DIRECTORIES 

#    install(TARGETS RGBDAcquisitionProject
#                    LIBRARY DESTINATION lib
#                    ARCHIVE DESTINATION lib
#                    RUNTIME DESTINATION bin)
 

