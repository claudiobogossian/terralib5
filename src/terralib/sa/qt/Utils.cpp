/*  Copyright (C) 2008-2013 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/sa/qt/Utils.cpp

  \brief Utilitary interface function for spatial analysis module.
*/

//TerraLib
#include "../../color/ColorBar.h"
#include "../../color/ColorScheme.h"
#include "../../color/ColorSchemeCatalog.h"
#include "../../color/ColorSchemeCatalogManager.h"
#include "../../color/ColorSchemeGroup.h"
#include "../../color/RGBAColor.h"
#include "../../common/Globals.h"
#include "../../common/StringUtils.h"
#include "../../dataaccess/datasource/DataSource.h"
#include "../../dataaccess/datasource/DataSourceInfo.h"
#include "../../dataaccess/datasource/DataSourceInfoManager.h"
#include "../../dataaccess/datasource/DataSourceManager.h"
#include "../../maptools/Grouping.h"
#include "../../maptools/GroupingAlgorithms.h"
#include "../../maptools/GroupingItem.h"
#include "../../maptools/Utils.h"
#include "../../raster/RasterSummary.h"
#include "../../raster/RasterSummaryManager.h"
#include "../../se/ColorMap.h"
#include "../../se/Enums.h"
#include "../../se/Interpolate.h"
#include "../../se/InterpolationPoint.h"
#include "../../se/ParameterValue.h"
#include "../../se/RasterSymbolizer.h"
#include "../../se/Utils.h"
#include "../../qt/widgets/layer/utils/DataSet2Layer.h"
#include "Utils.h"

//Boost
#include <boost/filesystem.hpp>
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>

//Qt
#include <QColor>

te::da::DataSourcePtr te::sa::CreateOGRDataSource(std::string repository)
{
  //create new data source
  boost::filesystem::path uri(repository);

  std::map<std::string, std::string> dsInfo;
  dsInfo["URI"] = uri.string();

  boost::uuids::basic_random_generator<boost::mt19937> gen;
  boost::uuids::uuid u = gen();
  std::string id_ds = boost::uuids::to_string(u);

  te::da::DataSourceInfoPtr dsInfoPtr(new te::da::DataSourceInfo);
  dsInfoPtr->setConnInfo(dsInfo);
  dsInfoPtr->setTitle(uri.stem().string());
  dsInfoPtr->setAccessDriver("OGR");
  dsInfoPtr->setType("OGR");
  dsInfoPtr->setDescription(uri.string());
  dsInfoPtr->setId(id_ds);

  te::da::DataSourceInfoManager::getInstance().add(dsInfoPtr);

  return te::da::DataSourceManager::getInstance().get(id_ds, "OGR", dsInfoPtr->getConnInfo());
}

te::da::DataSourcePtr te::sa::CreateOGRDataSource(std::string path, std::string dataSetName)
{
  std::string name = path + "/" +  dataSetName + ".shp";

  return CreateOGRDataSource(name);
}

te::da::DataSourcePtr te::sa::CreateGDALDataSource(std::string path, std::string dataSetName)
{
  std::string name = path + "/" + dataSetName + ".tif";

  //create new data source
  boost::filesystem::path uri(name);

  std::map<std::string, std::string> dsInfo;
  dsInfo["URI"] = uri.string();

  boost::uuids::basic_random_generator<boost::mt19937> gen;
  boost::uuids::uuid u = gen();
  std::string id_ds = boost::uuids::to_string(u);

  te::da::DataSourceInfoPtr dsInfoPtr(new te::da::DataSourceInfo);
  dsInfoPtr->setConnInfo(dsInfo);
  dsInfoPtr->setTitle(uri.stem().string());
  dsInfoPtr->setAccessDriver("GDAL");
  dsInfoPtr->setType("GDAL");
  dsInfoPtr->setDescription(uri.string());
  dsInfoPtr->setId(id_ds);

  te::da::DataSourceInfoManager::getInstance().add(dsInfoPtr);

  return te::da::DataSourceManager::getInstance().get(id_ds, "GDAL", dsInfoPtr->getConnInfo());
}

te::map::AbstractLayerPtr te::sa::CreateLayer(te::da::DataSourcePtr ds, std::string dataSetName)
{
  te::qt::widgets::DataSet2Layer converter(ds->getId());
      
  te::da::DataSetTypePtr dt(ds->getDataSetType(dataSetName).release());

  return converter(dt);
}

