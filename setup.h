

//#include "i18n.h"
#include <ctype.h>
#include <climits>
#include <vector>
#include <string>
#include <vdr/plugin.h>


class cWetterSetup {
	public:
		char stationId[9];
		int w_left;
		int w_top;
		int w_width;
		int w_height;
		cWetterSetup(void);
		int w_fontsize;
};

//extern char *DestinationDir;
