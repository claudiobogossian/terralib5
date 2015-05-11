/*  Copyright (C) 2008 National Institute For Space Research (INPE) - Brazil.

    This file is part of the TerraLib - a Framework for building GIS enabled applications.

    TerraLib is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License,
    or (at your option) any later version.

    TerraLib is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with TerraLib. See COPYING. If not, write to
    TerraLib Team at <terralib-team@terralib.org>.
 */

/*!
  \file TsLayerExplorer.cpp
 
  \brief Test suite for the LayerExplorer framework.
 */

// Unit-Test TerraLib
#include "TsLayerExplorer.h"

// TerraLib
#include <terralib/common.h>
#include <terralib/maptools.h>
#include <terralib/qt/widgets.h>

// Qt
#include <QtGui/QApplication>

// STL
#include <ctime>

CPPUNIT_TEST_SUITE_REGISTRATION( TsLayerExplorer );

void TsLayerExplorer::setUp()
{
}

void TsLayerExplorer::tearDown()
{
}

void TsLayerExplorer::tcCreateExplorer()
{
#ifdef TE_COMPILE_ALL
  int argc = 0;
  QApplication app(argc, 0);

// create the layers
  te::map::FolderLayer* rootLayer = new te::map::FolderLayer("0", "Root Layer");

  te::map::FeatureLayer* f1 = new te::map::FeatureLayer("1", "DataSet Layer 1", rootLayer);
  te::map::FeatureLayer* f11 = new te::map::FeatureLayer("1.1", "DataSet Layer 1.1", f1);
  te::map::FeatureLayer* f12 = new te::map::FeatureLayer("1.2", "DataSet Layer 1.2", f1);
  te::map::FeatureLayer* f13 = new te::map::FeatureLayer("1.3", "DataSet Layer 1.3", f1);

  te::map::FeatureLayer* f2 = new te::map::FeatureLayer("2", "DataSet Layer 2", rootLayer);
  te::map::FeatureLayer* f21 = new te::map::FeatureLayer("2.1", "DataSet Layer 2.1", f2);
  te::map::FeatureLayer* f22 = new te::map::FeatureLayer("2.2", "DataSet Layer 2.2", f2);
  te::map::FeatureLayer* f23 = new te::map::FeatureLayer("2.3", "DataSet Layer 2.3", f2);
  te::map::FeatureLayer* f24 = new te::map::FeatureLayer("2.4", "DataSet Layer 2.4", f2);

// create the explorer model and set the layer tree
  te::qt::widgets::LayerExplorerModel* model = new te::qt::widgets::LayerExplorerModel(rootLayer, 0);

// create the explorer view and set its model
  te::qt::widgets::LayerExplorer explorer(0);
  //explorer.setSelectionMode(QAbstractItemView::MultiSelection);
  explorer.setModel(model);

  QObject::connect(model, SIGNAL(dragDropEnded(const QModelIndex&, const QModelIndex&)),
                   &explorer, SLOT(updateCurrentIndex(const QModelIndex&, const QModelIndex&)));

  explorer.setDragEnabled(true);
  explorer.setAcceptDrops(true);
  explorer.setDropIndicatorShown(true);
  explorer.show();

  app.exec();

  delete rootLayer;

  return;
#endif
}

void TsLayerExplorer::tcPerformanceExplorer()
{
#ifdef TE_COMPILE_ALL
  int argc = 0;
  QApplication app(argc, 0);

  clock_t begin = 0;
  clock_t end = 0;  

// create the layers
  begin = clock();

  te::map::FolderLayer* rootLayer = new te::map::FolderLayer("0", "Root Layer");

  const int maxi = 1000;
  const int maxj = 10;
  const int maxk = 5;
  for(int i = 1; i <= maxi; ++i)
  {
    std::string id = te::common::Convert2String(i);
    std::string title = "DataSet Layer "  + id;

    te::map::FeatureLayer* f = new te::map::FeatureLayer(id, title, rootLayer);

    for(int j = 1; j <= maxj; ++j)
    {
      std::string jid = id + "." + te::common::Convert2String(j);
      title = "DataSet Layer "  + jid;

      te::map::FeatureLayer* fj = new te::map::FeatureLayer(jid, title, f);

      for(int k = 1; k <= maxk; ++k)
      {
        std::string kid = id + "." + te::common::Convert2String(k);
        title = "DataSet Layer "  + kid;

        te::map::FeatureLayer* fk = new te::map::FeatureLayer(kid, title, fj);
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

  te::qt::widgets::LayerExplorer explorer(0);
  //explorer.setSelectionMode(QAbstractItemView::MultiSelection);
  explorer.setModel(model);

  QObject::connect(model, SIGNAL(dragDropEnded(const QModelIndex&, const QModelIndex&)),
                   &explorer, SLOT(updateCurrentIndex(const QModelIndex&, const QModelIndex&)));

  explorer.setDragEnabled(true);
  explorer.setAcceptDrops(true);
  explorer.setDropIndicatorShown(true);

  explorer.show();

  end = clock();

  std::cout << std::endl << "Time to show the LayerExplorer widget for the first time: " << end - begin << " miliseconds" << std:: endl;

  app.exec();

  delete rootLayer;

  return;

#endif
}

