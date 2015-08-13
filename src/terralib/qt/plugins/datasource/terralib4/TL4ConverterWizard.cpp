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
  \file terralib/qt/plugins/terralib4/TL4ConverterWizard.cpp

  \brief A wizard for guiding the conversion of a TerraLib 4.x database to a OGC SFS compliant format or any other format known by OGR and GDAL.
*/

// TerraLib
#include "../../../../common/Exception.h"
#include "../../../../common/StringUtils.h"
#include "../../../../common/progress/ProgressManager.h"
#include "../../../../common/progress/TaskProgress.h"
#include "../../../../common/Translator.h"
#include "../../../../dataaccess.h"
#include "../../../../geometry/GeometryProperty.h"
#include "../../../../maptools/DataSetLayer.h"
#include "../../../../qt/widgets/datasource/selector/DataSourceSelectorWidget.h"
#include "../../../../qt/widgets/datasource/selector/DataSourceSelectorWizardPage.h"
#include "../../../../qt/widgets/layer/utils/DataSet2Layer.h"
#include "../../../../qt/widgets/help/HelpPushButton.h"
#include "../../../../qt/widgets/progress/ProgressViewerDialog.h"
#include "../../../../qt/widgets/rp/Utils.h"
#include "../../../../qt/widgets/utils/ScopedCursor.h"
#include "../../../../qt/af/ApplicationController.h"
#include "../../../../qt/af/Project.h"
#include "../../../../qt/af/events/LayerEvents.h"
#include "../../../../raster/Utils.h"
#include "../../../../terralib4/DataSource.h"
#include "../../../../terralib4/ThemeInfo.h"
#include "../../../../se/ChannelSelection.h"
#include "../../../../se/ColorMap.h"
#include "../../../../se/FeatureTypeStyle.h"
#include "../../../../se/Fill.h"
#include "../../../../se/PolygonSymbolizer.h"
#include "../../../../se/RasterSymbolizer.h"
#include "../../../../se/Rule.h"
#include "../../../../se/SelectedChannel.h"
#include "../../../../se/Stroke.h"
#include "../../../../se/Style.h"
#include "../../../../se/Symbolizer.h"
#include "../../../../se/Utils.h"
#include "TL4ConverterWizard.h"
#include "TL4ConnectorWizardPage.h"
#include "TL4LayerSelectionWizardPage.h"
#include "TL4RasterFolderSelectionWizardPage.h"
#include "TL4FinalPageWizardPage.h"
#include "TL4ThemeSelectionWizardPage.h"
#include "Utils.h"
#include "ui_TL4ConverterWizardForm.h"
#include "ui_TL4ConnectorWizardPageForm.h"
#include "ui_TL4LayerSelectionWizardPageForm.h"
#include "ui_TL4RasterFolderSelectionWizardPageForm.h"
#include "ui_TL4FinalPageWizardPageForm.h"
#include "ui_TL4ThemeSelectionWizardPageForm.h"

// STL
#include <cassert>

// Boost
#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>

// Qt
#include <QIcon>
#include <QMessageBox>
#include <QTableWidgetItem>
#include <QGridLayout>

// TerraLib 4
#include <terralib4/kernel/TeLegendEntry.h>
#include <terralib4/kernel/TeRasterTransform.h>
#include <terralib4/kernel/TeTheme.h>

