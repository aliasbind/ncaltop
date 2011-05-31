#include "calcutils.h"
#include "storeop.h"
#include "main.h"
#include "callbacks.h"
#include <math.h>

void setup_view(GtkWidget *view)
{
	GtkTreeSelection *selection;

	selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(view));
	gtk_tree_selection_set_mode(selection, GTK_SELECTION_MULTIPLE);

	gtk_tree_view_set_grid_lines(GTK_TREE_VIEW(view), GTK_TREE_VIEW_GRID_LINES_HORIZONTAL);
	gtk_tree_view_set_rubber_banding(GTK_TREE_VIEW(view), TRUE);
	gtk_tree_view_set_fixed_height_mode(GTK_TREE_VIEW(view), TRUE);
}

void calc(gdouble xbase, gdouble ybase, 
		gdouble xorient, gdouble yorient,
		gdouble xpoint, gdouble ypoint,
		gchar **xval, gchar **yval)
{
	gdouble distance, angle;
	distance = sqrt((xbase - xpoint) * (xbase - xpoint) + (ybase - ypoint) * (ybase - ypoint));
	*xval = malloc(20);
	*yval = malloc(20);
	sprintf((*xval), "%.3f", distance);

	if(xbase == xorient == xpoint && ypoint > ybase)
	{
		sprintf(*yval, "%.3f", 0.000);
		return;
	}

	if(xbase == xpoint)
	{
		sprintf(*yval, "%.3f", 180.0f);
		return;
	}
	gdouble dist_a, dist_b, dist_c;
	dist_a = sqrt( (xorient - xbase) * (xorient - xbase) + (yorient - ybase) * (yorient - ybase) );
	dist_b = distance;
	dist_c = sqrt( (xorient - xpoint) * (xorient - xpoint) + (yorient - ypoint) * (yorient - ypoint) );

	gdouble semiperimeter;
	semiperimeter = (dist_a + dist_b + dist_c) / 2;

	gdouble area;
	area = sqrt( semiperimeter * (semiperimeter - dist_a) * (semiperimeter - dist_b) * (semiperimeter - dist_c) );

	gdouble cos_a;
	cos_a = ( dist_a * dist_a + dist_b * dist_b - dist_c * dist_c ) / (2 * dist_a * dist_b);

	angle = acos(cos_a) * 180 / M_PI;

	if( xbase * (yorient - ypoint) + xorient * (ypoint - ybase) + xpoint * (ybase - yorient) > 0 )
		angle = 360 - angle;
	sprintf(*yval, "%.3f", angle);
	return;
}

