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

char *Radarmap;

const int MAX_LOGO_COLORS=16;
const cFont *font;

int part=0;
bool IsSatelite,satelite;
std::string replaceDay(std::string value){
if (value=="Monday")
{
	return "Montag";
}
else if(value=="Tuesday")
{
        return "Dienstag";
}
else if(value=="Wednesday")
{
        return "Mittwoch";
}
else if(value=="Thursday")
{
        return "Donnerstag";
}
else if(value=="Friday")
{
        return "Freitag";
}
else if(value=="Saturday")
{
        return "Samstag";
}
else if(value=="Sunday")
{
        return "Sonntag";
}
else
{
	return "x";
}

} 

cWetterOsd::cWetterOsd(cWetterSetup *setup)
{
        code          = setup->stationId;
        Radar_left    = setup->w_left;
        Radar_top     = setup->w_top;
        Radar_width   = setup->w_width;
        Radar_height  = setup->w_height;
        fontsize     = setup->w_fontsize;

	std::string url="http://xoap.weather.com/weather/local/";
        url=url + code ;
        url=url + "?cc=*&unit=m&dayf=10&prod=xoap&par=1004124588&key=079f24145f208494";
        printf("Destination: %s\n",DestinationDir);
	std::string dest=DestinationDir;	
        if (!Get(url.c_str(),dest+"/daten.dat")){
                printf ("Unable to connect to http://xoap.weather.com\n");
        }

  this->setup = setup;
  osd = NULL;
  day=1;
  IsSatelite=false;
}

