#include <vector>
#include <string>
#include <vdr/plugin.h>
#include <vdr/osd.h>
#include <vdr/config.h>
#include "OsdWeather.h"
#include "parsing.h"
#include "bitmap.h"
#include "vars.h"
#include "i18n.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>

#include "symbols/left1.xpm"
#include "symbols/right1.xpm"
#include "symbols/day1.xpm"
#include "symbols/night1.xpm"
#include "symbols/left.xpm"
#include "symbols/right.xpm"
#include "symbols/day.xpm"
#include "symbols/night.xpm"



#define TRUE 1
#define FALSE 0
#define BOOL int
#define OSDWIDTH 600
#define OSDHEIGHT (Setup.OSDHeight + wetterSetup.w_osdoffset_y)
#define CELLWIDTH ((OSDWIDTH / 4)-1)	


cBitmap cWetterOsd::bmLeft(left1_xpm);
cBitmap cWetterOsd::bmRight(right1_xpm);
cBitmap cWetterOsd::bmDay(day1_xpm);
cBitmap cWetterOsd::bmNight(night1_xpm);
cBitmap cWetterOsd::bmLeft_inv(left_xpm);
cBitmap cWetterOsd::bmRight_inv(right_xpm);
cBitmap cWetterOsd::bmDay_inv(day_xpm);
cBitmap cWetterOsd::bmNight_inv(night_xpm);



char *Radarmap;
//char *data_file;

const int MAX_LOGO_COLORS=16;
const cFont *font;

int part=0;
int colordepth  =  16;
int areadepth   =   4;

bool IsSatelite,satelite;

static int if_exist_file( const char *filename) {
  FILE *f = fopen( filename, "r" );
  if( f != NULL ) {
        fclose(f);
	return TRUE;
      }
  else
      return FALSE;
}            	


cWetterOsd::cWetterOsd(void)
{
  osd           = NULL;
  day           = 1;
  IsSatelite    = false;
  Radar_left    = wetterSetup.w_left;
  Radar_top     = wetterSetup.w_top;
  Radar_width   = wetterSetup.w_width;
  Radar_height  = wetterSetup.w_height;
#ifdef HAVE_4MB
  hicolor       = wetterSetup.w_hicolor;
#endif
  row_y         = 30;
  fontsize      = wetterSetup.w_fontsize;
  inverted      = wetterSetup.w_inverted;
  corner        = wetterSetup.w_corner;
  data_file     = "/data1.xml";
}

cWetterOsd::~cWetterOsd(void)
{
  cWeatherBitmap::FlushCache();     
  if (osd)
     delete osd;

}