void te::sa::CreateBayesGrouping(te::map::AbstractLayerPtr layer)
{
  std::string bayesAttr = TE_SA_BAYES_ATTR_NAME;
  int attrType = te::dt::DOUBLE_TYPE;
  int slices = 10;
  int prec = 6;
  int nullValues = 0;
  std::vector<double> vec;
  
  //get data
  std::auto_ptr<te::da::DataSet> ds(layer->getData());
  ds->moveBeforeFirst();

  while(ds->moveNext())
  {
    if(ds->isNull(bayesAttr))
    {
      ++nullValues;
      continue;
    }

    vec.push_back(ds->getDouble(bayesAttr));
  }

  //create grouping items
  std::vector<te::map::GroupingItem*> legend;

  te::map::GroupingByEqualSteps(vec.begin(), vec.end(), slices, legend, prec);

  std::auto_ptr<te::color::ColorBar> cb(GetColorBar("Default", "Basic", "Vermelho"));

  int legendSize = legend.size();

  std::vector<te::color::RGBAColor> colorVec;

  colorVec = cb->getSlices(legendSize);

  //create symbolizer
  int geomType = te::map::GetGeomType(layer);

  for(size_t t = 0; t < colorVec.size(); ++t)
  {
    std::vector<te::se::Symbolizer*> symbVec;

    te::se::Symbolizer* s = te::se::CreateSymbolizer((te::gm::GeomType)geomType, colorVec[t].getColor());

    symbVec.push_back(s);

    legend[t]->setSymbolizers(symbVec);
  }

  //create null grouping item
  if(nullValues != 0)
  {
    te::map::GroupingItem* legendItem = new te::map::GroupingItem;
    legendItem->setLowerLimit(te::common::Globals::sm_nanStr);
    legendItem->setUpperLimit(te::common::Globals::sm_nanStr);
    legendItem->setTitle("No Value");
    legendItem->setCount(nullValues);

    std::vector<te::se::Symbolizer*> symbVec;
    te::se::Symbolizer* s = te::se::CreateSymbolizer((te::gm::GeomType)geomType, "#dddddd");
    symbVec.push_back(s);
    legendItem->setSymbolizers(symbVec);

    legend.push_back(legendItem);
  }

  //create grouping
  te::map::Grouping* group = new te::map::Grouping(bayesAttr, te::map::EQUAL_STEPS);
  group->setPropertyType(attrType);
  group->setNumSlices(slices);
  group->setPrecision(prec);
  group->setStdDeviation(0.);
  group->setGroupingItems(legend);

  layer->setGrouping(group);
}

void te::sa::CreateKernelGrouping(te::map::AbstractLayerPtr layer, std::string kernelAttr)
{
  int attrType = te::dt::DOUBLE_TYPE;
  int slices = 20;
  int prec = 15;
  int nullValues = 0;
  std::vector<double> vec;
  
  //get data
  std::auto_ptr<te::da::DataSet> ds(layer->getData());
  ds->moveBeforeFirst();

  while(ds->moveNext())
  {
    if(ds->isNull(kernelAttr))
    {
      ++nullValues;
      continue;
    }

    vec.push_back(ds->getDouble(kernelAttr));
  }

  //create grouping items
  std::vector<te::map::GroupingItem*> legend;

  te::map::GroupingByEqualSteps(vec.begin(), vec.end(), slices, legend, prec);

  std::auto_ptr<te::color::ColorBar> cb(GetColorBar("Default", "Classification", "Kernel"));

  int legendSize = legend.size();

  std::vector<te::color::RGBAColor> colorVec;

  colorVec = cb->getSlices(legendSize);

  //create symbolizer
  int geomType = te::map::GetGeomType(layer);

  for(size_t t = 0; t < colorVec.size(); ++t)
  {
    std::vector<te::se::Symbolizer*> symbVec;

    te::se::Symbolizer* s = te::se::CreateSymbolizer((te::gm::GeomType)geomType, colorVec[t].getColor());

    symbVec.push_back(s);

    legend[t]->setSymbolizers(symbVec);
  }

  //create null grouping item
  if(nullValues != 0)
  {
    te::map::GroupingItem* legendItem = new te::map::GroupingItem;
    legendItem->setLowerLimit(te::common::Globals::sm_nanStr);
    legendItem->setUpperLimit(te::common::Globals::sm_nanStr);
    legendItem->setTitle("No Value");
    legendItem->setCount(nullValues);

    std::vector<te::se::Symbolizer*> symbVec;
    te::se::Symbolizer* s = te::se::CreateSymbolizer((te::gm::GeomType)geomType, "#dddddd");
    symbVec.push_back(s);
    legendItem->setSymbolizers(symbVec);

    legend.push_back(legendItem);
  }

  //create grouping
  te::map::Grouping* group = new te::map::Grouping(kernelAttr, te::map::EQUAL_STEPS);
  group->setPropertyType(attrType);
  group->setNumSlices(slices);
  group->setPrecision(prec);
  group->setStdDeviation(0.);
  group->setGroupingItems(legend);

  layer->setGrouping(group);
}

