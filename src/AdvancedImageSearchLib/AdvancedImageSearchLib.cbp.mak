#------------------------------------------------------------------------------#
# This makefile was generated by 'cbp2make' tool rev.138                       #
#------------------------------------------------------------------------------#


WORKDIR = `pwd`

CC = gcc
CXX = g++
AR = ar
LD = g++
WINDRES = windres

INC = 
CFLAGS =  -Wall -fPIC `pkg-config --cflags opencv`
RESINC = 
LIBDIR = 
LIB =  ../PatternRecognition/libPatternRecognition.so
LDFLAGS =  -lpng -ljpeg `pkg-config --libs opencv`

INC_DEBUG =  $(INC)
CFLAGS_DEBUG =  $(CFLAGS) -g -fPIC
RESINC_DEBUG =  $(RESINC)
RCFLAGS_DEBUG =  $(RCFLAGS)
LIBDIR_DEBUG =  $(LIBDIR)
LIB_DEBUG = $(LIB)
LDFLAGS_DEBUG =  $(LDFLAGS)
OBJDIR_DEBUG = obj/Debug
DEP_DEBUG = 
OUT_DEBUG = AdvancedImageSearch.so

INC_RELEASE =  $(INC)
CFLAGS_RELEASE =  $(CFLAGS) -fexpensive-optimizations -O3 -fPIC -march=native -mtune=native
RESINC_RELEASE =  $(RESINC)
RCFLAGS_RELEASE =  $(RCFLAGS)
LIBDIR_RELEASE =  $(LIBDIR)
LIB_RELEASE = $(LIB)
LDFLAGS_RELEASE =  $(LDFLAGS) -s
OBJDIR_RELEASE = obj/Release
DEP_RELEASE = 
OUT_RELEASE = AdvancedImageSearch.so

OBJ_DEBUG = $(OBJDIR_DEBUG)/image_processing/findObjectsSURF.o $(OBJDIR_DEBUG)/tools/timers.o $(OBJDIR_DEBUG)/tools/string_extension_scanner.o $(OBJDIR_DEBUG)/tools/parameter_parser.o $(OBJDIR_DEBUG)/main.o $(OBJDIR_DEBUG)/image_processing/imageResizer.o $(OBJDIR_DEBUG)/image_processing/imageComparison.o $(OBJDIR_DEBUG)/image_processing/histograms.o $(OBJDIR_DEBUG)/image_processing/filters.o $(OBJDIR_DEBUG)/image_processing/faceDetection.o $(OBJDIR_DEBUG)/codecs/ppmInput.o $(OBJDIR_DEBUG)/codecs/pngInput.o $(OBJDIR_DEBUG)/codecs/jpgInput.o $(OBJDIR_DEBUG)/codecs/jpgExifexternal.o $(OBJDIR_DEBUG)/codecs/codecs.o

OBJ_RELEASE = $(OBJDIR_RELEASE)/image_processing/findObjectsSURF.o $(OBJDIR_RELEASE)/tools/timers.o $(OBJDIR_RELEASE)/tools/string_extension_scanner.o $(OBJDIR_RELEASE)/tools/parameter_parser.o $(OBJDIR_RELEASE)/main.o $(OBJDIR_RELEASE)/image_processing/imageResizer.o $(OBJDIR_RELEASE)/image_processing/imageComparison.o $(OBJDIR_RELEASE)/image_processing/histograms.o $(OBJDIR_RELEASE)/image_processing/filters.o $(OBJDIR_RELEASE)/image_processing/faceDetection.o $(OBJDIR_RELEASE)/codecs/ppmInput.o $(OBJDIR_RELEASE)/codecs/pngInput.o $(OBJDIR_RELEASE)/codecs/jpgInput.o $(OBJDIR_RELEASE)/codecs/jpgExifexternal.o $(OBJDIR_RELEASE)/codecs/codecs.o

all: debug release

clean: clean_debug clean_release

before_debug: 
	test -d $(OBJDIR_DEBUG)/image_processing || mkdir -p $(OBJDIR_DEBUG)/image_processing
	test -d $(OBJDIR_DEBUG)/tools || mkdir -p $(OBJDIR_DEBUG)/tools
	test -d $(OBJDIR_DEBUG) || mkdir -p $(OBJDIR_DEBUG)
	test -d $(OBJDIR_DEBUG)/codecs || mkdir -p $(OBJDIR_DEBUG)/codecs

after_debug: 

debug: before_debug out_debug after_debug

out_debug: before_debug $(OBJ_DEBUG) $(DEP_DEBUG)
	$(LD) -shared $(LIBDIR_DEBUG) $(OBJ_DEBUG)  -o $(OUT_DEBUG) $(LDFLAGS_DEBUG) $(LIB_DEBUG)

$(OBJDIR_DEBUG)/image_processing/findObjectsSURF.o: image_processing/findObjectsSURF.c
	$(CC) $(CFLAGS_DEBUG) $(INC_DEBUG) -c image_processing/findObjectsSURF.c -o $(OBJDIR_DEBUG)/image_processing/findObjectsSURF.o