void compute_values(GtkListStore *store, gchar **xval, gchar **yval)
{
	GtkTreeModel *model;
	GtkTreeIter iter;

	model = GTK_TREE_MODEL(store);
    gchar *xb, *yb, *xo, *yo;
	gtk_tree_model_get_iter_first(model, &iter);
	gtk_tree_model_get(model, &iter, 1, &xb, 2, &yb, -1);
	gtk_tree_model_iter_next(model, &iter);
	gtk_tree_model_get(model, &iter, 1, &xo, 2, &yo, -1);

	gdouble xbase, ybase, xorient, yorient, xpoint, ypoint;
	gdouble distance, angle;

	xbase = g_ascii_strtod(xb, NULL);
	ybase = g_ascii_strtod(yb, NULL);
	xorient = g_ascii_strtod(xo, NULL);
	yorient = g_ascii_strtod(yo, NULL);
	xpoint = g_ascii_strtod(*xval, NULL);
	ypoint = g_ascii_strtod(*yval, NULL);

	//distance = sqrt((xbase - xpoint) * (xbase - xpoint) + (ybase - ypoint) * (ybase - ypoint));
	//*xval = g_strdup_printf("%.3f", distance);

	//if(xbase == xorient == xpoint && ypoint > ybase)
	//{
		//*yval = g_strdup_printf("%.3f", 0.000);
		//return;
	//}

	//if(xbase == xpoint)
	//{
		//*yval = g_strdup_printf("%.3f", 180.0f);
		//return;
	//}

	//if(xbase == xorient)
	//{
		//gdouble dist_a, dist_b, dist_c;
		//dist_a = ybase - yorient > 0 ? ybase - yorient : yorient - ybase;
		//dist_b = distance;
		//dist_c = sqrt( (xorient - xpoint) * (xorient - xpoint) + (yorient - ypoint) * (yorient - ypoint) );

		//gdouble semiperimeter;
		//semiperimeter = (dist_a + dist_b + dist_c) / 2;

		//gdouble area;
		//area = sqrt( semiperimeter * (semiperimeter - dist_a) * (semiperimeter - dist_b) * (semiperimeter - dist_c) );

		//gdouble cos_a;
		//cos_a = ( dist_a * dist_a + dist_b * dist_b - dist_c * dist_c ) / (2 * dist_a * dist_b);

		//angle = acos(cos_a) * 180 / M_PI;

		//if( (xpoint < xbase && ybase < yorient) || (xpoint > xbase && ybase > yorient) )
			//angle = 360 - angle;

		//*yval = g_strdup_printf("%.3f", angle);
		//return;
	//}

	//if(xbase < xorient && ybase < yorient && xpoint < xbase)
		//angle += 180;

	//gdouble a1, b1, c1;
	//gdouble a2, b2, c2;

	//a1 = yorient - ybase;
	//b1 = xbase - xorient;
	//c1 = a1 * (-xbase) + b1 * (-ybase);

	//a2 = ypoint - ybase;
	//b2 = xbase - xpoint;
	//c2 = a2 * (-xbase) + b2 * (-ybase);

	//g_print("%fx + %fy + %f\n", a1, b1, c1);
	//g_print("%fx + %fy + %f\n\n", a2, b2, c2);
	//gdouble m1, m2;

	//m1 = - (a1 / b1);
	//m2 = - (a2 / b2);

	//g_print("PANTA BASE-ORIENT %.3f\n", m1);
	//g_print("PANTA BASE-POINT %.3f\n", m2);

	//angle = - (atan( (m2 - m1) / (1 + (m1 * m2)) ) * 180 / M_PI);
	//if(m2 > m1)
		//angle += 180;
	//*yval = g_strdup_printf("%.3f", angle);
    calc(xbase, ybase, xorient, yorient, xpoint, ypoint, xval, yval);
}

void save_results_to_file(gchar *file, GtkWidget *bo_view, GtkWidget *points_view)
{
	GtkTreeModel *bo_model = gtk_tree_view_get_model(GTK_TREE_VIEW(bo_view));
	GtkTreeModel *points_model = gtk_tree_view_get_model(GTK_TREE_VIEW(points_view));

	FILE *f;
	f = fopen(file, "w");

	GtkTreeIter iter;
	gchar *id, *x, *y;
	gtk_tree_model_get_iter_first(bo_model, &iter);
	do
	{
		gtk_tree_model_get(bo_model, &iter, 0, &id, 1, &x, 2, &y, -1);
		fprintf(f, "%s\t%s\t%s\n", id, x, y);
	}
	while(gtk_tree_model_iter_next(bo_model, &iter));

	fprintf(f, "*****\n");

	gtk_tree_model_get_iter_first(points_model, &iter);
	do
	{
		gtk_tree_model_get(points_model, &iter, 0, &id, 1, &x, 2, &y, -1);
		fprintf(f, "%s\t%s\t%s\n", id, x, y);
	}
	while(gtk_tree_model_iter_next(points_model, &iter));
}

void save_results(GtkToolButton *toolitem, gpointer data)
{
	GtkWidget *main_view, *bo_view, *points_view;
	GtkWidget *main_window;
	GList *user_data = data;
	main_view = user_data->data;
	bo_view = user_data->next->data;
	points_view = user_data->next->next->data;
	main_window = gtk_widget_get_parent(main_view);

	gint i;
	for(i=1; i<=4; i++)
		main_window = gtk_widget_get_parent(main_window);

	GtkWidget *save_dialog;
	save_dialog = gtk_file_chooser_dialog_new("Save file as", GTK_WINDOW(main_window), GTK_FILE_CHOOSER_ACTION_SAVE, 
			GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL, GTK_STOCK_SAVE, GTK_RESPONSE_ACCEPT, NULL);
	if(gtk_dialog_run(GTK_DIALOG(save_dialog)) == GTK_RESPONSE_ACCEPT)
	{
		gchar *file_results_name = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(save_dialog));
		save_results_to_file(file_results_name, bo_view, points_view);
	}
	gtk_widget_destroy(save_dialog);
}

