#include "gio/gio.h"
#include <gtk/gtk.h>

#include "../backEnd/client.h"

struct data {
    int server_fd;
    GtkWidget *label;
};

int connect_to_server(GtkButton button, gpointer *parameters) {
    
    char ip[] = "127.0.0.1";
    int port = 17112;
    int server_id = server_connect(ip, port);
    if (server_id == -1) {
        printf("server connection failed\n"
        "try again");
        return -1;
    }
    struct data *temp = (struct data *)parameters;
    temp -> server_fd = server_id;
    gtk_label_set_text(GTK_LABEL(temp -> label), "Connected to Server");
    return 0;
}

int send_msg_to_server(GtkButton button, gpointer *parameters) {

    struct data *temp = (struct data *)parameters;
    int server_id = temp -> server_fd;
    if (server_id == -1 || server_id == 0) {
        printf("Check connection to server and try again\n");
        return -1;
    }
    send_msg(server_id);
    gtk_label_set_text(GTK_LABEL(temp -> label), "Messege Sending...");
    return 0;
}

int recv_msg_from_server(GtkButton button, gpointer *parameters) {

    struct data *temp = (struct data *)parameters;
    int server_id = temp -> server_fd;
    if (server_id == -1 || server_id == 0) {
        printf("Check connection to server and try again\n");
        return -1;
    }
    recv_msg(server_id);
    gtk_label_set_text(GTK_LABEL(temp -> label), "Messege Received");
    return 0;
}

int client_window(GtkApplication *app) {

    GtkWidget *window;
    GtkWidget *grid;
    GtkWidget *client;
    GtkWidget *connect_to_server;
    GtkWidget *send_msg;
    GtkWidget *recv_msg;

    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "client");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 800);

    client = gtk_label_new("Client Status");

    grid = gtk_grid_new();

    gtk_grid_set_column_spacing(GTK_GRID(grid), 10);
    gtk_grid_set_row_spacing(GTK_GRID(grid), 10);
    gtk_widget_set_halign(grid, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(grid, GTK_ALIGN_CENTER);

    connect_to_server = gtk_button_new_with_label("Connect to Server");
    send_msg = gtk_button_new_with_label("Send Message");
    recv_msg = gtk_button_new_with_label("Receive Message");


    struct data *parameters = malloc(sizeof(struct data));
    parameters -> label = client;
    
    g_signal_connect(connect_to_server, "clicked", G_CALLBACK(connect_to_server), parameters);
    g_signal_connect(send_msg, "clicked", G_CALLBACK(send_msg_to_server), parameters);
    g_signal_connect(recv_msg, "clicked", G_CALLBACK(recv_msg_from_server), parameters);

    gtk_grid_attach(GTK_GRID(grid), client, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), connect_to_server, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), send_msg, 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), recv_msg, 0, 3, 1, 1);

    gtk_window_set_child(GTK_WINDOW(window), grid);
    gtk_window_present(GTK_WINDOW(window));
    
    return 0;
}

int main() {

    GtkApplication *app;
    int status;

    app = gtk_application_new("mychat.client.window", G_APPLICATION_DEFAULT_FLAGS); 
    g_signal_connect(app, "activate", G_CALLBACK(client_window), NULL);
    status = g_application_run(G_APPLICATION(app), 0, 0);
    g_object_unref(app);
    return 0; 
}
