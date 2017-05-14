#ifndef _ERIK_NOGUI_H
#define _ERIK_NOGUI_H 1

//#include "Util.h"
#include "Params.h"
#include "SysUtil.h"
#include "Log.h"
#include <string>
#include <set>
#include "MaceTypes.h"
#include "lib/mace_constants.h"
#include "services/Http/HttpClient.h"
#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

void Do_Query(char *);
void Do_Get_File(MaceKey server, int port, const char *filename);
void Handle_Row_Activated(int rowNum);

class MyHttpClientResponseHandler : public HttpClientResponseHandler{
 public:
  void postRequestResult(HttpClientResponseState s)
  {
  }

  void getRequestResult(HttpClientResponseState s)
  {
    try
    {
      mace::string path = Params::Instance()->getStr("share");
      if(path.substr(path.size()-1,  mace::string::npos) == "/")
	path = path.substr(0, path.size()-1);

      HttpResponse& r = s.getResponse();
      mace::string fileName = r.url.substr(1, r.url.size()-1);
      
      int fd = creat(mace::string(path + "/" + fileName).c_str(), S_IRWXU);
      write(fd, r.content.data(), r.content.size());      
      close(fd);
    }
    catch(const Exception& e)
    {
      std::cerr << e << std::endl;
    }

  }
};


#endif
