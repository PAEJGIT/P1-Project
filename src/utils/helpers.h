//
// Created by main on 10/12/2023.
//

#ifndef P1_PROGRAM_HELPERS_H
#define P1_PROGRAM_HELPERS_H
#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include "../../.packages/cJSON.h"
#include "helpers.h"
#include "../../src/configs/constants.h"
#include <fltwinerror.h>
#include <windows.h>
extern GtkWidget** SCROLLABLE_WINDOWS_ARRAY;
extern int SIZET;
int ConvertStringToInt(char *string);
char *MakeFirstLetterLowercase(char *str);
char *MakeFirstLetterUppercase(char *str);
int GetRandomNumber(int MIN, int MAX);
int *getScreenDimensions();
char *CombineStringArray(char *arr, bool makeUpperCase);
char *GetContentOfMedication(char *name_of_medication);
int CenterObjectsWithEqualWidthOnPage(int windowWidth, int objectAmount, int objectWidth, int numberOfTheObjectWhichXPosUWant);
void LoadCSS();
void ApplyCSSWithID(GtkWidget *widget, char *name, char *cssText);
void ApplyCSSOnWidget(GtkWidget *widget, char *widgetName, char *cssText);
char *ReadJSON(char* chooseDatabase);
char* UpdateStorageValue(const char *filename, const char *dbEntry, const char *nameOfFieldToChange, int intModificationValue, char *stringModificationValue, char *medicineNameInMedicineArray, int indexInArray, bool isArray);
int GetJSONArrayLength(char* array);
char* GetJSONArrayItem(char* dataBase, int index, bool value);
int GetDBArrayItemInt(char* dataBase, int index);
char* GetJSONArray(char* entry, char* arrayName);
char* GetJSONItem(char* entry, char* itemName);
int GetJSONItemInt(char* entry, char* itemName);
char *UpdateProfile(char *newDBString, char *fieldName, char *findableName);
void UpdateStorageBasedOnAmount();
char *GetQuantityOfMedication(char *name_of_medication, char *residentMedicationArray);
void AddScrollableWindowToArray(GtkWidget *scrollableWindow);
void On_Visible_Child_Changed(GtkWidget *widget, GParamSpec *pspec, gpointer data);
int RoundUpToNearest(int number, int roundTo);
void AddTimeDelay(int milliseconds);
char *MakeStringCopy(char *string);
char *CalculateDosageEach(const char* dosageRequiredStr, const char* dosageEachStr);
char *ConvertIntToString(int integer);
char *GetCurrentDate(void);
void Log(const char* function, const char* type, const char* message);
#endif //P1_PROGRAM_HELPERS_H
