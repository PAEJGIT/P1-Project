// =====================================================================================================================
// Initialization
// =====================================================================================================================

// Includes
#include <gtk/gtk.h>
#include <stdbool.h>
#include "OrderPage.h"
#include "../utils/helpers.h"
#include "../configs/constants.h"
#include "../components/listElements.h"
#include "../../.packages/cJSON.h"

// Global
int ORDER_LIST_ROW_ARRAY_SIZE = 0;
GtkWidget *listBox;
GtkWidget** ORDER_LIST_ROW_ARRAY = NULL;
OrderListButtonData** ORDER_LIST_BUTTON_DATA_ARRAY = NULL;


/**
 * Initializes the order page
 *
 * @param stack The stack to add the order page to
 * @param width The width of the window
 * @param height The height of the window
 * @return The order page widget
 */
GtkWidget *InitializeOrderPage(GtkWidget *stack, gint width, gint height) {
    // Make a new fixed window and add it to the stack
    GtkWidget *orderPage = gtk_fixed_new();
    gtk_widget_set_size_request(orderPage, width, height);
    gtk_stack_add_titled(GTK_STACK(stack), orderPage, "Order Page", "Order Page");

    // Make fixed to hold the table
    GtkWidget *tableBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_size_request(tableBox, ORDER_SCROLLABLE_WINDOW_WIDTH, ORDER_SCROLLABLE_WINDOW_HEIGHT+60);
    gtk_fixed_put(GTK_FIXED(orderPage), tableBox, (width-ORDER_SCROLLABLE_WINDOW_WIDTH)/2, 40);
    ApplyCSSWithID(tableBox, "tableBox",
                   "{ background-color: #FFFFFF; border-radius: 30px; box-shadow: rgb(204, 219, 232) 3px 3px 6px 0px inset, rgba(255, 255, 255, 0.5) -3px -3px 6px 1px inset; }");


    // Create a GtkScrolledWindow to hold the GtkTreeView
    GtkWidget *scrolledWindow = gtk_scrolled_window_new(NULL, NULL);
    gtk_widget_set_size_request(scrolledWindow, ORDER_SCROLLABLE_WINDOW_WIDTH, ORDER_SCROLLABLE_WINDOW_HEIGHT);
    AddScrollableWindowToArray(scrolledWindow);
    ApplyCSSWithID(scrolledWindow, "scrolledWindow", "{ background-color: white; }");

    // Add list box
    GtkWidget *listBoxHeaders = NewListBoxHeaders(tableBox, false, ORDER_SCROLLABLE_WINDOW_WIDTH, 60, (width-ORDER_SCROLLABLE_WINDOW_WIDTH)/2, 40, "{}");
    GtkWidget *listRowHeader = NewListRow(GTK_BOX(listBoxHeaders), 60);
    char *css = "{ background-color: white; border-top: 2px solid black; border-bottom: 2px solid black; font-size: 18px; }";
    GtkWidget *listColumn1Header = NewListTextColumn(GTK_BOX(listRowHeader), 60, "Resident Name", TRUE, css);
    GtkWidget *listColumn2Header = NewListTextColumn(GTK_BOX(listRowHeader), 60, "Medicine Name", TRUE, css);
    GtkWidget *listColumn3Header = NewListTextColumn(GTK_BOX(listRowHeader), 60, "Current Stock", TRUE, css);
    GtkWidget *listColumn4Header = NewListTextColumn(GTK_BOX(listRowHeader), 60, "Amount To Order", TRUE, css);
    GtkWidget *listColumn5Header = NewListTextColumn(GTK_BOX(listRowHeader), 60, "Order", TRUE, css);

    gtk_box_pack_start(GTK_BOX(tableBox), scrolledWindow, FALSE, FALSE, 0);

    listBox = NewListBox(GTK_CONTAINER(scrolledWindow), ORDER_SCROLLABLE_WINDOW_WIDTH, ORDER_SCROLLABLE_WINDOW_HEIGHT, "{ padding: 0; }");

    return orderPage;
}
/**
 * Adds a new order to the orderlist
 *
 * @param dsEntry The database entry for the resident
 * @param residentName The name of the resident
 * @param medicineName The name of the medicine
 * @param currentStock The current stock of the medicine
 * @param amountToOrder The amount of medicine to order
 * @param amountUsed The amount of medicine to use
 */
