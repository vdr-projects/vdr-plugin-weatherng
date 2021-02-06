
#ifndef __SETUP_H
#define __SETUP_H


#include <ctype.h>
#include <climits>
#include <vector>
#include <string>
#include <vdr/plugin.h>

enum eWetterModes
{
 eWetterModeMaxNumber
}; 

struct cWetterSetup
{
public:
 cWetterSetup(void);
 char stationId[9];
 int w_left;
 int w_top;
 int w_width;
 int w_height;
 int w_rcolor;
 int w_fontsize;
 int w_offline;
 int w_theme;
};

extern cWetterSetup wetterSetup;

enum eWetterThemes
{
 eWetterThemeClassic,
 eWetterThemeenElchi,
 eWetterThemeMoronimo,
 eWetterThemeDeepBlue,
 eWetterThemeEnigma,
 eWetterThemeMaxNumber
 };

struct cWetterTheme
{
 int clrBackground;
 int clrFgHiColor;
 int clrFgLowColor;
 int clrFgText;
};

extern const cWetterTheme wetterTheme[eWetterThemeMaxNumber];

#endif //__SETUP_H 
