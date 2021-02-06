/*
 * wetter.c: A plugin for the Video Disk Recorder
 *
 * See the README file for copyright information and how to reach the author.
 *
 * $Id: wetter.c,v 1.2 2005/01/18 08:46:02 boonkerz Exp $
 */

#include "wetter.h" 

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
  // Return a string that describes all known command line options.
  return NULL;
}

bool cPluginWetter::ProcessArgs(int argc, char *argv[])
{
  // Implement command line argument processing here if applicable.
  return true;
}

bool cPluginWetter::Initialize(void)
{
//	   xmlParse("GMXX0056"); 
// Initialize any background activities the plugin shall perform.
  return true;
}

bool cPluginWetter::Start(void)
{
// Start any background activities the plugin shall perform.
  return true;
}

void cPluginWetter::Housekeeping(void)
{
// Perform any cleanup or other regular tasks.
}

cOsdObject *cPluginWetter::MainMenuAction(void)
{
// Perform the action when selected from the main VDR menu.
  return new cWetterOsd(&setup);
}

cMenuSetupPage *cPluginWetter::SetupMenu(void)
{
  // Return a setup menu in case the plugin supports one.
    return new cMenuSetupWetter(&setup, this);
}

bool cPluginWetter::SetupParse(const char *Name, const char *Value) {
	if (!strcasecmp(Name, "StationId")) {
		strncpy(setup.stationId, Value, 8);
	} else {
		return false;
	}

	return true;
}



cMenuSetupWetter::cMenuSetupWetter(cWetterSetup *setup, cPluginWetter *plugin) {
	this->setup = setup;
	this->plugin = plugin;

	Add(new cMenuEditStrItem(tr("Station Id"), setup->stationId, 9, ALLOWED_STATION_CHARS));
	
}

void cMenuSetupWetter::Store(void) {
	for(char *ptr = setup->stationId; *ptr; ++ptr) {
		if (islower(*ptr)) {
			*ptr = toupper(*ptr);
		}
	}

	SetupStore("StationId", setup->stationId);
	}

VDRPLUGINCREATOR(cPluginWetter); // Don't touch this!