te::qt::plugins::terralib4::TL4ConverterWizard::TL4ConverterWizard(QWidget* parent, Qt::WindowFlags f)
  : QWizard(parent, f),
    m_hasNonRaster(false),
    m_hasRaster(false),
    m_rollback(false),
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
  m_rasterFolderSelectionPage->setTitle(tr("Target Raster Folder"));
  m_rasterFolderSelectionPage->setSubTitle(tr("Select folder to store raster data"));

  m_resolveNamePage.reset(new QWizardPage(this));
  m_resolveNamePage->setTitle(tr("Resolve Name Conflicts"));
  m_resolveNamePage->setSubTitle(tr("Some layer names clashes with target data source names. Please, give a new name for the layers showed below"));
  m_resolveNamePage->setCommitPage(true);

  QGridLayout* displayLayout = new QGridLayout(m_resolveNamePage.get());
  m_resolveNameTableWidget.reset(new QTableWidget(m_resolveNamePage.get()));
  displayLayout->addWidget(m_resolveNameTableWidget.get());
  displayLayout->setContentsMargins(0,0,0,0);
  m_resolveNameTableWidget->setColumnCount(3);
  QStringList labels;
  labels << "" << tr("Source Names") << tr("Target Names");
  m_resolveNameTableWidget->setHorizontalHeaderLabels(labels);

  m_finalPage.reset(new TL4FinalPageWizardPage(this));
  m_finalPage->setTitle(tr("Layer Creation"));
  m_finalPage->setSubTitle(tr("Select the TerraLib 4.x Layer converted that you want to create a TerraLib 5 Project Layer."));

  m_themeSelection.reset(new TL4ThemeSelectionWizardPage(this));
  m_themeSelection->setTitle(tr("Theme Creation"));
  m_themeSelection->setSubTitle(tr("Select the TerraLib 4.x Theme of converted Layers that you want to create a TerraLib 5 Project Layer, with informations like Visual and Grouping."));
  m_themeSelection->setFinalPage(true);

  setPage(PAGE_TERRALIB4_CONNECTOR, m_connectorPage.get());
  setPage(PAGE_LAYER_SELECTION, m_layerSelectionPage.get());
  setPage(PAGE_DATASOURCE_SELECTOR, m_datasourceSelectorPage.get());
  setPage(PAGE_RASTERFOLDER_SELECTOR, m_rasterFolderSelectionPage.get());
  setPage(PAGE_NAME_RESOLVE_SELECTOR, m_resolveNamePage.get());
  setPage(PAGE_FINALPAGE, m_finalPage.get());
  setPage(PAGE_THEME_SELECTION, m_themeSelection.get());

  connect(this->button(QWizard::NextButton), SIGNAL(pressed()), this, SLOT(next()));
  connect(this->button(QWizard::BackButton), SIGNAL(pressed()), this, SLOT(back()));
  connect(this->button(QWizard::FinishButton), SIGNAL(pressed()), this, SLOT(finish()));
  connect(this->button(QWizard::CommitButton), SIGNAL(pressed()), this, SLOT(commit()));

  te::qt::widgets::HelpPushButton* helpButton = new te::qt::widgets::HelpPushButton(this);

  this->setButton(QWizard::HelpButton, helpButton);

  helpButton->setPageReference("plugins/terralib4/Terralib4Converter.html");
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

  if(currentId() == PAGE_FINALPAGE)
    return PAGE_THEME_SELECTION;

  return -1;
}

