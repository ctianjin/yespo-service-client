#ifndef UIGLOBAL_H
#define UIGLOBAL_H

#include <QtCore/QtGlobal>

#ifdef BUILDSYS_SHARED_LIBS
#  ifdef BUILDSYS_UI_SOURCE
#    define BUILDSYS_UI_DECL Q_DECL_EXPORT
#  else
#    define BUILDSYS_UI_DECL Q_DECL_IMPORT
#  endif // BUILDSYS_UI_SOURCE
#endif // BUILDSYS_SHARED_LIBS

#ifndef BUILDSYS_UI_DECL
#  define BUILDSYS_UI_DECL
#endif

#endif // UIGLOBAL_H
