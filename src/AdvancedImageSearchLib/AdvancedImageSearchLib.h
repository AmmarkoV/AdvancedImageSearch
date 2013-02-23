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


struct CStr
{
   unsigned int length;
   char * str;
};



struct AISLib_SearchCriteria
{


};

struct AISLib_SearchResults
{
   unsigned int resultsMAX;
   unsigned int resultsCurrent;

};


struct AISLib_SearchResults *  AISLib_Search(char * directory,struct AISLib_SearchCriteria * criteria);





#endif // ADVANCEDIMAGESEARCHLIB_H_INCLUDED
