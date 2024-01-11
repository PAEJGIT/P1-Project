#include <gtk/gtk.h>
#include "ResidentProfilePage.h"
#include "ResidentListPage.h"
#include "../../src/components/button.h"
#include "../../src/utils/helpers.h"
#include "../../src/utils/handlers.h"
#include "../configs/constants.h"
#include "../utils/Checks.h"
#include "../pages/CasePage.h"
#include "../components/icon.h"
#include "../components/separator.h"
#include "../components/flag.h"
#include "../components/action.h"
#include "../components/label.h"


// =====================================================================================================================
// DATA
// =====================================================================================================================
typedef struct {
    char *profileEntry;
    gchar *profileName;
    gchar *profileCPR;
    gchar *profileRoomNumber;
    gchar *profileDateOfBirth;
    gchar *profilePhoneNumber;
    gchar *profileEmergencyNumber;
    gchar *profileConditions;
    gchar *profileQuantity;
    gchar *profileAllergies;
    gchar *profileMedications;
    int correctMedicineScanned;
    gchar *Notes;
} Resident;
Resident resident;
typedef struct {
    char *medicineName;
    char *medicineDosage;
    char *medicineExpiryDate;
    bool correctlyChecked;
} ResidentMedicationStruct;
typedef struct {
    GtkWidget *statusImage;
    GtkWidget *statusLabel;
    GtkWidget *actionLabel;
    GtkWidget *storageLabel;
    GtkWidget *quantityLabel;
} MedicineRowChangableInformation;
typedef struct form_widgets form_widgets;
int residentMedicationStructArraySize = 0;
Resident resident;
ResidentMedicationStruct *residentMedicationStructArray = NULL;
MedicineRowChangableInformation *medicineRowChangableInformationArray = NULL;
GtkWidget** quantityBoxWidgetArray = NULL;
int quantityBoxWidgetArraySize = 0;
char *quantityArray[] = {"0", "0", "0", "0"};

// =====================================================================================================================
// Initialize
// =====================================================================================================================
void UpdateCurrentlyScanningStruct(int integer);
void StopScanning();
GtkWidget *TEST_CASE_WINDOW;
GtkWidget *START_MEDICINE_DISPENSING_BUTTON;
bool finishedScanning = false;

// =====================================================================================================================
// ON EVENT FUNCTIONS
// =====================================================================================================================
static gboolean On_Window_Delete_Event(GtkWidget *widget, GdkEvent *event, gpointer data) {
    TEST_CASE_WINDOW = NULL;
    gtk_button_set_label(GTK_BUTTON(START_MEDICINE_DISPENSING_BUTTON), "Start Scan");
    gtk_widget_set_name(START_MEDICINE_DISPENSING_BUTTON, "button_start_scan");
    if(!finishedScanning) {
        UpdateMedicineRowInformation(ReturnIntegerOfFirstNonCorrectScanned(), "Not scanned", "READY");
    }
    return FALSE;
}
void Button_Start_Medicine_Dispensing(GtkWidget *widget, gpointer user_data) {
    if(finishedScanning){
        gtk_widget_set_name(START_MEDICINE_DISPENSING_BUTTON, "button_start_scan");
        gtk_button_set_image(GTK_BUTTON(START_MEDICINE_DISPENSING_BUTTON), (GtkWidget *) "../src/assets/icons/icon_active_scanning.png");
        return; }
    if(TEST_CASE_WINDOW != NULL) {
        StopScanning();
        return;
    }
    else {
        gtk_widget_set_name(START_MEDICINE_DISPENSING_BUTTON, "button_stop_scan");
    }
    int integerOfFirstNonCorrectScanned = ReturnIntegerOfFirstNonCorrectScanned();
    UpdateCurrentlyScanningStruct(integerOfFirstNonCorrectScanned);
    UpdateMedicineRowInformation(integerOfFirstNonCorrectScanned, "", "");
    TEST_CASE_WINDOW = CreateTestCaseWindow();
    g_signal_connect(G_OBJECT(TEST_CASE_WINDOW), "delete-event", G_CALLBACK(On_Window_Delete_Event), NULL);
    gtk_button_set_label(GTK_BUTTON(START_MEDICINE_DISPENSING_BUTTON), "Stop Scan");
}
void ResetQuantityBox(void){
    quantityArray[0] = "0";
    quantityArray[1] = "0";
    quantityArray[2] = "0";
    quantityArray[3] = "0";
}
void AddQuantityToBox(char *interval, char *amount) {
    // Update with new values
    int quantity;
    char *time = MakeFirstLetterLowercase(interval);
    if (strcmp(time, "morning") == 0){
        quantity = ConvertStringToInt(quantityArray[0]);
        quantity += ConvertStringToInt(amount) / 14;
        char *result = g_strdup_printf("%d", quantity);
        quantityArray[0] = result;
    }
    if (strcmp(time, "lunch") == 0){
        quantity = ConvertStringToInt(quantityArray[1]);
        quantity += ConvertStringToInt(amount) / 14;
        char *result = g_strdup_printf("%d", quantity);
        quantityArray[1] = result;
    }
    if (strcmp(time, "evening") == 0){
        quantity = ConvertStringToInt(quantityArray[2]);
        quantity += ConvertStringToInt(amount) / 14;
        char *result = g_strdup_printf("%d", quantity);
        quantityArray[2] = result;
    }
    if (strcmp(time, "night") == 0){
        quantity = ConvertStringToInt(quantityArray[3]);
        quantity += ConvertStringToInt(amount) / 14;
        char *result = g_strdup_printf("%d", quantity);
        quantityArray[3] = result;
    }

}


