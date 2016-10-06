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
#include "RpToolsWidget.h"
#include "RpToolsDialog.h"

#include "../../widgets/tools/PointPicker.h"


#include "ui_MixtureModelWizardPageForm.h"

// Qt
#include <QColorDialog>
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

//QWT
#include <qwt_symbol.h>
#include <qwt_text.h>


#define PATTERN_SIZE 12

te::qt::widgets::MixtureModelWizardPage::MixtureModelWizardPage(QWidget* parent)
  : QWizardPage(parent),
    m_ui(new Ui::MixtureModelWizardPageForm),
    m_countComponents(0),
    m_layers(0),
    m_mapDisplay(0)
{
//setup controls
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
  QObject::connect(m_ui->m_mixturetabWidget, SIGNAL(currentChanged(int)), this, SLOT(onMixturetabChanged(int)));
  QObject::connect(m_ui->m_componentTreeWidget, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(onComponentItemClicked(QTreeWidgetItem*, int)));

  connect(m_ui->m_removeToolButton, SIGNAL(clicked()), this, SLOT(onRemoveToolButtonClicked()));
  connect(m_ui->m_loadToolButton, SIGNAL(clicked()), this, SLOT(onLoadToolButtonClicked()));
  connect(m_ui->m_saveToolButton, SIGNAL(clicked()), this, SLOT(onSaveToolButtonClicked()));

  connect(m_ui->m_componentLineEdit, SIGNAL(editingFinished()), this, SLOT(oncomponentChanged()));
  connect(m_ui->m_colorToolButton, SIGNAL(clicked()), this, SLOT(oncolorToolButtonClicked()));
  connect(m_ui->m_selectedRadioButton, SIGNAL(toggled(bool)), this, SLOT(onselectedEnabled(bool)));
  connect(m_ui->m_allRadioButton, SIGNAL(toggled(bool)), this, SLOT(onallEnabled(bool)));

//configure raster navigator
  QGridLayout* layout = new QGridLayout(m_ui->m_navigatorWidget);
  m_navigator.reset(new te::qt::widgets::RpToolsWidget(m_ui->m_navigatorWidget));
  layout->addWidget(m_navigator.get(), 0, 0);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSizeConstraint(QLayout::SetMinimumSize);

  m_navigator->setSelectionMode(false);
  m_navigator->hidePickerTool(false);
  m_navigator->hideInfoTool(true);
  m_navigator->hideBoxTool(true);
  m_navigator->hideGeomTool(true);

//connects
  connect(m_navigator.get(), SIGNAL(mapDisplayExtentChanged()), this, SLOT(onMapDisplayExtentChanged()));
  connect(m_navigator.get(), SIGNAL(pointPicked(double, double)), this, SLOT(onPointPicked(double, double)));

  //configure page
  this->setTitle(tr("Mixture Model"));
  this->setSubTitle(tr("Select the type of mixture model and set their specific parameters."));

  m_ui->m_removeToolButton->setIcon(QIcon::fromTheme("edit-deletetool"));

  te::qt::widgets::ChartStyle *m_chartStyle = new te::qt::widgets::ChartStyle();
  te::qt::widgets::ChartDisplay* m_chartDisplay;
  QwtPlotCurve *m_graphic;

  //Adjusting the chart Display's style.
  m_chartStyle->setTitle("Spectral Curves");
  m_chartStyle->setAxisX("Wave Lenght");
  m_chartStyle->setAxisY("Answer");
  m_chartStyle->setGridChecked(true);

  //Creating and adjusting the chart Display
  m_chartDisplay = new te::qt::widgets::ChartDisplay(m_ui->m_graphicFrame, "", m_chartStyle);
 
  m_graphic = new QwtPlotCurve();
  m_graphic->setOrientation(Qt::Horizontal);
  m_graphic->attach(m_chartDisplay);
  m_displayWidget = new te::qt::widgets::ChartDisplayWidget(m_graphic, m_graphic->rtti(), m_chartDisplay, m_ui->m_graphicFrame);

  m_layoutg = new QGridLayout(m_ui->m_graphicFrame);
  m_layoutg->addWidget(m_displayWidget);
  m_chartDisplay->adjustDisplay();

  QPixmap px(16, 16);
  px.fill(m_color);
  m_ui->m_colorToolButton->setIcon(px);

  m_ui->m_normalizeOutputCheckBox->setVisible(false); //In future see this option, currently not normalizes

}

