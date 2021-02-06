/*
 *  $Id: parsing.c,v 1.2 2005/01/18 08:46:02 boonkerz Exp $
 */

#include "parsing.h"

#include "xml.h"
#include <vdr/tools.h>
#include <stdio.h>
#include <vector>
#include <string>
#include "getdata.h"


#define TAG_ERR_REMAIN(_context) do { \
                esyslog("ERROR: Text2Skin: Unexpected tag %s within %s", \
                                name.c_str(), _context); \
                return false; \
        } while (0)

#define TAG_ERR_CHILD(_context) do { \
                esyslog("ERROR: Text2Skin: No child tag %s expected within %s", \
                                name.c_str(), _context); \
                return false; \
        } while (0)

#define TAG_ERR_END(_context) do { \
                esyslog("ERROR: Text2Skin: Unexpected closing tag for %s within %s", \
                                name.c_str(), _context); \
                return false; \
        } while (0)

#define ATTRIB_OPT_STRING(_attr,_target) \
        if (attrs.find(_attr) != attrs.end()) { \
                _target = attrs[_attr]; \
        }
#define ATTRIB_MAN_STRING(_attr,_target) \
        ATTRIB_OPT_STRING(_attr,_target) \
        else { \
                esyslog("ERROR: Text2Skin: Mandatory attribute %s missing in tag %s", \
                                _attr, name.c_str()); \
                return false; \
        }

#define ATTRIB_MAN_NUMBER(_attr,_target) \
        ATTRIB_OPT_NUMBER(_attr,_target) \
        else { \
                esyslog("ERROR: Text2Skin: Mandatory attribute %s missing in tag %s", \
                                _attr, name.c_str()); \
                return false; \
        }

#define ATTRIB_OPT_FUNC(_attr,_func) \
        if (attrs.find(_attr) != attrs.end()) { \
                if (!_func(attrs[_attr])) { \
                        esyslog("ERROR: Text2Skin: Unexpected value %s for attribute %s", \
                                        attrs[_attr].c_str(), _attr); \
                        return false; \
                } \
        }

#define ATTRIB_MAN_FUNC(_attr,_func) \
        ATTRIB_OPT_FUNC(_attr,_func) \
        else { \
                esyslog("ERROR: Text2Skin: Mandatory attribute %s missing in tag %s", \
                                _attr, name.c_str()); \
                return false; \
        }


static std::vector<std::string> context;
bool locked;
int count=0;
std::string param[255][255];
std::string day,date,town;

bool xStartElem(const std::string &name, std::map<std::string, std::string> &attrs) {
	if (name=="wind"){
		locked=true;
	}
	if(name=="day"){
		count++;
		ATTRIB_MAN_STRING("t",param[count][6]);
		ATTRIB_MAN_STRING("dt",param[count][7]);
	}
	context.push_back(name);
	return true;
}

bool xCharData(const std::string &text) {
	if (locked==false){	
	if(context.size()>0 ) {
		if (context[context.size()-1]=="dnam"){
			town=text.c_str();
		}
		if (context[context.size()-1]=="sunr"){
			//cxmlParse::parameter[count][0]= text.c_str();
			param[count][0]= text.c_str();
		}
		if (context[context.size() -1]=="suns"){
			param[count][1]=text.c_str();
		}
		if (context[context.size()-1]=="hi"){
			param[count][2]=text.c_str();
		}
		if (context[context.size()-1]=="low"){
			param[count][3]=text.c_str();
		}
		if (context[context.size()-1]=="t"){
			param[count][4]=text.c_str();
		}
		if (context[context.size()-1]=="icon"){
			param[count][5]=text.c_str();
		}
	}

	}

    return true;
}

bool xEndElem(const std::string &name) {
	if (context[context.size()-1]=="wind"){
		locked= false;
	}
	if (context.size()>0){
		context.pop_back();   
	}
	return true;
}


void cxmlParse::xmlParse(int daycount, std::string plugindir) {
	/*std::string url="http://xoap.weather.com/weather/local/";
  	url=url + code ;
  	url=url + "?cc=*&unit=m&dayf=4&prod=xoap&par=1004124588&key=079f24145f208494";
	printf("url %s\n",url.c_str());
	if (!Get(url.c_str(),"/video/daten.dat")){
		printf ("Unable to connect to http://xoap.weather.com\n");
	}
	else{*/
		//context.clear();
	
		XML xml(plugindir + "/daten.dat");
		xml.nodeStartCB(xStartElem);
		xml.nodeEndCB(xEndElem);
		xml.cdataCB(xCharData);
		if (xml.parse() != 0) {
		}else{
			cxmlParse::ort=town.c_str();		
			printf("Stadt: %s\n",town.c_str());
			for (int x=0;count>x;x++){
				if (x==daycount){
					cxmlParse::sunrise=param[x][0].c_str();
					cxmlParse::sunset=param[x][1].c_str();
					cxmlParse::hi=param[x][2].c_str();
					cxmlParse::low=param[x][3].c_str();
					cxmlParse::wetter=param[x][4].c_str();
					cxmlParse::icon=param[x][5].c_str();
					cxmlParse::dayname=param[x][6].c_str();
					cxmlParse::date=param[x][7].c_str();
				}
				
/*
				printf("Sunrise(%i):\t %s\n",x,param[x][0].c_str());
				printf("Sunset(%i):\t %s\n",x,param[x][1].c_str());
				printf("Hi(%i):\t\t %s\n",x,param[x][2].c_str());
				printf("Low(%i):\t\t %s\n",x,param[x][3].c_str());
				printf("Wetter(%i):\t %s\n",x,param[x][4].c_str());
				printf("Icon(%i):\t %s\n",x,param[x][5].c_str());
*/
			}
//		}
	}
}
	
