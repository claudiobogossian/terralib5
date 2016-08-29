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
  \file terralib/qt/widgets/rp/ClippingWizardPage.cpp

  \brief This file defines a class for a Clipping Wizard page.
*/

// TerraLib
#include "../../../common/StringUtils.h"
#include "../../../dataaccess/dataset/DataSet.h"
#include "../../../dataaccess/dataset/ObjectIdSet.h"
#include "../../../dataaccess/utils/Utils.h"
#include "../../../geometry/Geometry.h"
#include "../../../geometry/GeometryProperty.h"
#include "../../../geometry/GeometryCollection.h"
#include "../../../geometry/LinearRing.h"
#include "../../../geometry/Polygon.h"
#include "../../../geometry/Utils.h"
#include "../../../raster/Grid.h"
#include "../../../raster/Raster.h"
#include "../canvas/Canvas.h"
#include "../canvas/MapDisplay.h"
#include "RpToolsWidget.h"

#include "ClippingWizardPage.h"
#include "ui_ClippingWizardPageForm.h"

// Qt
#include <QGridLayout>
#include <QCheckBox>
#include <QDoubleValidator>
#include <QIntValidator>
#include <QMessageBox>


// STL
#include <memory>

Q_DECLARE_METATYPE(te::map::AbstractLayerPtr);

te::qt::widgets::ClippingWizardPage::ClippingWizardPage(QWidget* parent)
  : QWizardPage(parent),
    m_ui(new Ui::ClippingWizardPageForm),
    m_layer(0)
{
// setup controls
  m_ui->setupUi(this);

  fillClippingTypes();

//build form
  QGridLayout* displayLayout = new QGridLayout(m_ui->m_frame);
  m_navigator.reset( new te::qt::widgets::RpToolsWidget(m_ui->m_frame));
  m_navigator->hideGeomTool(true);
  m_navigator->hideInfoTool(true);
  m_navigator->hidePickerTool(true);
  displayLayout->addWidget(m_navigator.get());
  displayLayout->setContentsMargins(0,0,0,0);

  connect(m_navigator.get(), SIGNAL(envelopeAcquired(te::gm::Envelope)), this, SLOT(onEnvelopeAcquired(te::gm::Envelope)));

  connect(m_navigator.get(), SIGNAL(geomAquired(te::gm::Polygon*)), this, SLOT(onGeomAquired(te::gm::Polygon*)));

  connect(m_navigator.get(), SIGNAL(mapDisplayExtentChanged()), this, SLOT(drawGeom()));

//configure page
  this->setTitle(tr("Clipping"));
  this->setSubTitle(tr("Select the type of clipping and set their specific parameters."));

  m_ui->m_llxLineEdit->setValidator(new QDoubleValidator(this));
  m_ui->m_llyLineEdit->setValidator(new QDoubleValidator(this));
  m_ui->m_urxLineEdit->setValidator(new QDoubleValidator(this));
  m_ui->m_uryLineEdit->setValidator(new QDoubleValidator(this));

  m_ui->m_startRowLineEdit->setValidator(new QIntValidator(this));
  m_ui->m_startColumnLineEdit->setValidator(new QIntValidator(this));
  m_ui->m_widthLineEdit->setValidator(new QIntValidator(this));
  m_ui->m_heightLineEdit->setValidator(new QIntValidator(this));

//Radio Button - By Drawing
  m_ui->m_drawingRadioButton->setChecked(true);
  onChangeCollectType();

//connects
  connect(m_ui->m_strategyTypeComboBox, SIGNAL(activated(int)), this, SLOT(onStrategyTypeComboBoxActivated(int)));
  connect(m_ui->m_strategyTypeComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onStrategyTypeComboBoxChanged(int)));
  connect(m_ui->m_layerComboBox, SIGNAL(activated(int)), this, SLOT(onLayerComboBoxActivated(int)));
  connect(m_ui->m_layerComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onLayerComboBoxChanged(int)));

  connect(m_ui->m_llxLineEdit, SIGNAL(editingFinished()), this, SIGNAL(completeChanged()));
  connect(m_ui->m_llyLineEdit, SIGNAL(editingFinished()), this, SIGNAL(completeChanged()));
  connect(m_ui->m_urxLineEdit, SIGNAL(editingFinished()), this, SIGNAL(completeChanged()));
  connect(m_ui->m_uryLineEdit, SIGNAL(editingFinished()), this, SIGNAL(completeChanged()));

  connect(m_ui->m_startRowLineEdit, SIGNAL(editingFinished()), this, SIGNAL(completeChanged()));
  connect(m_ui->m_startColumnLineEdit, SIGNAL(editingFinished()), this, SIGNAL(completeChanged()));
  connect(m_ui->m_widthLineEdit, SIGNAL(editingFinished()), this, SIGNAL(completeChanged()));
  connect(m_ui->m_heightLineEdit, SIGNAL(editingFinished()), this, SIGNAL(completeChanged()));

  connect(m_ui->m_startRowLineEdit, SIGNAL(editingFinished()), this, SLOT(onEnvelopeDimensionAcquired()));
  connect(m_ui->m_startColumnLineEdit, SIGNAL(editingFinished()), this, SLOT(onEnvelopeDimensionAcquired()));
  connect(m_ui->m_widthLineEdit, SIGNAL(editingFinished()), this, SLOT(onEnvelopeDimensionAcquired()));
  connect(m_ui->m_heightLineEdit, SIGNAL(editingFinished()), this, SLOT(onEnvelopeDimensionAcquired()));

  connect(m_ui->m_drawingRadioButton, SIGNAL(clicked()), this, SLOT(onChangeCollectType()));
  connect(m_ui->m_parameterRadioButton, SIGNAL(clicked()), this, SLOT(onChangeCollectType()));

  connect(m_ui->m_previewButton, SIGNAL(clicked()), this, SLOT(onPreviewClicked()));
}

