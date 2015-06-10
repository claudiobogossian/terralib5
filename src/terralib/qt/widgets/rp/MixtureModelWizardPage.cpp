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
  \file terralib/qt/widgets/rp/MixtureModelWizardPage.cpp

  \brief This file defines a class for a Mixture Model Wizard page.
*/

// TerraLib
#include "../../../common/STLUtils.h"
#include "../../../dataaccess/dataset/DataSet.h"
#include "../../../dataaccess/utils/Utils.h"
#include "../../../geometry/Point.h"
#include "../../../maptools/MarkRendererManager.h"
#include "../../../raster/Raster.h"
#include "../../../rp/MixtureModel.h"
#include "../../../rp/MixtureModelLinearStrategy.h"
#include "../../../rp/MixtureModelPCAStrategy.h"
#include "../../../rp/Functions.h"
#include "../../../se/Fill.h"
#include "../../../se/Mark.h"
#include "../../../se/Stroke.h"
#include "../../../se/Utils.h"
#include "../canvas/Canvas.h"
#include "../canvas/MapDisplay.h"
#include "MixtureModelWizardPage.h"
#include "RasterNavigatorWidget.h"
#include "RasterNavigatorDialog.h"
#include "ui_MixtureModelWizardPageForm.h"

// Qt
#include <QFileDialog>
#include <QGridLayout>
#include <QMessageBox>

// Boost
#include <boost/foreach.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>

//STL
#include <fstream>
#include <memory>

#define PATTERN_SIZE 12

te::qt::widgets::MixtureModelWizardPage::MixtureModelWizardPage(QWidget* parent)
  : QWizardPage(parent),
    m_ui(new Ui::MixtureModelWizardPageForm),
    m_countComponents(0),
    m_layer(0)
{
// setup controls
  m_ui->setupUi(this);

  m_ui->m_loadToolButton->setIcon(QIcon::fromTheme("document-open"));
  m_ui->m_saveToolButton->setIcon(QIcon::fromTheme("document-save"));

  fillMixtureModelTypes();

//define mark
  te::se::Stroke* stroke = te::se::CreateStroke("#000000", "1");
  te::se::Fill* fill = te::se::CreateFill("#FF0000", "1.0");
  m_mark = te::se::CreateMark("cross", stroke, fill);

  m_rgbaMark = te::map::MarkRendererManager::getInstance().render(m_mark, PATTERN_SIZE);

//connects
  connect(m_ui->m_tableWidget, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(onItemChanged(QTableWidgetItem*)));
  connect(m_ui->m_removeToolButton, SIGNAL(clicked()), this, SLOT(onRemoveToolButtonClicked()));
  connect(m_ui->m_acquireToolButton, SIGNAL(toggled(bool)), this, SLOT(showNavigator(bool)));
  connect(m_ui->m_loadToolButton, SIGNAL(clicked()), this, SLOT(onLoadToolButtonClicked()));
  connect(m_ui->m_saveToolButton, SIGNAL(clicked()), this, SLOT(onSaveToolButtonClicked()));


  //configure raster navigator
  m_navigatorDlg.reset(new te::qt::widgets::RasterNavigatorDialog(this, Qt::Tool));
  m_navigatorDlg->setWindowTitle(tr("Display"));
  m_navigatorDlg->setMinimumSize(550, 400);
  m_navigatorDlg->getWidget()->hideGeomTool(true);
  m_navigatorDlg->getWidget()->hideInfoTool(true);
  m_navigatorDlg->getWidget()->hideBoxTool(true);

  //connects
  connect(m_navigatorDlg.get(), SIGNAL(navigatorClosed()), this, SLOT(onNavigatorClosed()));
  connect(m_navigatorDlg->getWidget(), SIGNAL(mapDisplayExtentChanged()), this, SLOT(onMapDisplayExtentChanged()));
  connect(m_navigatorDlg->getWidget(), SIGNAL(pointPicked(double, double)), this, SLOT(onPointPicked(double, double)));

// configure page
  this->setTitle(tr("Mixture Model"));
  this->setSubTitle(tr("Select the type of mixture model and set their specific parameters."));

  m_ui->m_removeToolButton->setIcon(QIcon::fromTheme("list-remove"));
  m_ui->m_acquireToolButton->setIcon(QIcon::fromTheme("wand"));
}