void te::sa::CreateKernelColorMaping(te::map::AbstractLayerPtr layer)
{
  int slices = 20;
  int prec = 15;

  //get raster
  te::rst::Raster* raster = te::map::GetRaster((te::map::DataSetLayer*)layer.get());

  if(!raster)
    return;

  const te::rst::RasterSummary* rsMin = te::rst::RasterSummaryManager::getInstance().get(raster, te::rst::SUMMARY_MIN);
  const te::rst::RasterSummary* rsMax = te::rst::RasterSummaryManager::getInstance().get(raster, te::rst::SUMMARY_MAX);
  const std::complex<double>* cmin = rsMin->at(0).m_minVal;
  const std::complex<double>* cmax = rsMax->at(0).m_maxVal;
  double min = cmin->real();
  double max = cmax->real();

  delete raster;
  
  //get color bar
  std::auto_ptr<te::color::ColorBar> cb(GetColorBar("Default", "Classification", "Kernel"));

  std::vector<te::color::RGBAColor> colorVec = cb->getSlices(slices + 1);
  
  std::vector<te::map::GroupingItem*> legVec;

  std::vector<double> vec;
  vec.push_back(min);
  vec.push_back(max);

  //create grouping
  te::map::GroupingByEqualSteps(vec.begin(), vec.end(), slices, legVec, prec);

  te::se::Interpolate* interpolate = new te::se::Interpolate();

  interpolate->setFallbackValue("#000000");
  interpolate->setLookupValue(new te::se::ParameterValue("Rasterdata"));
  interpolate->setMethodType(te::se::Interpolate::COLOR);

  for(std::size_t i = 0; i < colorVec.size(); ++i)
  {
    QColor color(colorVec[i].getRed(), colorVec[i].getGreen(), colorVec[i].getBlue(), colorVec[i].getAlpha());

    if(i == colorVec.size() - 1)
    {
      QString rangeStr = legVec[i - 1]->getUpperLimit().c_str();
      std::string colorStr = color.name().toLatin1().data();

      te::se::InterpolationPoint* ip = new te::se::InterpolationPoint();

      ip->setData(rangeStr.toDouble());
      ip->setValue(new te::se::ParameterValue(colorStr));

      interpolate->add(ip);
    }
    else
    {
      QString rangeStr = legVec[i]->getLowerLimit().c_str();
      std::string colorStr = color.name().toLatin1().data();

      te::se::InterpolationPoint* ip = new te::se::InterpolationPoint();

      ip->setData(rangeStr.toDouble());
      ip->setValue(new te::se::ParameterValue(colorStr));

      interpolate->add(ip);
    }
  }

  te::se::ColorMap* cm = new te::se::ColorMap();
  cm->setInterpolate(interpolate);

  te::se::RasterSymbolizer* rasterSymb = te::se::GetRasterSymbolizer(layer->getStyle());
  rasterSymb->setColorMap(cm);
}

