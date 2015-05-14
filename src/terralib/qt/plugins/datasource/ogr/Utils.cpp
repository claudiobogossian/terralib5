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
  \file terralib/qt/plugins/datasource/ogr/Utils.cpp
   
  \brief Utility functions for the OGR data source widget plugin.
*/

// TerraLib
#include "Utils.h"

// Qt
#include <QtCore/QDir>
#include <QtCore/QFileInfo>

// STL
#include <cassert>

bool te::qt::plugins::ogr::IsShapeFile(const QString& path)
{
  QFileInfo fileInfo(path);

  if(!fileInfo.isFile())
    return false;

  if(fileInfo.suffix().toLower() == "shp")
    return true;

  return false;
}

bool te::qt::plugins::ogr::HasShapeFileSpatialIndex(const QString& path)
{
  assert(IsShapeFile(path));

  QFileInfo fileInfo(path);

  // Gets the ShapeFile name
  QString fileName = fileInfo.fileName();

  // Builds the ShapeFile directory
  QDir dir(fileInfo.absolutePath());

  // Looking for .qix file
  QString qixFile = fileName;
  qixFile.replace(".shp", ".qix");
  if(dir.exists(qixFile))
    return true;

  // Looking for .sbn file
  QString sbnFile = fileName;
  sbnFile.replace(".shp", ".sbn");
  if(dir.exists(sbnFile))
    return true;

  return false;
}
