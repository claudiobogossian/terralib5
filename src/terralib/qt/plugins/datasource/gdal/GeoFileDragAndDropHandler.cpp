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
\file terralib/qt/widgets/utils/GeoFileDragAndDropHandler.h

\brief A class used to handle geofile drag and drop events.
*/

#include "../../../widgets/layer/explorer/LayerItemView.h"
#include "GeoFileDragAndDropHandler.h"
#include "Utils.h"

// Terralib
#include "../../../../dataaccess/datasource/DataSource.h"
#include "../../../../dataaccess/datasource/DataSourceFactory.h"

// Qt
#include <QDragEnterEvent>
#include <QEvent>
#include <QMimeData>

bool IsSupported(const std::string& fileName)
{
  std::map<std::string, std::string> connInfo;
  connInfo["URI"] = fileName;

  std::auto_ptr<te::da::DataSource> ds(te::da::DataSourceFactory::make("GDAL"));
  ds->setConnectionInfo(connInfo);

  try
  {
    ds->open();

    te::da::DataSetTypePtr dsType;

    std::vector<std::string> dsets = ds->getDataSetNames();

    if (dsets.size() != 1)
      return false;

    dsType = ds->getDataSetType(dsets[0]);

    if (dsType.get() == 0)
      return false;
  }
  catch (const std::exception& ex)
  {
    return false;
  }
  catch (...)
  {
    return false;
  }

  return true;
}

te::qt::plugins::gdal::GeoFileDragAndDropHandler::GeoFileDragAndDropHandler() : m_isValid(false)
{

}

te::qt::plugins::gdal::GeoFileDragAndDropHandler::~GeoFileDragAndDropHandler()
{

}

bool te::qt::plugins::gdal::GeoFileDragAndDropHandler::eventFilter(QObject* watched, QEvent* e)
{
  if (e->type() == QEvent::DragEnter)
  {
    QDragEnterEvent* dragEvent = dynamic_cast<QDragEnterEvent*>(e);
    dragEvent->setDropAction(Qt::DropAction::LinkAction);

    const QMimeData* mimeData = dragEvent->mimeData();

    QList<QUrl> urlList = mimeData->urls();

    m_isValid = false;

    // extract the local paths of the files
    for (int i = 0; i < urlList.size(); ++i)
    {
      std::string fileName = urlList.at(i).toLocalFile().toUtf8().constData();

      if (IsSupported(fileName))
      {
        m_isValid = true;
        break;
      }
    }

    if (m_isValid)
    {
      dragEvent->accept();
      return true;
    }
    else
    {
      return QObject::eventFilter(watched, e);
    }
  }
  else if (e->type() == QEvent::Drop && m_isValid)
  {
    QDropEvent* dropEvent = dynamic_cast<QDropEvent*>(e);

    const QMimeData* mimeData = dropEvent->mimeData();

    QStringList pathList;
    QList<QUrl> urlList = mimeData->urls();

    // extract the local paths of the files
    for (int i = 0; i < urlList.size(); ++i)
    {
      pathList.append(urlList.at(i).toLocalFile());
    }

    te::qt::plugins::gdal::CreateLayers(pathList);

    return true;
  }

  return QObject::eventFilter(watched, e);
}
