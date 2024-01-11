#include "label.h"
#include "../utils/helpers.h"


/**
 * \n LABEL COMPONENT\n
 * Creates a label with various settings. Used in medication table.
 * @param label —text to render inside label.
 * @param size —size of container [width].
 * @param classname —CSS Class to apply to label.
 * @return GtkWidget
 */
GtkWidget *LabelComponent(char *label, int size, char *classname){
    // Create container box for label
    GtkWidget *container = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    // Set classname for container
    gtk_widget_set_name(container, g_strdup_printf("profile_medication_label_container_%s", classname));
    // Create label with text from @label
    GtkWidget *table_label = gtk_label_new(MakeFirstLetterUppercase(label));
    // Set label's size—Container will automatically adjust its size to accommodate the label's size
    gtk_widget_set_size_request(table_label, size, 2);
    // Set classname for label
    gtk_widget_set_name(table_label, g_strdup_printf("profile_medication_table_%s", classname));

    // Add label to container
    gtk_container_add(GTK_CONTAINER(container), table_label);
    // Align label to start-position
    gtk_label_set_xalign(GTK_LABEL(table_label), (gfloat)(0.0));
    // Return container
    return container;
}