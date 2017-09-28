#include <gtk/gtk.h>

static void activate(GtkApplication* app, gpointer user_data) {
    int i, x, y;
    unsigned int seed;
    const int CELLS = 40;
    char alphabet[37] = {'a','b','c','d','e','f','g','h','i','j','k','l','m',
        'n','o','p','q','r','s','t','u','v','w','x','y','x','.',',','?','!',' '};
    
    GtkWidget *window;
    GtkWidget *grid;
    GtkWidget *plaintext;
    GtkWidget *key;
    GtkWidget *button_box;
    GtkWidget *button;
    GtkWidget *output_grid;
    GtkWidget *output;
    GtkWidget *cells[CELLS];

    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Colta");
    //gtk_window_set_default_size(GTK_WINDOW(window), 500, 500);

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

    //gtk_override_background_color(GTK_WIDGET(cells[0]), GTK_STATE_FLAG_NORMAL,

    output_grid = gtk_grid_new();
    //gtk_grid_set_column_spacing(GTK_GRID(grid), 2);
    for (i=0; i<CELLS; i++) {
        cells[i] = gtk_label_new("██");
    }

    i=0;
    for (y=0; y<4; y++) {
        for (x=0; x<10; x++) {
            gtk_grid_attach(GTK_GRID(output_grid), cells[i], x, y, 1, 1);
            i++;
        }
    }

    gtk_grid_attach(GTK_GRID(grid), output_grid, 0, 2, 2, 1);

    gtk_widget_show_all(window);
}

void encode() {
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
// seed between 0 to 4'294'967'295
