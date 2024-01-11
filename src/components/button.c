#include <gtk/gtk.h>
#include <stdbool.h>

/**
 * \n BUTTON COMPONENT\n
 * @param text —text to show in label
 * @param image_path —path to image (optional)
 * @param class —CSS Class to use
 * @return GtkWidget
 */
GtkWidget *BUTTON(char* text, char* image_path, char* class){
    GtkButton *button = (GtkButton *) gtk_button_new();
    gtk_widget_set_size_request((GtkWidget *) button, 65, 2);
    if (image_path) {
        GtkImage *image = (GtkImage *) gtk_image_new_from_file(image_path);
        gtk_widget_set_name((GtkWidget *) image, "button_image");
        gtk_widget_set_name((GtkWidget *) button, class);
        gtk_button_set_image(button, (GtkWidget *) image);
        gtk_button_set_image_position(button, GTK_POS_LEFT); // Position the image above the text
        gtk_button_set_always_show_image (GTK_BUTTON (button), TRUE);
    }
    gtk_button_set_label(GTK_BUTTON(button), text);
    return (GtkWidget *) button;
}