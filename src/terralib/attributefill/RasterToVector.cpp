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
 \file RasterToVector.cpp
 */

#include "../common/progress/TaskProgress.h"
#include "../common/Translator.h"

#include "../dataaccess/dataset/DataSet.h"
#include "../dataaccess/dataset/DataSetAdapter.h"

#include "../dataaccess/dataset/DataSetType.h"
#include "../dataaccess/dataset/DataSetTypeConverter.h"
#include "../dataaccess/dataset/ObjectIdSet.h"
#include "../dataaccess/datasource/DataSource.h"
#include "../dataaccess/datasource/DataSourceCapabilities.h"
#include "../dataaccess/utils/Utils.h"

#include "../datatype/Property.h"
#include "../datatype/StringProperty.h"

#include "../geometry.h"

#include "../memory/DataSetItem.h"

#include "../raster/RasterProperty.h"
#include "../raster/Utils.h"

#include "../rp/RasterAttributes.h"
#include "../rp/Texture.h"

#include "../statistics.h"

#include "Exception.h"
#include "RasterToVector.h"

// Boost
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>


te::attributefill::RasterToVector::RasterToVector()
{
}

void te::attributefill::RasterToVector::setInput(te::rst::Raster* inRaster,
                                              te::da::DataSourcePtr inVectorDsrc,
                                              std::string inVectorName,
                                              std::auto_ptr<te::da::DataSetTypeConverter> inVectorDsType,
                                              const te::da::ObjectIdSet* oidSet)
{
  m_inRaster = inRaster;
  m_inVectorDsrc = inVectorDsrc;
  m_inVectorName = inVectorName;
  m_inVectorDsType = inVectorDsType;
  m_oidSet = oidSet;
}

void te::attributefill::RasterToVector::setParams(std::vector<unsigned int> bands,
                                               std::vector<te::stat::StatisticalSummary> statSum,
                                               bool texture)
{
  m_bands = bands;
  m_statSum = statSum;
  m_texture = texture;
}

void te::attributefill::RasterToVector::setOutput(te::da::DataSourcePtr outDsrc, std::string dsName)
{
  m_outDsrc = outDsrc;
  m_outDset = dsName;
}

bool te::attributefill::RasterToVector::paramsAreValid()
{
  if (!m_inVectorDsType.get())
    return false;
  
  if (!m_inVectorDsType->getResult()->hasGeom())
    return false;

  if (m_outDset.empty() || !m_outDsrc.get())
    return false;
  
  return true;
}

