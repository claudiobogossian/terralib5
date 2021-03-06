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

#include "../../core/uri/URI.h"
#include "../../dataaccess/datasource/DataSourceInfo.h"
#include "../../dataaccess/datasource/DataSourceInfoManager.h"
#include "../../maptools/DataSetLayer.h"

#include <QUrl>

void te::qt::af::XMLFormatter::format(te::map::AbstractLayer *l, const bool &encode)
{
  if(l == 0)
    return;

  l->setTitle(format(l->getTitle(), encode));

  std::list<te::common::TreeItemPtr> ls = l->getChildren();
  std::list<te::common::TreeItemPtr>::iterator it;

  if(l->getType() == "DATASETLAYER")
  {
    te::map::DataSetLayer* dl = static_cast<te::map::DataSetLayer*>(l);

    dl->setDataSetName(format(dl->getDataSetName(), encode));
  }

  for(it = ls.begin(); it != ls.end(); ++it)
    format(dynamic_cast<te::map::AbstractLayer*>((*it).get()), encode);
}

void te::qt::af::XMLFormatter::format(te::da::DataSourceInfo *d, const bool &encode)
{
  d->setTitle(format(d->getTitle(), encode));
  d->setDescription(format(d->getDescription(), encode));

  const te::core::URI& i = d->getConnInfo();
  std::string path = i.host() + i.path();

  if (!path.empty())
    path = format(path, encode);
}

void te::qt::af::XMLFormatter::formatDataSourceInfos(const bool &encode)
{
  te::da::DataSourceInfoManager::iterator it;
  te::da::DataSourceInfoManager::iterator beg = te::da::DataSourceInfoManager::getInstance().begin();
  te::da::DataSourceInfoManager::iterator end = te::da::DataSourceInfoManager::getInstance().end();

  for(it = beg; it != end; ++it)
    format(it->second.get(), encode);
}

std::string te::qt::af::XMLFormatter::format(const std::string &s, const bool& encode)
{
  return (encode) ?
        QUrl::toPercentEncoding(s.c_str()).data() :
        QUrl::fromPercentEncoding(QByteArray(s.c_str())).toUtf8().data();
}
