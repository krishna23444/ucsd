#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif
#ifndef ENABLE_NLS
#define ENABLE_NLS 1
#endif

#include <gtk/gtk.h>
#include "TritonShare_gui.h"
#include "callbacks.h"
#include "interface.h"
#include "support.h"

#include <string>

gboolean
on_CalALumWindow_delete_event          (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data)
{

  return FALSE;
}


void
on_searchButton_clicked                (GtkButton       *button,
                                        gpointer         user_data)
{
  //oops, mislabelled "network status entry" instead of "query entry"
  GtkWidget *ptr = lookup_widget(CalALumWindow, "networkStatusEntry");
  char * entryText;

  if(ptr)
  {
    entryText = (char*)gtk_entry_get_text(GTK_ENTRY(ptr));
    Do_Query(entryText);
  }
}

void
on_treeview1_row_activated             (GtkTreeView     *treeview,
                                        GtkTreePath     *path,
                                        GtkTreeViewColumn *column,
                                        gpointer         user_data)
{
  GtkTreeModel *model;
  GtkTreeIter   iter;
  model = gtk_tree_view_get_model(treeview);

  if (gtk_tree_model_get_iter(model, &iter, path))
  {

    int rowNum;    
    gtk_tree_model_get(model, &iter, 0, &rowNum, -1); 
    Handle_Row_Activated( rowNum );
  }
}
