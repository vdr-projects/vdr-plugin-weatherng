#include "setup.h"

//const char *DestinationDir = NULL;

cWetterSetup wetterSetup;

cWetterSetup::cWetterSetup(void)
{
	strcpy(stationId, "GMXX0049");
	w_left       = 150;
	w_top        = 120;
	w_width      = 420;
	w_height     = 336;
	w_rcolor     = 0;
	w_fontsize   = 1;
	w_offline    = 1;
	w_theme      = 0;
}

const cWetterTheme wetterTheme[eWetterThemeMaxNumber] =
{
  {
    // Classic OK
    0x7F000000, //clrBackground
    0xFFFCC024, //clrFgHiColor
    0xFF00FCFC, //clrFgLowColor
//    0x7FFC1414, //clrBorder
    0xFFFCFCFC, //clrFgText + Border
  },
  { // enElchi  OK
    0xC8000066, //clrBackground
    0xFFCCBB22, //clrFgHiColor
    0xFF00FFFF, //clrFgLowColor
//    0xC833AAEE, //clrBorder
    0xC833AAEE, //clrFgText + Border
  },    
  { // Moronimo OK
    0xDF294A6B, //clrBackground
    0xFFCE7B00, //clrFgHiColor
    0xFF00FCFC, //clrFgLowColor
//    0xDF9BBAD7, //clrBorder
    0xFF9BBAD7, //clrFgText + Border
  },    
  { // DeepBlue OK
    0xC80C0C0C, //clrBackground
    0xFFCE7B00, //clrFgHiColor
    0xFF00FCFC, //clrFgLowColor
//    0xFF339999, //clrBorder
    0xFF9A9A9A, //clrFgText + Border
  },    
  { // Enigma OK
    0xB8BFC9E6, //clrBackground
    0xFFC4C400, //clrFgHiColor
    0xFF4158BC, //clrFgLowColor
//    0xB84158BC, //clrBorder
    0xFFFFFFFF, //clrFgText + Border
  },    
}; 
