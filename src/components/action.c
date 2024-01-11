#include "action.h"

/**
 * \n ACTION COMPONENT\n
 * Used for table in ACTION COLUMN
 * @param text —Text to render in label inside of component
 * @return GtkWidget
 */
GtkWidget *ActionComponent(char *text) {
    // Create container box
    GtkWidget *container = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    // Create label with text
    GtkWidget *TABLE_ACTION = gtk_label_new(text);
    gtk_widget_set_name(TABLE_ACTION, "profile_medication_table_action_text");
    gtk_widget_set_name(container, "profile_medication_table_action_container");
    gtk_container_add(GTK_CONTAINER(container), TABLE_ACTION);
    gtk_widget_set_halign(TABLE_ACTION, GTK_ALIGN_FILL);
    return container;
}