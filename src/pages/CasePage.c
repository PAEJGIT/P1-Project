// Includes
#include <gtk/gtk.h>
#include "stdbool.h"
#include "../utils/helpers.h"
#include "../utils/Checks.h"
#include "../configs/constants.h"

// Initialize functions
GtkWidget *CreateTestCaseWindow();
void CreateTestCase(GtkWidget *box, char *dsEntry);
void On_Test_Case_Button_Clicked(GtkWidget *widget, gpointer user_data) {
    char *string = user_data;
    RunAllChecks(string);
}
GtkWidget *CreateTestCaseWindow() {
    GtkWidget *testCaseWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(testCaseWindow), "Test Case Window");
    gtk_window_set_default_size(GTK_WINDOW(testCaseWindow), 400, WINDOW_HEIGHT);
    gtk_window_set_resizable(GTK_WINDOW(testCaseWindow), FALSE);
    gtk_window_set_position(GTK_WINDOW(testCaseWindow), GTK_WIN_POS_MOUSE );

    int *_screen_dimensions = getScreenDimensions();
    int screen_height = _screen_dimensions[0];
    int screen_width = _screen_dimensions[1];
    gtk_window_move(GTK_WINDOW(testCaseWindow), ((screen_width/2)+1)+((screen_width/4)+2), (screen_height/5)+27);

    ApplyCSSWithID(testCaseWindow, "testCaseWindow", "{ background-color: #FFFFFF; }");
    ApplyCSSOnWidget(testCaseWindow, "window:backdrop", "{ background-color: #FFFFFF; color: black; }");

    GtkWidget *fixedTestCase = gtk_fixed_new();
    gtk_widget_set_size_request(fixedTestCase, 400, WINDOW_HEIGHT);
    gtk_container_add(GTK_CONTAINER(testCaseWindow), fixedTestCase);

    GtkWidget *scrolledWindowTestCase = gtk_scrolled_window_new(NULL, NULL);
    gtk_widget_set_size_request(scrolledWindowTestCase, 400, WINDOW_HEIGHT);
    ApplyCSSWithID(scrolledWindowTestCase, "scrolledWindowTestCase", "{ background-color: #dce0e8; }");
    gtk_fixed_put(GTK_FIXED(fixedTestCase), scrolledWindowTestCase, 0, 0);

    GtkWidget *testCaseWindowBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_size_request(testCaseWindowBox, 400, WINDOW_HEIGHT);
    gtk_box_set_homogeneous(GTK_BOX(testCaseWindowBox), TRUE);
    ApplyCSSWithID(testCaseWindowBox, "testCaseWindowBox", "{margin: 2px }");
    gtk_container_add(GTK_CONTAINER(scrolledWindowTestCase), testCaseWindowBox);

    char *medicineJsonDS = ReadJSON("medicine.json");
    int medicineJsonEntries = GetJSONArrayLength(medicineJsonDS);

    for(int i = 0; i < medicineJsonEntries; i++) {
        CreateTestCase(testCaseWindowBox, GetJSONArrayItem(medicineJsonDS, i, false));
    }

    gtk_widget_show_all(testCaseWindow);
    return testCaseWindow;
}
void CreateTestCase(GtkWidget *box, char *dsEntry) {
    GtkWidget *testCaseBox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_widget_set_size_request(testCaseBox, 40, 80);
    ApplyCSSWithID(testCaseBox, "testCaseBox", "{ background-color: #FFFFFF; margin: 2px; }");
    gtk_box_pack_start(GTK_BOX(box), testCaseBox, FALSE, FALSE, 0);

    char *ecc200Path = GetJSONItem(dsEntry, "ECC_image");
    GdkPixbuf *pixBuf = gdk_pixbuf_new_from_file(ecc200Path, NULL);
    GdkPixbuf *scaledPixBuf = gdk_pixbuf_scale_simple(pixBuf, 80, 80, GDK_INTERP_BILINEAR);
    g_object_unref(G_OBJECT(pixBuf));

    GtkWidget *ecc200 = gtk_image_new_from_pixbuf(scaledPixBuf);
    ApplyCSSWithID(ecc200, "ecc200", "{ border: 2px solid black }");
    gtk_box_pack_start(GTK_BOX(testCaseBox), ecc200, FALSE, FALSE, 0);

    char *medicineName = GetJSONItem(dsEntry, "name");
    char *medicineDosage = GetJSONItem(dsEntry, "dosage");
    char *medicineExpiryDate = GetJSONItem(dsEntry, "expiry_date");

    char *labelValue = g_strdup_printf("Name:  %s\nDosage:  %s\nExpiry date:  %s", medicineName, medicineDosage, medicineExpiryDate);

    GtkWidget *testCaseLabel = gtk_label_new(labelValue);
    gtk_widget_set_size_request(testCaseLabel, 227, 80);
    gtk_label_set_xalign(GTK_LABEL(testCaseLabel), (gfloat)0.05);
    ApplyCSSWithID(testCaseLabel, "TestCaseLabel", "{ background-color: #FFFFFF; }");
    gtk_box_pack_start(GTK_BOX(testCaseBox), testCaseLabel, FALSE, FALSE, 0);

    GtkWidget *testCaseButton = gtk_button_new_with_label(">");
    gtk_widget_set_size_request(testCaseButton, 80, 80);
    GtkWidget *buttonLabel = gtk_bin_get_child(GTK_BIN(testCaseButton));
    ApplyCSSWithID(buttonLabel, "testCaseButtonLabel", "{ margin-bottom: 10px }");
    ApplyCSSWithID(testCaseButton, "testCaseButton",
                   "{ background-color: #043D73; background: #043D73; font-size: 30px; outline: none; box-shadow: none; margin: 5px; border-radius: 10px; color: #FFFFFF;}");
    gtk_box_pack_start(GTK_BOX(testCaseBox), testCaseButton, FALSE, FALSE, 0);
    g_signal_connect(testCaseButton, "clicked", G_CALLBACK(On_Test_Case_Button_Clicked), dsEntry);

}