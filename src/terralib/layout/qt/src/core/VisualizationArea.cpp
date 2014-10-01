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
#include "VerticalRulerModel.h"
#include "VerticalRulerController.h"
#include "VerticalRulerItem.h"
#include "PaperModel.h"
#include "PaperController.h"
#include "PaperItem.h"
#include "ItemObserver.h"
#include "Context.h"

te::layout::VisualizationArea::VisualizationArea(te::gm::Envelope* boxArea)
{
  m_boxArea = boxArea;
}

te::layout::VisualizationArea::~VisualizationArea()
{

}

void te::layout::VisualizationArea::build()
{
  init();
}

void te::layout::VisualizationArea::init()
{
  createPaper();
  createHorizontalRuler();
  createVerticalRuler();
}

void te::layout::VisualizationArea::rebuildWithoutPaper()
{
  createHorizontalRuler();
  createVerticalRuler();
}

void te::layout::VisualizationArea::createVerticalRuler()
{
  double dpiX = te::layout::Context::getInstance()->getDpiX();

  double llx = m_boxArea->getLowerLeftX();
  double lly = m_boxArea->getLowerLeftY();
  double urx = m_boxArea->getUpperRightX();
  double ury = m_boxArea->getUpperRightY();

  //Régua Vertical
  VerticalRulerModel* modelRulerV = new VerticalRulerModel();		
  modelRulerV->setBox(te::gm::Envelope(llx, lly, llx + 10, ury));
  VerticalRulerController* controllerRulerV = new VerticalRulerController(modelRulerV);
  ItemObserver* itemRulerV = (ItemObserver*)controllerRulerV->getView();
  VerticalRulerItem* rectRulerV = dynamic_cast<VerticalRulerItem*>(itemRulerV);
  rectRulerV->setPPI(dpiX);		
  rectRulerV->setPos(QPointF(llx, lly));
  rectRulerV->redraw();
  rectRulerV->setZValue(10001);
}

void te::layout::VisualizationArea::createHorizontalRuler()
{
  double dpiX = te::layout::Context::getInstance()->getDpiX();

  double llx = m_boxArea->getLowerLeftX();
  double lly = m_boxArea->getLowerLeftY();
  double urx = m_boxArea->getUpperRightX();
  double ury = m_boxArea->getUpperRightY();

  //Régua Horizontal
  HorizontalRulerModel* modelRuler = new HorizontalRulerModel();		
  modelRuler->setBox(te::gm::Envelope(llx, lly, urx, lly + 10));
  HorizontalRulerController* controllerRuler = new HorizontalRulerController(modelRuler);
  ItemObserver* itemRuler = (ItemObserver*)controllerRuler->getView();
  HorizontalRulerItem* rectRuler = dynamic_cast<HorizontalRulerItem*>(itemRuler);
  rectRuler->setPPI(dpiX);
  rectRuler->setPos(QPointF(llx, lly));
  rectRuler->redraw();
  rectRuler->setZValue(10000);
}

void te::layout::VisualizationArea::createPaper()
{
  double dpiX = te::layout::Context::getInstance()->getDpiX();

  double llx = m_boxArea->getLowerLeftX();
  double lly = m_boxArea->getLowerLeftY();
  double urx = m_boxArea->getUpperRightX();
  double ury = m_boxArea->getUpperRightY();

  //Paper
  PaperModel* modelPaper = new PaperModel(te::layout::Context::getInstance()->getPaperConfig());	
  PaperController* controllerPaper = new PaperController(modelPaper);
  ItemObserver* itemPaper = (ItemObserver*)controllerPaper->getView();
  PaperItem* qPaper = dynamic_cast<PaperItem*>(itemPaper);
  qPaper->setPos(QPointF(0,lly));
  qPaper->setZValue(1);
  qPaper->redraw();
}

void te::layout::VisualizationArea::changeBoxArea( te::gm::Envelope* boxArea )
{
  m_boxArea = boxArea;
}
