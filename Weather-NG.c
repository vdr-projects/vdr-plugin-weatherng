/*
 * wetter.c: A plugin for the Video Disk Recorder
 *
 * See the README file for copyright information and how to reach the author.
 *
 * $Id: wetter.c,v 1.3 2005/01/23 00:56:49 beejay Exp $
 */

#include <string>
#include "getdata.h"
#include "Weather-NG.h" 
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
// Return a string that describes all known command line options.
  return "-D	Where can i save the Tempfiles? (Target Directory)		-S	Where can I find the SatDownload- script?)";
}

bool cPluginWetter::ProcessArgs(int argc, char *argv[])
{
asprintf(&ScriptTarget,"%s/SatDownload",cPlugin::ConfigDirectory("weatherng"));
asprintf(&DestinationDir,"%s",cPlugin::ConfigDirectory("weatherng"));
    int counter=0;
    for(int i=0;i<argc;i++)
    {
	if (argv[i])
	{
	    if (strcmp(argv[i],"-D")==0)
	    {
		printf("argv[i+1]= %s\n",argv[i+1]);	
	    
		if (argv[i+1]){
		    counter++;
		    printf("-D Set to: %s\n",argv[i+1]);
		    DestinationDir=argv[i+1];
		}else
		{
		    printf("Missing -D Option!\n");
		    return false;
		}
	    }
	    if (strcmp(argv[i],"-S")==0)
	    {
		if(argv[i+1]){
		counter++;
		printf("-S Set to: %s\n",argv[i+1]);
		    ScriptTarget=argv[i+1];
		}else
		{
		    printf("Missing -S option!\n");
		}
	    }
	}
    }
if (counter<2){
    printf("Weater-NG: Too few Parameters!\n");
    return false;
}
  return true;
}

bool cPluginWetter::Initialize(void)
{
    char *execute;
    
    #ifdef DEBUG 
        asprintf(&execute,"%s %s",ScriptTarget,DestinationDir);
    #else
        asprintf(&execute,"%s %s 2>/dev/null",ScriptTarget,DestinationDir);
    #endif
    printf("WEATHER EXECUTE : %s\n",execute);
    int ret = system(execute);
    printf("%d\n", WEXITSTATUS(ret));
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
/*std::string plugindir;
plugindir = cPlugin::ConfigDirectory("weatherng");
if (!Get("http://image.weather.com/images/sat/germany_sat_720x486.jpg",plugindir +"/sat.jpg")){
	printf("New SAtelite Picture downloaded\n");
}*/
// Perform any cleanup or other regular tasks.
}

cOsdObject *cPluginWetter::MainMenuAction(void)
{
// Perform the action when selected from the main VDR menu.
    if(setup.stationId){
	return new cWetterOsd(&setup);
    }else
    {
        printf("Keins StationsId vorhanden!\n");
    }
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
