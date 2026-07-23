#include <gtk/gtk.h>

#include "../backEnd/client.h"

#include <stdio.h>

struct data {

    int        server_fd;
    GtkWidget *label, *ip_entry, *port_entry, *input_entry;
};

int connect_to_server(GtkButton *button, gpointer parameters) {
    
    struct data *temp = (struct data *) parameters;

    const char *ip_address  = gtk_editable_get_text(GTK_EDITABLE(temp -> ip_entry));
    const char *char_port = gtk_editable_get_text(GTK_EDITABLE(temp -> port_entry));
    int port_number = atoi(char_port);

    int server_id = server_connect((char *)ip_address, port_number);
    if (server_id == -1) {
        printf("server connection failed\n"
        "try again");
        return -1;
    }
    temp -> server_fd = server_id;
    gtk_label_set_text(GTK_LABEL(temp -> label), "Connected to Server");
    return 0;
}

int send_msg_to_server(GtkButton *button, gpointer parameters) {

    struct data *temp = (struct data *)parameters;
    int server_id = temp -> server_fd;

    if (server_id == -1 || server_id == 0) {
        printf("Check connection to server and try again\n");
        return -1;
    }

    int len = gtk_entry_get_text_length(GTK_ENTRY(temp -> input_entry));
    const char *msg = gtk_editable_get_text(GTK_EDITABLE(temp -> input_entry));

    send_msg(server_id, (char *)msg, len);
    
    gtk_label_set_text(GTK_LABEL(temp -> label), "Messege Sending...");
    gtk_label_set_text(GTK_LABEL(temp -> label), "Message Sent");
    return 0;
}

int recv_msg_from_server(GtkButton *button, gpointer parameters) {

    struct data *temp = (struct data *)parameters;
    int server_id = temp -> server_fd;

    if (server_id == -1 || server_id == 0) {
        printf("Check connection to server and try again\n");
        return -1;
    }
    char *msg = recv_msg(server_id);
    gtk_label_set_text(GTK_LABEL(temp -> label), "Messege Received");
    return 0;
}

