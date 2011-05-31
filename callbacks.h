#include <gtk/gtk.h>

gchar *filename;
gint safe;
gboolean toggle;
GtkWidget *backup;

void destroy_CB(GtkWidget *widget, gpointer data);
void editing_canceled_CB(GtkCellRenderer *renderer, gpointer data);
void buffer_CB(GtkEntryBuffer *buffer, guint arg1, gchar *arg2, guint arg3, gpointer data);
void editing_started_CB(GtkCellRenderer *renderer, GtkCellEditable *editable, const gchar *path, gpointer data);
void edited_CB(GtkCellRendererText *renderer, gchar *path_string, gchar *new_text, gpointer data);

void file_new(GtkAction *action, gpointer data);
void file_open(GtkAction *action, gpointer data);
void file_save(GtkAction *action, gpointer data);
void file_save_as(GtkAction *action, gpointer data);
void file_quit(GtkAction *action, gpointer data);

void table_add_row(GtkAction *action, gpointer data);
void table_remove_row(GtkAction *action, gpointer data);

void create_calc_view(GtkAction *action, gpointer data);
