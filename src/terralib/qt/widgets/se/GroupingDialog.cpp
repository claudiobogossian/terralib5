/*  Copyright (C) 2011-2012 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/qt/widgets/se/GroupingDialog.cpp

  \brief A dialog used to build a grouping.
*/

// TerraLib
#include "../../../dataaccess/dataset/DataSet.h"
#include "../../../dataaccess/dataset/DataSetType.h"
#include "../../../dataaccess/utils/Utils.h"
#include "../../../maptools/DataSetLayer.h"
#include "../../../maptools/RasterLayer.h"
#include "../../../se/ChannelSelection.h"
#include "../../../se/ColorMap.h"
#include "../../../se/CoverageStyle.h"
#include "../../../se/RasterSymbolizer.h"
#include "../../../se/SelectedChannel.h"
#include "../../../se/Utils.h"
#include "ColorMapWidget.h"
#include "GroupingDialog.h"
#include "GroupingWidget.h"
#include "ui_GroupingDialogForm.h"

te::qt::widgets::GroupingDialog::GroupingDialog(QWidget* parent, Qt::WindowFlags f)
  : QDialog(parent, f),
    m_ui(new Ui::GroupingDialogForm),
    m_groupingWidget(0),
    m_colorMapWidget(0),
    m_rasterSymb(0)
{
  m_ui->setupUi(this);

  m_ui->m_okPushButton->setEnabled(false);

  //connect
  connect(m_ui->m_okPushButton, SIGNAL(clicked()), this, SLOT(onPushButtonClicked()));
  m_ui->m_helpPushButton->setPageReference("widgets/grouping/grouping.html");
}

te::qt::widgets::GroupingDialog::~GroupingDialog()
{
}

void te::qt::widgets::GroupingDialog::setLayer(te::map::AbstractLayerPtr layer)
{
  m_layer = layer;

  //VERIFY LATER
  std::auto_ptr<te::map::LayerSchema> dsType(layer->getSchema());

  if(dsType->hasGeom())
  {
    buildVectorialGroupingInteface();

    m_groupingWidget->setLayer(layer);
  }
  else if(dsType->hasRaster())
  {
    buildRasterGroupingInteface();

    if(layer->getType() == "DATASETLAYER")
    {
      te::map::DataSetLayer* l = dynamic_cast<te::map::DataSetLayer*>(layer.get());

      if(l)
      {
        m_rasterSymb = te::se::GetRasterSymbolizer(l->getStyle());

        std::auto_ptr<te::da::DataSet> ds = m_layer->getData();

        if(ds.get())
        {
          std::size_t rpos = te::da::GetFirstPropertyPos(ds.get(), te::dt::RASTER_TYPE);
          m_raster.reset(ds->getRaster(rpos).release());
        }
      }
    }
    else if(layer->getType() == "RASTERLAYER")
    {
      te::map::RasterLayer* l = dynamic_cast<te::map::RasterLayer*>(layer.get());

      if(l)
      {
        m_rasterSymb = te::se::GetRasterSymbolizer(l->getStyle());

        m_raster.reset(l->getRaster());
      }
    }

    if(m_rasterSymb)
    {
      if(m_rasterSymb->getColorMap())
      {
        m_colorMapWidget->setColorMap(m_rasterSymb->getColorMap());
      }
      else
      {
        te::se::ColorMap* cm = new te::se::ColorMap();

        m_colorMapWidget->setColorMap(cm);
      }
    }

    if(m_raster.get())
    {
      m_colorMapWidget->setRaster(m_raster.get());
    }
  }
}

void te::qt::widgets::GroupingDialog::setGroupingLayers(std::vector<te::map::AbstractLayerPtr> groupingLayers)
{
  m_groupingLayers = groupingLayers;
}

void te::qt::widgets::GroupingDialog::buildVectorialGroupingInteface()
{
  // Fill Widget
  m_groupingWidget = new te::qt::widgets::GroupingWidget(m_ui->m_widget);

  m_groupingWidget->setGroupingLayers(m_groupingLayers);

  connect(m_groupingWidget, SIGNAL(applyPushButtonClicked()), this, SLOT(onApplyClicked()));

  // Adjusting...
  QGridLayout* layout = new QGridLayout(m_ui->m_widget);
  layout->setContentsMargins(0,0,0,0);
  layout->addWidget(m_groupingWidget);
}

void te::qt::widgets::GroupingDialog::buildRasterGroupingInteface()
{
  // Fill Widget
  m_colorMapWidget = new te::qt::widgets::ColorMapWidget(m_ui->m_widget);

  connect(m_colorMapWidget, SIGNAL(applyPushButtonClicked()), this, SLOT(onApplyClicked()));

  // Adjusting...
  QGridLayout* layout = new QGridLayout(m_ui->m_widget);
  layout->setContentsMargins(0,0,0,0);
  layout->addWidget(m_colorMapWidget);
}

void te::qt::widgets::GroupingDialog::onPushButtonClicked()
{
  if(m_groupingWidget)
  {
    te::map::Grouping* g = m_groupingWidget->getGrouping().release();

    m_layer->setGrouping(g);
  }

  if(m_colorMapWidget)
  {
    te::se::ColorMap* cm = m_colorMapWidget->getColorMap();

    m_rasterSymb->setColorMap(cm);

    std::string band = m_colorMapWidget->getCurrentBand();

    te::se::ChannelSelection* cs = m_rasterSymb->getChannelSelection();

    if(cs->getGrayChannel())
    {
      te::se::SelectedChannel* scGray = cs->getGrayChannel();

      scGray->setSourceChannelName(band);
    }
    else
    {
      te::se::SelectedChannel* scGray = new te::se::SelectedChannel();

      scGray->setSourceChannelName(band);

      cs->setGrayChannel(scGray);
    }

    cs->setColorCompositionType(te::se::GRAY_COMPOSITION);
  }

  accept();
}

void te::qt::widgets::GroupingDialog::onApplyClicked()
{
  m_ui->m_okPushButton->setEnabled(true);
}
