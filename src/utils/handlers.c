#include "handlers.h"

GtkWidget *WARNING_WINDOW;

void MakeWarningWindow(char *text) {
    GtkWidget *dialog;
    dialog = gtk_message_dialog_new(GTK_WINDOW(WARNING_WINDOW), GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, text);
    GtkWidget *messageArea = gtk_message_dialog_get_message_area(GTK_MESSAGE_DIALOG(dialog));
    GtkWidget *label = g_list_nth_data(gtk_container_get_children(GTK_CONTAINER(messageArea)), 0);
    gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_CENTER);
    gtk_label_set_xalign(GTK_LABEL(label), 0.5);
    gtk_window_set_title(GTK_WINDOW(dialog), "WARNING!");
    // Make the dialog modal
    gtk_window_set_modal(GTK_WINDOW(dialog), TRUE);
    // Run the dialog and wait for the user response
    gtk_dialog_run(GTK_DIALOG(dialog));
    // After the user responds, destroy the dialog
    gtk_widget_destroy(dialog);
}
