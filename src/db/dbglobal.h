#ifndef DBGLOBAL_H
#define DBGLOBAL_H

#include <QtCore/QtGlobal>

#ifdef BUILDSYS_SHARED_LIBS
#  ifdef BUILDSYS_DB_SOURCE
#    define BUILDSYS_DB_DECL Q_DECL_EXPORT
#  else
#    define BUILDSYS_DB_DECL Q_DECL_IMPORT
#  endif // BUILDSYS_DB_SOURCE
#endif // BUILDSYS_SHARED_LIBS

#ifndef BUILDSYS_DB_DECL
#  define BUILDSYS_DB_DECL
#endif

#endif // DBGLOBAL_H