int client_window(GtkApplication *app) {

    //Window creation
    GtkWidget *main_window;
    GtkWidget *control_window;
    GtkWidget *interactable_window;
    
    //Grids and button
    GtkWidget *grid;
    GtkWidget *window_grid;
    GtkWidget *interact_grid;
    GtkWidget *connect_server;
    GtkWidget *send_msg;
    GtkWidget *recv_msg;

    //Entries for control window
    GtkWidget *ip_address_entry;
    GtkWidget *port_number_entry;

    //labels for control window
    GtkWidget *client;
    GtkWidget *ip_address_label;
    GtkWidget *port_number_label;

    //input string for sending messeges
    GtkWidget *input_entry;

    ip_address_entry  = gtk_entry_new();
    port_number_entry = gtk_entry_new();

    gtk_entry_set_placeholder_text(GTK_ENTRY(ip_address_entry), "Ex : 167.12.32.11");
    gtk_entry_set_placeholder_text(GTK_ENTRY(port_number_entry), "Ex : 13000");

    main_window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(main_window), "client");
    gtk_window_set_default_size(GTK_WINDOW(main_window), 800, 800);

    /* control_window = gtk_frame_new(""); */
    /* gtk_widget_set_size_request(control_window, 300, 800); */
    /* gtk_frame_set_child(GTK_FRAME(control_window), grid); */

    /* interactable_window = gtk_frame_new(""); */
    /* gtk_widget_set_size_request(interactable_window, 500, 800); */
    /* gtk_frame_set_child(GTK_FRAME(interactable_window), interact_grid); */

    /* window_grid = gtk_grid_new(); */
    /* gtk_grid_set_column_spacing(GTK_GRID(window_grid), 10); */
    /* gtk_grid_set_row_spacing(GTK_GRID(window_grid), 10); */
    /* gtk_widget_set_size_request(grid, 300, 500); */
    /* gtk_grid_attach(GTK_GRID(grid), grid, 0, 0, 1, 1); */
    /* gtk_widget_set_halign(window_grid, GTK_ALIGN_CENTER); */
    /* gtk_widget_set_valign(window_grid, GTK_ALIGN_CENTER); */

    /* gtk_grid_attach(GTK_GRID(window_grid), control_window, 0, 0, 1, 1); */
    /* gtk_grid_attach(GTK_GRID(window_grid), interactable_window, 1, 0, 1, 1); */

    /* interact_grid = gtk_grid_new(); */
    /* gtk_grid_set_column_spacing(GTK_GRID(interact_grid), 10); */
    /* gtk_grid_set_row_spacing(GTK_GRID(interact_grid), 10); */
    /* gtk_widget_set_halign(interact_grid, GTK_ALIGN_CENTER); */
    /* gtk_widget_set_valign(interact_grid, GTK_ALIGN_CENTER); */
    
    client            = gtk_label_new("Client Status");
    ip_address_label  = gtk_label_new("IP address   : ");
    port_number_label = gtk_label_new("Port Number  : ");

    connect_server = gtk_button_new_with_label("Connect to Server");
    send_msg       = gtk_button_new_with_label("Send Message");
    recv_msg       = gtk_button_new_with_label("Receive Message");

    struct data *parameters = malloc(sizeof(struct data));
    parameters -> label      = client;
    parameters -> ip_entry   = ip_address_entry;
    parameters -> port_entry = port_number_entry;

    g_signal_connect(connect_server, "clicked", G_CALLBACK(connect_to_server), parameters);
    g_signal_connect(send_msg,       "clicked", G_CALLBACK(send_msg_to_server), parameters);
    g_signal_connect(recv_msg,       "clicked", G_CALLBACK(recv_msg_from_server), parameters);

    GtkWidget *pan;
    pan = gtk_paned_new(1);
    gtk_paned_set_position(GTK_PANED(pan), 300);

    grid = gtk_grid_new();
    /* gtk_widget_set_size_request(grid, 300, 500); */
    gtk_grid_set_column_spacing(GTK_GRID(grid), 10);
    gtk_grid_set_row_spacing(GTK_GRID(grid), 10);
    /* gtk_widget_set_halign(grid, GTK_ALIGN_CENTER); */
    /* gtk_widget_set_valign(grid, GTK_ALIGN_CENTER); */
    GtkWidget *hello = gtk_grid_new();

    gtk_grid_attach(GTK_GRID(grid), client, 2, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), ip_address_label, 1, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), port_number_label, 1, 2, 1, 1);

    gtk_grid_attach(GTK_GRID(grid), ip_address_entry, 2, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), port_number_entry, 2, 2, 1, 1);
    
    gtk_grid_attach(GTK_GRID(grid), connect_server, 2, 3, 1, 1);
    /* gtk_grid_attach(GTK_GRID(grid), send_msg, 1, 4, 1, 1); */
    /* gtk_grid_attach(GTK_GRID(grid), recv_msg, 1, 5, 1, 1); */

    window_grid = gtk_grid_new();
    gtk_grid_set_column_spacing(GTK_GRID(window_grid), 10);
    gtk_grid_set_row_spacing(GTK_GRID(window_grid), 10);
    /* gtk_grid_attach(GTK_GRID(window_grid), control_window, 0, 0, 1, 1); */
    gtk_grid_attach(GTK_GRID(window_grid), hello, 1, 1, 1, 10);
    gtk_grid_attach(GTK_GRID(window_grid), grid, 10, 10, 1, 1);

    /* gtk_frame_set_child(GTK_FRAME(control_window), grid); */
    /* gtk_window_set_child(GTK_WINDOW(main_window), window_grid); */
    gtk_window_set_child(GTK_WINDOW(main_window), pan);
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
