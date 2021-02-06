#include <vector>
#include <string>
#include <vdr/plugin.h>
#include <vdr/osd.h>
#include <vdr/config.h>
#include "OsdWeather.h"
#include "parsing.h"
#include "img/bitmap.h"
#include "getdata.h"
#include "vars.h"
#include "i18n.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>


#define TRUE 1
#define FALSE 0
#define BOOL int

char *Radarmap;

const int MAX_LOGO_COLORS=16;
const cFont *font;

int part=0;
int Radar_Color=4;
bool IsSatelite,satelite;

static int if_exist_file( const char *filename) {
  FILE *f = fopen( filename, "r" );
  if( errno != ENOENT ) {
        fclose(f);
	return TRUE;
      }
  else
      return FALSE;
}            	


cWetterOsd::cWetterOsd(void)
//:cOsdObject(true), cThread("wetter osd")
{
//  Dprintf)"%s()\n", __PRETTY_FUNCTION__);
  osd = NULL;
  day=1;
  IsSatelite=false;
  code          = wetterSetup.stationId;
  Radar_left    = wetterSetup.w_left;
  Radar_top     = wetterSetup.w_top;
  Radar_width   = wetterSetup.w_width;
  Radar_height  = wetterSetup.w_height;
  radarcolor    = wetterSetup.w_rcolor;
  fontsize      = wetterSetup.w_fontsize;
  Offlinemode   = wetterSetup.w_offline;

  std::string url="http://xoap.weather.com/weather/local/";
  url=url + code ;
  url=url + "?cc=*&unit=m&dayf=10&prod=xoap&par=1004124588&key=079f24145f208494";

  std::string dest=DataDir;	
  FILE * datadir;
  datadir = fopen (DataDir, "r" );
 
 if (datadir != NULL) {
              printf("Data directory: %s\n",DataDir);

              if (Offlinemode==false) {
                 if (!Get(url.c_str(),dest+"/daten.dat")); {
		    printf("Status: Onlinemode\n");
		    }
		 }
      }	    
  else {
    printf("ERROR : Directory %s not found\n",DataDir);
    }        

}

cWetterOsd::~cWetterOsd(void)
{
//  Dprintf)"%s()\n", __PRETTY_FUNCTION__);
  if (osd)
     delete osd;

}

eOSState cWetterOsd::ProcessKey(eKeys Key)
{
  eOSState state = cOsdObject::ProcessKey(Key);
  if (state == osUnknown) {
     switch (Key & ~k_Repeat) {
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
	case k5:
			IsSatelite=true;
			Radarmap="/overall.wet";
			cWetterOsd::Satelite();
		break;
	case kRed:
			IsSatelite=true;
			Radarmap="/pic1.wet";
			cWetterOsd::Satelite();
		break;
	case kGreen:
			IsSatelite=true;
			Radarmap="/pic2.wet";
			cWetterOsd::Satelite();
		break;
	case kYellow:
			IsSatelite=true;
			Radarmap="/pic3.wet";
			cWetterOsd::Satelite();
		break;
	case kBlue:
			IsSatelite=true;
			Radarmap="/pic4.wet";
			cWetterOsd::Satelite();
		break;
	default: return state;
       }
     state = osContinue;
     }
  return state;
}

void cWetterOsd::Satelite(void)
{
delete osd;
osd = cOsdProvider::NewOsd(Radar_left ,Radar_top );
satelite=false;
if (osd) {

	        if (radarcolor==false) {
		       Radar_Color= 4;
		    }
		  else {
		       Radar_Color = 8; }


                esyslog("left=%i top=%i witdh=%i height=%i",Radar_left,Radar_top,Radar_width,Radar_height);
                tArea Area = {1,1,Radar_width,Radar_height,Radar_Color};
                
                eOsdError result = osd->CanHandleAreas(&Area, 1);
                if (result == oeOk) {
	           osd->SetAreas(&Area, 1);
		   }
	        else	  
                   esyslog("%s: OSD open failed! Can't handle areas (%d)\n",PLUGIN_NAME_I18N,result);

                std::string sat_dir;
                sat_dir = DataDir;
		sat_dir = sat_dir  + Radarmap;
		esyslog("Load Satelite picture : %s", sat_dir.c_str());
                cBitmap *b = LoadMagick(sat_dir.c_str(),Radar_height - 2,Radar_width - 2,16,true);
                if (b){
                 osd->DrawBitmap(2,2,*b,clrTransparent,clrTransparent);

}

		delete b;
		osd->Flush();
}

}




