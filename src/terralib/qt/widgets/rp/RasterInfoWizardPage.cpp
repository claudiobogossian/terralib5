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
  \file terralib/qt/widgets/rp/RasterInfoWizardPage.cpp

  \brief This file defines a class for a Raster Info Wizard page.
*/

// TerraLib
#include "RasterInfoWidget.h"
#include "RasterInfoWizardPage.h"
#include "ui_RasterInfoWidgetForm.h"

// Qt
#include <QGridLayout>
#include <QMessageBox>

te::qt::widgets::RasterInfoWizardPage::RasterInfoWizardPage(QWidget* parent)
  : QWizardPage(parent)
{
//build form
  QGridLayout* layout = new QGridLayout(this);
  m_rinfo.reset( new te::qt::widgets::RasterInfoWidget(true, this, 0));
  layout->addWidget(m_rinfo.get(), 0, 0);

//configure page
  this->setTitle(tr("Raster Information"));
  this->setSubTitle(tr("Defines the parameters of raster creation."));

//connects
  connect(m_rinfo->getForm()->m_nameLineEdit, SIGNAL(textChanged(QString)), this, SLOT(onChanged(QString)));
  connect(m_rinfo->getForm()->m_fileNameLineEdit, SIGNAL(textChanged(QString)), this, SLOT(onChanged(QString)));
  connect(m_rinfo->getForm()->m_fileRadioButton, SIGNAL(clicked()), this, SLOT(onChanged()));
  connect(m_rinfo->getForm()->m_memRadioButton, SIGNAL(clicked()), this, SLOT(onChanged()));
  connect(m_rinfo->getForm()->m_overightRadioButton, SIGNAL(clicked()), this, SLOT(onChanged()));
}

te::qt::widgets::RasterInfoWizardPage::~RasterInfoWizardPage()
{
}

bool te::qt::widgets::RasterInfoWizardPage::isComplete() const
{
  if(m_rinfo->getForm()->m_overightRadioButton->isChecked() == false)
  {
    if(m_rinfo->getForm()->m_nameLineEdit->text().isEmpty())
      return false;

    if(m_rinfo->getForm()->m_fileRadioButton->isChecked())
    {
      if( m_rinfo->getName().empty() && m_rinfo->getPath().empty() )
        return false;
    }
  }

  return true;
}

te::qt::widgets::RasterInfoWidget* te::qt::widgets::RasterInfoWizardPage::getWidget()
{
  return m_rinfo.get();
}

void te::qt::widgets::RasterInfoWizardPage::onChanged()
{
  emit completeChanged();
}

void te::qt::widgets::RasterInfoWizardPage::onChanged(QString)
{
  emit completeChanged();
}
