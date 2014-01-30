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
#include "../../../dataaccess/utils/Utils.h"
#include "../../../geometry/Coord2D.h"
#include "../../../geometry/Geometry.h"
#include "../../../geometry/GeometryProperty.h"
#include "../../../geometry/MultiPolygon.h"
#include "../../../geometry/Point.h"
#include "../../../geometry/Utils.h"
#include "../../../se/PolygonSymbolizer.h"
#include "../../../se/Fill.h"
#include "../../../se/Rule.h"
#include "../../../se/Style.h"
#include "../../../se/Symbolizer.h"
#include "../../../se/Utils.h"
#include "../canvas/Canvas.h"
#include "../canvas/MapDisplay.h"
#include "../rp/RasterNavigatorWidget.h"
#include "../utils/ColorPickerToolButton.h"
#include "ROIManagerWidget.h"
#include "ui_ROIManagerWidgetForm.h"

// Qt
#include <QtCore/QPointF>
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
  m_ui->m_vectorLayerToolButton->setIcon(QIcon::fromTheme("map-draw"));

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

  m_navigator->setSelectionMode(true);
  m_navigator->hidePickerTool(true);
  m_navigator->hideInfoTool(true);

  //connects
  connect(m_ui->m_openLayerROIToolButton, SIGNAL(clicked()), this, SLOT(onOpenLayerROIToolButtonClicked()));
  connect(m_ui->m_addROIToolButton, SIGNAL(clicked()), this, SLOT(onAddROIToolButtonClicked()));
  connect(m_ui->m_removeROIToolButton, SIGNAL(clicked()), this, SLOT(onRemoveROIToolButtonClicked()));
  connect(m_ui->m_fileDialogToolButton, SIGNAL(clicked()), this, SLOT(onFileDialogToolButtonClicked()));
  connect(m_ui->m_exportROISetToolButton, SIGNAL(clicked()), this, SLOT(onExportROISetToolButtonClicked()));
  connect(m_ui->m_vectorLayerToolButton, SIGNAL(clicked(bool)), this, SLOT(onVectorLayerToolButtonClicked(bool)));
  connect(m_ui->m_roiSetTreeWidget, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(onROITreItemClicked(QTreeWidgetItem*, int)));
  connect(m_navigator.get(), SIGNAL(mapDisplayExtentChanged()), this, SLOT(onMapDisplayExtentChanged()));
  connect(m_navigator.get(), SIGNAL(geomAquired(te::gm::Polygon*)), this, SLOT(onGeomAquired(te::gm::Polygon*)));
  connect(m_navigator.get(), SIGNAL(pointPicked(double, double)), this, SLOT(onPointPicked(double, double)));
}

te::qt::widgets::ROIManagerWidget::~ROIManagerWidget()
{
  if(m_vectorLayer.get())
  {
    m_vectorLayer->setVisibility(m_vectorLayerVisibility);

    te::se::Style* s = m_vectorLayer->getStyle();
    te::se::Rule* r = s->getRule(0);

    r->setSymbolizer(0, m_symb);
  }
}

Ui::ROIManagerWidgetForm* te::qt::widgets::ROIManagerWidget::getForm() const
{
  return m_ui.get();
}

