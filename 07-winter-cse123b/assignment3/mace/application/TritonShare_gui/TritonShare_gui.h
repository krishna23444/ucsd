#ifndef _CALALUM_GUI_H
#define _CALALUM_GUI_H 1

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

extern GtkWidget *CalALumWindow;

void Do_Query(char *);
void Do_Get_File(MaceKey server, int port, const char *filename);
void Handle_Row_Activated(int rowNum);

class MyHttpClientResponseHandler : public HttpClientResponseHandler{
 public:
  void postRequestResult(HttpClientResponseState s) {
  }

  void getRequestResult(HttpClientResponseState s) {
    try {
      mace::string path = Params::Instance()->getStr("share");
      if (path.substr(path.size()-1,  mace::string::npos) == "/")
        path = path.substr(0, path.size()-1);

      HttpResponse& r = s.getResponse();
      mace::string fileName = r.url.substr(1, r.url.size()-1);
      
      int fd = creat(mace::string(path + "/" + fileName).c_str(), S_IRWXU);
      write(fd, r.content.data(), r.content.size());      
      close(fd);
    }
    catch(const Exception& e) {
      std::cerr << e << std::endl;
    }

  }
};


//extern MyHttpClientResponseHandler myHttpClientResponseHandler;

class GuiManager {
 public:
  static const int GUI_COUNT_COLUMN = 0;
  static const int GUI_IP_COLUMN = 1;
  static const int GUI_FILENAME_COLUMN = 2;
  
 private:
  GtkStatusbar *statusBar;
  GtkListStore *listStore;

 public:  
  GuiManager(GtkStatusbar * sbar, GtkListStore *ls) {
    statusBar = sbar;
    listStore = ls;
  }
  
  void Update_Status_Bar(std::string newStatus) {
    gtk_statusbar_pop(statusBar, 1);
    gtk_statusbar_push(statusBar, 1, newStatus.c_str());
  }

  void Clear_Search_Results() {
    gtk_list_store_clear(listStore);
  }

  void Push_Search_Result(int column, MaceKey user, std::string fileName) {
    GtkTreeIter iter;
    gtk_list_store_append(listStore, &iter);
    gtk_list_store_set(listStore, &iter, GUI_COUNT_COLUMN, column,
                       GUI_IP_COLUMN, user.toString().c_str(),
                       GUI_FILENAME_COLUMN, fileName.c_str(), -1);    
  }
};

#endif
