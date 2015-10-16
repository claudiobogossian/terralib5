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
  \file terralib/rp/widgets/se/MixtureModelDialog.cpp

  \brief A dialog used to execute mixture model decomposition.
*/

// TerraLib
#include "../canvas/Canvas.h"
#include "../canvas/MapDisplay.h"
#include "../../../common/StringUtils.h"
#include "../../../geometry/Coord2D.h"
#include "../../../geometry/Envelope.h"
#include "../../../geometry/Point.h"
#include "../../../maptools/Utils.h"
#include "../../../raster/Grid.h"
#include "../../../rp/MixtureModel.h"
#include "../../../rp/MixtureModelLinearStrategy.h"
#include "../../../rp/MixtureModelPCAStrategy.h"
#include "../../../rp/Functions.h"
#include "../../widgets/tools/CoordTracking.h"
#include "../../widgets/tools/Pan.h"
#include "../../widgets/tools/ZoomWheel.h"

// GUI
#include "MixtureModelDialog.h"
#include <ui_MixtureModelForm.h>

// QT
#include <QCheckBox>
#include <QComboBox>
#include <QGridLayout>
#include <QImage>
#include <QKeyEvent>
#include <QLineEdit>
#include <QMessageBox>

// defines
#define PATTERN_W 11

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

te::qt::widgets::MixtureModelDialog::MixtureModelDialog(const te::map::DataSetLayerPtr& inputLayerPtr, const std::string& outpuRasterDSType,
                                                        const std::map<std::string, std::string>& outpuRasterInfo, QWidget* parent, Qt::WindowFlags f)
  : QDialog(parent, f),
    m_inputRasterPtr(te::map::GetRaster(inputLayerPtr.get())),
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
  std::list<te::map::AbstractLayerPtr> layerList;
  layerList.push_back(inputLayerPtr);
  m_mapDisplay->setLayerList(layerList);
  m_mapDisplay->setSRID(m_inputRasterPtr->getSRID());
  te::gm::Envelope displayExtent(*(m_inputRasterPtr->getExtent()));
  m_mapDisplay->setExtent(displayExtent);

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
  connect(m_mapDisplay, SIGNAL(extentChanged()), this, SLOT(on_mapDisplay_extentChanged()));

// define sensor information
  QStringList sensorsDescriptions;
  std::vector<std::string> bandNames = te::rp::GetBandNames();
  for(unsigned int i = 0; i < bandNames.size(); i++)
    sensorsDescriptions.append(bandNames[i].c_str());

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

// creating "x" pattern to plot on map
  m_selectedPointPattern = new te::color::RGBAColor*[ PATTERN_W ];

  for(unsigned int line = 0; line < PATTERN_W; line++)
  {
    m_selectedPointPattern[line] = new te::color::RGBAColor[PATTERN_W];

    for(unsigned int col = 0; col < PATTERN_W; col++)
    {
      if((line == col) || (line == (PATTERN_W - col - 1)))
        m_selectedPointPattern[line][col].setColor(255, 0, 0, TE_OPAQUE);
      else
        m_selectedPointPattern[line][col].setColor(0, 0, 0, TE_TRANSPARENT);
    }
  }
}

te::qt::widgets::MixtureModelDialog::~MixtureModelDialog()
{
// delete "x" patterns
  for(unsigned int line = 0; line < PATTERN_W; line++)
    delete[] m_selectedPointPattern[line];

  delete[] m_selectedPointPattern;

// delete interface
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
  std::vector<bool> selectedBandsVector;
  for (int i = 0; i < (m_uiPtr->m_bandsListGroupBox->layout()->count() - 1); i += 2)
  {
    QCheckBox* bandCheckBox = (QCheckBox*) m_uiPtr->m_bandsListGroupBox->layout()->itemAt(i)->widget();

    if (bandCheckBox->isChecked())
    {
      selectedBandsVector.push_back(true);
      selectedBands++;

// insert band number
      algoInputParameters.m_inputRasterBands.push_back(i / 2);

// insert sensor description of band
      QComboBox *sensorComboBox = (QComboBox*) m_uiPtr->m_bandsListGroupBox->layout()->itemAt(i + 1)->widget();
#if (QT_VERSION >= 0x050000)
      algoInputParameters.m_inputSensorBands.push_back(std::string(sensorComboBox->currentText().toLatin1()));
#else
      algoInputParameters.m_inputSensorBands.push_back(std::string(sensorComboBox->currentText().toAscii()));
#endif
    }
    else
      selectedBandsVector.push_back(false);
  }

// insert mixture components
  std::map<std::string, std::vector<double> >::const_iterator cit = m_components.begin();
  std::vector<double> components;
  while (cit != m_components.end())
  {
    components.clear();

    for (unsigned int i = 0; i < selectedBandsVector.size(); i++)
      if (selectedBandsVector[i])
        components.push_back(cit->second[i]);

    algoInputParameters.m_components[cit->first] = components;

    ++cit;
  }

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

  te::gm::Coord2D coordinate(currentColumn, currentRow);
  m_coordinates[className->toStdString()] = coordinate;

  updateComponentsGrid();
}