// =====================================================================================================================
// OTHER
// =====================================================================================================================
int ReturnIntegerOfFirstNonCorrectScanned() {
    for(int i = 0; i < residentMedicationStructArraySize; i++) {
        if(!residentMedicationStructArray[i].correctlyChecked) {
            return i;
        }
    }
    return residentMedicationStructArraySize;
}
void UpdateCurrentlyScanningStruct(int integer) {
    CurrentlyScanning newCurrentlyScanning;
    newCurrentlyScanning.medicineName = residentMedicationStructArray[integer].medicineName;
    newCurrentlyScanning.medicineDosage = residentMedicationStructArray[integer].medicineDosage;
    newCurrentlyScanning.medicineExpiryDate = residentMedicationStructArray[integer].medicineExpiryDate;
    newCurrentlyScanning.allResidentMedicine = resident.profileMedications;
    newCurrentlyScanning.residentDSEntry = resident.profileEntry;
    currentlyScanning = newCurrentlyScanning;
}
void UpdateMedicineRowInformation(int integer, char *status, char *action) {
    GList *statusLabelChildren = gtk_container_get_children(GTK_CONTAINER(medicineRowChangableInformationArray[integer].statusLabel));
    GtkWidget *statusLabelChild = GTK_WIDGET(statusLabelChildren->data);

    GList *statusImageChildren = gtk_container_get_children(GTK_CONTAINER(medicineRowChangableInformationArray[integer].statusImage));
    GtkWidget *statusImageChild = GTK_WIDGET(statusImageChildren->data);

    GList *actionLabelChildren = gtk_container_get_children(GTK_CONTAINER(medicineRowChangableInformationArray[integer].actionLabel));
    GtkWidget *actionLabelChild = GTK_WIDGET(actionLabelChildren->data);

    if(strcmp(status, "Error") == 0 || strcmp(status, "Failed") == 0 ) {
        gtk_label_set_text(GTK_LABEL(statusLabelChild), status);
        gtk_image_set_from_file(GTK_IMAGE(statusImageChild), "../src/assets/flags/flag_0.png");
    }
    if(strcmp(status, "Warning") == 0 || strcmp(status, "Shortage") == 0) {
        gtk_label_set_text(GTK_LABEL(statusLabelChild), status);
        gtk_image_set_from_file(GTK_IMAGE(statusImageChild), "../src/assets/flags/flag_1.png");
    }
    if(strcmp(status, "Success") == 0) {
        gtk_label_set_text(GTK_LABEL(statusLabelChild), "Success!");
        gtk_image_set_from_file(GTK_IMAGE(statusImageChild), "../src/assets/flags/flag_2.png");
    }
    if(strcmp(status, "Not scanned") == 0) {
        gtk_label_set_text(GTK_LABEL(statusLabelChild), status);
        gtk_image_set_from_file(GTK_IMAGE(statusImageChild), "../src/assets/flags/flag_3.png");
    }
    if(strcmp(status, "") == 0) {
        gtk_label_set_text(GTK_LABEL(statusLabelChild), "Pending..");
        gtk_image_set_from_file(GTK_IMAGE(statusImageChild), "../src/assets/flags/flag_4.png");
    }
    if(strcmp(action, "") == 0) {
        gtk_label_set_text(GTK_LABEL(actionLabelChild), "Scanning..");
    } else {
        gtk_label_set_text(GTK_LABEL(actionLabelChild), action);
    }
}
void UpdateMedicineRowInformationQuantity(char *newQuantity) {
    GList *children = gtk_container_get_children(GTK_CONTAINER(medicineRowChangableInformationArray[ReturnIntegerOfFirstNonCorrectScanned()].quantityLabel));
    GtkWidget *labelWidget = GTK_WIDGET(g_list_nth_data(children, 0));
    gtk_label_set_text(GTK_LABEL(labelWidget), newQuantity);
}
void UpdateMedicineRowInformationStorage() {
    char *residentDS = ReadJSON("residents.json");
    for(int i = 0; i < GetJSONArrayLength(residentDS); i++) {
        char *dsEntry = GetJSONArrayItem(residentDS, i, false);
        char *residentName = GetJSONItem(dsEntry, "full_name");
        if(strcmp(residentName, resident.profileName) == 0) {
            char *residentMedicineArray = GetJSONArray(dsEntry, "medicine");
            for(int l = 0; l < GetJSONArrayLength(residentMedicineArray); l++) {

                GList *storageChildren = gtk_container_get_children(GTK_CONTAINER(medicineRowChangableInformationArray[l].storageLabel));
                GtkWidget *storageLabelWidget = GTK_WIDGET(g_list_nth_data(storageChildren, 0));

                GList *quantityChildren = gtk_container_get_children(GTK_CONTAINER(medicineRowChangableInformationArray[l].quantityLabel));
                GtkWidget *quantityLabelWidget = GTK_WIDGET(g_list_nth_data(quantityChildren, 0));

                char *medicineStorage = GetJSONArrayItem(GetJSONArrayItem(residentMedicineArray, l, false), 2, true);
                char *medicineQuantity = GetJSONArrayItem(GetJSONArrayItem(residentMedicineArray, l, false), 5, true);

                //char str[12];

                //snprintf(str, sizeof(str), "%d", medicineQuantity);

                gtk_label_set_text(GTK_LABEL(storageLabelWidget), medicineStorage);
                gtk_label_set_text(GTK_LABEL(quantityLabelWidget), medicineQuantity);
            }
        }
    }
}
void UpdateQuantityBoxes(char *quantity) {
    char *interval = GetJSONArrayItem(
            GetJSONArrayItem(resident.profileMedications, ReturnIntegerOfFirstNonCorrectScanned(), false), 3, true);
    AddQuantityToBox(interval, quantity);
    for(int l = 0; l < quantityBoxWidgetArraySize; l++) {
        GList *children = gtk_container_get_children(GTK_CONTAINER(quantityBoxWidgetArray[l]));
        GtkWidget *boxWidget = GTK_WIDGET(g_list_nth_data(children, 1));
        GList *boxWidgetChildren = gtk_container_get_children(GTK_CONTAINER(boxWidget));
        GtkWidget *labelWidget = GTK_WIDGET(g_list_nth_data(boxWidgetChildren, 0));
        gtk_label_set_text(GTK_LABEL(labelWidget), quantityArray[l]);
    }
}

