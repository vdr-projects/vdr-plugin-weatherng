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
			  "  -D DIR    --dest=DIR    DIR to configuration of plugin.  e.g. /etc/plugins/weatherng\n");
  return help_str;         
}

bool cPluginWetter::ProcessArgs(int argc, char *argv[]) {
  static struct option long_options[] = {
       { "dest",	required_argument, NULL, 'D' },
       { NULL }
     };
     
  int c, option_index = 0;
  while ((c = getopt_long(argc, argv, "D:", long_options, &option_index)) != -1) {
          switch (c) {
	  case 'D': DestinationDir = optarg; break;
	  default: return false;
	  }		    
	}
  return true;
}

bool cPluginWetter::Initialize(void)
{
  if (DestinationDir == NULL) {
          DestinationDir = strdup(ConfigDirectory(PLUGIN_NAME_I18N));	         
          esyslog("%s: Commandline parameter '-D' not set. Set default destination to : %s\n", PLUGIN_NAME_I18N, DestinationDir);
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
  return new cWetterOsd(&setup);
}

cMenuSetupPage *cPluginWetter::SetupMenu(void)
{
    return new cMenuSetupWetter(&setup, this);
}

bool cPluginWetter::SetupParse(const char *Name, const char *Value)
 {
	if      (!strcasecmp(Name, "StationId"))   strncpy(setup.stationId, Value, 8);
	else if (!strcasecmp(Name, "RadarLeft"))   setup.w_left   = atoi(Value);
	else if (!strcasecmp(Name, "RadarTop"))    setup.w_top    = atoi(Value);
	else if (!strcasecmp(Name, "RadarWidth"))  setup.w_width  = atoi(Value);
	else if (!strcasecmp(Name, "RadarHeight")) setup.w_height = atoi(Value);
	else if (!strcasecmp(Name, "Fontsize"))    setup.w_fontsize = atoi(Value);
        else 
	  return false;
}



cMenuSetupWetter::cMenuSetupWetter(cWetterSetup *setup, cPluginWetter *plugin) {
	this->setup = setup;
	this->plugin = plugin;

	Add(new cMenuEditStrItem(tr("Station ID"),   setup->stationId, 9, ALLOWED_STATION_CHARS));
	Add(new cMenuEditIntItem(tr("Radar left"),   &setup->w_left, 1, 200));
	Add(new cMenuEditIntItem(tr("Radar top"),    &setup->w_top, 1, 200));
	Add(new cMenuEditIntItem(tr("Radar width"),  &setup->w_width, 400, 450));
	Add(new cMenuEditIntItem(tr("Radar height"), &setup->w_height, 336, 400));
	Add(new cMenuEditBoolItem(tr("Use small fonts"), &setup->w_fontsize));
	
}

void cMenuSetupWetter::Store(void) {
	for(char *ptr = setup->stationId; *ptr; ++ptr) {
		if (islower(*ptr)) {
			*ptr = toupper(*ptr);
		}
	}

	SetupStore("StationId",    setup->stationId);
	SetupStore("RadarLeft",    setup->w_left);
	SetupStore("RadarTop",     setup->w_top);
	SetupStore("RadarWidth",   setup->w_width);
	SetupStore("RadarHeight",  setup->w_height);
	SetupStore("Fontsize",     setup->w_fontsize);
	}

VDRPLUGINCREATOR(cPluginWetter); // Don't touch this!
