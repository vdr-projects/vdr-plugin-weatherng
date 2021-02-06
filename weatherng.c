/*
 * wetter.c: A plugin for the Video Disk Recorder
 *
 * See the README file for copyright information and how to reach the author.
 *
 * $Id: wetter.c,v 1.3 2005/01/23 00:56:49 beejay Exp $
 */

#include <getopt.h>
#include "setup.h"
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
			  "  -I DIR    --images=DIR  DIR to imagefolder of plugin e.g. /etc/plugins/weatherng\n"
			  "  -S DIR    --script=DIR  DIR to weatherng.sh  e.g. /usr/local/bin\n");
  return help_str;         
}

bool cPluginWetter::ProcessArgs(int argc, char *argv[]) {
  static struct option long_options[] = {
       { "data",	required_argument, NULL, 'D' },
       { "images",	required_argument, NULL, 'I' },
       { "script",	required_argument, NULL, 'S' },
       { NULL }
     };
     
  int c, option_index = 0;
  while ((c = getopt_long(argc, argv, "D:I:S:", long_options, &option_index)) != -1) {
          switch (c) {
	  case 'D': DataDir        = optarg; break;
	  case 'I': ImageDir       = optarg; break;
	  case 'S': ScriptDir      = optarg; break;
	  default: return false;
	  }		    
	}
  return true;
}

bool cPluginWetter::Initialize(void)
{
  if (DataDir == NULL) {
          DataDir = strdup(ConfigDirectory(PLUGIN_NAME_I18N));	         
          esyslog("%s: Parameter '-D' not set.\n"
	          " Set default value : %s\n", PLUGIN_NAME_I18N, DataDir);
  }
  if (ImageDir == NULL) {
          ImageDir = strdup(ConfigDirectory(PLUGIN_NAME_I18N));	         
          esyslog("%s: Parameter '-I' not set.\n"
	          " Set default value : %s\n", PLUGIN_NAME_I18N, ImageDir);
  }
  if (ScriptDir == NULL) {
          ScriptDir = strdup(ConfigDirectory(PLUGIN_NAME_I18N));	         
          esyslog("%s: Parameter '-S' not set.\n"
	          " Set default value : %s\n", PLUGIN_NAME_I18N, ImageDir);
  }
#ifdef HAVE_MAGICK
  dsyslog("DEBUG : %s: defined HAVE_MAGICK\n",PLUGIN_NAME_I18N);  
#endif

#ifdef HAVE_IMLIB2
  dsyslog("DEBUG : %s: defined HAVE_IMLIB2\n",PLUGIN_NAME_I18N);  
#endif

#ifdef HAVE_4MB
  dsyslog("DEBUG : %s: defined HAVE_4MB\n",PLUGIN_NAME_I18N);  
#endif    
  
  return true;
}

bool cPluginWetter::Start(void)
{
  if ( wetterSetup.w_update == true ) {
     FILE *cmd;
     char *buffer;
	
	asprintf(&buffer, "%s%s &", ScriptDir, "/weatherng.sh");
	cmd = popen(buffer, "r");
	dsyslog("DEBUG : %s: Executing as autoupdate: '%s'", PLUGIN_NAME_I18N, buffer);
	free(buffer);
	pclose(cmd);
  }	
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
  if      (!strcasecmp(Name, "AutoUpdate"))    wetterSetup.w_update      = atoi(Value);
  else if (!strcasecmp(Name, "RadarLeft"))     wetterSetup.w_left        = atoi(Value);
  else if (!strcasecmp(Name, "RadarTop"))      wetterSetup.w_top         = atoi(Value);
  else if (!strcasecmp(Name, "RadarWidth"))    wetterSetup.w_width       = atoi(Value);
  else if (!strcasecmp(Name, "RadarHeight"))   wetterSetup.w_height      = atoi(Value);
#ifdef HAVE_4MB
  else if (!strcasecmp(Name, "HiColor"))       wetterSetup.w_hicolor     = atoi(Value);
#endif
  else if (!strcasecmp(Name, "Fontsize"))      wetterSetup.w_fontsize    = atoi(Value);
  else if (!strcasecmp(Name, "RoundedCorner")) wetterSetup.w_corner      = atoi(Value);
  else if (!strcasecmp(Name, "Inverted"))      wetterSetup.w_inverted    = atoi(Value);
  else if (!strcasecmp(Name, "OSDOffsetX"))    wetterSetup.w_osdoffset_x = atoi(Value);
  else if (!strcasecmp(Name, "OSDOffsetY"))    wetterSetup.w_osdoffset_y = atoi(Value);
  else if (!strcasecmp(Name, "Theme"))         wetterSetup.w_theme       = atoi(Value);
  else if (!strcasecmp(Name, "ImgDither"))     wetterSetup.w_dither      = atoi(Value);
  else if (!strcasecmp(Name, "ImgAlpha"))      wetterSetup.w_alpha       = atoi(Value);
#ifdef HAVE_MAGICK
  else if (!strcasecmp(Name, "ImgMaxCacheFill"))  wetterSetup.w_maxcachefill= atoi(Value);
  else if (!strcasecmp(Name, "ImgTreeDepth"))     wetterSetup.w_treedepth   = atoi(Value);
#endif
  else 
       return false;

  return true;  
}


