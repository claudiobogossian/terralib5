/*  Copyright (C) 2008 National Institute For Space Research (INPE) - Brazil.

    This file is part of the TerraLib - a Framework for building GIS enabled applications.

    TerraLib is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License,
    or (at your option) any later version.

    TerraLib is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with TerraLib. See COPYING. If not, write to
    TerraLib Team at <terralib-team@terralib.org>.
 */

/*!
  \file terralib/qt/af/Utils.h

  \brief Utility routines for the TerraLib Application Framework module.
*/

#ifndef __TERRAVIEW__INTERNAL_UTILS_H
#define __TERRAVIEW__INTERNAL_UTILS_H

// TerraLib
#include <terralib/common/Exception.h>
#include "Config.h"

// Forward declarations
class QString;

/*
\brief

\param[out]

\param[out]
*/
void GetProjectInformationsFromSettings(QString& defaultAuthor, int& maxSaved);

/*
\brief

\param

\param
*/
void SaveProjectInformationsOnSettings(const QString& defaultAuthor, const int& maxSaved);

/*!
\brief

\param
*/
void SaveOpenLastProjectOnSettings(bool openLast);

/*!
\brief

\return
*/
bool GetOpenLastProjectFromSettings();

/*!
\brief Writes the default project file.
*/
void WriteDefaultProjectFile(const QString& fileName);

/*!
\brief Creates a default QSettings.
*/
void CreateDefaultSettings();


#endif  // __TERRAVIEW__INTERNAL_UTILS_H

