// Includes
#include <gtk/gtk.h>
#include "stdbool.h"
#include "../utils/helpers.h"
#include "../pages/DosagePage.h"
#include "../pages/ResidentListPage.h"
#include "../pages/OrderPage.h"
#include "../pages/StoragePage.h"
#include "../configs/constants.h"
#include <fltwinerror.h>
#include <windows.h>


void On_Order_Button_Clicked(GtkWidget *widget, gpointer user_data) {
    OrderListButtonData *orderListButtonData = (OrderListButtonData *)user_data;
    printf("Resident Name: %s", orderListButtonData->residentName);
    char *storageModifyText;
    if(orderListButtonData->currentStock < orderListButtonData->amountUsed) {
        storageModifyText = g_strdup_printf("%i", orderListButtonData->currentStock+orderListButtonData->orderedAmount);
    } else {
        storageModifyText = g_strdup_printf("%i", orderListButtonData->currentStock+orderListButtonData->orderedAmount-orderListButtonData->amountUsed);
    }

    UpdateStorageValue("residents.json", orderListButtonData->dsEntry, "", 0, storageModifyText,
                       orderListButtonData->medicineName, 2, true);
    UpdateStorageBasedOnAmount();
    UpdateQuantityLabelInTable();
    UpdateAllButtonDSEntriesToNewDSEntry();

    GtkWidget *parent = gtk_widget_get_parent(widget);

    // Get the grandparent, which is the box you want to delete
    GtkWidget *grandparent = gtk_widget_get_parent(parent);

    gtk_widget_destroy(grandparent);
}

GtkWidget *NewListBoxHeaders(GtkWidget *container, bool IsFixed, int width, int height, int xPos, int yPos, char *cssText) {
    GtkWidget *listBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_size_request(listBox, width, height);
    if(strcmp(cssText, "") == 0) {
        ApplyCSSWithID(listBox, "listBox", "{ background-color: #dce0e8;}");
    } else {
        ApplyCSSWithID(listBox, "listBox", cssText);
    }
    if(IsFixed) {
        gtk_fixed_put(GTK_FIXED(container), listBox, xPos, yPos);
    } else {
        gtk_box_pack_start(GTK_BOX(container), listBox, FALSE, FALSE, 0);
    }
    return listBox;
}
GtkWidget *NewListBox(GtkContainer *container, int width, int height, char *cssText) {
    GtkWidget *listBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_size_request(listBox, width, height);
    if(strcmp(cssText, "") == 0) {
        ApplyCSSWithID(listBox, "listBox", "{ background-color: #dce0e8;}");
    } else {
        ApplyCSSWithID(listBox, "listBox", cssText);
    }
    gtk_container_add(container, listBox);
    return listBox;
}
GtkWidget *NewListRow(GtkBox *listBox, int height) {
    GtkWidget *listRow = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_widget_set_size_request(listRow, 1, height);
    gtk_box_pack_start(listBox, listRow, FALSE, FALSE, 0);
    gtk_box_set_homogeneous(GTK_BOX(listRow), TRUE);
    return listRow;
}
GtkWidget *NewListTextColumn(GtkBox *listRow, int height, char *string, bool isHeader, char *cssText) {
    GtkWidget *listColumn = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_widget_set_size_request(listColumn, 1, height);
    gtk_box_pack_start(GTK_BOX(listRow), listColumn, TRUE, TRUE, 0);
    GtkWidget *label = gtk_label_new(string);
    if(strcmp(cssText, "") == 0) {
        if(isHeader) {
            ApplyCSSWithID(listColumn, "listColumnHeader",
                           "{ background-color: #f0f3fa; font-size: 18px; margin: 1px;}");
            gtk_label_set_xalign(GTK_LABEL(label), 0.5);
        } else {
            ApplyCSSWithID(listColumn, "listColumn", "{ background-color: white; margin: 1px;}");
            gtk_label_set_xalign(GTK_LABEL(label), 0.1);
        }
    } else {
        ApplyCSSWithID(listColumn, "listColumnHeader", cssText);
        gtk_label_set_xalign(GTK_LABEL(label), 0.5);
    }
    gtk_box_pack_start(GTK_BOX(listColumn), label, TRUE, TRUE, 0);

    return listColumn;
}
GtkWidget *NewListButtonColumn(GtkBox *listRow, int width, int height, char *string, char *cssText, ResidentListButtonClickedData *residentListButtonClickedData, OrderListButtonData *orderListButtonData) {
    // Create new container
    GtkWidget *listColumn = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    // Set size of container
    gtk_widget_set_size_request(listColumn, 1, height);
    gtk_box_pack_start(GTK_BOX(listRow), listColumn, TRUE, TRUE, 0);

    // Create new button
    GtkWidget *button = gtk_button_new_with_label(string);
    // Set size of button
    gtk_widget_set_size_request(button, width/5-2, height);
    // Apply CSS
    if(strcmp(cssText, "") == 0) {
        ApplyCSSWithID(listColumn, "listColumn", "{ background-color: white; padding: 0px; margin: 1px;}");
        ApplyCSSOnWidget(button, "button",
                         "{"
                         "margin: 10px;"
                         "border-radius: 24px;"
                         "background: #043D73;"
                         "background-color: #043D73;"
                         "color: #FFFFFF;"
                         "outline: none;"
                         "box-shadow: none;"
                         "}");
    } else {
        ApplyCSSOnWidget(button, "button", cssText);
    }

    if(residentListButtonClickedData != NULL) {
        g_signal_connect(button, "clicked", G_CALLBACK(CreateDosagePage), residentListButtonClickedData);
    }
    if(orderListButtonData != NULL) {
        g_signal_connect(button, "clicked", G_CALLBACK(On_Order_Button_Clicked), orderListButtonData);
    }
    gtk_box_pack_start(GTK_BOX(listColumn), button, TRUE, TRUE, 0);

    return listColumn;
}