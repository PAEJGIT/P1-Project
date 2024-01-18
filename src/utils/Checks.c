#include "Checks.h"
#include "stdbool.h"
#include "ctype.h"
#include "string.h"
#include "../utils/helpers.h"
#include "../utils/handlers.h"
#include "../pages/ResidentProfilePage.h"
#include "../../.packages/cJSON.h"
#include "../pages/StoragePage.h"
#include "../pages/OrderPage.h"

CurrentlyScanning currentlyScanning;
int currentlyScanningMedicineStorage = 0;

/**
 * \n
 * This function checks if the name of a scanned medication matches any of the medications in the database entry.
 * \n
 * @param medicineName The name of the medication retrieved from the prescription
 * @param medicineDSEntry The database entry containing the medication's information from barcode
 * @return TRUE if a matching medication is found, FALSE otherwise
 */
bool CheckNameOfMedication(char *medicineName, char *medicineDSEntry) {
    // Assign
    char *barcodeName = MakeFirstLetterLowercase(GetJSONItem(medicineDSEntry, "name"));
    char *prescriptionName = MakeFirstLetterLowercase(medicineName);

    // Compare name of barcodeName with name of medication in prescription
    if(strcmp(barcodeName, prescriptionName) == 0) {
        Log(__FUNCTION__, "CHECK", g_strdup_printf("Found match with name (%s) in prescription.", prescriptionName));
        return true;
    }
    Log(__FUNCTION__, "CHECK", g_strdup_printf("Did not find match with name (%s) in prescription.", barcodeName));
    return false;
}
/**
 * \n
 * This function checks if the dosage of the scanned medication matches the prescribed dosage.
 * \n
 * @param prescriptionDosage The prescribed dosage of the medication
 * @param medicineDSEntry The database entry containing the medication's information
 * @return TRUE if the dosages match or can be adjusted to match, FALSE otherwise
 */
bool CheckDosageOfMedication(char *medicationDosage, char *medicineDSEntry) {
    // Retrieve the dosage from prescription
    char *barcodeDosage = GetJSONItem(medicineDSEntry, "dosage");
    char *prescriptionDosage = MakeStringCopy(medicationDosage);
    // Remove the units from the dosages
    prescriptionDosage[strlen(prescriptionDosage)-2] = '\0';  // fjerner mg
    barcodeDosage[strlen(barcodeDosage)-2] = '\0';   // fjerner mg
    // Convert dosages to integers
    int prescriptionDosageInt = ConvertStringToInt(prescriptionDosage);
    int barcodeDosageInt = ConvertStringToInt(barcodeDosage);
    // Check if dosage from barcode is higher than prescription allows
    if(barcodeDosageInt > prescriptionDosageInt) {
        currentlyScanning.amountToGive = 0;
        Log(__FUNCTION__, "CHECK", g_strdup_printf("The dosage of (%s mg) at (%d mg) is higher than prescribed. Expected %d mg.", barcodeDosage, barcodeDosageInt, prescriptionDosageInt));
        return false;
    }
    // Determine whether the dosage from barcode matches or can be adjusted to match the prescription
    else if(barcodeDosageInt == prescriptionDosageInt) {
        currentlyScanning.amountToGive = 1;
        Log(__FUNCTION__, "CHECK", g_strdup_printf("The dosage of (%s) matches the prescription.", barcodeDosage));
        return true;
    } else {
        char *dosageRequired = g_strdup_printf("%d\\mg", prescriptionDosageInt);
        char *dosageEach = g_strdup_printf("%d\\mg", barcodeDosageInt);
        char *totalTabletsRequired = CalculateDosageEach(dosageRequired, dosageEach);
        currentlyScanning.amountToGive = ConvertStringToInt(totalTabletsRequired);
        Log(__FUNCTION__, "CHECK", g_strdup_printf("The dosage of (%s mg) is lower than prescribed (%s mg). Total tablets required to give %s.", barcodeDosage, prescriptionDosage, totalTabletsRequired));
        return true;
    }
}
/**
 * \n
 * This function checks the expiry date of a scanned medication against the current date(today).
 * \n
 * @param medicineDSEntry The database entry containing the medication's information from barcode
 * @return FALSE if the medication has expired, TRUE otherwise
 */
