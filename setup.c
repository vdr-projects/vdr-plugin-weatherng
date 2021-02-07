#include "setup.h"

cWetterSetup wetterSetup;

cWetterSetup::cWetterSetup(void)
{
	w_update       = 0;
	w_left         = 150;
	w_top          = 120;
	w_width        = 420;
	w_height       = 336;
        w_hiquality    = 0;
	w_osdoffset_x  = 0;
	w_osdoffset_y  = 0;
        w_dither       = 1;
	w_alpha        = 210;

        strncpy(w_id1,"GMXX0049",sizeof(w_id1));        
        strncpy(w_id2,"UKXX0085",sizeof(w_id2));        
        strncpy(w_id3,"SFXX0023",sizeof(w_id3));        

        strncpy(w_id1_name,"Hamburg",     sizeof(w_id1_name));        
        strncpy(w_id2_name,"London",      sizeof(w_id2_name));        
        strncpy(w_id3_name,"Johannesburg",sizeof(w_id3_name));        

#ifdef USE_MAGICK
	w_treedepth    = 4;
#endif


}