bool te::qt::plugins::terralib4::TL4ConverterWizard::validateCurrentPage()
{
  int current_page_id = currentId();

  if(current_page_id == PAGE_TERRALIB4_CONNECTOR)
  {
    if(!validTerraLib4Connection())
      return false;

    ::terralib4::DataSource* tl4Ds = dynamic_cast<::terralib4::DataSource*>(m_tl4Database.get());

    std::vector<std::string> layers = tl4Ds->getTL4Layers();
    std::vector<std::string> rasters = tl4Ds->getTL4Rasters();
    std::vector<std::string> tables = tl4Ds->getTL4Tables();

    m_layerSelectionPage->setDatasets(layers, tables, rasters);

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
    return !m_rollback;
  }

  try
  {
    if(nextId() == PAGE_NAME_RESOLVE_SELECTOR)
    {
      te::qt::widgets::ScopedCursor sc(Qt::WaitCursor);

      std::vector<QListWidgetItem*> selectedLayerItems = m_layerSelectionPage->getCheckedItems();

      m_resolveNameTableWidget->clearContents();
      m_resolveNameTableWidget->setRowCount((int)selectedLayerItems.size());

      te::da::DataSourcePtr tl5ds;

      if(m_hasNonRaster)
        tl5ds = te::da::DataSourceManager::getInstance().get(m_targetDataSource->getId(), m_targetDataSource->getType(), m_targetDataSource->getConnInfo());

      bool hasConflicts = false;

      for(std::size_t i = 0; i < selectedLayerItems.size(); ++i)
      {
        std::string targetDatasetName = selectedLayerItems[i]->text().toStdString();

  // is it a raster?
        std::auto_ptr<te::da::DataSetType> input_dataset_type(m_tl4Database->getDataSetType(selectedLayerItems[i]->text().toStdString()));

        if(input_dataset_type->hasRaster())
        {
          QTableWidgetItem *conflictItem = 0;
        
          if(boost::filesystem::exists(m_rasterFolderPath + "/" + targetDatasetName + ".tif"))
          {
            hasConflicts = true;

            conflictItem = new QTableWidgetItem(QIcon::fromTheme("delete"), "");
          }
          else
          {
            conflictItem = new QTableWidgetItem(QIcon::fromTheme("check"), "");
          }

          conflictItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
          m_resolveNameTableWidget->setItem((int)i, 0, conflictItem);
        }
        else
        {
  // non-raster
          QTableWidgetItem *conflictItem = 0;

          std::string targetDatasetNameAux = targetDatasetName;

          if(tl5ds->getType() == "POSTGIS")
          {
            targetDatasetNameAux = te::common::Convert2LCase(targetDatasetName);
          }

          if(tl5ds->dataSetExists(targetDatasetNameAux))
          {
            hasConflicts = true;

            conflictItem = new QTableWidgetItem(QIcon::fromTheme("delete"), "");
          }
          else
          {
            conflictItem = new QTableWidgetItem(QIcon::fromTheme("check"), "");
          }

          conflictItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
          m_resolveNameTableWidget->setItem((int)i, 0, conflictItem);
        }

        QTableWidgetItem *oldNameItem = new QTableWidgetItem(selectedLayerItems[i]->text(), selectedLayerItems[i]->type());
        oldNameItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        m_resolveNameTableWidget->setItem((int)i, 1, oldNameItem);

        QTableWidgetItem *newNameItem = new QTableWidgetItem(targetDatasetName.c_str(), selectedLayerItems[i]->type());
        m_resolveNameTableWidget->setItem((int)i, 2, newNameItem);
      }

      m_resolveNameTableWidget->resizeColumnsToContents();
      m_resolveNameTableWidget->sortByColumn(1, Qt::AscendingOrder);

      if(hasConflicts)
      {
        m_resolveNamePage->setTitle(tr("Resolve Name Conflicts"));
        m_resolveNamePage->setSubTitle(tr("Some layer names clash with target data source dataset names. Please, give a new name for the layers showed below"));
      }
      else
      {
        m_resolveNamePage->setTitle(tr("No Name Conflicts"));
        m_resolveNamePage->setSubTitle(tr("You can change the layer names in the target data source"));
      }
    }
  }
  catch(const te::da::Exception& e)
  {
    QMessageBox::warning(this, tr("Warning"), e.what());
    return false;
  }
  catch(...)
  {
    QMessageBox::warning(this, tr("Warning"), tr("Could not valid this page!"));
    return false;
  }

  return true;
}

bool te::qt::plugins::terralib4::TL4ConverterWizard::validTerraLib4Connection()
{
  setCursor(Qt::WaitCursor);

  std::map<std::string, std::string> connInfo = m_connectorPage->getConnInfo();

  try
  {
    te::qt::widgets::ScopedCursor sc(Qt::WaitCursor);
    m_tl4Database = te::da::DataSourceFactory::make("TERRALIB4");
    m_tl4Database->setConnectionInfo(connInfo);
    m_tl4Database->open();
  }
  catch(const te::da::Exception& e)
  {
    setCursor(Qt::ArrowCursor);
    QMessageBox::warning(this, tr("Warning"), e.what());
    return false;
  }
  catch(...)
  {
    setCursor(Qt::ArrowCursor);
    QMessageBox::warning(this, tr("Warning"), tr("A connection to the informed Terralib 4.x database could not be established. Please, verify the informed parameters."));
    return false;
  }
  setCursor(Qt::ArrowCursor);

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

  try
  {
    te::qt::widgets::ScopedCursor sc(Qt::WaitCursor);

    for(std::size_t i = 0; i < layersNames.size(); ++i)
    {
      std::auto_ptr<te::da::DataSetType> dst(m_tl4Database->getDataSetType(layersNames[i]));

      if(dst->hasRaster())
        m_hasRaster = true;
      else
        m_hasNonRaster = true;
    }
  }
  catch(const te::da::Exception& e)
  {
    QMessageBox::warning(this, tr("Warning"), e.what());
    return false;
  }
  catch(...)
  {
    QMessageBox::warning(this, tr("Warning"), tr("Could not valid layer selection!"));
    return false;
  }

  return true;
}