te::qt::widgets::MixtureModelWizardPage::~MixtureModelWizardPage()
{
  m_components.clear();

  te::common::Free(m_rgbaMark, PATTERN_SIZE);

  delete m_mark;
}

bool te::qt::widgets::MixtureModelWizardPage::isComplete() const
{
  if(m_ui->m_tableWidget->rowCount() == 0)
    return false;

  int nBands = m_ui->m_bandTableWidget->rowCount();

  bool isChecked = false;

  std::size_t count = 0;
  for(int i = 0; i < nBands; ++i)
  {
    QCheckBox* checkBox = (QCheckBox*)m_ui->m_bandTableWidget->cellWidget(i, 0);
    
    if(checkBox->isChecked())
    {
      isChecked = true;
      ++count;
    }
  }

  if(!isChecked)
    return false;

  if(count != m_components.size())
    return false;

  return true;
}

void te::qt::widgets::MixtureModelWizardPage::set(te::map::AbstractLayerPtr layer)
{
  m_layer = layer;

  m_navigatorDlg->set(m_layer);

  listBands();
}

te::map::AbstractLayerPtr te::qt::widgets::MixtureModelWizardPage::get()
{
  return m_layer;
}

te::rp::MixtureModel::InputParameters te::qt::widgets::MixtureModelWizardPage::getInputParams()
{
  int idx = m_ui->m_typeComboBox->currentIndex();
  int type = m_ui->m_typeComboBox->itemData(idx).toInt();

  te::rp::MixtureModel::InputParameters algoInputParams;

  if(type == MIXMODEL_LINEAR)
  {
    algoInputParams.m_strategyName = "linear";

    te::rp::MixtureModelLinearStrategy::Parameters specificParameters;
    algoInputParams.setMixtureModelStrategyParams(specificParameters);
  }
  else if(type == MIXMODEL_PCA)
  {
    algoInputParams.m_strategyName = "pca";

    te::rp::MixtureModelPCAStrategy::Parameters specificParameters;
    algoInputParams.setMixtureModelStrategyParams(specificParameters);
  }

// insert selected bands
  unsigned selectedBands = 0;
  std::vector<bool> selectedBandsVector;

  for (int i = 0; i < m_ui->m_bandTableWidget->rowCount(); ++i)
  {
    QCheckBox* bandCheckBox = (QCheckBox*) m_ui->m_bandTableWidget->cellWidget(i, 0);

    if (bandCheckBox->isChecked())
    {
      selectedBandsVector.push_back(true);
      selectedBands++;

      algoInputParams.m_inputRasterBands.push_back(i);

      QComboBox *sensorComboBox = (QComboBox*) m_ui->m_bandTableWidget->cellWidget(i, 1);
      algoInputParams.m_inputSensorBands.push_back(std::string(sensorComboBox->currentText().toStdString()));
    }
    else
      selectedBandsVector.push_back(false);
  }

// insert mixture components
  std::map<std::string, MixModelComponent>::iterator it = m_components.begin();
  std::vector<double> components;

  while(it != m_components.end())
  {
    components.clear();

    for (unsigned int i = 0; i < selectedBandsVector.size(); i++)
      if (selectedBandsVector[i])
        components.push_back(it->second.m_values[i]);

    algoInputParams.m_components[it->second.m_name] = components;

    ++it;
  }


  return algoInputParams;
}

te::rp::MixtureModel::OutputParameters te::qt::widgets::MixtureModelWizardPage::getOutputParams()
{
  te::rp::MixtureModel::OutputParameters algoOutputParams;

  algoOutputParams.m_normalizeOutput = m_ui->m_normalizeOutputCheckBox->isChecked();
  algoOutputParams.m_createErrorRaster = m_ui->m_createErrorRasterCheckBox->isChecked();

  return algoOutputParams;
}

