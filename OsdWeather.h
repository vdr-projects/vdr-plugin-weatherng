#ifndef WEATHER_OSD_H
#define WEATHER_OSD_H
#include <string>
#include <vdr/plugin.h>
#include "setup.h"

class cWetterOsd : public cOsdObject {
private:
  std::string code,file;
  cOsd *osd;
  int day;
  int Radar_left;
  int Radar_top;
  int Radar_width;
  int Radar_height;
  bool fontsize;
  tColor color;
  cWetterSetup *setup;
public:
  cWetterOsd(cWetterSetup *setup);
  ~cWetterOsd();
  void Satelite(void);
  virtual void Show(void);
  virtual eOSState ProcessKey(eKeys Key);
  };

#endif