bool te::qt::plugins::terralib4::TL4ConverterWizard::validLayerNames()
{
  bool hasConflict = false;

// TODO: acrescentar try e catch para evitar problemas
  te::da::DataSourcePtr tl5ds;

  try
  {
    if(m_hasNonRaster)
      tl5ds = te::da::DataSourceManager::getInstance().get(m_targetDataSource->getId(), m_targetDataSource->getType(), m_targetDataSource->getConnInfo());
  }
  catch(const te::da::Exception& e)
  {
    QMessageBox::warning(this, tr("Warning"), e.what());
    return false;
  }
  catch(...)
  {
    QMessageBox::warning(this, tr("Warning"), tr("Could not connect to TerraLib 5 data source!"));
    return false;
  }

  int nrows = m_resolveNameTableWidget->rowCount();

  for(int i = 0; i != nrows; ++i)
  {
// get original dataset name
    QTableWidgetItem* item_source = m_resolveNameTableWidget->item(i, 1);

    if(item_source == 0)
      throw te::common::Exception(TE_TR("Invalid source table item!"));

    std::string sourceName = item_source->text().toStdString();

// get target dataset name
    QTableWidgetItem* item_target = m_resolveNameTableWidget->item(i, 2);

    if(item_target == 0)
      throw te::common::Exception(TE_TR("Invalid target table item!"));

    std::string targetName = item_target->text().toStdString();

// ask if the dataset is a raster
    try
    {
      te::qt::widgets::ScopedCursor sc(Qt::WaitCursor);

      std::auto_ptr<te::da::DataSetType> input_dataset_type(m_tl4Database->getDataSetType(sourceName));

      if(input_dataset_type->hasRaster())
      {
// yes!
        if(boost::filesystem::exists(m_rasterFolderPath + "/" + targetName + ".tif"))
        {
          hasConflict = true;
        }
        else
        {
          QTableWidgetItem *nonconflictItem = new QTableWidgetItem(QIcon::fromTheme("check"), "");

          m_resolveNameTableWidget->setItem(i, 0, nonconflictItem);
        }
      }
      else
      {
// no!
        std::string targetDatasetNameAux = targetName;

        if(tl5ds->getType() == "POSTGIS")
        {
          targetDatasetNameAux = te::common::Convert2LCase(targetName);
        }

        if(tl5ds->dataSetExists(targetDatasetNameAux))
        {
          hasConflict = true;
        }
        else
        {
          QTableWidgetItem *nonconflictItem = new QTableWidgetItem(QIcon::fromTheme("check"), "");

          m_resolveNameTableWidget->setItem(i, 0, nonconflictItem);
        }
      }
    }
    catch(const te::da::Exception& e)
    {
      QMessageBox::warning(this, tr("Warning"), e.what());
      return false;
    }
    catch(...)
    {
      QMessageBox::warning(this, tr("Warning"), tr("Could not valid layer names in target data source!"));
      return false;
    }
  }

  if(hasConflict)
  {
    QString errMsg(tr("There still have name conflicts. Please, resolve the indicated conflicts before continue!"));

    QMessageBox::warning(this, tr("TerraLib 4.x Converter"), errMsg);

    return false;
  }

  return true;
}

std::string te::qt::plugins::terralib4::TL4ConverterWizard::getOriginalName(const std::string& targetName)
{
  int rowCount = m_resolveNameTableWidget->rowCount();

  for(int i = 0; i < rowCount; ++i)
  {
    QString targetNameInTable = m_resolveNameTableWidget->item(i, 2)->text();

    std::string aux = targetNameInTable.toLatin1();

    if(targetName.c_str() == aux)
      return aux;
  }

  return "";
}