eOSState cWetterOsd::ProcessKey(eKeys Key)
{
  eOSState state = cOsdObject::ProcessKey(Key);
  if (state == osUnknown) {
     switch (Key & ~k_Repeat) {
       case kOk:
		if(IsSatelite==true)
		    {
        		IsSatelite = false; 
			cWetterOsd::Show();
		    }
		else
		    {
			file = ImageDir;
			file = file  + "/images/wait.png";

#ifdef HAVE_4MB
			if (hicolor==false) {
			    colordepth  =  16;
			    }
			else {
			    colordepth  = 256;
			}
#else
			colordepth = 16;
#endif               


			cWeatherBitmap *bmp;
		        if((bmp = cWeatherBitmap::Load(file, wetterSetup.w_alpha, 196, 196 , colordepth )) != NULL) {
				// Fix for color
			   osd->DrawRectangle(CELLWIDTH-1, 0, 3 * CELLWIDTH, (OSDHEIGHT - (7 * row_y)) -11, clrTransparent);
			        // top
			   osd->DrawRectangle(((OSDWIDTH-196)/2)-4,(((OSDHEIGHT/2)-196)/2)-4,((OSDWIDTH-196)/2)+200,(((OSDHEIGHT/2)-196)/2), wetterTheme[wetterSetup.w_theme].clrBgBorder);
// bottom (something to fix)
			    osd->DrawRectangle(((OSDWIDTH-196)/2)-4,(((OSDHEIGHT/2)-196)/2)+128,((OSDWIDTH-196)/2)+200,(((OSDHEIGHT/2)-196)/2)+200, wetterTheme[wetterSetup.w_theme].clrBgBorder);
				// left
			    osd->DrawRectangle(((OSDWIDTH-196)/2)-4,(((OSDHEIGHT/2)-196)/2),((OSDWIDTH-196)/2),(((OSDHEIGHT/2)-196)/2)+196, wetterTheme[wetterSetup.w_theme].clrBgBorder);
				// right
			    osd->DrawRectangle(((OSDWIDTH-196)/2)+196,(((OSDHEIGHT/2)-196)/2),((OSDWIDTH-196)/2)+200,(((OSDHEIGHT/2)-196)/2)+196, wetterTheme[wetterSetup.w_theme].clrBgBorder);
				// Picture
	                    osd->DrawBitmap((OSDWIDTH-196) /2,((OSDHEIGHT/2)-196)/2 , bmp->Get(),clrTransparent,clrTransparent);
//		            delete b;
			 }
			 osd->Flush();
			 cWetterOsd::GetData();
			 cWetterOsd::Show();
		    }
		break;
       case kLeft: 
			day--;
			if (day<1){
			day=1;
			}
			cWetterOsd::Show();  
		break;
       case kUp:
                	part++;
                	if (part>1){
                	part=1;
                	}
                	cWetterOsd::Show();
		break;
       case kDown:
			part--;
                	if (part<0){
                	part=0;
                	}
                	cWetterOsd::Show();
		break;
       case kRight: 
                	day++;
              		if (day>10){
                	day=10;
                	}
			cWetterOsd::Show();
		break;
       case kBack:
		if(IsSatelite==true){
			IsSatelite=false;	
			cWetterOsd::Show();
		}
		else{
		return osEnd;
		}
		break;
	case k1:
		if(IsSatelite==true){
        		IsSatelite = false; }
			data_file  = "/data1.xml";
			cWetterOsd::Show();
		break;
	case k2:
		if(IsSatelite==true){
        		IsSatelite = false; }
			data_file  = "/data2.xml";
			cWetterOsd::Show();
		break;
	case k3:
		if(IsSatelite==true){
        		IsSatelite =false; }
			data_file  = "/data3.xml";
			cWetterOsd::Show();
		break; 
	case k5:
			IsSatelite=true;
			Radarmap="/pic0";
			cWetterOsd::Satelite();
		break;
	case kRed:
			IsSatelite=true;
			Radarmap="/pic1";
			cWetterOsd::Satelite();
		break;
	case kGreen:
			IsSatelite=true;
			Radarmap="/pic2";
			cWetterOsd::Satelite();
		break;
	case kYellow:
			IsSatelite=true;
			Radarmap="/pic3";
			cWetterOsd::Satelite();
		break;
	case kBlue:
			IsSatelite=true;
			Radarmap="/pic4";
			cWetterOsd::Satelite();
		break;
	default: return state;
       }
     state = osContinue;
     }
  return state;
}


void cWetterOsd::GetData(void)
{
 FILE *script;
 char *buffer;

 asprintf(&buffer, "%s%s",ScriptDir,"/weatherng.sh");
 
 script = popen(buffer, "r");
 dsyslog("DEBUG : weatherng: Executing GetData :'%s'", buffer);

 free(buffer);
 pclose(script);

 cWeatherBitmap::FlushCache();
}


