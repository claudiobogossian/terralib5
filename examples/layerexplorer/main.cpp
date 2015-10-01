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

// TerraLib
#include <terralib/common/TerraLib.h>
#include <terralib/common.h>
#include <terralib/dataaccess.h>
#include <terralib/maptools.h>
#include <terralib/qt/widgets.h>
#include <terralib/qt/widgets/layer/explorer/LayerItemModel.h>
#include <terralib/qt/widgets/layer/explorer/LayerItemView.h>

//Qt
#include <QApplication>
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

  const int maxi = 1000;
  const int maxj = 10;
  const int maxk = 5;

  std::list<te::map::AbstractLayerPtr> layers;

  for(int i = 1; i <= maxi; ++i)
  {
    std::string id = te::common::Convert2String(i);
    std::string title = "Folder Layer "  + id;

    te::map::FolderLayer* f = new te::map::FolderLayer(id, title, 0);

    for(int j = 1; j <= maxj; ++j)
    {
      std::string jid = id + "." + te::common::Convert2String(j);
      title = "Folder Layer "  + jid;

      te::map::FolderLayer* fj = new te::map::FolderLayer(jid, title, f);

      for(int k = 1; k <= maxk; ++k)
      {
        std::string kid = jid + "." + te::common::Convert2String(k);
        title = "DataSet Layer "  + kid;

        /*te::map::DataSetLayer* fk = */new te::map::DataSetLayer(kid, title, fj);
      }
    }

    te::map::AbstractLayerPtr layerPtr(f);

    layers.push_back(layerPtr);
  }

  end = clock();

  std::cout << std::endl << "Time to create te::map::AbstractLayer hierarchical tree with " << maxi * maxj * maxk << " items in: " << end - begin << " miliseconds" << std:: endl;

  // Create the layer view
  begin = clock();

  te::qt::widgets::LayerItemView* layerItemView = new te::qt::widgets::LayerItemView();

  end = clock();

  std::cout << std::endl << "Time to create LayerExplorer for the hierarchical tree with " << maxi * maxj * maxk << " items in: " << end - begin << " miliseconds" << std:: endl;

  begin = clock();

  layerItemView->setLayers(layers);

  layerItemView->show();

  end = clock();

  std::cout << std::endl << "Time to show the LayerExplorer widget for the first time: " << end - begin << " miliseconds" << std:: endl;

  int ret = app.exec();

  delete layerItemView;

  TerraLib::getInstance().finalize();

  return ret;
}
