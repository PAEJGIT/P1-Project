#include <gtk/gtk.h>


/**
 * Creates an icon image from the specified filename
 *
 * @param name The name of the icon file
 * @return The created icon image
 */
GtkImage *IconComponent(char *name){
    char *path = g_strdup_printf("../src/assets/icons/%s.png", name);
    GtkImage *icon = (GtkImage *) gtk_image_new_from_file(path);
    return icon;
}