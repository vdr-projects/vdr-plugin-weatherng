
#include <vdr/plugin.h>
#include "i18n.h"
#include "parser.h"
#include <string>
#include <vector>
#include <vdr/osd.h>
#include "OsdWeather.h"
#include "i18n.h"

static const char *VERSION        = "0.0.3";
static const char *DESCRIPTION    = "Weather forecast";
static const char *MAINMENUENTRY  = "Weather-NG";

static const char *ALLOWED_STATION_CHARS 	= "abcdefghijklmnopqrstuvwxyz0123456789";

class cPluginWetter : public cPlugin {
private:
static	std::vector<std::string> honz; 
cWetterSetup setup;
 // Add any member variables or functions you may need here.
public:
  cPluginWetter(void);
  virtual ~cPluginWetter();
  virtual const char *Version(void) { return VERSION; }
  virtual const char *Description(void) { return DESCRIPTION; }
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

class cMenuSetupWetter : public cMenuSetupPage {
	private:
		cWetterSetup *setup;
		cPluginWetter *plugin;
	protected:
		virtual void Store(void);
	public:
  		cMenuSetupWetter(cWetterSetup *setup, cPluginWetter *plugin);
};
