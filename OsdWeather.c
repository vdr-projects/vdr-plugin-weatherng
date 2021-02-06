#include <vector>
#include <string>
#include <vdr/plugin.h>
#include <vdr/config.h>
#include "OsdWeather.h"
#include "parsing.h"
#include "img/bitmap.h"
#include "getdata.h"
#include "i18n.h"
#include "vars.h"
const int MAX_LOGO_COLORS=16;
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
        code = setup->stationId;
	plugindir = cPlugin::ConfigDirectory("weatherng");
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
		if (IsSatelite==false) {	
			IsSatelite=true;
			cWetterOsd::Satelite();
		//http://image.weather.com/images/sat/germany_sat_720x486.jpg
		}
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
osd = cOsdProvider::NewOsd(Setup.OSDLeft,Setup.OSDTop);
satelite=false;
if (osd) {
/*
                int MAXHOEHE = 480;
                int MAXBREITE = 624;
                int HALBEHOEHE= (Setup.OSDHeight/2)+1;
                int HALBEBREITE= (Setup.OSDWidth/2)+1;
                int CELLWIDTH= (Setup.OSDWidth/4)-1;
                
		printf("hoehe:%i max weite: %i \n", HALBEHOEHE,HALBEBREITE);
		printf("OSD LEFT: %i\n",Setup.OSDLeft);	
                tArea Area[] = {{Setup.OSDLeft+20,Setup.OSDTop+20,Setup.OSDWidth-124,Setup.OSDHeight-115,4}
                		};
			osd->DrawRectangle(Setup.OSDLeft+20,Setup.OSDTop+20,Setup.OSDWidth-100,Setup.OSDHeight-100, clrTransparent);
		osd->SetAreas(Area, sizeof(Area)/sizeof(tArea));
		std::string sat_dir;
		sat_dir = plugindir +"/sat.jpg";
		printf("Load Satelite picture : %s", sat_dir.c_str());
		cBitmap *b = LoadMagick(sat_dir.c_str(),390,420,16,true);
		if (b){
		 osd->DrawBitmap(Setup.OSDLeft+20,Setup.OSDTop+20 ,*b,clrTransparent,clrTransparent);

}*/
int top = Setup.OSDLeft+20;
                int left = Setup.OSDLeft+20;
                int width = Setup.OSDWidth-124;
                int height = Setup.OSDHeight-115;

                // durch 8 teilbar machen
                width -= width % 8;
                width --;
                // height -= height % 8 -1;


                esyslog("left=%i top=%i witdh=%i height=%i",left,top,width,height);
                tArea Area[] = {{left,top,width,height,4}};
                // esyslog("1");
                osd->DrawRectangle(left,top,width,height, clrTransparent);
                // esyslog("2");
                osd->SetAreas(Area, sizeof(Area)/sizeof(tArea));
                // esyslog("3");
                std::string sat_dir;
                sat_dir = DestinationDir;
		sat_dir = sat_dir  +"/sat.jpg";
		esyslog("Load Satelite picture : %s", sat_dir.c_str());
                cBitmap *b = LoadMagick(sat_dir.c_str(),390,420,16,true);
                if (b){
                 osd->DrawBitmap(left,top ,*b,clrTransparent,clrTransparent);

}

		delete b;
		osd->Flush();
//	}
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
	std::string plugindir = cPlugin::ConfigDirectory("weatherng");
	// unused int MAXWIDTH = 299;
		int MAXHOEHE = 480;
	 	int MAXBREITE = 624;
		int HALBEHOEHE= (Setup.OSDHeight/2)+1;
		int HALBEBREITE= (Setup.OSDWidth/2)+1;	
	 	int CELLWIDTH= (Setup.OSDWidth/4)-1;	
		printf("hoehe:%i max weite: %i \n", HALBEHOEHE,HALBEBREITE);
		tArea Area[] = {{CELLWIDTH-1,0,3*CELLWIDTH,(Setup.OSDHeight/2)-1,4},
				{0,(Setup.OSDHeight/2)+1,(Setup.OSDWidth/2) -1,MAXHOEHE,2},
				{(Setup.OSDWidth/2)+1,(Setup.OSDHeight/2)+1,Setup.OSDWidth,MAXHOEHE,1}
				};
                osd->SetAreas(Area, sizeof(Area)/sizeof(tArea));
		printf ("left: %i Width: %i\n",Setup.OSDLeft,Setup.OSDWidth); 
		cxmlParse parser;
		const cFont *font = cFont::GetFont(fontOsd);
		
		osd->DrawRectangle(0,0,MAXBREITE,HALBEHOEHE, clrTransparent);
		osd->DrawRectangle(0,(Setup.OSDHeight/2)+1,(Setup.OSDWidth/2) -1,MAXHOEHE, clrBlue);
		osd->DrawRectangle((Setup.OSDWidth/2)+1,(Setup.OSDHeight/2)+1,Setup.OSDWidth,MAXHOEHE,  clrBlue);

		printf("day: %i\n",day);	
		parser.xmlParse((int) day, plugindir,part);
		file=DestinationDir;
		file= file  + "/images/";
		file = file + parser.icon.c_str();
		file = file + ".png";
		cBitmap *b = LoadMagick(file.c_str(),128,128,16,true);
                //file=plugindir + "/images/";
                //file = file + "1.png";
                if (b){
                        osd->DrawBitmap((Setup.OSDWidth-128) /2,((Setup.OSDHeight/2)-128)/2 ,*b,clrTransparent,clrTransparent);
                }

		ausgabe =tr("Wetter für: ");
		ausgabe = ausgabe + replaceDay(parser.dayname.c_str()) + " ("+  parser.date.c_str() +")";
		osd->DrawText(2,HALBEHOEHE+4,ausgabe.c_str(),clrWhite,clrTransparent,font);
	       	
		osd->DrawText(2,HALBEHOEHE+34,tr("Höchsttemperatur: "),clrWhite,clrTransparent,font);
		ausgabe = parser.hi.c_str();
		ausgabe= ausgabe + tr("°C");
		osd->DrawText(CELLWIDTH,HALBEHOEHE+34,ausgabe.c_str(),clrRed,clrTransparent,font,CELLWIDTH,20,taRight);
		
		osd->DrawText(2,HALBEHOEHE+60,tr("Tiefsttemperatur: "),clrWhite,clrTransparent,font);
                ausgabe = parser.low.c_str();
                ausgabe= ausgabe + tr("°C");
                osd->DrawText(CELLWIDTH,HALBEHOEHE+60,ausgabe.c_str(),clrCyan,clrTransparent,font,CELLWIDTH,20,taRight);

		osd->DrawText(1,HALBEHOEHE+90,tr("Wetter: "),clrWhite,clrTransparent,font);
                ausgabe = parser.wetter.c_str();
                ausgabe= ausgabe;
                osd->DrawText(55,HALBEHOEHE+120,ausgabe.c_str(),clrWhite,clrTransparent,font,(Setup.OSDWidth/2)/2,20,taCenter);

 		osd->DrawText(2,HALBEHOEHE+160,tr("Sonnenaufgang: "),clrWhite,clrTransparent,font);
                ausgabe = parser.sunrise.c_str();
                ausgabe= ausgabe;
                osd->DrawText(CELLWIDTH,HALBEHOEHE+160,ausgabe.c_str(),clrWhite,clrTransparent,font,CELLWIDTH,20,taRight);

		osd->DrawText(2,HALBEHOEHE+190,tr("Sonnenuntergang: "),clrWhite,clrTransparent,font);
                ausgabe = parser.sunset.c_str();
                ausgabe= ausgabe;
                osd->DrawText(CELLWIDTH,HALBEHOEHE+190,ausgabe.c_str(),clrWhite,clrTransparent,font,CELLWIDTH,20,taRight);
		
		//*right Side
		printf("Daten ausserhalb: %s\n",parser.ort.c_str());
                osd->DrawText(HALBEBREITE,HALBEHOEHE+4,tr("Angaben für den Ort"),clrWhite,clrTransparent,font);
		osd->DrawText(HALBEBREITE,HALBEHOEHE+34,parser.ort.c_str(),clrWhite,clrTransparent,font);
		
		osd->DrawText(HALBEBREITE,HALBEHOEHE+60,"Navigation",clrWhite,clrTransparent,font);
		if (part==0) {
		osd->DrawText(HALBEBREITE,HALBEHOEHE+100,tr("Auf=Wetter Nacht"),clrWhite,clrTransparent,font);
		}
		if (part==1) {
                osd->DrawText(HALBEBREITE,HALBEHOEHE+100,tr("Ab=Wetter Tag"),clrWhite,clrTransparent,font);
                }

		if (day>2){
			osd->DrawText(HALBEBREITE,HALBEHOEHE+180,tr("<-- Tag Zurück"),clrWhite,clrTransparent,font);
		}
		if (day<3){
			osd->DrawText(HALBEBREITE+ 150,HALBEHOEHE+180,tr("Tag vor -->"),clrWhite,clrTransparent,font,CELLWIDTH,20,taRight);
		}
		if (b){
		delete b;
		}
		osd->Flush();
	
     }

}

