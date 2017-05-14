#include <gtk/gtk.h>


gboolean
on_CalALumWindow_delete_event          (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data);

void
on_searchButton_clicked                (GtkButton       *button,
                                        gpointer         user_data);
void
on_treeview1_row_activated             (GtkTreeView     *treeview,
                                        GtkTreePath     *path,
                                        GtkTreeViewColumn *column,
                                        gpointer         user_data);
