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
#include "../../../rp/Utils.h"
#include "../../../se/Fill.h"
#include "../../../se/Mark.h"
#include "../../../se/Stroke.h"
#include "../../../se/Utils.h"
#include "../../widgets/canvas/Canvas.h"
#include "../../widgets/canvas/MapDisplay.h"
#include "MixtureModelWizardPage.h"
#include "MapDisplay.h"
#include "ui_MixtureModelWizardPageForm.h"

// Qt
#include <QtGui/QGridLayout>

// Boost
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>

#define PATTERN_SIZE 12

te::qt::widgets::MixtureModelWizardPage::MixtureModelWizardPage(QWidget* parent)
  : QWizardPage(parent),
    m_ui(new Ui::MixtureModelWizardPageForm),
    m_layer(0),
    m_countComponents(0)
{
// setup controls
  m_ui->setupUi(this);

  fillMixtureModelTypes();

//define mark
  te::se::Stroke* stroke = te::se::CreateStroke("#000000", "1");
  te::se::Fill* fill = te::se::CreateFill("#FF0000", "1.0");
  m_mark = te::se::CreateMark("cross", stroke, fill);

  m_rgbaMark = te::map::MarkRendererManager::getInstance().render(m_mark, PATTERN_SIZE);

//connects
  connect(m_ui->m_tableWidget, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(onItemChanged(QTableWidgetItem*)));
  connect(m_ui->m_removeToolButton, SIGNAL(clicked()), this, SLOT(onRemoveToolButtonClicked()));

// configure page
  this->setTitle(tr("Mixture Model"));
  this->setSubTitle(tr("Select the type of mixture model and set their specific parameters."));

  m_ui->m_removeToolButton->setIcon(QIcon::fromTheme("list-remove"));
}

te::qt::widgets::MixtureModelWizardPage::~MixtureModelWizardPage()
{
  m_components.clear();

  te::common::Free(m_rgbaMark, PATTERN_SIZE);

  delete m_mark;
}

void te::qt::widgets::MixtureModelWizardPage::set(te::map::AbstractLayerPtr layer)
{
  m_layer = layer;

  listBands();
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
  for (int i = 0; i < (m_ui->m_bandsListGroupBox->layout()->count() - 1); i += 2)
  {
    QCheckBox* bandCheckBox = (QCheckBox*) m_ui->m_bandsListGroupBox->layout()->itemAt(i)->widget();

    if (bandCheckBox->isChecked())
    {
      selectedBandsVector.push_back(true);
      selectedBands++;

      algoInputParams.m_inputRasterBands.push_back(i / 2);

      QComboBox *sensorComboBox = (QComboBox*) m_ui->m_bandsListGroupBox->layout()->itemAt(i + 1)->widget();
      algoInputParams.m_inputSensorBands.push_back(std::string(sensorComboBox->currentText().toAscii()));
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

void te::qt::widgets::MixtureModelWizardPage::onMapDisplayExtentChanged()
{
  if(m_components.empty() == false)
    drawMarks();
}

void te::qt::widgets::MixtureModelWizardPage::onPointPicked(double x, double y, te::qt::widgets::MapDisplay* map)
{
  assert(m_layer.get());

  m_display = map;

  //get input raster
  te::da::DataSet* ds = m_layer->getData();

  if(ds)
  {
    std::size_t rpos = te::da::GetFirstPropertyPos(ds, te::dt::RASTER_TYPE);
    te::rst::Raster* inputRst = ds->getRaster(rpos);

    if(inputRst)
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

  delete ds;
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
  te::da::DataSet* ds = m_layer->getData();

  if(ds)
  {
    std::size_t rpos = te::da::GetFirstPropertyPos(ds, te::dt::RASTER_TYPE);
    te::rst::Raster* inputRst = ds->getRaster(rpos);

    if(inputRst)
    {
      // define sensor information
      QStringList sensorsDescriptions;
      std::vector<std::string> bandNames = te::rp::GetBandNames();

      for(unsigned int i = 0; i < bandNames.size(); i++)
        sensorsDescriptions.append(bandNames[i].c_str());

      // initializing the list of bands
      QGridLayout* bandsListLayout = new QGridLayout(m_ui->m_bandsListGroupBox);
      for(unsigned b = 0 ; b < inputRst->getNumberOfBands(); b++)
      {
        QString bName(tr("Band: "));
        bName.append(QString::number(b));
        
        QCheckBox* bandCheckBox = new QCheckBox(bName, this);

        QComboBox* sensorDescriptionComboBox = new QComboBox(this);
        sensorDescriptionComboBox->addItems(sensorsDescriptions);

        bandsListLayout->addWidget(bandCheckBox, b, 0);
        bandsListLayout->addWidget(sensorDescriptionComboBox, b, 1);
      }

      m_ui->m_bandsListGroupBox->setLayout(bandsListLayout);
    }
  }

  delete ds;
}

void te::qt::widgets::MixtureModelWizardPage::drawMarks()
{
  assert(m_display);

  m_display->getDraftPixmap()->fill(QColor(0, 0, 0, 0));

  const te::gm::Envelope& mapExt = m_display->getExtent();

  te::qt::widgets::Canvas canvasInstance(m_display->getDraftPixmap());
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

  m_display->repaint();
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
}
