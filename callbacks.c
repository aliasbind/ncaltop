#include "viewmotion.h"
#include "callbacks.h"
#include "calcutils.h"
#include <stdio.h>
#include <stdlib.h>


void destroy_CB(GtkWidget *widget, gpointer data)
{
	GtkAction *action;
	file_quit(action, data);
}

void editing_canceled_CB(GtkCellRenderer *renderer, gpointer data)
{
	editing = 0;
}

void buffer_CB(GtkEntryBuffer *buffer, guint arg1, gchar *arg2, guint arg3, gpointer data)
{
	editVal = gtk_entry_buffer_get_text(buffer);
	safe = 0;
}

void editing_started_CB(GtkCellRenderer *renderer, GtkCellEditable *editable, const gchar *path, gpointer data)
{
	editing = 1;
	GtkEntryBuffer *buffer;
	buffer = gtk_entry_get_buffer(GTK_ENTRY(editable));
	g_signal_connect(buffer, "inserted-text", G_CALLBACK(buffer_CB), NULL);
	g_signal_connect(editable, "editing-done", G_CALLBACK(editing_canceled_CB), NULL);

	PangoFontDescription *font = pango_font_description_from_string("Monospace Bold 10");
	gtk_widget_modify_font(GTK_WIDGET(editable), font);
}

void edited_CB(GtkCellRendererText *renderer, gchar *path_string, gchar *new_text, gpointer data)
{
	GtkTreeModel *model;
	GtkTreePath *path;
	GtkTreeIter iter;
	gint column;

	model = (GtkTreeModel *) data;
	path = gtk_tree_path_new_from_string(path_string);
	column = GPOINTER_TO_INT (g_object_get_data (G_OBJECT (renderer), "column")); 

	gtk_tree_model_get_iter(model, &iter, path);
	gtk_list_store_set(GTK_LIST_STORE(model), &iter, column, new_text, -1);
}

void table_add_row(GtkAction *action, gpointer data)
{
	GtkWidget *widget = (GtkWidget *) data;
	AddToStore(widget);
}

void table_remove_row(GtkAction *action, gpointer data)
{
	GtkWidget *widget = (GtkWidget *) data;
	RemoveFromStore(widget);
}

void file_new(GtkAction *action, gpointer data)
{
	GtkTreeModel *model;

	filename = "";

	model = gtk_tree_view_get_model((GtkTreeView *) data);
	gtk_list_store_clear(GTK_LIST_STORE(model));
}

void save_to_file(gchar *filename, gpointer data)
{
	safe = 1;
	GtkTreeModel *model;
	GtkTreeIter iter;
	gchar *id, *x, *y;
	FILE *f = fopen(filename, "w");

	model = gtk_tree_view_get_model( (GtkTreeView *) data);
	gtk_tree_model_get_iter_first(model, &iter);
	do
	{
		gtk_tree_model_get(model, &iter, POINT_ID_COLUMN, &id, POINT_X_COLUMN, &x, POINT_Y_COLUMN, &y, -1);
		fprintf(f, "%s\t%s\t%s\n", id, x, y);
	}
	while(gtk_tree_model_iter_next(model, &iter));
	fclose(f);
}

void open_file(gchar *filename, gpointer data)
{
	GtkTreeModel *model;
	GtkListStore *store;
	GtkTreeIter iter;
	gchar *id, *x, *y;

	FILE *f = fopen(filename, "r");

	id = (gchar *) malloc(sizeof(gchar) * 50);
	x = (gchar *) malloc(sizeof(gchar) * 50);
	y = (gchar *) malloc(sizeof(gchar) * 50);

	model = gtk_tree_view_get_model( (GtkTreeView *) data);
	store = GTK_LIST_STORE(model);
	gtk_tree_model_get_iter_first(model, &iter);
	while(!feof(f))
	{
		fscanf(f, "%s\t%s\t%s\n", id, x, y);
		gtk_list_store_append(store, &iter);
		gtk_list_store_set(store, &iter, POINT_ID_COLUMN, id, POINT_X_COLUMN, x, POINT_Y_COLUMN, y, -1);
	}
	g_free(id);
	g_free(x);
	g_free(y);
}

