/**
 * @file LinkDef.h
 * @brief File needed for ROOT dictionaries creation
 *
 */

#include "FastFrames/MainFrame.h"
#include "FastFrames/DefineHelpersDict.h"

#ifdef __CINT__

#pragma extra_include "FastFrames/MainFrame.h";
#pragma extra_include "FastFrames/DefineHelpersDict.h";
#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;
#pragma link C++ nestedclass;
#pragma link C++ class MainFrame+;
#pragma link C++ class ROOT6_FastFramesHelperAutoloadHook+;

#endif
