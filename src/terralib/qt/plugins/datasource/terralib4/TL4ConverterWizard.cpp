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
  \file terralib/qt/plugins/terralib4/TL4ConverterWizard.cpp

  \brief A Qt dialog for ....
*/

// TerraLib
#include "../../../../dataaccess/datasource/DataSource.h"
#include "../../../../dataaccess/datasource/DataSourceFactory.h"
#include "TL4ConverterWizard.h"
#include "TL4ConnectorWizardPage.h"
#include "TL4LayerSelectionWizardPage.h"
#include "ui_TL4ConverterWizardForm.h"
#include "ui_TL4ConnectorWizardPageForm.h"
#include "ui_TL4LayerSelectionWizardPageForm.h"

// STL
#include <cassert>

// Qt
#include <QtGui/QAbstractButton>
#include <QtGui/QMessageBox>
#include <QtGui/QVBoxLayout>

te::qt::plugins::terralib4::TL4ConverterWizard::TL4ConverterWizard(QWidget* parent, Qt::WindowFlags f)
  : QWizard(parent, f),
    m_ui(new Ui::TL4ConverterWizardForm)
{
// setup controls
  m_ui->setupUi(this);

// add pages
  m_connectorPage.reset(new TL4ConnectorWizardPage(this));
  m_connectorPage->setTitle(tr("Data Source Selection"));
  m_connectorPage->setSubTitle(tr("Please, select the data source where the data is stored"));

  m_layerSelectionPage.reset(new TL4LayerSelectionWizardPage(this));
  m_layerSelectionPage->setTitle(tr("Data Source Selection"));
  m_layerSelectionPage->setSubTitle(tr("Please, select the data source where the data is stored"));

  m_layerSelectionPage->setFinalPage(true);

  setPage(0, m_connectorPage.get());
  setPage(1, m_layerSelectionPage.get());

  connect(this->button(QWizard::NextButton), SIGNAL(pressed()), this, SLOT(next()));
  connect(this->button(QWizard::BackButton), SIGNAL(pressed()), this, SLOT(back()));
  connect(this->button(QWizard::CommitButton), SIGNAL(pressed()), this, SLOT(commit()));
  connect(this->button(QWizard::HelpButton), SIGNAL(pressed()), this, SLOT(help()));

}

te::qt::plugins::terralib4::TL4ConverterWizard::~TL4ConverterWizard()
{
}

int te::qt::plugins::terralib4::TL4ConverterWizard::nextId() const
{
  return QWizard::nextId();
}

void te::qt::plugins::terralib4::TL4ConverterWizard::back()
{

}

void te::qt::plugins::terralib4::TL4ConverterWizard::next()
{
  if(currentId() == 0)
  {
    std::map<std::string, std::string> connInfo = m_connectorPage->getConnInfo();

    std::auto_ptr<te::da::DataSource> ds(te::da::DataSourceFactory::make("TERRALIB4"));
    ds->setConnectionInfo(connInfo);
    ds->open();

    std::vector<std::string> datasets = ds->getDataSetNames();

    m_layerSelectionPage->setDatasets(datasets);
  }
  else
  {
    
  }
  QWizard::next();
}

void te::qt::plugins::terralib4::TL4ConverterWizard::commit()
{

}

void te::qt::plugins::terralib4::TL4ConverterWizard::help()
{
  QMessageBox::warning(this,
                       tr("TerraLib Qt Components"),
                       tr("This option is not implemented yet!\nWe will provide it soon!"));
}


