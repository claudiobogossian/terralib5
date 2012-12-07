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
  \file terralib/rp/widgets/se/MixtureModelDialog.cpp

  \brief A dialog used to execute mixture model decomposition.
*/

// TerraLib
#include "../canvas/MapDisplay.h"
#include "../../../common/StringUtils.h"
#include "../../../geometry/Coord2D.h"
#include "../../../raster/Grid.h"
#include "../../../rp/MixtureModel.h"
#include "../../../rp/MixtureModelLinearStrategy.h"
#include "../../../rp/MixtureModelPCAStrategy.h"
#include "../../widgets/tools/CoordTracking.h"
#include "../../widgets/tools/Pan.h"
#include "../../widgets/tools/ZoomWheel.h"


// GUI
#include "MixtureModelDialog.h"
#include <ui_MixtureModelForm.h>

// QT
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QGridLayout>
#include <QtGui/QImage>
#include <QtGui/QKeyEvent>
#include <QtGui/QLineEdit>
#include <QtGui/QMessageBox>

te::qt::widgets::MixtureModelDialogMDEventFilter::MixtureModelDialogMDEventFilter(te::qt::widgets::MapDisplay* parent)
  : QObject(parent), m_mDisplay(parent)
{
}

te::qt::widgets::MixtureModelDialogMDEventFilter::~MixtureModelDialogMDEventFilter()
{
}

bool te::qt::widgets::MixtureModelDialogMDEventFilter::eventFilter(QObject* watched, QEvent* event)
{
  if(event->type() == QEvent::Enter)
    m_mDisplay->grabKeyboard();
  else if(event->type() == QEvent::Leave)
    m_mDisplay->releaseKeyboard();
  else if(event->type() == QEvent::KeyPress)
  {
    if(m_mDisplay->underMouse())
      emit(keyPressedOverMapDisplay(((QKeyEvent*)event)->key()));
  }

  return false;
}

te::qt::widgets::MixtureModelDialog::MixtureModelDialog(const te::map::RasterLayer* inputRasterLayerPtr, const std::string& outpuRasterDSType,
                                                        const std::map<std::string, std::string>& outpuRasterInfo, QWidget* parent, Qt::WindowFlags f)
  : QDialog(parent, f),
    m_inputRasterPtr(inputRasterLayerPtr->getRaster()),
    m_outpuRasterDSType(outpuRasterDSType),
    m_outpuRasterInfo(outpuRasterInfo),
    m_maxComponentsInserted(0)
{
  m_uiPtr = new Ui::MixtureModelForm;

  m_uiPtr->setupUi(this);

// insert image in frame
  QGridLayout* rasterGridLayout = new QGridLayout(m_uiPtr->m_rasterFrame);
  m_mapDisplay = new te::qt::widgets::MapDisplay(m_uiPtr->m_rasterFrame->size(), m_uiPtr->m_rasterFrame);
  m_mapDisplay->setMouseTracking(true);
  rasterGridLayout->addWidget(m_mapDisplay);
  m_mapDisplay->setResizePolicy(te::qt::widgets::MapDisplay::Center); // ???
  std::list<te::map::AbstractLayer*> layerList;
  layerList.push_back((te::map::RasterLayer*) inputRasterLayerPtr);
  m_mapDisplay->setLayerList(layerList);
  m_mapDisplay->setSRID(m_inputRasterPtr->getSRID());
  m_mapDisplay->setExtent(*(m_inputRasterPtr->getExtent()));

// image frame events
  m_panClickEvent = new te::qt::widgets::Pan(m_mapDisplay, Qt::OpenHandCursor, Qt::ClosedHandCursor);
  m_zoomScroolEvent = new te::qt::widgets::ZoomWheel(m_mapDisplay, 1.5, m_mapDisplay);
  m_coordTracking = new te::qt::widgets::CoordTracking(m_mapDisplay, m_mapDisplay);
  m_keyboardPressTracking = new MixtureModelDialogMDEventFilter(m_mapDisplay);

  m_mapDisplay->installEventFilter(m_coordTracking);
  m_mapDisplay->installEventFilter(m_keyboardPressTracking);
  m_mapDisplay->installEventFilter(m_panClickEvent);
  m_mapDisplay->installEventFilter(m_zoomScroolEvent);

// signals & slots
  connect(m_uiPtr->m_okPushButton, SIGNAL(clicked()), SLOT(on_okPushButton_clicked()));
  connect(m_coordTracking, SIGNAL(coordTracked(QPointF&)), this, SLOT(on_coordTracked_changed(QPointF&)));
  connect(m_keyboardPressTracking, SIGNAL(keyPressedOverMapDisplay(int)), this, SLOT(on_keyPressedOverMapDisplay(int)));

// define sensor information
  QStringList sensorsDescriptions;
  sensorsDescriptions.append("sensor_description1");
  sensorsDescriptions.append("sensor_description2");
  sensorsDescriptions.append("sensor_description3");

// initializing the list of bands
  QGridLayout *bandsListLayout = new QGridLayout();
  for(unsigned b = 0 ; b < m_inputRasterPtr->getNumberOfBands(); b++)
  {
    std::string bname = "Band " + te::common::Convert2String(b);
    QCheckBox* bandCheckBox = new QCheckBox(tr(bname.c_str()));

    QComboBox* sensorDescriptionComboBox = new QComboBox();
    sensorDescriptionComboBox->addItems(sensorsDescriptions);

    bandsListLayout->addWidget(bandCheckBox, b, 0);
    bandsListLayout->addWidget(sensorDescriptionComboBox, b, 1);
  }
  m_uiPtr->m_bandsListGroupBox->setLayout(bandsListLayout);

// initializing the list of components
  QGridLayout *componentsListLayout = new QGridLayout();
  m_uiPtr->m_componentsListGroupBox->setLayout(componentsListLayout);
}

