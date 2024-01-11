#include "helpers.h"


typedef struct {
    char *name;
    int amount;
} GatherMedicineForStorage;

// Initialize functions
GtkWidget** SCROLLABLE_WINDOWS_ARRAY;
int SIZET = 0;

void LoadCSS () {
    const gchar *CSS_PATH = "../src/styles/style.css";
    GFile *css_fp = g_file_new_for_path(CSS_PATH);
    GError *error = 0;

    GtkCssProvider *provider = gtk_css_provider_new();
    GdkDisplay  *display = gdk_display_get_default();
    GdkScreen *screen = gdk_display_get_default_screen((display));

    // Add the provider to the default screen rather than the currently focused window
    gtk_style_context_add_provider_for_screen(screen, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

    // Load the CSS file
    gtk_css_provider_load_from_file(provider, css_fp, &error);
    if (error) {
        Log(__FUNCTION__, "ERROR", "Loading CSS File");
        g_error_free(error);
    }
    g_object_unref(provider);
}
void ApplyCSSWithID(GtkWidget *widget, char *name, char *cssText) {
    gchar *data = g_strdup_printf("#%s %s", name, cssText);
    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(provider, data,-1, NULL);
    gtk_style_context_add_provider(gtk_widget_get_style_context(widget), GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
    gtk_widget_set_name(widget, name);
    g_object_unref(provider);
    g_free(data);
}
void ApplyCSSOnWidget(GtkWidget *widget, char *widgetName, char *cssText) {
    gchar *data = g_strdup_printf("%s %s", widgetName, cssText);
    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(provider, data,-1, NULL);
    gtk_style_context_add_provider(gtk_widget_get_style_context(widget), GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
    g_object_unref(provider);
    g_free(data);
}
int CenterObjectsWithEqualWidthOnPage(int windowWidth, int objectAmount, int objectWidth, int numberOfTheObjectWhichXPosUWant) {
    int remainder = windowWidth - (objectWidth * objectAmount);
    int spaceBetweenObjects = remainder / (objectAmount + 1);
    int xPosOutput = spaceBetweenObjects * numberOfTheObjectWhichXPosUWant + objectWidth * (numberOfTheObjectWhichXPosUWant-1);
    return xPosOutput;
}
char *GetContentOfMedication(char *name_of_medication){
    // Get Database of Storage
    char*DATABASE_STORAGE = ReadJSON("medicine.json");
    int DATABASE_STORAGE_LENGTH = GetJSONArrayLength(DATABASE_STORAGE);
    for (int i = 0; i < DATABASE_STORAGE_LENGTH; i++) {
        // Get medication name of [i]
        char *medicationName = GetJSONArrayItem(GetJSONArrayItem(DATABASE_STORAGE, i, FALSE), 0, TRUE);
        // Compare needed medication name with name from @name_of_medication
        int result = strcmp(MakeFirstLetterLowercase(medicationName), MakeFirstLetterLowercase(name_of_medication));
        // If match is found, return
        if(result == 0) {
            char *medicationQuantity = GetJSONArrayItem(GetJSONArrayItem(DATABASE_STORAGE, i, FALSE), 2, TRUE);
            return medicationQuantity;
        }
    }
    // If no match is found, return 0
    return "0";
}
char *CombineStringArray(char *arr, bool makeUpperCase) {
    // Get Length of array
    int _arr_length = GetJSONArrayLength(arr);
    // Check if empty
    if(strcmp(arr, "[]") == 0) {
        return "None";
    }
    // Calculate approximate string length
    int _estimated_length = 0;
    for (int i = 0; i < _arr_length; i++) {
        char *item = GetJSONArrayItem(arr, i, TRUE);
        int _item_length = strlen(item);
        _estimated_length += _item_length;
    }
    // Add extra space for null terminator
    _estimated_length += 1;
    // Allocate memory with extra space for null terminator
    char *_string = malloc(_estimated_length);
    memset(_string, 0, _estimated_length);
    // Loop through each item in array
    for (int i = 0; i < _arr_length; i++){
        // Assign array item to string
        char *item = GetJSONArrayItem(arr, i, TRUE);
        if (makeUpperCase) {
            item = MakeFirstLetterUppercase(item);
        }
        // Add ", " after first item
        if(i!=0 && i != _arr_length){
            strcat(_string, "\n");
        }
        // Combine strings
        strcat(_string, item);
    }
    return _string;
}
int *getScreenDimensions() {
    // Get width and height
    int windowWidth = GetSystemMetrics(SM_CYSCREEN);
    int windowHeight = GetSystemMetrics(SM_CXSCREEN);
    // Allocate memory for the coordinates array
    int *coordinates = malloc(2 * sizeof(int));
    // Assign
    *coordinates = windowWidth;
    coordinates[1] = windowHeight;
    // Return the coordinates array
    return coordinates;
}
/**
 * Read JSON Database file
 * @param chooseDatabase —choose between "residents.json" or "medicine.json".
 * @return string[]
 */
char *ReadJSON(char* chooseDatabase) {
    const char* filename;

    if(strcmp(chooseDatabase, "residents.json") == 0) {
        filename = "../src/data/residents.json";
    } else if(strcmp(chooseDatabase, "medicine.json") == 0) {
        filename = "../src/data/medicine.json";
    } else {
        perror("The chosen database can't be found");
    }

    FILE* file = fopen(filename, "r");
    if (file == NULL) return NULL;

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);

    char* content = (char*)malloc(length + 1);
    fread(content, 1, length, file);
    fclose(file);

    content[length] = '\0';
    cJSON* json = cJSON_Parse(content);
    free(content);
    return cJSON_Print(json);
}
char* UpdateStorageValue(const char *filename, const char *dsEntry, const char *nameOfFieldToChange, int intModificationValue, char *stringModificationValue, char *medicineNameInMedicineArray, int indexInArray, bool isArray) {
    // Read the entire file into a string
    FILE *file;
    char *filePath;
    bool changeIsInt = false;
    if(strcmp(filename, "residents.json") == 0) {
        filePath = "../src/data/residents.json";
        file = fopen(filePath, "r");
    } else if(strcmp(filename, "medicine.json") == 0) {
        filePath = "../src/data/medicine.json";
        file = fopen(filePath, "r");
    }
    if(intModificationValue != 0) {
        changeIsInt = true;
    }


    if (file == NULL) {
        Log(__FUNCTION__, "ERROR", "Opening file");
        return NULL;
    }
// function int fseek(FILE *stream, long int offset, int whence) sets the file position of the stream to the given position.

    //SEEK_END = ending of file
    fseek(file, 0, SEEK_END);
    //returns the current file position of the given stream.
    long int fileSize = ftell(file);
    //SEEK_SET = Beginning of file
    fseek(file, 0, SEEK_SET);

    char *jsonString = (char *)malloc(fileSize + 1);
    if (jsonString == NULL) {
        Log(__FUNCTION__, "ERROR", "Memory Allocation");
        fclose(file);
        return 0;
    }

    //reads data from the given stream into the array pointed to, by ptr.
    fread(jsonString, 1, fileSize, file);
    jsonString[fileSize] = '\0';

    fclose(file);

    // Analyze the JSON string
    cJSON *json = cJSON_Parse(jsonString);
    if (json == NULL) {
        Log(__FUNCTION__, "ERROR", "Parsing JSON");
        free(jsonString);
        return NULL;
    }

    // Loop through each medicine
    int arraySize = GetJSONArrayLength(jsonString);
    for (int i = 0; i < arraySize; ++i) {
        // Use the original GetDStry function
        char* originalDataBaseEntry = GetJSONArrayItem(jsonString, i, false);
        // Use the original GetJSONItem function to get the name in each medicine structure
        cJSON *dsEntryJSON = cJSON_Parse(dsEntry);
        char* originalDataBaseEntryName;
        if(strcmp(dsEntryJSON->child->string, "full_name") == 0) {
            originalDataBaseEntryName = GetJSONItem(originalDataBaseEntry, "full_name");
        } else if(strcmp(dsEntryJSON->child->string, "name") == 0) {
            originalDataBaseEntryName = GetJSONItem(originalDataBaseEntry, "name");
        }
        // Compare the name with the name you want to find
        if (strcmp(originalDataBaseEntryName, dsEntryJSON->child->valuestring) == 0) {
            // Use the original GetJSONItem function to get the quantity
            if(!changeIsInt) {
                if(isArray) {
                    cJSON *mainDatabaseEntryObject = cJSON_GetArrayItem(json, i);
                    cJSON *residentMedicineArray = cJSON_GetObjectItemCaseSensitive(mainDatabaseEntryObject, "medicine");
                    for(int l = 0; l < cJSON_GetArraySize(residentMedicineArray); l++) {
                        cJSON *residentMedicineArrayChildArray = cJSON_GetArrayItem(residentMedicineArray, l);
                        cJSON *residentMedicineArrayChildArrayName = cJSON_GetArrayItem(residentMedicineArrayChildArray, 0);
                        MakeFirstLetterUppercase(medicineNameInMedicineArray);
                        if(strcmp(residentMedicineArrayChildArrayName->valuestring, medicineNameInMedicineArray) == 0) {
                            cJSON *residentMedicineArrayChildArrayQuantity = cJSON_GetArrayItem(residentMedicineArrayChildArray, indexInArray);
                            char *test = cJSON_Print(cJSON_CreateString(stringModificationValue));
                            //printf("\n\nTest: %s\n\n", test);
                            cJSON_ReplaceItemInArray(residentMedicineArrayChildArray, indexInArray, cJSON_CreateString(stringModificationValue));
                        }
                    }
                    //cJSON *residentMedicineArrayChildArray = cJSON_GetArrayItem(residentMedicineArray, );
                } else {
                    char *newStringToReplace = stringModificationValue;
                    cJSON *mainDatabaseEntryObject = cJSON_GetArrayItem(json, i);
                    cJSON_ReplaceItemInObjectCaseSensitive(mainDatabaseEntryObject, nameOfFieldToChange, cJSON_CreateString(newStringToReplace));
                }
            } else {
                if(isArray) {
                    cJSON *mainDatabaseEntryObject = cJSON_GetArrayItem(json, i);
                    cJSON *residentMedicineArray = cJSON_GetObjectItemCaseSensitive(mainDatabaseEntryObject, "medicine");
                    for(int l = 0; l < cJSON_GetArraySize(residentMedicineArray); l++) {
                        cJSON *residentMedicineArrayChildArray = cJSON_GetArrayItem(residentMedicineArray, l);
                        cJSON *residentMedicineArrayChildArrayName = cJSON_GetArrayItem(residentMedicineArrayChildArray, 0);
                        if(strcmp(residentMedicineArrayChildArrayName->valuestring, medicineNameInMedicineArray) == 0) {
                            cJSON *residentMedicineArrayChildArrayQuantity = cJSON_GetArrayItem(residentMedicineArrayChildArray, indexInArray);
                            cJSON_ReplaceItemInArray(residentMedicineArrayChildArray, indexInArray, cJSON_CreateNumber(intModificationValue));
                        }
                    }
                    //cJSON *residentMedicineArrayChildArray = cJSON_GetArrayItem(residentMedicineArray, );
                } else {
                    cJSON *mainDatabaseEntryObject = cJSON_GetArrayItem(json, i);
                    cJSON_ReplaceItemInObjectCaseSensitive(mainDatabaseEntryObject, nameOfFieldToChange, cJSON_CreateNumber(intModificationValue));
                }
            }
        }
        free(originalDataBaseEntry);  // Free memory allocated for originalDataBaseEntry
    }

    // Write the modified JSON back to the file
    FILE *fileWrite = fopen(filePath, "w");

    if (file == NULL) {
        Log(__FUNCTION__, "ERROR", "Opening file for writing");
        cJSON_Delete(json);
        free(jsonString);
        return 0;
    }

    char *modifiedJson = cJSON_Print(json);
    fprintf(fileWrite, "%s", modifiedJson);

    fclose(file);
    cJSON_free(modifiedJson);


    return cJSON_Print(json);

    // Clean up
    cJSON_Delete(json);
    free(jsonString);
}
int GetJSONArrayLength(char* array) {
    cJSON *json = cJSON_Parse(array);
    return cJSON_GetArraySize(json);
}
char* GetJSONArrayItem(char* dataBase, int index, bool value) {
    cJSON *json = cJSON_Parse(dataBase);
    cJSON *jsonObject = cJSON_GetArrayItem(json, index);
    if(value) {
        return jsonObject->valuestring;
    }
    return cJSON_Print(jsonObject);
}
int GetDSArrayItemInt(char* dataBase, int index) {
    cJSON *json = cJSON_Parse(dataBase);
    cJSON *jsonObject = cJSON_GetArrayItem(json, index);
    return jsonObject->valueint;
}
char* GetJSONArray(char* entry, char* arrayName) {
    cJSON *json = cJSON_Parse(entry);
    cJSON *arrayString = cJSON_GetObjectItemCaseSensitive(json, arrayName);
    return cJSON_Print(arrayString);
}
char* GetJSONItem(char* entry, char* itemName) {
    cJSON *json = cJSON_Parse(entry);
    cJSON *itemValue = cJSON_GetObjectItemCaseSensitive(json, itemName);
    char* string = itemValue->valuestring;
    return string;
}
int GetJSONItemInt(char* entry, char* itemName) {
    cJSON *json = cJSON_Parse(entry);
    cJSON *itemValue = cJSON_GetObjectItemCaseSensitive(json, itemName);
    int outputValueInt = itemValue->valueint;
    return outputValueInt;
}
char *UpdateProfile(char *newDSString, char *fieldName, char *findableName) {
    cJSON *json = cJSON_Parse(newDSString);
    for(int i = 0; i < GetJSONArrayLength(newDSString); i++) {
        char *tempEntry = GetJSONArrayItem(newDSString, i, false);
        char *tempEntryName = GetJSONItem(tempEntry, fieldName);
        if(strcmp(findableName, tempEntryName) == 0) {
            return tempEntry;
        }
    }
    return "";
}
char *GetQuantityOfMedication(char *name_of_medication, char *residentMedicationArray){
    for(int i = 0; i < GetJSONArrayLength(residentMedicationArray); i++) {
        if(strcmp(name_of_medication, GetJSONArrayItem(GetJSONArrayItem(residentMedicationArray, i, false), 0, true)) == 0) {
            char *medicineQuantity = GetJSONArrayItem(GetJSONArrayItem(residentMedicationArray, i, false), 2, true);
            return medicineQuantity;
        }
    }
    return "0";
}
void UpdateStorageBasedOnAmount() {
    char *medicineDS = ReadJSON("medicine.json");
    char *residentDS = ReadJSON("residents.json");
    GatherMedicineForStorage *gatherMedicineForStorageArray = malloc(
            GetJSONArrayLength(medicineDS) * sizeof(GatherMedicineForStorage));
    int gatherMedicineForStorageArraySize = 0;
    for(int i = 0; i < GetJSONArrayLength(medicineDS); i++) {
        char *tempDSEntry = GetJSONArrayItem(medicineDS, i, false);
        GatherMedicineForStorage gatherMedicineForStorage;
        gatherMedicineForStorage.name = GetJSONItem(tempDSEntry, "name");
        gatherMedicineForStorage.amount = 0;
        gatherMedicineForStorageArray[i] = gatherMedicineForStorage;
        gatherMedicineForStorageArraySize++;
    }
    for(int l = 0; l < GetJSONArrayLength(residentDS); l++) {
        char *tempDSEntry = GetJSONArrayItem(residentDS, l, false);
        char *medicineArray = GetJSONArray(tempDSEntry, "medicine");
        for(int k = 0; k < GetJSONArrayLength(medicineArray); k++) {
            char *medicineName = GetJSONArrayItem(GetJSONArrayItem(medicineArray, k, false), 0, true);
            int medicineAmount = ConvertStringToInt(GetJSONArrayItem(GetJSONArrayItem(medicineArray, k, false), 2, true));
            for(int p = 0; p < gatherMedicineForStorageArraySize; p++) {
                if(strcmp(medicineName, gatherMedicineForStorageArray[p].name) == 0) {
                    gatherMedicineForStorageArray[p].amount += medicineAmount;
                }
            }
        }
    }
    for(int j = 0; j < gatherMedicineForStorageArraySize; j++) {
        for(int h = 0; h < GetJSONArrayLength(medicineDS); h++) {
            char *tempDSEntry = GetJSONArrayItem(medicineDS, h, false);
            if(strcmp(gatherMedicineForStorageArray[j].name, GetJSONItem(tempDSEntry, "name")) == 0) {
                char str[12];
                int amount = gatherMedicineForStorageArray[j].amount;
                snprintf(str, sizeof(str), "%i", amount);
                UpdateStorageValue("medicine.json", tempDSEntry, "quantity", 0, str, "", 0, false);
            }
        }
    }
    Log(__FUNCTION__, "DATA", "Updated JSON");
}
void AddScrollableWindowToArray(GtkWidget *scrollableWindow) {
    SIZET++;
    if(SIZET == 1) {
        SCROLLABLE_WINDOWS_ARRAY = (GtkWidget**)malloc(SIZET * sizeof(GtkWidget*));
    } else {
        SCROLLABLE_WINDOWS_ARRAY = (GtkWidget**)realloc(SCROLLABLE_WINDOWS_ARRAY, SIZET * sizeof(GtkWidget*));
    }
    SCROLLABLE_WINDOWS_ARRAY[SIZET - 1] = scrollableWindow;
}
void On_Visible_Child_Changed(GtkWidget *widget, GParamSpec *pspec, gpointer data) {
    for(int i = 0; i < SIZET; i++) {
        GtkAdjustment *vAdjustment = gtk_scrolled_window_get_vadjustment(GTK_SCROLLED_WINDOW(SCROLLABLE_WINDOWS_ARRAY[i]));
        gtk_adjustment_set_value(vAdjustment, 0);
        gtk_widget_queue_draw(GTK_WIDGET(SCROLLABLE_WINDOWS_ARRAY[i]));
    }
    GtkWidget *residentMedicineDosagePage = gtk_stack_get_child_by_name(data, "Resident Medicine Dosage Page");
    // Make sure the dosage page exists
    if (residentMedicineDosagePage != NULL) {
        // Get the widget's name
        if(gtk_stack_get_visible_child(data) != residentMedicineDosagePage) {
            gtk_container_remove(data, residentMedicineDosagePage);
        }
    } else {
        // Do something if the widget is not found
        ////printf("On_Visible_Child_Changed: Could not find the dosage page widget.\n");
    }
}
/**
 * Delay time in milliseconds
 * @param milliseconds
 */
void AddTimeDelay(int milliseconds) {
    usleep(milliseconds * 1000); // Convert milliseconds to microseconds
}
int ConvertStringToInt(char *string) {
    if (!string || *string == '\0') {
        return 0;
    }
    int len = strlen(string) + 1; // Add 1 for the null terminator
    char *myString = malloc(len);
    strcpy(myString, string);
    myString[strlen(string)] = '\0'; // Add the null terminator
    return atoi(string);
}
char *MakeFirstLetterLowercase(char *str) {
    int firstLetter = (int)str[0];
    if (firstLetter >= 65 && firstLetter <= 90) {
        firstLetter += 32;
        str[0] = (char)firstLetter;
    }
    return str;
}
char *MakeFirstLetterUppercase(char *str) {
    if (str[0] >= 97 && str[0] <= 122) {
        str[0] -= 32;
    }
    return str;
}
/**
 * \n
 * Random Number Generator
 * \n
 * Get a random number between MIN and MAX
 * @param MIN —minimum value
 * @param MAX —maximum value
 * @example GetRandomNumber(int 1, int 10) > 6
 * @return integer [MIN \< MAX]
 */
int GetRandomNumber(int MIN, int MAX) {
    int randomNumber = rand() % MAX + MIN; // Generate a random integer between 0 and 10
    return randomNumber;
}
int RoundUpToNearest(int number, int roundTo) {
    if (number % roundTo == 0) {
        return number;
    } else {
        return number + (roundTo - (number % roundTo));
    }
}
/**
 * \n
 * This function calculates the number of dosages required to cover the prescribed dosage.
 * \n
 * @param dosageRequiredStr String for total dosage required
 * @param dosageEachStr String for dosage for each tablet
 * @return A string containing the calculated number of dosages
 */
char *CalculateDosageEach(const char* dosageRequiredStr, const char* dosageEachStr) {
    // Extract the dosages as integers
    int dosageRequired = atof(dosageRequiredStr);
    int dosageEach = atof(dosageEachStr);
    // Calculate the number of dosages required
    int numberOfDosages = dosageRequired / dosageEach;
    // Convert the number of dosages to a string and return it
    char *quantity = g_strdup_printf("%d", numberOfDosages);
    return quantity;
}
/**
 *
 * @param integer
 * @return
 */
char *ConvertIntToString(int integer) {
    return g_strdup_printf("%d", integer);
}
/**
 * Returns a copy of the supplied string
 * @param string
 * @return string
 */
char *MakeStringCopy(char *string) {
    char *copy = malloc(strlen(string) + 1); // Allocate memory for the copy
    strcpy(copy, string); // Copy the original string to the copy
    return copy; // Return the copy
}
/**
 *
 * @return
 */
char *GetCurrentDate() {
    time_t currentTime;
    struct tm *timeInfo;
    char *currentDate = NULL;

    // Get time
    time(&currentTime);
    timeInfo = localtime(&currentTime);

    // Allocate memory for a new string
    char formattedDate[20];

    // Format date
    strftime(formattedDate, sizeof(formattedDate), "%d-%m-%Y", timeInfo);

    // Create a new string to return
    char *returnDate = (char *)malloc(sizeof(char) * 20);

    // Copy the formatted date into the new string
    strcpy(returnDate, formattedDate);

    // Return the new string
    return returnDate;
}
/**
 * Logs a message with the current time, function name, and log type
 *
 * @param currentFunction The name of the current function
 * @param type The type of log (e.g., INFO, WARNING, ERROR)
 * @param message The log message
 * @example
    Log(__FUNCTION__, "INFO", "This is an informational log message.");\n
    Log(__FUNCTION__, "WARNING", "This is a warning log message.");\n
    Log(__FUNCTION__, "ERROR", "This is an error log message.");\n
 */
void Log(const char* function, const char* type, const char* message) {
    time_t currentTime;
    struct tm *timeInfo;
    time(&currentTime);
    timeInfo = localtime(&currentTime);
    char timeStr[80];
    strftime(timeStr, sizeof(timeStr), "%H:%M:%S", timeInfo);
    printf("[%s] [%s]\n%s: %s\n\n", timeStr, function, type, message);
}