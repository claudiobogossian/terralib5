/*  Copyright (C) 2001-2014 National Institute For Space Research (INPE) - Brazil.

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
  \file VisualizationArea.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "VisualizationArea.h"
#include "HorizontalRulerModel.h"
#include "HorizontalRulerController.h"
#include "HorizontalRulerItem.h"
#include "RectangleItem.h"
#include "RectangleModel.h"
#include "RectangleController.h"
#include "VerticalRulerModel.h"
#include "VerticalRulerController.h"
#include "VerticalRulerItem.h"
#include "PaperModel.h"
#include "PaperController.h"
#include "PaperItem.h"
#include "MapModel.h"
#include "MapController.h"
#include "MapItem.h"
#include "ItemObserver.h"
#include "Context.h"
#include "MapGridModel.h"
#include "MapGridController.h"
#include "MapGridItem.h"

te::layout::VisualizationArea::VisualizationArea(te::gm::Envelope* boxArea)
{
  m_boxArea = boxArea;
}

te::layout::VisualizationArea::~VisualizationArea()
{

}

void te::layout::VisualizationArea::create()
{
  init();
}

void te::layout::VisualizationArea::init()
{
  double dpiX = te::layout::Context::getInstance()->getDpiX();

  double llx = m_boxArea->getLowerLeftX();
  double lly = m_boxArea->getLowerLeftY();
  double urx = m_boxArea->getUpperRightX();
  double ury = m_boxArea->getUpperRightY();

  //Paper
  //Retângulo: utilizando o canvas da Terralib 5
  PaperModel* modelPaper = new PaperModel(te::layout::Context::getInstance()->getPaperConfig());	
  PaperController* controllerPaper = new PaperController(modelPaper);
  ItemObserver* itemPaper = (ItemObserver*)controllerPaper->getView();
  PaperItem* qPaper = dynamic_cast<PaperItem*>(itemPaper);
  itemPaper->setItemPosition(0, 0);
  itemPaper->redraw();

  //Régua: utilizando o canvas da Terralib 5
  HorizontalRulerModel* modelRuler = new HorizontalRulerModel();		
  modelRuler->setBox(te::gm::Envelope(llx, ury - 10, urx + 10, ury));
  HorizontalRulerController* controllerRuler = new HorizontalRulerController(modelRuler);
  ItemObserver* itemRuler = (ItemObserver*)controllerRuler->getView();
  HorizontalRulerItem* rectRuler = dynamic_cast<HorizontalRulerItem*>(itemRuler);
  rectRuler->setPPI(dpiX);
  rectRuler->setItemPosition(llx, lly);
  rectRuler->redraw();

  VerticalRulerModel* modelRulerV = new VerticalRulerModel();		
  modelRulerV->setBox(te::gm::Envelope(llx, lly, llx + 10, ury + 10));
  VerticalRulerController* controllerRulerV = new VerticalRulerController(modelRulerV);
  ItemObserver* itemRulerV = (ItemObserver*)controllerRulerV->getView();
  VerticalRulerItem* rectRulerV = dynamic_cast<VerticalRulerItem*>(itemRulerV);
  rectRulerV->setPPI(dpiX);		
  rectRulerV->setItemPosition(llx, lly + 10);
  rectRulerV->redraw();

  //Retângulo: utilizando o canvas da Terralib 5
  RectangleModel* model = new RectangleModel();	
  model->setName("RECT_01");
  RectangleController* controller = new RectangleController(model);
  ItemObserver* item = (ItemObserver*)controller->getView();
  RectangleItem* rect = dynamic_cast<RectangleItem*>(item);
  item->setItemPosition(llx, lly);
  item->redraw();

  //Retângulo: utilizando o canvas da Terralib 5
  RectangleModel* model2 = new RectangleModel();		  
  model2->setName("RECT_02");
  RectangleController* controller2 = new RectangleController(model2);
  ItemObserver* item2 = (ItemObserver*)controller2->getView();
  RectangleItem* rect2 = dynamic_cast<RectangleItem*>(item2);
  item2->setItemPosition(llx + 40, lly + 40);
  item2->redraw();

  //-------------------------TESTE MAP---------------------------------------------
  //Retângulo: utilizando o canvas da Terralib 5
  MapModel* modelMap = new MapModel();		  
  modelMap->setName("MAPA_01");
  MapController* controllerMap = new MapController(modelMap);
  ItemObserver* itemMap = (ItemObserver*)controllerMap->getView();
  MapItem* qrectMap = dynamic_cast<MapItem*>(itemMap);
  itemMap->setItemPosition(llx + 60, lly + 60);
  itemMap->redraw();

  //Map with grid
  MapGridModel* modelMapGrid = new MapGridModel();		  
  modelMapGrid->setName("MAPA_GRID_01");
  MapGridController* controllerMapGrid = new MapGridController(modelMapGrid);
  ItemObserver* itemMapGrid = (ItemObserver*)controllerMapGrid->getView();
  MapGridItem* qrectMapGrid = dynamic_cast<MapGridItem*>(itemMapGrid);
  itemMapGrid->setItemPosition(llx + 100, lly + 100);
  itemMapGrid->redraw();
}
