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
  \file terralib/qt/widgets/layer/info/LayerPropertiesInfo.cpp

  \brief 
*/

#include "../../../../dataaccess/dataset/DataSet.h"
#include "../../../../dataaccess/datasource/DataSourceInfoManager.h"
#include "../../../../dataaccess/utils/Utils.h"
#include "../../../../datatype/Utils.h"
#include "../../../../maptools/AbstractLayer.h"
#include "../../../../raster/Band.h"
#include "../../../../raster/BandProperty.h"
#include "../../../../raster/Utils.h"
#include "../../../../srs/SpatialReferenceSystemManager.h"
#include "../../propertybrowser/AbstractPropertyManager.h"

#include "LayerPropertiesInfo.h"

te::qt::widgets::LayerPropertiesInfo::LayerPropertiesInfo(QtTreePropertyBrowser* pb, te::map::AbstractLayer* layer) :
  te::qt::widgets::AbstractPropertyItem(pb),
  m_layer(layer)
{
  QtProperty* layerInfo_prop = te::qt::widgets::AbstractPropertyManager::getInstance().m_groupManager->addProperty(tr("Layer properties"));

  // Attributes
  QtProperty* id_prop = te::qt::widgets::AbstractPropertyManager::getInstance().m_stringManager->addProperty(tr("Id"));
  layerInfo_prop->addSubProperty(id_prop);
  id_prop->setEnabled(false);

  QtProperty* title_prop = te::qt::widgets::AbstractPropertyManager::getInstance().m_stringManager->addProperty(tr("Title"));
  layerInfo_prop->addSubProperty(title_prop);
  title_prop->setPropertyName("title");
  title_prop->setEnabled(false);

  QtProperty* srid_prop = te::qt::widgets::AbstractPropertyManager::getInstance().m_stringManager->addProperty(tr("SRID"));
  layerInfo_prop->addSubProperty(srid_prop);
  srid_prop->setPropertyName("srid");
  srid_prop->setEnabled(false);

  QtProperty* connection_prop = te::qt::widgets::AbstractPropertyManager::getInstance().m_stringManager->addProperty(tr("Connection"));
  layerInfo_prop->addSubProperty(connection_prop);
  connection_prop->setPropertyName("connection");
  connection_prop->setEnabled(false);

  QtProperty* numofitens_prop = te::qt::widgets::AbstractPropertyManager::getInstance().m_intManager->addProperty(tr("Num of Itens"));
  layerInfo_prop->addSubProperty(numofitens_prop);
  numofitens_prop->setPropertyName("numofitens");
  numofitens_prop->setEnabled(false);

  /// Bounding Box
  QtProperty* bbox_prop = te::qt::widgets::AbstractPropertyManager::getInstance().m_groupManager->addProperty(tr("Bounding box"));

  QtProperty* llx_prop = te::qt::widgets::AbstractPropertyManager::getInstance().m_stringManager->addProperty(tr("Lower left x"));
  QtProperty* lly_prop = te::qt::widgets::AbstractPropertyManager::getInstance().m_stringManager->addProperty(tr("Lower left y"));
  QtProperty* urx_prop = te::qt::widgets::AbstractPropertyManager::getInstance().m_stringManager->addProperty(tr("Upper right x"));
  QtProperty* ury_prop = te::qt::widgets::AbstractPropertyManager::getInstance().m_stringManager->addProperty(tr("Upper right y"));

  llx_prop->setEnabled(false);
  lly_prop->setEnabled(false);
  urx_prop->setEnabled(false);
  ury_prop->setEnabled(false);

  bbox_prop->addSubProperty(llx_prop);
  bbox_prop->addSubProperty(lly_prop);
  bbox_prop->addSubProperty(urx_prop);
  bbox_prop->addSubProperty(ury_prop);

  layerInfo_prop->addSubProperty(bbox_prop);

  // Visibility
  QtProperty* vis_prop = te::qt::widgets::AbstractPropertyManager::getInstance().m_enumManager->addProperty(tr("Visibility"));
  QStringList visEnum;
  visEnum << tr("Not visible") << tr("visible") << tr("Partially visible");
  te::qt::widgets::AbstractPropertyManager::getInstance().m_enumManager->setEnumNames(vis_prop, visEnum);

  layerInfo_prop->addSubProperty(vis_prop);
  vis_prop->setPropertyName("visibility");
  vis_prop->setEnabled(false);

  //setting values
  QString sridStr = QString::number(layer->getSRID());
  sridStr += QObject::tr(" -  ");
  sridStr += QString(te::srs::SpatialReferenceSystemManager::getInstance().getName(layer->getSRID()).c_str());

  te::qt::widgets::AbstractPropertyManager::getInstance().m_stringManager->setValue(id_prop, m_layer->getId().c_str());
  te::qt::widgets::AbstractPropertyManager::getInstance().m_stringManager->setValue(title_prop, m_layer->getTitle().c_str());
  te::qt::widgets::AbstractPropertyManager::getInstance().m_stringManager->setValue(srid_prop, sridStr);

  // Get the data souce info
  te::da::DataSourceInfoPtr info = te::da::DataSourceInfoManager::getInstance().get(layer->getDataSourceId());
  te::qt::widgets::AbstractPropertyManager::getInstance().m_stringManager->setValue(connection_prop, info->getConnInfoAsString().c_str());

  // Get the DataSource
  te::da::DataSourcePtr dsource = te::da::GetDataSource(layer->getDataSourceId(), false);
  te::qt::widgets::AbstractPropertyManager::getInstance().m_intManager->setValue(numofitens_prop, dsource->getNumberOfItems(layer->getDataSetName()));

  te::qt::widgets::AbstractPropertyManager::getInstance().m_stringManager->setValue(llx_prop, QString::number(m_layer->getExtent().getLowerLeftX(), 'f'));
  te::qt::widgets::AbstractPropertyManager::getInstance().m_stringManager->setValue(lly_prop, QString::number(m_layer->getExtent().getLowerLeftY(), 'f'));
  te::qt::widgets::AbstractPropertyManager::getInstance().m_stringManager->setValue(urx_prop, QString::number(m_layer->getExtent().getUpperRightX(), 'f'));
  te::qt::widgets::AbstractPropertyManager::getInstance().m_stringManager->setValue(ury_prop, QString::number(m_layer->getExtent().getUpperRightY(), 'f'));
  te::qt::widgets::AbstractPropertyManager::getInstance().m_enumManager->setValue(vis_prop, (int) m_layer->getVisibility());

  addProperty(layerInfo_prop, tr("Layer properties"), Qt::lightGray);

  //check if layer has raster
  std::auto_ptr<te::da::DataSetType> dsType = layer->getSchema();

  if(dsType->hasRaster())
  {
    setLayerRasterProperties(layer);
  }
}