te::qt::widgets::ClippingWizardPage::~ClippingWizardPage()
{

}

bool te::qt::widgets::ClippingWizardPage::isComplete() const
{
  int index = m_ui->m_strategyTypeComboBox->currentIndex();
  int type = m_ui->m_strategyTypeComboBox->itemData(index).toInt();

  if (type == CLIPPING_ROI)
  {
    if (m_ui->m_drawingRadioButton->isChecked())
    {
      if (m_envExt.isValid())
        return true;
      else
        return false;
    }
    else if(m_ui->m_llxLineEdit->text().isEmpty() ||
       m_ui->m_llyLineEdit->text().isEmpty() ||
       m_ui->m_urxLineEdit->text().isEmpty() ||
       m_ui->m_uryLineEdit->text().isEmpty()
      )
      return false;
    
    return true;
  }
  else if(type == CLIPPING_DIMENSION)
  {
    if(m_ui->m_startRowLineEdit->text().isEmpty() ||
       m_ui->m_startColumnLineEdit->text().isEmpty() ||
       m_ui->m_widthLineEdit->text().isEmpty() ||
       m_ui->m_heightLineEdit->text().isEmpty()
      )
      return false;
    
    return true;
  }
  else if(type == CLIPPING_LAYER)
  {
    if(m_ui->m_layerComboBox->currentText().isEmpty())
      return false;

    return true;
  }

  return false;
}

void te::qt::widgets::ClippingWizardPage::set(te::map::AbstractLayerPtr layer)
{
  m_layer = layer;

  std::list<te::map::AbstractLayerPtr> list;

  list.push_back(m_layer);

  m_navigator->set(m_layer);
}

void te::qt::widgets::ClippingWizardPage::setList(std::list<te::map::AbstractLayerPtr>& layerList)
{
  std::list<te::map::AbstractLayerPtr>::iterator it = layerList.begin();

  while(it != layerList.end())
  {
    te::map::AbstractLayerPtr l = *it;

    std::auto_ptr<te::da::DataSetType> dsType = l->getSchema();

    if(dsType->hasGeom())
      m_ui->m_layerComboBox->addItem(l->getTitle().c_str(), QVariant::fromValue(l));

    ++it;
  }

  if(m_ui->m_layerComboBox->count() > 0)
    onLayerComboBoxActivated(0);
}

void te::qt::widgets::ClippingWizardPage::setMapDisplay(te::qt::widgets::MapDisplay* mapDisplay)
{
  m_navigator->setMapDisplay(mapDisplay);
  m_mapDisplay = mapDisplay;
}

te::map::AbstractLayerPtr te::qt::widgets::ClippingWizardPage::get()
{
  return m_layer;
}