cMenuWetterSetup::cMenuWetterSetup(void)
{
  themes[eWetterThemeClassic]          = tr("Classic");
  themes[eWetterThemeenElchi]          = tr("enElchi");
  themes[eWetterThemeMoronimo]         = tr("Moronimo");
  themes[eWetterThemeDeepBlue]         = tr("DeepBlue");
  themes[eWetterThemeEnigma]           = tr("Enigma");
  themes[eWetterThemeSilverGreen]      = tr("SilverGreen");
  themes[eWetterThemeMoronimoMKII]     = tr("MoronimoMKII");
  
  
  Add(new cMenuEditBoolItem(tr("Update data on start"),          &wetterSetup.w_update));
  Add(new cMenuEditIntItem(tr("Radarbitmap left"),                  &wetterSetup.w_left, 1, 200));
  Add(new cMenuEditIntItem(tr("Radarbitmap top"),                   &wetterSetup.w_top, 1, 200));
#ifdef HAVE_4MB
// 4 MB FF-CARD
  Add(new cMenuEditIntItem(tr("Radarbitmap width"),                 &wetterSetup.w_width, 100, 672));
  Add(new cMenuEditIntItem(tr("Radarbitmap height"),                &wetterSetup.w_height, 100, 567));
  Add(new cMenuEditBoolItem(tr("Use 256 colors for radarmaps"),      &wetterSetup.w_hicolor));
#else
// 2 MB FF-CARD
  Add(new cMenuEditIntItem(tr("Radarbitmap width"),                 &wetterSetup.w_width, 100, 450));
  Add(new cMenuEditIntItem(tr("Radarbitmap height"),                &wetterSetup.w_height, 100, 400));
#endif
  Add(new cMenuEditBoolItem(tr("Use small fonts"),                  &wetterSetup.w_fontsize));
  Add(new cMenuEditBoolItem(tr("Corner rounded"),                   &wetterSetup.w_corner));
  Add(new cMenuEditBoolItem(tr("Symbols inverted"),                 &wetterSetup.w_inverted));
  Add(new cMenuEditIntItem(tr("Offset horizontal"),                 &wetterSetup.w_osdoffset_x, -50, 50));
  Add(new cMenuEditIntItem(tr("Offset vertical"),                   &wetterSetup.w_osdoffset_y, -50, 0));
  Add(new cMenuEditIntItem(tr("Transparency for images"),           &wetterSetup.w_alpha, 1, 255));
  Add(new cMenuEditBoolItem(tr("Use dithering for images"),         &wetterSetup.w_dither));
#ifdef HAVE_MAGICK
  Add(new cMenuEditIntItem(tr("Image quality >=slower"),            &wetterSetup.w_treedepth, 1, 6));
  Add(new cMenuEditIntItem(tr("Imagecache in MByte"),               &wetterSetup.w_maxcachefill, 1, 5));
#endif
  Add(new cMenuEditStraItem(tr("Theme"),                            &wetterSetup.w_theme, eWetterThemeMaxNumber, themes));
}    


void cMenuWetterSetup::Store(void)
{
  SetupStore("AutoUpdate",	wetterSetup.w_update);
  SetupStore("RadarLeft",       wetterSetup.w_left);
  SetupStore("RadarTop",        wetterSetup.w_top);
  SetupStore("RadarWidth",      wetterSetup.w_width);
  SetupStore("RadarHeight",     wetterSetup.w_height);
#ifdef HAVE_4MB
  SetupStore("HiColor",		wetterSetup.w_hicolor);
#endif
  SetupStore("Fontsize",        wetterSetup.w_fontsize);
  SetupStore("RoundedCorner",   wetterSetup.w_corner);	
  SetupStore("Inverted",        wetterSetup.w_inverted);
  SetupStore("OSDOffsetX",      wetterSetup.w_osdoffset_x);
  SetupStore("OSDOffsetY",      wetterSetup.w_osdoffset_y);
  SetupStore("Theme",           wetterSetup.w_theme);
  SetupStore("ImgAlpha",        wetterSetup.w_alpha);
  SetupStore("ImgDither",       wetterSetup.w_dither);
#ifdef HAVE_MAGICK
  SetupStore("ImgMaxCacheFill",    wetterSetup.w_maxcachefill);
  SetupStore("ImgTreeDepth",       wetterSetup.w_treedepth);
#endif
}

        
VDRPLUGINCREATOR(cPluginWetter); // Don't touch this!