bool te::attributefill::RasterToVector::run()
{
// prepare raster
  double resX = m_inRaster->getResolutionX();
  double resY = m_inRaster->getResolutionY();

  te::gm::Envelope* env = m_inRaster->getExtent();

// raster Attributes
  te::rp::RasterAttributes* rasterAtt = 0;


// prepare vector
  te::gm::GeometryProperty* vectorProp = te::da::GetFirstGeomProperty(m_inVectorDsType->getResult());
  std::size_t geomIdx = boost::lexical_cast<std::size_t>(m_inVectorDsType->getResult()->getPropertyPosition(vectorProp->getName()));

  std::auto_ptr<te::da::DataSet> dataSetVector;
  if(m_oidSet == 0)
    dataSetVector = m_inVectorDsrc->getDataSet(m_inVectorName);
  else
    dataSetVector = m_inVectorDsrc->getDataSet(m_inVectorName, m_oidSet);

  std::auto_ptr<te::da::DataSetAdapter> dsVector(te::da::CreateAdapter(dataSetVector.get(), m_inVectorDsType.get()));

// Verify if mode was checked to calculating.
  bool mode = false;
  std::vector<te::stat::StatisticalSummary>::iterator it_mode = std::find(m_statSum.begin(), m_statSum.end(), te::stat::MODE);
  if (it_mode != m_statSum.end())
    mode = true;
  

// Parameters to get the percentage of classes by area
  bool percentByArea = false;
  std::vector<std::set<int> > pixelDistinct;
  std::vector<te::stat::StatisticalSummary>::iterator it = std::find(m_statSum.begin(), m_statSum.end(), te::stat::PERCENT_EACH_CLASS_BY_AREA);
  if (it != m_statSum.end())
  {
    pixelDistinct = getPixelDistinct(m_inRaster, m_bands);
    percentByArea = true;
  }


// get output DataSetType.
  std::auto_ptr<te::da::DataSetType> outDsType;
  if (percentByArea)
    outDsType = getDataSetType(pixelDistinct);
  else
    outDsType = getDataSetType();
// create the output dataset in memory
  std::auto_ptr<te::mem::DataSet> outDataset(new te::mem::DataSet(outDsType.get()));


// task progress
  te::common::TaskProgress task("Processing Operation...");
  task.setTotalSteps((int)dsVector->size() * (int)m_statSum.size() * (int)m_bands.size());
  task.useTimer(true);

  bool remap = false;

  if (m_inRaster->getSRID() != vectorProp->getSRID())
    remap = true;

  dsVector->moveBeforeFirst();
  while(dsVector->moveNext())
  {
    te::mem::DataSetItem* outDSetItem = new te::mem::DataSetItem(outDataset.get());
    
    std::vector<te::dt::Property*> vecProp = m_inVectorDsType->getResult()->getProperties();
    for(std::size_t i = 0; i < vecProp.size(); ++i)
    {
      if (!dsVector->isNull(i))
        outDSetItem->setValue(i, dsVector->getValue(i).release());
    }

// Geometry
    std::auto_ptr<te::gm::Geometry> geom = dsVector->getGeometry(geomIdx);
    if (!geom->isValid())
      continue;

    if (remap)
      geom->transform(m_inRaster->getSRID());


// Add Item in DataSet if geom does not intersects the raster envelope and continue to the next dataSet item.
    if(!env->intersects(*geom->getMBR()))
    {
      outDataset->add(outDSetItem);
      continue;
    }

    double area = 0;

// Values from raster
    std::vector<std::vector<double> > valuesFromRaster;
    valuesFromRaster.resize(m_bands.size());

// Contains
    bool contains = true;
    bool isPoint = false;

    switch (geom->getGeomTypeId())
    {
      case te::gm::MultiPolygonType:
      {
                                     te::gm::MultiPolygon* mPolygon = dynamic_cast< te::gm::MultiPolygon* >(geom.get());
                                     contains = env->contains(*mPolygon->getMBR());

                                     if (percentByArea)
                                       area = mPolygon->getArea();

                                     std::size_t n_geom = mPolygon->getNumGeometries();

                                     for (std::size_t n = 0; n < n_geom; ++n)
                                     {
                                       te::gm::Polygon* polygon = dynamic_cast< te::gm::Polygon* >(mPolygon->getGeometryN(n));
                                       std::vector<std::vector<double> > tempValues = rasterAtt->getValuesFromRaster(*m_inRaster, *polygon, m_bands);


                                       for (std::size_t band = 0; band < tempValues.size(); ++band)
                                       {
                                         std::vector<double>::iterator it;
                                         it = valuesFromRaster[band].end();

                                         valuesFromRaster[band].insert(it,
                                           tempValues[band].begin(),
                                           tempValues[band].end());
                                       }
                                     }

                                     isPoint = false;

                                     break;
      }
      case te::gm::PolygonType:
      {
                                te::gm::Polygon* polygon = dynamic_cast< te::gm::Polygon* >(geom.get());
                                contains = env->contains(*polygon->getMBR());

                                if (percentByArea)
                                  area = polygon->getArea();

                                valuesFromRaster = rasterAtt->getValuesFromRaster(*m_inRaster, *polygon, m_bands);

                                isPoint = false;

                                break;
      }
      case te::gm::MultiPointType:
      {
                              te::gm::MultiPoint* mPoint = dynamic_cast<te::gm::MultiPoint*>(geom.get());

                              std::size_t n_geom = mPoint->getNumGeometries();

                              for (std::size_t n = 0; n < n_geom; ++n)
                              {
                                te::gm::Point* point = dynamic_cast<te::gm::Point*>(mPoint->getGeometryN(n));

                                const double coordX = point->getX();
                                const double coordY = point->getY();

                                te::gm::Coord2D coord2d = m_inRaster->getGrid()->geoToGrid(coordX, coordY);

                                std::vector<double> values;

                                for (std::size_t band = 0; band < m_inRaster->getNumberOfBands(); ++band)
                                {
                                  double value;
                                  m_inRaster->getValue((int)coord2d.getX(), (int)coord2d.getY(), value, band);

                                  values.push_back(value);

                                  std::vector<double>::iterator it;
                                  it = valuesFromRaster[band].end();

                                  valuesFromRaster[band].insert(it,
                                                                values.begin(),
                                                                values.end());

                                }

                                isPoint = true;
                              }
                              break;
      }
      default:
        continue;
    }

    std::size_t init_index = m_inVectorDsType->getResult()->getProperties().size();

// Statistics set value
    if (!isPoint)
    {
      for (std::size_t band = 0; band < valuesFromRaster.size(); ++band)
      {
        te::stat::NumericStatisticalSummary summary = rasterAtt->getStatistics(valuesFromRaster[band]);

        if (mode)
          te::stat::Mode(valuesFromRaster[band], summary);

        if (percentByArea)
          te::stat::GetPercentOfEachClassByArea(valuesFromRaster[band], resX, resY, area, summary, contains);

        std::size_t current_index = init_index + m_statSum.size();

        for (std::size_t it = 0, i = init_index; i < current_index; ++it, ++i)
        {
          te::stat::StatisticalSummary ss = m_statSum[it];

          switch (ss)
          {
          case te::stat::MIN_VALUE:
            outDSetItem->setDouble(i, summary.m_minVal);
            break;
          case te::stat::MAX_VALUE:
            outDSetItem->setDouble(i, summary.m_maxVal);
            break;
          case te::stat::COUNT:
            outDSetItem->setDouble(i, summary.m_count);
            break;
          case te::stat::VALID_COUNT:
            outDSetItem->setDouble(i, summary.m_validCount);
            break;
          case te::stat::MEAN:
            outDSetItem->setDouble(i, summary.m_mean);
            break;
          case te::stat::SUM:
            outDSetItem->setDouble(i, summary.m_sum);
            break;
          case te::stat::STANDARD_DEVIATION:
            outDSetItem->setDouble(i, summary.m_stdDeviation);
            break;
          case te::stat::VARIANCE:
            outDSetItem->setDouble(i, summary.m_variance);
            break;
          case te::stat::SKEWNESS:
            outDSetItem->setDouble(i, summary.m_skewness);
            break;
          case te::stat::KURTOSIS:
            outDSetItem->setDouble(i, summary.m_kurtosis);
            break;
          case te::stat::AMPLITUDE:
            outDSetItem->setDouble(i, summary.m_amplitude);
            break;
          case te::stat::MEDIAN:
            outDSetItem->setDouble(i, summary.m_median);
            break;
          case te::stat::VAR_COEFF:
            outDSetItem->setDouble(i, summary.m_varCoeff);
            break;
          case te::stat::MODE:
          {
                               std::string mode;

                               if (!summary.m_mode.empty())
                               {
                                 mode = boost::lexical_cast<std::string>(summary.m_mode[0]);
                                 for (std::size_t m = 1; m < summary.m_mode.size(); ++m)
                                 {
                                   mode += ",";
                                   mode += boost::lexical_cast<std::string>(summary.m_mode[m]);
                                 }
                                 outDSetItem->setString(i, mode);
                               }
                               else
                               {
                                 outDSetItem->setString(i, "");
                               }
                               break;
          }
          case te::stat::PERCENT_EACH_CLASS_BY_AREA:
          {
                                                     std::set<int>::iterator itPixelDistinct = pixelDistinct[band].begin();
                                                     std::map<double, double>::iterator itPercent = summary.m_percentEachClass.begin();

                                                     while (itPixelDistinct != pixelDistinct[band].end())
                                                     {
                                                       if (itPercent != summary.m_percentEachClass.end())
                                                       {
                                                         std::string name = outDSetItem->getPropertyName(i);
                                                         std::vector<std::string> splitString;
                                                         boost::split(splitString, name, boost::is_any_of("_"));
                                                         if (splitString[1] == boost::lexical_cast<std::string>(itPercent->first))
                                                         {
                                                           outDSetItem->setDouble(i, itPercent->second);
                                                           ++itPercent;
                                                         }
                                                         else
                                                         {
                                                           outDSetItem->setDouble(i, 0);
                                                         }
                                                       }
                                                       else
                                                       {
                                                         outDSetItem->setDouble(i, 0);
                                                       }
                                                       ++itPixelDistinct;
                                                       ++i;
                                                     }
                                                     current_index += pixelDistinct[band].size() - 1;
                                                     break;
          }
          default:
            continue;
          }
          task.pulse();
        }

        // texture
        std::vector<te::rp::Texture> metrics;
        init_index = current_index;

        if (m_texture == true)
        {
          metrics = getTexture(m_inRaster, geom.get(), (int)m_bands.size());
          current_index += 5;
          for (std::size_t t = 0, i = init_index; i < current_index; ++t, ++i)
          {
            switch (t)
            {
            case 0:
            {
                    outDSetItem->setDouble(i, metrics[band].m_contrast);
                    break;
            }
            case 1:
            {
                    outDSetItem->setDouble(i, metrics[band].m_dissimilarity);
                    break;
            }
            case 2:
            {
                    outDSetItem->setDouble(i, metrics[band].m_energy);
                    break;
            }
            case 3:
            {
                    outDSetItem->setDouble(i, metrics[band].m_entropy);
                    break;
            }
            case 4:
            {
                    outDSetItem->setDouble(i, metrics[band].m_homogeneity);
                    break;
            }
            }
          }
        }

        init_index = current_index;
      }
    }
    else
    {
      for (std::size_t i = 0; i < valuesFromRaster.size(); ++i)
      {
        for (std::size_t j = 0; j < valuesFromRaster[i].size(); ++j)
        {
          outDSetItem->setDouble(init_index, valuesFromRaster[i][j]);
        }
      }
    }

    outDataset->add(outDSetItem);

    if (task.isActive() == false)
      throw te::attributefill::Exception(TE_TR("Operation canceled!"));
  }

  return save(outDataset,outDsType);
}

