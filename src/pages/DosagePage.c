#include <gtk/gtk.h>
#include "ResidentProfilePage.h"
#include "ResidentListPage.h"

/**
 * Creates and initializes the dosage page for the selected resident.
 *
 * @param widget The GtkWidget that was clicked to invoke this function.
 * @param user_data A pointer to a ResidentListButtonClickedData structure, which contains information about the selected resident.
 */
GtkWidget** CreateDosagePage(GtkWidget *widget, gpointer user_data) {
    ResidentListButtonClickedData *residentData = (ResidentListButtonClickedData *)user_data;
    // Check if the dosage page exists
    GtkWidget *dosagePage = gtk_stack_get_child_by_name(GTK_STACK(residentData->stack), "Resident Medicine Dosage Page");
    if (dosagePage != NULL) {
        // Destroy the dosage page
        gtk_widget_destroy(dosagePage);
    }
    InitializeResidentMedicineDosagePage(residentData->stack, 1280, 720-100, residentData->dsEntry);
    return NULL;
}