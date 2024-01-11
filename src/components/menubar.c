// =====================================================================================================================
// Initialization
// =====================================================================================================================

// Includes
#include <gtk/gtk.h>
#include "menubar.h"
#include "../configs/constants.h"
#include "../pages/ResidentProfilePage.h."

// Global variables
int BUTTONS_MADE = 0;

// BUTTON
#define BUTTON_WIDTH 250
#define BUTTON_HEIGHT 50
#define BUTTON_STYLE "#menu_bar_button { font-size: 16px; border-radius: 24px; color: #043D73; font-weight: 600}"

// =====================================================================================================================
// On event functions
// =====================================================================================================================

void On_Menu_Bar_Button_Clicked(GtkWidget *widget, gpointer user_data) {

    if(TEST_CASE_WINDOW != NULL) {
        MakeWarningWindow("You are currently in the middle of scanning!\nPlease finish scanning before changing page.");
        return;
    }

    const char *label = gtk_button_get_label(GTK_BUTTON(widget));

    if(strcmp(label, "Resident List") == 0) {
        gtk_stack_set_visible_child_name(user_data, "Resident List Page");
    } else if(strcmp(label, "Storage") == 0) {
        gtk_stack_set_visible_child_name(user_data, "Storage Page");
    } else if(strcmp(label, "Order") == 0) {
        gtk_stack_set_visible_child_name(user_data, "Order Page");
    }


}

// =====================================================================================================================
// Header functions
// =====================================================================================================================

// Header MakeMenuBar function
GtkWidget *MakeMenuBar(GtkWidget *fixed) {
    GtkWidget *menuBar = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_widget_set_size_request(menuBar, SCREEN_WIDTH, HEADER_HEIGHT);
    gtk_fixed_put(GTK_FIXED(fixed), menuBar, 0, 0);
    gtk_widget_set_name(menuBar, "colored_menu_bar");
    return menuBar;
}

// Header MakeMenuBarButton function
GtkWidget *MakeMenuBarButton(GtkWidget *fixed, gchar *label, gint menuWidth, gint menuHeight, GtkWidget *stack) {
    GtkWidget *menuBarButton = gtk_button_new_with_label(label);

    BUTTONS_MADE++;

    gint buttonYPosInMenuBar = (menuHeight - BUTTON_HEIGHT)/2;

    int remainder = menuWidth-BUTTON_WIDTH*3;
    int spaceBetweenButtons = remainder/4;
    gint buttonXPosInMenuBar = spaceBetweenButtons * BUTTONS_MADE + (BUTTON_WIDTH * (BUTTONS_MADE-1));

    gchar *data = g_strdup_printf(BUTTON_STYLE);

    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(provider, data,-1, NULL);

    gtk_style_context_add_provider(
            gtk_widget_get_style_context(menuBarButton),
            GTK_STYLE_PROVIDER(provider),
            GTK_STYLE_PROVIDER_PRIORITY_USER
    );

    gtk_widget_set_name(menuBarButton, "menu_bar_button");

    gtk_widget_set_size_request(menuBarButton, BUTTON_WIDTH, BUTTON_HEIGHT);
    gtk_fixed_put(GTK_FIXED(fixed), menuBarButton, buttonXPosInMenuBar, buttonYPosInMenuBar);

    g_signal_connect(menuBarButton, "clicked", G_CALLBACK(On_Menu_Bar_Button_Clicked), stack);

    return menuBarButton;

}
// =====================================================================================================================

