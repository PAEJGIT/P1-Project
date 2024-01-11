// =====================================================================================================================
// Storage_page Header file
// =====================================================================================================================
#include <gtk/gtk.h>
#include "ResidentListPage.h"


#ifndef P1_PROGRAM_STORAGE_PAGE_H
#define P1_PROGRAM_STORAGE_PAGE_H

GtkWidget *InitializeStoragePage(GtkWidget *stack, gint width, gint height);
void UpdateQuantityLabelInTable();

#endif //P1_PROGRAM_STORAGE_PAGE_H
// =====================================================================================================================
