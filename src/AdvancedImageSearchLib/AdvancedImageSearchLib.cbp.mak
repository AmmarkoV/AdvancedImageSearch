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
CFLAGS =  -Wall -fPIC
RESINC = 
LIBDIR = 
LIB = 
LDFLAGS =  -lpng -ljpeg

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

OBJ_DEBUG = $(OBJDIR_DEBUG)/image_processing/imageComparison.o $(OBJDIR_DEBUG)/tools/string_extension_scanner.o $(OBJDIR_DEBUG)/tools/parameter_parser.o $(OBJDIR_DEBUG)/main.o $(OBJDIR_DEBUG)/image_processing/imageResizer.o $(OBJDIR_DEBUG)/image_processing/histograms.o $(OBJDIR_DEBUG)/codecs/ppm.o $(OBJDIR_DEBUG)/codecs/jpg.o $(OBJDIR_DEBUG)/codecs/codecs.o

OBJ_RELEASE = $(OBJDIR_RELEASE)/image_processing/imageComparison.o $(OBJDIR_RELEASE)/tools/string_extension_scanner.o $(OBJDIR_RELEASE)/tools/parameter_parser.o $(OBJDIR_RELEASE)/main.o $(OBJDIR_RELEASE)/image_processing/imageResizer.o $(OBJDIR_RELEASE)/image_processing/histograms.o $(OBJDIR_RELEASE)/codecs/ppm.o $(OBJDIR_RELEASE)/codecs/jpg.o $(OBJDIR_RELEASE)/codecs/codecs.o

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

$(OBJDIR_DEBUG)/image_processing/imageComparison.o: image_processing/imageComparison.c
	$(CC) $(CFLAGS_DEBUG) $(INC_DEBUG) -c image_processing/imageComparison.c -o $(OBJDIR_DEBUG)/image_processing/imageComparison.o

$(OBJDIR_DEBUG)/tools/string_extension_scanner.o: tools/string_extension_scanner.c
	$(CC) $(CFLAGS_DEBUG) $(INC_DEBUG) -c tools/string_extension_scanner.c -o $(OBJDIR_DEBUG)/tools/string_extension_scanner.o

$(OBJDIR_DEBUG)/tools/parameter_parser.o: tools/parameter_parser.c
	$(CC) $(CFLAGS_DEBUG) $(INC_DEBUG) -c tools/parameter_parser.c -o $(OBJDIR_DEBUG)/tools/parameter_parser.o

$(OBJDIR_DEBUG)/main.o: main.c
	$(CC) $(CFLAGS_DEBUG) $(INC_DEBUG) -c main.c -o $(OBJDIR_DEBUG)/main.o

$(OBJDIR_DEBUG)/image_processing/imageResizer.o: image_processing/imageResizer.c
	$(CC) $(CFLAGS_DEBUG) $(INC_DEBUG) -c image_processing/imageResizer.c -o $(OBJDIR_DEBUG)/image_processing/imageResizer.o

$(OBJDIR_DEBUG)/image_processing/histograms.o: image_processing/histograms.c
	$(CC) $(CFLAGS_DEBUG) $(INC_DEBUG) -c image_processing/histograms.c -o $(OBJDIR_DEBUG)/image_processing/histograms.o

$(OBJDIR_DEBUG)/codecs/ppm.o: codecs/ppm.c
	$(CC) $(CFLAGS_DEBUG) $(INC_DEBUG) -c codecs/ppm.c -o $(OBJDIR_DEBUG)/codecs/ppm.o

$(OBJDIR_DEBUG)/codecs/jpg.o: codecs/jpg.c
	$(CC) $(CFLAGS_DEBUG) $(INC_DEBUG) -c codecs/jpg.c -o $(OBJDIR_DEBUG)/codecs/jpg.o

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

$(OBJDIR_RELEASE)/image_processing/imageComparison.o: image_processing/imageComparison.c
	$(CC) $(CFLAGS_RELEASE) $(INC_RELEASE) -c image_processing/imageComparison.c -o $(OBJDIR_RELEASE)/image_processing/imageComparison.o

$(OBJDIR_RELEASE)/tools/string_extension_scanner.o: tools/string_extension_scanner.c
	$(CC) $(CFLAGS_RELEASE) $(INC_RELEASE) -c tools/string_extension_scanner.c -o $(OBJDIR_RELEASE)/tools/string_extension_scanner.o

$(OBJDIR_RELEASE)/tools/parameter_parser.o: tools/parameter_parser.c
	$(CC) $(CFLAGS_RELEASE) $(INC_RELEASE) -c tools/parameter_parser.c -o $(OBJDIR_RELEASE)/tools/parameter_parser.o

$(OBJDIR_RELEASE)/main.o: main.c
	$(CC) $(CFLAGS_RELEASE) $(INC_RELEASE) -c main.c -o $(OBJDIR_RELEASE)/main.o

$(OBJDIR_RELEASE)/image_processing/imageResizer.o: image_processing/imageResizer.c
	$(CC) $(CFLAGS_RELEASE) $(INC_RELEASE) -c image_processing/imageResizer.c -o $(OBJDIR_RELEASE)/image_processing/imageResizer.o

$(OBJDIR_RELEASE)/image_processing/histograms.o: image_processing/histograms.c
	$(CC) $(CFLAGS_RELEASE) $(INC_RELEASE) -c image_processing/histograms.c -o $(OBJDIR_RELEASE)/image_processing/histograms.o

$(OBJDIR_RELEASE)/codecs/ppm.o: codecs/ppm.c
	$(CC) $(CFLAGS_RELEASE) $(INC_RELEASE) -c codecs/ppm.c -o $(OBJDIR_RELEASE)/codecs/ppm.o

$(OBJDIR_RELEASE)/codecs/jpg.o: codecs/jpg.c
	$(CC) $(CFLAGS_RELEASE) $(INC_RELEASE) -c codecs/jpg.c -o $(OBJDIR_RELEASE)/codecs/jpg.o

$(OBJDIR_RELEASE)/codecs/codecs.o: codecs/codecs.c
	$(CC) $(CFLAGS_RELEASE) $(INC_RELEASE) -c codecs/codecs.c -o $(OBJDIR_RELEASE)/codecs/codecs.o

clean_release: 
	rm -f $(OBJ_RELEASE) $(OUT_RELEASE)
	rm -rf $(OBJDIR_RELEASE)/image_processing
	rm -rf $(OBJDIR_RELEASE)/tools
	rm -rf $(OBJDIR_RELEASE)
	rm -rf $(OBJDIR_RELEASE)/codecs

.PHONY: before_debug after_debug clean_debug before_release after_release clean_release

