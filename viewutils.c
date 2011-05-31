#include "viewutils.h"
#include "callbacks.h"

void append_custom_column(GtkTreeView *view, gint COLUMN, gchar *COLUMN_NAME)
{
	GtkCellRenderer *renderer;
	GtkTreeViewColumn *column;
	GtkTreeModel *model;

	model = gtk_tree_view_get_model(view);
	renderer = gtk_cell_renderer_text_new();
	g_object_set(renderer, "editable", TRUE, "font", "Monospace Bold 10", "xalign", 0.5, NULL);

	column = gtk_tree_view_column_new_with_attributes(COLUMN_NAME, renderer, "text", COLUMN, NULL);
	g_object_set_data(G_OBJECT(renderer), "column", GINT_TO_POINTER(COLUMN));
	g_object_set(column, "expand", TRUE, "alignment", 0.5, NULL);
	g_object_set_data(G_OBJECT(column), "colnum", GINT_TO_POINTER(COLUMN));
	gtk_tree_view_column_set_sizing(column, GTK_TREE_VIEW_COLUMN_FIXED);
	gtk_tree_view_append_column(GTK_TREE_VIEW(view), column);

	g_signal_connect(G_OBJECT(renderer), "edited", G_CALLBACK(edited_CB), model);
	g_signal_connect(G_OBJECT(renderer), "editing-started", G_CALLBACK(editing_started_CB), NULL);
	g_signal_connect(G_OBJECT(renderer), "editing-canceled", G_CALLBACK(editing_canceled_CB), NULL);
}