void send_right(GtkToolButton *toolitem, gpointer data)
{
	GtkWidget *main_view, *bo_view, *points_view;
	GList *user_data = data;
	main_view = user_data->data;
	bo_view = user_data->next->data;
	points_view = user_data->next->next->data;

	GtkListStore *main_store, *bo_store, *points_store;
	main_store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(main_view)));
	bo_store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(bo_view)));
	points_store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(points_view)));

	GtkTreeSelection *selection;
	selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(main_view));

	GList *rows, *it;
	rows = gtk_tree_selection_get_selected_rows(selection, NULL); 

	GtkTreeIter iter;
	gchar *point_id, *xval, *yval;
	gint rows_bo;
	for(it=rows; it; it=it->next)
	{
		gtk_tree_model_get_iter(GTK_TREE_MODEL(main_store), &iter, (GtkTreePath *) it->data);
		gtk_tree_model_get(GTK_TREE_MODEL(main_store), &iter, 0, &point_id, 1, &xval, 2, &yval, -1);
		rows_bo = gtk_tree_model_iter_n_children(GTK_TREE_MODEL(bo_store), NULL); 
		if(rows_bo < 2)
		{
			gtk_list_store_append(bo_store, &iter);
			gtk_list_store_set(bo_store, &iter, 0, point_id, 1, xval, 2, yval, -1);
		}
		else
		{
			gtk_list_store_append(points_store, &iter);
			compute_values(bo_store, &xval, &yval);
			gtk_list_store_set(points_store, &iter, 0, point_id, 1, xval, 2, yval, -1);
		}
	}
}

void send_left(GtkToolButton *toolitem, gpointer data)
{
	GtkWidget *bo_view, *points_view;
	GList *user_data = data;
	bo_view = user_data->next->data;
	points_view = user_data->next->next->data;

	RemoveFromStore(bo_view);
	RemoveFromStore(points_view);

	GtkTreeModel *bo_model, *points_model;
	bo_model = gtk_tree_view_get_model(GTK_TREE_VIEW(bo_view));
	points_model = gtk_tree_view_get_model(GTK_TREE_VIEW(points_view));
}

GtkWidget *make_bottom_buttons(GtkWidget *main_view, GtkWidget *bo_view, GtkWidget *points_view)
{
	GList *view_list = NULL;
	view_list = g_list_append(view_list, main_view);
	view_list = g_list_append(view_list, bo_view);
	view_list = g_list_append(view_list, points_view);

	GtkWidget *hbox, *toolbar;
	GtkToolItem *toolitem;


	hbox = gtk_hbox_new(FALSE, 0);
	toolbar = gtk_toolbar_new();

	toolitem = gtk_tool_button_new_from_stock(GTK_STOCK_GO_BACK);
	gtk_toolbar_insert(GTK_TOOLBAR(toolbar), toolitem, 0);
	g_signal_connect(G_OBJECT(toolitem), "clicked", G_CALLBACK(send_left), view_list);

	toolitem = gtk_tool_button_new_from_stock(GTK_STOCK_GO_FORWARD);
	gtk_toolbar_insert(GTK_TOOLBAR(toolbar), toolitem, 1);
	g_signal_connect(G_OBJECT(toolitem), "clicked", G_CALLBACK(send_right), view_list);

	toolitem = gtk_tool_button_new_from_stock(GTK_STOCK_SAVE);
	gtk_toolbar_insert(GTK_TOOLBAR(toolbar), toolitem, 2);
	g_signal_connect(G_OBJECT(toolitem), "clicked", G_CALLBACK(save_results), view_list);

	gtk_box_pack_end(GTK_BOX(hbox), toolbar, TRUE, TRUE, 0);

	return hbox;
}

