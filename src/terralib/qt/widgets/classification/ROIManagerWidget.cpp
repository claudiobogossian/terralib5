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
  \file terralib/qt/widgets/classification/ROIManagerWidget.cpp

  \brief This file has the ROIManagerWidget class.
*/

// TerraLib
#include "../../../classification/ROI.h"
#include "../../../classification/ROISet.h"
#include "../canvas/Canvas.h"
#include "../canvas/MapDisplay.h"
#include "../rp/RasterNavigatorWidget.h"
#include "../utils/ColorPickerToolButton.h"
#include "ROIManagerWidget.h"
#include "ui_ROIManagerWidgetForm.h"

// Qt
#include <QtGui/QFileDialog>
#include <QtGui/QMessageBox>
#include <QtGui/QGridLayout>

//STL
#include <memory>

// Boost
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>

Q_DECLARE_METATYPE(te::map::AbstractLayerPtr);

#define ROI_TREE_ITEM           0
#define ROI_POLYGON_TREE_ITEM   1

te::qt::widgets::ROIManagerWidget::ROIManagerWidget(QWidget* parent, Qt::WindowFlags f)
  : QWidget(parent, f),
    m_ui(new Ui::ROIManagerWidgetForm),
    m_rs(0),
    m_sampleCounter(0)
{
  m_ui->setupUi(this);

  m_ui->m_openLayerROIToolButton->setIcon(QIcon::fromTheme("folder-open"));
  m_ui->m_removeROIToolButton->setIcon(QIcon::fromTheme("list-remove"));
  m_ui->m_exportROISetToolButton->setIcon(QIcon::fromTheme("document-export"));
  m_ui->m_addROIToolButton->setIcon(QIcon::fromTheme("list-add"));

  // Color Picker
  m_colorPicker = new te::qt::widgets::ColorPickerToolButton(this);
  m_colorPicker->setFixedSize(70, 24);
  m_colorPicker->setColor(QColor(255, 255, 0, 128));

  //build form
  QGridLayout* colorPickerLayout = new QGridLayout(m_ui->m_colorWidget);
  colorPickerLayout->setContentsMargins(0, 0, 0, 0);
  colorPickerLayout->addWidget(m_colorPicker);

  QGridLayout* layout = new QGridLayout(m_ui->m_navigatorWidget);
  m_navigator.reset( new te::qt::widgets::RasterNavigatorWidget(m_ui->m_navigatorWidget));
  layout->addWidget(m_navigator.get(), 0, 0);
  layout->setContentsMargins(0,0,0,0);
  layout->setSizeConstraint(QLayout::SetMinimumSize);

  m_navigator->hideInfoTool(true);
  m_navigator->hidePickerTool(true);

  //connects
  connect(m_ui->m_openLayerROIToolButton, SIGNAL(clicked()), this, SLOT(onOpenLayerROIToolButtonClicked()));
  connect(m_ui->m_addROIToolButton, SIGNAL(clicked()), this, SLOT(onAddROIToolButtonClicked()));
  connect(m_ui->m_removeROIToolButton, SIGNAL(clicked()), this, SLOT(onRemoveROIToolButtonClicked()));
  connect(m_ui->m_fileDialogToolButton, SIGNAL(clicked()), this, SLOT(onFileDialogToolButtonClicked()));
  connect(m_ui->m_exportROISetToolButton, SIGNAL(clicked()), this, SLOT(onExportROISetToolButtonClicked()));
  connect(m_navigator.get(), SIGNAL(mapDisplayExtentChanged()), this, SLOT(onMapDisplayExtentChanged()));
  connect(m_navigator.get(), SIGNAL(geomAquired(te::gm::Polygon*)), this, SLOT(onGeomAquired(te::gm::Polygon*)));
}

te::qt::widgets::ROIManagerWidget::~ROIManagerWidget()
{
}

Ui::ROIManagerWidgetForm* te::qt::widgets::ROIManagerWidget::getForm() const
{
  return m_ui.get();
}

void te::qt::widgets::ROIManagerWidget::setList(std::list<te::map::AbstractLayerPtr>& layerList)
{
  m_ui->m_layerROIComboBox->clear();

  std::list<te::map::AbstractLayerPtr>::iterator it = layerList.begin();

  while(it != layerList.end())
  {
    te::map::AbstractLayerPtr l = *it;

    std::auto_ptr<te::da::DataSetType> dsType = l->getSchema();

    if(dsType->hasGeom())
      m_ui->m_layerROIComboBox->addItem(it->get()->getTitle().c_str(), QVariant::fromValue(l));

    ++it;
  }
}

void te::qt::widgets::ROIManagerWidget::set(te::map::AbstractLayerPtr layer)
{
  m_layer = layer;

  m_navigator->set(layer);
}

