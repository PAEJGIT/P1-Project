//
// Created by main on 10/12/2023.
//

#include <gtk/gtk.h>
#include "stdbool.h"
#include "../pages/ResidentListPage.h"
#include "../pages/OrderPage.h"

#ifndef P1_PROGRAM_LISTELEMENTS_H
#define P1_PROGRAM_LISTELEMENTS_H

// New List Functions
GtkWidget *NewListBoxHeaders(GtkWidget *container, bool IsFixed, int width, int height, int xPos, int yPos, char *cssText);
GtkWidget *NewListBox(GtkContainer *container, int width, int height, char *cssText);
GtkWidget *NewListRow(GtkBox *listBox, int height);
GtkWidget *NewListTextColumn(GtkBox *listRow, int height, char *string, bool isHeader, char *cssText);
GtkWidget *NewListButtonColumn(GtkBox *listRow, int width, int height, char *string, char *cssText, ResidentListButtonClickedData *residentListButtonClickedData, OrderListButtonData *orderListButtonData);

#endif //P1_PROGRAM_LISTELEMENTS_H
