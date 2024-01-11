//
// Created by danie on 10-12-2023.
//

#ifndef P1_PROGRAM_CHECKS_H
#define P1_PROGRAM_CHECKS_H

typedef struct {
    char *medicineName;
    char *medicineDosage;
    char *medicineExpiryDate;
    char *allResidentMedicine;
    char *residentDSEntry;
    int amountToGive;
} CurrentlyScanning;

extern CurrentlyScanning currentlyScanning;

void RunAllChecks(char *medicineDBEntry);

#endif //P1_PROGRAM_CHECKS_H