te::cl::ROISet* te::qt::widgets::ROIManagerWidget::getROISet()
{
  return m_rs;
}

void te::qt::widgets::ROIManagerWidget::drawROISet()
{
  if(!m_rs)
    return;

  te::qt::widgets::MapDisplay* mapDisplay = m_navigator->getDisplay();

  mapDisplay->getDraftPixmap()->fill(QColor(0, 0, 0, 0));

  const te::gm::Envelope& mapExt = mapDisplay->getExtent();

  te::qt::widgets::Canvas canvasInstance(mapDisplay->getDraftPixmap());
  canvasInstance.setWindow(mapExt.m_llx, mapExt.m_lly, mapExt.m_urx, mapExt.m_ury);

  canvasInstance.setPolygonContourColor(te::color::RGBAColor(0,0,0, TE_OPAQUE));
  canvasInstance.setPolygonContourWidth(1);

  std::map<std::string, te::cl::ROI*> roiMap = m_rs->getROISet();
  std::map<std::string, te::cl::ROI*>::iterator it = roiMap.begin();

  while(it != roiMap.end())
  {
    te::cl::ROI* roi = it->second;

    std::map<std::string, te::gm::Polygon*> polyMap = roi->getPolygons();
    std::map<std::string, te::gm::Polygon*>::iterator itPoly = polyMap.begin();

    te::color::RGBAColor rgba(roi->getColor());

    canvasInstance.setPolygonFillColor(rgba);

    while(itPoly != polyMap.end())
    {
      te::gm::Polygon* poly = itPoly->second;

      canvasInstance.draw(poly);

      ++itPoly;
    }
    ++it;
  }

  mapDisplay->repaint();
}

void te::qt::widgets::ROIManagerWidget::onOpenLayerROIToolButtonClicked()
{
  int idxLayer = m_ui->m_layerROIComboBox->currentIndex();
  QVariant varLayer = m_ui->m_layerROIComboBox->itemData(idxLayer, Qt::UserRole);
  te::map::AbstractLayerPtr layer = varLayer.value<te::map::AbstractLayerPtr>();

  std::auto_ptr<te::da::DataSet> ds = layer->getData();

  if(m_rs)
    delete m_rs;

  m_ui->m_roiSetTreeWidget->clear();

  m_sampleCounter = 0;

  m_rs = te::cl::ROISet::createROISet(ds);

  std::map<std::string, te::cl::ROI*> roiMap = m_rs->getROISet();
  std::map<std::string, te::cl::ROI*>::iterator it = roiMap.begin();

  while(it != roiMap.end())
  {
    te::cl::ROI* roi = it->second;

    std::map<std::string, te::gm::Polygon*> polyMap = roi->getPolygons();
    std::map<std::string, te::gm::Polygon*>::iterator itPoly = polyMap.begin();

    //update roi set tree
    QPixmap pix(16,16);
    pix.fill(QColor(roi->getColor().c_str()));
    QIcon icon(pix);

    QTreeWidgetItem* item = new QTreeWidgetItem(m_ui->m_roiSetTreeWidget, ROI_TREE_ITEM);
    item->setText(0, roi->getLabel().c_str());
    item->setIcon(0, icon);

    m_ui->m_roiSetTreeWidget->addTopLevelItem(item);

    while(itPoly != polyMap.end())
    {
      te::gm::Polygon* poly = itPoly->second;

      //update tree
      m_sampleCounter ++;

      QString sampleCounter;
      sampleCounter.setNum(m_sampleCounter);
      QString sampleName(tr("Sample"));
      QString fullName;
      fullName.append(sampleName);
      fullName.append(" - ");
      fullName.append(sampleCounter);

      QTreeWidgetItem* subItem = new QTreeWidgetItem(item, ROI_POLYGON_TREE_ITEM);
      subItem->setText(0, fullName);
      subItem->setData(0, Qt::UserRole, QVariant(itPoly->first.c_str()));
      subItem->setIcon(0, QIcon::fromTheme("file-vector"));
      item->addChild(subItem);
      item->setExpanded(true);

      ++itPoly;
    }
    ++it;
  }

  drawROISet();
}

