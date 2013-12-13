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
#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>

// Qt
#include <QtGui/QAbstractButton>
#include <QtGui/QCursor>
#include <QtGui/QMessageBox>
#include <QtGui/QTableWidgetItem>
#include <QtGui/QVBoxLayout>

te::qt::plugins::terralib4::TL4ConverterWizard::TL4ConverterWizard(QWidget* parent, Qt::WindowFlags f)
  : QWizard(parent, f),
    m_hasNonRaster(false),
    m_hasRaster(false),
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

  m_resolveNamePage.reset(new QWizardPage(this));
  m_resolveNamePage->setTitle(tr("Resolve Name Conflicts"));
  m_resolveNamePage->setSubTitle(tr("Some layer names clashes with target data source names. Please, give a new name for the layers showed below"));
  m_resolveNamePage->setCommitPage(true);

  m_resolveNameTableWidget.reset(new QTableWidget(m_resolveNamePage.get()));
  m_resolveNameTableWidget->setColumnCount(2);
  QStringList labels;
  labels << tr("Source Names") << tr("Target Names");
  m_resolveNameTableWidget->setHorizontalHeaderLabels(labels);

  m_finalPage.reset(new TL4FinalPageWizardPage(this));
  m_finalPage->setTitle(tr("Layer Creation"));
  m_finalPage->setSubTitle(tr("Select the layers that you want to create a Project Layer"));
  m_finalPage->setFinalPage(true);

  setPage(PAGE_TERRALIB4_CONNECTOR, m_connectorPage.get());
  setPage(PAGE_LAYER_SELECTION, m_layerSelectionPage.get());
  setPage(PAGE_DATASOURCE_SELECTOR, m_datasourceSelectorPage.get());
  setPage(PAGE_RASTERFOLDER_SELECTOR, m_rasterFolderSelectionPage.get());
  setPage(PAGE_NAME_RESOLVE_SELECTOR, m_resolveNamePage.get());
  setPage(PAGE_FINALPAGE, m_finalPage.get());

  connect(this->button(QWizard::NextButton), SIGNAL(pressed()), this, SLOT(next()));
  connect(this->button(QWizard::BackButton), SIGNAL(pressed()), this, SLOT(back()));
  connect(this->button(QWizard::FinishButton), SIGNAL(pressed()), this, SLOT(finish()));
  connect(this->button(QWizard::CommitButton), SIGNAL(pressed()), this, SLOT(commit()));

  te::qt::widgets::HelpPushButton* helpButton = new te::qt::widgets::HelpPushButton(this);

  this->setButton(QWizard::HelpButton, helpButton);

  helpButton->setPageReference("plugins/terralib4/Terralib4Converter.html");

  //connect(this->button(QWizard::HelpButton), SIGNAL(pressed()), helpButton, SLOT(help()));
}

te::qt::plugins::terralib4::TL4ConverterWizard::~TL4ConverterWizard()
{
}

int te::qt::plugins::terralib4::TL4ConverterWizard::nextId() const
{
  if(currentId() == PAGE_TERRALIB4_CONNECTOR)
    return PAGE_LAYER_SELECTION;

  if(currentId() == PAGE_LAYER_SELECTION)
  {
    if(m_hasNonRaster)
      return PAGE_DATASOURCE_SELECTOR;

    if(m_hasRaster)
      return PAGE_RASTERFOLDER_SELECTOR;

    return PAGE_LAYER_SELECTION;
  }

  if(currentId() == PAGE_DATASOURCE_SELECTOR)
  {
    if(m_hasRaster)
      return PAGE_RASTERFOLDER_SELECTOR;

    return PAGE_NAME_RESOLVE_SELECTOR;
  }

  if(currentId() == PAGE_RASTERFOLDER_SELECTOR)
    return PAGE_NAME_RESOLVE_SELECTOR;

  if(currentId() == PAGE_NAME_RESOLVE_SELECTOR)
    return PAGE_FINALPAGE;

  return -1;
}

