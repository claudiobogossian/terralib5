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

  \brief A wizard for guiding the conversion of a TerraLib 4.x database to a OGC SFS compliant format or any other format known by OGR and GDAL.
*/

// TerraLib
#include "../../../../common/Exception.h"
#include "../../../../common/Translator.h"
#include "../../../../dataaccess.h"
#include "../../../../qt/widgets/datasource/selector/DataSourceSelectorWidget.h"
#include "../../../../qt/widgets/datasource/selector/DataSourceSelectorWizardPage.h"
#include "../../../../qt/widgets/layer/utils/DataSet2Layer.h"
#include "../../../../qt/widgets/help/HelpPushButton.h"
#include "../../../../qt/widgets/rp/Utils.h"
#include "../../../../qt/widgets/utils/ScopedCursor.h"
#include "../../../../qt/af/ApplicationController.h"
#include "../../../../qt/af/Project.h"
#include "../../../../qt/af/events/LayerEvents.h"
#include "../../../../raster/Utils.h"
#include "TL4ConverterWizard.h"
#include "TL4ConnectorWizardPage.h"
#include "TL4LayerSelectionWizardPage.h"
#include "TL4RasterFolderSelectionWizardPage.h"
#include "TL4FinalPageWizardPage.h"
#include "ui_TL4ConverterWizardForm.h"
#include "ui_TL4ConnectorWizardPageForm.h"
#include "ui_TL4LayerSelectionWizardPageForm.h"
#include "ui_TL4RasterFolderSelectionWizardPageForm.h"
#include "ui_TL4FinalPageWizardPageForm.h"

// STL
#include <cassert>

// Boost
#include <boost/lexical_cast.hpp>

// Qt
#include <QtGui/QAbstractButton>
#include <QtGui/QCursor>
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
  m_connectorPage->setTitle(tr("TerraLib 4.x Database"));
  m_connectorPage->setSubTitle(tr("Connect to a TerraLib 4.x database."));

  m_layerSelectionPage.reset(new TL4LayerSelectionWizardPage(this));
  m_layerSelectionPage->setTitle(tr("Layer Selection"));
  m_layerSelectionPage->setSubTitle(tr("Select the layers to be added to the project"));

  m_datasourceSelectorPage.reset(new te::qt::widgets::DataSourceSelectorWizardPage(this));
  m_datasourceSelectorPage->setTitle(tr("Target Data Source"));
  m_datasourceSelectorPage->setSubTitle(tr("Select a data source to store the converted data"));
  m_datasourceSelectorPage->getSelectorWidget()->setSelectionMode(QAbstractItemView::SingleSelection);
  m_datasourceSelectorPage->getSelectorWidget()->showDataSourceWithRasterSupport(false);

  m_rasterFolderSelectionPage.reset(new TL4RasterFolderSelectionWizardPage(this));
  m_rasterFolderSelectionPage->setTitle(tr("Raster Folder"));
  m_rasterFolderSelectionPage->setSubTitle(tr("Select folder to store raster data"));

  m_finalPage.reset(new TL4FinalPageWizardPage(this));
  m_finalPage->setTitle(tr("Layer Creation"));
  m_finalPage->setSubTitle(tr("Select the layers that you want to create a Project Layer"));
  m_finalPage->setFinalPage(true);

  setPage(PAGE_TERRALIB4_CONNECTOR, m_connectorPage.get());
  setPage(PAGE_LAYER_SELECTION, m_layerSelectionPage.get());
  setPage(PAGE_DATASOURCE_SELECTOR, m_datasourceSelectorPage.get());
  setPage(PAGE_RASTERFOLDER_SELECTOR, m_rasterFolderSelectionPage.get());
  setPage(PAGE_FINALPAGE, m_finalPage.get());

  connect(this->button(QWizard::NextButton), SIGNAL(pressed()), this, SLOT(next()));
  connect(this->button(QWizard::BackButton), SIGNAL(pressed()), this, SLOT(back()));
  connect(this->button(QWizard::HelpButton), SIGNAL(pressed()), this, SLOT(help()));
  connect(this->button(QWizard::FinishButton), SIGNAL(pressed()), this, SLOT(finish()));

  te::qt::widgets::HelpPushButton* helpButton = new te::qt::widgets::HelpPushButton(this);

  this->setButton(QWizard::HelpButton, helpButton);

  helpButton->setPageReference("plugins/terralib4/Terralib4Converter.html");
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
  switch(currentId())
  {
    case PAGE_TERRALIB4_CONNECTOR:
      terralib4ConnectorPageNext();
      break;

    case PAGE_LAYER_SELECTION:
      layerSelectionPageNext();
      break;

    case PAGE_DATASOURCE_SELECTOR:
      datasourceSelectionPageNext();
      break;

    case PAGE_RASTERFOLDER_SELECTOR:
      rasterFolderSelectionPageNext();
      break;
  }
}