te::qt::widgets::MixtureModelDialog::~MixtureModelDialog()
{
  delete m_uiPtr;
}

bool te::qt::widgets::MixtureModelDialog::getOutputRaster(boost::shared_ptr<te::rst::Raster>& outputRasterPtr)
{
  if(m_outputRasterPtr.get())
  {
    outputRasterPtr = m_outputRasterPtr;

    return true;
  }

  return false;
}

void te::qt::widgets::MixtureModelDialog::on_okPushButton_clicked()
{
  m_outputRasterPtr.reset();

  if (!m_inputRasterPtr)
  {
    QMessageBox::critical(this, "", tr("Invalid input raster"));

    return;
  }

// define classification parameters

// input parameters
  te::rp::MixtureModel::InputParameters algoInputParameters;
  algoInputParameters.m_strategyName = (m_uiPtr->m_mmTypeComboBox->currentText() == "Linear" ? "linear" : "pca");
  algoInputParameters.m_inputRasterPtr = m_inputRasterPtr;

  unsigned selectedBands = 0;
  for (int i = 0; i < (m_uiPtr->m_bandsListGroupBox->layout()->count() - 1); i += 2)
  {
    QCheckBox* bandCheckBox = (QCheckBox*) m_uiPtr->m_bandsListGroupBox->layout()->itemAt(i)->widget();

    if (bandCheckBox->isChecked())
    {
      selectedBands++;

// insert band number
      algoInputParameters.m_inputRasterBands.push_back(i / 2);

// insert sensor description of band
      QComboBox *sensorComboBox = (QComboBox*) m_uiPtr->m_bandsListGroupBox->layout()->itemAt(i + 1)->widget();
      algoInputParameters.m_inputSensorBands.push_back(std::string(sensorComboBox->currentText().toAscii()));
    }
  }

// insert mixture components
  std::map<std::string, std::vector<double> >::const_iterator cit = m_components.begin();
  while (cit != m_components.end())
  {
    algoInputParameters.m_components[cit->first] = cit->second;

    ++cit;
  }

/*
  QGridLayout *componentsListLayout = (QGridLayout*) m_uiPtr->m_componentsListGroupBox->layout();
// insert mixture components
  for (unsigned int i = 0; i < m_components.size(); i++)
  {
    QLineEdit* componentLineEdit = (QLineEdit*) componentsListLayout->itemAtPosition(i, 1)->widget();

    for (unsigned int b = 0; b < algoInputParameters.m_inputRasterBands.size(); b++)
    {
      QLineEdit* pixelValueLineEdit = (QLineEdit*) componentsListLayout->itemAtPosition(i, algoInputParameters.m_inputRasterBands[b] + 2)->widget();

      algoInputParameters.m_components[std::string(componentLineEdit->text().toAscii())].push_back(pixelValueLineEdit->text().toDouble());
    }
  }
*/
// link specific parameters with chosen implementation
  if (m_uiPtr->m_mmTypeComboBox->currentText() == "Linear")
  {
    te::rp::MixtureModelLinearStrategy::Parameters specificParameters;
    algoInputParameters.setMixtureModelStrategyParams(specificParameters);
  }
  else // "pca"
  {
    te::rp::MixtureModelPCAStrategy::Parameters specificParameters;
    algoInputParameters.setMixtureModelStrategyParams(specificParameters);
  }

// output parameters
  te::rp::MixtureModel::OutputParameters algoOutputParameters;
  algoOutputParameters.m_rInfo = m_outpuRasterInfo;
  algoOutputParameters.m_rType = m_outpuRasterDSType;
  algoOutputParameters.m_normalizeOutput = m_uiPtr->m_normalizeOutputCheckBox->isChecked();
  algoOutputParameters.m_createErrorRaster = m_uiPtr->m_createErrorRasterCheckBox->isChecked();

// execute the algorithm
  te::rp::MixtureModel MixtureModelinstance;

  if(!MixtureModelinstance.initialize(algoInputParameters))
  {
    QMessageBox::critical(this, "", tr("MixtureModel initialization error"));

    return;
  }

  if(!MixtureModelinstance.execute(algoOutputParameters))
  {
    QMessageBox::critical(this, "", tr("MixtureModel execution error"));

    return;
  }

  QMessageBox::information(this, "", tr("Mixture model decomposition ended sucessfully"));
}

