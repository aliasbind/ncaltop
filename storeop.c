#include "storeop.h"
#include "viewmotion.h"

void AddToStore(GtkWidget *widget)
{
	GtkTreeModel *model = gtk_tree_view_get_model(GTK_TREE_VIEW(widget));
	GtkListStore *store = GTK_LIST_STORE(model);
	GtkTreeIter iter, first;
	GtkTreeViewColumn *column;
	GtkTreePath *path;

	if(editing)
	{  
		GtkTreePath *pathopened;
		GtkTreeViewColumn *columnopened;
		GtkCellRenderer *renderer;
		GList *renderers;
		gchar *pathstr; 

		gtk_tree_view_get_cursor(GTK_TREE_VIEW(widget), &pathopened, &columnopened);
		pathstr = gtk_tree_path_to_string(pathopened);
		renderers = gtk_cell_layout_get_cells(GTK_CELL_LAYOUT(columnopened));
		renderer = (GtkCellRenderer *) renderers->data;

		if(editVal) 
			g_signal_emit_by_name(renderer, "edited", pathstr, editVal, model); 
		editVal = NULL;
	}

	gtk_tree_model_get_iter_first(model, &iter);
	gtk_tree_model_get_iter_first(model, &first);

	if(first.stamp != iter.stamp)
		gtk_list_store_insert_before(store, &iter, &first);
	else
		gtk_list_store_insert(store, &iter, 0);

	path = gtk_tree_path_new_first();
	column = gtk_tree_view_get_column(GTK_TREE_VIEW(widget), POINT_ID_COLUMN);

	gtk_tree_view_set_cursor(GTK_TREE_VIEW(widget), path, column, TRUE);
}

void RemoveRow(gpointer data, gpointer user_data)
{
	GtkTreeModel *model = (GtkTreeModel *) user_data;
	GtkTreeIter *iter = (GtkTreeIter *) data;

	gtk_list_store_remove(GTK_LIST_STORE(model), iter);
}

void RemoveFromStore(GtkWidget *widget)
{
	GtkTreeModel *model = gtk_tree_view_get_model(GTK_TREE_VIEW(widget));
	GtkListStore *store = GTK_LIST_STORE(model);

	GtkTreeSelection *selection;
	GtkTreeIter iter, *aux;
	GList *selectedRows, *i;
	gboolean ok;

	selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(widget));
	selectedRows = gtk_tree_selection_get_selected_rows(selection, &model);

	for(i=selectedRows; i; i=i->next)
	{
		gtk_tree_model_get_iter(model, &iter, (GtkTreePath *) i->data);
		gtk_list_store_set(store, &iter, POINT_ID_COLUMN, "*TOBEDELETED*", -1);
	}
	gtk_tree_model_get_iter_first(model, &iter);
	while(gtk_list_store_iter_is_valid(store, &iter))
	{
		ok = FALSE;
		gchar *currentCellValue;
		gtk_tree_model_get(model, &iter, POINT_ID_COLUMN, &currentCellValue, -1);
		if(g_strcmp0(currentCellValue, "*TOBEDELETED*") == 0)
		{
			aux = gtk_tree_iter_copy(&iter);
			ok = gtk_tree_model_iter_next(model, aux);
			gtk_list_store_remove(store, &iter);
			if(!ok)
				break;
			iter = *gtk_tree_iter_copy(aux);
			gtk_tree_iter_free(aux);
			continue;
		}
		gtk_tree_model_iter_next(model, &iter);
	}
}

