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


#ifndef ADVANCEDIMAGESEARCHLIB_H_INCLUDED
#define ADVANCEDIMAGESEARCHLIB_H_INCLUDED

#define MAX_CRITERIA_STRING_SIZE 1024

enum AISLib_ImageCategories
{
  NO_IMAGE_CATEGORY=0,
  FACE,
  COMIC,
  PHOTO,
  TEXT
};


enum AISLib_ImageColorRanges
{
  NO_COLOR_RANGE=0,
  FULL_COLOR,
  BLACK_AND_WHITE,
  SPECIFIC_COLOR
};


struct AISLib_SearchCriteria
{

   char criteriaSpecified;
   unsigned int limitResults;
   char needHelp;


   //Dimension restrains ( only header required )
   char minDimensionsUsed;
   unsigned int minWidth;
   unsigned int minHeight;

   char maxDimensionsUsed;
   unsigned int maxWidth;
   unsigned int maxHeight;

   char minAspectRatioUsed;
   float minAspectRatio;

   char maxAspectRatioUsed;
   float maxAspectRatio;

   char categoryUsed;
   unsigned int category;

   char minFacesUsed;
   unsigned int minimumFaceCount;

   char maxFacesUsed;
   unsigned int maximumFaceCount;

   //Content restrains ( image body required)
   char colorRangeUsed;
   unsigned int colorRange;
   unsigned char colorRangeSpecificR,colorRangeSpecificG,colorRangeSpecificB;


   unsigned int comparisonWidth;
   unsigned int comparisonHeight;

   char similarityUsed;
   char similarImageFilename[MAX_CRITERIA_STRING_SIZE];
   float similarityPercent;
   unsigned int perPixelThreshold;
   void * similarImage;

   char containsUsed;
   char containsImageFilename[MAX_CRITERIA_STRING_SIZE];
   float containsSimilarityPercent;
   void * containsImage;

};

struct AISLib_SearchResults
{
   unsigned int resultsMAX;
   unsigned int resultsCurrent;

};

const char * AISLib_Version();

void AISLib_printHelp();
char * AISLib_loadDirAndCriteriaFromArgs(int argc, char *argv[], struct AISLib_SearchCriteria * criteria );

struct AISLib_SearchResults *  AISLib_Search(char * directory,struct AISLib_SearchCriteria * criteria);
void AISLib_destroySearchResults(struct AISLib_SearchResults * sr);

struct AISLib_SearchCriteria * AISLib_createCriteria();
int AISLib_destroyCriteria(struct AISLib_SearchCriteria * criteria);



#endif // ADVANCEDIMAGESEARCHLIB_H_INCLUDED