bool te::qt::widgets::ClippingWizardPage::isExtentClipping()
{
  int index = m_ui->m_strategyTypeComboBox->currentIndex();
  int type = m_ui->m_strategyTypeComboBox->itemData(index).toInt();

  return type == CLIPPING_ROI;
}

bool te::qt::widgets::ClippingWizardPage::isDimensionClipping()
{
  int index = m_ui->m_strategyTypeComboBox->currentIndex();
  int type = m_ui->m_strategyTypeComboBox->itemData(index).toInt();

  return type == CLIPPING_DIMENSION;
}

bool te::qt::widgets::ClippingWizardPage::isLayerClipping()
{
  int index = m_ui->m_strategyTypeComboBox->currentIndex();
  int type = m_ui->m_strategyTypeComboBox->itemData(index).toInt();

  return type == CLIPPING_LAYER;
}

bool te::qt::widgets::ClippingWizardPage::isSingleRasterResult()
{
  if(m_ui->m_aggregAllRadioButton->isChecked())
    return true;
  if(m_ui->m_groupByRadioButton->isChecked())
    return false;
  if(m_ui->m_selectedGeomRadioButton->isChecked())
    return true;

  return true;
}

void te::qt::widgets::ClippingWizardPage::onChangeCollectType()
{
  if (m_ui->m_drawingRadioButton->isChecked())
  {
    m_ui->m_llxLineEdit->clear();
    m_ui->m_llyLineEdit->clear();
    m_ui->m_urxLineEdit->clear();
    m_ui->m_uryLineEdit->clear();

    m_ui->m_llxLineEdit->setDisabled(true);
    m_ui->m_llyLineEdit->setDisabled(true);
    m_ui->m_urxLineEdit->setDisabled(true);
    m_ui->m_uryLineEdit->setDisabled(true);
    m_ui->m_previewButton->setDisabled(true);
    m_ui->m_frame->setDisabled(false);
  }
  else
  {
    m_ui->m_llxLineEdit->setDisabled(false);
    m_ui->m_llyLineEdit->setDisabled(false);
    m_ui->m_urxLineEdit->setDisabled(false);
    m_ui->m_uryLineEdit->setDisabled(false);
    m_ui->m_previewButton->setEnabled(true);
    m_ui->m_frame->setDisabled(true);
  }
}

void te::qt::widgets::ClippingWizardPage::getExtentClipping(te::gm::Envelope& env)
{
  if (m_ui->m_drawingRadioButton->isChecked())
  {
    if (m_envExt.isValid())
    {
      env.m_llx = m_envExt.getLowerLeftX();
      env.m_lly = m_envExt.getLowerLeftY();
      env.m_urx = m_envExt.getUpperRightX();
      env.m_ury = m_envExt.getUpperRightY();
    }
  }
  else
  {
    env.m_llx = m_ui->m_llxLineEdit->text().toDouble();
    env.m_lly = m_ui->m_llyLineEdit->text().toDouble();
    env.m_urx = m_ui->m_urxLineEdit->text().toDouble();
    env.m_ury = m_ui->m_uryLineEdit->text().toDouble();
  }
}


void te::qt::widgets::ClippingWizardPage::getPolygonClipping(te::gm::Polygon* poly)
{
  if (m_ui->m_drawingRadioButton->isChecked())
  {
    if (m_polyROI->isValid())
    {
      poly = m_polyROI;
    }
  }
}

void te::qt::widgets::ClippingWizardPage::getDimensionClipping(int& x, int& y, int& width, int& height)
{
  x = m_ui->m_startColumnLineEdit->text().toInt();
  y = m_ui->m_startRowLineEdit->text().toInt();
  width = m_ui->m_widthLineEdit->text().toInt();
  height = m_ui->m_heightLineEdit->text().toInt();
}

