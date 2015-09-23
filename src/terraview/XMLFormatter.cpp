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


#include "XMLFormatter.h"

#include "Project.h"

#include <terralib/dataaccess/datasource/DataSourceInfo.h>
#include <terralib/dataaccess/datasource/DataSourceInfoManager.h>
#include <terralib/maptools/DataSetLayer.h>
#include <terralib/qt/af/XMLFormatter.h>

#include <QUrl>

void XMLFormatter::format(ProjectMetadata* p, const std::list<te::map::AbstractLayerPtr>& layers, const bool& encode)
{
  p->m_author = QString::fromStdString(te::qt::af::XMLFormatter::format(p->m_author.toStdString(), encode).c_str());
  p->m_title = QString::fromStdString(te::qt::af::XMLFormatter::format(p->m_title.toStdString(), encode));

  for(std::list<te::map::AbstractLayerPtr>::const_iterator it = layers.begin(); it != layers.end(); ++it)
    te::qt::af::XMLFormatter::format((*it).get(), encode);
}