bool te::qt::plugins::terralib4::TL4ConverterWizard::validateCurrentPage()
{
  int current_page_id = currentId();

  if(current_page_id == PAGE_TERRALIB4_CONNECTOR)
  {
    if(!validTerraLib4Connection())
      return false;

    std::vector<std::string> datasets = m_tl4Database->getDataSetNames();

    m_layerSelectionPage->setDatasets(datasets);
  }
  else if(current_page_id == PAGE_LAYER_SELECTION)
  {
    if(!validLayerSelection())
      return false;
  }
  else if(current_page_id == PAGE_DATASOURCE_SELECTOR)
  {
    m_targetDataSource = *m_datasourceSelectorPage->getSelectorWidget()->getSelecteds().begin();
  }
  else if(current_page_id == PAGE_RASTERFOLDER_SELECTOR)
  {
    m_rasterFolderPath = m_rasterFolderSelectionPage->getPath();
  }
  else if(current_page_id == PAGE_NAME_RESOLVE_SELECTOR)
  {
    return validLayerNames();
  }

  if(nextId() == PAGE_NAME_RESOLVE_SELECTOR)
  {
    std::vector<std::string> selectedLayer = m_layerSelectionPage->getChecked();

    m_resolveNameTableWidget->clearContents();
    m_resolveNameTableWidget->setRowCount(selectedLayer.size());
    //m_resolveNameTableWidget->setColumnCount(2);

    te::da::DataSourcePtr tl5ds;

    if(m_hasNonRaster)
      tl5ds = te::da::DataSourceManager::getInstance().get(m_targetDataSource->getId(), m_targetDataSource->getType(), m_targetDataSource->getConnInfo());

    for(std::size_t i = 0; i < selectedLayer.size(); ++i)
    {
      std::string targetDatasetName = selectedLayer[i];

// is it a raster?
      std::auto_ptr<te::da::DataSetType> input_dataset_type(m_tl4Database->getDataSetType(selectedLayer[i]));

      if(input_dataset_type->hasRaster())
      {
      }
      else
      {
// non-raster

// valid dataset name
        int j = 0;

        while(tl5ds->dataSetExists(targetDatasetName))
        {
          targetDatasetName += "_" + boost::lexical_cast<std::string>(j);
          ++j;
        }
      }

      QTableWidgetItem *oldNameItem = new QTableWidgetItem(selectedLayer[i].c_str());
      oldNameItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
      m_resolveNameTableWidget->setItem(i, 0, oldNameItem);

      QTableWidgetItem *newNameItem = new QTableWidgetItem(targetDatasetName.c_str());

      m_resolveNameTableWidget->setItem(i, 1, newNameItem);
    }
  }

  return true;
}

bool te::qt::plugins::terralib4::TL4ConverterWizard::validTerraLib4Connection()
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
    return false;
  }
  catch(...)
  {
    QMessageBox::warning(this, tr("Warning"), tr("A connection to the informed Terralib 4.x database could not be established. Please, verify the informed parameters."));
    return false;
  }

  return true;
}

bool te::qt::plugins::terralib4::TL4ConverterWizard::validLayerSelection()
{
  if(!m_layerSelectionPage->hasChecked())
  {
    // TODO: ao inves de dar esta mensagem deve-se desabilitar o botao next quando nenhum item esta selecionado!
    QMessageBox::warning(this,
                        tr("Warning"),
                        tr("Please, select at least one layer for conversion!"));
    return false;
  }

  std::vector<std::string> layersNames = m_layerSelectionPage->getChecked();

// do we have vector or raster layers?
  m_hasNonRaster = false;
  m_hasRaster = false;

  for(std::size_t i = 0; i < layersNames.size(); ++i)
  {
    std::auto_ptr<te::da::DataSetType> dst(m_tl4Database->getDataSetType(layersNames[i]));

    if(dst->hasRaster())
      m_hasRaster = true;
    else
      m_hasNonRaster = true;
  }

  return true;
}