void te::qt::widgets::ClippingWizardPage::getLayerClipping(
  std::auto_ptr< te::gm::GeometryCollection >& geomColl)
{
  //get layer
  int index = m_ui->m_layerComboBox->currentIndex();
  QVariant varLayer = m_ui->m_layerComboBox->itemData(index, Qt::UserRole);
  te::map::AbstractLayerPtr layer = varLayer.value<te::map::AbstractLayerPtr>();

  //get geometry property data
  std::auto_ptr<te::da::DataSetType> dsType = layer->getSchema();
  te::gm::GeometryProperty* geomProp = te::da::GetFirstGeomProperty(dsType.get());
  std::string name = geomProp->getName();

  geomColl.reset( new te::gm::GeometryCollection(0, geomProp->getGeometryType(), geomProp->getSRID()) );

  if(m_ui->m_aggregAllRadioButton->isChecked())
  {
    std::auto_ptr<te::da::DataSet> ds = layer->getData();
    ds->moveBeforeFirst();
    
    while(ds->moveNext())
    {
      std::auto_ptr<te::gm::Geometry> geom = ds->getGeometry(name);

      if (geom.get())
      {
        te::dt::AbstractData* adCloned = geom->clone();

        geomColl->add(static_cast<te::gm::Geometry*>(adCloned));
      }
    }
  }  if(m_ui->m_selectedGeomRadioButton->isChecked())
  {
    const te::da::ObjectIdSet* oidSet = layer->getSelected();
    if(!oidSet)
    {
      QMessageBox::information(this, "Clipping", "Select the layer objects to perform the clipping operation.");
      return;
    }
    
    std::auto_ptr<te::da::DataSet> ds = layer->getData(oidSet);
    ds->moveBeforeFirst();
    while(ds->moveNext())
    {
      geomColl->add(static_cast<te::gm::Geometry*>(ds->getGeometry(name)->clone()));
    }
  }
  if(m_ui->m_groupByRadioButton->isChecked())
  {
    std::string propName = m_ui->m_layerAttrComboBox->currentText().toUtf8().data();

    std::map<std::string, std::vector<te::gm::Geometry*> > groups;
    std::map<std::string, std::vector<te::gm::Geometry*> >::iterator itg;

    std::auto_ptr<te::da::DataSet> ds = layer->getData();

    ds->moveBeforeFirst();
    while(ds->moveNext())
    {
      std::string value = ds->getAsString(propName);

      itg = groups.find(value);
      if (itg==groups.end())
      {
        std::vector<te::gm::Geometry*> geomVec;
        geomVec.push_back(static_cast<te::gm::Geometry*>(ds->getGeometry(name)->clone()));
        groups.insert(std::pair<std::string, std::vector<te::gm::Geometry*> >(value,geomVec));
      }
      else
        itg->second.push_back(static_cast<te::gm::Geometry*>(ds->getGeometry(name)->clone()));
    }

    itg = groups.begin();
    while(itg != groups.end())
    {
      te::gm::Geometry* resultGeometry(0);
      std::vector<te::gm::Geometry*> aggregVec = itg->second;
      if(aggregVec.size() < 2)
        geomColl->add(aggregVec[0]);
      if(aggregVec.size() == 2)
      {
        resultGeometry = aggregVec[0]->Union(aggregVec[1]);
        geomColl->add(resultGeometry);
      }
      if(aggregVec.size() > 2)
      {
        te::gm::GeometryCollection* geomCollTemp = new te::gm::GeometryCollection(0, te::gm::GeometryCollectionType, aggregVec[0]->getSRID());

        for(std::size_t i = 1; i < aggregVec.size(); ++i)
        {
          std::auto_ptr<te::gm::Geometry> currentGeom(aggregVec[i]);

          if(currentGeom->isValid())
            geomCollTemp->add(currentGeom.release());
        }

        resultGeometry = aggregVec[0]->Union(geomCollTemp);
        geomColl->add(resultGeometry);

      }
      ++itg;
    }

  }
}

void te::qt::widgets::ClippingWizardPage::onStrategyTypeComboBoxActivated(int index)
{
  emit completeChanged();
}

void te::qt::widgets::ClippingWizardPage::onStrategyTypeComboBoxChanged(int index)
{
  switch(index)
  {
    case 0:
      {
        m_navigator->removeVectorial();
        break;
      }
    case 1:
      {
        m_navigator->removeVectorial();
        break;
      }
    case 2:
      {
        if(m_ui->m_layerComboBox->count() > 0)
        {
          QVariant varLayer = m_ui->m_layerComboBox->itemData(0, Qt::UserRole);
          te::map::AbstractLayerPtr l = varLayer.value<te::map::AbstractLayerPtr>();

          if(l->isValid())
            m_navigator->setVectorial(l);
        }
        break;
      }
  }

  drawGeom();
}

