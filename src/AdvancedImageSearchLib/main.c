/* AdvancedImageSearch Lib..
A fancy , OpenGL slideshow application !
URLs: http://ammar.gr
Written by Ammar Qammaz a.k.a. AmmarkoV 2013

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.
*/


#include "AdvancedImageSearchLib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include "version.h"

#include "codecs/codecs.h"
#include "codecs/jpgInput.h"
#include "codecs/ppmInput.h"

#include "image_processing/histograms.h"
#include "image_processing/imageResizer.h"
#include "image_processing/imageComparison.h"
#include "image_processing/faceDetection.h"
#include "image_processing/findPatterns.h"

#include "caching/resultbank.h"


#include "tools/string_extension_scanner.h"
#include "tools/parameter_parser.h"
#include "tools/timers.h"

#define INITIAL_ALLOCATED_MEMORY_FOR_RESULTS 1000


const char * AISLib_Version()
{
  return FULLVERSION_STRING;
}

// Creators / Destructors for CRITERIA
//----------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------
struct AISLib_SearchCriteria * AISLib_createCriteria()
{
   struct AISLib_SearchCriteria * criteria = ( struct AISLib_SearchCriteria *  ) malloc(sizeof(struct AISLib_SearchCriteria));
   if (criteria==0) { fprintf(stderr,"Could not allocate space for criteria , returning null criteria\n"); }

   memset(criteria,0,sizeof(struct AISLib_SearchCriteria));

   criteria->comparisonWidth=64;
   criteria->comparisonHeight=64;

   return criteria;
}


int AISLib_destroyCriteria(struct AISLib_SearchCriteria * criteria)
{
  if (criteria==0) { return 1; }
  if (criteria->similarImage!=0)
     {
       struct Image * img = (struct Image *) criteria->similarImage;
       destroyImage(img);
     }
  if (criteria->containsImage!=0)
     {
       struct Image * img = (struct Image *) criteria->containsImage;
       destroyImage(img);
     }


  free(criteria);
  return 1;
}
//----------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------


// Creators / Destructors for RESULTS
//----------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------
struct AISLib_SearchResults * createSearchResults(unsigned int initialNumberOfResults)
{
  struct AISLib_SearchResults * sr = (struct AISLib_SearchResults *)  malloc(initialNumberOfResults* sizeof(struct AISLib_SearchResults));
  if (sr==0) { fprintf(stderr,"Could not allocate new search results"); return 0;}

  sr->resultsMAX = initialNumberOfResults;
  return sr;
}

void AISLib_destroySearchResults(struct AISLib_SearchResults * sr)
{
  if (sr==0) { return ; }
  free(sr);
  return;
}



struct AISLib_SearchResults * addMoreSearchResults(struct AISLib_SearchResults * initial_results,unsigned int addedNumberOfResults)
{
  struct AISLib_SearchResults * sr = (struct AISLib_SearchResults *)  realloc(initial_results , (initial_results->resultsMAX+addedNumberOfResults)* sizeof(struct AISLib_SearchResults));
  if (sr==0)
   {
       fprintf(stderr,"Could not allocate more results ");
       return initial_results;
   }

  sr->resultsMAX += addedNumberOfResults;
  return sr;
}
//----------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------



void AISLib_printHelp()
{
    //Exposes parameters that are recognized by the library
    printListOfParametersRecognized();
}

char * AISLib_loadDirAndCriteriaFromArgs(int argc, char *argv[], struct AISLib_SearchCriteria * criteria )
{
    //Parse argc and argv , populate criteria and return working directory
    return parseCommandLineParameters(argc,argv,criteria);
}

inline int fileIsImage(char * filename)
{
  return scanStringForImageExtensionsSimple(filename);
}

