#include <stdio.h>
#include <gtk/gtk.h>

#define LETTERS 31
#define CELLS 40
#define CAPACITY 122
#define TWO_TENS 256
#define FOUR_TENS 65536

const char ALPHABET[LETTERS] =
    {'a','b','c','d','e','f','g','h','i','j','k', 'l','m', 'n','o','p','q','r',
     's','t','u','v','w','x','y','z','.',',','?','!',' '};
const int DEFAULT_CODE[LETTERS] =
    {7,14,21,28,35,42,49,56,63,70,77,84,91,98,105,112,119,
     126,133,140,147,154,161,168,175,182,189,196,203,210,217};

struct template {
    GtkWidget *window;
    GtkWidget *grid;
    GtkWidget *box;
    GtkWidget *plaintext;
    GtkWidget *button_box;
    GtkWidget *button;
    GtkWidget *output_grid;
    GtkWidget *cells[CELLS];
};


int *make_triplets(char str[]) {
    int start, i, j, k=0;
    int temp[CAPACITY];
    static int triplets[CELLS];

    for (i = 0; str[i] != '\0'; i++) {
        for (j = 0; str[i] != ALPHABET[j]; j++);
        printf("        DEFAULT_CODE[%i] = 0x%x\n", j, DEFAULT_CODE[j]);
        temp[i] = DEFAULT_CODE[j];
    }

    if (i % 3 == 2)
        temp[i++] = 0;

    if (i % 3 == 1) {
        temp[i++] = 0;
        temp[i] = 0;
    }

    for (start = 0; start < i; start+=3) {
        triplets[k] = temp[start]*FOUR_TENS + temp[start+1]*TWO_TENS + temp[start+2];
        k++;
    }
    
    return triplets;
}


char *strip_string(char str[]) {
    int i, j, k = 0;
    static char cleantext[CAPACITY];
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
    char *plaintext;
    char temp_str[8];
    GdkColor color;
    struct template *pw;

    pw = (struct template *) data;
    
    plaintext = gtk_entry_get_text(GTK_ENTRY(pw -> plaintext));
    printf("    plaintext contents: %s\n", plaintext);

    triplets = make_triplets(strip_string(plaintext));
    for (int i = 0; i<4; i++) {
        for (int j = 0; j<10; j++) {
            printf("[%02i]:%06x ", k, triplets[k]);
            k++;
        }
        printf("\n");
    }

    for (int i = 0; i < CELLS; i++) {
        snprintf(temp_str, 8, "#%06x", triplets[i]);
        gdk_color_parse(temp_str, &color);
        gtk_widget_modify_fg(GTK_WIDGET(pw -> cells[i]), GTK_STATE_NORMAL, &color);
    }

    for (int i = 0; i < CELLS; i++)
        triplets[i] = 0;
}


static void activate(GtkApplication* app, gpointer user_data) {
    int i, x, y;
    static struct template gui;

    gui.window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(gui.window), "Colta");

    gui.box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 3);
    gtk_container_add(GTK_CONTAINER(gui.window), gui.box);

    gui.plaintext = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(gui.plaintext), "Text (a-z .,!?, 120 characters maximum)");
    gtk_entry_set_max_length(GTK_ENTRY(gui.plaintext), 120);
    gtk_box_pack_start(GTK_BOX(gui.box), gui.plaintext, 1, 1, 0);

    gui.button_box = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_box_pack_start(GTK_BOX(gui.box), gui.plaintext, 1, 1, 0);

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

    gtk_box_pack_start(GTK_BOX(gui.box), gui.output_grid, 1, 1, 0);
    gtk_box_pack_end(GTK_BOX(gui.box), gui.button_box, 1, 1, 0);

    g_signal_connect(
            G_OBJECT(gui.button),
            "clicked",
            G_CALLBACK(encode),
            &gui
    );

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
