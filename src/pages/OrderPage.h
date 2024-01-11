// =====================================================================================================================
// Order_page Header file
// =====================================================================================================================
#include <gtk/gtk.h>
#include "stdbool.h"

#ifndef P1_PROGRAM_ORDER_PAGE_H
#define P1_PROGRAM_ORDER_PAGE_H
GtkWidget *InitializeOrderPage(GtkWidget *stack, gint width, gint height);
typedef struct {
    char *dsEntry;
    char *residentName;
    char *medicineName;
    int orderedAmount;
    int currentStock;
    int amountUsed;
} OrderListButtonData;
void AddOrderToOrderList(char *dsEntry, char *residentName, char *medicineName, int currentStock, int amountToOrder, int amountUsed);
bool CheckIfMedicineAlreadyOrdered(char *residentName, char *medicineName);
#endif //P1_PROGRAM_ORDER_PAGE_H
// =====================================================================================================================