int imageFitsCriteria(char * filename , struct Image * img,struct AISLib_SearchCriteria * criteria)
{
   if (img==0) { fprintf(stderr,"imageFitsCriteria : Incorrect image \n");    return 0; }
   if (criteria==0) { fprintf(stderr,"imageFitsCriteria : Incorrect criteria \n"); return 0; }

   if (criteria->minDimensionsUsed)
    { //Ready to discard for violation of minimum dimensions
      if ( ( img->width < criteria->minWidth ) || ( img->height < criteria->minHeight ) ) { return 0; }
    }

   if (criteria->maxDimensionsUsed)
    { //Ready to discard for violation of maximum dimensions
      if ( ( img->width > criteria->maxWidth ) || ( img->height > criteria->maxHeight ) ) { return 0; }
    }

   if (criteria->colorRangeUsed)
    {
      //Ready to discard for violation of histogram
      if (img->pixels==0) { return 0; } // No pixels , no histogram , no success

      struct Histogram * histogram = request_generateHistogram( filename , img );

      if ( histogram!=0 )
      {
         if (! histogramIsCloseToColor(histogram,  criteria->colorRangeSpecificR  ,
                                                   criteria->colorRangeSpecificG  ,
                                                   criteria->colorRangeSpecificB,
                                                   criteria->colorRange,
                                                   img->width * img->height * img->channels * (img->bitsperpixel/8),
                                                   30.0 ) )
                                                   {
                                                    free(histogram);
                                                    histogram=0;
                                                    return 0;
                                                   }
        free(histogram);
        histogram=0;
       } else
       {
          //No Histogram no color but range enforced , doesnt fit the criteria
          return 0;
       }
    }


    if (criteria->similarityUsed)
    {
       if (img->pixels==0) { return 0; } // No pixels , no histogram , no success
       //fprintf(stderr,"Trying %s \n",filename);
       struct Image * imgThumbnail = resizeImage(img , criteria->comparisonWidth , criteria->comparisonHeight );
       struct Image * referenceImg = (struct Image *) criteria->similarImage;

       if   (imgThumbnail==0)  { return 0; /*Can't resize , Can't compare doesnt fit criteria */ }

       unsigned int similar = imagesAreSimilar(referenceImg,imgThumbnail,  criteria->perPixelThreshold , criteria->similarityPercent);
       //WritePPM("fail_comp.ppm",imgThumbnail);
       destroyImage(imgThumbnail);  /*We succesfully resized , we don't need the original any more*/
       if (!similar) { return 0; }
    }


    if ( (criteria->minFacesUsed) || (criteria->maxFacesUsed) )
    {
      if (img->pixels==0) { return 0; } // No pixels , no histogram , no success
      unsigned int faces = request_imageHasNFaces(filename, img);
      if ( ( criteria->minimumFaceCount < faces ) && (criteria->minFacesUsed) ) { return 0; }
      if ( ( criteria->maximumFaceCount > faces ) && (criteria->maxFacesUsed) ) { return 0; }
    }


    //This should be the last test since it converts input  image to grayscale
    //it also converts the pattern to grayscale but only one time!
    if (criteria->containsUsed)
    {
      if (!request_findPatternInImage(filename , criteria->containsImageFilename , criteria->containsImage,img)) { return 0; }
    }

    return 1;
}



struct AISLib_SearchResults * AISLib_Search(char * directory,struct AISLib_SearchCriteria * criteria)
{
  initTimers();

  DIR *dpdf=0;
  unsigned int numberOfResults = 0;
  struct dirent *epdf=0;

  char fullPath[4096]={0};

  if (directory == 0) { dpdf = opendir("./"); } else
                      { dpdf = opendir(directory); }

  if (dpdf != 0)
   {
     StartTimer(SEARCH_OPERATION_DELAY);

     struct AISLib_SearchResults * sr = createSearchResults(INITIAL_ALLOCATED_MEMORY_FOR_RESULTS);
     if (sr==0) { return 0; }

     epdf = readdir(dpdf);
     while ( ( epdf !=  0 ) && ( (criteria->limitResults==0)||(numberOfResults < criteria->limitResults) )  )
      {
          unsigned int image_type  = fileIsImage(epdf->d_name);
          if (image_type!=0)
          {
             if (!criteria->criteriaSpecified)
               {
                  //No criteria specified , return all images
                  printf("%s ",epdf->d_name);
               } else
               {
                 strcpy(fullPath,directory);
                 strcat(fullPath,"/");
                 strcat(fullPath,epdf->d_name);

                 struct Image * img = readImage(fullPath , image_type , searchCriteriaRequireOnlyImageHeaderLoaded(criteria) );
                 if (  img!=0 )
                   {
                     if (imageFitsCriteria(epdf->d_name,img,criteria))
                        {
                          ++numberOfResults;
                          printf("%s ",epdf->d_name);
                        } else
                        {
                          #if PRINT_DEBUG_INFO
                            fprintf(stderr,"Image doesn't fit criteria %s\n",epdf->d_name);
                          #endif // PRINT_DEBUG_INFO
                        }

                  //fprintf(stderr,"Survived read , I have a %ux%u image ",pic.width,pic.height);
                   destroyImage(img);
                   img=0;
                  } else
                  {
                     #if PRINT_DEBUG_INFO
                        fprintf(stderr,"Could not readImage %s\n",epdf->d_name);
                     #endif // PRINT_DEBUG_INFO
                  }

                }
          } else
          {
             #if PRINT_DEBUG_INFO
               fprintf(stderr,"Image %s is not compatible \n",epdf->d_name);
             #endif // PRINT_DEBUG_INFO
          }

        //Next Filename to check
        epdf = readdir(dpdf);
      }

      EndTimer(SEARCH_OPERATION_DELAY);
      if (criteria->printTimers) { printTimersToStderr();  }
      destroyTimers();

      closedir(dpdf);
      return sr;
    }

  return 0;
}


int AIS_CompareImages(char * image1,char * image2)
{
   struct Image * img1  = readImage(image1,JPG_CODEC,0);
   if (img1==0) { return COULD_NOT_PERFORM_COMPARISON ; }
   struct Image * img2  = readImage(image2,JPG_CODEC,0);
   if (img2==0) { return COULD_NOT_PERFORM_COMPARISON ; }


   int result = findPatternInImage(img1,img2);
   //Not implemented yet

   destroyImage(img1);
   destroyImage(img2);

   if (result) { return SIMILAR; }
   return COULD_NOT_PERFORM_COMPARISON;
}



