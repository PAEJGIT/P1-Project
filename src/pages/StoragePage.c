#include <gtk/gtk.h>
#include "StoragePage.h"
#include <stdbool.h>
#include "../../src/utils/helpers.h"
#include "../components/listElements.h"
#include "ResidentListPage.h"
#include "../configs/constants.h"

// Global variables
char *COLUMN_HEADERS[5] = {"Medication", "Dosage", "Expiry", "Stock", "Orders"};
GtkWidget** STORAGE_LIST_ROW_ARRAY = NULL;
int STORAGE_LIST_ROW_ARRAY_SIZE = 0;



GtkWidget *InitializeStoragePage(GtkWidget *stack, gint width, gint height) {
    // Make a new fixed window and add it to the stack
    GtkWidget *storagePage = gtk_fixed_new();
    gtk_widget_set_size_request(storagePage, width, height);
    gtk_stack_add_titled(GTK_STACK(stack), storagePage, "Storage Page", "Storage Page");

    // Make fixed to hold the table
    GtkWidget *tableBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_size_request(tableBox, STORAGE_SCROLLABLE_WINDOW_WIDTH, STORAGE_SCROLLABLE_WINDOW_HEIGHT+60);
    gtk_fixed_put(GTK_FIXED(storagePage), tableBox, (width-STORAGE_SCROLLABLE_WINDOW_WIDTH)/2, 40);
    ApplyCSSWithID(tableBox, "tableBox",
                   "{ background-color: #FFFFFF; border-radius: 30px; box-shadow: rgb(204, 219, 232) 3px 3px 6px 0px inset, rgba(255, 255, 255, 0.5) -3px -3px 6px 1px inset; }");


    // Create a GtkScrolledWindow to hold the GtkTreeView
    GtkWidget *scrolledWindow = gtk_scrolled_window_new(NULL, NULL);
    gtk_widget_set_size_request(scrolledWindow, STORAGE_SCROLLABLE_WINDOW_WIDTH, STORAGE_SCROLLABLE_WINDOW_HEIGHT);
    AddScrollableWindowToArray(scrolledWindow);
    ApplyCSSWithID(scrolledWindow, "scrolledWindow", "{ background-color: white; }");


    char *dsMain = ReadJSON("medicine.json");
    int dsLength = GetJSONArrayLength(dsMain);

    // Assign size to list row array
    STORAGE_LIST_ROW_ARRAY_SIZE = dsLength;
    STORAGE_LIST_ROW_ARRAY = (GtkWidget**)malloc(STORAGE_LIST_ROW_ARRAY_SIZE * sizeof(GtkWidget*));

    // Add list box
    GtkWidget *listBoxHeaders = NewListBoxHeaders(tableBox, false, STORAGE_SCROLLABLE_WINDOW_WIDTH, 60, (width-STORAGE_SCROLLABLE_WINDOW_WIDTH)/2, 40, "{}");
    GtkWidget *listRowHeader = NewListRow(GTK_BOX(listBoxHeaders), 60);
    char *css = "{ background-color: white; border-top: 2px solid black; border-bottom: 2px solid black; font-size: 18px; }";
    GtkWidget *listColumn1Header = NewListTextColumn(GTK_BOX(listRowHeader), 60, "Name", TRUE, css);
    GtkWidget *listColumn2Header = NewListTextColumn(GTK_BOX(listRowHeader), 60, "Dosage", TRUE, css);
    GtkWidget *listColumn3Header = NewListTextColumn(GTK_BOX(listRowHeader), 60, "Quantity", TRUE, css);
    GtkWidget *listColumn4Header = NewListTextColumn(GTK_BOX(listRowHeader), 60, "Expiry Date", TRUE, css);
    GtkWidget *listColumn5Header = NewListTextColumn(GTK_BOX(listRowHeader), 60, "Amount Ordered", TRUE, css);

    gtk_box_pack_start(GTK_BOX(tableBox), scrolledWindow, FALSE, FALSE, 0);

    GtkWidget *listBox = NewListBox(GTK_CONTAINER(scrolledWindow), STORAGE_SCROLLABLE_WINDOW_WIDTH, STORAGE_SCROLLABLE_WINDOW_HEIGHT, "{ padding: 0; }");
    for(int i = 0; i < dsLength; i++) {
        char *dsEntry = GetJSONArrayItem(dsMain, i, FALSE);
        GtkWidget *listRow = NewListRow(GTK_BOX(listBox), 50);
        STORAGE_LIST_ROW_ARRAY[i] = listRow;
        GtkWidget *listColumn1 = NewListTextColumn(GTK_BOX(listRow), 50, GetJSONItem(dsEntry, "name"), FALSE, "{ background-color: white; border-bottom: 1px solid #dce0e8}");
        GtkWidget *listColumn2 = NewListTextColumn(GTK_BOX(listRow), 50, GetJSONItem(dsEntry, "dosage"), FALSE, "{ background-color: white; border-bottom: 1px solid #dce0e8}");
        GtkWidget *listColumn3 = NewListTextColumn(GTK_BOX(listRow), 50, GetJSONItem(dsEntry, "quantity"), FALSE, "{ background-color: white; border-bottom: 1px solid #dce0e8}");
        GtkWidget *listColumn4 = NewListTextColumn(GTK_BOX(listRow), 50, GetJSONItem(dsEntry, "expiry_date"), FALSE, "{ background-color: white; border-bottom: 1px solid #dce0e8}");
        GtkWidget *listColumn5 = NewListTextColumn(GTK_BOX(listRow), 50, "0", FALSE, "{ background-color: white; border-bottom: 1px solid #dce0e8}");
    }

    return storagePage;
}

// =====================================================================================================================
// Update Function
// =====================================================================================================================
void UpdateQuantityLabelInTable() {
    for(int i = 0; i < STORAGE_LIST_ROW_ARRAY_SIZE; i++) {
        GList *children = gtk_container_get_children(GTK_CONTAINER(STORAGE_LIST_ROW_ARRAY[i]));
        GtkWidget *quantityBox = GTK_WIDGET(g_list_nth_data(children, 2));

        GList *quantityBoxChildren = gtk_container_get_children(GTK_CONTAINER(quantityBox));
        GtkWidget *quantityLabelWidget = GTK_WIDGET(g_list_nth_data(quantityBoxChildren, 0));

        gtk_label_set_text(GTK_LABEL(quantityLabelWidget), GetJSONItem(
                GetJSONArrayItem(ReadJSON("medicine.json"), i, false), "quantity"));

    }
}
// =====================================================================================================================