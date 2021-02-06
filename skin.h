/*
 *  $Id: skin.h,v 1.1.1.1 2005/01/17 20:03:09 beejay Exp $
 */

#ifndef VDR_TEXT2SKIN_XML_SKIN_H 
#define VDR_TEXT2SKIN_XML_SKIN_H 

#include "xml/display.h"
#include <vdr/osd.h>
#include <expat.h>
#include <map>
#include <string>

// --- cxSkin -----------------------------------------------------------------


class cMemberVars{

public:

private:
	std::string      mName;
	std::string      mTitle;
	std::string      mVersion;
	
	

public:
	const std::string &Name(void)       const { return mName; }
	const std::string &Title(void)      const { return mTitle; }
	const std::string &Version(void)    const { return mVersion; }

};


#endif // VDR_TEXT2SKIN_XML_SKIN_H 