te::qt::widgets::LayerPropertiesInfo::~LayerPropertiesInfo()
{
}

void te::qt::widgets::LayerPropertiesInfo::valueChanged(QtProperty* p, int value)
{
  //if(p->propertyName() == "srid")
  //  m_layer->setSRID(value);
  //else if(p->propertyName() == "visibility")
  //  m_layer->setVisibility((te::map::Visibility)value);
}

void te::qt::widgets::LayerPropertiesInfo::valueChanged(QtProperty* p, const QString& value)
{
  //if(p->propertyName() == "title")
  //  m_layer->setTitle(value.toStdString());
}

void te::qt::widgets::LayerPropertiesInfo::setLayerRasterProperties(te::map::AbstractLayer* rasterLayer)
{
  //get raster from layer
  std::auto_ptr<te::da::DataSet> dataSet = rasterLayer->getData();

  std::size_t rpos = te::da::GetFirstPropertyPos(dataSet.get(), te::dt::RASTER_TYPE);

  std::auto_ptr<te::rst::Raster> raster = dataSet->getRaster(rpos);

  if(!raster.get())
    return;

  //create top property
  QtProperty* rasterInfoProp = te::qt::widgets::AbstractPropertyManager::getInstance().m_groupManager->addProperty(tr("Raster properties"));

  //add properties
  QtProperty* nLinesProp = te::qt::widgets::AbstractPropertyManager::getInstance().m_intManager->addProperty(tr("Number of Lines"));
  QtProperty* nColumnsProp = te::qt::widgets::AbstractPropertyManager::getInstance().m_intManager->addProperty(tr("Number of Columns"));
  QtProperty* nBandsProp = te::qt::widgets::AbstractPropertyManager::getInstance().m_intManager->addProperty(tr("Number of Bands"));
  QtProperty* nResXProp = te::qt::widgets::AbstractPropertyManager::getInstance().m_doubleManager->addProperty(tr("Resolution X"));
  QtProperty* nResYProp = te::qt::widgets::AbstractPropertyManager::getInstance().m_doubleManager->addProperty(tr("Resolution Y"));
  QtProperty* nAccessProp = te::qt::widgets::AbstractPropertyManager::getInstance().m_stringManager->addProperty(tr("Access Policy"));
  QtProperty* nMultiResProp = te::qt::widgets::AbstractPropertyManager::getInstance().m_intManager->addProperty(tr("Multi Resolution Levels"));


  //add properties to parent property
  rasterInfoProp->addSubProperty(nLinesProp);
  rasterInfoProp->addSubProperty(nColumnsProp);
  rasterInfoProp->addSubProperty(nBandsProp);
  rasterInfoProp->addSubProperty(nResXProp);
  rasterInfoProp->addSubProperty(nResYProp);
  rasterInfoProp->addSubProperty(nAccessProp);
  rasterInfoProp->addSubProperty(nMultiResProp);

  //set the property state
  nLinesProp->setEnabled(false);
  nColumnsProp->setEnabled(false);
  nBandsProp->setEnabled(false);
  nResXProp->setEnabled(false);
  nResYProp->setEnabled(false);
  nAccessProp->setEnabled(false);
  nMultiResProp->setEnabled(false);

  //set properties values
  te::qt::widgets::AbstractPropertyManager::getInstance().m_intManager->setValue(nLinesProp, (int)raster->getNumberOfRows());
  te::qt::widgets::AbstractPropertyManager::getInstance().m_intManager->setValue(nColumnsProp, (int)raster->getNumberOfColumns());
  te::qt::widgets::AbstractPropertyManager::getInstance().m_intManager->setValue(nBandsProp, (int)raster->getNumberOfBands());
  te::qt::widgets::AbstractPropertyManager::getInstance().m_doubleManager->setValue(nResXProp, raster->getResolutionX());
  te::qt::widgets::AbstractPropertyManager::getInstance().m_doubleManager->setValue(nResYProp, raster->getResolutionY());
  te::qt::widgets::AbstractPropertyManager::getInstance().m_intManager->setValue(nMultiResProp, (int)raster->getMultiResLevelsCount());
  

  if(raster->getAccessPolicy() == te::common::NoAccess)
    te::qt::widgets::AbstractPropertyManager::getInstance().m_stringManager->setValue(nAccessProp, tr("No Access"));
  else if(raster->getAccessPolicy() == te::common::RAccess)
    te::qt::widgets::AbstractPropertyManager::getInstance().m_stringManager->setValue(nAccessProp, tr("Read Only Access"));
  else if(raster->getAccessPolicy() == te::common::WAccess)
    te::qt::widgets::AbstractPropertyManager::getInstance().m_stringManager->setValue(nAccessProp, tr("Write Only Access"));
  else if(raster->getAccessPolicy() == te::common::RWAccess)
    te::qt::widgets::AbstractPropertyManager::getInstance().m_stringManager->setValue(nAccessProp, tr("Read and Write Access"));

  //set information for each band
  for(std::size_t t = 0; t < raster->getNumberOfBands(); ++t)
  {
    //get band property
    te::rst::Band* band = raster->getBand(t);

    //create band top property
    QtProperty* bandProp = te::qt::widgets::AbstractPropertyManager::getInstance().m_groupManager->addProperty(tr("Band ") + QString::number(t));

    //create band properties
    QtProperty* descriptionProp = te::qt::widgets::AbstractPropertyManager::getInstance().m_stringManager->addProperty(tr("Description"));
    QtProperty* dataTypeProp = te::qt::widgets::AbstractPropertyManager::getInstance().m_stringManager->addProperty(tr("Data Type"));
    QtProperty* dummyValueProp = te::qt::widgets::AbstractPropertyManager::getInstance().m_doubleManager->addProperty(tr("Dummy Value"));
    QtProperty* colorInterpProp = te::qt::widgets::AbstractPropertyManager::getInstance().m_stringManager->addProperty(tr("Color Interpretation"));
    QtProperty* palleteInterpProp = te::qt::widgets::AbstractPropertyManager::getInstance().m_stringManager->addProperty(tr("Pallete Interpretation"));

    //add band properties to band top property
    bandProp->addSubProperty(descriptionProp);
    bandProp->addSubProperty(dataTypeProp);
    bandProp->addSubProperty(dummyValueProp);
    bandProp->addSubProperty(colorInterpProp);
    bandProp->addSubProperty(palleteInterpProp);

    //set the band properties state
    descriptionProp->setEnabled(false);
    dataTypeProp->setEnabled(false);
    dummyValueProp->setEnabled(false);
    colorInterpProp->setEnabled(false);
    palleteInterpProp->setEnabled(false);

    //set the band properties values
    te::qt::widgets::AbstractPropertyManager::getInstance().m_stringManager->setValue(descriptionProp, band->getProperty()->m_description.c_str());
    te::qt::widgets::AbstractPropertyManager::getInstance().m_stringManager->setValue(dataTypeProp, te::dt::ConvertDataTypeToString(band->getProperty()->getType()).c_str());
    te::qt::widgets::AbstractPropertyManager::getInstance().m_doubleManager->setValue(dummyValueProp, band->getProperty()->m_noDataValue);
    te::qt::widgets::AbstractPropertyManager::getInstance().m_stringManager->setValue(colorInterpProp, te::rst::ConvertColorInterpTypeToString(band->getProperty()->m_colorInterp).c_str());
    te::qt::widgets::AbstractPropertyManager::getInstance().m_stringManager->setValue(palleteInterpProp, te::rst::ConvertPalleteInterpTypeToString(band->getProperty()->m_paletteInterp).c_str());

    //add block information
    QtProperty* blockProp = te::qt::widgets::AbstractPropertyManager::getInstance().m_groupManager->addProperty(tr("Block properties"));

    QtProperty* nBlockXProp = te::qt::widgets::AbstractPropertyManager::getInstance().m_intManager->addProperty(tr("Number of blocks in X"));
    QtProperty* nBlockYProp = te::qt::widgets::AbstractPropertyManager::getInstance().m_intManager->addProperty(tr("Number of blocks in Y"));
    QtProperty* blockWProp = te::qt::widgets::AbstractPropertyManager::getInstance().m_intManager->addProperty(tr("Block Width (pixels)"));
    QtProperty* blockHProp = te::qt::widgets::AbstractPropertyManager::getInstance().m_intManager->addProperty(tr("Block Height (pixels)"));

    blockProp->addSubProperty(nBlockXProp);
    blockProp->addSubProperty(nBlockYProp);
    blockProp->addSubProperty(blockWProp);
    blockProp->addSubProperty(blockHProp);

    nBlockXProp->setEnabled(false);
    nBlockYProp->setEnabled(false);
    blockWProp->setEnabled(false);
    blockHProp->setEnabled(false);

    te::qt::widgets::AbstractPropertyManager::getInstance().m_intManager->setValue(nBlockXProp, band->getProperty()->m_nblocksx);
    te::qt::widgets::AbstractPropertyManager::getInstance().m_intManager->setValue(nBlockYProp, band->getProperty()->m_nblocksy);
    te::qt::widgets::AbstractPropertyManager::getInstance().m_intManager->setValue(blockWProp, band->getProperty()->m_blkw);
    te::qt::widgets::AbstractPropertyManager::getInstance().m_intManager->setValue(blockHProp, band->getProperty()->m_blkh);

    bandProp->addSubProperty(blockProp);

    //get category names
    if(!band->getProperty()->m_categoryNames.empty())
    {
      QtProperty* categProp = te::qt::widgets::AbstractPropertyManager::getInstance().m_groupManager->addProperty(tr("Name of categories"));

      //create a property for each category name
      for(std::size_t c = 0; c < band->getProperty()->m_categoryNames.size(); ++c)
      {
        QtProperty* cNameProp = te::qt::widgets::AbstractPropertyManager::getInstance().m_stringManager->addProperty(tr("Category ") + QString::number(c));

        categProp->addSubProperty(cNameProp);

        cNameProp->setEnabled(false);

        te::qt::widgets::AbstractPropertyManager::getInstance().m_stringManager->setValue(cNameProp, band->getProperty()->m_categoryNames[c].c_str());
      }

      bandProp->addSubProperty(categProp);
    }

    //add band top property
    rasterInfoProp->addSubProperty(bandProp);
  }

  //add top property to tree
  addProperty(rasterInfoProp, tr("Raster properties"), Qt::lightGray);
}
