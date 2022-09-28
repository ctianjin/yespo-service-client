#ifndef IMGLOBAL_H
#define IMGLOBAL_H

#include <QtCore/QtGlobal>

#ifdef BUILDSYS_SHARED_LIBS
#  ifdef BUILDSYS_IM_SOURCE
#    define BUILDSYS_IM_DECL Q_DECL_EXPORT
#  else
#    define BUILDSYS_IM_DECL Q_DECL_IMPORT
#  endif // BUILDSYS_IM_DECL
#endif // BUILDSYS_SHARED_LIBS

#ifndef BUILDSYS_IM_DECL
#  define BUILDSYS_IM_DECL
#endif

#endif // IMGLOBAL_H