void te::qt::widgets::MixtureModelWizardPage::saveMixtureModelComponents(std::string fileName)
{
  boost::property_tree::ptree pt;

  std::map<std::string, MixModelComponent >::iterator it = m_components.begin();

  boost::property_tree::ptree children;

  while(it != m_components.end())
  {
    boost::property_tree::ptree child;

    child.put("id", it->first);
    child.put("name", it->second.m_name);

    boost::property_tree::ptree coordGrid;
    coordGrid.put("xGrid", it->second.m_coordGrid.getX());
    coordGrid.put("yGrid", it->second.m_coordGrid.getY());
    child.push_back(std::make_pair("coordGrid", coordGrid));

    boost::property_tree::ptree coordGeo;
    coordGeo.put("xGeo", it->second.m_coordGeo.getX());
    coordGeo.put("yGeo", it->second.m_coordGeo.getY());
    child.push_back(std::make_pair("coordGeo", coordGeo));

    boost::property_tree::ptree values;

    for(std::size_t t = 0; t < it->second.m_values.size(); ++t)
    {
      boost::property_tree::ptree val;
      val.put("value", it->second.m_values[t]);
      values.push_back(std::make_pair("", val));
    }

    child.add_child("Values", values);

    children.push_back(std::make_pair("Component", child));

    ++it;
  }

  pt.add_child("MixModel_Components", children);

  boost::property_tree::json_parser::write_json(fileName, pt);
}

void te::qt::widgets::MixtureModelWizardPage::loadMixtureModelComponents(std::string fileName)
{
  try
  {
    boost::property_tree::ptree pt;
    boost::property_tree::json_parser::read_json(fileName, pt);

    BOOST_FOREACH(boost::property_tree::ptree::value_type &v, pt.get_child("MixModel_Components"))
    {
      std::string id = v.second.get<std::string>("id");
      std::string name = v.second.get<std::string>("name");

      int xGrid = v.second.get<int>("coordGrid.xGrid");
      int yGrid = v.second.get<int>("coordGrid.yGrid");

      double xGeo = v.second.get<double>("coordGeo.xGeo");
      double yGeo = v.second.get<double>("coordGeo.yGeo");

      std::vector<double> valuesVec;
      BOOST_FOREACH(boost::property_tree::ptree::value_type &c, v.second.get_child("Values"))
      {
        double val = c.second.get<double>("value");

        valuesVec.push_back(val);
      }

      MixModelComponent mmc;
      mmc.m_id = id;
      mmc.m_name = name;
      mmc.m_values = valuesVec;
      mmc.m_coordGrid = te::gm::Coord2D(xGrid, yGrid);
      mmc.m_coordGeo = te::gm::Coord2D(xGeo, yGeo);

      m_components.insert(std::map<std::string, MixModelComponent >::value_type(id, mmc));
    }

    updateComponents();
  }
  catch(boost::property_tree::json_parser::json_parser_error &je)
  {
    QString errmsg = tr("Error parsing: ") + je.filename().c_str() + ": " + je.message().c_str();

    QMessageBox::warning(this, tr("Warning"), errmsg);

    return;
  }
  catch (std::exception const& e)
  {
    QString errmsg = e.what();

    QMessageBox::warning(this, tr("Warning"), errmsg);
  }
}

void te::qt::widgets::MixtureModelWizardPage::onSaveToolButtonClicked()
{
  QString fileName = QFileDialog::getSaveFileName(this, tr("Save MixModel Components"), "", "JSON File (*.json)");

  if(!fileName.isEmpty())
    saveMixtureModelComponents(fileName.toStdString());
}

void te::qt::widgets::MixtureModelWizardPage::onLoadToolButtonClicked()
{
  QString fileName = QFileDialog::getOpenFileName(this, tr("Load MixModel Components"), "", "JSON File (*.json)");

  if(!fileName.isEmpty())
    loadMixtureModelComponents(fileName.toStdString());
}

void te::qt::widgets::MixtureModelWizardPage::onMapDisplayExtentChanged()
{
  if(m_components.empty() == false)
    drawMarks();
}

