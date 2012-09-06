/*  Copyright (C) 2001-2010 National Institute For Space Research (INPE) - Brazil.

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

// TerraLib
#include <terralib/common/TerraLib.h>
#include <terralib/common.h>
#include <terralib/dataaccess.h>
#include <terralib/maptools.h>
#include <terralib/qt/widgets.h>

//Qt
#include <QtGui/QApplication>
#include <QTableView>

// STL
#include <ctime>
#include <iostream>

int main(int argc, char *argv[])
{
  TerraLib::getInstance().initialize();

  QApplication app(argc, argv);

  clock_t begin = 0;
  clock_t end = 0;  

// create the layers
  begin = clock();

  te::map::FolderLayer* rootLayer = new te::map::FolderLayer("0", "Layers");

  const int maxi = 1;
  const int maxj = 3;
  const int maxk = 3;
  for(int i = 1; i <= maxi; ++i)
  {
    std::string id = te::common::Convert2String(i);
    std::string title = "Folder Layer "  + id;

    te::map::FolderLayer* f = new te::map::FolderLayer(id, title, rootLayer);

    for(int j = 1; j <= maxj; ++j)
    {
      std::string jid = id + "." + te::common::Convert2String(j);
      title = "Folder Layer "  + jid;

      te::map::FolderLayer* fj = new te::map::FolderLayer(jid, title, f);

      for(int k = 1; k <= maxk; ++k)
      {
        std::string kid = jid + "." + te::common::Convert2String(k);
        title = "Layer "  + kid;

        te::map::Layer* fk = new te::map::Layer(kid, title, fj);
      }
    }
  }

  end = clock();

  std::cout << std::endl << "Time to create te::map::AbstractLayer hierarchical tree with " << maxi * maxj * maxk << " items in: " << end - begin << " miliseconds" << std:: endl;

// create the explorer model and set the layer tree
  begin = clock();

  te::qt::widgets::LayerExplorerModel* model = new te::qt::widgets::LayerExplorerModel(rootLayer, 0);
  
  end = clock();

  std::cout << std::endl << "Time to create LayerExplorerModel for the hierarchical tree with " << maxi * maxj * maxk << " items in: " << end - begin << " miliseconds" << std:: endl;

// create the explorer view and set its model
  begin = clock();

  te::qt::widgets::LayerExplorer* explorer = new te::qt::widgets::LayerExplorer(model);

  explorer->setDragEnabled(true);
  explorer->setAcceptDrops(true);
  explorer->setDropIndicatorShown(true);

  explorer->show();

  end = clock();

  std::cout << std::endl << "Time to show the LayerExplorer widget for the first time: " << end - begin << " miliseconds" << std:: endl;

  int ret = app.exec();

  delete rootLayer;

  TerraLib::getInstance().finalize();

  return ret;
}