bool CheckExpiryOfMedication(char *medicineDSEntry) {
    // Get name of medication
    char *barcodeName = GetJSONArrayItem(medicineDSEntry, 0, TRUE);
    // Get expiry date of medication
    char *barcodeExpiryDate = GetJSONArrayItem(medicineDSEntry, 4, TRUE);
    // Get medication entry from data storage (barcode)
    char *barcodeExpiry = GetJSONArrayItem(medicineDSEntry, 4, TRUE);
    barcodeExpiry[strlen(barcodeExpiry)-8] = '\0'; // kun dage
    int barcodeExpiryInt = ConvertStringToInt(barcodeExpiry);

    // Get date (today)
    char *dateToday = GetCurrentDate();
    dateToday[strlen(dateToday)-8] = '\0';
    int dateTodayInt = ConvertStringToInt(dateToday);

    // Difference in day(s)
    int difference;
    // Return conditions
    if (dateTodayInt == barcodeExpiryInt){
        Log(__FUNCTION__, "CHECK", g_strdup_printf("The medication (%s) scanned (%s) expires today.", barcodeName, barcodeExpiryDate));
        return true;
    }
    else if (dateTodayInt < barcodeExpiryInt){
        difference = barcodeExpiryInt-dateTodayInt;
        Log(__FUNCTION__, "CHECK", g_strdup_printf("The medication (%s) scanned (%s) expires in %d days.", barcodeName,  barcodeExpiryDate, difference));
        return true;
    }
    else {
        difference = dateTodayInt-barcodeExpiryInt;
        Log(__FUNCTION__, "CHECK", g_strdup_printf("The scanned (%s) medicine (%s) expired (%d) days ago.", barcodeName, barcodeExpiryDate, difference));
        return false;
    }
}
/**
 * This function checks the current stock of the scanned medication to ensure there is enough for the prescribed dosage.
 *
 * @return An integer indicating the status of the stock:
    0 - Insufficient for today's usage
    1 - Insufficient for 14 days usage
    2 - Sufficient for 14 days usage
 */
int CheckStockOfMedication() {
    int amountOfPillsToGive = currentlyScanning.amountToGive;
    int amountOfPillsToGiveOverTwoWeeks = amountOfPillsToGive*14;
    char *nameInMedicineArray;
    // TODO: Fix double loop
    // Iterate through the resident's medication list
    for(int i = 0; i < GetJSONArrayLength(currentlyScanning.allResidentMedicine); i++) {
        // Extract the name of the medication from the database entry
        nameInMedicineArray = MakeFirstLetterLowercase(
                GetJSONArrayItem(GetJSONArrayItem(currentlyScanning.allResidentMedicine, i, false), 0, true));
        // Compare the scanned medicine name with the name from the database
        if(strcmp(currentlyScanning.medicineName, nameInMedicineArray) == 0) {
            // Retrieve the current stock of the scanned medication
            currentlyScanningMedicineStorage = ConvertStringToInt(
                    GetJSONArrayItem(GetJSONArrayItem(currentlyScanning.allResidentMedicine, i, false), 2, true));
            // Check if the current stock is sufficient for today's usage
            if(currentlyScanningMedicineStorage < amountOfPillsToGiveOverTwoWeeks) {
                Log(__FUNCTION__, "CHECK", g_strdup_printf("Currently have %d after dispensing, require an additional %d.", currentlyScanningMedicineStorage-amountOfPillsToGiveOverTwoWeeks, amountOfPillsToGiveOverTwoWeeks-currentlyScanningMedicineStorage));
                return 2;
            }
            if(currentlyScanningMedicineStorage-amountOfPillsToGiveOverTwoWeeks < amountOfPillsToGiveOverTwoWeeks) {
                Log(__FUNCTION__, "CHECK", g_strdup_printf("Currently have %d after dispensing, require an additional %d.", currentlyScanningMedicineStorage-amountOfPillsToGiveOverTwoWeeks, amountOfPillsToGiveOverTwoWeeks-currentlyScanningMedicineStorage));
                return 1;
            }
        }
    }
    return 0;
}
/* Get medicine name of the medicine that is currently being scanned.
 * Get all the medicine the targeted resident is getting.
 * Get the interactions of the currently scanning medicine.
 * Loop through the array of interactions and if they match then add to string array
 * Return the string array when finished.
 */