$(OBJDIR_DEBUG)/tools/timers.o: tools/timers.c
	$(CC) $(CFLAGS_DEBUG) $(INC_DEBUG) -c tools/timers.c -o $(OBJDIR_DEBUG)/tools/timers.o

$(OBJDIR_DEBUG)/tools/string_extension_scanner.o: tools/string_extension_scanner.c
	$(CC) $(CFLAGS_DEBUG) $(INC_DEBUG) -c tools/string_extension_scanner.c -o $(OBJDIR_DEBUG)/tools/string_extension_scanner.o

$(OBJDIR_DEBUG)/tools/parameter_parser.o: tools/parameter_parser.c
	$(CC) $(CFLAGS_DEBUG) $(INC_DEBUG) -c tools/parameter_parser.c -o $(OBJDIR_DEBUG)/tools/parameter_parser.o

$(OBJDIR_DEBUG)/main.o: main.c
	$(CC) $(CFLAGS_DEBUG) $(INC_DEBUG) -c main.c -o $(OBJDIR_DEBUG)/main.o

$(OBJDIR_DEBUG)/image_processing/imageResizer.o: image_processing/imageResizer.c
	$(CC) $(CFLAGS_DEBUG) $(INC_DEBUG) -c image_processing/imageResizer.c -o $(OBJDIR_DEBUG)/image_processing/imageResizer.o

$(OBJDIR_DEBUG)/image_processing/imageComparison.o: image_processing/imageComparison.c
	$(CC) $(CFLAGS_DEBUG) $(INC_DEBUG) -c image_processing/imageComparison.c -o $(OBJDIR_DEBUG)/image_processing/imageComparison.o

$(OBJDIR_DEBUG)/image_processing/histograms.o: image_processing/histograms.c
	$(CC) $(CFLAGS_DEBUG) $(INC_DEBUG) -c image_processing/histograms.c -o $(OBJDIR_DEBUG)/image_processing/histograms.o

$(OBJDIR_DEBUG)/image_processing/filters.o: image_processing/filters.c
	$(CC) $(CFLAGS_DEBUG) $(INC_DEBUG) -c image_processing/filters.c -o $(OBJDIR_DEBUG)/image_processing/filters.o

$(OBJDIR_DEBUG)/image_processing/faceDetection.o: image_processing/faceDetection.c
	$(CC) $(CFLAGS_DEBUG) $(INC_DEBUG) -c image_processing/faceDetection.c -o $(OBJDIR_DEBUG)/image_processing/faceDetection.o

$(OBJDIR_DEBUG)/codecs/ppmInput.o: codecs/ppmInput.c
	$(CC) $(CFLAGS_DEBUG) $(INC_DEBUG) -c codecs/ppmInput.c -o $(OBJDIR_DEBUG)/codecs/ppmInput.o

$(OBJDIR_DEBUG)/codecs/pngInput.o: codecs/pngInput.c
	$(CC) $(CFLAGS_DEBUG) $(INC_DEBUG) -c codecs/pngInput.c -o $(OBJDIR_DEBUG)/codecs/pngInput.o

$(OBJDIR_DEBUG)/codecs/jpgInput.o: codecs/jpgInput.c
	$(CC) $(CFLAGS_DEBUG) $(INC_DEBUG) -c codecs/jpgInput.c -o $(OBJDIR_DEBUG)/codecs/jpgInput.o

$(OBJDIR_DEBUG)/codecs/jpgExifexternal.o: codecs/jpgExifexternal.c
	$(CC) $(CFLAGS_DEBUG) $(INC_DEBUG) -c codecs/jpgExifexternal.c -o $(OBJDIR_DEBUG)/codecs/jpgExifexternal.o

$(OBJDIR_DEBUG)/codecs/codecs.o: codecs/codecs.c
	$(CC) $(CFLAGS_DEBUG) $(INC_DEBUG) -c codecs/codecs.c -o $(OBJDIR_DEBUG)/codecs/codecs.o

clean_debug: 
	rm -f $(OBJ_DEBUG) $(OUT_DEBUG)
	rm -rf $(OBJDIR_DEBUG)/image_processing
	rm -rf $(OBJDIR_DEBUG)/tools
	rm -rf $(OBJDIR_DEBUG)
	rm -rf $(OBJDIR_DEBUG)/codecs

before_release: 
	test -d $(OBJDIR_RELEASE)/image_processing || mkdir -p $(OBJDIR_RELEASE)/image_processing
	test -d $(OBJDIR_RELEASE)/tools || mkdir -p $(OBJDIR_RELEASE)/tools
	test -d $(OBJDIR_RELEASE) || mkdir -p $(OBJDIR_RELEASE)
	test -d $(OBJDIR_RELEASE)/codecs || mkdir -p $(OBJDIR_RELEASE)/codecs

after_release: 

release: before_release out_release after_release