int cWetterOsd::Satelite(void)
{
delete osd;
osd = cOsdProvider::NewOsd(Radar_left ,Radar_top );
satelite=false;
if (osd) {


#ifdef HAVE_4MB
		if (hicolor==false) {
		    areadepth   =   4;
		    colordepth  =  16;
		    }
		else {
		    areadepth   =   8;
		    colordepth  = 255;
		}
#else
		areadepth  = 4;
		colordepth = 16;
#endif               
//                esyslog("left=%i top=%i witdh=%i height=%i",Radar_left,Radar_top,Radar_width,Radar_height);

		tArea Area = {1,1,Radar_width,Radar_height,areadepth};
		eOsdError result = osd->CanHandleAreas(&Area, 1);
		if (result == oeOk) {
			osd->SetAreas(&Area, 1);
		   }
	        else
                   esyslog("ERROR : %s (satelite): OSD open failed! Can't handle areas (%d)\n",PLUGIN_NAME_I18N,result); 

		std::string sat_dir;
		sat_dir = DataDir;
		sat_dir = sat_dir  + Radarmap;

		BOOL ex = if_exist_file(sat_dir.c_str());

		if ( ex ) {

            			cWeatherBitmap *bmp;
	        	        if((bmp = cWeatherBitmap::Load(sat_dir.c_str(), wetterSetup.w_alpha, Radar_height -2, Radar_width -2 , colordepth )) != NULL) {
	                          osd->DrawBitmap(2, 2, bmp->Get(), clrTransparent, clrTransparent);
    				}
//				delete b;

				// debug

				  dsyslog("DEBUG : weatherng: SATELITE\n");
#ifdef HAVE_IMLIB2
				  dsyslog("DEBUG : weatherng: USE IMLIB2\n");;
#endif
#ifdef HAVE_MAGICK
				  dsyslog("DEBUG : weatherng: USE IMAGEMAGICK\n");;
#endif
				  dsyslog("DEBUG : weatherng: USE XPM\n");;
				  dsyslog("DEBUG : weatherng: SCRIPTDIR  = %s\n", ScriptDir);
				  dsyslog("DEBUG : weatherng: IMAGEDIR   = %s\n", ImageDir);
				  dsyslog("DEBUG : weatherng: DATADIR   = %s\n", DataDir);
#ifdef HAVE_4MB
				  dsyslog("DEBUG : weatherng: 256 COLORS ENABLED  = %i\n", hicolor);
#endif
				  dsyslog("DEBUG : weatherng: AREADEPTH   = %i\n", areadepth);
				  dsyslog("DEBUG : weatherng: COLORDEPTH = %i\n", colordepth);
				  dsyslog("DEBUG : weatherng: RADAR_LEFT = %i\n", Radar_left);
				  dsyslog("DEBUG : weatherng: RADAR_TOP  = %i\n", Radar_top);
				  dsyslog("DEBUG : weatherng: RADAR_WIDTH  = %i\n", Radar_width);
				  dsyslog("DEBUG : weatherng: RADAR_HEIGHT  = %i\n", Radar_height);
				
			  }	

		else {	
				perror(sat_dir.c_str());

				sat_dir = ImageDir;
    				sat_dir = sat_dir + "/images/nopic.png";
				sat_dir = ImageDir;
    				sat_dir = sat_dir + "/images/nopic.png";

            			cWeatherBitmap *bmp;
	        	        if((bmp = cWeatherBitmap::Load(sat_dir.c_str(), wetterSetup.w_alpha, Radar_height -2, Radar_width -2 , colordepth )) != NULL) {
		                  osd->DrawBitmap(2, 2, bmp->Get(), clrTransparent, clrTransparent);
				}			
//				delete b;
				
		     }
               osd->Flush();
	}
  return EXIT_SUCCESS;

}