char** CheckInteractionOfMedication() {
    // Assign
    char *medicineName = MakeFirstLetterLowercase(currentlyScanning.medicineName);
    char *residentMedicineArrayAsString = currentlyScanning.allResidentMedicine;
    char **interactions = NULL;
    int amountOfInteractions = 0;
    char *currentlyScanningInteractions = NULL;
    char *medicineDS = ReadJSON("medicine.json");
    char *medicineDSEntry;

    // Iterate
    for(int i = 0; i < GetJSONArrayLength(medicineDS); i++) {
        medicineDSEntry = GetJSONArrayItem(medicineDS, i, false);
        cJSON *medicineDSEntryCJSON = cJSON_Parse(medicineDSEntry);
        char *valueName = MakeFirstLetterLowercase(medicineDSEntryCJSON->child->valuestring);
        if(strcmp(valueName, medicineName) == 0) {
            currentlyScanningInteractions = GetJSONArray(medicineDSEntry, "interactions");
            break;
        }
    } // hvis ny medicin blev indskannet, og der ingen interaction er fundet
    if(currentlyScanningInteractions == NULL) {
        perror("\ncurrentlyScanningInteractions has not been set, which means that the medicine name doesn't match any in the medicine database");
    }

    // Iterate
    for(int l = 0; l < GetJSONArrayLength(residentMedicineArrayAsString); l++) {
        char *tempMedicineName = MakeFirstLetterLowercase(
                GetJSONArrayItem(GetJSONArrayItem(residentMedicineArrayAsString, l, false), 0, true));
        if(strcmp(tempMedicineName, medicineName) == 1) {
            for(int k = 0; k < GetJSONArrayLength(currentlyScanningInteractions); k++) {
                char *currentlyScanningInteractionsName = GetJSONArrayItem(currentlyScanningInteractions, k, true);
                if(strcmp(currentlyScanningInteractionsName, tempMedicineName) == 0) {
                    amountOfInteractions++;
                    if(interactions == NULL) {
                        interactions = malloc(amountOfInteractions * sizeof(char**));
                    } else {
                        interactions = realloc(interactions, amountOfInteractions * sizeof(char**));
                    }
                    interactions[amountOfInteractions-1] = tempMedicineName;
                }
            }
        }
    }
    return interactions;
}

/**
 * \n
 * This primary function performs a comprehensive set of checks on the scanned medication to ensure it matches the resident's prescription and is safe for administration.
 *
 * @param medicineDSEntry The database entry containing the scanned medication's information
 */
