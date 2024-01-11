// Includes
#include <gtk/gtk.h>
#include "ResidentListPage.h"
#include "string.h"
#include "ctype.h"
#include "../utils/helpers.h"
#include "../configs/constants.h"
#include "../components/listElements.h"

// Initialize functions
GtkWidget *AddListRow(GtkWidget *parentBox);

// Global variables
GtkWidget** LIST_ROW_ARRAY = NULL;
ResidentListButtonClickedData** residentlistButtonClickedDataArray = NULL;
int LIST_ROW_ARRAY_SIZE = 0;
int residentlistButtonClickedDataArraySize = 0;

/**
 * \n Handles the search bar input and filters the resident list
 *
 * @param entry The search bar widget
 * @param user_data User data associated with the signal
 */
void Handle_Search_Bar_Input(GtkEntry *entry, gpointer user_data) {
    const char *search_text = gtk_entry_get_text(entry);
    for(int i = 0; i < LIST_ROW_ARRAY_SIZE; i++) {
        gtk_widget_set_visible(LIST_ROW_ARRAY[i], true);
        GList *childList = gtk_container_get_children(GTK_CONTAINER(LIST_ROW_ARRAY[i]));
        GtkWidget *child = GTK_WIDGET(g_list_nth_data(childList, 0));
        GList *secondChildList = gtk_container_get_children(GTK_CONTAINER(child));
        GtkLabel *residentNameLabel = GTK_LABEL(g_list_nth_data(secondChildList, 0));
        const gchar *residentName = gtk_label_get_text(residentNameLabel);
        for(int l = 0; l < strlen(search_text); l++) {
            if(tolower(search_text[l]) != tolower(residentName[l])) {
                gtk_widget_set_visible(LIST_ROW_ARRAY[i], false);
            }
        }
    }

}
/**
 * \n Initializes the resident list page
 *
 * @param stack The stack to add the resident list page to
 * @param width The width of the window
 * @param height The height of the window
 * @return NULL
 */
GtkWidget *InitializeResidentList(GtkWidget *stack, gint width, gint height) {

    GtkWidget *betterResidentListPage = gtk_fixed_new();
    gtk_widget_set_size_request(betterResidentListPage, width, height);
    gtk_stack_add_titled(GTK_STACK(stack), betterResidentListPage, "Resident List Page", "Resident List Page");

    // Make a search bar
    GtkWidget *searchBar = gtk_search_entry_new();
    gtk_widget_set_size_request(searchBar, SEARCH_BAR_WIDTH, SEARCH_BAR_HEIGHT);
    gtk_fixed_put(GTK_FIXED(betterResidentListPage), searchBar, (width-SEARCH_BAR_WIDTH)/2, 20);
    g_signal_connect(searchBar, "changed", G_CALLBACK(Handle_Search_Bar_Input), NULL);
    gtk_entry_set_placeholder_text(GTK_ENTRY(searchBar), "Search after name");

    // Make a scrollable window
    GtkWidget *scrolledWindow = gtk_scrolled_window_new(NULL, NULL);
    gtk_widget_set_size_request(scrolledWindow, SCROLLABLE_WINDOW_WIDTH, SCROLLABLE_WINDOW_HEIGHT);
    gtk_fixed_put(GTK_FIXED(betterResidentListPage), scrolledWindow, (width-SCROLLABLE_WINDOW_WIDTH)/2, 20+SEARCH_BAR_HEIGHT+20+60);
    AddScrollableWindowToArray(scrolledWindow);

    // Read Database
    char *dsMain = ReadJSON("residents.json");
    int dsLength = GetJSONArrayLength(dsMain);

    // Assign size to list row array
    LIST_ROW_ARRAY_SIZE = dsLength;
    LIST_ROW_ARRAY = (GtkWidget**)malloc(LIST_ROW_ARRAY_SIZE * sizeof(GtkWidget*));
    residentlistButtonClickedDataArray = (ResidentListButtonClickedData**)malloc(LIST_ROW_ARRAY_SIZE * sizeof(ResidentListButtonClickedData*));


    // Add list box
    GtkWidget *listBoxHeaders = NewListBoxHeaders(betterResidentListPage, true, SCROLLABLE_WINDOW_WIDTH, 60, (width-SCROLLABLE_WINDOW_WIDTH)/2, 20+SEARCH_BAR_HEIGHT+20, "");
    GtkWidget *listRowHeader = NewListRow(GTK_BOX(listBoxHeaders), 60);
    GtkWidget *listColumn1Header = NewListTextColumn(GTK_BOX(listRowHeader), 60, "Name", TRUE, "");
    GtkWidget *listColumn2Header = NewListTextColumn(GTK_BOX(listRowHeader), 60, "Room Number", TRUE, "");
    GtkWidget *listColumn3Header = NewListTextColumn(GTK_BOX(listRowHeader), 60, "CPR", TRUE, "");
    GtkWidget *listColumn4Header = NewListTextColumn(GTK_BOX(listRowHeader), 60, "Phone Number", TRUE, "");
    GtkWidget *listColumn5Header = NewListTextColumn(GTK_BOX(listRowHeader), 60, "Administrate Medicine", TRUE, "");

    GtkWidget *listBox = NewListBox(GTK_CONTAINER(scrolledWindow), SCROLLABLE_WINDOW_WIDTH, SCROLLABLE_WINDOW_HEIGHT, "");
    for(int i = 0; i < dsLength; i++) {
        char *dsEntry = GetJSONArrayItem(dsMain, i, FALSE);
        GtkWidget *listRow = NewListRow(GTK_BOX(listBox), 50);
        LIST_ROW_ARRAY[i] = listRow;
        GtkWidget *listColumn1 = NewListTextColumn(GTK_BOX(listRow), 50, GetJSONItem(dsEntry, "full_name"), FALSE, "");
        GtkWidget *listColumn2 = NewListTextColumn(GTK_BOX(listRow), 50, GetJSONItem(dsEntry, "room_number"), FALSE, "");
        GtkWidget *listColumn3 = NewListTextColumn(GTK_BOX(listRow), 50, GetJSONItem(dsEntry, "CPR"), FALSE, "");
        GtkWidget *listColumn4 = NewListTextColumn(GTK_BOX(listRow), 50, GetJSONItem(dsEntry, "phone_number"), FALSE, "");

        ResidentListButtonClickedData *residentListButtonClickedData = malloc(1 * sizeof(ResidentListButtonClickedData));
        residentListButtonClickedData->dsEntry = dsEntry;
        residentListButtonClickedData->stack = stack;
        residentlistButtonClickedDataArray[i] = residentListButtonClickedData;
        residentlistButtonClickedDataArraySize++;
        GtkWidget *listColumn5 = NewListButtonColumn(GTK_BOX(listRow), SCROLLABLE_WINDOW_WIDTH, 50, "      Start Dispensing   >", "", residentListButtonClickedData, NULL);
    }

    return NULL;
}
/**
 * \n Updates the DS entries of all button clicked data structures to the latest data from the database
 */
void UpdateAllButtonDSEntriesToNewDSEntry() {
    for(int i = 0; i < LIST_ROW_ARRAY_SIZE; i++) {
        residentlistButtonClickedDataArray[i]->dsEntry = GetJSONArrayItem(ReadJSON("residents.json"), i, false);
    }
}