void te::qt::widgets::ClippingWizardPage::onLayerComboBoxActivated(int index)
{
  m_ui->m_layerAttrComboBox->clear();

  QVariant varLayer = m_ui->m_layerComboBox->itemData(index, Qt::UserRole);
  te::map::AbstractLayerPtr l = varLayer.value<te::map::AbstractLayerPtr>();
  
  std::auto_ptr<te::da::DataSetType> dsType = l->getSchema();

  for(std::size_t t = 0; t < dsType->getProperties().size(); ++t)
  {
    m_ui->m_layerAttrComboBox->addItem(dsType->getProperties()[t]->getName().c_str());
  }
}

void te::qt::widgets::ClippingWizardPage::onLayerComboBoxChanged(int index)
{
  if((m_ui->m_strategyTypeComboBox->currentText() == "Layer") && m_ui->m_layerComboBox->count() > 0)
  {
    QVariant varLayer = m_ui->m_layerComboBox->itemData(index, Qt::UserRole);
    te::map::AbstractLayerPtr l = varLayer.value<te::map::AbstractLayerPtr>();

    if(l->isValid())
      m_navigator->setVectorial(l);
  }
}

void te::qt::widgets::ClippingWizardPage::onEnvelopeAcquired(te::gm::Envelope env)
{
  if (!env.isValid())
    return;

  int index = m_ui->m_strategyTypeComboBox->currentIndex();
  int type = m_ui->m_strategyTypeComboBox->itemData(index).toInt();

  if (type == CLIPPING_ROI)
  {
    m_envExt = env;
  }
  else if (type == CLIPPING_DIMENSION)
  {
    m_envDim = env;

    std::auto_ptr<te::da::DataSet> ds = m_layer->getData();

    if (ds.get())
    {
      std::size_t rpos = te::da::GetFirstPropertyPos(ds.get(), te::dt::RASTER_TYPE);
      std::auto_ptr<te::rst::Raster> inputRst = ds->getRaster(rpos);

      if (inputRst.get())
      {
        // find the envelope coords in grid coordenates
        te::gm::Coord2D ul = inputRst->getGrid()->geoToGrid((double)env.getLowerLeftX(), (double)env.getUpperRightY());
        te::gm::Coord2D lr = inputRst->getGrid()->geoToGrid((double)env.getUpperRightX(), (double)env.getLowerLeftY());

        m_ui->m_startRowLineEdit->setText(QString::number((int)ul.y));
        m_ui->m_startColumnLineEdit->setText(QString::number((int)ul.x));
        m_ui->m_heightLineEdit->setText(QString::number((int)(lr.y - ul.y)));
        m_ui->m_widthLineEdit->setText(QString::number((int)(lr.x - ul.x)));
      }
    }
  }

  drawGeom();

  emit completeChanged();
}



void te::qt::widgets::ClippingWizardPage::onEnvelopeDimensionAcquired()
{
  if (m_ui->m_startRowLineEdit->text().isEmpty() ||
    m_ui->m_startColumnLineEdit->text().isEmpty() ||
    m_ui->m_widthLineEdit->text().isEmpty() ||
    m_ui->m_heightLineEdit->text().isEmpty())
    return;

  int x = m_ui->m_startColumnLineEdit->text().toInt();
  int y = m_ui->m_startRowLineEdit->text().toInt();
  int width = m_ui->m_widthLineEdit->text().toInt();
  int height = m_ui->m_heightLineEdit->text().toInt();

  std::auto_ptr<te::da::DataSet> ds = m_layer->getData();

  if (ds.get())
  {
    std::size_t rpos = te::da::GetFirstPropertyPos(ds.get(), te::dt::RASTER_TYPE);
    std::auto_ptr<te::rst::Raster> inputRst = ds->getRaster(rpos);

    if (inputRst.get())
    {
      te::gm::Coord2D ulc = inputRst->getGrid()->gridToGeo(((double)x) - 0.5, ((double)y) - 0.5);
      te::gm::Coord2D lrc = inputRst->getGrid()->gridToGeo(((double)(x + width)) - 0.5, ((double)(y + height)) - 0.5);

      m_envDim = te::gm::Envelope(ulc.x, lrc.y, lrc.x, ulc.y);
    }
  }

  drawGeom();
}