void RunAllChecks(char *medicineDSEntry) {
    // Random (min) and (max) values to calculate delay
    int randomMin = 200;
    int randomMax = 300;

    // Check expiry of scanned medication
    AddTimeDelay(GetRandomNumber(randomMin, randomMax));
    if(!CheckExpiryOfMedication(medicineDSEntry)) {
        UpdateMedicineRowInformation(ReturnIntegerOfFirstNonCorrectScanned(), "Failed", "EXPIRED");
        Log(__FUNCTION__, "CHECK", "Expiry Failed");
        return;
    }

    // Check name of scanned medication
    AddTimeDelay(GetRandomNumber(randomMin, randomMax));
    if(!CheckNameOfMedication(currentlyScanning.medicineName, medicineDSEntry)) {
        UpdateMedicineRowInformation(ReturnIntegerOfFirstNonCorrectScanned(), "Failed", "MISMATCH");
        Log(__FUNCTION__, "CHECK", "Name Failed");
        return;
    }

    // Check dosage of scanned medication
    AddTimeDelay(GetRandomNumber(randomMin, randomMax));
    if(!CheckDosageOfMedication(currentlyScanning.medicineDosage, medicineDSEntry)) {
        UpdateMedicineRowInformation(ReturnIntegerOfFirstNonCorrectScanned(), "Failed", "DOSAGE");
        Log(__FUNCTION__, "CHECK", "Dosage Failed");
        return;
    }

    // Check storage of scanned medication
    AddTimeDelay(GetRandomNumber(randomMin, randomMax));
    int medicineStockCheck = CheckStockOfMedication();
    if(medicineStockCheck > 0) {
        if(!CheckIfMedicineAlreadyOrdered(GetJSONItem(currentlyScanning.residentDSEntry, "full_name"), currentlyScanning.medicineName)) {
            AddOrderToOrderList(currentlyScanning.residentDSEntry,
                                GetJSONItem(currentlyScanning.residentDSEntry, "full_name"),
                                currentlyScanning.medicineName,
                                currentlyScanningMedicineStorage,
                                RoundUpToNearest(currentlyScanning.amountToGive * 14, 100),
                                currentlyScanning.amountToGive * 14
                                );
            gchar *warningText = g_strdup_printf("A shortage for the next dispensing of %s's %s has been found.\nAn order has been added to the list in the Order page.",
                                                 GetJSONItem(currentlyScanning.residentDSEntry, "full_name"), currentlyScanning.medicineName);
            MakeWarningWindow(warningText);
        } else {
            gchar *warningText = g_strdup_printf("A shortage for the next dispensing of %s's %s has been found.\nAn order is already present in the list in the Order page.",
                                                 GetJSONItem(currentlyScanning.residentDSEntry, "full_name"), currentlyScanning.medicineName);
            MakeWarningWindow(warningText);
        }

        if(medicineStockCheck == 1) {
            UpdateMedicineRowInformation(ReturnIntegerOfFirstNonCorrectScanned(), "Warning", "SHORTAGE");
        }
        if(medicineStockCheck == 2) {
            UpdateMedicineRowInformation(ReturnIntegerOfFirstNonCorrectScanned(), "Failed", "SHORTAGE");
            Log(__FUNCTION__, "CHECK", "Stock Failed");
           return;
        }
    }

    // Check for interactions between scanned medication and prescription(s)
    AddTimeDelay(GetRandomNumber(randomMin, randomMax));
    char **medicineInteractions = CheckInteractionOfMedication();
    if(medicineInteractions != NULL) {
        //UpdateMedicineRowInformation(ReturnIntegerOfFirstNonCorrectScanned(), "Failed", "Mismatch");
        int medicineInteractionSize = (int)(sizeof(medicineInteractions) / sizeof(medicineInteractions[0]));
        char inputText[200];
        for(int i = 0; i < medicineInteractionSize; i++) {
            snprintf(inputText, sizeof(inputText), " and %s", medicineInteractions[i]);
        }
        char *WarningTextInteraction = g_strdup_printf("An interaction between %s%s has been found!\nPlease double check that this is intended!", currentlyScanning.medicineName, inputText);
        MakeWarningWindow(WarningTextInteraction);
        Log(__FUNCTION__, "CHECK", "Interaction found. Triggering Warning Window.");
    }

    // Update storage after checks
    char *storageModifyText = g_strdup_printf("%i", currentlyScanningMedicineStorage - currentlyScanning.amountToGive*14);
    char *quantityModifyText = g_strdup_printf("%i", currentlyScanning.amountToGive*14);
    UpdateStorageValue("residents.json", currentlyScanning.residentDSEntry, "", 0, storageModifyText,
                       currentlyScanning.medicineName, 2, true);
    UpdateStorageValue("residents.json", currentlyScanning.residentDSEntry, "", 0, quantityModifyText,
                       currentlyScanning.medicineName, 5, true);
    UpdateStorageBasedOnAmount();
    UpdateQuantityLabelInTable();
    UpdateMedicineRowInformation(ReturnIntegerOfFirstNonCorrectScanned(), "Success", "Match");
    UpdateMedicineRowInformationStorage();
    UpdateMedicineRowInformationQuantity(g_strdup_printf("%i", currentlyScanning.amountToGive*14));
    char *quantityToUpdate = g_strdup_printf("%i", currentlyScanning.amountToGive*14);
    UpdateQuantityBoxes(quantityToUpdate);
    CorrectlyScanned(ReturnIntegerOfFirstNonCorrectScanned());
    AddTimeDelay(GetRandomNumber(randomMin, randomMax));
    Log(__FUNCTION__, "CHECK", "All checks succeeded. Clear for dispensing.");
    //MakeWarningWindow("An interaction between x and y has been found!\nPlease double check that this is intended!");
}

