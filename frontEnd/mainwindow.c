#include <gtk/gtk.h>
#include "../backEnd/server.h"

void button_server_start(GtkWidget *button, gpointer information) {

   int status = server_start("127.0.0.1", 17112);
   if (status == -1) {
       g_print("server connection failed\n");
       gtk_label_set_text(GTK_LABEL(information), "Server status : Failed");
   }
   else {
       g_print("server connected %d\n", status);
       gtk_label_set_text(GTK_LABEL(information), "Server status : Connected");
   }
}

void button_server_stop(GtkWidget *button, gpointer information) {

   int status = server_stop(status);
   if (status != 0) {
       g_print("server still running\n");
       gtk_label_set_text(GTK_LABEL(information), "Server status : Server Still Running");
   }
   else {
       g_print("server stopped\n");
       gtk_label_set_text(GTK_LABEL(information), "Server status : Stopped");
   }
}

void button_server_reset(GtkWidget *button, gpointer information) {
   gtk_label_set_text(GTK_LABEL(information), "Server Status"); 
}

/* void button_server_reset(GtkWidget *button) { */

/*    int status = server_reset(); */
/*    if (status == 0) */
/*        g_print("server reset failed"); */
/*    else */
/*        g_print("server reset"); */
/* } */

int window(GtkApplication *app) {

    GtkWidget *window;
    GtkWidget *grid;
    GtkWidget *information;
    GtkWidget *button_start;
    GtkWidget *button_stop;
    GtkWidget *button_reset;

    GtkEntryBuffer *text;
    GtkWidget *entry;

    entry = gtk_entry_new();
    text = gtk_entry_buffer_new("", 0);
    char *msg = (char *)gtk_entry_buffer_get_text(text);
    gtk_widget_set_visible(GTK_WIDGET(entry), 1);

    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Server" );
    gtk_window_set_default_size(GTK_WINDOW(window),800 ,600);

    information = gtk_label_new("Connection Status");

    button_start = gtk_button_new_with_label("Start Server");
    button_stop  = gtk_button_new_with_label("Stop Server");
    button_reset = gtk_button_new_with_label("Reset server");

    printf("%s\n", msg);
    g_signal_connect(button_start, "clicked", G_CALLBACK(button_server_start), information);
    g_signal_connect(button_stop, "clicked", G_CALLBACK(button_server_stop), information);
    g_signal_connect(button_reset, "clicked", G_CALLBACK(button_server_reset), information);

    grid = gtk_grid_new();
    gtk_grid_set_column_spacing(GTK_GRID(grid), 20);
    gtk_grid_set_row_spacing(GTK_GRID(grid), 20);
    gtk_widget_set_halign(grid, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(grid, GTK_ALIGN_CENTER);

    gtk_grid_attach(GTK_GRID(grid), information, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), button_start, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), button_stop, 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), button_reset, 0, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry, 0, 4, 1, 1);


    gtk_window_set_child(GTK_WINDOW(window), grid);

    gtk_window_present(GTK_WINDOW(window));

    return 0;
}

int main() {

    GtkApplication *app;
    int status;

    app = gtk_application_new("mychat.server.application",
                              G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(window), NULL);
    status = (g_application_run(G_APPLICATION(app), 0, 0));
    g_object_unref(app);
    return 0;
}
