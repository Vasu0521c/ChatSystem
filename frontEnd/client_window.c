
#include "gio/gio.h"
#include <gtk/gtk.h>

#include "../backEnd/client.h"

#include <stdio.h>

struct data {
    int server_fd;
    int length;
    char *message;
    GtkWidget *label;
    GtkWidget *grid_;
};

int connect_to_server(GtkButton *button, gpointer parameters) {
    
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

void send_msg_actual(GtkButton *button, gpointer pointer) {

    struct data *temp = (struct data *) pointer;
    send_msg(temp -> server_fd, temp -> message, temp -> length);
    return;
}

int send_msg_to_server(GtkButton *button, gpointer parameters) {

    struct data *temp = (struct data *)parameters;
    int server_id = temp -> server_fd;
    if (server_id == -1 || server_id == 0) {
        printf("Check connection to server and try again\n");
        return -1;
    }


    temp -> message = msg;
    printf("%s\n",msg);
    
    gtk_grid_attach(GTK_GRID(temp -> grid_), text_entry, 2, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(temp -> grid_), send, 3, 2, 1, 1);
    
    g_signal_connect(GTK_BUTTON(send), "clicked", G_CALLBACK(send_msg_actual), temp);
    gtk_label_set_text(GTK_LABEL(temp -> label), "Messege Sending...");
    return 0;
}

int recv_msg_from_server(GtkButton *button, gpointer parameters) {

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

    GtkWidget *main_window;
    GtkWidget *control_window;
    GtkWidget *interactable_window;
    GtkWidget *window_grid;
    
    GtkWidget *grid;
    GtkWidget *client;
    GtkWidget *connect_server;
    GtkWidget *send_msg;
    GtkWidget *recv_msg;

    //widgets for message funtion
    GtkWidget *text;
    GtkWidget *text_entry;
    GtkWidget *buffer;
    GtkWidget *send;

    main_window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(main_window), "client");
    gtk_window_set_default_size(GTK_WINDOW(main_window), 800, 800);

    window_grid = gtk_grid_new();
    gtk_grid_set_column_spacing(GTK_GRID(window_grid), 10);
    gtk_grid_set_row_spacing(GTK_GRID(window_grid), 10);
    gtk_widget_set_halign(window_grid, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(window_grid, GTK_ALIGN_CENTER);

    gtk_grid_attach(GTK_GRID(window_grid), control_window, 0, 0, 300, 800); 
    gtk_grid_attach(GTK_GRID(window_grid), interactable_window, 1, 0, 500, 800);

    client = gtk_label_new("Client Status");

    grid = gtk_grid_new();

    gtk_grid_set_column_spacing(GTK_GRID(grid), 10);
    gtk_grid_set_row_spacing(GTK_GRID(grid), 10);
    gtk_widget_set_halign(grid, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(grid, GTK_ALIGN_CENTER);

    connect_server = gtk_button_new_with_label("Connect to Server");
    send_msg = gtk_button_new_with_label("Send Message");
    recv_msg = gtk_button_new_with_label("Receive Message");

    struct data *parameters = malloc(sizeof(struct data));
    parameters -> label = client;
    parameters -> grid_ = grid;
    
    g_signal_connect(connect_server, "clicked", G_CALLBACK(connect_to_server), (void *)parameters);
    g_signal_connect(send_msg, "clicked", G_CALLBACK(send_msg_to_server), parameters);
    g_signal_connect(recv_msg, "clicked", G_CALLBACK(recv_msg_from_server), parameters);

    gtk_grid_attach(GTK_GRID(grid), client, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), connect_server, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), send_msg, 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), recv_msg, 0, 3, 1, 1);

    gtk_window_set_child(GTK_WINDOW(main_window), grid);
    gtk_window_present(GTK_WINDOW(main_window));
    
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
