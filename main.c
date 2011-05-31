#include "callbacks.h"
#include "storeop.h"
#include "viewmotion.h"

GtkUIManager *makeUI(GtkWidget *view)
{
	static GtkActionEntry entries[] =
	{
		{ "FileMenuAction", NULL, "_File" },
		{ "TableMenuAction", NULL, "_Table" },

		{ "NewAction", GTK_STOCK_NEW, "_New", "<control>N", "Start a new measurement", G_CALLBACK(file_new)},
		{ "OpenAction", GTK_STOCK_OPEN, "_Open", "<control>O", "Open a file", G_CALLBACK(file_open)},
		{ "SaveAction", GTK_STOCK_SAVE, "_Save", "<control>S", "Save table to current file", G_CALLBACK(file_save)},
		{ "SaveAsAction", GTK_STOCK_SAVE_AS, "Sa_ve As...", NULL, "Save a file as...", G_CALLBACK(file_save_as)},
		{ "QuitAction", GTK_STOCK_QUIT, "_Quit", "<control>Q",
			"Quit", G_CALLBACK(file_quit) },


		{ "AddRowAction", GTK_STOCK_ADD, "_Add Row", "grave",
			"Add row to table", G_CALLBACK(table_add_row)},

		{ "RemoveRowAction", GTK_STOCK_REMOVE, "_Remove Row", "Delete",
			"Remove selected rows from table", G_CALLBACK(table_remove_row) },

		{ "GetOrientationAction", GTK_STOCK_CONVERT, "Calculate _Orientation", NULL,
			"Uses table entries to calculate an orientation", G_CALLBACK(create_calc_view) }

	};
	static guint n_entries = G_N_ELEMENTS(entries);

	GtkActionGroup *action_group;
	GtkUIManager *uimanager;
	GError *error;

	action_group = gtk_action_group_new("TestActions");
	gtk_action_group_add_actions(action_group, entries, n_entries, view);

	uimanager = gtk_ui_manager_new();
	gtk_ui_manager_insert_action_group(uimanager, action_group, 0);
	gtk_ui_manager_add_ui_from_file(uimanager, "gui.xml", &error);

	return uimanager;
}

GtkListStore *makeModel()
{
	GtkListStore *store;
	GtkTreeIter iter;

	store = gtk_list_store_new (N_COLUMNS, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING);
	return store;
}

GtkWidget *makeView()
{
	GtkListStore *store;
	GtkWidget *view;
	GtkCellRenderer *renderer;
	GtkTreeViewColumn *column;
	GtkTreeSelection *selection;

	store = makeModel();
	view = gtk_tree_view_new_with_model(GTK_TREE_MODEL(store));

	append_custom_column(GTK_TREE_VIEW(view), POINT_ID_COLUMN, "Point ID");
	append_custom_column(GTK_TREE_VIEW(view), POINT_X_COLUMN, "X");
	append_custom_column(GTK_TREE_VIEW(view), POINT_Y_COLUMN, "Y");

	g_signal_connect(G_OBJECT(view), "key-press-event", G_CALLBACK(KeyStroke), NULL);

	selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(view));
	gtk_tree_selection_set_mode(selection, GTK_SELECTION_MULTIPLE);

	gtk_tree_view_set_grid_lines(GTK_TREE_VIEW(view), GTK_TREE_VIEW_GRID_LINES_HORIZONTAL);
	gtk_tree_view_set_rubber_banding(GTK_TREE_VIEW(view), TRUE);
	gtk_tree_view_set_fixed_height_mode(GTK_TREE_VIEW(view), TRUE);
	return view;
}

GtkWidget *makeWindow()
{
	GtkWidget *win;

	GtkWidget *menubar;
	GtkWidget *toolbar;
	GtkWidget *scrollwindow, *view;

	GtkWidget *boxer;
	GtkUIManager *uimanager;

	boxer = gtk_vbox_new(FALSE, 0);
	win = gtk_window_new(GTK_WINDOW_TOPLEVEL);


	view = makeView();
	uimanager = makeUI(view);
	menubar = gtk_ui_manager_get_widget(uimanager, "/MainMenu");
	toolbar = gtk_ui_manager_get_widget(uimanager, "/MainToolbar");
	gtk_orientable_set_orientation(GTK_ORIENTABLE(toolbar), GTK_ORIENTATION_HORIZONTAL);
	gtk_window_add_accel_group(GTK_WINDOW(win), gtk_ui_manager_get_accel_group(uimanager));

	scrollwindow = gtk_scrolled_window_new(NULL, NULL);
	gtk_container_add(GTK_CONTAINER(scrollwindow), view);
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrollwindow), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

	gtk_box_pack_start(GTK_BOX(boxer), menubar, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(boxer), toolbar, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(boxer), scrollwindow, TRUE, TRUE, 0);
	gtk_container_add(GTK_CONTAINER(win), boxer);

	gtk_window_set_position(GTK_WINDOW(win), GTK_WIN_POS_CENTER);
	gtk_window_set_default_size(GTK_WINDOW(win), 600, 400);

	g_signal_connect(G_OBJECT(win), "destroy", G_CALLBACK(destroy_CB), view);

	return win;
}

int main(int argc, char *argv[])
{
	gtk_init(&argc, &argv);
	safe = 1;
	GtkWidget *win = makeWindow();
	gtk_widget_show_all(win);
	gtk_main();
	return 0;
}
