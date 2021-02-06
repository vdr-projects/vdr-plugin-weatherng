#include <string>
#include "getdata.h"

#include "contrib/HttpGetSocket.h"
#include "contrib/SocketHandler.h"
#include "contrib/Parse.h"

bool Get(const std::string& url_in,const std::string& file)
{
	Parse pa(url_in,":/");
	std::string protocol = pa.getword();
	std::string host = pa.getword();
	int port;
	{
		Parse pa(host,":");
		pa.getword();
		port = pa.getvalue();
	}
	std::string url = "/" + pa.getrest();
//	std::string file;
	{
		Parse pa(url,"/");
		std::string tmp = pa.getword();
		while (tmp.size())
		{
//			file = tmp;
			tmp = pa.getword();
		}
	}
	SocketHandler h;
	if (!strcasecmp(protocol.c_str(),"http"))
	{
		HttpGetSocket s(h,host,port ? port : 80,url,file) ;
		h.Add(&s);
		h.Select(1,0);
		while (h.GetCount())
		{
			h.Select(1,0);
		}
		return s.Complete();
	}
	else
/*	if (!strcasecmp(protocol.c_str(),"https"))
	{
		HttpsGetSocket s(h,host,port ? port : 443,url,file);
		h.Add(&s);
		h.Select(1,0);
		while (h.GetCount())
		{
			h.Select(1,0);
		}
		return s.Complete();
	}
	else
	{
		printf("Unknown protocol: '%s'\n",protocol.c_str());
	}
*/
	return false;
}
