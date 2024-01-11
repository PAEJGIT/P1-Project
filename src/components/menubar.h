// =====================================================================================================================
// Menubar Header file
// =====================================================================================================================
#include <gtk/gtk.h>

#ifndef P1_PROGRAM_MENUBAR_H
#define P1_PROGRAM_MENUBAR_H

GtkWidget *MakeMenuBar(GtkWidget *fixed);

GtkWidget *MakeMenuBarButton(GtkWidget *fixed, gchar *label, gint menuWidth, gint menuHeight, GtkWidget *stack);

#endif //P1_PROGRAM_MENUBAR_H
// =====================================================================================================================