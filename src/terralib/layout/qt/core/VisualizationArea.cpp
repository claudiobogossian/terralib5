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
  \file VisualizationArea.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "VisualizationArea.h"
#include "../../item/PaperModel.h"
#include "../../item/PaperController.h"
#include "../item/PaperItem.h"
#include "../../core/pattern/mvc/ItemObserver.h"
#include "../../core/pattern/singleton/Context.h"

te::layout::VisualizationArea::VisualizationArea(te::gm::Envelope boxArea)
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
}

void te::layout::VisualizationArea::createPaper()
{
  //Paper
  PaperModel* modelPaper = new PaperModel(te::layout::Context::getInstance().getPaperConfig());	

  double x = modelPaper->getBox().getLowerLeftX();
  double y = modelPaper->getBox().getLowerLeftY();

  PaperController* controllerPaper = new PaperController(modelPaper);
  ItemObserver* itemPaper = (ItemObserver*)controllerPaper->getView();
  PaperItem* qPaper = dynamic_cast<PaperItem*>(itemPaper);
  
  qPaper->setPos(QPointF(x,y));
  qPaper->setZValue(0);
  qPaper->redraw();
}

void te::layout::VisualizationArea::changeBoxArea( te::gm::Envelope boxArea )
{
  m_boxArea = boxArea;
}
