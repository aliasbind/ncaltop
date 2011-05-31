#include "viewmotion.h"

void tab_right(GtkWidget *widget, 
		GtkTreeModel *model, 
		GtkTreeIter iter, 
		GtkTreePath *path,
		GtkTreeViewColumn *column, 
		gint colnum)
{
	if(colnum < POINT_Y_COLUMN)
		column = gtk_tree_view_get_column(GTK_TREE_VIEW(widget), colnum+1);
	else
	{
		if(gtk_tree_model_iter_next(model, &iter))
			gtk_tree_path_next(path);
		else
			path = gtk_tree_path_new_first();
		column = gtk_tree_view_get_column(GTK_TREE_VIEW(widget), POINT_ID_COLUMN);
	}
	gtk_tree_view_set_cursor(GTK_TREE_VIEW(widget), path, column, TRUE);
}

void tab_left(GtkWidget *widget, 
		GtkTreeModel *model, 
		GtkTreePath *path,
		GtkTreeViewColumn *column, 
		gint colnum,
		gint rownum)
{
	rownum = gtk_tree_model_iter_n_children(model, NULL) - 1;
	if(colnum > POINT_ID_COLUMN)
		column = gtk_tree_view_get_column(GTK_TREE_VIEW(widget), colnum-1);
	else
	{
		if(atoi(gtk_tree_path_to_string(path)))
			gtk_tree_path_prev(path);
		else
			path = gtk_tree_path_new_from_string(g_strdup_printf("%d", rownum));
		column = gtk_tree_view_get_column(GTK_TREE_VIEW(widget), POINT_Y_COLUMN);
	}
	gtk_tree_view_set_cursor(GTK_TREE_VIEW(widget), path, column, TRUE);
}

void move_up(GtkWidget *widget, 
		GtkTreeModel *model, 
		GtkTreePath *path,
		GtkTreeViewColumn *column, 
		gint rownum,
		gchar *pathstr)
{
	rownum = gtk_tree_model_iter_n_children(model, NULL) - 1;
	if(g_strcmp0(pathstr,"0"))
		path = gtk_tree_path_new_from_string(g_strdup_printf("%d", atoi(pathstr) -1));
	else
		path = gtk_tree_path_new_from_string(g_strdup_printf("%d", rownum));
	gtk_tree_view_set_cursor(GTK_TREE_VIEW(widget), path, column, FALSE);
}

void move_down(GtkWidget *widget, 
		GtkTreeModel *model, 
		GtkTreePath *path,
		GtkTreeViewColumn *column, 
		gint rownum,
		gchar *pathstr)
{
	rownum = gtk_tree_model_iter_n_children(model, NULL) - 1;
	if(g_strcmp0(pathstr,g_strdup_printf("%d", rownum)))
		path = gtk_tree_path_new_from_string(g_strdup_printf("%d", atoi(pathstr) +1));
	else
		path = gtk_tree_path_new_from_string(g_strdup_printf("%d", 0));
	gtk_tree_view_set_cursor(GTK_TREE_VIEW(widget), path, column, FALSE);
}

void move_left(GtkWidget *widget, 
		GtkTreeModel *model, 
		GtkTreePath *path,
		GtkTreeViewColumn *column, 
		gint colnum)
{
	if(colnum > POINT_ID_COLUMN)
		column = gtk_tree_view_get_column(GTK_TREE_VIEW(widget), colnum-1);
	else
		column = gtk_tree_view_get_column(GTK_TREE_VIEW(widget), POINT_Y_COLUMN);
	gtk_tree_view_set_cursor(GTK_TREE_VIEW(widget), path, column, FALSE);
}

void move_right(GtkWidget *widget, 
		GtkTreeModel *model, 
		GtkTreePath *path,
		GtkTreeViewColumn *column, 
		gint colnum)
{
	if(colnum < POINT_Y_COLUMN)
		column = gtk_tree_view_get_column(GTK_TREE_VIEW(widget), colnum+1);
	else
		column = gtk_tree_view_get_column(GTK_TREE_VIEW(widget), POINT_ID_COLUMN);
	gtk_tree_view_set_cursor(GTK_TREE_VIEW(widget), path, column, FALSE);
}

void KeyStroke(GtkWidget *widget, GdkEventKey *event, gpointer data)
{
	if(event->keyval == KB_SHIFT)
		return;

	GtkTreeModel *model;
	GtkListStore *store;
	GtkTreePath *path;
	GtkTreeViewColumn *column;
	GtkTreeIter iter;
	GtkCellRenderer *renderer;
	GList *renderers;

	gint colnum;
	gint rownum;
	gchar *pathstr;

	model = gtk_tree_view_get_model(GTK_TREE_VIEW(widget));
	store = GTK_LIST_STORE(model);

	gtk_tree_view_get_cursor(GTK_TREE_VIEW(widget), &path, &column);
	gtk_tree_model_get_iter(model, &iter, path);

	renderers = gtk_cell_layout_get_cells(GTK_CELL_LAYOUT(column));
	renderer = (GtkCellRenderer *) renderers->data;

	colnum = GPOINTER_TO_INT(g_object_get_data(G_OBJECT(column), "colnum"));
	pathstr = gtk_tree_path_to_string(path);

	if(editVal && editing)
		g_signal_emit_by_name(renderer, "edited", pathstr, editVal, model); 
	editVal = NULL;

	switch(event->keyval)
	{
		case KB_TAB:
			tab_right(widget, model, iter, path, column, colnum);
			break;

		case KB_SHIFT_TAB:
			tab_left(widget, model, path, column, colnum, rownum);
			break;

		case KB_UP:
			move_up(widget, model, path, column, rownum, pathstr);
			break;

		case KB_DOWN:
			move_down(widget, model, path, column, rownum, pathstr);
			break;

		case KB_LEFT:
			move_left(widget, model, path, column, colnum);
			break;

		case KB_RIGHT:
			move_right(widget, model, path, column, colnum);
			break;

		case KB_ENTER:
			gtk_tree_view_set_cursor(GTK_TREE_VIEW(widget), path, column, TRUE);
			break;
	}
	gtk_tree_path_free(path);
}