void te::qt::widgets::MixtureModelDialog::on_coordTracked_changed(QPointF& coordinate)
{
// find the selected pixel location for component
  te::gm::Coord2D pixelLocation = m_inputRasterPtr->getGrid()->geoToGrid((double) coordinate.rx(), (double)coordinate.ry());
  currentColumn = pixelLocation.x;
  currentRow = pixelLocation.y;
}

void te::qt::widgets::MixtureModelDialog::on_keyPressedOverMapDisplay(int key)
{
// check range
  if (currentColumn < 0 || currentColumn >= (int) m_inputRasterPtr->getNumberOfColumns())
    return;
  if (currentRow < 0 || currentRow >= (int) m_inputRasterPtr->getNumberOfRows())
    return;

  std::vector<double> componentsVector;

  QString* className = new QString("Change name " + QString::number(m_maxComponentsInserted++));

  double value;
  for(unsigned b = 0 ; b < m_inputRasterPtr->getNumberOfBands(); b++)
  {
    m_inputRasterPtr->getValue(currentColumn, currentRow, value, b);

    componentsVector.push_back(value);
  }

  m_components[className->toStdString()] = componentsVector;

  updateComponentsGrid();
}

void te::qt::widgets::MixtureModelDialog::on_removeButton_clicked()
{
  QPushButton* sender = (QPushButton*) QObject::sender();

//  int componentRow = sender->statusTip().toInt();

//  QGridLayout *componentsListLayout = (QGridLayout*) m_uiPtr->m_componentsListGroupBox->layout();

// retrieve component class name
  std::string className = sender->statusTip().toStdString();
std::cout << "removing className " << className << std::endl << std::endl;
  m_components.erase(className);

  updateComponentsGrid();
}

void te::qt::widgets::MixtureModelDialog::updateComponentsGrid()
{
  QGridLayout *componentsListLayout = (QGridLayout*) m_uiPtr->m_componentsListGroupBox->layout();

// clear layout
  QLayoutItem *child;
  while ((child = componentsListLayout->takeAt(0)) != 0)
  {
    delete child->widget();
    delete child;
  }

/*
  unsigned int ccount = componentsListLayout->count();
  QItem* child;
  for (unsigned int i = 0; i < ccount; i++)
  {
    child = componentsListLayout->takeAt(0);
    componentsListLayout->removeItem(child);
    delete child;
    std::cout << componentsListLayout->count() << std::endl;
  }
  std::cout << "end remove" << std::endl;
*/

// add lines
  std::map<std::string, std::vector<double> >::const_iterator cit = m_components.begin();
  unsigned int totalComponents = 0;
  while (cit != m_components.end())
  {
    QPushButton* removeButton = new QPushButton("x");
    removeButton->setStatusTip(cit->first.c_str());
    removeButton->setMaximumSize(20, 20);
    connect(removeButton, SIGNAL(clicked()), SLOT(on_removeButton_clicked()));
    componentsListLayout->addWidget(removeButton, totalComponents, 0);
std::cout << "inserting item " << cit->first.c_str() << std::endl;
std::cout << "  total components: " << totalComponents << std::endl;
    QLineEdit* className = new QLineEdit(cit->first.c_str());
    componentsListLayout->addWidget(className, totalComponents, 1);

    std::vector<double> components = cit->second;
    for (unsigned int i = 0; i < components.size(); i++)
    {
      QLineEdit* currentPixel = new QLineEdit(QString::number(components[i]));

      currentPixel->setMaximumSize(50, currentPixel->maximumHeight());

      componentsListLayout->addWidget(currentPixel, totalComponents, i + 2);
    }

    totalComponents++;
    ++cit;
  }
}