std::string te::qt::plugins::terralib4::TL4ConverterWizard::getNewName(const std::string& originalName)
{
  int rowCount = m_resolveNameTableWidget->rowCount();

  for(int i = 0; i < rowCount; ++i)
  {
    QString oName = m_resolveNameTableWidget->item(i, 1)->text();

    std::string aux = oName.toLatin1();

    if(originalName.c_str() == aux)
      return aux;
  }

  return "";
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
  if(!validLayerNames())
  {
    m_rollback = true;
    return;
  }

// validation successful => convert the source layers!
  std::vector<std::pair<std::string, std::string> > problematicDatasets;
  std::vector<std::string> successfulDatasets;

  te::da::DataSourcePtr tl5ds;

  try
  {
    if(m_hasNonRaster)
      tl5ds = te::da::DataSourceManager::getInstance().get(m_targetDataSource->getId(), m_targetDataSource->getType(), m_targetDataSource->getConnInfo());
  }
  catch(const te::da::Exception& e)
  {
    m_rollback = true;
    QMessageBox::warning(this, tr("Warning"), e.what());
    return;
  }
  catch(...)
  {
    m_rollback = true;
    QMessageBox::warning(this, tr("Warning"), tr("Could not connect to TerraLib 5 data source!"));
    return;
  }

  int nrows = m_resolveNameTableWidget->rowCount();

  //progress
  te::qt::widgets::ProgressViewerDialog v(this);
  int id = te::common::ProgressManager::getInstance().addViewer(&v);

  te::common::TaskProgress task("TerraLib4 Converter...");
  task.setTotalSteps(nrows);
  task.useTimer(true);

  for(int i = 0; i != nrows; ++i)
  {
// get original dataset name
    QTableWidgetItem* item_source = m_resolveNameTableWidget->item(i, 1);

    if(item_source == 0)
    {
      te::common::ProgressManager::getInstance().removeViewer(id);
      throw te::common::Exception(TE_TR("Invalid source table item!"));
    }

    std::string sourceName = item_source->text().toLatin1();

// get target dataset name
    QTableWidgetItem* item_target = m_resolveNameTableWidget->item(i, 2);

    if(item_target == 0)
    {
      te::common::ProgressManager::getInstance().removeViewer(id);
      throw te::common::Exception(TE_TR("Invalid target table item!"));
    }

    std::string targetName = item_target->text().toLatin1();

// ask if the dataset is a raster
    try
    {
      te::qt::widgets::ScopedCursor sc(Qt::WaitCursor);

      std::auto_ptr<te::da::DataSetType> input_dataset_type(m_tl4Database->getDataSetType(sourceName));

      if(input_dataset_type->hasRaster())
      {
// yes!
        std::auto_ptr<te::da::DataSet> ds = m_tl4Database->getDataSet(sourceName);

        std::auto_ptr<te::rst::Raster> raster = ds->getRaster("Raster");

        std::string newName = m_rasterFolderPath + "/" + targetName + ".tif";

        te::rst::CreateCopy(*raster.release(), newName);

        successfulDatasets.push_back(targetName);
      }
      else
      {
// no!
        input_dataset_type->setName(targetName);

        std::auto_ptr<te::da::DataSetTypeConverter> dt_adapter(new te::da::DataSetTypeConverter(input_dataset_type.get(), tl5ds->getCapabilities()));

        std::auto_ptr<te::da::DataSet> ds(m_tl4Database->getDataSet(sourceName));

        std::auto_ptr<te::da::DataSetAdapter> ds_adapter(new te::da::DataSetAdapter(ds.get()));//te::da::CreateAdapter(ds.get(), dt_adapter.get()));

        const std::vector<std::vector<std::size_t> >& indexes = dt_adapter->getConvertedPropertyIndexes();
        const std::vector<te::da::AttributeConverter>& funcs = dt_adapter->getConverters();

        te::da::DataSetType* type = dt_adapter->getResult();

        te::da::PrimaryKey* pk = 0;
        pk = type->getPrimaryKey();

        if(pk)
        {
          pk->setName(te::common::Convert2LCase(targetName) + "_pk");
        }

        te::common::CharEncoding encTo = tl5ds->getEncoding();

        for(std::size_t i = 0; i < type->size(); ++i)
        {
          te::dt::Property* p = type->getProperty(i);

          if(p->getType() == te::dt::STRING_TYPE)
          {
            te::da::CharEncodingConverter conversor(encTo);
            ds_adapter->add(p->getName(), p->getType(), indexes[i], conversor);
          }
          else
          {
            ds_adapter->add(p->getName(), p->getType(), indexes[i], funcs[i]);
          }
        }

        ::terralib4::DataSource* tl4Ds = dynamic_cast<::terralib4::DataSource*>(m_tl4Database.get());

        int finalSrid = tl4Ds->getLayerSRID(sourceName);

        te::da::AssociateDataSetTypeConverterSRID(dt_adapter.get(), finalSrid);

        std::map<std::string, std::string> opt;

        ds_adapter->moveBeforeFirst();

        te::da::Create(tl5ds.get(), dt_adapter->getResult(), ds_adapter.get(), opt);

        successfulDatasets.push_back(targetName);
      }
    }
    catch(const te::common::Exception& e)
    {
      std::pair<std::string, std::string> dproblem;
      dproblem.first = sourceName;
      dproblem.second = e.what();
      
      problematicDatasets.push_back(dproblem);
    }
    catch(std::exception& e)
    {
      std::pair<std::string, std::string> dproblem;
      dproblem.first = sourceName;
      dproblem.second = e.what();
      
      problematicDatasets.push_back(dproblem);
    }
    catch(...)
    {
      std::pair<std::string, std::string> dproblem;
      dproblem.first = sourceName;
      dproblem.second = TE_TR("unknown problem in conversion!");
      
      problematicDatasets.push_back(dproblem);
    }

    if (task.isActive() == false)
    {
      te::common::ProgressManager::getInstance().removeViewer(id);
      throw te::common::Exception(TE_TR("Operation canceled!"));
    }

    task.pulse();
  }

// give a warning
  if(!problematicDatasets.empty())
  {
    QString error(tr("Some TerraLib 4.x Layers could not be converted: \n\n"));
    QString details;

    for(std::size_t i = 0; i < problematicDatasets.size(); ++i)
    {
      error.append(QString(" - ") + problematicDatasets[i].first.c_str() + QString(""));
      details.append(problematicDatasets[i].first.c_str() + QString(":\n"));
      details.append(problematicDatasets[i].second.c_str() + QString("\n\n"));
    }

    QMessageBox message(QMessageBox::Warning, tr("TerraLib 4.x Converter"), error, QMessageBox::Ok, this);
    message.setDetailedText(details);

    message.exec();
  }

// fill next page!
  ::terralib4::DataSource* tl4Ds = dynamic_cast<::terralib4::DataSource*>(m_tl4Database.get());

  m_finalPage->setDataSets(successfulDatasets);

  std::vector<::terralib4::ThemeInfo> themes = tl4Ds->getTL4Themes();

  std::vector<::terralib4::ThemeInfo> convertedThemes;
  for(std::size_t i = 0; i < themes.size(); ++i)
  {
    for(std::size_t j = 0; j < successfulDatasets.size(); ++j)
    {
      if(themes[i].m_layerName == getOriginalName(successfulDatasets[j]))
      {
        convertedThemes.push_back(themes[i]);
      }
    }
  }

  m_themeSelection->setThemes(convertedThemes);

  m_rollback = false;

  te::common::ProgressManager::getInstance().removeViewer(id);
}

