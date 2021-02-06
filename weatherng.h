#ifndef __WEATHERNG_H
#define __WEATHERNG_H

#include <vdr/plugin.h>
#include "i18n.h"
//#include "parser.h"
#include <string>
#include <vector>
#include <vdr/osd.h>
#include "OsdWeather.h"
#include "i18n.h"

static const char *VERSION        = "0.0.5";
static const char *DESCRIPTION    = "Weather forecast";
static const char *MAINMENUENTRY  = "Weather-NG";

static const char *ALLOWED_STATION_CHARS 	= "abcdefghijklmnopqrstuvwxyz0123456789";
//static const char hex[] = { "0123456789abcdef" };

class cPluginWetter : public cPlugin {
private:
static	std::vector<std::string> honz; 

 // Add any member variables or functions you may need here.
public:
  cPluginWetter(void);
  virtual ~cPluginWetter();
  virtual const char *Version(void) { return VERSION; }
  virtual const char *Description(void) { return tr(DESCRIPTION); }
  virtual const char *CommandLineHelp(void);
  virtual bool ProcessArgs(int argc, char *argv[]);
  virtual bool Initialize(void);
  virtual bool Start(void);
  virtual void Housekeeping(void);
  virtual const char *MainMenuEntry(void) { return tr(MAINMENUENTRY); }
  virtual cOsdObject *MainMenuAction(void);
  virtual cMenuSetupPage *SetupMenu(void);
  virtual bool SetupParse(const char *Name, const char *Value);
  };

class cMenuWetterSetup : public cMenuSetupPage {
	private:
                const char *themes[eWetterThemeMaxNumber];
		virtual void Setup(void);
	protected:
	        virtual eOSState ProcessKey(eKeys Key);
		virtual void Store(void);
	public:
  		cMenuWetterSetup(void);
};

#endif // __WEATHERNG_H