// =====================================================================================================================
// SCANNING
// =====================================================================================================================
void CorrectlyScanned(int integer) {
    if(ReturnIntegerOfFirstNonCorrectScanned() != residentMedicationStructArraySize) {
        residentMedicationStructArray[integer].correctlyChecked = TRUE;
        if(ReturnIntegerOfFirstNonCorrectScanned() != residentMedicationStructArraySize) {
            UpdateCurrentlyScanningStruct(ReturnIntegerOfFirstNonCorrectScanned());
            UpdateMedicineRowInformation(ReturnIntegerOfFirstNonCorrectScanned(), "", "");
        } else {
            finishedScanning = true;
            StopScanning();
        }
        if(resident.correctMedicineScanned < GetJSONArrayLength(resident.profileMedications)) {
            resident.correctMedicineScanned = GetJSONItemInt(resident.profileEntry, "correct_medicine_scanned") + 1;
            char *newDSString = UpdateStorageValue("residents.json", resident.profileEntry,
                                                   "correct_medicine_scanned", resident.correctMedicineScanned, "",
                                                   "", 0, false);
            resident.profileEntry = UpdateProfile(newDSString, "full_name", resident.profileName);
            UpdateAllButtonDSEntriesToNewDSEntry();
        }
    } else {
        //Call finished scanning everything function
        return;
    }
}
void StopScanning() {
    if(!finishedScanning) {
        UpdateMedicineRowInformation(ReturnIntegerOfFirstNonCorrectScanned(), "Not scanned", "READY");
        gtk_button_set_label(GTK_BUTTON(START_MEDICINE_DISPENSING_BUTTON), "Start Scan");
        gtk_widget_set_name(START_MEDICINE_DISPENSING_BUTTON, "button_start_scan");
    } else {
        gtk_widget_set_name(START_MEDICINE_DISPENSING_BUTTON, "button_finished_scan");
        gtk_button_set_label(GTK_BUTTON(START_MEDICINE_DISPENSING_BUTTON), "Finished Scanning");
        gtk_widget_set_sensitive(START_MEDICINE_DISPENSING_BUTTON, FALSE);
    }
    gtk_widget_destroy(TEST_CASE_WINDOW);
    TEST_CASE_WINDOW = NULL;
}
void UpdateResidentInformationStruct(char *dsEntry) {
    resident.profileEntry = dsEntry;
    resident.profileName = GetJSONItem(dsEntry, "full_name");
    resident.profileCPR = GetJSONItem(dsEntry, "CPR");
    resident.profileRoomNumber = GetJSONItem(dsEntry, "room_number");
    resident.profileDateOfBirth = GetJSONItem(dsEntry, "date_of_birth");
    resident.profilePhoneNumber = GetJSONItem(dsEntry, "phone_number");
    resident.profileEmergencyNumber = GetJSONArrayItem(GetJSONArray(dsEntry, "emergency_contact"), 0, TRUE);
    resident.profileConditions = GetJSONArray(dsEntry, "conditions");
    resident.profileAllergies = GetJSONArray(dsEntry, "allergies");
    resident.profileMedications = GetJSONArray(dsEntry, "medicine");
    resident.correctMedicineScanned = GetJSONItemInt(dsEntry, "correct_medicine_scanned");
    resident.Notes = "No notes found regarding resident.";
}
void CheckIfScannedCorrectlyInDatabase() {
    if(residentMedicationStructArraySize == 0) {
        Log(__FUNCTION__, "INFO", "Array size is 0");
        return;
    }
    for(int i = 0; i < resident.correctMedicineScanned; i++) {
        if(i > residentMedicationStructArraySize) {return;}
        residentMedicationStructArray[i].correctlyChecked = true;
        UpdateMedicineRowInformation(i, "Success", "Match");
    }
    if(GetJSONArrayLength(resident.profileMedications) == resident.correctMedicineScanned) {
        finishedScanning = true;
        gtk_button_set_label(GTK_BUTTON(START_MEDICINE_DISPENSING_BUTTON), "Finished Scanning");
        gtk_widget_set_sensitive(START_MEDICINE_DISPENSING_BUTTON, FALSE);
    }

}

