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
#include "../../item/PaperModel.h"
#include "../../item/PaperController.h"
#include "../item/PaperItem.h"
#include "../../core/pattern/mvc/ItemObserver.h"
#include "../../core/pattern/singleton/Context.h"

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
}

void te::layout::VisualizationArea::createPaper()
{
  if(!m_boxArea)
    return;

  double dpiX = te::layout::Context::getInstance().getDpiX();

  double llx = m_boxArea->getLowerLeftX();
  double lly = m_boxArea->getLowerLeftY();
  double urx = m_boxArea->getUpperRightX();
  double ury = m_boxArea->getUpperRightY();

  //Paper
  PaperModel* modelPaper = new PaperModel(te::layout::Context::getInstance().getPaperConfig());	
  PaperController* controllerPaper = new PaperController(modelPaper);
  ItemObserver* itemPaper = (ItemObserver*)controllerPaper->getView();
  PaperItem* qPaper = dynamic_cast<PaperItem*>(itemPaper);
  qPaper->setPos(QPointF(0, 0));
  qPaper->setZValue(0);
  qPaper->redraw();
}

void te::layout::VisualizationArea::changeBoxArea( te::gm::Envelope* boxArea )
{
  m_boxArea = boxArea;
}