void cWetterOsd::Show(void)
{
  IsSatelite=false;
  printf("Part: %i\n",part);

  delete osd;
  osd = cOsdProvider::NewOsd(Setup.OSDLeft,Setup.OSDTop);
  if (osd) {
//	cSetup Setup;
	std::string ausgabe;
        std::string sat_file;
	sat_file = DataDir;
	sat_file = sat_file + "/daten.dat";
	BOOL ex = if_exist_file(sat_file.c_str());
	
		int MAXHOEHE = 480;
	 	int MAXBREITE = 624;
		int HALBEHOEHE= (Setup.OSDHeight/2)+1;
		int HALBEBREITE= (Setup.OSDWidth/2)+1;	
	 	int CELLWIDTH= (Setup.OSDWidth/4)-1;	
		printf("Height:%i max height: %i \n", HALBEHOEHE,HALBEBREITE);

		tArea Area[] = {{CELLWIDTH-1,0,3*CELLWIDTH,(Setup.OSDHeight/2)-1,4},
				{0,(Setup.OSDHeight/2)+1,(Setup.OSDWidth/2) -1,MAXHOEHE,2},
				{(Setup.OSDWidth/2)+1,(Setup.OSDHeight/2)+1,Setup.OSDWidth,MAXHOEHE,2}
				};

	        eOsdError result = osd->CanHandleAreas(Area, sizeof(Area) / sizeof(tArea));
                if (result == oeOk) {
	           osd->SetAreas(Area, sizeof(Area) / sizeof(tArea));
		   }
	        else	  
                   esyslog("%s: OSD open failed! Can't handle areas (%d)\n",PLUGIN_NAME_I18N, result);

	        if (!fontsize) {
		       font= cFont::GetFont(fontOsd);
		    }
		  else {
		       font = cFont::GetFont(fontSml); }

		osd->DrawRectangle(0,0,MAXBREITE,HALBEHOEHE, clrTransparent);
		osd->DrawRectangle(0,(Setup.OSDHeight/2)+1,(Setup.OSDWidth/2) -1,MAXHOEHE, wetterTheme[wetterSetup.w_theme].clrBackground);
		osd->DrawRectangle((Setup.OSDWidth/2)+1,(Setup.OSDHeight/2)+1,Setup.OSDWidth,MAXHOEHE,  wetterTheme[wetterSetup.w_theme].clrBackground);
		
		printf ("left: %i Width: %i\n",Setup.OSDLeft,Setup.OSDWidth); 

                if ( ex ) {
			printf("Parse %s/daten.dat !\n",DataDir);

           		cxmlParse parser;

			printf("day: %i\n",day);	

			parser.xmlParse((int) day, DataDir,part);
			file = ImageDir;
			file = file  + "/images/";
			file = file + parser.icon.c_str();
			file = file + ".png";
			cBitmap *b = LoadMagick(file.c_str(),128,128,16,true);
	        
		        if (b){
				osd->DrawRectangle(((Setup.OSDWidth-128)/2)-4,(((Setup.OSDHeight/2)-128)/2)-4,((Setup.OSDWidth-128)/2)+132,(((Setup.OSDHeight/2)-128)/2), wetterTheme[wetterSetup.w_theme].clrFgText);
				osd->DrawRectangle(((Setup.OSDWidth-128)/2)-4,(((Setup.OSDHeight/2)-128)/2)+128,((Setup.OSDWidth-128)/2)+132,(((Setup.OSDHeight/2)-128)/2)+132, wetterTheme[wetterSetup.w_theme].clrFgText);
				osd->DrawRectangle(((Setup.OSDWidth-128)/2)-4,(((Setup.OSDHeight/2)-128)/2),((Setup.OSDWidth-128)/2),(((Setup.OSDHeight/2)-128)/2)+128, wetterTheme[wetterSetup.w_theme].clrFgText);
				osd->DrawRectangle(((Setup.OSDWidth-128)/2)+128,(((Setup.OSDHeight/2)-128)/2),((Setup.OSDWidth-128)/2)+132,(((Setup.OSDHeight/2)-128)/2)+128, wetterTheme[wetterSetup.w_theme].clrFgText);
	                        osd->DrawBitmap((Setup.OSDWidth-128) /2,((Setup.OSDHeight/2)-128)/2 ,*b,clrTransparent,clrTransparent);
	                }

			printf("Information about: %s\n",parser.ort.c_str());

			  ausgabe = tr("Weather for: ");
			  ausgabe = ausgabe + parser.ort.c_str();
			osd->DrawText(8,HALBEHOEHE+4,ausgabe.c_str(), wetterTheme[wetterSetup.w_theme].clrFgText,clrTransparent,font);


//			  ausgabe = replaceDay(parser.dayname.c_str()) + " ("+  parser.date.c_str() +")";
			  ausgabe = parser.dayname;
//			  ausgabe = tr(ausgabe);
			osd->DrawText(HALBEBREITE+70,HALBEHOEHE+4,tr(ausgabe.c_str()), wetterTheme[wetterSetup.w_theme].clrFgText,clrTransparent,font,CELLWIDTH,20,taRight);

		          ausgabe = "(" + parser.date + ")";
    			osd->DrawText(HALBEBREITE+144,HALBEHOEHE+4,ausgabe.c_str(), wetterTheme[wetterSetup.w_theme].clrFgText,clrTransparent,font,CELLWIDTH,20,taRight);


			osd->DrawText(8,HALBEHOEHE+40,tr("High temperature: "), wetterTheme[wetterSetup.w_theme].clrFgHiColor,clrTransparent,font);
		          ausgabe = parser.hi.c_str();
		          ausgabe= ausgabe + tr("°C");
			osd->DrawText(CELLWIDTH,HALBEHOEHE+40,ausgabe.c_str(), wetterTheme[wetterSetup.w_theme].clrFgHiColor,clrTransparent,font,CELLWIDTH,20,taRight);
		
			osd->DrawText(8,HALBEHOEHE+70,tr("Low temperature: "), wetterTheme[wetterSetup.w_theme].clrFgLowColor,clrTransparent,font);
	                  ausgabe = parser.low.c_str();
	                  ausgabe= ausgabe + tr("°C");
	                osd->DrawText(CELLWIDTH,HALBEHOEHE+70,ausgabe.c_str(), wetterTheme[wetterSetup.w_theme].clrFgLowColor,clrTransparent,font,CELLWIDTH,20,taRight);

			osd->DrawText(8,HALBEHOEHE+160,tr("Weather: "), wetterTheme[wetterSetup.w_theme].clrFgText,clrTransparent,font);
	                  ausgabe = parser.wetter;
	                osd->DrawText((Setup.OSDWidth/2)-225,HALBEHOEHE+160,tr(ausgabe.c_str()), wetterTheme[wetterSetup.w_theme].clrFgText,clrTransparent,font,225,20,taRight);

	 		osd->DrawText(8,HALBEHOEHE+100,tr("Sunrise: "), wetterTheme[wetterSetup.w_theme].clrFgText,clrTransparent,font);
	                  ausgabe = parser.sunrise.c_str();
	                  ausgabe= ausgabe;
	                osd->DrawText(CELLWIDTH,HALBEHOEHE+100,ausgabe.c_str(), wetterTheme[wetterSetup.w_theme].clrFgText,clrTransparent,font,CELLWIDTH,20,taRight);

			osd->DrawText(8,HALBEHOEHE+130,tr("Sunset: "), wetterTheme[wetterSetup.w_theme].clrFgText,clrTransparent,font);
	                  ausgabe = parser.sunset.c_str();
	                  ausgabe= ausgabe;
	                osd->DrawText(CELLWIDTH,HALBEHOEHE+130,ausgabe.c_str(), wetterTheme[wetterSetup.w_theme].clrFgText,clrTransparent,font,CELLWIDTH,20,taRight);
		
			//right Side
			osd->DrawText(HALBEBREITE+15,HALBEHOEHE+40,tr("Wind comes from: "), wetterTheme[wetterSetup.w_theme].clrFgText,clrTransparent,font);
			  ausgabe = parser.winddir.c_str();
			osd->DrawText(HALBEBREITE+144,HALBEHOEHE+40,ausgabe.c_str(), wetterTheme[wetterSetup.w_theme].clrFgText,clrTransparent,font,CELLWIDTH,20,taRight);

			osd->DrawText(HALBEBREITE+15,HALBEHOEHE+70,tr("Wind speed: "), wetterTheme[wetterSetup.w_theme].clrFgText,clrTransparent,font);
			  ausgabe = parser.windspeed.c_str();
			  ausgabe= ausgabe + tr(" km/h");
			osd->DrawText(HALBEBREITE+144,HALBEHOEHE+70,ausgabe.c_str(), wetterTheme[wetterSetup.w_theme].clrFgText,clrTransparent,font,CELLWIDTH,20,taRight);

			osd->DrawText(HALBEBREITE+15,HALBEHOEHE+100,tr("Humitidy: "), wetterTheme[wetterSetup.w_theme].clrFgText,clrTransparent,font);
			  ausgabe = parser.humidity.c_str();
			  ausgabe= ausgabe + tr(" %");
			osd->DrawText(HALBEBREITE+144,HALBEHOEHE+100,ausgabe.c_str(), wetterTheme[wetterSetup.w_theme].clrFgText,clrTransparent,font,CELLWIDTH,20,taRight);

			osd->DrawText(HALBEBREITE+15,HALBEHOEHE+130,tr("Precipitation: "), wetterTheme[wetterSetup.w_theme].clrFgText,clrTransparent,font);
			  ausgabe = parser.raindown.c_str();
			  ausgabe= ausgabe + tr(" %");
			osd->DrawText(HALBEBREITE+144,HALBEHOEHE+130,ausgabe.c_str(), wetterTheme[wetterSetup.w_theme].clrFgText,clrTransparent,font,CELLWIDTH,20,taRight);


                     // BORDER
                     // Oben OK
         		osd->DrawRectangle(0,(Setup.OSDHeight/2)+1,Setup.OSDWidth,(Setup.OSDHeight/2)+4, wetterTheme[wetterSetup.w_theme].clrFgText);
                     // Mitte
         		osd->DrawRectangle(4,(Setup.OSDHeight/2)+30,Setup.OSDWidth-3,(Setup.OSDHeight/2)+33, wetterTheme[wetterSetup.w_theme].clrFgText);
                     // links OK    Mitte OK    Rechts OK
         		osd->DrawRectangle(0,(Setup.OSDHeight/2)+5,3,MAXHOEHE-31, wetterTheme[wetterSetup.w_theme].clrFgText);
         		osd->DrawRectangle(HALBEBREITE+5,(Setup.OSDHeight/2)+34,HALBEBREITE+7,MAXHOEHE-31, wetterTheme[wetterSetup.w_theme].clrFgText);
         		osd->DrawRectangle((Setup.OSDWidth)-3,(Setup.OSDHeight/2)+3,Setup.OSDWidth,MAXHOEHE-31, wetterTheme[wetterSetup.w_theme].clrFgText);
                     // Unten OK
         		osd->DrawRectangle(0,MAXHOEHE-30,Setup.OSDWidth,MAXHOEHE, wetterTheme[wetterSetup.w_theme].clrFgText);


			if (part==0) {
			osd->DrawText((Setup.OSDWidth/2)-120,MAXHOEHE-29,tr("Up=Weather at night"), wetterTheme[wetterSetup.w_theme].clrBackground,clrTransparent,font,240,20,taCenter);
			}

			if (part==1) {
	                osd->DrawText((Setup.OSDWidth/2)-120,MAXHOEHE-29,tr("Down=Weather at day"), wetterTheme[wetterSetup.w_theme].clrBackground,clrTransparent,font,240,20,taCenter);
	                }




			if (day>1){
				osd->DrawText(5,MAXHOEHE-29,tr("<-- Day backward"), wetterTheme[wetterSetup.w_theme].clrBackground,clrTransparent,font);
			}

			if (day<10){
				osd->DrawText(HALBEBREITE+ 150,MAXHOEHE-29,tr("Day forward -->"), wetterTheme[wetterSetup.w_theme].clrBackground,clrTransparent,font,CELLWIDTH,20,taRight);
//				osd->DrawText((Setup.OSDWidth)-5,MAXHOEHE-29,tr("Day forward -->"), wetterTheme[wetterSetup.w_theme].clrBackground,clrTransparent,font,CELLWIDTH,20,taRight);
			}

			if (b){
			delete b;
			}

			osd->Flush(); 
                 }
		else
	         {
			osd->DrawText(20,HALBEHOEHE+10,tr("ERROR : NO DATA"), wetterTheme[wetterSetup.w_theme].clrFgText,clrTransparent,font);
			osd->DrawText(20,HALBEHOEHE+50,tr("Check path and that you are in offlinemode"), wetterTheme[wetterSetup.w_theme].clrFgText,clrTransparent,font);
			osd->DrawText(20,HALBEHOEHE+80,tr("Your path to 'daten.dat' is:"), wetterTheme[wetterSetup.w_theme].clrFgText,clrTransparent,font);
			osd->DrawText(20,HALBEHOEHE+110,tr(DataDir), wetterTheme[wetterSetup.w_theme].clrFgHiColor,clrTransparent,font);
			osd->DrawText(20,HALBEHOEHE+140,tr("Restart VDR with '--loglevel 3' and watch stdout"), wetterTheme[wetterSetup.w_theme].clrFgText,clrTransparent,font);
			osd->DrawText(20,HALBEHOEHE+170,tr("Maybe you have a problem to access the internet ?"), wetterTheme[wetterSetup.w_theme].clrFgText,clrTransparent,font);

			printf("ERROR: %s/daten.dat don't exist !\n",DataDir);
			osd->Flush();
		 }
	}
}
