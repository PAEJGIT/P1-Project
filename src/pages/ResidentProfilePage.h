// =====================================================================================================================
// Resident_medicine_dosage_page Header file
// =====================================================================================================================

#ifndef P1_PROGRAM_RESIDENT_PROFILE_PAGE_H
#define P1_PROGRAM_RESIDENT_PROFILE_PAGE_H
#include <gtk/gtkwidget.h>

GtkWidget *InitializeResidentMedicineDosagePage(GtkWidget *stack, gint width, gint height, char *dbEntry);
int CenterObjectsWithEqualWidthOnPage(int windowWidth, int objectAmount, int objectWidth, int numberOfTheObjectWhichXPosUWant);
void UpdateMedicineRowInformation(int integer, char *status, char *action);
void UpdateMedicineRowInformationStorage();
void UpdateMedicineRowInformationQuantity();
void UpdateQuantityBoxes(char *quantity);
int ReturnIntegerOfFirstNonCorrectScanned();
void CorrectlyScanned(int integer);
void MakeWarningWindow(char *text);

extern GtkWidget *TEST_CASE_WINDOW;

#endif //P1_PROGRAM_RESIDENT_PROFILE_PAGE_H
// =====================================================================================================================