void te::qt::plugins::terralib4::TL4ConverterWizard::finish()
{
  std::vector<std::string> selected = m_finalPage->getSelected();

  te::da::DataSourcePtr outDataSource;

  try
  {
    if(m_hasNonRaster)
      outDataSource = te::da::DataSourceManager::getInstance().find(m_targetDataSource->getId());
  }
  catch(const te::da::Exception& e)
  {
    m_rollback = true;
    QMessageBox::warning(this, tr("Warning"), e.what());
    return;
  }
  catch(...)
  {
    m_rollback = true;
    QMessageBox::warning(this, tr("Warning"), tr("Could not connect to TerraLib 5 data source!"));
    return;
  }

  try
  {
    //te::qt::widgets::ScopedCursor sc(Qt::WaitCursor);
    setCursor(Qt::WaitCursor);

    for(std::size_t i = 0; i < selected.size(); ++i)
    {
      te::map::AbstractLayerPtr layer = 0;

      std::auto_ptr<te::da::DataSetType> sourceDt = m_tl4Database->getDataSetType(getOriginalName(selected[i]));

      if(sourceDt->hasRaster())
      {
        std::map<std::string, std::string> connInfo;
        connInfo["URI"] = m_rasterFolderPath + "/" + selected[i] + ".tif";

        layer = te::qt::widgets::createLayer("GDAL", connInfo);
      }
      else
      {
        te::qt::widgets::DataSet2Layer converter(m_targetDataSource->getId());

        std::auto_ptr<te::da::DataSetType> dsType = outDataSource->getDataSetType(selected[i]);

        te::da::DataSetTypePtr dt(dsType.release());

        layer = converter(dt);
      }

      te::qt::af::evt::LayerAdded evt(layer);

      te::qt::af::ApplicationController::getInstance().broadcast(&evt);
    }

    std::vector<::terralib4::ThemeInfo> themes = m_themeSelection->getThemes();

    for(std::size_t i = 0; i < themes.size(); ++i)
    {
      te::map::AbstractLayerPtr layer = 0;

      std::string newName = getNewName(themes[i].m_layerName);

      std::auto_ptr<te::da::DataSetType> sourceDt = m_tl4Database->getDataSetType(newName);

      ::terralib4::DataSource* tl4Ds = dynamic_cast<::terralib4::DataSource*>(m_tl4Database.get());

      TeTheme* theme = tl4Ds->getTL4Theme(themes[i]);

      te::se::Style* style = 0;

      if(sourceDt->hasRaster())
      {
        std::map<std::string, std::string> connInfo;
        connInfo["URI"] = m_rasterFolderPath + "/" + themes[i].m_name + ".tif";

        layer = te::qt::widgets::createLayer("GDAL", connInfo);

        style = Convert2TerraLib5(0, theme, true);

        te::se::RasterSymbolizer* symb = te::se::GetRasterSymbolizer(style);

        if(theme->grouping().groupMode_ != TeNoGrouping)
        {
          te::se::ColorMap* cm = 0;

          cm = GetRasterGrouping(theme);

          symb->setColorMap(cm);

          std::string band = "0";

          te::se::ChannelSelection* cs = symb->getChannelSelection();

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
      }
      else
      {
        std::auto_ptr<te::da::DataSetType> dst = outDataSource->getDataSetType(getNewName(themes[i].m_layerName));

        te::qt::widgets::DataSet2Layer converter(m_targetDataSource->getId());

        te::da::DataSetTypePtr dstPtr(dst.release());

        layer = converter(dstPtr);
        layer->setTitle(themes[i].m_name);

        // Get Style
        te::gm::GeometryProperty* geomProp = te::da::GetFirstGeomProperty(dstPtr.get());

        style = Convert2TerraLib5(geomProp->getGeometryType(), theme);

        if(theme->grouping().groupMode_ != TeNoGrouping)
          layer->setGrouping(GetGrouping(theme));
      }

      layer->setStyle(style->clone());

      delete style;

      te::qt::af::evt::LayerAdded evt(layer);

      te::qt::af::ApplicationController::getInstance().broadcast(&evt);
    }
  }
  catch(const te::da::Exception& e)
  {
    setCursor(Qt::ArrowCursor);
    m_rollback = true;
    QMessageBox::warning(this, tr("Warning"), e.what());
    return;
  }
  catch(...)
  {
    setCursor(Qt::ArrowCursor);
    m_rollback = true;
    QMessageBox::warning(this, tr("Warning"), tr("Automatic layer creation failed!"));
    return;
  }
}

void te::qt::plugins::terralib4::TL4ConverterWizard::help()
{
  QMessageBox::warning(this,
                       tr("TerraLib 4.x Converter"),
                       tr("This option is not implemented yet!\nWe will provide it soon!"));
}







