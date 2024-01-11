#include <stdio.h>
#include <stdlib.h>
#include "../Internal_Libraries/cJSON.h"
#include "string.h"
#include "stdbool.h"
#include "../src/utils/helpers.h"

// Function to modify a JSON value based on a signed int in a file
char* modifyJsonValueInFile(const char *filename, const char *nameToFind, int modificationValue) {
    // Read the entire file into a string
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening file: %s\n", filename);
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
        printf("Memory allocation error\n");
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
        printf("Error parsing JSON\n");
        free(jsonString);
        return NULL;
    }

    // Loop through each medicine
    int arraySize = GetDBArrayLength(jsonString);
    for (int i = 0; i < arraySize; ++i) {
        // Use the original GetDBEntry function
        char* medicineEntry = GetDBArrayItem(jsonString, i, false);
        // Use the original GetDBItem function to get the name in each medicine structure
        char* medicineName = GetDBItem(medicineEntry, "name");

        // Compare the name with the name you want to find
        if (strcmp(medicineName, nameToFind) == 0) {
            // Use the original GetDBItem function to get the quantity
            char* quantityChange = GetDBItem(medicineEntry, "quantity");

            char *ptr;
            long quantityNum = strtol(quantityChange, &ptr, 10);

            // Update the quantity
            quantityNum += modificationValue;

            // Allocate space for null terminator
            char *newstringQuantity = malloc((strlen(quantityChange) + 1) * sizeof(char));

            sprintf(newstringQuantity, "%ld", quantityNum);

            // Directly modify the quantity within the existing cJSON object
            cJSON *medicineObject = cJSON_GetArrayItem(json, i);
            cJSON_ReplaceItemInObjectCaseSensitive(medicineObject, "quantity", cJSON_CreateString(newstringQuantity));
        }
        free(medicineEntry);  // Free memory allocated for medicineEntry
        free(medicineName);   // Free memory allocated for medicineName
    }




    // Write the modified JSON back to the file
            file = fopen(filename, "w");
            if (file == NULL) {
                printf("Error opening file for writing: %s\n", filename);
                cJSON_Delete(json);
                free(jsonString);
                return 0;
            }

            char *modifiedJson = cJSON_Print(json);
            fprintf(file, "%s", modifiedJson);

            fclose(file);
            cJSON_free(modifiedJson);


        return cJSON_Print(json);

    // Clean up
    cJSON_Delete(json);
    free(jsonString);
}

void UpdateStorage() {
    // Example JSON file name
    const char *json =  "../src/data/medicine.json";

    // Modify the value of "key1" in the file based on a signed int
    // a function that need to get passed medicin name - and change the d
    char *mod = modifyJsonValueInFile(json, "Paracetamol", 10);
    printf("Modified JSON:\n%s\n", mod);


    // Debugging: Print the modified JSON file
    /*FILE *debugFile = fopen(json, "r");
    if (debugFile != NULL) {
        fseek(debugFile, 0, SEEK_END);
        long int debugFileSize = ftell(debugFile);
        fseek(debugFile, 0, SEEK_SET);

        char *debugJsonString = (char *)malloc(debugFileSize + 1);
        if (debugJsonString != NULL) {
            fread(debugJsonString, 1, debugFileSize, debugFile);
            debugJsonString[debugFileSize] = '\0';
            fclose(debugFile);

            printf("Debugging: Modified JSON:\n%s\n", debugJsonString);
            free(debugJsonString);
        } else {
            fclose(debugFile);
        }
    }*/

}