// =====================================================================================================================
// LEFT PANEL
// =====================================================================================================================
GtkWidget *LEFT_PANEL_HEADER() {
    // Generate random number to choose profile avatar
    int random_num = GetRandomNumber(1, 20);
    // Get image for profile
    GtkWidget *image = gtk_image_new_from_file(g_strdup_printf("../src/assets/avatars/circle_%d.png", random_num));
    // Set size of image
    gtk_widget_set_size_request(image, 70, 70);

    GtkWidget *grid = gtk_grid_new();
    GtkWidget *image_text = gtk_label_new(resident.profileRoomNumber);
    gtk_widget_set_name(image_text, "profile_header_roomNumber");

    gtk_grid_attach(GTK_GRID(grid), image_text, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), image, 0, 0, 1, 1);

    // Create container for image
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 15);
    // Add image to container
    gtk_container_add(GTK_CONTAINER(vbox), grid);
    // Set name for container
    gtk_widget_set_name(vbox, "profile_header_image");
    // Create container to contain title and subtitle
    GtkWidget *hbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    // Create the Headers title
    GtkWidget *label_residentName = gtk_label_new(resident.profileName);
    gtk_widget_set_name(label_residentName, "profile_header_title");
    gtk_widget_set_halign(label_residentName, GTK_ALIGN_START);
    // Create the Headers subtitle
    GtkWidget *label_CPR = gtk_label_new(resident.profileCPR);
    gtk_widget_set_name(label_CPR, "profile_header_subtitle");
    gtk_widget_set_halign(label_CPR, GTK_ALIGN_START);
    // Add title to box container
    gtk_container_add(GTK_CONTAINER(hbox), label_residentName);
    // Add subtitle to box container
    gtk_container_add(GTK_CONTAINER(hbox), label_CPR);
    // Pack the box into the vbox
    gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, FALSE, 0);
    gtk_widget_set_name(vbox, "profile_header_container");
    // Return the vbox
    return vbox;
}
GtkWidget *LEFT_PANEL_TAGS() {
    GtkWidget *grid = gtk_grid_new();
    gtk_widget_set_name(grid, "profile_content_grid");
    int CONDITIONS_LENGTH = GetJSONArrayLength(resident.profileConditions);
    for (int i = 0; i < CONDITIONS_LENGTH; i++) {
        // Create container to contain tags
        GtkWidget *hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
        int randomNumber = GetRandomNumber(1, 10);
        gtk_widget_set_name(hbox, g_strdup_printf("profile_tags_container_%d", randomNumber));
        // Create the title of tag
        GtkWidget *label_tag = gtk_label_new(GetJSONArrayItem(resident.profileConditions, i, TRUE));
        // Set classname of tag
        gtk_widget_set_name(label_tag, "profile_tags_label");
        gtk_widget_set_halign(hbox, GTK_ALIGN_FILL);
        gtk_label_set_xalign(GTK_LABEL(label_tag), (gfloat)(0.5));
        // Add label to box container
        gtk_container_add(GTK_CONTAINER(hbox), label_tag);
        // Add container to grid
        if (i < 4) {
            gtk_grid_attach(GTK_GRID(grid), hbox, i, 0, 1, 1);
        } else {
            gtk_grid_attach(GTK_GRID(grid), hbox, i-4, 1, 1, 1);
        }
    }
    return grid;
}
GtkWidget *LEFT_PANEL_CONTENT() {
    // Create container to contain title and subtitle
    GtkWidget *hbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    // Create the Headers title
    GtkWidget *label_residentName = gtk_label_new("Resident Information");
    gtk_widget_set_name(label_residentName, "profile_content_title");
    gtk_widget_set_halign(label_residentName, GTK_ALIGN_START);
    // Create the Headers subtitle
    GtkWidget *label_CPR = gtk_label_new(g_strdup_printf("Relevant information regarding %s", resident.profileName));
    gtk_widget_set_name(label_CPR, "profile_content_subtitle");
    gtk_widget_set_halign(label_CPR, GTK_ALIGN_START);
    // Add title to box container
    gtk_container_add(GTK_CONTAINER(hbox), label_residentName);
    // Add subtitle to box container
    gtk_container_add(GTK_CONTAINER(hbox), label_CPR);
    // Create grid
    GtkWidget *grid = gtk_grid_new();
    // Set CSS Class to grid
    gtk_widget_set_name(grid, "profile_content_grid");
    // Create Header titles for information panel
    char *arrKeys[] = {"Full Name:", "Room Number:", "Date of Birth:", "Phone Number:", "Emergency Number",  "Allergies"};


    // Create values to Header Titles
    char *name = resident.profileName;
    char *room = resident.profileRoomNumber;
    char *date = resident.profileDateOfBirth;
    char *phone = resident.profilePhoneNumber;
    char *emergency = resident.profileEmergencyNumber;
    //char *conditions = CombineStringArray(resident.profileConditions, TRUE);
    char *allergies = CombineStringArray(resident.profileAllergies, TRUE);
    // Assign values to Array
    char *arrValues[] = {name, room, date, phone, emergency,  allergies};

    // Get length of array
    int arrayLength = (sizeof(arrKeys) / sizeof(*arrKeys));
    // Add Header titles and Values to information panel
    for (int i = 0; i < arrayLength; i++) {
        GtkWidget *residentName = gtk_label_new(arrKeys[i]);
        if (i == 5) {
            // Add separator to information panel
            GtkWidget *separator = SeparatorComponent(200, 1, "resident-profile-separator_transparent");
            gtk_grid_attach(GTK_GRID(grid), separator, 0, i, 1, 1);

            gtk_widget_set_name(residentName, "profile_content_key");
            gtk_widget_set_halign(residentName, GTK_ALIGN_START);
            gtk_widget_set_hexpand(residentName, TRUE);
            gtk_grid_attach(GTK_GRID(grid), residentName, 0, 1+i, 1, 1);

            GtkWidget *residentName_DS = gtk_label_new(arrValues[i]);
            GtkWidget *residentName_Box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
            gtk_widget_set_name(residentName_DS, "profile_content_value");
            gtk_widget_set_halign(residentName_DS, GTK_ALIGN_END);
            gtk_label_set_xalign(GTK_LABEL(residentName_DS), (gfloat)(1));
            gtk_label_set_justify(GTK_LABEL(residentName_DS), GTK_JUSTIFY_RIGHT);
            gtk_container_add(GTK_CONTAINER(residentName_Box), residentName_DS);
            gtk_grid_attach(GTK_GRID(grid), residentName_Box, 1, 1+i, 1, 1);
        } else {
            gtk_widget_set_name(residentName, "profile_content_key");
            gtk_widget_set_halign(residentName, GTK_ALIGN_START);
            gtk_widget_set_hexpand(residentName, TRUE);
            gtk_grid_attach(GTK_GRID(grid), residentName, 0, i, 1, 1);

            GtkWidget *residentName_DS = gtk_label_new(arrValues[i]);
            GtkWidget *residentName_Box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
            gtk_widget_set_name(residentName_DS, "profile_content_value");
            gtk_widget_set_halign(residentName_DS, GTK_ALIGN_END);
            gtk_label_set_xalign(GTK_LABEL(residentName_DS), (gfloat)(1));
            gtk_label_set_justify(GTK_LABEL(residentName_DS), GTK_JUSTIFY_RIGHT);
            gtk_container_add(GTK_CONTAINER(residentName_Box), residentName_DS);
            gtk_grid_attach(GTK_GRID(grid), residentName_Box, 1, i, 1, 1);
        }
        if (i == 5) {
            // Add separator to information panel
            GtkWidget *separator = SeparatorComponent(200, 1, "resident-profile-separator_transparent");
            gtk_grid_attach(GTK_GRID(grid), separator, 0, 10, 1, 1);
            // Add Notes to information panel
            GtkWidget *residentNotes = gtk_label_new("Notes about Resident");
            gtk_widget_set_name(residentNotes, "profile_content_key");
            gtk_widget_set_halign(residentNotes, GTK_ALIGN_START);
            gtk_widget_set_hexpand(residentNotes, TRUE);
            gtk_grid_attach(GTK_GRID(grid), residentNotes, 0, 11, 1, 1);
            GtkWidget *residentNotes_DS = gtk_label_new(resident.Notes);
            gtk_widget_set_name(residentNotes_DS, "profile_content_value");
            gtk_widget_set_halign(residentNotes_DS, GTK_ALIGN_START);
            gtk_grid_attach(GTK_GRID(grid), residentNotes_DS, 0, 12, 1, 1);
        }
    }
    // Add grid to box container
    gtk_container_add(GTK_CONTAINER(hbox), grid);
    // Return the vbox
    return hbox;
}