void te::qt::widgets::MixtureModelDialog::on_removeButton_clicked()
{
  QPushButton* sender = (QPushButton*) QObject::sender();

// retrieve component class name
  std::string className = sender->statusTip().toStdString();

  m_components.erase(className);
  m_coordinates.erase(className);

  updateComponentsGrid();
}

void te::qt::widgets::MixtureModelDialog::on_mapDisplay_extentChanged()
{
// get canvas to draw the "x" on selected points
  const te::gm::Envelope& mapDisplayExtent = m_mapDisplay->getExtent();

  m_mapDisplay->getDraftPixmap()->fill(QColor(0, 0, 0, 0).rgba());

  te::qt::widgets::Canvas canvasInstance(m_mapDisplay->getDraftPixmap());
  canvasInstance.setWindow(mapDisplayExtent.m_llx, mapDisplayExtent.m_lly,
                           mapDisplayExtent.m_urx, mapDisplayExtent.m_ury);

  canvasInstance.setPointPattern(m_selectedPointPattern, PATTERN_W, PATTERN_W);

  te::gm::Point auxPoint;
  te::gm::Coord2D auxCoord2D;

  std::map<std::string, te::gm::Coord2D>::const_iterator cpit = m_coordinates.begin();
  while (cpit != m_coordinates.end())
  {
// draw "x"
    m_inputRasterPtr->getGrid()->gridToGeo(cpit->second.x, cpit->second.y,
                                           auxCoord2D.x, auxCoord2D.y);

    auxPoint.setX(auxCoord2D.x);
    auxPoint.setY(auxCoord2D.y);

    canvasInstance.draw(&auxPoint);

    ++cpit;
  }
}

void te::qt::widgets::MixtureModelDialog::updateComponentsGrid()
{
  QGridLayout* componentsListLayout = (QGridLayout*) m_uiPtr->m_componentsListGroupBox->layout();

// update class names
  for (int i = 0; i < (componentsListLayout->count() / 5); i++)
  {
    std::string className = ((QLineEdit*) componentsListLayout->itemAtPosition(i, 1)->widget())->text().toStdString();
    std::string originalClassName = ((QLineEdit*) componentsListLayout->itemAtPosition(i, 1)->widget())->toolTip().toStdString();

// check if className is already in m_components
    if (className != originalClassName)
    {
      m_components[className] = m_components[originalClassName];
      m_coordinates[className] = m_coordinates[originalClassName];

      m_components.erase(originalClassName);
      m_coordinates.erase(originalClassName);
    }

  }

// clear layout
  QLayoutItem* child;
  while ((child = componentsListLayout->takeAt(0)) != 0)
  {
    delete child->widget();
    delete child;
  }

// clear map display to redraw "x" points
  te::gm::Envelope auxEnvelope(m_mapDisplay->getExtent());
  m_mapDisplay->setExtent(auxEnvelope);

// add lines
  std::map<std::string, std::vector<double> >::const_iterator cit = m_components.begin();
  unsigned int totalComponents = 0;
  while (cit != m_components.end())
  {
// define remove button and line edits
    QPushButton* removeButton = new QPushButton("x");

    removeButton->setStatusTip(cit->first.c_str());
    removeButton->setMaximumSize(20, 20);
    connect(removeButton, SIGNAL(clicked()), SLOT(on_removeButton_clicked()));

    componentsListLayout->addWidget(removeButton, totalComponents, 0);

    QLineEdit* className = new QLineEdit(cit->first.c_str());
    className->setToolTip(cit->first.c_str());
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