void te::qt::widgets::ROIManagerWidget::onAddROIToolButtonClicked()
{
  if(m_ui->m_labelROILineEdit->text().isEmpty())
  {
    QMessageBox::warning(this, tr("Warning"), tr("ROI Label is empty."));
    return;
  }

  //get roi info
  std::string label = m_ui->m_labelROILineEdit->text().toStdString();
  std::string color = m_colorPicker->getColor().name().toStdString();

  //create roi
  te::cl::ROI* roi = new te::cl::ROI(label);
  roi->setColor(color);

  //check if roi set exist
  if(!m_rs)
  {
    m_rs = new te::cl::ROISet();
  }

  //add roi into roi set
  m_rs->addROI(roi);

  //update roi set tree
  QPixmap pix(16,16);
  pix.fill(m_colorPicker->getColor());
  QIcon icon(pix);

  QTreeWidgetItem* item = new QTreeWidgetItem(m_ui->m_roiSetTreeWidget, ROI_TREE_ITEM);
  item->setText(0, roi->getLabel().c_str());
  item->setIcon(0, icon);

  m_ui->m_roiSetTreeWidget->addTopLevelItem(item);
}

void te::qt::widgets::ROIManagerWidget::onRemoveROIToolButtonClicked()
{
  if(m_ui->m_roiSetTreeWidget->selectedItems().empty())
  {
    QMessageBox::warning(this, tr("Warning"), tr("Select a item first."));
    return;
  }

  QTreeWidgetItem* item = m_ui->m_roiSetTreeWidget->selectedItems()[0];

  if(item->type() == ROI_TREE_ITEM)
  {
    std::string label = item->text(0).toStdString();
    m_rs->removeROI(label);

    delete item;
  }
  else if(item->type() == ROI_POLYGON_TREE_ITEM)
  {
    std::string id = item->data(0, Qt::UserRole).toString().toStdString();

    QTreeWidgetItem* parent = item->parent();

    std::string label = parent->text(0).toStdString();

    te::cl::ROI* roi = m_rs->getROI(label);

    roi->removePolygon(id);

    parent->removeChild(item);
  }

  drawROISet();
}

void te::qt::widgets::ROIManagerWidget::onFileDialogToolButtonClicked()
{
  QString fileName = QFileDialog::getSaveFileName(this, tr("Save ROI Set to File"), "", tr("Shape Files (*.shp *.SHP)"));

  if (fileName.isEmpty())
    return;

  QFileInfo file(fileName);

  if(file.suffix().isEmpty())
    fileName.append(".shp");
  
  m_ui->m_roiSetNameLineEdit->setText(fileName);
}

void te::qt::widgets::ROIManagerWidget::onExportROISetToolButtonClicked()
{
  if(m_ui->m_roiSetNameLineEdit->text().isEmpty())
  {
    QMessageBox::warning(this, tr("Warning"), tr("File name not defined."));
    return;
  }
  std::string fileName = m_ui->m_roiSetNameLineEdit->text().toStdString();

  if(m_rs->getROISet().empty())
  {
    QMessageBox::warning(this, tr("Warning"), tr("ROI Set is empty."));
    return;
  }
  int srid = m_layer->getSRID();

  try
  {
    m_rs->exportToFile(fileName, srid);
  }
  catch(...)
  {
    QMessageBox::warning(this, tr("Warning"), tr("Error exporting ROI Set."));
    return;
  }

  QMessageBox::warning(this, tr("Warning"), tr("ROI Set exported successfully."));
}

void te::qt::widgets::ROIManagerWidget::onMapDisplayExtentChanged()
{
  drawROISet();
}

void te::qt::widgets::ROIManagerWidget::onGeomAquired(te::gm::Polygon* poly)
{
  if(m_ui->m_roiSetTreeWidget->selectedItems().empty())
  {
    QMessageBox::warning(this, tr("Warning"), tr("Select a ROI item first."));
    return;
  }

  QTreeWidgetItem* item = m_ui->m_roiSetTreeWidget->selectedItems()[0];

  if(item->type() != ROI_TREE_ITEM)
  {
    QMessageBox::warning(this, tr("Warning"), tr("Select a ROI item first."));
  }

  //get roi
  std::string label = item->text(0).toStdString();
  te::cl::ROI* roi = m_rs->getROI(label);

  if(roi)
  {
    //create a polygon id
    static boost::uuids::basic_random_generator<boost::mt19937> gen;
    boost::uuids::uuid u = gen();
    std::string id = boost::uuids::to_string(u);

    //add polygon into roi
    roi->addPolygon(poly, id);

    //update tree
    m_sampleCounter ++;

    QString sampleCounter;
    sampleCounter.setNum(m_sampleCounter);
    QString sampleName(tr("Sample"));
    QString fullName;
    fullName.append(sampleName);
    fullName.append(" - ");
    fullName.append(sampleCounter);

    QTreeWidgetItem* subItem = new QTreeWidgetItem(item, ROI_POLYGON_TREE_ITEM);
    subItem->setText(0, fullName);
    subItem->setData(0, Qt::UserRole, QVariant(id.c_str()));
    subItem->setIcon(0, QIcon::fromTheme("file-vector"));
    item->addChild(subItem);
    item->setExpanded(true);
  }
  
  drawROISet();
}

