#include <gtk/gtk.h>
#include "model.h"

void KeyStroke(GtkWidget *widget, GdkEventKey *event, gpointer data);

void move_up(GtkWidget *widget, 
		GtkTreeModel *model, 
		GtkTreePath *path,
		GtkTreeViewColumn *column, 
		gint rownum,
		gchar *pathstr);

void move_down(GtkWidget *widget, 
		GtkTreeModel *model, 
		GtkTreePath *path,
		GtkTreeViewColumn *column, 
		gint rownum,
		gchar *pathstr);

void move_left(GtkWidget *widget, 
		GtkTreeModel *model, 
		GtkTreePath *path,
		GtkTreeViewColumn *column, 
		gint colnum);

void move_right(GtkWidget *widget, 
		GtkTreeModel *model, 
		GtkTreePath *path,
		GtkTreeViewColumn *column, 
		gint colnum);
