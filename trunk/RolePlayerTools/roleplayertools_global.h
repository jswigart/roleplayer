#ifndef ROLEPLAYERTOOLS_GLOBAL_H
#define ROLEPLAYERTOOLS_GLOBAL_H

#include <QtCore/qglobal.h>

#ifdef ROLEPLAYERTOOLS_LIB
# define ROLEPLAYERTOOLS_EXPORT Q_DECL_EXPORT
#else
# define ROLEPLAYERTOOLS_EXPORT Q_DECL_IMPORT
#endif

#endif // ROLEPLAYERTOOLS_GLOBAL_H
