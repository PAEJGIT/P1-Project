#include <gtk/gtk.h>

/**
 * \n FLAG COMPONENT\n
 * Component to show different colored (grey, green, yellow and red) flags in medication table.
 * @param type —Choose between `success`, `warning`, `error` or `default`.\n\n Each will return a image component depending on state.
 * @return GtkWidget Image
 */
GtkWidget *FlagComponent(char *type) {
    GtkWidget *image;
    // Get image for flag
    if (strcmp(type, "success") == 0) {
        // Success Flag [GREEN]
        image = gtk_image_new_from_file("../src/assets/flags/flag_2.png");
    } else if (strcmp(type, "warning") == 0) {
        // Warning Flag [YELLOW]
        image = gtk_image_new_from_file("../src/assets/flags/flag_1.png");
    } else if (strcmp(type, "error") == 0) {
        // Error Flag [RED]
        image = gtk_image_new_from_file("../src/assets/flags/flag_0.png");
    } else {
        // Default Flag [GREY]
        image = gtk_image_new_from_file("../src/assets/flags/flag_3.png");
    }
    // Create container for image
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    // Set name for container
    gtk_widget_set_name(vbox, "profile_table_image_container");
    // Add image to container
    gtk_container_add(GTK_CONTAINER(vbox), image);
    return vbox;
}