void te::qt::widgets::ROIManagerWidget::setList(std::list<te::map::AbstractLayerPtr>& layerList)
{
  m_ui->m_layerROIComboBox->clear();
  m_ui->m_vectorLayerComboBox->clear();

  std::list<te::map::AbstractLayerPtr>::iterator it = layerList.begin();

  while(it != layerList.end())
  {
    te::map::AbstractLayerPtr l = *it;

    std::auto_ptr<te::da::DataSetType> dsType = l->getSchema();

    if(dsType->hasGeom())
    {
      te::gm::GeometryProperty* gp = te::da::GetFirstGeomProperty(dsType.get());
      
      if(gp && gp->getGeometryType() == te::gm::MultiPolygonType)
        m_ui->m_vectorLayerComboBox->addItem(it->get()->getTitle().c_str(), QVariant::fromValue(l));


      if(dsType->getProperties().size() == 5 &&
         dsType->getProperty(1)->getName() == TE_CL_ROI_GEOM_ID_NAME &&
         dsType->getProperty(2)->getName() == TE_CL_ROI_LABEL_NAME &&
         dsType->getProperty(3)->getName() == TE_CL_ROI_COLOR_NAME)
      {
        m_ui->m_layerROIComboBox->addItem(it->get()->getTitle().c_str(), QVariant::fromValue(l));
      }
    }

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

  std::map<std::string, te::cl::ROI*> roiMap = m_rs->getROISet();
  std::map<std::string, te::cl::ROI*>::iterator it = roiMap.begin();

  while(it != roiMap.end())
  {
    te::cl::ROI* roi = it->second;

    std::map<std::string, te::gm::Polygon*> polyMap = roi->getPolygons();
    std::map<std::string, te::gm::Polygon*>::iterator itPoly = polyMap.begin();

    te::color::RGBAColor rgba(roi->getColor());

    canvasInstance.setPolygonContourWidth(2);
    canvasInstance.setPolygonContourColor(rgba);
    canvasInstance.setPolygonFillColor(te::color::RGBAColor(rgba.getRed(), rgba.getGreen(), rgba.getBlue(), 80));

    while(itPoly != polyMap.end())
    {
      te::gm::Polygon* poly = itPoly->second;

      canvasInstance.draw(poly);

      ++itPoly;
    }
    ++it;
  }

  //draw selected
  if(!m_ui->m_roiSetTreeWidget->selectedItems().isEmpty())
  {
    QTreeWidgetItem* item = m_ui->m_roiSetTreeWidget->selectedItems()[0];

    if(item->type() == ROI_POLYGON_TREE_ITEM)
    {
      std::string id = item->data(0, Qt::UserRole).toString().toStdString();

      QTreeWidgetItem* parent = item->parent();

      std::string label = parent->text(0).toStdString();

      te::cl::ROI* roi = m_rs->getROI(label);

      te::color::RGBAColor rgba(roi->getColor());

      te::gm::Polygon* p = roi->getPolygons()[id];

      canvasInstance.setPolygonContourWidth(1);
      canvasInstance.setPolygonContourColor(rgba);
      canvasInstance.setPolygonFillColor(rgba);

      canvasInstance.draw(p);
    }
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

  try
  {
    m_rs = te::cl::ROISet::createROISet(ds);
  }
  catch(...)
  {
    QMessageBox::warning(this, tr("Warning"), tr("Error extracting ROISet Information. Invalid layer."));
    return;
  }

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

  emit roiSetChanged(m_rs);

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

  m_ui->m_labelROILineEdit->clear();

  emit roiSetChanged(m_rs);
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

  if(m_rs && m_rs->getROISet().empty())
  {
    m_sampleCounter = 0;
  }

  drawROISet();

  emit roiSetChanged(m_rs);
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

void te::qt::widgets::ROIManagerWidget::onROITreItemClicked(QTreeWidgetItem* item, int column)
{
  drawROISet();

  if(!item || item->type() != ROI_POLYGON_TREE_ITEM)
    return;

  te::qt::widgets::MapDisplay* mapDisplay = m_navigator->getDisplay();
  const te::gm::Envelope& mapExt = mapDisplay->getExtent();

  te::qt::widgets::Canvas canvasInstance(mapDisplay->getDraftPixmap());
  canvasInstance.setWindow(mapExt.m_llx, mapExt.m_lly, mapExt.m_urx, mapExt.m_ury);

  std::string id = item->data(0, Qt::UserRole).toString().toStdString();

  QTreeWidgetItem* parent = item->parent();

  std::string label = parent->text(0).toStdString();

  te::cl::ROI* roi = m_rs->getROI(label);

  te::color::RGBAColor rgba(roi->getColor());

  te::gm::Polygon* p = roi->getPolygons()[id];

  canvasInstance.setPolygonContourWidth(1);
  canvasInstance.setPolygonContourColor(rgba);
  canvasInstance.setPolygonFillColor(rgba);

  canvasInstance.draw(p);

  mapDisplay->repaint();

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

void te::qt::widgets::ROIManagerWidget::onVectorLayerToolButtonClicked(bool flag)
{
  if(flag)
  {
    int idxLayer = m_ui->m_vectorLayerComboBox->currentIndex();
    QVariant varLayer = m_ui->m_vectorLayerComboBox->itemData(idxLayer, Qt::UserRole);
    te::map::AbstractLayerPtr layer = varLayer.value<te::map::AbstractLayerPtr>();

    m_vectorLayer = layer;
    m_vectorLayerVisibility = m_layer->getVisibility();
    m_vectorLayer->setVisibility(te::map::VISIBLE);

    te::se::Style* s = m_vectorLayer->getStyle();
    te::se::Rule* r = s->getRule(0);
    const te::se::Symbolizer* symb = r->getSymbolizer(0);
    
    m_symb = symb->clone();
    
    if(symb->getType() == "PolygonSymbolizer")
    {
      te::se::PolygonSymbolizer* ps = (te::se::PolygonSymbolizer*)(symb)->clone();

      te::se::Fill* fill = ps->getFill()->clone();
      fill->setOpacity("0.2");

      ps->setFill(fill);

      r->setSymbolizer(0, ps);
    }

    m_navigator->setVectorial(m_vectorLayer);
    m_navigator->hidePickerTool(false);
  }
  else
  {
    m_navigator->removeVectorial();
    m_navigator->hidePickerTool(true);
  }
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

  emit roiSetChanged(m_rs);
  
  drawROISet();
}

void te::qt::widgets::ROIManagerWidget::onPointPicked(double x, double y)
{
  if(!m_ui->m_vectorLayerToolButton->isChecked())
    return;

  te::qt::widgets::MapDisplay* mapDisplay = m_navigator->getDisplay();

  int idxLayer = m_ui->m_vectorLayerComboBox->currentIndex();
  QVariant varLayer = m_ui->m_vectorLayerComboBox->itemData(idxLayer, Qt::UserRole);
  te::map::AbstractLayerPtr layer = varLayer.value<te::map::AbstractLayerPtr>();

  //create envelope
  te::gm::Envelope envelope(x, y, x, y);

  //reproject rect if needs
  te::gm::Envelope reprojectedEnvelope(envelope);

  if((layer->getSRID() != TE_UNKNOWN_SRS) && (mapDisplay->getSRID() != TE_UNKNOWN_SRS) && (layer->getSRID() != mapDisplay->getSRID()))
    reprojectedEnvelope.transform(mapDisplay->getSRID(), layer->getSRID());

  if(!reprojectedEnvelope.intersects(layer->getExtent()))
    return;

  //get geometries
  try
  {
    // Gets the layer schema
    std::auto_ptr<const te::map::LayerSchema> schema(layer->getSchema());
    if(!schema->hasGeom())
      return;

    te::gm::GeometryProperty* gp = te::da::GetFirstGeomProperty(schema.get());

    // Gets the dataset
    std::auto_ptr<te::da::DataSet> dataset = layer->getData(gp->getName(), &reprojectedEnvelope, te::gm::INTERSECTS);

    // Generates a geometry from the given extent. It will be used to refine the results
    std::auto_ptr<te::gm::Geometry> geometryFromEnvelope(te::gm::GetGeomFromEnvelope(&reprojectedEnvelope, layer->getSRID()));

    // The restriction point. It will be used to refine the results
    te::gm::Coord2D center = reprojectedEnvelope.getCenter();
    te::gm::Point point(center.x, center.y, layer->getSRID());

    while(dataset->moveNext())
    {
      std::auto_ptr<te::gm::Geometry> g(dataset->getGeometry(gp->getName()));

      if(g->contains(&point) || g->crosses(geometryFromEnvelope.get()) || geometryFromEnvelope->contains(g.get()))
      {
        te::gm::MultiPolygon* mp = (te::gm::MultiPolygon*)dataset->getGeometry(gp->getName()).release();
        te::gm::Polygon* p = (te::gm::Polygon*)mp->getGeometries()[0];

        onGeomAquired(p);
        
        break;
      }
    }
  }
  catch(...)
  {
  }
}