void te::qt::widgets::MixtureModelWizardPage::onPointPicked(double x, double y)
{
  assert(m_layer.get());

  //get input raster
  std::auto_ptr<te::da::DataSet> ds = m_layer->getData();

  if(ds.get())
  {
    std::size_t rpos = te::da::GetFirstPropertyPos(ds.get(), te::dt::RASTER_TYPE);
    std::auto_ptr<te::rst::Raster> inputRst = ds->getRaster(rpos);

    if(inputRst.get())
    {
      te::gm::Coord2D pixelLocation = inputRst->getGrid()->geoToGrid(x, y);

      int currentColumn = pixelLocation.x;
      int currentRow = pixelLocation.y;

      if (currentColumn < 0 || currentColumn >= (int) inputRst->getNumberOfColumns())
        return;
      if (currentRow < 0 || currentRow >= (int) inputRst->getNumberOfRows())
        return;

      //component id
      static boost::uuids::basic_random_generator<boost::mt19937> gen;
      boost::uuids::uuid u = gen();
      std::string id = boost::uuids::to_string(u);

      //component name
      QString className = QString(tr("Component ") + QString::number(m_countComponents++));

      //component values
      std::vector<double> componentsVector;

      double value;
      for(unsigned b = 0 ; b < inputRst->getNumberOfBands(); b++)
      {
        inputRst->getValue(currentColumn, currentRow, value, b);

        componentsVector.push_back(value);
      }
      
      //component coordinate
      te::gm::Coord2D coordinateGrid(currentColumn, currentRow);
      te::gm::Coord2D coordinateGeo(x, y);

      MixModelComponent mmc;
      mmc.m_id = id;
      mmc.m_name = className.toStdString();
      mmc.m_values = componentsVector;
      mmc.m_coordGrid = coordinateGrid;
      mmc.m_coordGeo = coordinateGeo;

      m_components.insert(std::map<std::string, MixModelComponent >::value_type(id, mmc));

      updateComponents();
    }
  }
}

void te::qt::widgets::MixtureModelWizardPage::onItemChanged(QTableWidgetItem* item)
{
  std::string id = item->data(Qt::UserRole).toString().toStdString();

  std::string name = item->text().toStdString();

  std::map<std::string, MixModelComponent >::iterator it = m_components.find(id);

  bool update = false;

  if(it != m_components.end())
  {
    if(it->second.m_name != name)
    {
      it->second.m_name = name;
      update = true;
    }
  }

  if(update)
    updateComponents();
}

void te::qt::widgets::MixtureModelWizardPage::onRemoveToolButtonClicked()
{
  if(m_ui->m_tableWidget->currentRow() == -1)
    return;

  std::string id = m_ui->m_tableWidget->item(m_ui->m_tableWidget->currentRow(), 0)->data(Qt::UserRole).toString().toStdString();

  std::map<std::string, MixModelComponent >::iterator it = m_components.find(id);

  if(it != m_components.end())
  {
    m_components.erase(it);

    if(m_components.empty())
      m_countComponents = 0;

    updateComponents();
  }
}

void te::qt::widgets::MixtureModelWizardPage::showNavigator(bool show)
{
  if(show)
    m_navigatorDlg->show();
  else
    m_navigatorDlg->hide();
}

void te::qt::widgets::MixtureModelWizardPage::onNavigatorClosed()
{
  m_ui->m_acquireToolButton->setChecked(false);
}

void te::qt::widgets::MixtureModelWizardPage::fillMixtureModelTypes()
{
  m_ui->m_typeComboBox->clear();

  m_ui->m_typeComboBox->addItem(tr("Linear"), MIXMODEL_LINEAR);
  m_ui->m_typeComboBox->addItem(tr("PCA - Principal Component Analysis"), MIXMODEL_PCA);
}

void te::qt::widgets::MixtureModelWizardPage::listBands()
{
  assert(m_layer.get());

  //get input raster
  std::auto_ptr<te::da::DataSet> ds = m_layer->getData();

  if(ds.get())
  {
    std::size_t rpos = te::da::GetFirstPropertyPos(ds.get(), te::dt::RASTER_TYPE);
    std::auto_ptr<te::rst::Raster> inputRst = ds->getRaster(rpos);

    if(inputRst.get())
    {
      // define sensor information
      QStringList sensorsDescriptions;
      std::vector<std::string> bandNames = te::rp::GetBandNames();

      for(unsigned int i = 0; i < bandNames.size(); i++)
        sensorsDescriptions.append(bandNames[i].c_str());

      m_ui->m_bandTableWidget->setRowCount(0);

      // initializing the list of bands
      for(unsigned b = 0 ; b < inputRst->getNumberOfBands(); b++)
      {
        int newrow = m_ui->m_bandTableWidget->rowCount();
        m_ui->m_bandTableWidget->insertRow(newrow);

        QString bName(tr("Band "));
        bName.append(QString::number(b));
        
        QCheckBox* bandCheckBox = new QCheckBox(bName, this);

        connect(bandCheckBox, SIGNAL(stateChanged(int)), this, SIGNAL(completeChanged()));

        bandCheckBox->setChecked(true);

        QComboBox* sensorDescriptionComboBox = new QComboBox(this);
        sensorDescriptionComboBox->addItems(sensorsDescriptions);

        m_ui->m_bandTableWidget->setCellWidget(newrow, 0, bandCheckBox);
        m_ui->m_bandTableWidget->setCellWidget(newrow, 1, sensorDescriptionComboBox);
      }

      m_ui->m_bandTableWidget->resizeColumnToContents(0);
    }
  }
}

