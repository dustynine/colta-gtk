#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>

#define LETTERS 31
#define CELLS 40
#define CAPACITY 122
#define TWO_TENS 256
#define FOUR_TENS 65536

const gchar ALPHABET[LETTERS] =
    {'a','b','c','d','e','f','g','h','i','j','k', 'l','m', 'n','o','p','q','r',
     's','t','u','v','w','x','y','z','.',',','?','!',' '};
const int DEFAULT_CODE[LETTERS] =
    {7,14,21,28,35,42,49,56,63,70,77,84,91,98,105,112,119,
     126,133,140,147,154,161,168,175,182,189,196,203,210,217};

struct template {
    GtkWidget *window;
    GtkWidget *grid;
    GtkWidget *plaintext;
    GtkWidget *key;
    GtkWidget *button_box;
    GtkWidget *button;
    GtkWidget *output_grid;
    GtkWidget *cells[CELLS];
};

struct passed_widgets {
    GtkWidget *plaintext;
    GtkWidget *key;
    GtkWidget **cls;
};


int *make_triplets(gchar str[]) {
    int start, i, j, k=0;
    int temp[CAPACITY];
    static int triplets[CELLS];

    printf("        str = %s\n", str);

    for (i = 0; str[i] != '\0'; i++) {
        for (j = 0; str[i] != ALPHABET[j]; j++);
        printf("        DEFAULT_CODE[%i] = 0x%x\n", j, DEFAULT_CODE[j]);
        temp[i] = DEFAULT_CODE[j];
        printf("    temp[%i] = %i\n", i, temp[i]);
    }

    if (i % 3 == 2)
        temp[i++] = DEFAULT_CODE[30];

    if (i % 3 == 1) {
        temp[i++] = DEFAULT_CODE[30];
        temp[i] = DEFAULT_CODE[30];
    }

    for (start = 0; start < i; start+=3) {
        triplets[k] = temp[start]*FOUR_TENS + temp[start+1]*TWO_TENS + temp[start+2];
        k++;
    }
    return triplets;
}


gchar *strip_string(gchar str[]) {
    int i, j, k = 0;
    static gchar cleantext[CAPACITY];
    for (i = 0; str[i] != '\0'; i++) {
        for (j = 0; str[i] != ALPHABET[j] && j < LETTERS; j++);
        if (j == LETTERS) {
            continue;
        }
        else {
            cleantext[k] = ALPHABET[j];
            k++;
        }
    }
    cleantext[k] = '\0';
    return cleantext;
}


static void encode(GtkButton *button, gpointer data) {
    printf("encoding initiated:\n");

    int k = 0;
    int *triplets;
    gchar *plaintext;
    gchar *key;
    struct template *pw;

    pw = (struct template *) data;
    
    printf("data address: %p\n", (struct template *)data);
    printf("pw address: %p\n", pw);
    printf("*pw address: %p\n", *pw);
    printf("pw -> plaintext address: %p\n", pw -> plaintext);
    //plaintext = gtk_entry_get_text(GTK_ENTRY(pw -> plaintext));
    //printf("   plaintext contents: %s\n", plaintext);

    //key = gtk_entry_buffer_get_text(gtk_entry_get_buffer(GTK_ENTRY(&(pw -> key))));
    //gtk_entry_set_buffer(GTK_ENTRY(pw -> key),gtk_entry_get_buffer(GTK_ENTRY(pw -> plaintext)));
    //printf("   key contents: %s\n", key);

    //triplets = make_triplets(strip_string(plaintext));
    //for (int i = 0; i<4; i++) {
    //    for (int j = 0; j<10; j++) {
    //        printf("[%02i]:%06x ", k, triplets[k]);
    //        k++;
    //    }
    //    printf("\n");
    //}

    //GdkColor color;
    //gdk_color_parse("#07d9d9", &color);
    //gtk_widget_modify_fg(GTK_WIDGET(pw -> cells), GTK_STATE_NORMAL, &color);
}


static void activate(GtkApplication* app, gpointer user_data) {
    int i, x, y;
    unsigned int seed;
    static struct template gui;

    gui.window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(gui.window), "Colta");

    gui.grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(gui.grid), 3);
    gtk_grid_set_column_spacing(GTK_GRID(gui.grid), 3);
    gtk_container_add(GTK_CONTAINER(gui.window), gui.grid);

    gui.plaintext = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(gui.grid), gui.plaintext, 0, 0, 2, 1);
    printf("gui.plaintext address: %p\n", gui.plaintext);

    gui.key = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(gui.grid), gui.key, 0, 1, 1, 1);

    gui.button_box = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_grid_attach(GTK_GRID(gui.grid), gui.button_box, 1, 1, 1, 1);

    gui.button = gtk_button_new_with_label("Encode");
    gtk_container_add(GTK_CONTAINER(gui.button_box), gui.button);

    gui.output_grid = gtk_grid_new();
    gtk_grid_set_column_spacing(GTK_GRID(gui.grid), 2);
    gtk_grid_set_column_homogeneous(GTK_GRID(gui.output_grid), 1);
    gtk_grid_set_row_homogeneous(GTK_GRID(gui.output_grid), 1);
    for (i=0; i<CELLS; i++) {
        gui.cells[i] = gtk_label_new("██");
    }

    i=0;
    for (y=0; y<4; y++) {
        for (x=0; x<10; x++) {
            gtk_grid_attach(GTK_GRID(gui.output_grid), gui.cells[i], x, y, 1, 1);
            i++;
        }
    }

    gtk_grid_attach(GTK_GRID(gui.grid), gui.output_grid, 0, 2, 2, 1);

    g_signal_connect(
            G_OBJECT(gui.button),
            "clicked",
            G_CALLBACK(encode),
            &gui
            //make_data(gui.plaintext, gui.key, gui.cells)
    );

    printf("gui address: %p\n", gui);
    printf("&gui address: %p\n", &gui);
    gtk_widget_show_all(gui.window);
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
