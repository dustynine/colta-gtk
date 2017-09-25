#include <gtk/gtk.h>

static void activate(GtkApplication* app, gpointer user_data) {
    int i;
    const int CELLS = 40;
    
    GtkWidget *window;
    GtkWidget *grid;
    GtkWidget *plaintext;
    GtkWidget *key;
    GtkWidget *button_box;
    GtkWidget *button;
    GtkWidget *output_grid;
    GtkWidget *output;

    GtkWidget *cells[CELLS];
    //GtkWidget *color_1_1, *color_1_2, *color_1_3, *color_1_4, *color_1_5, *color_1_6,
    //          *color_1_7, *color_1_8, *color_1_9, *color_1_10, *color_2_1, *color_2_2,
    //          *color_2_3, *color_2_4, *color_2_5, *color_2_6, *color_2_7, *color_2_8, 
    //          *color_2_9, *color_2_10, *color_3_1, *color_3_2, *color_3_3, *color_3_4,
    //          *color_3_5, *color_3_6, *color_3_7, *color_3_8, *color_3_9, *color_3_10, 
    //          *color_4_1, *color_4_2, *color_4_3, *color_4_4, *color_4_5, *color_4_6,
    //          *color_4_7, *color_4_8, *color_4_9, *color_4_10;

    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Colta");
    gtk_window_set_default_size(GTK_WINDOW(window), 500, 500);

    grid = gtk_grid_new();
    //gtk_grid_set_column_homogeneous(GTK_GRID(grid), 1);
    //gtk_grid_set_row_homogeneous(GTK_GRID(grid), 1);
    gtk_grid_set_row_spacing(GTK_GRID(grid), 3);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 3);
    gtk_container_add(GTK_CONTAINER(window), grid);

    plaintext = gtk_text_view_new();
    gtk_text_view_set_editable(GTK_TEXT_VIEW(plaintext), 1);
    gtk_grid_attach(GTK_GRID(grid), plaintext, 0, 0, 2, 1);

    key = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), key, 0, 1, 1, 1);

    button_box = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_grid_attach(GTK_GRID(grid), button_box, 1, 1, 1, 1);

    button = gtk_button_new_with_label("Encode");
    gtk_container_add(GTK_CONTAINER(button_box), button);

    for (i=0, i<CELLS, i++) {
        cells[i] = gtk_label_new()

    output_grid = gtk_grid_new();
    gtk_grid_attach(GTK_GRID(grid), output_grid, 0, 1, 2, 1);
    //gtk_grid_attach(GTK_GRID(output_grid), button_box, 1, 1, 1, 1);

    gtk_widget_show_all(window);
}


int main(int argc, char **argv) {
    GtkApplication *app;
    int status;

    app = gtk_application_new
          ("com.github.dustynine.colta", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}

// 255 characters = 4 rows 10 columns of colors