// =====================================================================================================================
// RIGHT PANEL
// =====================================================================================================================
GtkWidget *RIGHT_PANEL_HEADER(){
    GtkWidget *grid = gtk_grid_new();
    GtkWidget *headerTitle = gtk_label_new("Medications");
    gtk_widget_set_name(headerTitle, "profile_header_medications");
    gtk_grid_attach(GTK_GRID(grid), headerTitle, 0, 0, 1, 1);
    return grid;
}
GtkWidget *MEDICATION_BOX_HEADER(int department, char *title, char *subtitle, char *quantity) {
    // Variables
    int box_height = 140;
    int box_width = 103;

    // Widgets
    GtkWidget *primaryBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    GtkWidget *headerGrid = gtk_grid_new();
    GtkWidget *headerIconLeft = NULL;
    GtkWidget *headerIconRight = NULL;
    GtkWidget *titleLabel = gtk_label_new(title);
    GtkWidget *subtitleLabel = gtk_label_new(subtitle);
    GtkWidget *morningGridLabels = gtk_grid_new();
    GtkWidget *morningBoxLabels = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    GtkWidget *box_quantity= gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

    GtkWidget *label_quantity_title = gtk_label_new(quantity);
    int _quantity = ConvertStringToInt(quantity);

    if (department == 1){
        if (_quantity == 0){
            headerIconLeft = (GtkWidget *) IconComponent("icon_pill_1_off");
        }
        else {
            headerIconLeft = (GtkWidget *) IconComponent("icon_pill_1");
        }
    }
    else if (department == 2){
        if (_quantity == 0){
            headerIconLeft = (GtkWidget *) IconComponent("icon_pill_2_off");
        }
        else {
            headerIconLeft = (GtkWidget *) IconComponent("icon_pill_2");
        }
    }
    else if (department == 3){
        if (_quantity == 0){
            headerIconLeft = (GtkWidget *) IconComponent("icon_pill_3_off");
        }
        else {
            headerIconLeft = (GtkWidget *) IconComponent("icon_pill_3");
        }
    }
    else if (department == 4){
        if (_quantity == 0){
            headerIconLeft = (GtkWidget *) IconComponent("icon_pill_4_off");
        }
        else {
            headerIconLeft = (GtkWidget *) IconComponent("icon_pill_4");
        }
    }
    else {
        Log(__FUNCTION__ , "ERROR", "Failed setting icon");
    }
    GtkWidget *label_quantity_subtitle = gtk_label_new("TABLETS");

    // CSS
    gtk_widget_set_name(primaryBox, "medication_box");
    gtk_widget_set_name(titleLabel, "medication_box_title");
    gtk_widget_set_name(subtitleLabel, "medication_box_subtitle");
    gtk_widget_set_name(box_quantity, "medication_box_quantity");
    gtk_widget_set_name(label_quantity_title, "medication_label_quantity_title");
    gtk_widget_set_name(label_quantity_subtitle, "medication_label_quantity_subtitle");

    // Alignment
    gtk_label_set_xalign((GtkLabel *) subtitleLabel, (gfloat)(0.0));
    gtk_label_set_justify((GtkLabel *) subtitleLabel, GTK_JUSTIFY_LEFT);

    // Add and attach
    gtk_grid_attach(GTK_GRID(morningGridLabels), titleLabel, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(morningGridLabels), subtitleLabel, 0, 1, 1, 1);
    gtk_widget_set_name(morningGridLabels, "medication_grid_header");
    gtk_container_add(GTK_CONTAINER(morningBoxLabels), morningGridLabels);
    gtk_widget_set_size_request(morningBoxLabels, box_width-55, 15);
    gtk_grid_attach(GTK_GRID(headerGrid), headerIconLeft, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(headerGrid), morningBoxLabels, 1, 0, 1, 1);
    //gtk_grid_attach(GTK_GRID(headerGrid), headerIconRight, 2, 0, 1, 1);
    gtk_container_add(GTK_CONTAINER(box_quantity), label_quantity_title);

    // Pack components from highest to lowest order
    gtk_box_pack_start(GTK_BOX(primaryBox), headerGrid, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(primaryBox), box_quantity, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(primaryBox), label_quantity_subtitle, TRUE, TRUE, 0);

    // Dimensions
    gtk_widget_set_size_request(primaryBox, box_width, box_height);
    gtk_widget_set_size_request(box_quantity, box_width, box_height-70);

    return primaryBox;
}
GtkWidget *MEDICATION_BOX() {
    // Create grid
    GtkWidget *grid_container = gtk_grid_new();
    gtk_widget_set_name(grid_container, "medication_grid");

    quantityBoxWidgetArray = (GtkWidget**)malloc(4 * sizeof(GtkWidget*));

    // Add to grid
    GtkWidget *morningQuantityBox = MEDICATION_BOX_HEADER(1,  "Morning", "08:00", quantityArray[0]);
    GtkWidget *middayQuantityBox = MEDICATION_BOX_HEADER(2,  "Midday", "12:00", quantityArray[1]);
    GtkWidget *eveningQuantityBox = MEDICATION_BOX_HEADER(3,  "Evening", "18:00", quantityArray[2]);
    GtkWidget *nightQuantityBox = MEDICATION_BOX_HEADER(4,  "Night", "22:00", quantityArray[3]);
    quantityBoxWidgetArray[0] = morningQuantityBox;
    quantityBoxWidgetArray[1] = middayQuantityBox;
    quantityBoxWidgetArray[2] = eveningQuantityBox;
    quantityBoxWidgetArray[3] = nightQuantityBox;
    quantityBoxWidgetArraySize = 4;
    gtk_grid_attach(GTK_GRID(grid_container), morningQuantityBox, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid_container), middayQuantityBox, 1, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid_container), eveningQuantityBox, 2, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid_container), nightQuantityBox, 3, 0, 1, 1);
    gtk_grid_set_column_spacing((GtkGrid *) grid_container, 6);
    return grid_container;
}
GtkWidget *MEDICATION_TABLE() {
    GtkWidget *grid = gtk_grid_new();
    gtk_widget_set_name(grid, "profile_medication_grid");

    GtkWidget *TABLE_FLAGS = gtk_label_new("FLAGS");
    GtkWidget *TABLE_NAME = gtk_label_new("NAME");
    GtkWidget *TABLE_INTERVAL = gtk_label_new("INTERVAL");
    GtkWidget *TABLE_DOSAGE = gtk_label_new("DOSAGE");
    GtkWidget *TABLE_QUANTITY = gtk_label_new("QUANTITY");
    GtkWidget *TABLE_STORAGE = gtk_label_new("STOCK");
    GtkWidget *TABLE_STATUS = gtk_label_new("STATUS");
    GtkWidget *TABLE_ACTION = gtk_label_new("ACTION");

    gtk_widget_set_name(TABLE_FLAGS, "profile_medication_table_header_flags");
    gtk_widget_set_name(TABLE_NAME, "profile_medication_table_header_name");
    gtk_widget_set_name(TABLE_INTERVAL, "profile_medication_table_header_interval");
    gtk_widget_set_name(TABLE_DOSAGE, "profile_medication_table_header_dosage");
    gtk_widget_set_name(TABLE_QUANTITY, "profile_medication_table_header_quantity");
    gtk_widget_set_name(TABLE_STORAGE, "profile_medication_table_header_storage");
    gtk_widget_set_name(TABLE_STATUS, "profile_medication_table_header_status");
    gtk_widget_set_name(TABLE_ACTION, "profile_medication_table_header_action");


    gtk_grid_attach(GTK_GRID(grid), TABLE_FLAGS, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), TABLE_NAME, 1, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), TABLE_INTERVAL, 2, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), TABLE_DOSAGE, 3, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), TABLE_QUANTITY, 4, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), TABLE_STORAGE, 5, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), TABLE_STATUS, 6, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), TABLE_ACTION, 7, 0, 1, 1);


    return grid;
}
GtkWidget *MEDICATION_TABLE_CONTENT() {
    finishedScanning = false;
    // Get length of medication list for resident
    int DATABASE_LENGTH = GetJSONArrayLength(resident.profileMedications);
    residentMedicationStructArray = malloc(DATABASE_LENGTH * sizeof(ResidentMedicationStruct));
    medicineRowChangableInformationArray = malloc(DATABASE_LENGTH * sizeof(MedicineRowChangableInformation));
    // Create grid
    GtkWidget *grid = gtk_grid_new();
    // Set classname of grid
    gtk_widget_set_name(grid, "profile_medication_grid_content");
    // Reset quantities
    ResetQuantityBox();
    for (int i = 0; i < DATABASE_LENGTH; i++){
        char *MEDICATION_NAME = GetJSONArrayItem(GetJSONArrayItem(resident.profileMedications, i, FALSE), 0, TRUE);
        char *MEDICATION_DOSAGE = GetJSONArrayItem(GetJSONArrayItem(resident.profileMedications, i, FALSE), 1, TRUE);
        char *MEDICATION_INTERVAL = GetJSONArrayItem(GetJSONArrayItem(resident.profileMedications, i, FALSE), 3, TRUE);
        char *MEDICATION_QUANTITY = GetJSONArrayItem(GetJSONArrayItem(resident.profileMedications, i, FALSE), 5, TRUE);

        ResidentMedicationStruct residentMedicationStruct;
        residentMedicationStruct.medicineName = MEDICATION_NAME;
        residentMedicationStruct.medicineDosage = MEDICATION_DOSAGE;
        // Get content (e.g. 5mg) of designated medication name
        char *medicineMass = GetContentOfMedication(MEDICATION_NAME);
        // Calculate total quantity needed to get correct dosage and assign
        //resident.profileQuantity = CalculateDosageEach(MEDICATION_DOSAGE, medicineMass);

        // Update quantities
        AddQuantityToBox(MEDICATION_INTERVAL, MEDICATION_QUANTITY);
        residentMedicationStruct.medicineExpiryDate = GetJSONArrayItem(GetJSONArrayItem(resident.profileMedications, i, FALSE),
                                                                       4, TRUE);
        residentMedicationStruct.correctlyChecked = FALSE;
        residentMedicationStructArray[i] = residentMedicationStruct;
        residentMedicationStructArraySize++;
        MedicineRowChangableInformation medicineRowChangableInformation;
        // Set value of FLAG (choose between "success", "warning" or "error")
        GtkWidget *flag = FlagComponent("default");
        medicineRowChangableInformation.statusImage = flag;
        gtk_grid_attach(GTK_GRID(grid), flag, 0, i, 1, 1);
        // Set value of NAME
        gtk_grid_attach(GTK_GRID(grid), LabelComponent((MEDICATION_NAME), 85, "name"), 1, i, 1, 1);
        // Set value of INTERVAL
        gtk_grid_attach(GTK_GRID(grid), LabelComponent(MEDICATION_INTERVAL, 75, "interval"), 2, i, 1, 1);
        // Set value of DOSAGE
        gtk_grid_attach(GTK_GRID(grid), LabelComponent(MEDICATION_DOSAGE, 45, "dosage"), 3, i, 1, 1);
        // Set value of QUANTITY
        int quantityForTwoWeeks = ConvertStringToInt(MEDICATION_QUANTITY);
        GtkWidget *quantityLabel = LabelComponent(ConvertIntToString(quantityForTwoWeeks), 45, "quantity");
        gtk_grid_attach(GTK_GRID(grid), quantityLabel, 4, i, 1, 1);
        // Set value of STORAGE
        GtkWidget *storageLabel = LabelComponent(GetQuantityOfMedication(MEDICATION_NAME, resident.profileMedications), 45,
                                                 "storage");
        gtk_grid_attach(GTK_GRID(grid), storageLabel, 5, i, 1, 1);
        /*
        if (convertStringToInt(GetQuantityOfMedication(MEDICATION_NAME, resident.profileMedications)) < 14){
            storageLabel = CREATE_LABEL(GetQuantityOfMedication(MEDICATION_NAME, resident.profileMedications), 45, "storage_low");
            gtk_grid_attach(GTK_GRID(grid), storageLabel, 4, i, 1, 1);
        }
        else if (convertStringToInt(GetQuantityOfMedication(MEDICATION_NAME, resident.profileMedications)) < 28){
            storageLabel = CREATE_LABEL(GetQuantityOfMedication(MEDICATION_NAME, resident.profileMedications), 45, "storage_med");
            gtk_grid_attach(GTK_GRID(grid), storageLabel, 4, i, 1, 1);
        }
        else if (ConvertStringToInt(GetQuantityOfMedication(MEDICATION_NAME, resident.profileMedications)) > 28){
            storageLabel = CREATE_LABEL(GetQuantityOfMedication(MEDICATION_NAME, resident.profileMedications), 45, "storage");
            gtk_grid_attach(GTK_GRID(grid), storageLabel, 4, i, 1, 1);
        }
        else {
            storageLabel = LabelComponent(GetQuantityOfMedication(MEDICATION_NAME, resident.profileMedications), 45, "storage");
            gtk_grid_attach(GTK_GRID(grid), storageLabel, 4, i, 1, 1);
        }
        */
        medicineRowChangableInformation.quantityLabel = quantityLabel;
        medicineRowChangableInformation.storageLabel = storageLabel;
        //printf("\n                      MEDICATION_TABLE_CONTENT: Added storage to grid\n");
        // Set value of STATUS
        GtkWidget *status = LabelComponent("Not scanned", 45, "status");
        medicineRowChangableInformation.statusLabel = status;
        gtk_grid_attach(GTK_GRID(grid), status, 6, i, 1, 1);
        // Set value of ACTION
        GtkWidget *action = ActionComponent("READY");
        medicineRowChangableInformation.actionLabel = action;
        gtk_grid_attach(GTK_GRID(grid), action, 7, i, 1, 1);
        medicineRowChangableInformationArray[i] = medicineRowChangableInformation;
    }
    return grid;
}