bool te::qt::plugins::terralib4::TL4ConverterWizard::validLayerNames()
{
// TODO: acrescentar try e catch para evitar problemas
  te::da::DataSourcePtr tl5ds;

  if(m_hasNonRaster)
    tl5ds = te::da::DataSourceManager::getInstance().get(m_targetDataSource->getId(), m_targetDataSource->getType(), m_targetDataSource->getConnInfo());

  int nrows = m_resolveNameTableWidget->rowCount();

  for(int i = 0; i != nrows; ++i)
  {
// get original dataset name
    QTableWidgetItem* item_source = m_resolveNameTableWidget->item(i, 0);

    if(item_source == 0)
      throw te::common::Exception(TE_QT_PLUGIN_TERRALIB4("Invalid source table item!"));

    std::string sourceName = item_source->text().toStdString();

// get target dataset name
    QTableWidgetItem* item_target = m_resolveNameTableWidget->item(i, 1);

    if(item_target == 0)
      throw te::common::Exception(TE_QT_PLUGIN_TERRALIB4("Invalid target table item!"));

    std::string targetName = item_target->text().toStdString();

// ask if the dataset is a raster
    std::auto_ptr<te::da::DataSetType> input_dataset_type(m_tl4Database->getDataSetType(sourceName));

    if(input_dataset_type->hasRaster())
    {
// yes!
    }
    else
    {
// no!
// TODO => avisar o layer que esta com problemas => ja dar o foco para esta linha!!!
      if(tl5ds->dataSetExists(targetName))
        return false;
    }
  }

  return true;
}

void te::qt::plugins::terralib4::TL4ConverterWizard::back()
{
  QWizard::back();
}

void te::qt::plugins::terralib4::TL4ConverterWizard::next()
{
  QWizard::next();
}

void te::qt::plugins::terralib4::TL4ConverterWizard::commit()
{
  return;
}

void te::qt::plugins::terralib4::TL4ConverterWizard::finish()
{
  return;
  //std::vector<std::string> selected = m_finalPage->getSelected();

  //for(std::size_t i = 0; i < selected.size(); ++i)
  //{
  //  te::map::AbstractLayerPtr lay = 0;

  //  std::string originalName = "";
  //  std::map<std::string, std::string>::iterator it = m_datasetFinalNames.begin();
  //  while(it != m_datasetFinalNames.end())
  //  {
  //    if(it->second == selected[i])
  //    {
  //      originalName = it->first;
  //      break;
  //    }
  //    
  //    ++it;
  //  }

  //  std::auto_ptr<te::da::DataSetType> dt = m_tl4Database->getDataSetType(originalName);

  //  if(dt->hasRaster())
  //  {
  //    std::map<std::string, std::string> connInfo;
  //    connInfo["URI"] = m_rasterFolderPath + "/" + selected[i] + ".tif";

  //    lay = te::qt::widgets::createLayer("GDAL", connInfo);
  //  }
  //  else
  //  {
  //    te::da::DataSourcePtr outDataSource = te::da::DataSourceManager::getInstance().find(m_targetDataSource->getId());

  //    te::qt::widgets::DataSet2Layer converter(m_targetDataSource->getId());

  //    std::auto_ptr<te::da::DataSetType> dsType = outDataSource->getDataSetType(selected[i]);

  //    te::da::DataSetTypePtr dt(dsType.release());

  //    lay = converter(dt);
  //  }

  //  te::qt::af::evt::LayerAdded evt(lay);

  //  te::qt::af::ApplicationController::getInstance().broadcast(&evt);
  //}
}

void te::qt::plugins::terralib4::TL4ConverterWizard::help()
{
  QMessageBox::warning(this,
                       tr("TerraLib Qt Components"),
                       tr("This option is not implemented yet!\nWe will provide it soon!"));
}