void te::qt::widgets::MixtureModelWizardPage::drawMarks()
{
  te::qt::widgets::MapDisplay* mapDisplay = m_navigatorDlg->getWidget()->getDisplay();

  mapDisplay->getDraftPixmap()->fill(QColor(0, 0, 0, 0));

  const te::gm::Envelope& mapExt = mapDisplay->getExtent();

  te::qt::widgets::Canvas canvasInstance(mapDisplay->getDraftPixmap());
  canvasInstance.setWindow(mapExt.m_llx, mapExt.m_lly, mapExt.m_urx, mapExt.m_ury);

  canvasInstance.setPointPattern(m_rgbaMark, PATTERN_SIZE, PATTERN_SIZE);

  std::map<std::string, MixModelComponent>::iterator it = m_components.begin();

  while(it != m_components.end())
  {
    te::gm::Coord2D cGeo = it->second.m_coordGeo;

    te::gm::Point point;
    point.setX(cGeo.x);
    point.setY(cGeo.y);

    canvasInstance.draw(&point);

    ++it;
  }

  mapDisplay->repaint();
}

void te::qt::widgets::MixtureModelWizardPage::updateComponents()
{
  m_ui->m_tableWidget->setRowCount(0);

  std::map<std::string, MixModelComponent>::iterator it = m_components.begin();

  while(it != m_components.end())
  {
    int newrow = m_ui->m_tableWidget->rowCount();
    m_ui->m_tableWidget->insertRow(newrow);
    
    //name
    QTableWidgetItem* itemName = new QTableWidgetItem(QString::fromStdString(it->second.m_name));
    itemName->setFlags(Qt::ItemIsEnabled | Qt::ItemIsEditable);
    itemName->setData(Qt::UserRole, QVariant(it->second.m_id.c_str()));
    m_ui->m_tableWidget->setItem(newrow, 0, itemName);

    //values
    QComboBox* combo = new QComboBox(m_ui->m_tableWidget);

    for(size_t t = 0; t < it->second.m_values.size(); ++t)
    {
      combo->addItem(QString::number(it->second.m_values[t]));
    }

    m_ui->m_tableWidget->setCellWidget(newrow, 1, combo);

    ////coord x
    //QTableWidgetItem* itemCoordX = new QTableWidgetItem(QString::number(it->second.m_coordGeo.x));
    //itemCoordX->setFlags(Qt::ItemIsEnabled);
    //m_ui->m_tableWidget->setItem(newrow, 2, itemCoordX);

    ////coord y
    //QTableWidgetItem* itemCoordY = new QTableWidgetItem(QString::number(it->second.m_coordGeo.y));
    //itemCoordY->setFlags(Qt::ItemIsEnabled);
    //m_ui->m_tableWidget->setItem(newrow, 3, itemCoordY);

    ////coord x
    //QTableWidgetItem* itemCoordC = new QTableWidgetItem(QString::number(it->second.m_coordGrid.x));
    //itemCoordC->setFlags(Qt::ItemIsEnabled);
    //m_ui->m_tableWidget->setItem(newrow, 4, itemCoordC);

    ////coord y
    //QTableWidgetItem* itemCoordL = new QTableWidgetItem(QString::number(it->second.m_coordGrid.y));
    //itemCoordL->setFlags(Qt::ItemIsEnabled);
    //m_ui->m_tableWidget->setItem(newrow, 5, itemCoordL);

    ++it;
  }

  m_ui->m_tableWidget->sortByColumn(0, Qt::AscendingOrder);

  drawMarks();

  emit completeChanged();
}
