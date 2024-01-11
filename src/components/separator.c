#include <gtk/gtk.h>

/**
 * \n SeparatorComponent COMPONENT\n
 * @param width Width of separator
 * @param height Height of separator
 * @param className CSS Class of separator
 * @return GtkWidget
 */
GtkWidget *SeparatorComponent(int width, int height, char *className) {
    GtkWidget *separator = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_widget_set_name(separator, className);
    gtk_widget_set_size_request(separator, width, height);
    return separator;
}