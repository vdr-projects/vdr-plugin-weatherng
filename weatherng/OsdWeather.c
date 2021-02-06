#include "OsdWeather.h"
#include "parsing.h"
#include <vector>
#include <string>
#include "img/bitmap.h"
#include "getdata.h"
#include <vdr/plugin.h>
#include "i18n.h"

const int MAX_LOGO_COLORS=16;
cWetterOsd::cWetterOsd(cWetterSetup *setup)
{
  this->setup = setup;
  osd = NULL;
}

cWetterOsd::~cWetterOsd()
{
  delete osd;

}
void cWetterOsd::Show(void)
{
osd = cOsdProvider::NewOsd(50, 50);
  if (osd) {
	std::string code,file,ausgabe;
	std::string plugindir = cPlugin::ConfigDirectory("weatherng");
	// unused int MAXWIDTH = 299;
	int left_tab=130;
	code=setup->stationId;
	std::string url="http://xoap.weather.com/weather/local/";
        url=url + code ;
        url=url + "?cc=*&unit=m&dayf=4&prod=xoap&par=1004124588&key=079f24145f208494";
        printf("url %s\n",url.c_str());
	if (!Get(url.c_str(),plugindir +"/daten.dat")){
                printf ("Unable to connect to http://xoap.weather.com\n");
        }
        else{
		cxmlParse parser;
		parser.xmlParse(2, plugindir);
		printf("test:%s\n",parser.ort.c_str());
		const cFont *font = cFont::GetFont(fontOsd);
		tArea Area = { 0, 0, 400, 400,  4 };
		osd->SetAreas(&Area, 1);
		osd->DrawRectangle(0, 0, 400, 400,clrTransparent);
		file=plugindir + "/images/";
		file = file + parser.icon.c_str();
		file = file + ".png";
		cBitmap *b = LoadMagick(file.c_str(),64,64,MAX_LOGO_COLORS,true);
		ausgabe= "State: ";
		ausgabe= ausgabe + parser.ort.c_str();
		
		osd->DrawText(2,5,ausgabe.c_str(),clrWhite,clrTransparent,font);
		if (b)
			osd->DrawBitmap(65,50,*b);
		ausgabe ="Wetter für: ";
		ausgabe = ausgabe + parser.dayname.c_str();
		osd->DrawText(2,25,ausgabe.c_str(),clrWhite,clrTransparent,font);
	       	
		osd->DrawText(2,120,"Höchsttemp.: ",clrWhite,clrTransparent,font);
		ausgabe = parser.hi.c_str();
		ausgabe= ausgabe + "°C";
	        osd->DrawText(left_tab,120,ausgabe.c_str(),clrWhite,clrTransparent,font,0,0,taRight);

		osd->DrawText(2,140,"Tiefsttemp.: ",clrWhite,clrTransparent,font);
		ausgabe = parser.low.c_str();
	        ausgabe= ausgabe + "°C";
		osd->DrawText(left_tab,140,ausgabe.c_str(),clrWhite,clrTransparent,font);
	
		osd->DrawText(2,160,"Sonnenaufg.: ",clrWhite,clrTransparent,font);
	        osd->DrawText(left_tab,160,parser.sunset.c_str(),clrWhite,clrTransparent,font,0,0,taRight);

		osd->DrawText(2,180,"Sonnenunt..: ",clrWhite,clrTransparent,font);
	        osd->DrawText(left_tab,180,parser.sunrise.c_str(),clrWhite,clrTransparent,font,0,0,taRight);
		delete b;
		
		osd->Flush();
	}
     }

}

