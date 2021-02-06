#ifndef __OSDWEATHER_H
#define __OSDWEATHER_H

#include <string>
#include <vdr/thread.h>
#include <vdr/plugin.h>
#include <vdr/status.h>
//#include <vdr/thread.h>
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
  bool radarcolor;
  bool fontsize;
  bool Offlinemode;

//protected:
//  virtual void Action(void);

public:
  cWetterOsd(void);
  ~cWetterOsd();
  void Satelite(void);
  virtual void Show(void);
  virtual eOSState ProcessKey(eKeys Key);
  };

#endif //__OSDWEATHER_H
