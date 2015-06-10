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
  \file  terralib/qt/widgets/externalTable/DataPropertiesDialog.h

  \brief A dialog used to configure the properties of a new textual file based layer
*/

//Terralib
#include "../../../dataaccess/dataset/DataSet.h"
#include "../../../dataaccess/dataset/DataSetAdapter.h"
#include "../../../dataaccess/dataset/DataSetType.h"
#include "../../../dataaccess/dataset/DataSetTypeConverter.h"
#include "../../../dataaccess/datasource/DataSource.h"
#include "../../../dataaccess/utils/Utils.h"
#include "../../../geometry/GeometryProperty.h"
#include "../../../maptools/DataSetAdapterLayer.h"
#include "../../../qt/widgets/utils/ScopedCursor.h"
#include "../../../se/FeatureTypeStyle.h"
#include "../../../se/Utils.h"
#include "DataPropertiesDialog.h"
#include "DataPropertiesWidget.h"
#include "ui_DataPropertiesDialogForm.h"
#include "ui_DataPropertiesWidgetForm.h"

// Boost
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>

//Utility functions used mianly to pupulate ui elements.
te::gm::Envelope& computeDataSetEnvelope(std::auto_ptr<te::da::DataSet> dataset,
                            const std::string& propertyName)
{
  te::gm::Envelope* result = new te::gm::Envelope;

  while(dataset->moveNext())
  {
    te::gm::Geometry* geom = dataset->getGeometry(propertyName).release();
    assert(geom);
    result->Union(*geom->getMBR());
  }
  return *result;
}

te::qt::widgets::DataPropertiesDialog::DataPropertiesDialog(QWidget* parent, Qt::WindowFlags f)
  : QDialog(parent, f),
    m_ui(new Ui::DataPropertiesDialogForm)
{
  m_ui->setupUi(this);

  // data properties Widget
  m_dataPropertiesWidget = new te::qt::widgets::DatapPropertiesWidget(this, 0);

  // Adjusting...
  QGridLayout* layout = new QGridLayout(m_ui->m_dataWidgetFrame);
  layout->addWidget(m_dataPropertiesWidget);
  layout->setContentsMargins(0,0,0,0);
  this->layout()->setSizeConstraint(QLayout::SetFixedSize);

// connect signal and slots
  connect(m_ui->m_okPushButton, SIGNAL(clicked()), this, SLOT(onOkPushButtonClicked()));

  m_ui->m_helpPushButton->setPageReference("widgets/external_table/external_table.html");
}

te::qt::widgets::DataPropertiesDialog::~DataPropertiesDialog()
{
  delete m_dataPropertiesWidget;
}

void te::qt::widgets::DataPropertiesDialog::onOkPushButtonClicked()
{
  te::qt::widgets::ScopedCursor c(Qt::WaitCursor);

  static boost::uuids::basic_random_generator<boost::mt19937> gen;
  boost::uuids::uuid u = gen();
  std::string id = boost::uuids::to_string(u);

  if(m_dataPropertiesWidget->getDataSetType())
  {
    std::string title = m_dataPropertiesWidget->getDataSetType()->getTitle().empty() ? m_dataPropertiesWidget->getDataSetType()->getName() :  m_dataPropertiesWidget->getDataSetType()->getTitle();
    m_DataSetAdapterLayer = new te::map::DataSetAdapterLayer(id, title);
    m_DataSetAdapterLayer->setDataSetName(m_dataPropertiesWidget->getDataSetType()->getName());
    m_DataSetAdapterLayer->setDataSourceId(m_dataPropertiesWidget->getDataSource()->getId());
    m_DataSetAdapterLayer->setVisibility(te::map::NOT_VISIBLE);
    m_DataSetAdapterLayer->setRendererType("ABSTRACT_LAYER_RENDERER");
    m_DataSetAdapterLayer->setConverter(m_dataPropertiesWidget->getConverter());

    if(m_DataSetAdapterLayer->getConverter()->getResult()->hasGeom())
    {
      te::gm::GeometryProperty* gp = te::da::GetFirstGeomProperty(m_DataSetAdapterLayer->getConverter()->getResult());
      std::auto_ptr<te::gm::Envelope> mbr(te::da::GetExtent(m_DataSetAdapterLayer->getDataSetName(), gp->getName(), m_DataSetAdapterLayer->getDataSourceId()));

      //Checking if it was possible to obtain an envelope
      if(!mbr.get())
        m_DataSetAdapterLayer->setExtent(computeDataSetEnvelope(m_DataSetAdapterLayer->getData(), gp->getName()));  //Build an envelope
      else
        m_DataSetAdapterLayer->setExtent(*(mbr.release())); // using the envelope obtained by common means

      m_DataSetAdapterLayer->setSRID(gp->getSRID());
      m_DataSetAdapterLayer->setStyle(te::se::CreateFeatureTypeStyle(gp->getGeometryType()));
    }

    this->accept();
  }
  else
    this->reject();
}

te::map::DataSetAdapterLayerPtr te::qt::widgets::DataPropertiesDialog::getDataSetAdapterLayer()
{
  return m_DataSetAdapterLayer;
}