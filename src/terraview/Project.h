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
  \file Project.h

  \brief This class models the concept of a project for the TerraView.
*/

#ifndef __TERRALIB_TERRAVIEW_INTERNAL_PROJECT_H
#define __TERRALIB_TERRAVIEW_INTERNAL_PROJECT_H

// Terralib
#include "../terralib/maptools/AbstractLayer.h"
#include "Config.h"

// Qt
#include <QString>

// STL
#include <list>

struct ProjectMetadata
{
  QString m_title;                                      //!< The title of the project.
  QString m_author;                                     //!< The author of the project.
  bool m_changed;                                       //!< Flag indicating that the project needs to be saved.
  QString m_fileName;                                   //!< The project file.
};

void SaveProject(const ProjectMetadata& proj, const std::list<te::map::AbstractLayerPtr>& layers);

void LoadProject(const QString& projFile, ProjectMetadata& proj, std::list<te::map::AbstractLayerPtr>& layers);

#endif  // __TERRALIB_QT_AF_INTERNAL_PROJECT_H
