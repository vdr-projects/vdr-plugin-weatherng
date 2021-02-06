#ifndef WEATHER_OSD_H
#define WEATHER_OSD_H
#include <vdr/plugin.h>
#include "setup.h"
class cWetterOsd : public cOsdObject {
private:
  cOsd *osd;
  int x;
  int y;
  tColor color;
  cWetterSetup *setup;
public:
  cWetterOsd(cWetterSetup *setup);
  ~cWetterOsd();
  virtual void Show(void);
  };

#endif