void build_view(GtkWidget *main_window)
{
	GtkWidget *main_box;
	GtkWidget *main_view;
	GtkWidget *main_scrolled;

	GList *kids;

	kids = gtk_container_get_children(GTK_CONTAINER(main_window));
	main_box = (GtkWidget *) kids->data;

	kids = gtk_container_get_children(GTK_CONTAINER(main_box));
	main_scrolled = kids->next->next->data;

	kids = gtk_container_get_children(GTK_CONTAINER(main_scrolled));
	main_view = (GtkWidget *) kids->data;

	GtkWidget *bo_view, *points_view;
	GtkListStore *bo_store, *points_store;
	GtkTreeIter iter;

	bo_store = makeModel();
	bo_view = gtk_tree_view_new_with_model(GTK_TREE_MODEL(bo_store));
	append_custom_column(bo_view, 0, "Point ID");
	append_custom_column(bo_view, 1, "X");
	append_custom_column(bo_view, 2, "Y");

	points_store = makeModel();
	points_view = gtk_tree_view_new_with_model(GTK_TREE_MODEL(points_store));
	append_custom_column(points_view, 0, "Point ID");
	append_custom_column(points_view, 1, "Distance");
	append_custom_column(points_view, 2, "Orientation");

	setup_view(bo_view);
	setup_view(points_view);

	GtkWidget *scrolled_bo, *scrolled_points;
	scrolled_bo = gtk_scrolled_window_new(NULL, NULL);
	gtk_container_add(GTK_CONTAINER(scrolled_bo), bo_view);
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_bo), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

	scrolled_points = gtk_scrolled_window_new(NULL, NULL);
	gtk_container_add(GTK_CONTAINER(scrolled_points), points_view);
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_points), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

	GtkWidget *r_side_box1, *r_side_box2, *label1, *label2, *vpaned;
	vpaned = gtk_vpaned_new();

	label1 = gtk_label_new("Input the base and orientation point below:");
	gtk_label_set_line_wrap(GTK_LABEL(label1), TRUE);
	label2 = gtk_label_new("Input the rest of the points for which you wish to find the orientation and distance:");
	gtk_label_set_line_wrap(GTK_LABEL(label2), TRUE);

	r_side_box1 = gtk_vbox_new(FALSE, 0);
	gtk_box_pack_start(GTK_BOX(r_side_box1), label1, FALSE, FALSE, 3);
	gtk_box_pack_start(GTK_BOX(r_side_box1), scrolled_bo, FALSE, FALSE, 0);
	gtk_paned_pack1(GTK_PANED(vpaned), r_side_box1, TRUE, FALSE);

	r_side_box2 = gtk_vbox_new(FALSE, 0);
	gtk_box_pack_start(GTK_BOX(r_side_box2), label2, FALSE, FALSE, 3);
	gtk_box_pack_start(GTK_BOX(r_side_box2), scrolled_points, TRUE, TRUE, 0);
	gtk_paned_pack2(GTK_PANED(vpaned), r_side_box2, TRUE, TRUE);
	gtk_paned_set_position(GTK_PANED(vpaned), 0);

	GtkWidget *smallvbox;
	smallvbox = gtk_vbox_new(TRUE, 0);

	GtkWidget *add, *remove;

	add = gtk_button_new_with_label(">");
	remove = gtk_button_new_with_label("<");

	gtk_box_pack_start(GTK_BOX(smallvbox), add, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(smallvbox), remove, FALSE, FALSE, 0);


	GtkWidget *hbox;
	GtkWidget *l_side_box;
	GtkWidget *l_bottom_h_box;
	GtkWidget *new_scrolled;
	GtkWidget *hpaned;
	gint x, y;

	l_side_box = gtk_vbox_new(FALSE, 0);
	l_bottom_h_box = make_bottom_buttons(main_view, bo_view, points_view);
	hpaned = gtk_hpaned_new();

	//gtk_window_get_size(GTK_WINDOW(main_window), &x, &y);
	gtk_widget_reparent(main_scrolled, l_side_box);
	gtk_box_pack_start(GTK_BOX(l_side_box), l_bottom_h_box, FALSE, FALSE, 0);

	gtk_paned_pack1(GTK_PANED(hpaned), l_side_box, TRUE, TRUE);
	gtk_paned_pack2(GTK_PANED(hpaned), vpaned, TRUE, TRUE);
	gtk_paned_set_position(GTK_PANED(hpaned), 300);

	//hbox = gtk_hbox_new(TRUE, 0);
	//gtk_box_set_child_packing(GTK_BOX(hbox), main_scrolled, TRUE, TRUE, 0, GTK_PACK_START);
	//gtk_box_pack_start(GTK_BOX(hbox), smallvbox, FALSE, FALSE, 0);
	//gtk_box_pack_start(GTK_BOX(hbox), r_side_box, TRUE, TRUE, 0);

	gtk_box_pack_start(GTK_BOX(main_box), hpaned, TRUE, TRUE, 0);
	gtk_box_reorder_child(GTK_BOX(main_box), hpaned, 2);
	gtk_widget_show_all(main_box);
}
