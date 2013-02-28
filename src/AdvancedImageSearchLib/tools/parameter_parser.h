#ifndef PARAMETER_PARSER_H_INCLUDED
#define PARAMETER_PARSER_H_INCLUDED


#include "../AdvancedImageSearchLib.h"

void printTimersToStderr();
void printListOfParametersRecognized();
char * parseCommandLineParameters(int argc, char *argv[], struct AISLib_SearchCriteria * criteria );

char searchCriteriaRequireOnlyImageHeaderLoaded(struct AISLib_SearchCriteria * criteria);


#endif // FILENAME_PARSER_H_INCLUDED