te::qt::widgets::MixtureModelWizardPage::~MixtureModelWizardPage()
{
  m_components.clear();

  te::common::Free(m_rgbaMark, PATTERN_SIZE);

  delete m_mark;
 }

bool te::qt::widgets::MixtureModelWizardPage::isComplete() const
{
  if(m_ui->m_componentTreeWidget->selectedItems().size() == 0)
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

void te::qt::widgets::MixtureModelWizardPage::set(std::list<te::map::AbstractLayerPtr> layers)
{
  m_layers = layers;

   while (m_ui->m_bandTableWidget->rowCount())
   m_ui->m_bandTableWidget->removeRow(0);

  listBands();
}

void te::qt::widgets::MixtureModelWizardPage::setMapDisplay(te::qt::widgets::MapDisplay* mapDisplay)
{
  m_mapDisplay = mapDisplay;
  m_navigator->setMapDisplay(mapDisplay);

  connect(m_mapDisplay, SIGNAL(extentChanged()), this, SLOT(onMapDisplayExtentChanged()));
}

void te::qt::widgets::MixtureModelWizardPage::setActionGroup(QActionGroup* actionGroup)
{
  m_navigator->setActionGroup(actionGroup);

  m_navigator->enablePickerAction();
}

std::list<te::map::AbstractLayerPtr> te::qt::widgets::MixtureModelWizardPage::get()
{
  return m_layers;
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
      algoInputParams.m_inputSensorBands.push_back(std::string(sensorComboBox->currentText().toUtf8().data()));
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

void te::qt::widgets::MixtureModelWizardPage::saveMixtureModelComponents(std::string &fileName)
{
  boost::property_tree::ptree pt;
  std::map<std::string, MixModelComponent >::iterator it = m_components.begin();
  boost::property_tree::ptree children;

  while(it != m_components.end())
  {
    boost::property_tree::ptree child;

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
    child.put("Color", it->second.m_color.getColor());
    children.push_back(std::make_pair("Component", child));

    ++it;
  }

  pt.add_child("MixModel_Components", children);

  boost::property_tree::json_parser::write_json(fileName, pt);
}

void te::qt::widgets::MixtureModelWizardPage::loadMixtureModelComponents(std::string & fileName)
{
  try
  {
    m_components.clear();

    boost::property_tree::ptree pt;
    boost::property_tree::json_parser::read_json(fileName, pt);

    int nBands = m_ui->m_bandTableWidget->rowCount();

    BOOST_FOREACH(boost::property_tree::ptree::value_type &v, pt.get_child("MixModel_Components"))
    {
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

      std::string color = v.second.get<std::string>("Color");

      MixModelComponent mmc;
      mmc.m_name = name;
      mmc.m_values = valuesVec;
      mmc.m_coordGrid = te::gm::Coord2D(xGrid, yGrid);
      mmc.m_coordGeo = te::gm::Coord2D(xGeo, yGeo);
      mmc.m_color.setColor(color);

      m_components.insert(std::map<std::string, MixModelComponent >::value_type(name, mmc));
    }

    if (m_components.size())
      m_ui->m_componentLineEdit->setText(m_components.begin()->first.c_str());

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
  std::string fileName = QFileDialog::getSaveFileName(this, tr("Save MixModel Components"), "", "JSON File (*.json)").toUtf8().data();

  if(!fileName.empty())
    saveMixtureModelComponents(fileName);
}

void te::qt::widgets::MixtureModelWizardPage::onLoadToolButtonClicked()
{
  std::string fileName = QFileDialog::getOpenFileName(this, tr("Load MixModel Components"), "", "JSON File (*.json)").toUtf8().data();

  if(!fileName.empty())
    loadMixtureModelComponents(fileName);
}

void te::qt::widgets::MixtureModelWizardPage::onMapDisplayExtentChanged()
{
  if(m_components.empty() == false)
    drawMarks();
}

void te::qt::widgets::MixtureModelWizardPage::onPointPicked(double x, double y)
{
  assert(m_layers.size());

  QString comp(m_ui->m_componentLineEdit->text());

  //get input raster
  std::list<te::map::AbstractLayerPtr>::iterator it = m_layers.begin();
  while (it != m_layers.end())
  {
    std::auto_ptr<te::da::DataSet> ds =it->get()->getData();
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

      //component name
        QString className = comp;

      //component values
      std::vector<double> componentsVector;

      double value;
      for(unsigned b = 0 ; b < inputRst->getNumberOfBands(); b++)
      {
        inputRst->getValue(currentColumn, currentRow, value, b);
        QString bName(it->get()->getTitle().c_str());
        bName.append(tr(" Band "));
        bName.append(QString::number(b));
        componentsVector.push_back(value);
      }
      
      //component coordinate
      te::gm::Coord2D coordinateGrid(currentColumn, currentRow);
      te::gm::Coord2D coordinateGeo(x, y);

      MixModelComponent mmc;
      mmc.m_name = className.toUtf8().data();
      mmc.m_values = componentsVector;
      mmc.m_coordGrid = coordinateGrid;
      mmc.m_coordGeo = coordinateGeo;
      te::color::RGBAColor newcolor(m_color.red(), m_color.green(), m_color.blue(), 255);
      mmc.m_color.setColor(newcolor.getColor());

      std::map<std::string, MixModelComponent > ::iterator it = m_components.find(className.toUtf8().data());
      if (it != m_components.end())
      {
        it->second = mmc;
      }
      else
        m_components.insert(std::map<std::string, MixModelComponent >::value_type(className.toUtf8().data(), mmc));

      updateComponents();

    } //if (inputRst.get())
  } //if (ds.get())
   ++it;
  }
}

void te::qt::widgets::MixtureModelWizardPage::onRemoveToolButtonClicked()
{
  if(m_ui->m_componentLineEdit->text().isEmpty())
    return;

  std::string comp = m_ui->m_componentLineEdit->text().toUtf8().data();
  std::map<std::string, MixModelComponent>::iterator it = m_components.find(comp);
  if (it == m_components.end())
    return;

  //Set first as current
  if (m_components.size())
    m_ui->m_componentLineEdit->setText(m_components.begin()->first.c_str());

  updateComponents();
}

void te::qt::widgets::MixtureModelWizardPage::clearCanvas()
{
  te::qt::widgets::Canvas canvasInstance(m_mapDisplay->getDraftPixmap());

  canvasInstance.clear();

  m_mapDisplay->repaint();
}

void te::qt::widgets::MixtureModelWizardPage::fillMixtureModelTypes()
{
  m_ui->m_typeComboBox->clear();

  m_ui->m_typeComboBox->addItem(tr("Linear"), MIXMODEL_LINEAR);
  m_ui->m_typeComboBox->addItem(tr("PCA - Principal Component Analysis"), MIXMODEL_PCA);
}

void te::qt::widgets::MixtureModelWizardPage::listBands()
{
  assert(m_layers.size());

  //get input raster
  std::list<te::map::AbstractLayerPtr>::iterator it = m_layers.begin();
  int newrow = 0;// m_ui->m_bandTableWidget->rowCount();
  while (it != m_layers.end())
  {
    std::auto_ptr<te::da::DataSet> ds = it->get()->getData();
  if(ds.get())
  {
    std::size_t rpos = te::da::GetFirstPropertyPos(ds.get(), te::dt::RASTER_TYPE);
    std::auto_ptr<te::rst::Raster> inputRst = ds->getRaster(rpos);

    if(inputRst.get())
    {
      // define sensor information
      QStringList sensorsDescriptions;
      std::vector<std::string> bandNames = te::rp::GetBandNames();
      std::sort(bandNames.begin(), bandNames.end());

      for(unsigned int i = 0; i < bandNames.size(); i++)
        sensorsDescriptions.append(bandNames[i].c_str());

        m_ui->m_bandTableWidget->setRowCount(newrow);

      // initializing the list of bands
      for(unsigned b = 0 ; b < inputRst->getNumberOfBands(); b++)
      {
        m_ui->m_bandTableWidget->insertRow(newrow);

          QString bName(it->get()->getTitle().c_str());
          bName.append(tr(" Band "));

        bName.append(QString::number(b));

        QCheckBox* bandCheckBox = new QCheckBox(bName, this);

        connect(bandCheckBox, SIGNAL(stateChanged(int)), this, SIGNAL(completeChanged()));

        QComboBox* sensorDescriptionComboBox = new QComboBox(this);
        sensorDescriptionComboBox->addItems(sensorsDescriptions);

        m_ui->m_bandTableWidget->setCellWidget(newrow, 0, bandCheckBox);
        m_ui->m_bandTableWidget->setCellWidget(newrow, 1, sensorDescriptionComboBox);
        m_ui->m_bandTableWidget->resizeColumnToContents(0);
        newrow++;

        bandCheckBox->setChecked(true);
      }
    }
  }
    ++it;
  }
}

void te::qt::widgets::MixtureModelWizardPage::drawMarks()
{
  //te::qt::widgets::MapDisplay* mapDisplay = m_navigatorDlg->getWidget()->getDisplay();

  m_mapDisplay->getDraftPixmap()->fill(Qt::transparent);

  const te::gm::Envelope& mapExt = m_mapDisplay->getExtent();
  
  te::qt::widgets::Canvas canvasInstance(m_mapDisplay->getDraftPixmap());
  
  canvasInstance.setWindow(mapExt.m_llx, mapExt.m_lly, mapExt.m_urx, mapExt.m_ury);

  canvasInstance.setPointPattern(m_rgbaMark, PATTERN_SIZE, PATTERN_SIZE);

  std::map<std::string, MixModelComponent>::iterator it = m_components.begin();

  while(it != m_components.end())
  {
    te::gm::Coord2D cGeo = it->second.m_coordGeo;

    te::gm::Point point;
    point.setX(cGeo.x);
    point.setY(cGeo.y);
    canvasInstance.setPointColor(it->second.m_color);

    canvasInstance.draw(&point);

    ++it;
  }

  m_mapDisplay->repaint();
}

void te::qt::widgets::MixtureModelWizardPage::updateComponents()
{
  std::string comp = m_ui->m_componentLineEdit->text().toUtf8().data();

  std::map<std::string, MixModelComponent>::iterator it = m_components.find(comp);
  if (it == m_components.end())
    return;

  m_ui->m_componentTreeWidget->clear();

  int nBands = m_ui->m_bandTableWidget->rowCount();

  for (it = m_components.begin(); it != m_components.end(); ++it)
  {
    QBrush brush(QColor(it->second.m_color.getRed(), it->second.m_color.getGreen(), it->second.m_color.getBlue(), 255));
    QTreeWidgetItem* compItem = new QTreeWidgetItem;
    compItem->setText(0, it->first.c_str());
    compItem->setForeground(0, brush);
    m_ui->m_componentTreeWidget->addTopLevelItem(compItem);
    m_ui->m_componentTreeWidget->setCurrentItem(compItem);

    std::size_t count = 0;
    for (int i = 0; i < nBands; ++i)
    {
      QCheckBox* checkBox = (QCheckBox*)m_ui->m_bandTableWidget->cellWidget(i, 0);
      if (checkBox && checkBox->isChecked())
      {
        QComboBox* comboBox = (QComboBox*)m_ui->m_bandTableWidget->cellWidget(i, 1);
        QTreeWidgetItem* item = new QTreeWidgetItem(compItem);
        item->setText(0, comboBox->currentText());
        if (i < it->second.m_values.size())
          item->setText(1, QString::number(it->second.m_values[i]));
        else
          item->setText(1, QString::number(0.0));
        item->setForeground(0, brush);
        item->setForeground(1, brush);
        m_ui->m_componentTreeWidget->setCurrentItem(item);
        m_ui->m_componentTreeWidget->resizeColumnToContents(0);
        m_ui->m_componentTreeWidget->resizeColumnToContents(1);
      }
    }
  }

  drawMarks();

  PlotSpectralSignature();

  emit completeChanged();
}

void te::qt::widgets::MixtureModelWizardPage::onComponentItemClicked(QTreeWidgetItem * item, int column)
{
  assert(item);
  if (!item->childCount())
  {
    QTreeWidgetItem* parent = item->parent();
    if (!parent)
      return;
    item = parent;
  }  

  m_ui->m_componentLineEdit->setText(item->text(0));

  std::map<std::string, MixModelComponent>::iterator it = m_components.find(item->text(0).toStdString());
  if (it != m_components.end())
  {
    m_color = QColor(it->second.m_color.getRed(), it->second.m_color.getGreen(), it->second.m_color.getBlue(), it->second.m_color.getAlpha());
    QPixmap px(16, 16);
    px.fill(m_color);
    m_ui->m_colorToolButton->setIcon(px);
  }

  drawMarks();

  PlotSpectralSignature();
}

void te::qt::widgets::MixtureModelWizardPage::PlotSpectralSignature()
{
  if (!m_components.size())
    return;

  te::qt::widgets::ChartStyle *m_chartStyle = new te::qt::widgets::ChartStyle();
  te::qt::widgets::ChartDisplay* m_chartDisplay;
  //QwtPlotCurve *m_graphic;
  if (m_displayWidget)
  {
    m_layoutg->removeWidget(m_displayWidget);
    delete m_displayWidget;
    m_displayWidget = 0;
  }

  if (m_ui->m_selectedRadioButton->isChecked())
  {
    m_chartStyle->setTitle(m_ui->m_componentLineEdit->text());
  }
  else
    m_chartStyle->setTitle("Spectral Curves");
 
  m_chartStyle->setAxisX("Wave Lenght");
  m_chartStyle->setAxisY("Answer");
  m_chartStyle->setGridChecked(true);

  ////Creating and adjusting the chart Display
  m_chartDisplay = new te::qt::widgets::ChartDisplay(m_ui->m_graphicFrame, "", m_chartStyle);
  m_chartDisplay->adjustDisplay();

  bool first = true;
  for (std::map<std::string, MixModelComponent > ::iterator it = m_components.begin(); it != m_components.end(); it++)
  {
    if (m_ui->m_selectedRadioButton->isChecked())
    {
      if (it->first.compare(m_ui->m_componentLineEdit->text().toStdString()) != 0)
        continue;
    }
    QwtPlotCurve *graphic = new QwtPlotCurve();
    graphic->setOrientation(Qt::Horizontal);
    QVector<QPointF> values;
    double wave = 0;
    for (unsigned int ii = 0; ii < it->second.m_values.size(); ++ii)
    {
      QCheckBox* bandCheckBox = (QCheckBox*)m_ui->m_bandTableWidget->cellWidget(ii, 0);
      if (bandCheckBox->isChecked())
      {
        QComboBox* sensor = (QComboBox*)m_ui->m_bandTableWidget->cellWidget(ii, 1);
        wave = GetMediumWavelength(sensor->currentText().toStdString());
        values.push_back(QPointF(wave, it->second.m_values[ii]));
      }
    }

    graphic->setSamples(values);

    QPen CurvePen;
    CurvePen.setColor(QColor(it->second.m_color.getRed(), it->second.m_color.getGreen(), it->second.m_color.getBlue()));
    CurvePen.setStyle(Qt::SolidLine);
    CurvePen.setWidth(0);
    graphic->setPen(CurvePen);

    graphic->attach(m_chartDisplay);

    if (first)
    {
      m_displayWidget = new te::qt::widgets::ChartDisplayWidget(graphic, graphic->rtti(), m_chartDisplay, m_ui->m_graphicFrame);
      first = false;
    }
  }

  if (!m_displayWidget)
    return;

  m_layoutg->addWidget(m_displayWidget);
  m_chartDisplay->adjustDisplay();
  m_displayWidget->show();

}

double te::qt::widgets::MixtureModelWizardPage::GetMediumWavelength(std::string sensor)
{
  std::pair<double, double> val = te::rp::GetSpectralBandInfo(sensor);

  return ((val.first + val.second)/2.);
}

void te::qt::widgets::MixtureModelWizardPage::oncomponentChanged()
{
  m_navigator->setPointPickedTool(true);
  updateComponents();
}

void te::qt::widgets::MixtureModelWizardPage::oncolorToolButtonClicked()
{
 // m_rgbaMark
  QString comp(m_ui->m_componentLineEdit->text());


  std::map<std::string, MixModelComponent>::iterator itc = m_components.find(comp.toUtf8().data());
  if (itc != m_components.end())
    m_color = QColor(itc->second.m_color.getRed(), itc->second.m_color.getGreen(),itc->second.m_color.getBlue(),itc->second.m_color.getAlpha());

  m_color = QColorDialog::getColor(m_color, this);
  if (itc != m_components.end())
  {
    te::color::RGBAColor newcolor(m_color.red(), m_color.green(), m_color.blue(),255);
    itc->second.m_color.setColor(newcolor.getColor());
  }

  QPixmap px(16, 16);
  px.fill(m_color);
  m_ui->m_colorToolButton->setIcon(px);

  updateComponents();
}

void te::qt::widgets::MixtureModelWizardPage::onselectedEnabled(bool status)
{
  if (status)
    PlotSpectralSignature();
}

void te::qt::widgets::MixtureModelWizardPage::onallEnabled(bool status)
{
  if (status)
    PlotSpectralSignature();
}

void te::qt::widgets::MixtureModelWizardPage::onMixturetabChanged(int page)
{
  if (page == 1)
    updateComponents();
}
