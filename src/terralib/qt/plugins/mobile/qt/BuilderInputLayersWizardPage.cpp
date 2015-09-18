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
\file src/terraMobilePlugin/qt/BuilderInputLayersWizardPage.h

\brief This interface is used to get the input layers parameters for GeoPackage Builder wizard operation.
*/

// TerraLib
#include "../../../widgets/utils/DoubleListWidget.h"
#include "BuilderInputLayersWizardPage.h"

// Qt
#include <QGridLayout>


te::qt::plugins::terramobile::BuilderInputLayersWizardPage::BuilderInputLayersWizardPage(QWidget* parent)
  : QWizardPage(parent)
{
  m_widget.reset(new te::qt::widgets::DoubleListWidget(this));

  QGridLayout* layout = new QGridLayout(this);

  layout->addWidget(m_widget.get());

  //configure page
  this->setTitle(tr("Input Layers Selection"));
  this->setSubTitle(tr("Select the layers to be exported to geopackage."));

  m_widget->setLeftLabel("Input Layers");
  m_widget->setRightLabel("GeoPackage Layers");

  //connect slots
  connect(m_widget.get(), SIGNAL(itemChanged()), this, SLOT(onItemChanged()));
}

te::qt::plugins::terramobile::BuilderInputLayersWizardPage::~BuilderInputLayersWizardPage()
{

}

bool te::qt::plugins::terramobile::BuilderInputLayersWizardPage::isComplete() const
{
  if (m_widget->getOutputValues().empty())
    return false;

  return true;
}

te::qt::widgets::DoubleListWidget* te::qt::plugins::terramobile::BuilderInputLayersWizardPage::getWidget() const
{
  return m_widget.get();
}

void te::qt::plugins::terramobile::BuilderInputLayersWizardPage::onItemChanged()
{
  emit completeChanged();
}