std::vector<std::set<int> > te::attributefill::RasterToVector::getPixelDistinct(te::rst::Raster* rst, std::vector<unsigned int> bands)
{
  std::vector<std::set<int> > pixelDistinct;
  int numRows = rst->getNumberOfRows();
  int numCols = rst->getNumberOfColumns();

  for (std::size_t b = 0; b < bands.size(); ++b)
  {
    std::set<int> pixelBandDistinct;
  
    for (int i = 0; i < numRows; ++i)
    {
      for (int j = 0; j < numCols; ++j)
      {
        double value;
        rst->getValue(j, i, value, bands[b]);
        pixelBandDistinct.insert((int)value);
      }
    }

    pixelDistinct.push_back(pixelBandDistinct);
  }

  return pixelDistinct;
}

std::auto_ptr<te::da::DataSetType> te::attributefill::RasterToVector::getDataSetType(std::vector<std::set<int> > pixelDistinct)
{
  std::auto_ptr<te::da::DataSetType> outdsType(new te::da::DataSetType(*m_inVectorDsType->getResult()));
  outdsType->setCompositeName(m_outDset);
  outdsType->setName(m_outDset);
  outdsType->setTitle(m_outDset);

  te::da::PrimaryKey* pk = outdsType->getPrimaryKey();
  
  std::string name = pk->getName();
  name += "_" + m_outDset;
  pk->setName(name);

  for (std::size_t b = 0; b < m_bands.size(); ++b)
  {
    if (!m_statSum.empty() || m_texture == true)
    {
      for (std::size_t i = 0; i < m_statSum.size(); ++i)
      {
        te::dt::SimpleProperty* prop;
        switch (m_statSum[i])
        {
        case 0:
          prop = new te::dt::SimpleProperty("B" + boost::lexical_cast<std::string>(m_bands[b]) + "_Min_Value", te::dt::DOUBLE_TYPE);
          outdsType->add(prop);
          break;
        case 1:
          prop = new te::dt::SimpleProperty("B" + boost::lexical_cast<std::string>(m_bands[b]) + "_Max_Value", te::dt::DOUBLE_TYPE);
          outdsType->add(prop);
          break;
        case 2:
          prop = new te::dt::SimpleProperty("B" + boost::lexical_cast<std::string>(m_bands[b]) + "_Mean", te::dt::DOUBLE_TYPE);
          outdsType->add(prop);
          break;
        case 3:
          prop = new te::dt::SimpleProperty("B" + boost::lexical_cast<std::string>(m_bands[b]) + "_Sum", te::dt::DOUBLE_TYPE);
          outdsType->add(prop);
          break;
        case 4:
          prop = new te::dt::SimpleProperty("B" + boost::lexical_cast<std::string>(m_bands[b]) + "_Count", te::dt::DOUBLE_TYPE);
          outdsType->add(prop);
          break;
        case 5:
          prop = new te::dt::SimpleProperty("B" + boost::lexical_cast<std::string>(m_bands[b]) + "_Valid_Count", te::dt::DOUBLE_TYPE);
          outdsType->add(prop);
          break;
        case 6:
          prop = new te::dt::SimpleProperty("B" + boost::lexical_cast<std::string>(m_bands[b]) + "_Standard_Deviation", te::dt::DOUBLE_TYPE);
          outdsType->add(prop);
          break;
        case 7:
          prop = new te::dt::SimpleProperty("B" + boost::lexical_cast<std::string>(m_bands[b]) + "_Variance", te::dt::DOUBLE_TYPE);
          outdsType->add(prop);
          break;
        case 8:
          prop = new te::dt::SimpleProperty("B" + boost::lexical_cast<std::string>(m_bands[b]) + "_Skewness", te::dt::DOUBLE_TYPE);
          outdsType->add(prop);
          break;
        case 9:
          prop = new te::dt::SimpleProperty("B" + boost::lexical_cast<std::string>(m_bands[b]) + "_Kurtosis", te::dt::DOUBLE_TYPE);
          outdsType->add(prop);
          break;
        case 10:
          prop = new te::dt::SimpleProperty("B" + boost::lexical_cast<std::string>(m_bands[b]) + "_Amplitude", te::dt::DOUBLE_TYPE);
          outdsType->add(prop);
          break;
        case 11:
          prop = new te::dt::SimpleProperty("B" + boost::lexical_cast<std::string>(m_bands[b]) + "_Median", te::dt::DOUBLE_TYPE);
          outdsType->add(prop);
          break;
        case 12:
          prop = new te::dt::SimpleProperty("B" + boost::lexical_cast<std::string>(m_bands[b]) + "_Var_Coeff", te::dt::DOUBLE_TYPE);
          outdsType->add(prop);
          break;
        case 13:
          prop = new te::dt::StringProperty("B" + boost::lexical_cast<std::string>(m_bands[b]) + "_Mode");
          outdsType->add(prop);
          break;
        case 14:
        {
                 std::set<int>::iterator it = pixelDistinct[b].begin();
                 while (it != pixelDistinct[b].end())
                 {
                   prop = new te::dt::SimpleProperty("B" + boost::lexical_cast<std::string>(m_bands[b]) + "_" + boost::lexical_cast<std::string>(*it), te::dt::DOUBLE_TYPE);
                   outdsType->add(prop);
                   ++it;
                 }
        }
          break;
        default:
          continue;
        }
      }
      if (m_texture == true)
      {
        te::dt::SimpleProperty* propContrast = new te::dt::SimpleProperty("B" + boost::lexical_cast<std::string>(m_bands[b]) + "_Contrast", te::dt::DOUBLE_TYPE);
        outdsType->add(propContrast);

        te::dt::SimpleProperty* propDissimilarity = new te::dt::SimpleProperty("B" + boost::lexical_cast<std::string>(m_bands[b]) + "_Dissimilarity", te::dt::DOUBLE_TYPE);
        outdsType->add(propDissimilarity);

        te::dt::SimpleProperty* propEnergy = new te::dt::SimpleProperty("B" + boost::lexical_cast<std::string>(m_bands[b]) + "_Energy", te::dt::DOUBLE_TYPE);
        outdsType->add(propEnergy);

        te::dt::SimpleProperty* propEntropy = new te::dt::SimpleProperty("B" + boost::lexical_cast<std::string>(m_bands[b]) + "_Entropy", te::dt::DOUBLE_TYPE);
        outdsType->add(propEntropy);

        te::dt::SimpleProperty* propHomogeneity = new te::dt::SimpleProperty("B" + boost::lexical_cast<std::string>(m_bands[b]) + "_Homogeneity", te::dt::DOUBLE_TYPE);
        outdsType->add(propHomogeneity);
      }
    }
    else
    {
      te::dt::SimpleProperty* propValue = new te::dt::SimpleProperty("B" + boost::lexical_cast<std::string>(m_bands[b]) + "_Value", te::dt::DOUBLE_TYPE);
      outdsType->add(propValue);
    }
  }

  return outdsType;
}