// =====================================================================================================================
// PAGE
// =====================================================================================================================
GtkWidget *InitializeResidentMedicineDosagePage(GtkWidget *stack, gint width, gint height, char *dsEntry) {
    GtkWidget *Header;
    GtkWidget *TAGS;
    GtkWidget *RESIDENT_CONTENT;
    UpdateResidentInformationStruct(dsEntry);
    residentMedicationStructArraySize = 0;

    // Make a new fixed window and add it to the stack
    GtkWidget *residentMedicineDosagePage = gtk_fixed_new();
    gtk_widget_set_size_request(residentMedicineDosagePage, width, height);

    // LEFT PANEL Make a new fixed window for the residents personal information, such as: Name, social security number, ect...
    GtkWidget *residentPersonalInformation = gtk_fixed_new();
    gtk_widget_set_size_request(residentPersonalInformation, LEFT_PANEL_WIDTH, LEFT_PANEL_HEIGHT);
    gtk_fixed_put(GTK_FIXED(residentMedicineDosagePage), residentPersonalInformation, CenterObjectsWithEqualWidthOnPage(width, 3, RESIDENT_PERSONAL_INFORMATION_AND_MEDICINE_WIDTH, 1), 20);

    // Make the window scrollable
    GtkWidget *residentPersonalInformationScrollableWindow = gtk_scrolled_window_new(NULL, NULL);
    gtk_widget_set_size_request(residentPersonalInformationScrollableWindow, LEFT_PANEL_WIDTH, LEFT_PANEL_HEIGHT);
    gtk_fixed_put(GTK_FIXED(residentPersonalInformation), residentPersonalInformationScrollableWindow, 0, 0);

    // Make background on the fixed window
    GtkWidget *fixedBackgroundPersonalInformation = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_size_request(fixedBackgroundPersonalInformation, 400, LEFT_PANEL_HEIGHT);
    gtk_container_add(GTK_CONTAINER(residentPersonalInformationScrollableWindow), fixedBackgroundPersonalInformation);
    // Add label to fixed window
    gtk_widget_set_name(fixedBackgroundPersonalInformation, "resident-profile-left");
    // Add Header Box to Left Panel
    Header = LEFT_PANEL_HEADER();
    gtk_container_add(GTK_CONTAINER(fixedBackgroundPersonalInformation), Header);
    // Add SeparatorComponent to Left Panel
    GtkWidget *separator_profile = SeparatorComponent(150, 1, "resident-profile-separator");
    gtk_container_add(GTK_CONTAINER(fixedBackgroundPersonalInformation), separator_profile);
    // Add Tags to left Panel
    TAGS = LEFT_PANEL_TAGS();
    gtk_container_add(GTK_CONTAINER(fixedBackgroundPersonalInformation), TAGS);
    // Add Content Box to left Panel
    RESIDENT_CONTENT = LEFT_PANEL_CONTENT();
    gtk_container_add(GTK_CONTAINER(fixedBackgroundPersonalInformation), RESIDENT_CONTENT);
    // RIGHT PANEL Make a new fixed window for the residents medicine
    GtkWidget *residentMedicine = gtk_fixed_new();
    gtk_widget_set_size_request(residentMedicine, RIGHT_PANEL_WIDTH, RIGHT_PANEL_HEIGHT);
    gtk_fixed_put(GTK_FIXED(residentMedicineDosagePage), residentMedicine, 500, 20);
    // Make the window scrollable
    GtkWidget *residentMedicineScrollableWindow = gtk_scrolled_window_new(NULL, NULL);
    gtk_widget_set_size_request(residentMedicineScrollableWindow, RIGHT_PANEL_WIDTH, RIGHT_PANEL_HEIGHT);
    gtk_fixed_put(GTK_FIXED(residentMedicine), residentMedicineScrollableWindow, 0, 0);
    // Make background on the fixed window
    GtkWidget *fixedBackgroundMedicine = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_size_request(fixedBackgroundMedicine, 60, RIGHT_PANEL_HEIGHT);
    gtk_container_add(GTK_CONTAINER(residentMedicineScrollableWindow), fixedBackgroundMedicine);
    // Add label to fixed window
    gtk_widget_set_name(fixedBackgroundMedicine, "resident-profile-right");
    // Add Header to Right Panel
    GtkWidget *header_right = RIGHT_PANEL_HEADER();
    gtk_container_add(GTK_CONTAINER(fixedBackgroundMedicine), header_right);
    // Add "Start Scan" button to Right Panel
    GtkWidget *button_start = BUTTON("Start Scan", "../src/assets/icons/icon_active_scanning.png", "button_start_scan");
    START_MEDICINE_DISPENSING_BUTTON = button_start;
    g_signal_connect(button_start, "clicked", G_CALLBACK(Button_Start_Medicine_Dispensing), NULL);
    gtk_container_add(GTK_CONTAINER(fixedBackgroundMedicine), button_start);
    // Add Medication Table Headers
    GtkWidget *medication_table = MEDICATION_TABLE();
    gtk_container_add(GTK_CONTAINER(fixedBackgroundMedicine), medication_table);
    // Add SeparatorComponent
    GtkWidget *separator_table_2 = SeparatorComponent(150, 5, "resident-profile-separator-light");
    gtk_container_add(GTK_CONTAINER(fixedBackgroundMedicine), separator_table_2);
    // Add Table Content
    GtkWidget *medication_table_content = MEDICATION_TABLE_CONTENT();
    gtk_container_add(GTK_CONTAINER(fixedBackgroundMedicine), medication_table_content);
    // Add Medication Box
    GtkWidget *MedicationBox = MEDICATION_BOX();
    gtk_widget_set_name(MedicationBox, "medication_grid");
    gtk_container_add(GTK_CONTAINER(fixedBackgroundMedicine), MedicationBox);

    gtk_widget_show_all(residentMedicineDosagePage);
    gtk_widget_set_visible(residentMedicineDosagePage, TRUE);
    gtk_stack_add_titled(GTK_STACK(stack), residentMedicineDosagePage, "Resident Medicine Dosage Page", "Resident Medicine Dosage Page");
    if(gtk_stack_get_child_by_name(GTK_STACK(stack), "Resident Medicine Dosage Page") != NULL) {
        gtk_stack_set_visible_child_name(GTK_STACK(stack), "Resident Medicine Dosage Page");
    }
    GtkWidget *MAIN_WINDOW = gtk_widget_get_toplevel(residentMedicineDosagePage);
    CheckIfScannedCorrectlyInDatabase();
    return residentMedicineDosagePage;
}