void file_open(GtkAction *action, gpointer data)
{
	GtkWidget *dialog, *widget;
	widget = (GtkWidget *) data;
	widget = gtk_widget_get_parent(widget);
	widget = gtk_widget_get_parent(widget);
	widget = gtk_widget_get_parent(widget);
	if(g_strcmp0(G_OBJECT_TYPE_NAME(widget), "GtkHPaned") == 0)
	{
		widget = gtk_widget_get_parent(widget);
		widget = gtk_widget_get_parent(widget);
	}
	if(!safe)
	{
		dialog = gtk_message_dialog_new(GTK_WINDOW(widget), GTK_DIALOG_DESTROY_WITH_PARENT,
				GTK_MESSAGE_QUESTION, GTK_BUTTONS_YES_NO, "Do you want to save first?", NULL);
		if(gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_YES)
		{
			if(!filename)
				file_save_as(action, data);
			else
				file_save(action, data);
		}
		gtk_widget_destroy(dialog);
	}
	dialog = gtk_file_chooser_dialog_new("Open file", GTK_WINDOW(widget), GTK_FILE_CHOOSER_ACTION_OPEN, GTK_STOCK_CANCEL,
			GTK_RESPONSE_CANCEL, GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT, NULL);
	if(gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT)
	{
		gtk_list_store_clear(GTK_LIST_STORE(gtk_tree_view_get_model( (GtkTreeView *) data)));
		filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
		open_file(filename, data);
	}
	gtk_widget_destroy(dialog);
	safe = 1;
}
void file_save(GtkAction *action, gpointer data)
{
	if(!filename)
	{
		file_save_as(action, data);
		return;
	}
	save_to_file(filename, data);
	safe = 1;
}

void file_save_as(GtkAction *action, gpointer data)
{
	GtkWidget *dialog, *widget;

	widget = (GtkWidget *) data;
	widget = gtk_widget_get_parent(widget);
	widget = gtk_widget_get_parent(widget);
	widget = gtk_widget_get_parent(widget);
	dialog = gtk_file_chooser_dialog_new("Save file as", GTK_WINDOW(widget), GTK_FILE_CHOOSER_ACTION_SAVE, GTK_STOCK_CANCEL,
			GTK_RESPONSE_CANCEL, GTK_STOCK_SAVE, GTK_RESPONSE_ACCEPT, NULL);
	gtk_file_chooser_set_do_overwrite_confirmation(GTK_FILE_CHOOSER(dialog), TRUE);

	gtk_widget_show(dialog);
	if(gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT)
	{
		filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
		save_to_file(filename, data);
	}
	gtk_widget_destroy(dialog);
	safe = 1;
}

void file_quit(GtkAction *action, gpointer data)
{
	/*if(safe)
	  {
	  gtk_main_quit();
	  return;
	  }

	  GtkWidget *dialog, *widget;
	  widget = (GtkWidget *) data;
	  widget = gtk_widget_get_parent(widget);
	  widget = gtk_widget_get_parent(widget);
	  dialog = gtk_message_dialog_new(GTK_WINDOW(widget), GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_QUESTION,
	  GTK_BUTTONS_YES_NO, "Save before you exit?", NULL);
	  if(gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_YES)
	  file_save(action, data);
	  else*/
	gtk_main_quit();
}

void create_calc_view(GtkAction *action, gpointer data)
{
	GtkWidget *window;
	GtkWidget *scrollable;
	GtkWidget *box;
	GtkWidget *hpaned;
	window = gtk_widget_get_parent(data);
	scrollable = window;
	window = gtk_widget_get_parent(window);
	window = gtk_widget_get_parent(window);

	//Following lines handle toggling between "calculation mode" and "edit mode"
	if(g_strcmp0(G_OBJECT_TYPE_NAME(window), "GtkWindow") == 0 && !toggle)
	{
		build_view(window);
		toggle = 1;
	}//First Use
	else
	{
		if(g_strcmp0(G_OBJECT_TYPE_NAME(window), "GtkHPaned") == 0)
		{
			hpaned = window;
			backup = hpaned;
			box = gtk_widget_get_parent(window);
			g_object_ref(G_OBJECT(backup));
			gtk_container_remove(GTK_CONTAINER(box), hpaned);
			gtk_widget_reparent(scrollable, box);
		}//To edit mode
		else
		{
			GList *hlist = gtk_container_get_children(GTK_CONTAINER(backup));
			GtkWidget *l_side_pan = hlist->data;
			box = gtk_container_get_children(GTK_CONTAINER(window))->data;
			g_object_ref(G_OBJECT(scrollable));
			gtk_container_remove(GTK_CONTAINER(box), scrollable);
			gtk_container_add(GTK_CONTAINER(l_side_pan), scrollable);
			gtk_box_reorder_child(GTK_BOX(l_side_pan), scrollable, 0);
			g_object_unref(G_OBJECT(scrollable));
			gtk_container_add(GTK_CONTAINER(box), backup);
		}//back to calculation mode
	}
}
