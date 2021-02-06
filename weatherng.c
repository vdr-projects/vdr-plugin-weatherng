/*
 * wetter.c: A plugin for the Video Disk Recorder
 *
 * See the README file for copyright information and how to reach the author.
 *
 * $Id: wetter.c,v 1.3 2005/01/23 00:56:49 beejay Exp $
 */

#include <getopt.h>
#include <string>
#include "getdata.h"
#include "setup.h"
#include "OsdWeather.h"
#include "weatherng.h" 
#include "vars.h"

cPluginWetter::cPluginWetter(void)
{
  // Initialize any member variables here.
  // DON'T DO ANYTHING ELSE THAT MAY HAVE SIDE EFFECTS, REQUIRE GLOBAL
  // VDR OBJECTS TO EXIST OR PRODUCE ANY OUTPUT!
}

cPluginWetter::~cPluginWetter()
{
    
  // Clean up after yourself!
}

const char *cPluginWetter::CommandLineHelp(void)
{
        static char *help_str = NULL;

       free(help_str);	
       asprintf(&help_str,
			  "  -D DIR    --data=DIR    DIR to datafiles of plugin.  e.g. /etc/plugins/weatherng\n"
			  "  -I DIR    --images=DIR  DIR to imagefolder of plugin e.g. /etc/plugins/weatherng\n");
  return help_str;         
}

bool cPluginWetter::ProcessArgs(int argc, char *argv[]) {
  static struct option long_options[] = {
       { "data",	required_argument, NULL, 'D' },
       { "images",	required_argument, NULL, 'I' },
       { NULL }
     };
     
  int c, option_index = 0;
  while ((c = getopt_long(argc, argv, "D:I:", long_options, &option_index)) != -1) {
          switch (c) {
	  case 'D': DataDir = optarg; break;
	  case 'I': ImageDir       = optarg; break;
	  default: return false;
	  }		    
	}
  return true;
}

bool cPluginWetter::Initialize(void)
{
  if (DataDir == NULL) {
          DataDir = strdup(ConfigDirectory(PLUGIN_NAME_I18N));	         
          esyslog("%s: Commandline parameter '-D' not set. Set default data directory to : %s\n", PLUGIN_NAME_I18N, DataDir);
  }
  if (ImageDir == NULL) {
          ImageDir = strdup(ConfigDirectory(PLUGIN_NAME_I18N));	         
          esyslog("%s: Commandline parameter '-I' not set. Set default plugin directory to : %s\n", PLUGIN_NAME_I18N, ImageDir);
  }

  return true;
}

bool cPluginWetter::Start(void)
{
  RegisterI18n(Phrases);
  return true;
}

void cPluginWetter::Housekeeping(void)
{
}

cOsdObject *cPluginWetter::MainMenuAction(void)
{
  return new cWetterOsd();
}

cMenuSetupPage *cPluginWetter::SetupMenu(void)
{
    return new cMenuWetterSetup;
}

bool cPluginWetter::SetupParse(const char *Name, const char *Value)
 {
	if      (!strcasecmp(Name, "StationId"))   strncpy(wetterSetup.stationId, Value, 8);
	else if (!strcasecmp(Name, "RadarLeft"))   wetterSetup.w_left     = atoi(Value);
	else if (!strcasecmp(Name, "RadarTop"))    wetterSetup.w_top      = atoi(Value);
	else if (!strcasecmp(Name, "RadarWidth"))  wetterSetup.w_width    = atoi(Value);
	else if (!strcasecmp(Name, "RadarHeight")) wetterSetup.w_height   = atoi(Value);
	else if (!strcasecmp(Name, "RadarColors")) wetterSetup.w_rcolor   = atoi(Value);
	else if (!strcasecmp(Name, "Fontsize"))    wetterSetup.w_fontsize = atoi(Value);
	else if (!strcasecmp(Name, "Offline"))     wetterSetup.w_offline  = atoi(Value);
	else if (!strcasecmp(Name, "Theme"))       wetterSetup.w_theme    = atoi(Value);
        else 
	  return false;
	return true;  
}


cMenuWetterSetup::cMenuWetterSetup(void)
{
  themes[eWetterThemeClassic]     = tr("Classic");
  themes[eWetterThemeenElchi]     = tr("enElchi");
  themes[eWetterThemeMoronimo]     = tr("Moronimo");
  themes[eWetterThemeDeepBlue]     = tr("DeepBlue");
  themes[eWetterThemeEnigma]     = tr("Enigma");

  Setup();
}    


void cMenuWetterSetup::Setup(void)
{
    int current = Current();

        Clear();
	Add(new cMenuEditStrItem(tr("Station ID"),                   wetterSetup.stationId, 9, ALLOWED_STATION_CHARS));
	Add(new cMenuEditIntItem(tr("Radar left"),                  &wetterSetup.w_left, 1, 200));
	Add(new cMenuEditIntItem(tr("Radar top"),                   &wetterSetup.w_top, 1, 200));
	Add(new cMenuEditIntItem(tr("Radar width"),                 &wetterSetup.w_width, 100, 450));
	Add(new cMenuEditIntItem(tr("Radar height"),                &wetterSetup.w_height, 100, 400));
	Add(new cMenuEditBoolItem(tr("256 Colordepth (radarmaps)"), &wetterSetup.w_rcolor));
	Add(new cMenuEditBoolItem(tr("Use small fonts"),            &wetterSetup.w_fontsize));
	Add(new cMenuEditBoolItem(tr("Use offlinemode"),            &wetterSetup.w_offline));
	Add(new cMenuEditStraItem(tr("Theme"),                      &wetterSetup.w_theme, eWetterThemeMaxNumber, themes));

  SetCurrent(Get(current));
  Display();	
}

void cMenuWetterSetup::Store(void)
{
	for(char *ptr = wetterSetup.stationId; *ptr; ++ptr) {
		if (islower(*ptr)) {
			*ptr = toupper(*ptr);
		}
	}

	SetupStore("StationId",       wetterSetup.stationId);
	SetupStore("RadarLeft",       wetterSetup.w_left);
	SetupStore("RadarTop",        wetterSetup.w_top);
	SetupStore("RadarWidth",      wetterSetup.w_width);
	SetupStore("RadarHeight",     wetterSetup.w_height);
	SetupStore("RadarColors",     wetterSetup.w_rcolor);
	SetupStore("Fontsize",        wetterSetup.w_fontsize);
	SetupStore("Offline",         wetterSetup.w_offline);
	SetupStore("Theme",           wetterSetup.w_theme);
	}

eOSState cMenuWetterSetup::ProcessKey(eKeys Key)
{
 eOSState state = cMenuSetupPage::ProcessKey(Key);
 if (Key != kNone) {
       Setup();
       }
 return state;
}
        
VDRPLUGINCREATOR(cPluginWetter); // Don't touch this!
