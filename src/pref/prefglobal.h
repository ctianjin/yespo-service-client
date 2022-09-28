#ifndef PREFGLOBAL_H
#define PREFGLOBAL_H

#include <QtCore/QtGlobal>

#ifdef BUILDSYS_SHARED_LIBS
#  ifdef BUILDSYS_PREF_SOURCE
#    define BUILDSYS_PREF_DECL Q_DECL_EXPORT
#  else
#    define BUILDSYS_PREF_DECL Q_DECL_IMPORT
#  endif // BUILDSYS_PREF_SOURCE
#endif // BUILDSYS_SHARED_LIBS

#ifndef BUILDSYS_PREF_DECL
#  define BUILDSYS_PREF_DECL
#endif

#endif // PREFGLOBAL_H