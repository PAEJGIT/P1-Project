//
// Created by danie on 04-12-2023.
//

#include "gtk/gtk.h"

#ifndef P1_PROGRAM_RESIDENT_LIST_PAGE_H
#define P1_PROGRAM_RESIDENT_LIST_PAGE_H

GtkWidget *InitializeResidentList(GtkWidget *stack, gint width, gint height);
typedef struct {
    char *dsEntry;
    GtkWidget *stack;
} ResidentListButtonClickedData;
void UpdateAllButtonDSEntriesToNewDSEntry();

#endif //P1_PROGRAM_RESIDENT_LIST_PAGE_H