out_release: before_release $(OBJ_RELEASE) $(DEP_RELEASE)
	$(LD) -shared $(LIBDIR_RELEASE) $(OBJ_RELEASE)  -o $(OUT_RELEASE) $(LDFLAGS_RELEASE) $(LIB_RELEASE)

$(OBJDIR_RELEASE)/image_processing/findObjectsSURF.o: image_processing/findObjectsSURF.c
	$(CC) $(CFLAGS_RELEASE) $(INC_RELEASE) -c image_processing/findObjectsSURF.c -o $(OBJDIR_RELEASE)/image_processing/findObjectsSURF.o

$(OBJDIR_RELEASE)/tools/timers.o: tools/timers.c
	$(CC) $(CFLAGS_RELEASE) $(INC_RELEASE) -c tools/timers.c -o $(OBJDIR_RELEASE)/tools/timers.o

$(OBJDIR_RELEASE)/tools/string_extension_scanner.o: tools/string_extension_scanner.c
	$(CC) $(CFLAGS_RELEASE) $(INC_RELEASE) -c tools/string_extension_scanner.c -o $(OBJDIR_RELEASE)/tools/string_extension_scanner.o

$(OBJDIR_RELEASE)/tools/parameter_parser.o: tools/parameter_parser.c
	$(CC) $(CFLAGS_RELEASE) $(INC_RELEASE) -c tools/parameter_parser.c -o $(OBJDIR_RELEASE)/tools/parameter_parser.o

$(OBJDIR_RELEASE)/main.o: main.c
	$(CC) $(CFLAGS_RELEASE) $(INC_RELEASE) -c main.c -o $(OBJDIR_RELEASE)/main.o

$(OBJDIR_RELEASE)/image_processing/imageResizer.o: image_processing/imageResizer.c
	$(CC) $(CFLAGS_RELEASE) $(INC_RELEASE) -c image_processing/imageResizer.c -o $(OBJDIR_RELEASE)/image_processing/imageResizer.o

$(OBJDIR_RELEASE)/image_processing/imageComparison.o: image_processing/imageComparison.c
	$(CC) $(CFLAGS_RELEASE) $(INC_RELEASE) -c image_processing/imageComparison.c -o $(OBJDIR_RELEASE)/image_processing/imageComparison.o

$(OBJDIR_RELEASE)/image_processing/histograms.o: image_processing/histograms.c
	$(CC) $(CFLAGS_RELEASE) $(INC_RELEASE) -c image_processing/histograms.c -o $(OBJDIR_RELEASE)/image_processing/histograms.o

$(OBJDIR_RELEASE)/image_processing/filters.o: image_processing/filters.c
	$(CC) $(CFLAGS_RELEASE) $(INC_RELEASE) -c image_processing/filters.c -o $(OBJDIR_RELEASE)/image_processing/filters.o

$(OBJDIR_RELEASE)/image_processing/faceDetection.o: image_processing/faceDetection.c
	$(CC) $(CFLAGS_RELEASE) $(INC_RELEASE) -c image_processing/faceDetection.c -o $(OBJDIR_RELEASE)/image_processing/faceDetection.o

$(OBJDIR_RELEASE)/codecs/ppmInput.o: codecs/ppmInput.c
	$(CC) $(CFLAGS_RELEASE) $(INC_RELEASE) -c codecs/ppmInput.c -o $(OBJDIR_RELEASE)/codecs/ppmInput.o

$(OBJDIR_RELEASE)/codecs/pngInput.o: codecs/pngInput.c
	$(CC) $(CFLAGS_RELEASE) $(INC_RELEASE) -c codecs/pngInput.c -o $(OBJDIR_RELEASE)/codecs/pngInput.o

$(OBJDIR_RELEASE)/codecs/jpgInput.o: codecs/jpgInput.c
	$(CC) $(CFLAGS_RELEASE) $(INC_RELEASE) -c codecs/jpgInput.c -o $(OBJDIR_RELEASE)/codecs/jpgInput.o

$(OBJDIR_RELEASE)/codecs/jpgExifexternal.o: codecs/jpgExifexternal.c
	$(CC) $(CFLAGS_RELEASE) $(INC_RELEASE) -c codecs/jpgExifexternal.c -o $(OBJDIR_RELEASE)/codecs/jpgExifexternal.o

$(OBJDIR_RELEASE)/codecs/codecs.o: codecs/codecs.c
	$(CC) $(CFLAGS_RELEASE) $(INC_RELEASE) -c codecs/codecs.c -o $(OBJDIR_RELEASE)/codecs/codecs.o

clean_release: 
	rm -f $(OBJ_RELEASE) $(OUT_RELEASE)
	rm -rf $(OBJDIR_RELEASE)/image_processing
	rm -rf $(OBJDIR_RELEASE)/tools
	rm -rf $(OBJDIR_RELEASE)
	rm -rf $(OBJDIR_RELEASE)/codecs

.PHONY: before_debug after_debug clean_debug before_release after_release clean_release