void cWetterOsd::Show(void)
{
  IsSatelite=false;
  dsyslog("DEBUG : weatherng: Part: %i\n",part);

  delete osd;
  osd = cOsdProvider::NewOsd(((Setup.OSDWidth -OSDWIDTH) / 2) + Setup.OSDLeft + wetterSetup.w_osdoffset_x, ((Setup.OSDHeight - OSDHEIGHT) / 2) + Setup.OSDTop);
  if (osd) {

	std::string ausgabe;
        std::string sat_file;
	sat_file = DataDir;
	sat_file = sat_file + data_file;


	        if (!fontsize) {
		    font     =    cFont::GetFont(fontOsd);
		    row_y =    30;
		    }
		  else {
		    font = cFont::GetFont(fontSml);
		    row_y =    25;
	            }

/*
#ifdef HAVE_4MB
		if (hicolor==false) {
		    colordepth  =  16;
		    areadepth   =   4;
		}
		else {
		    colordepth  = 255;
		    areadepth   =   8;
		}
#else
		colordepth  =  16;
		areadepth   =   4;

#endif               
*/
		colordepth  =  16;
		areadepth   =   4;

		tArea Area[] = {
				{CELLWIDTH-1, 0, 3 * CELLWIDTH, (OSDHEIGHT - (7 * row_y)) -11, areadepth},
				{0, OSDHEIGHT - ((7 * row_y) +10), OSDWIDTH -1, OSDHEIGHT -((6 * row_y) +10), 2},
				{0, OSDHEIGHT - ((6 * row_y) +9), OSDWIDTH -1, OSDHEIGHT -(row_y +1), 2},
				{0, OSDHEIGHT - row_y, OSDWIDTH -1, OSDHEIGHT, 2},
				};

	        eOsdError result = osd->CanHandleAreas(Area, sizeof(Area) / sizeof(tArea));
                if (result == oeOk) {
	           osd->SetAreas(Area, sizeof(Area) / sizeof(tArea));
		   }
	        else	  
                   esyslog("ERROR : %s: OSD open failed! Can't handle areas (%d)\n",PLUGIN_NAME_I18N, result);

		// icon
		osd->DrawRectangle(CELLWIDTH-1, 0, 3 * CELLWIDTH, (OSDHEIGHT / 2) - 1, clrTransparent);

		// Info top
     		osd->DrawRectangle(0, OSDHEIGHT - ((7 * row_y)+10), OSDWIDTH, OSDHEIGHT - ((6 * row_y)+10), wetterTheme[wetterSetup.w_theme].clrBgBorder);
                // Mainfield left
		osd->DrawRectangle(3, OSDHEIGHT - ((6 * row_y) + 9), (OSDWIDTH / 2) -4 , OSDHEIGHT - row_y, wetterTheme[wetterSetup.w_theme].clrBackground);
		// Mainfield right
		osd->DrawRectangle((OSDWIDTH / 2)+2, OSDHEIGHT - ((6 * row_y) + 9), OSDWIDTH -3, OSDHEIGHT - row_y,  wetterTheme[wetterSetup.w_theme].clrBackground);
		
                // BORDER
                // left OK    middle OK    right OK    horiz. OK
      		osd->DrawRectangle(0, (OSDHEIGHT - ((6 * row_y)+10))+1, 3, OSDHEIGHT-row_y, wetterTheme[wetterSetup.w_theme].clrBgBorder);
      		osd->DrawRectangle((OSDWIDTH / 2) -3, (OSDHEIGHT - ((6 * row_y)+10))+1, (OSDWIDTH / 2)+1,OSDHEIGHT-(2 * row_y), wetterTheme[wetterSetup.w_theme].clrBgBorder);
      		osd->DrawRectangle(OSDWIDTH -4, (OSDHEIGHT - ((6 * row_y)+10))+1, OSDWIDTH, OSDHEIGHT-row_y, wetterTheme[wetterSetup.w_theme].clrBgBorder);
      		osd->DrawRectangle(4, (OSDHEIGHT - (2 * row_y))-1, OSDWIDTH -5, (OSDHEIGHT - (2 *row_y)) +1, wetterTheme[wetterSetup.w_theme].clrBgBorder);

                // Info bottom
      		osd->DrawRectangle(0, OSDHEIGHT - row_y, OSDWIDTH, OSDHEIGHT, wetterTheme[wetterSetup.w_theme].clrBgBorder);


	     BOOL ex = if_exist_file(sat_file.c_str());

             if (ex) {
			dsyslog("DEBUG : weatherng: Parse %s !\n", sat_file.c_str());

           		cxmlParse parser;

			dsyslog("DEBUG : weatherng: day: %i\n",day);	

			parser.xmlParse((int) day, DataDir, data_file, part);

			file = ImageDir;
			file = file  + "/images/";
			file = file + parser.icon.c_str();
			file = file + ".png";

    			cWeatherBitmap *bmp;
        	        if((bmp = cWeatherBitmap::Load(file.c_str(), wetterSetup.w_alpha, 196, 196, colordepth )) != NULL) {
				// Fix for color
				osd->DrawRectangle(CELLWIDTH-1, 0, 3 * CELLWIDTH, (OSDHEIGHT - (7 * row_y)) -11, clrTransparent);
			        // top
				osd->DrawRectangle(((OSDWIDTH-196)/2)-4,(((OSDHEIGHT/2)-196)/2)-4,((OSDWIDTH-196)/2)+200,(((OSDHEIGHT/2)-196)/2), wetterTheme[wetterSetup.w_theme].clrBgBorder);
// bottom (something to fix)
				osd->DrawRectangle(((OSDWIDTH-196)/2)-4,(((OSDHEIGHT/2)-196)/2)+128,((OSDWIDTH-196)/2)+200,(((OSDHEIGHT/2)-196)/2)+200, wetterTheme[wetterSetup.w_theme].clrBgBorder);
				// left
				osd->DrawRectangle(((OSDWIDTH-196)/2)-4,(((OSDHEIGHT/2)-196)/2),((OSDWIDTH-196)/2),(((OSDHEIGHT/2)-196)/2)+196, wetterTheme[wetterSetup.w_theme].clrBgBorder);
				// right
				osd->DrawRectangle(((OSDWIDTH-196)/2)+196,(((OSDHEIGHT/2)-196)/2),((OSDWIDTH-196)/2)+200,(((OSDHEIGHT/2)-196)/2)+196, wetterTheme[wetterSetup.w_theme].clrBgBorder);
				// Picture

	                        osd->DrawBitmap((OSDWIDTH-196) /2,((OSDHEIGHT/2)-196)/2 , bmp->Get(),clrTransparent,clrTransparent);
	                }

			dsyslog("DEBUG : weatherng: Information about: %s\n",parser.ort.c_str());

			  // left Side
			  ausgabe = tr("Weather for: ");
			  ausgabe = ausgabe + parser.ort.c_str() + "  /  ";
			  ausgabe = ausgabe + tr(parser.dayname.c_str());
			  ausgabe = ausgabe + "  (" + parser.date + ")";
			osd->DrawText(16, (OSDHEIGHT - ((7 * row_y)+10)) + 1,ausgabe.c_str(), wetterTheme[wetterSetup.w_theme].clrFgBorder,clrTransparent,font,OSDWIDTH -16,20,taCenter);

			osd->DrawText(8, OSDHEIGHT - ((6 * row_y)+5), tr("High temperature: "), wetterTheme[wetterSetup.w_theme].clrFgHiTemp,clrTransparent,font);
		          ausgabe = parser.hi.c_str();
		          ausgabe = ausgabe + " ";
			  ausgabe = ausgabe + parser.celsius.c_str();
			osd->DrawText(CELLWIDTH - 8, OSDHEIGHT - ((6 * row_y)+5), ausgabe.c_str(), wetterTheme[wetterSetup.w_theme].clrFgHiTemp,clrTransparent,font,CELLWIDTH,20,taRight);
		
			osd->DrawText(8, OSDHEIGHT - ((5 * row_y)+5), tr("Low temperature: "), wetterTheme[wetterSetup.w_theme].clrFgText,clrTransparent,font);
	                  ausgabe = parser.low.c_str();
	                  ausgabe = ausgabe + " ";
			  ausgabe = ausgabe + parser.celsius.c_str();
	                osd->DrawText(CELLWIDTH - 8, OSDHEIGHT - ((5 * row_y)+5), ausgabe.c_str(), wetterTheme[wetterSetup.w_theme].clrFgText,clrTransparent,font,CELLWIDTH,20,taRight);

	 		osd->DrawText(8, OSDHEIGHT - ((4 * row_y)+5), tr("Sunrise: "), wetterTheme[wetterSetup.w_theme].clrFgText,clrTransparent,font);
	                  ausgabe = parser.sunrise.c_str();
	                  ausgabe = ausgabe;
	                osd->DrawText(CELLWIDTH - 8, OSDHEIGHT - ((4 * row_y)+5), ausgabe.c_str(), wetterTheme[wetterSetup.w_theme].clrFgText,clrTransparent,font,CELLWIDTH,20,taRight);

			osd->DrawText(8, OSDHEIGHT - ((3 * row_y)+5), tr("Sunset: "), wetterTheme[wetterSetup.w_theme].clrFgText,clrTransparent,font);
	                  ausgabe = parser.sunset.c_str();
	                  ausgabe = ausgabe;
	                osd->DrawText(CELLWIDTH - 8, OSDHEIGHT - ((3 * row_y)+5), ausgabe.c_str(), wetterTheme[wetterSetup.w_theme].clrFgText,clrTransparent,font,CELLWIDTH,20,taRight);

	                  ausgabe = parser.wetter;
	                osd->DrawText(8, (OSDHEIGHT - (2 * row_y))+2,tr(ausgabe.c_str()), wetterTheme[wetterSetup.w_theme].clrFgText,clrTransparent,font, OSDWIDTH-8, 20, taCenter);

			//right Side
			osd->DrawText((OSDWIDTH / 2)+8, OSDHEIGHT - ((6 * row_y)+5), tr("Wind comes from: "), wetterTheme[wetterSetup.w_theme].clrFgText,clrTransparent,font);
			  ausgabe = parser.winddir.c_str();
			osd->DrawText(OSDWIDTH - CELLWIDTH -8 , OSDHEIGHT - ((6 * row_y)+5), ausgabe.c_str(), wetterTheme[wetterSetup.w_theme].clrFgText,clrTransparent,font,CELLWIDTH,20,taRight);

			osd->DrawText((OSDWIDTH / 2)+8, OSDHEIGHT - ((5 * row_y)+5), tr("Wind speed: "), wetterTheme[wetterSetup.w_theme].clrFgText,clrTransparent,font);
			  ausgabe = parser.windspeed.c_str();
			  ausgabe = ausgabe + " ";
			  ausgabe = ausgabe + parser.speed.c_str();
			osd->DrawText(OSDWIDTH - CELLWIDTH -8 , OSDHEIGHT - ((5 * row_y)+5), ausgabe.c_str(), wetterTheme[wetterSetup.w_theme].clrFgText,clrTransparent,font,CELLWIDTH,20,taRight);

			osd->DrawText((OSDWIDTH / 2)+8, OSDHEIGHT - ((4 * row_y)+5), tr("Humitidy: "), wetterTheme[wetterSetup.w_theme].clrFgText,clrTransparent,font);
			  ausgabe = parser.humidity.c_str();
			  ausgabe = ausgabe + tr(" %");
			osd->DrawText(OSDWIDTH - CELLWIDTH -8 , OSDHEIGHT - ((4 * row_y)+5), ausgabe.c_str(), wetterTheme[wetterSetup.w_theme].clrFgText,clrTransparent,font,CELLWIDTH,20,taRight);

			osd->DrawText((OSDWIDTH / 2)+8, OSDHEIGHT - ((3 * row_y)+5), tr("Precipitation: "), wetterTheme[wetterSetup.w_theme].clrFgText,clrTransparent,font);
			  ausgabe = parser.raindown.c_str();
			  ausgabe = ausgabe + tr(" %");
			osd->DrawText(OSDWIDTH - CELLWIDTH -8 , OSDHEIGHT - ((3 * row_y)+5), ausgabe.c_str(), wetterTheme[wetterSetup.w_theme].clrFgText,clrTransparent,font,CELLWIDTH,20,taRight);

//			font     =    cFont::GetFont(fontSml);

			if (part==0) {
				osd->DrawText(((OSDWIDTH-196)/2)-4, (((OSDHEIGHT/2)-196)/2)+204, tr("DAY"), wetterTheme[wetterSetup.w_theme].clrFgBorder, wetterTheme[wetterSetup.w_theme].clrBgBorder,font,204,row_y,taCenter);
                                if (!inverted) {
				    osd->DrawBitmap((OSDWIDTH / 2) - (bmNight.Width() / 2), (OSDHEIGHT - (row_y / 2)) - (bmNight.Height() / 2), bmNight, wetterTheme[wetterSetup.w_theme].clrFgBorder, wetterTheme[wetterSetup.w_theme].clrBgBorder); }
				else {    
				    osd->DrawBitmap((OSDWIDTH / 2) - (bmNight_inv.Width() / 2), (OSDHEIGHT - (row_y / 2)) - (bmNight_inv.Height() / 2), bmNight_inv, wetterTheme[wetterSetup.w_theme].clrFgBorder, wetterTheme[wetterSetup.w_theme].clrBgBorder); }
			}

			if (part==1) {
				osd->DrawText(((OSDWIDTH-196)/2)-4, (((OSDHEIGHT/2)-196)/2)+204, tr("NIGHT"), wetterTheme[wetterSetup.w_theme].clrFgBorder, wetterTheme[wetterSetup.w_theme].clrBgBorder,font,204,row_y,taCenter);
                                if (!inverted) {
				    osd->DrawBitmap((OSDWIDTH / 2) - (bmDay.Width() / 2), (OSDHEIGHT - (row_y / 2)) - (bmDay.Height() / 2), bmDay, wetterTheme[wetterSetup.w_theme].clrFgBorder, wetterTheme[wetterSetup.w_theme].clrBgBorder); }
				else {    
				    osd->DrawBitmap((OSDWIDTH / 2) - (bmDay_inv.Width() / 2), (OSDHEIGHT - (row_y / 2)) - (bmDay_inv.Height() / 2), bmDay_inv, wetterTheme[wetterSetup.w_theme].clrFgBorder, wetterTheme[wetterSetup.w_theme].clrBgBorder); }
	                }

			if (day>1){
                                if (!inverted) {
				    osd->DrawBitmap(22, (OSDHEIGHT - (row_y / 2)) - (bmLeft.Height() / 2), bmLeft, wetterTheme[wetterSetup.w_theme].clrFgBorder, wetterTheme[wetterSetup.w_theme].clrBgBorder); }
				else {    
				    osd->DrawBitmap(22, (OSDHEIGHT - (row_y / 2)) - (bmLeft_inv.Height() / 2), bmLeft_inv, wetterTheme[wetterSetup.w_theme].clrFgBorder, wetterTheme[wetterSetup.w_theme].clrBgBorder); }
			}

			if (day<10){
                                if (!inverted) {
				    osd->DrawBitmap(OSDWIDTH - bmRight.Width() -22 , (OSDHEIGHT - (row_y / 2)) - (bmRight.Height() / 2), bmRight, wetterTheme[wetterSetup.w_theme].clrFgBorder, wetterTheme[wetterSetup.w_theme].clrBgBorder); }
				else {    
				    osd->DrawBitmap(OSDWIDTH - bmRight_inv.Width() -22 , (OSDHEIGHT - (row_y / 2)) - (bmRight_inv.Height() / 2), bmRight_inv, wetterTheme[wetterSetup.w_theme].clrFgBorder, wetterTheme[wetterSetup.w_theme].clrBgBorder); }
			}

			
			if ( corner == true ) {
				// Ellipse top left
				osd->DrawEllipse(0, OSDHEIGHT - ((7 * row_y)+10), 12, (OSDHEIGHT - ((7 * row_y)+10)) +12, clrTransparent, -2);
				// Ellipse top right
				osd->DrawEllipse(OSDWIDTH -12, OSDHEIGHT - ((7 * row_y)+10), OSDWIDTH, (OSDHEIGHT - ((7 * row_y)+10))+12, clrTransparent, -1);
				// Ellipse bottom left
				osd->DrawEllipse(0, OSDHEIGHT -12, 12, OSDHEIGHT, clrTransparent, -3);
				// Ellipse bottom right
				osd->DrawEllipse(OSDWIDTH -12, OSDHEIGHT -12, OSDWIDTH, OSDHEIGHT, clrTransparent, -4);
				// Ellipse day/night left
				osd->DrawEllipse(((OSDWIDTH-196)/2)-4, (((OSDHEIGHT/2)-196)/2)+204, ((OSDWIDTH-196)/2)-4+row_y, (((OSDHEIGHT/2)-196)/2)+204+row_y, clrTransparent, -3);
				// Ellipse day/night right
				osd->DrawEllipse(((OSDWIDTH-196)/2)+200-row_y, (((OSDHEIGHT/2)-196)/2)+204, ((OSDWIDTH-196)/2)+200, (((OSDHEIGHT/2)-196)/2)+204+row_y, clrTransparent, -4);
			}

//			if (b){
//			delete b;
//			}

			osd->Flush();
                 }
		else
	         {
			perror(sat_file.c_str());

			osd->DrawText(22, OSDHEIGHT - (7 * row_y), tr("ERROR : NO DATA"), wetterTheme[wetterSetup.w_theme].clrFgText,clrTransparent,font);
			osd->DrawText(22, OSDHEIGHT - (6 * row_y), tr("PRESS OK to download/update data/radarmaps"), wetterTheme[wetterSetup.w_theme].clrFgText,clrTransparent,font);

			osd->DrawText(22, OSDHEIGHT - (5 * row_y), tr("Path to weatherdata is :"), wetterTheme[wetterSetup.w_theme].clrFgText,clrTransparent,font);
			osd->DrawText(22, OSDHEIGHT - (4 * row_y), DataDir, wetterTheme[wetterSetup.w_theme].clrFgText,clrTransparent,font);

			osd->DrawText(22, OSDHEIGHT - (3 * row_y), tr("Have you edited weatherng.sh ?"), wetterTheme[wetterSetup.w_theme].clrFgText,clrTransparent,font);
			osd->DrawText(22, OSDHEIGHT - (2 * row_y), tr("Path to weatherng.sh is :"), wetterTheme[wetterSetup.w_theme].clrFgText,clrTransparent,font);
			osd->DrawText(22, OSDHEIGHT - row_y, ScriptDir, wetterTheme[wetterSetup.w_theme].clrFgText,clrTransparent,font);

			esyslog("ERROR : weatherng: %s don't exist !\n", sat_file.c_str());
			osd->Flush();
		 }
	}

 				// debug

				  dsyslog("DEBUG : weatherng: SHOW\n");

#ifdef HAVE_IMLIB2
				  dsyslog("DEBUG : weatherng: USE IMLIB2\n");;
#endif
#ifdef HAVE_MAGICK
				  dsyslog("DEBUG : weatherng: USE IMAGEMAGICK\n");;
#endif
#ifdef HAVE_4MB
				  dsyslog("DEBUG : weatherng: 256 COLORS ENABLED  = %i\n", hicolor);
#endif
				  dsyslog("DEBUG : weatherng: AREADEPTH   = %i\n", areadepth);
				  dsyslog("DEBUG : weatherng: COLORDEPTH = %i\n", colordepth);
				  dsyslog("DEBUG : weatherng: RADAR_LEFT = %i\n", Radar_left);
				  dsyslog("DEBUG : weatherng: RADAR_TOP  = %i\n", Radar_top);
				  dsyslog("DEBUG : weatherng: RADAR_WIDTH  = %i\n", Radar_width);
				  dsyslog("DEBUG : weatherng: RADAR_HEIGHT  = %i\n", Radar_height);



//return EXIT_SUCCESS;	
}