cWetterOsd::~cWetterOsd(void)
{
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

                esyslog("left=%i top=%i witdh=%i height=%i",Radar_left,Radar_top,Radar_width,Radar_height);
                tArea Area = {1,1,Radar_width,Radar_height,4};
                
               if (osd->CanHandleAreas(&Area, 1) == oeOk)
	          osd->SetAreas(&Area, 1);
	       else	  
                  esyslog("%s: OSD open failed! Can't handle areas\n",PLUGIN_NAME_I18N);

//                osd->DrawRectangle(1,1,Radar_width,Radar_height, clrGray50);
                std::string sat_dir;
                sat_dir = DestinationDir;
		sat_dir = sat_dir  + Radarmap;
		esyslog("Load Satelite picture : %s", sat_dir.c_str());
//                cBitmap *b = LoadMagick(sat_dir.c_str(),335,419,16,true);
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
	
	cSetup Setup;
	std::string ausgabe;
	// unused int MAXWIDTH = 299;
		int MAXHOEHE = 480;
	 	int MAXBREITE = 624;
		int HALBEHOEHE= (Setup.OSDHeight/2)+1;
		int HALBEBREITE= (Setup.OSDWidth/2)+1;	
	 	int CELLWIDTH= (Setup.OSDWidth/4)-1;	
		printf("Height:%i max height: %i \n", HALBEHOEHE,HALBEBREITE);
		tArea Area[] = {{CELLWIDTH-1,0,3*CELLWIDTH,(Setup.OSDHeight/2)-1,4},
				{0,(Setup.OSDHeight/2)+1,(Setup.OSDWidth/2) -1,MAXHOEHE,2},
				{(Setup.OSDWidth/2)+1,(Setup.OSDHeight/2)+1,Setup.OSDWidth,MAXHOEHE,1}
				};
                osd->SetAreas(Area, sizeof(Area)/sizeof(tArea));
		printf ("left: %i Width: %i\n",Setup.OSDLeft,Setup.OSDWidth); 
		cxmlParse parser;
               
	        if (fontsize==false) 
		  {font= cFont::GetFont(fontOsd);}
	        else 
		  {font = cFont::GetFont(fontSml);}

		osd->DrawRectangle(0,0,MAXBREITE,HALBEHOEHE, clrTransparent);
		osd->DrawRectangle(0,(Setup.OSDHeight/2)+1,(Setup.OSDWidth/2) -1,MAXHOEHE, clrGray50);
		osd->DrawRectangle((Setup.OSDWidth/2)+1,(Setup.OSDHeight/2)+1,Setup.OSDWidth,MAXHOEHE,clrGray50);

		printf("day: %i\n",day);	
		parser.xmlParse((int) day, DestinationDir,part);
		file=DestinationDir;
		file= file  + "/images/";
		file = file + parser.icon.c_str();
		file = file + ".png";
		cBitmap *b = LoadMagick(file.c_str(),128,128,16,true);
                if (b){
                        osd->DrawBitmap((Setup.OSDWidth-128) /2,((Setup.OSDHeight/2)-128)/2 ,*b,clrTransparent,clrTransparent);
                }

		ausgabe =tr("Weather for: ");
		ausgabe = ausgabe + replaceDay(parser.dayname.c_str()) + " ("+  parser.date.c_str() +")";
		osd->DrawText(2,HALBEHOEHE+4,ausgabe.c_str(),clrWhite,clrTransparent,font);
	       	
		osd->DrawText(2,HALBEHOEHE+34,tr("High temperature: "),clrYellow,clrTransparent,font);
		ausgabe = parser.hi.c_str();
		ausgabe= ausgabe + tr("°C");
		osd->DrawText(CELLWIDTH,HALBEHOEHE+34,ausgabe.c_str(),clrYellow,clrTransparent,font,CELLWIDTH,20,taRight);
		
		osd->DrawText(2,HALBEHOEHE+60,tr("Low temperature: "),clrCyan,clrTransparent,font);
                ausgabe = parser.low.c_str();
                ausgabe= ausgabe + tr("°C");
                osd->DrawText(CELLWIDTH,HALBEHOEHE+60,ausgabe.c_str(),clrCyan,clrTransparent,font,CELLWIDTH,20,taRight);

		osd->DrawText(1,HALBEHOEHE+90,tr("Weather: "),clrWhite,clrTransparent,font);
                ausgabe = parser.wetter.c_str();
                ausgabe= " " + ausgabe + " ";
                osd->DrawText(55,HALBEHOEHE+120,ausgabe.c_str(),clrWhite,clrTransparent,font,(Setup.OSDWidth/2)/2,20,taCenter);

 		osd->DrawText(2,HALBEHOEHE+160,tr("Sunrise: "),clrWhite,clrTransparent,font);
                ausgabe = parser.sunrise.c_str();
                ausgabe= ausgabe;
                osd->DrawText(CELLWIDTH,HALBEHOEHE+160,ausgabe.c_str(),clrWhite,clrTransparent,font,CELLWIDTH,20,taRight);

		osd->DrawText(2,HALBEHOEHE+190,tr("Sunset: "),clrWhite,clrTransparent,font);
                ausgabe = parser.sunset.c_str();
                ausgabe= ausgabe;
                osd->DrawText(CELLWIDTH,HALBEHOEHE+190,ausgabe.c_str(),clrWhite,clrTransparent,font,CELLWIDTH,20,taRight);
		
		//*right Side
		printf("Information about: %s\n",parser.ort.c_str());
                osd->DrawText(HALBEBREITE+20,HALBEHOEHE+4,tr("Information about country"),clrWhite,clrTransparent,font);
		osd->DrawText(HALBEBREITE+20,HALBEHOEHE+34,parser.ort.c_str(),clrWhite,clrTransparent,font);
		
		osd->DrawText(HALBEBREITE+20,HALBEHOEHE+60,"Navigation",clrWhite,clrTransparent,font);
		if (part==0) {
		osd->DrawText(HALBEBREITE+20,HALBEHOEHE+100,tr("Up=Weather at night"),clrWhite,clrTransparent,font);
		}
		if (part==1) {
                osd->DrawText(HALBEBREITE+20,HALBEHOEHE+100,tr("Down=Weather at day"),clrWhite,clrTransparent,font);
                }

		if (day>2){
			osd->DrawText(HALBEBREITE+20,HALBEHOEHE+190,tr("<-- Day backward"),clrWhite,clrTransparent,font);
		}
		if (day<3){
			osd->DrawText(HALBEBREITE+ 150,HALBEHOEHE+190,tr("Day forward -->"),clrWhite,clrTransparent,font,CELLWIDTH,20,taRight);
		}
		if (b){
		delete b;
		}
		osd->Flush();
	
     }

}