void AddOrderToOrderList(char *dsEntry, char *residentName, char *medicineName, int currentStock, int amountToOrder, int amountUsed) {
    OrderListButtonData *orderListButtonData = malloc(1 * sizeof(OrderListButtonData));
    orderListButtonData->residentName = residentName;
    orderListButtonData->medicineName = medicineName;
    orderListButtonData->dsEntry = dsEntry;
    orderListButtonData->orderedAmount = amountToOrder;
    orderListButtonData->currentStock = currentStock;
    orderListButtonData->amountUsed = amountUsed;

    if(CheckIfMedicineAlreadyOrdered(orderListButtonData->residentName, orderListButtonData->medicineName)) {
        return;
    }

    GtkWidget *listRow = NewListRow(GTK_BOX(listBox), 50);
    ApplyCSSWithID(listRow, "listRow", "{}");
    if(ORDER_LIST_ROW_ARRAY_SIZE == 0) {
        ORDER_LIST_ROW_ARRAY_SIZE++;
        ORDER_LIST_ROW_ARRAY = (GtkWidget**)malloc(ORDER_LIST_ROW_ARRAY_SIZE * sizeof(GtkWidget*));
        ORDER_LIST_BUTTON_DATA_ARRAY = (OrderListButtonData**)malloc(ORDER_LIST_ROW_ARRAY_SIZE * sizeof(OrderListButtonData*));
    } else {
        ORDER_LIST_ROW_ARRAY_SIZE++;
        ORDER_LIST_ROW_ARRAY = (GtkWidget**)realloc(ORDER_LIST_ROW_ARRAY, ORDER_LIST_ROW_ARRAY_SIZE * sizeof(GtkWidget*));
        ORDER_LIST_BUTTON_DATA_ARRAY = (OrderListButtonData**)realloc(ORDER_LIST_BUTTON_DATA_ARRAY, ORDER_LIST_ROW_ARRAY_SIZE * sizeof(OrderListButtonData*));
    }

    ORDER_LIST_BUTTON_DATA_ARRAY[ORDER_LIST_ROW_ARRAY_SIZE-1] = orderListButtonData;
    ORDER_LIST_ROW_ARRAY[ORDER_LIST_ROW_ARRAY_SIZE-1] = listRow;
    GtkWidget *listColumn1 = NewListTextColumn(GTK_BOX(listRow), 50, residentName, FALSE, "{ background-color: white; border-bottom: 1px solid #dce0e8}");
    GtkWidget *listColumn2 = NewListTextColumn(GTK_BOX(listRow), 50, medicineName, FALSE, "{ background-color: white; border-bottom: 1px solid #dce0e8}");
    GtkWidget *listColumn3 = NewListTextColumn(GTK_BOX(listRow), 50, g_strdup_printf("%i", currentStock), FALSE, "{ background-color: white; border-bottom: 1px solid #dce0e8}");
    GtkWidget *listColumn4 = NewListTextColumn(GTK_BOX(listRow), 50, g_strdup_printf("%i", amountToOrder), FALSE, "{ background-color: white; border-bottom: 1px solid #dce0e8}");
    GtkWidget *listColumn5 = NewListButtonColumn(GTK_BOX(listRow), ORDER_SCROLLABLE_WINDOW_WIDTH, 50, "Order Now", "", NULL, orderListButtonData);

    gtk_widget_show_all(listRow);
    gtk_widget_set_visible(listRow, TRUE);
}
/**
 * Checks if a specific medicine has already been ordered for a given resident
 *
 * @param residentName The name of the resident
 * @param medicineName The name of the medicine
 * @return `true` if the medicine has already been ordered for the resident, `false` otherwise
 */
bool CheckIfMedicineAlreadyOrdered(char *residentName, char *medicineName) {
    for(int i = 0; i < ORDER_LIST_ROW_ARRAY_SIZE; i++) {
        if(strcmp(ORDER_LIST_BUTTON_DATA_ARRAY[i]->residentName, residentName) == 0) {
            if(strcmp(ORDER_LIST_BUTTON_DATA_ARRAY[i]->medicineName, medicineName) == 0) {
                return true;
            }
        }
    }
    return false;
}