void te::qt::plugins::terralib4::TL4ConverterWizard::help()
{
  QMessageBox::warning(this,
                       tr("TerraLib Qt Components"),
                       tr("This option is not implemented yet!\nWe will provide it soon!"));
}

void te::qt::plugins::terralib4::TL4ConverterWizard::terralib4ConnectorPageNext()
{
  std::map<std::string, std::string> connInfo = m_connectorPage->getConnInfo();

  try
  {
    te::qt::widgets::ScopedCursor sc(Qt::WaitCursor);
    m_tl4Database = te::da::DataSourceFactory::make("TERRALIB4");
    m_tl4Database->setConnectionInfo(connInfo);
    m_tl4Database->open();
  }
  catch(te::da::Exception e)
  {
    QMessageBox::warning(this, tr("Warning"), e.what());
    return;
  }
  catch(...)
  {
    QMessageBox::warning(this, tr("Warning"), tr("The Terralib 4.x database connection could not be established."));
    return;
  }

  std::vector<std::string> datasets = m_tl4Database->getDataSetNames();

  m_layerSelectionPage->setDatasets(datasets);

  QWizard::next();
}

void te::qt::plugins::terralib4::TL4ConverterWizard::layerSelectionPageNext()
{
  std::vector<std::string> layersNames;
  layersNames = m_layerSelectionPage->getChecked();

  if(layersNames.size() == 0)
  {
    QMessageBox::warning(this, tr("Warning"), tr("No Layer selected!"));
    return;
  }

  bool hasRaster = false;
  bool hasVector = false;

  for(std::size_t i = 0; i < layersNames.size(); ++i)
  {
    std::auto_ptr<te::da::DataSetType> dst(m_tl4Database->getDataSetType(layersNames[i]));

    if(dst->hasRaster())
      hasRaster = true;
    else
      hasVector = true;
  }

  if(hasRaster)
  {
    QWizardPage* rasterPage = 0;

    rasterPage = this->page(PAGE_RASTERFOLDER_SELECTOR);

    if(!rasterPage)
      this->setPage(PAGE_RASTERFOLDER_SELECTOR, m_rasterFolderSelectionPage.get());
  }
  else
  {
    QWizardPage* rasterPage = 0;

    rasterPage = this->page(PAGE_RASTERFOLDER_SELECTOR);

    if(rasterPage)
      this->removePage(PAGE_RASTERFOLDER_SELECTOR);
  }

  if(hasVector)
  {
    QWizardPage* datasourcePage = 0;
    datasourcePage = this->page(PAGE_DATASOURCE_SELECTOR);
    if(!datasourcePage)
      this->setPage(PAGE_DATASOURCE_SELECTOR, m_datasourceSelectorPage.get());
  }
  else
  {
    QWizardPage* dsPage = 0;
    dsPage = this->page(PAGE_DATASOURCE_SELECTOR);
    if(dsPage)
      this->removePage(PAGE_DATASOURCE_SELECTOR);
  }

  QWizard::next();
}

void te::qt::plugins::terralib4::TL4ConverterWizard::datasourceSelectionPageNext()
{
  std::vector<std::pair<std::string, std::string> > problematicDatasets;

  try
  {
    te::qt::widgets::ScopedCursor sc(Qt::WaitCursor);

// get target data source
    m_targetDataSource = *m_datasourceSelectorPage->getSelectorWidget()->getSelecteds().begin();

    te::da::DataSourcePtr tl5ds = te::da::DataSourceManager::getInstance().get(m_targetDataSource->getId(), m_targetDataSource->getType(), m_targetDataSource->getConnInfo());

    if(!tl5ds->isOpened())
      tl5ds->open();

// check if we have name clash with datasets in the target data source and fill final dataset name index
    std::vector<std::string> dsNames = m_layerSelectionPage->getChecked();

    for(std::size_t i = 0; i != dsNames.size(); ++i)
    {
      std::string targetDatasetName = dsNames[i];
      int j = 0;

      while(tl5ds->dataSetExists(targetDatasetName))
      {
        targetDatasetName += "_" + boost::lexical_cast<std::string>(j);
        ++j;
      }

      m_datasetFinalNames[dsNames[i] ] = targetDatasetName;
    }

// iterate over datasets for converting them to new non-proprietary format.
    for(std::size_t i = 0; i < dsNames.size(); ++i)
    {
      std::string errors = "";

      std::string errorWhat = "";

      try
      {
        std::auto_ptr<te::da::DataSetType> dst = m_tl4Database->getDataSetType(dsNames[i]);

        if(dst->hasRaster())
          continue;

        std::auto_ptr<te::da::DataSetTypeConverter> dt_adapter(new te::da::DataSetTypeConverter(dst.get(), tl5ds->getCapabilities()));

        std::auto_ptr<te::da::DataSet> ds(m_tl4Database->getDataSet(dsNames[i]));

        std::auto_ptr<te::da::DataSetAdapter> ds_adapter(te::da::CreateAdapter(ds.get(), dt_adapter.get()));

        std::map<std::string, std::string> op;

        te::da::Create(tl5ds.get(), dt_adapter->getResult(), ds_adapter.get(), op);

        successfulVectorDatasets.push_back(dsNames[i]);
      }
      catch(const te::common::Exception& e)
      {
        std::pair<std::string, std::string> error;
        error.first = dsNames[i];
        error.second = e.what();
        problematicDatasets.push_back(error);
      }
    }

    m_finalPage->setDataSets(successfulVectorDatasets);
  }
  catch(const te::common::Exception& e)
  {
    QString errMsg(tr("Unexpected error during data conversion: %1"));

    errMsg = errMsg.arg(e.what());

    QMessageBox mes(QMessageBox::Critical, tr("TerraLib 4.x Converter"), errMsg, QMessageBox::Ok, this);

    mes.exec();

    return;
  }

  if(!problematicDatasets.empty())
  {
    QString err(tr("Some TerraLib 4.x Layers could not be converter:\n\n"));
    QString details;

    for(std::size_t i = 0; i < problematicDatasets.size(); ++i)
    {
      err.append(QString(" - ")+problematicDatasets[i].first.c_str()+QString("\n"));
      details.append(problematicDatasets[i].first.c_str() + QString(":\n"));
      details.append(problematicDatasets[i].second.c_str()+QString("\n\n"));
    }

    QMessageBox mes(QMessageBox::Warning, tr("TerraLib 4.x Converter"), err, QMessageBox::Ok, this);
    mes.setDetailedText(details);

    mes.exec();
  }

  QWizard::next();
}

