/*
 * DO NOT EDIT THIS FILE - it is generated by Glade.
 */

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

#include <gdk/gdkkeysyms.h>
#include <gtk/gtk.h>

#include "callbacks.h"
#include "interface.h"
#include "support.h"

#define GLADE_HOOKUP_OBJECT(component,widget,name) \
  g_object_set_data_full (G_OBJECT (component), name, \
    gtk_widget_ref (widget), (GDestroyNotify) gtk_widget_unref)

#define GLADE_HOOKUP_OBJECT_NO_REF(component,widget,name) \
  g_object_set_data (G_OBJECT (component), name, widget)

GtkWidget*
create_CalALumWindow (void)
{
  GtkWidget *CalALumWindow;
  GtkWidget *vbox1;
  GtkWidget *networkStatusBar;
  GtkWidget *hbox1;
  GtkWidget *networkStatusEntry;
  GtkWidget *searchButton;
  GtkWidget *scrolledwindow1;
  GtkWidget *treeview1;

  CalALumWindow = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title (GTK_WINDOW (CalALumWindow), _("CalALum"));
  gtk_window_set_default_size (GTK_WINDOW (CalALumWindow), 500, 500);

  vbox1 = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (vbox1);
  gtk_container_add (GTK_CONTAINER (CalALumWindow), vbox1);

  networkStatusBar = gtk_statusbar_new ();
  gtk_widget_show (networkStatusBar);
  gtk_box_pack_start (GTK_BOX (vbox1), networkStatusBar, FALSE, FALSE, 0);

  hbox1 = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox1);
  gtk_box_pack_start (GTK_BOX (vbox1), hbox1, FALSE, FALSE, 0);

  networkStatusEntry = gtk_entry_new ();
  gtk_widget_show (networkStatusEntry);
  gtk_box_pack_start (GTK_BOX (hbox1), networkStatusEntry, TRUE, TRUE, 0);

  searchButton = gtk_button_new_with_mnemonic (_("Search"));
  gtk_widget_show (searchButton);
  gtk_box_pack_start (GTK_BOX (hbox1), searchButton, FALSE, FALSE, 0);

  scrolledwindow1 = gtk_scrolled_window_new (NULL, NULL);
  gtk_widget_show (scrolledwindow1);
  gtk_box_pack_start (GTK_BOX (vbox1), scrolledwindow1, TRUE, TRUE, 0);

  treeview1 = gtk_tree_view_new ();
  
  /*  GtkCellRenderer     *renderer;
  renderer = gtk_cell_renderer_text_new ();
  gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (treeview1),
                                               -1,      
                                               "Peer",  
                                               renderer,
                                               "text", 0,
                                               NULL);



  renderer = gtk_cell_renderer_text_new ();
  gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (treeview1),
                                               -1,      
                                               "File Name",  
                                               renderer,
                                               "text", 1,
                                               NULL);

  GtkListStore *store = gtk_list_store_new (2, G_TYPE_STRING, G_TYPE_STRING );
  gtk_tree_view_set_model( GTK_TREE_VIEW (treeview1), GTK_TREE_MODEL(store));
  g_object_unref( store );*/


  
  gtk_widget_show (treeview1);
  gtk_container_add (GTK_CONTAINER (scrolledwindow1), treeview1);

  g_signal_connect ((gpointer) CalALumWindow, "delete_event",
                    G_CALLBACK (gtk_main_quit),
                    NULL);
  g_signal_connect ((gpointer) searchButton, "clicked",
                    G_CALLBACK (on_searchButton_clicked),
                    NULL);
  g_signal_connect ((gpointer) treeview1, "row_activated",
                    G_CALLBACK (on_treeview1_row_activated),
                    NULL);
  /* Store pointers to all widgets, for use by lookup_widget(). */
  GLADE_HOOKUP_OBJECT_NO_REF (CalALumWindow, CalALumWindow, "CalALumWindow");
  GLADE_HOOKUP_OBJECT (CalALumWindow, vbox1, "vbox1");
  GLADE_HOOKUP_OBJECT (CalALumWindow, networkStatusBar, "networkStatusBar");
  GLADE_HOOKUP_OBJECT (CalALumWindow, hbox1, "hbox1");
  GLADE_HOOKUP_OBJECT (CalALumWindow, networkStatusEntry, "networkStatusEntry");
  GLADE_HOOKUP_OBJECT (CalALumWindow, searchButton, "searchButton");
  GLADE_HOOKUP_OBJECT (CalALumWindow, scrolledwindow1, "scrolledwindow1");
  GLADE_HOOKUP_OBJECT (CalALumWindow, treeview1, "treeview1");

  return CalALumWindow;
}

