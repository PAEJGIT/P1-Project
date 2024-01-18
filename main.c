// Include
#include <gtk/gtk.h>
#include "src/pages/ResidentListPage.h"
#include "src/pages/OrderPage.h"
#include "src/pages/StoragePage.h"
#include "src/components/menubar.h"
#include "src/utils/helpers.h"
#include "src/configs/constants.h"

GtkWidget *window;

// =====================================================================================================================
// On event functions
// =====================================================================================================================




// =====================================================================================================================
// Main function
// =====================================================================================================================
// argc + argv er GTK parametre - ved ikke hvad de gør
int main(int argc, char *argv[]) {
    // Initialize GTK
    gtk_init(&argc, &argv);

    // Load CSS
    LoadCSS();
//loader generel CSS template
    // Create the main window, indbykkede GTK funktioner:
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "P1 - Program");
    gtk_window_set_default_size(GTK_WINDOW(window), SCREEN_WIDTH, SCREEN_HEIGHT);
    gtk_window_set_resizable(GTK_WINDOW(window), FALSE);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Create a GtkStack
    GtkWidget *stack = gtk_stack_new();
    gtk_widget_set_size_request(stack, SCREEN_WIDTH, SCREEN_HEIGHT-HEADER_HEIGHT);
    // Create a fixed container, definer "hjørnet"
    GtkWidget *fixed = gtk_fixed_new();
    gtk_container_add(GTK_CONTAINER(window), fixed);
    gtk_fixed_put(GTK_FIXED(fixed), stack, 0, 100);

// GTK, 1. parameter sætter eventen, og den givne widget,
// 2. parameter er det event der kalder funktionen på 3 parameter - sætter scroll tilbage til 0.
    g_signal_connect(G_OBJECT(stack), "notify::visible-child", G_CALLBACK(On_Visible_Child_Changed), stack);


    UpdateStorageBasedOnAmount();
    // Creates a menu bar
    GtkWidget *menuBar = MakeMenuBar(fixed);
    // Creates menu buttons
    GtkWidget *residentListButton = MakeMenuBarButton(fixed, "Resident List", SCREEN_WIDTH, HEADER_HEIGHT, stack);
    GtkWidget *storageButton = MakeMenuBarButton(fixed, "Storage", SCREEN_WIDTH, HEADER_HEIGHT, stack);
    GtkWidget *orderButton = MakeMenuBarButton(fixed, "Order", SCREEN_WIDTH, HEADER_HEIGHT, stack);
    // Creates the different pages
    GtkWidget *residentListPage = InitializeResidentList(stack, SCREEN_WIDTH, SCREEN_HEIGHT - HEADER_HEIGHT);
    GtkWidget *storagePage = InitializeStoragePage(stack, SCREEN_WIDTH, SCREEN_HEIGHT-HEADER_HEIGHT);
    GtkWidget *orderPage = InitializeOrderPage(stack, SCREEN_WIDTH, SCREEN_HEIGHT-HEADER_HEIGHT);
    // Show all widgets
    gtk_widget_show_all(window);

    // Start the main GTK loop
    gtk_main();

    return 0;
}
// =====================================================================================================================
// Get main window
// =====================================================================================================================