std::vector<te::rp::Texture> te::attributefill::RasterToVector::getTexture( te::rst::Raster* rst,
                                                                            te::gm::Geometry* geom,
                                                                            int bands)
{
  te::rp::RasterAttributes rattributes;
  std::vector<te::rp::Texture> textureVec;
  
  te::gm::Polygon* polygon;

  if(geom->getGeomTypeId() == te::gm::MultiPolygonType)
  {
    te::gm::MultiPolygon* mPolygon = dynamic_cast< te::gm::MultiPolygon* >(geom);
    polygon = dynamic_cast< te::gm::Polygon* >(mPolygon->getGeometryN(0));
  }
  else
  {
    polygon = dynamic_cast< te::gm::Polygon* >(geom);
  }

  for(int i = 0; i < bands; ++i)
  {
    boost::numeric::ublas::matrix<double> glcm = rattributes.getGLCM(*rst, i, 1, 1, *polygon);
    te::rp::Texture metrics = rattributes.getGLCMMetrics(glcm);
    textureVec.push_back(metrics);
  }

  return textureVec;
}

bool te::attributefill::RasterToVector::save(std::auto_ptr<te::mem::DataSet> result, std::auto_ptr<te::da::DataSetType> outDsType)
{
  std::map<std::string, std::string> options;
 
  // create the dataset
  m_outDsrc->createDataSet(outDsType.get(), options);
  
  // copy from memory to output datasource
  result->moveBeforeFirst();
  m_outDsrc->add(outDsType->getName(), result.get(), options);
  
  // create the primary key if it is possible
  if (m_outDsrc->getCapabilities().getDataSetTypeCapabilities().supportsPrimaryKey())
  {
    std::string pk_name = outDsType->getName() + "_pkey";
    te::da::PrimaryKey* pk = new te::da::PrimaryKey(pk_name, outDsType.get());
    pk->add(outDsType->getProperty(0));
    m_outDsrc->addPrimaryKey(m_outDset,pk);
  }
  
  return true;
}