void te::sa::CreateSampleGeneratorStratifiedGrouping(te::map::AbstractLayerPtr layer, std::vector<std::string> strVec)
{
  std::string stratifiedAttr = TE_SA_SPG_ATTR_CLASS_NAME;
  int attrType = te::dt::STRING_TYPE;
  int prec = 0;
  int nullValues = 0;

  //create grouping items
  std::vector<te::map::GroupingItem*> legend;

  te::map::GroupingByUniqueValues(strVec, attrType, legend, prec);

  std::auto_ptr<te::color::ColorBar> cb(GetColorBar("Default", "Classification", "Circular"));

  int legendSize = legend.size();

  std::vector<te::color::RGBAColor> colorVec;

  colorVec = cb->getSlices(legendSize);

  //create symbolizer
  int geomType = te::map::GetGeomType(layer);

  for(size_t t = 0; t < colorVec.size(); ++t)
  {
    std::vector<te::se::Symbolizer*> symbVec;

    te::se::Symbolizer* s = te::se::CreateSymbolizer((te::gm::GeomType)geomType, colorVec[t].getColor());

    symbVec.push_back(s);

    legend[t]->setSymbolizers(symbVec);
  }

  //create grouping
  te::map::Grouping* group = new te::map::Grouping(stratifiedAttr, te::map::UNIQUE_VALUE);
  group->setPropertyType(attrType);
  group->setPrecision(prec);
  group->setStdDeviation(0.);
  group->setGroupingItems(legend);

  layer->setGrouping(group);
}

void te::sa::CreateSkaterGrouping(te::map::AbstractLayerPtr layer, int nClasses)
{
  std::string skaterAttr = TE_SA_SKATER_ATTR_CLASS_NAME;
  int attrType = te::dt::INT32_TYPE;
  int prec = 0;

  //create grouping items
  std::vector<te::map::GroupingItem*> legend;

  std::vector<std::string> strVec;

  for(int i = 0; i < nClasses; ++i)
  {
    strVec.push_back(te::common::Convert2String(i));
  }

  te::map::GroupingByUniqueValues(strVec, attrType, legend, prec);

  std::auto_ptr<te::color::ColorBar> cb(GetColorBar("Default", "Classification", "Circular"));

  int legendSize = legend.size();

  std::vector<te::color::RGBAColor> colorVec;

  colorVec = cb->getSlices(legendSize);

  //create symbolizer
  int geomType = te::map::GetGeomType(layer);

  for(size_t t = 0; t < colorVec.size(); ++t)
  {
    std::vector<te::se::Symbolizer*> symbVec;

    te::se::Symbolizer* s = te::se::CreateSymbolizer((te::gm::GeomType)geomType, colorVec[t].getColor());

    symbVec.push_back(s);

    legend[t]->setSymbolizers(symbVec);
  }

  //create grouping
  te::map::Grouping* group = new te::map::Grouping(skaterAttr, te::map::UNIQUE_VALUE);
  group->setPropertyType(attrType);
  group->setPrecision(prec);
  group->setStdDeviation(0.);
  group->setGroupingItems(legend);

  layer->setGrouping(group);
}

te::color::ColorBar* te::sa::GetColorBar(std::string catalog, std::string group, std::string schema)
{
  te::color::ColorBar* cb = 0;

  te::color::ColorSchemeCatalog* csc = te::color::ColorSchemeCatalogManager::getInstance().findByName(catalog);

  if(csc)
  {
    std::vector<te::color::ColorSchemeGroup*> csgVec = csc->getColorSchemeGroups();

    for(std::size_t t = 0; t < csgVec.size(); ++t)
    {
      te::color::ColorSchemeGroup* csg = csgVec[t];

      if(csg->getName() == group)
      {
        std::vector<te::color::ColorScheme*> csVec = csg->getColorSchemes();

        for(std::size_t p = 0; p < csVec.size(); ++p)
        {
          te::color::ColorScheme* cs = csVec[p];

          if(cs->getName() == schema)
          {
            std::vector<te::color::RGBAColor>* colors = cs->getColors()[0];

            std::vector<te::color::RGBAColor>::iterator it = colors->begin();

            // create color bar
            cb = new te::color::ColorBar(*(colors->begin()), *(colors->end() - 1), 256);

            int count = 0;

            //fill color bar
            while(it != colors->end())
            {
              if(count != 0 && count != colors->size() - 1)
              {
                double pos = (1. / (colors->size() - 1)) * count;

                cb->addColor(*it, pos);
              }
              
              ++count;
              ++it;
            }
          }
        }
      }
    }
  }

  return cb;
}
