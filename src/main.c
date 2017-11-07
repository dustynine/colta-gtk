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

struct passed_widgets {
    GtkWidget *plaintext;
    GtkWidget *key;
};


gpointer make_data(GtkWidget *text, GtkWidget *key) {
    static struct passed_widgets pw;
    pw.plaintext = text;
    pw.key = key;
    return (gpointer) &pw;
}


int *make_triplets(gchar str[]) {
    int start, i, j, k=0;
    int temp[CAPACITY];
    static int triplets[CELLS];

    printf("        str = %s\n", str);

    for (i = 0; str[i] != '\0'; i++) {
        for (j = 0; str[i] != ALPHABET[j]; j++);
            //printf("        str[%i] = %c; ALPHABET[%i] = %c\n", i, str[i], j, ALPHABET[j]);
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

    // сделать счетчик количества символов, делать что-то если кол-во не равно трем

    for (start = 0; start < i; start+=3) {
        triplets[k] = temp[start]*FOUR_TENS + temp[start+1]*TWO_TENS + temp[start+2];
        //printf("    1/ triplets[%i] = %x\n", k, triplets[k]);
        k++;
    }
    // умножать на 256 и складывать по разрядам
    return triplets;
}


gchar *strip_string(gchar str[]) {
    int i, j, k = 0;
    static gchar cleantext[CAPACITY];
    //printf("      iterating through the raw string\n");
    for (i = 0; str[i] != '\0'; i++) {
        //printf("         str[%i] = %c\n", i, str[i]);
        for (j = 0; str[i] != ALPHABET[j] && j < LETTERS; j++);
        //printf("         j=%i;\n", j);
        if (j == LETTERS) {
            //printf("      dropping unknown character\n");
            continue;
        }
        else {
            //printf("      match found: %c\n", ALPHABET[j]);
            cleantext[k] = ALPHABET[j];
            k++;
        }
    }
    cleantext[k] = '\0';
    //printf("     cleantext is:%s\n", cleantext);
    return cleantext;
}


static void encode(GtkButton *button, gpointer *data) {
    printf("encoding initiated:\n");

    int k = 0;
    int *triplets;
    gchar *plaintext;
    gchar *key;
    GtkTextIter start, end;
    GtkTextBuffer *txt_buf;
    GtkEntryBuffer *ent_buf;
    struct passed_widgets *pw;
    
    pw = (struct passed_widgets *) data;
    
    txt_buf = gtk_text_view_get_buffer(GTK_TEXT_VIEW(pw -> plaintext));
    gtk_text_buffer_get_bounds(txt_buf, &start, &end);
    plaintext = gtk_text_buffer_get_text(txt_buf, &start, &end, FALSE);
    printf("   plaintext contents: %s\n", plaintext);
    
    ent_buf = gtk_entry_get_buffer(GTK_ENTRY(pw -> key));
    key = gtk_entry_buffer_get_text(ent_buf);
    printf("   key contents: %s\n", key);

    triplets = make_triplets(strip_string(plaintext));
    for (int i = 0; i<4; i++) {
        for (int j = 0; j<10; j++) {
            printf("[%02i]:%06x ", k, triplets[k]);
            k++;
        }
        printf("\n");
    }
}


static void activate(GtkApplication* app, gpointer user_data) {
    int i, x, y;
    unsigned int seed;

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
    gtk_text_view_set_accepts_tab(GTK_TEXT_VIEW(plaintext), 0);
    gtk_grid_attach(GTK_GRID(grid), plaintext, 0, 0, 2, 1);

    key = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), key, 0, 1, 1, 1);

    button_box = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_grid_attach(GTK_GRID(grid), button_box, 1, 1, 1, 1);

    button = gtk_button_new_with_label("Encode");
    gtk_container_add(GTK_CONTAINER(button_box), button);

    g_signal_connect(
            G_OBJECT(button),
            "clicked",
            G_CALLBACK(encode),
            make_data(plaintext, key)
    );

    //gtk_override_background_color(GTK_WIDGET(cells[0]), GTK_STATE_FLAG_NORMAL,

    output_grid = gtk_grid_new();
    gtk_grid_set_column_spacing(GTK_GRID(grid), 2);
    gtk_grid_set_column_homogeneous(GTK_GRID(output_grid), 1);
    gtk_grid_set_row_homogeneous(GTK_GRID(output_grid), 1);
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