void te::qt::plugins::terralib4::TL4ConverterWizard::rasterFolderSelectionPageNext()
{
  std::vector<std::pair<std::string, std::string> > problematicDatasets;

  {
    te::qt::widgets::ScopedCursor sc(Qt::WaitCursor);

    m_rasterFolderPath = m_rasterFolderSelectionPage->getPath();

    std::vector<std::string> dsNames = m_layerSelectionPage->getChecked();

    for(std::size_t i = 0; i < dsNames.size(); ++i)
    {
      try
      {
        std::auto_ptr<te::da::DataSetType> dst = m_tl4Database->getDataSetType(dsNames[i]);

        if(dst->hasRaster())
        {
          std::auto_ptr<te::da::DataSet> ds = m_tl4Database->getDataSet(dsNames[i]);

          std::auto_ptr<te::rst::Raster> raster = ds->getRaster("Raster");

          te::rst::CreateCopy(*raster.release(), m_rasterFolderPath + "/" + dsNames[i] + ".tif");

          successfulRasterDatasets.push_back(dsNames[i]);
        }
      }
      catch(const te::common::Exception& e)
      {
        std::pair<std::string, std::string> error;
        error.first = dsNames[i];
        error.second = e.what();
        problematicDatasets.push_back(error);
      }
    }

    m_finalPage->setDataSets(successfulRasterDatasets);
  }

  if(!problematicDatasets.empty())
  {
    QString err(tr("Some TerraLib 4.x Layers could not be converter:\n\n"));
    QString details;

    for(std::size_t i = 0; i < problematicDatasets.size(); ++i)
    {
      err.append(QString(" - ")+problematicDatasets[i].first.c_str()+QString("\n"));
      details.append(problematicDatasets[i].first.c_str() + QString(":\n"));
      details.append(problematicDatasets[i].second.c_str()+QString("\n\n"));
    }

    QMessageBox mes(QMessageBox::Warning, tr("TerraLib 4.x Converter"), err, QMessageBox::Ok, this);
    mes.setDetailedText(details);

    mes.exec();
  }
}

void te::qt::plugins::terralib4::TL4ConverterWizard::finish()
{
  std::vector<std::string> selected = m_finalPage->getSelected();

  for(std::size_t i = 0; i < selected.size(); ++i)
  {
    te::map::AbstractLayerPtr lay = 0;
    std::auto_ptr<te::da::DataSetType> dt = m_tl4Database->getDataSetType(selected[i]);

    if(dt->hasRaster())
    {
      std::map<std::string, std::string> connInfo;
      connInfo["URI"] = m_rasterFolderPath + "/" + selected[i] + ".tif";

      lay = te::qt::widgets::createLayer("GDAL", connInfo);
    }
    else
    {
      te::da::DataSourcePtr outDataSource = te::da::DataSourceManager::getInstance().find(m_targetDataSource->getId());

      te::qt::widgets::DataSet2Layer converter(m_targetDataSource->getId());

      std::auto_ptr<te::da::DataSetType> dsType = outDataSource->getDataSetType(selected[i]);
      te::da::DataSetTypePtr dt(dsType.release());
      lay = converter(dt);
    }

    te::qt::af::evt::LayerAdded evt(lay);

    te::qt::af::ApplicationController::getInstance().broadcast(&evt);
  }
}

