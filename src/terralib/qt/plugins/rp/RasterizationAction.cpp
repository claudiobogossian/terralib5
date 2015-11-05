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


#include "RasterizationAction.h"

#include "../../../dataaccess/utils/Utils.h"
#include "../../../geometry/GeometryProperty.h"
#include "../../../qt/af/ApplicationController.h"
#include "../../../qt/widgets/rp/RasterizationWizard.h"

te::qt::plugins::rp::RasterizationAction::RasterizationAction(QMenu *menu, QMenu *popupMenu) :
  AbstractAction(menu, popupMenu)
{
  createAction(tr("Rasterization...").toStdString(), "rasterization");
  m_action->setObjectName("Processing.Raster Processing.Rasterization");
  //createPopUpAction(tr("Rasterization...").toStdString(), "rasterization");
}

te::qt::plugins::rp::RasterizationAction::~RasterizationAction()
{
}

void te::qt::plugins::rp::RasterizationAction::onActionActivated(bool checked)
{
  te::qt::widgets::RasterizationWizard dlg(te::qt::af::AppCtrlSingleton::getInstance().getMainWindow());

  std::list<te::map::AbstractLayerPtr> layersList = getLayers();

  std::list<te::map::AbstractLayerPtr> utils;

  std::list<te::map::AbstractLayerPtr>::iterator it = layersList.begin();

  while (it != layersList.end())
  {
    std::auto_ptr<te::da::DataSetType> type = (*it)->getSchema();

    if (type->hasRaster())
    {
      ++it;
      continue;
    }

    te::gm::GeometryProperty* gp = te::da::GetFirstGeomProperty(type.get());
    if (gp->getGeometryType() == te::gm::PolygonType || gp->getGeometryType() == te::gm::MultiPolygonType)
    {
      utils.push_back(*it);
    }

    ++it;
  }

  dlg.setList(utils);

  if(dlg.exec() == QDialog::Accepted)
  {
    //add new layer
    addNewLayer(dlg.getOutputLayer());
  }
}

void te::qt::plugins::rp::RasterizationAction::onPopUpActionActivated(bool checked)
{
}


