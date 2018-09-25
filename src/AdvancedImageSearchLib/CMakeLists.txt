project( AdvancedImageSearchLib ) 
cmake_minimum_required( VERSION 2.8.7 )   
 
   
    set(ADV_IMG_LIB_INGREDIENTS

                 codecs/ppmInput.c
                 codecs/ppmInput.h
                 codecs/pngInput.c
                 codecs/pngInput.h
                 codecs/jpgInput.c
                 codecs/jpgInput.h
                 codecs/jpgExifexternal.c
                 codecs/jpgExifexternal.h
                 codecs/codecs.c    
                 codecs/codecs.h

                 caching/resultbank.c
                 caching/resultbank.h

                 image_processing/faceDetection.c
                 image_processing/faceDetection.h
                 image_processing/filters.c
                 image_processing/filters.h
                 image_processing/findPatterns.c
                 image_processing/findPatterns.h
                 image_processing/histograms.c
                 image_processing/histograms.h
                 image_processing/imageComparison.c
                 image_processing/imageComparison.h
                 image_processing/imageResizer.c
                 image_processing/imageResizer.h

                 tools/parameter_parser.c
                 tools/parameter_parser.h
                 tools/string_extension_scanner.c
                 tools/string_extension_scanner.h
                 tools/timers.c
                 tools/timers.h
 
                 main.c 
                 AdvancedImageSearchLib.h  
                 configuration.h
                 version.h
              )
  
    add_library(AdvancedImageSearchLib SHARED ${ADV_IMG_LIB_INGREDIENTS} ) 
    target_link_libraries(AdvancedImageSearchLib rt dl m  ${PatternRecognitionParts} ) 
    add_dependencies(AdvancedImageSearchLib ${PatternRecognitionParts} )  
 
 

set_target_properties(AdvancedImageSearchLib PROPERTIES 
                       DEBUG_POSTFIX "D"
                       ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
                       LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
                       RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
                      )