void te::qt::widgets::ClippingWizardPage::onGeomAquired(te::gm::Polygon* poly)
{
  if (!poly->isValid())
    return;

  int index = m_ui->m_strategyTypeComboBox->currentIndex();
  int type = m_ui->m_strategyTypeComboBox->itemData(index).toInt();

  if (type == CLIPPING_ROI)
  {
    m_polyROI = poly;
  }

  drawGeom();

  emit completeChanged();
}

void te::qt::widgets::ClippingWizardPage::onPreviewClicked()
{
  if (m_ui->m_llxLineEdit->text().isEmpty())
  {
    QMessageBox::information(this, "Clipping", "Lower Left X is empty, please enter value for preview.");
    return;
  }
  else if (m_ui->m_llyLineEdit->text().isEmpty())
  {
    QMessageBox::information(this, "Clipping", "Lower Left Y is empty, please enter value for preview.");
    return;
  }
  else if (m_ui->m_urxLineEdit->text().isEmpty())
  {
    QMessageBox::information(this, "Clipping", "Upper Right X is empty, please enter value for preview.");
    return;
  }
  else if (m_ui->m_uryLineEdit->text().isEmpty())
  {
    QMessageBox::information(this, "Clipping", "Upper Right Y is empty, please enter value for preview.");
    return;
  }

  const double& llx = m_ui->m_llxLineEdit->text().toDouble();
  const double& lly = m_ui->m_llyLineEdit->text().toDouble();
  const double& urx = m_ui->m_urxLineEdit->text().toDouble();
  const double& ury = m_ui->m_uryLineEdit->text().toDouble();

  te::gm::Envelope env(llx, lly, urx, ury);

  if (!env.isValid())
    return;

  int index = m_ui->m_strategyTypeComboBox->currentIndex();
  int type = m_ui->m_strategyTypeComboBox->itemData(index).toInt();

  if (type == CLIPPING_ROI)
  {
    m_envExt = env;
  }

  drawGeom();

  emit completeChanged();
}

void te::qt::widgets::ClippingWizardPage::fillClippingTypes()
{
  m_ui->m_strategyTypeComboBox->clear();

  m_ui->m_strategyTypeComboBox->addItem(tr("Region of interest"), CLIPPING_ROI);
  m_ui->m_strategyTypeComboBox->addItem(tr("Dimension"), CLIPPING_DIMENSION);
  m_ui->m_strategyTypeComboBox->addItem(tr("Layer"), CLIPPING_LAYER);
}

void te::qt::widgets::ClippingWizardPage::drawGeom()
{
  //te::qt::widgets::MapDisplay* mapDisplay = m_navigator->getDisplay();
  
  m_mapDisplay->getDraftPixmap()->fill(Qt::transparent);

  const te::gm::Envelope& mapExt = m_mapDisplay->getExtent();

  te::qt::widgets::Canvas canvasInstance(m_mapDisplay->getDraftPixmap());
  canvasInstance.setWindow(mapExt.m_llx, mapExt.m_lly, mapExt.m_urx, mapExt.m_ury);

  canvasInstance.setPolygonContourWidth(2);
  canvasInstance.setPolygonContourColor(te::color::RGBAColor(100, 177, 216, TE_OPAQUE));
  canvasInstance.setPolygonFillColor(te::color::RGBAColor(100, 177, 216, 80));

  te::gm::Geometry* geom = 0;

  int index = m_ui->m_strategyTypeComboBox->currentIndex();
  int type = m_ui->m_strategyTypeComboBox->itemData(index).toInt();

  if(type == CLIPPING_ROI)
  {
    if(m_envExt.isValid())
      geom = te::gm::GetGeomFromEnvelope(&m_envExt, m_layer->getSRID());
  }
  else if(type == CLIPPING_DIMENSION)
  {
    if(m_envDim.isValid())
     geom = te::gm::GetGeomFromEnvelope(&m_envDim, m_layer->getSRID());
  }

  if(geom)
  {
    canvasInstance.draw(geom);
    delete geom;
  }

  m_mapDisplay->repaint();
}

void te::qt::widgets::ClippingWizardPage::clearCanvas()
{
  te::qt::widgets::Canvas canvasInstance(m_mapDisplay->getDraftPixmap());

  canvasInstance.clear();

  m_mapDisplay->repaint();
}