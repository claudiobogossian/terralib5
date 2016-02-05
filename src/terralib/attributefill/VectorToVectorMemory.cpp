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
 \file VectorToVectorMemory.h
 
 \brief Vector to Vector processing.
 
 \ingroup attributefill
 */

//Terralib
#include "../BuildConfig.h"
#include "../common/Logger.h"
#include "../common/progress/TaskProgress.h"
#include "../common/STLUtils.h"
#include "../common/StringUtils.h"
#include "../common/Translator.h"
#include "../dataaccess/dataset/DataSetAdapter.h"
#include "../dataaccess/dataset/DataSetTypeConverter.h"
#include "../dataaccess/datasource/DataSourceInfo.h"
#include "../dataaccess/datasource/DataSourceInfoManager.h"
#include "../dataaccess/utils/Utils.h"
#include "../datatype/StringProperty.h"
#include "../datatype/SimpleData.h"
#include "../geometry.h"
#include "../memory/DataSet.h"
#include "../memory/DataSetItem.h"
#include "../statistics/core/NumericStatisticalSummary.h"
#include "../statistics/core/StringStatisticalSummary.h"
#include "../statistics/core/SummaryFunctions.h"
#include "../statistics/core/Enums.h"

#include "Config.h"
#include "VectorToVectorMemory.h"

// STL
#include <memory>

// Boost
#include <boost/algorithm/string/replace.hpp>
#include <boost/lexical_cast.hpp>

void te::attributefill::VectorToVectorMemory::normalizeClassName(std::string& name)
{
  std::string invalidChar;
  bool changed;
  if(!te::da::IsValidName(name, invalidChar))
  {
    name = te::common::ReplaceSpecialChars(name, changed);

    boost::replace_all(name, "/", "_");
    boost::replace_all(name, " ", "_");
    boost::replace_all(name, ".", "");
    boost::replace_all(name, "'", "");
    boost::replace_all(name, "-", "_");
    boost::replace_all(name, "&", "e");
  }
}

te::attributefill::VectorToVectorMemory::VectorToVectorMemory()
{

}

te::attributefill::VectorToVectorMemory::~VectorToVectorMemory()
{
  te::common::FreeContents(m_mapGeom);
  m_mapGeom.clear();
}

bool te::attributefill::VectorToVectorMemory::run()
{
  int fromSrid = m_fromLayer->getSRID();
  int toSrid = m_toLayer->getSRID();

  std::auto_ptr<te::da::DataSetType> fromSchemaOrigin = m_fromLayer->getSchema();

  te::da::DataSetTypeConverter* converter = new te::da::DataSetTypeConverter(fromSchemaOrigin.get(), m_outDsrc->getCapabilities(), m_outDsrc->getEncoding());

  te::da::AssociateDataSetTypeConverterSRID(converter, fromSrid, toSrid);

  std::auto_ptr<te::da::DataSetType> fromSchema(converter->getResult());

  std::auto_ptr<te::da::DataSet> fromDsOrigin = m_fromLayer->getData();

  std::auto_ptr<te::da::DataSetAdapter> fromDs(te::da::CreateAdapter(fromDsOrigin.get(), converter));

  te::gm::Envelope fromEnv = m_fromLayer->getExtent();

  te::gm::Envelope toEnv = m_toLayer->getExtent();
  std::auto_ptr<te::da::DataSet> toDs = m_toLayer->getData();
  std::auto_ptr<te::da::DataSetType> toSchema = m_toLayer->getSchema();

  if (fromSrid != toSrid)
    fromEnv.transform(fromSrid, toSrid);

  if(!fromEnv.intersects(toEnv))
  {
    throw te::common::Exception(TE_TR("The two layers do not intersect."));
  }

  std::auto_ptr<te::da::DataSetType> outDst(getOutputDataSetType());

  std::auto_ptr<te::mem::DataSet> outDs(new te::mem::DataSet(outDst.get()));

  te::sam::rtree::Index<size_t, 8>* rtree = getRtree(fromDs.get());
  KD_ADAPTATIVE_TREE* kdtree = 0;

  toDs->moveBeforeFirst();

  te::common::TaskProgress task("Processing Vector to Vector...");
  task.setTotalSteps((int)toDs->size());
  task.useTimer(true);

  std::string logInfo1 = "";
  std::string logInfo2 = "";

  std::vector<te::dt::Property*> outProps = outDst->getProperties();
  std::vector<te::dt::Property*> toProps = toSchema->getProperties();

  while(toDs->moveNext())
  {
    try
    {
      te::mem::DataSetItem* item = new te::mem::DataSetItem(outDs.get());

      for(std::size_t i = 0; i < outProps.size(); ++i)
      {
        std::string outPropName = outProps[i]->getName();

        for(std::size_t j = 0; j < toProps.size(); ++j)
        {
          if(toProps[j]->getName() == outPropName)
          {
            item->setValue(outPropName, toDs->getValue(outPropName).release());
            break;
          }
        }
      }

      bool hasInvalid = false;
      std::vector<std::size_t> intersections = getIntersections(toDs.get(), fromDs.get(), rtree, hasInvalid);

      if (hasInvalid)
      {
#ifdef TERRALIB_LOGGER_ENABLED
        te::da::PrimaryKey* toPk = toSchema->getPrimaryKey();
        te::dt::Property* pkProp = toPk->getProperties()[0];

        std::string ex = "The \"To\" layer geometry (" + pkProp->getName() + ": " + toDs->getValue(pkProp->getName())->toString() + ") has intersection candidate invalid!";
        te::common::Logger::logDebug("attributefill", ex.c_str());
#endif //TERRALIB_LOGGER_ENABLED

        m_hasErrors = true;
        outDs->add(item);
        continue;
      }


      if(intersections.empty() && !hasNoIntersectionOperations())
      {
        outDs->add(item);
        continue;
      }

      std::map<std::string, std::vector<te::attributefill::OperationType> >::iterator it = m_options.begin();

      std::vector< std::vector<te::dt::AbstractData*> > dataValues;

      dataValues = getDataValues(fromDs.get(), intersections);

      while(it != m_options.end())
      {
        logInfo1 = it->first;

        te::dt::Property* prop = fromSchema->getProperty(it->first);
        std::size_t propPos = fromSchema->getPropertyPosition(it->first);

        te::stat::NumericStatisticalSummary ssNum;
        te::stat::StringStatisticalSummary ssStr;

        std::vector<double> numValues;
        std::vector<std::string> strValues;

        if(prop->getType() == te::dt::STRING_TYPE)
        {
          strValues = getStrValues(dataValues, propPos);
          te::stat::GetStringStatisticalSummary(strValues, ssStr, "");
        }
        else
        {
          numValues = getNumValues(dataValues, propPos);
          te::stat::GetNumericStatisticalSummary(numValues, ssNum);
        }

        std::vector<te::attributefill::OperationType> funcs = it->second;

        for(std::size_t i = 0; i < funcs.size(); ++i)
        {
          std::string outPropName = getPropertyName(prop, funcs[i]);

          if(funcs[i] == te::attributefill::VALUE)
          {
            if(prop->getType() == te::dt::STRING_TYPE)
              item->setString(outPropName, ssStr.m_minVal);
            else
              item->setDouble(outPropName, ssNum.m_minVal);
          }
          else if(funcs[i] == te::attributefill::HIGHEST_INTERSECTION)
          {
            te::dt::AbstractData* value = getClassWithHighestIntersectionArea(toDs.get(), toSrid, fromDs.get(), fromSrid, intersections, prop->getName(), dataValues);

            item->setValue(outPropName, value);
          }
          else if(funcs[i] == te::attributefill::HIGHEST_OCCURRENCE)
          {
            te::dt::AbstractData* value = getClassWithHighestOccurrence(fromDs.get(), intersections, prop->getName(), dataValues);
            if (value)
              item->setValue(outPropName, value);
          }
          else if(funcs[i] == te::attributefill::PERCENT_CLASS)
          {
            std::map<std::string, double> result = getPercentagePerClass(fromDs.get(), intersections, prop->getName(), dataValues);

            std::map<std::string, double>::iterator itAux = result.begin();
            while(itAux != result.end())
            {
              std::string className = itAux->first;
              normalizeClassName(className);
              std::string newPropName = prop->getName() + "_" + className;

              item->setDouble(newPropName, itAux->second);

              ++itAux;
            }
          }
          else if(funcs[i] == te::attributefill::PRESENCE)
          {
            if(intersections.size() > 0)
            {
              item->setInt32(outPropName, 1);
            }
            else
            {
              item->setInt32(outPropName, 0);
            }
          }
          else if(funcs[i] == te::attributefill::PERCENT_TOTAL_AREA)
          {
            double area = getPercentageOfTotalArea(toDs.get(), toSrid, fromDs.get(), fromSrid, intersections, prop->getName(), dataValues);

            item->setDouble(outPropName, area);
          }
          else if(funcs[i] == te::attributefill::PERCENT_EACH_CLASS)
          {
            std::map<std::string, double> result = getPercentageOfEachClassByArea(toDs.get(), toSrid, fromDs.get(), fromSrid, intersections, prop->getName(), dataValues);

            std::map<std::string, double>::iterator itAux = result.begin();
            while(itAux != result.end())
            {
              std::string className = itAux->first;
              normalizeClassName(className);
              std::string newPropName = prop->getName() + "_" + className;

              item->setDouble(newPropName, itAux->second);

              ++itAux;
            }
          }
          else if(funcs[i] == te::attributefill::WEIGHTED)
          {
            double weigh = getWeightedByArea(toDs.get(), toSrid, fromDs.get(), fromSrid, intersections, prop->getName(), dataValues);

            item->setDouble(outPropName, weigh);
          }
          else if(funcs[i] == te::attributefill::WEIGHTED_SUM)
          {
            double weigh = getWeightedSumByArea(toDs.get(), toSrid, fromDs.get(), fromSrid, intersections, prop->getName(), dataValues);

            item->setDouble(outPropName, weigh);
          }
          else if(funcs[i] == te::attributefill::MIN_DISTANCE)
          {
            double result = 0;

            if(intersections.empty())
            {
              if(!kdtree)
                kdtree = getKDtree(fromDs.get(), toSrid);

              result = getMinimumDistance(toDs.get(), toSrid, fromDs.get(), fromSrid, kdtree);
            }

            item->setDouble(outPropName, result);
          }
          else if(isStatistical(funcs[i]))
          {
             if(prop->getType() == te::dt::STRING_TYPE)
             {
               std::string value = getValue(ssStr, funcs[i]);
               item->setString(outPropName, value);
             }
             else
             {
               if(funcs[i] == te::attributefill::MODE)
               {
                 std::string value = getModeValue(ssNum);
                 item->setString(outPropName, value);
               }
               else
               {
                 double value = getValue(ssNum, funcs[i]);
                 item->setDouble(outPropName, value);
               }
             }
          }
        }

        ++it;
      }

      for (std::size_t a = 0; a < dataValues.size(); ++a)
        te::common::FreeContents(dataValues[a]);

      dataValues.clear();

      outDs->add(item);

      if (task.isActive() == false)
        throw te::common::Exception(TE_TR("Operation canceled!"), 1);

      task.pulse();
    }
    catch(te::common::Exception& e)
    {
      if(e.code() == 1)
      {
        throw e;
      }

#ifdef TERRALIB_LOGGER_ENABLED
      std::string ex = e.what();
      ex += " | Ref: " + logInfo1 + " : " + logInfo2;
      te::common::Logger::logDebug("attributefill", ex.c_str());
#endif //TERRALIB_LOGGER_ENABLED

      m_hasErrors = true;
    }
    catch(std::exception& e)
    {
#ifdef TERRALIB_LOGGER_ENABLED
      std::string ex = e.what();
      ex += " | Ref: " + logInfo1 + " : " + logInfo2;
      te::common::Logger::logDebug("attributefill", ex.c_str());
#endif //TERRALIB_LOGGER_ENABLED
      m_hasErrors = true;
    }
  }

  if (!outDs->isEmpty())
    save(outDs, outDst);

  if(rtree)
    delete rtree;

  if(kdtree)
    delete kdtree;

  return true;
}

bool te::attributefill::VectorToVectorMemory::isToLayerOGR()
{
  std::string id = m_toLayer->getDataSourceId();

  std::string type = te::da::DataSourceInfoManager::getInstance().get(id)->getType();

  if (type == "OGR")
    return true;
  else
    return false;
}

te::da::DataSetType* te::attributefill::VectorToVectorMemory::getOutputDataSetType()
{
  std::auto_ptr<te::da::DataSet> fromDs = m_fromLayer->getData();
  std::auto_ptr<te::da::DataSetType> fromSchema = m_fromLayer->getSchema();

  std::auto_ptr<te::da::DataSetType> toScheme = m_toLayer->getSchema();

  te::da::DataSetType* dst = new te::da::DataSetType(*toScheme.get());
  dst->setName(m_outDset);
  dst->setTitle(m_outDset);

  if (isToLayerOGR())
    dst->remove(dst->getProperty("FID"));

  std::vector<te::dt::Property*> outProps = dst->getProperties();

  te::da::PrimaryKey* pk = dst->getPrimaryKey();

  std::vector<te::dt::Property*> pkProps;

  if (pk)
    pkProps = dst->getPrimaryKey()->getProperties();

  // Keep only the properties that the user selected
  for(std::size_t i = 0; i < outProps.size(); ++i)
  {
    bool isPk = false;
    for(std::size_t j = 0; j < pkProps.size(); ++j)
    {
      if(outProps[i]->getName() == pkProps[j]->getName())
      {
        isPk = true;
        break;
      }
    }

    if(outProps[i]->getType() != te::dt::GEOMETRY_TYPE && !isPk)
    {
      std::string name = outProps[i]->getName();
      if(std::find(m_toLayerProps.begin(), m_toLayerProps.end(), name) == m_toLayerProps.end())
        dst->remove(outProps[i]);
    }
  }

  std::map<std::string, std::vector<te::attributefill::OperationType> >::iterator it = m_options.begin();

  while(it != m_options.end())
  {
    te::dt::Property* currentProperty = fromSchema->getProperty(it->first);

    std::vector<te::attributefill::OperationType> funcs = it->second;

    for(std::size_t i = 0; i < funcs.size(); ++i)
    {
      te::dt::SimpleProperty* newProp = 0;

      std::string newName = getPropertyName(currentProperty, (te::attributefill::OperationType)funcs[i]);

      if(funcs[i] == te::attributefill::VALUE)
      {
        if(currentProperty->getType() == te::dt::STRING_TYPE)
          newProp = new te::dt::StringProperty(newName);
        else
          newProp = new te::dt::SimpleProperty(newName, te::dt::DOUBLE_TYPE);
      }
      else if(funcs[i] == te::attributefill::HIGHEST_OCCURRENCE ||
              funcs[i] == te::attributefill::HIGHEST_INTERSECTION)
      {
        newProp = dynamic_cast<te::dt::SimpleProperty*>(currentProperty->clone());
        newProp->setRequired(false);
        newProp->setAutoNumber(false);
        newProp->setDefaultValue(0);
        newProp->setName(newName);
      }
      else if(funcs[i] == te::attributefill::PERCENT_CLASS || funcs[i] == te::attributefill::PERCENT_EACH_CLASS)
      {
        continue;//Sera feito fora do for
      }
      else if(funcs[i] == te::attributefill::PRESENCE)
      {
        newProp = new te::dt::SimpleProperty(newName, te::dt::INT32_TYPE);
      }
      else if(funcs[i] == te::attributefill::PERCENT_TOTAL_AREA ||
              funcs[i] == te::attributefill::MIN_DISTANCE)
      {
        newProp = new te::dt::SimpleProperty(newName, te::dt::DOUBLE_TYPE);
      }
      else if(currentProperty->getType() == te::dt::STRING_TYPE || funcs[i] == te::attributefill::MODE)
      {
        newProp = new te::dt::StringProperty(newName);
      }
      else
      {
        newProp = new te::dt::SimpleProperty(newName, te::dt::DOUBLE_TYPE);
      }

      dst->add(newProp);
    }

    if(std::find(funcs.begin(), funcs.end(), te::attributefill::PERCENT_CLASS) != funcs.end() || 
      std::find(funcs.begin(), funcs.end(), te::attributefill::PERCENT_EACH_CLASS) != funcs.end())
    {
      std::vector<std::string> strClasses = getDistinctClasses(fromDs.get(), currentProperty->getName());

      for(std::size_t i = 0; i < strClasses.size(); ++i)
      {
        std::string className = strClasses[i];

        normalizeClassName(className);

        std::string newPropName = currentProperty->getName() + "_" + className;

        te::dt::SimpleProperty* newProp = new te::dt::SimpleProperty(newPropName, te::dt::DOUBLE_TYPE);

        dst->add(newProp);
      }
    }

    ++it;
  }

  return dst;
}

std::vector<std::string> te::attributefill::VectorToVectorMemory::getDistinctClasses(te::da::DataSet* fromDs,
                                                                                     const std::string& propertyName)
{
  std::vector<std::string> result;

  fromDs->moveBeforeFirst();

  while(fromDs->moveNext())
  {
    std::string strClass = fromDs->getAsString(propertyName, 9);

    if(std::find(result.begin(), result.end(), strClass) == result.end())
    {
      result.push_back(strClass);
    }
  }

  return result;
}

te::sam::rtree::Index<size_t, 8>* te::attributefill::VectorToVectorMemory::getRtree(te::da::DataSet* data)
{
  te::sam::rtree::Index<size_t, 8>* rtree = new te::sam::rtree::Index<size_t, 8>;

  std::size_t geomPos = te::da::GetFirstSpatialPropertyPos(data);

  data->moveBeforeFirst();

  int count = 0;

  te::common::FreeContents(m_mapGeom);
  m_mapGeom.clear();

  while(data->moveNext())
  {
    std::auto_ptr<te::gm::Geometry> geom = data->getGeometry(geomPos);

    rtree->insert(*geom->getMBR(), count);

    m_mapGeom.insert(std::map<int, te::gm::Geometry*>::value_type(count, geom.release()));

    ++count;
  }

  return rtree;
}

std::string te::attributefill::VectorToVectorMemory::getPropertyName(te::dt::Property* prop, te::attributefill::OperationType func)
{
  std::string name = te::common::Convert2LCase(prop->getName());

  std::string newName = name + "_";

  if(func == te::attributefill::VALUE)
    newName += "value";
  else if(func == te::attributefill::MIN_VALUE)
    newName += "min_val";
  else if(func == te::attributefill::MAX_VALUE)
    newName += "max_val";
  else if(func == te::attributefill::MEAN)
    newName += "mean";
  else if(func == te::attributefill::SUM)
    newName += "sum_values";
  else if(func == te::attributefill::COUNT)
    newName += "total_values";
  else if(func == te::attributefill::VALID_COUNT)
    newName += "total_notnull_values";
  else if(func == te::attributefill::STANDARD_DEVIATION)
    newName += "stand_dev";
  else if(func == te::attributefill::VARIANCE)
    newName += "variance";
  else if(func == te::attributefill::SKEWNESS)
    newName += "skewness";
  else if(func == te::attributefill::KURTOSIS)
    newName += "kurtosis";
  else if(func == te::attributefill::AMPLITUDE)
    newName += "amplitude";
  else if(func == te::attributefill::MEDIAN)
    newName += "median";
  else if(func == te::attributefill::VAR_COEFF)
    newName += "coeff_variation";
  else if(func == te::attributefill::MODE)
    newName += "mode";
  else if(func == te::attributefill::HIGHEST_OCCURRENCE)
    newName += "class_high_occurrence";
  else if(func == te::attributefill::HIGHEST_INTERSECTION)
    newName += "class_high_area";
  else if(func == te::attributefill::MIN_DISTANCE)
    newName += "min_distance";
  else if(func == te::attributefill::PRESENCE)
    newName += "presence";
  else if(func == te::attributefill::PERCENT_TOTAL_AREA)
    newName += "percent_of_total_area";
  else if(func == te::attributefill::PERCENT_EACH_CLASS)
    newName += "percent_area_class";
  else if(func == te::attributefill::WEIGHTED)
    newName += "weigh_area";
  else if(func == te::attributefill::WEIGHTED_SUM)
    newName += "weigh_sum_area";
  else if(func == te::attributefill::PERCENT_CLASS)
    newName += "percent_class";

  return newName;
}

std::vector<std::size_t> te::attributefill::VectorToVectorMemory::getIntersections(te::da::DataSet* toDs,
                                                                             te::da::DataSet*,
                                                                             te::sam::rtree::Index<size_t, 8>* rtree,
                                                                             bool& hasInvalid)
{
  std::size_t toSpatialPos = te::da::GetFirstSpatialPropertyPos(toDs);

  std::auto_ptr<te::gm::Geometry> geom = toDs->getGeometry(toSpatialPos);

  std::vector<size_t> report;
  rtree->search(*geom->getMBR(), report);

  std::vector<std::size_t> interVec;
  for(std::size_t i = 0; i < report.size(); ++i)
  {
    //fromDs->move(report[i]);

    te::gm::Geometry* g = m_mapGeom[report[i]];//fromDs->getGeometry(fromSpatialPos);
    
    if (!g->isValid())
      hasInvalid = true;

    if(geom->intersects(g))
    {
      interVec.push_back(report[i]);
    }
  }
  return interVec;
}

std::vector<double> te::attributefill::VectorToVectorMemory::getNumValues(std::vector< std::vector<te::dt::AbstractData*> > dataValues, std::size_t pos)
{
  std::vector<double> result;

  for (std::size_t t = 0; t < dataValues.size(); ++t)
  {
    std::vector<te::dt::AbstractData*> data = dataValues[t];

    if (!data[pos])
    {
      result.push_back(0.0f);
      continue;
    }

    if (data[pos]->getTypeCode() == te::dt::INT16_TYPE ||
      data[pos]->getTypeCode() == te::dt::UINT16_TYPE ||
      data[pos]->getTypeCode() == te::dt::INT32_TYPE ||
      data[pos]->getTypeCode() == te::dt::UINT32_TYPE ||
      data[pos]->getTypeCode() == te::dt::INT64_TYPE ||
      data[pos]->getTypeCode() == te::dt::UINT64_TYPE ||
      data[pos]->getTypeCode() == te::dt::FLOAT_TYPE ||
      data[pos]->getTypeCode() == te::dt::DOUBLE_TYPE ||
      data[pos]->getTypeCode() == te::dt::CINT16_TYPE ||
      data[pos]->getTypeCode() == te::dt::CINT32_TYPE ||
      data[pos]->getTypeCode() == te::dt::CFLOAT_TYPE ||
      data[pos]->getTypeCode() == te::dt::CDOUBLE_TYPE)
    {
      std::string strValue = data[pos]->toString();

      result.push_back(boost::lexical_cast<double>(strValue));
    }
  }

  return result;
}

std::vector<std::string> te::attributefill::VectorToVectorMemory::getStrValues(std::vector< std::vector<te::dt::AbstractData*> > dataValues, std::size_t pos)
{
  std::vector<std::string> result;

  for (std::size_t t = 0; t < dataValues.size(); ++t)
  {
    std::vector<te::dt::AbstractData*> data = dataValues[t];

    if (!data[pos])
    {
      result.push_back("");
      continue;
    }

    if (data[pos]->getTypeCode() == te::dt::STRING_TYPE)
      result.push_back(data[pos]->toString());

  }

  return result;
}

bool te::attributefill::VectorToVectorMemory::isStatistical(te::attributefill::OperationType type)
{
  if(type == te::attributefill::MIN_VALUE ||
     type == te::attributefill::MAX_VALUE ||
     type == te::attributefill::MEAN ||
     type == te::attributefill::SUM ||
     type == te::attributefill::COUNT ||
     type == te::attributefill::VALID_COUNT ||
     type == te::attributefill::STANDARD_DEVIATION ||
     type == te::attributefill::VARIANCE ||
     type == te::attributefill::SKEWNESS ||
     type == te::attributefill::KURTOSIS ||
     type == te::attributefill::AMPLITUDE ||
     type == te::attributefill::MEDIAN ||
     type == te::attributefill::VAR_COEFF ||
     type == te::attributefill::MODE)
  {
    return true;
  }

  return false;
}

double te::attributefill::VectorToVectorMemory::getValue(te::stat::NumericStatisticalSummary ss, te::attributefill::OperationType type)
{
  if(type == te::attributefill::AMPLITUDE)
    return ss.m_amplitude;
  else if(type == te::attributefill::COUNT)
    return ss.m_count;
  else if(type == te::attributefill::KURTOSIS)
    return ss.m_kurtosis;
  else if(type == te::attributefill::MAX_VALUE)
    return ss.m_maxVal;
  else if(type == te::attributefill::MEAN)
    return ss.m_mean;
  else if(type == te::attributefill::MEDIAN)
    return ss.m_median;
  else if(type == te::attributefill::MIN_VALUE)
    return ss.m_minVal;  
  else if(type == te::attributefill::SKEWNESS)
    return ss.m_skewness;
  else if(type == te::attributefill::STANDARD_DEVIATION)
    return ss.m_stdDeviation;
  else if(type == te::attributefill::SUM)
    return ss.m_sum;
  else if(type == te::attributefill::VALID_COUNT)
    return ss.m_validCount;
  else if(type == te::attributefill::VAR_COEFF)
    return ss.m_varCoeff;
  else if(type == te::attributefill::VARIANCE)
    return ss.m_variance;
  else
    return -1;
}

std::string te::attributefill::VectorToVectorMemory::getValue(te::stat::StringStatisticalSummary ss, te::attributefill::OperationType type)
{
  if(type == te::attributefill::MAX_VALUE)
    return ss.m_maxVal;
  else if(type == te::attributefill::MIN_VALUE)
    return ss.m_minVal; 
  else if(type == te::attributefill::MODE)
    return ss.m_mode;
  else if(type == te::attributefill::COUNT)
    return boost::lexical_cast<std::string>(ss.m_count);
  else if(type == te::attributefill::VALID_COUNT)
    return boost::lexical_cast<std::string>(ss.m_validCount);
  else
    return "null";
}

std::string te::attributefill::VectorToVectorMemory::getModeValue(te::stat::NumericStatisticalSummary ss)
{
  std::string result = "";
  std::vector<double> values = ss.m_mode;
  for(std::size_t i = 0; i < values.size(); ++i)
  {
    if(i == 0)
      result += boost::lexical_cast<std::string>(values[i]);
    else
      result += ", " + boost::lexical_cast<std::string>(values[i]);
  }

  return result;
}

std::vector<std::vector<te::dt::AbstractData*> > te::attributefill::VectorToVectorMemory::getDataValues(te::da::DataSet* fromDs,
                                                                                                        std::vector<std::size_t> dsPos)
{
  std::vector<std::vector<te::dt::AbstractData*> > result;

  for(std::size_t i = 0; i < dsPos.size(); ++i)
  {
    std::vector<te::dt::AbstractData*> resultItem;

    fromDs->move(dsPos[i]);

    for (std::size_t t = 0; t < fromDs->getNumProperties(); ++t)
    {
      if (fromDs->isNull(t))
        resultItem.push_back(0);
      else
        resultItem.push_back(fromDs->getValue(t).release());
    }
    result.push_back(resultItem);
  }

  return result;
}

te::dt::AbstractData* te::attributefill::VectorToVectorMemory::getClassWithHighestOccurrence(te::da::DataSet* fromDs,
                                                                                             std::vector<std::size_t> dsPos,
                                                                                             const std::string& propertyName,
                                                                                             std::vector< std::vector<te::dt::AbstractData*> >& dataValues)
{
  int propIndex = te::da::GetPropertyIndex(fromDs, propertyName);
  int propType = fromDs->getPropertyDataType(propIndex);

  std::size_t highOccur = 0;
  std::vector<std::string> highValues;

  std::map<std::string, std::size_t> counter;
  for (std::size_t i = 0; i < dsPos.size(); ++i)
  {
    //dataValues[i][propIndex];

    if (!dataValues[i][propIndex])
      continue;

    std::string value = dataValues[i][propIndex]->toString();

    if (counter.find(value) == counter.end())
    {
      counter[value] = 1;
      highOccur = 1;
    }
    else
    {
      std::size_t aux = counter[value] + 1;
      counter[value] = aux;

      if (aux > highOccur)
        highOccur = aux;
    }
  }

  std::map<std::string, std::size_t>::iterator it = counter.begin();
  while (it != counter.end())
  {
    if (it->second == highOccur)
      highValues.push_back(it->first);
    ++it;
  }

  if (highValues.size() > 1)
  {
    std::vector<double> intVec;

    if (propType == te::dt::STRING_TYPE)
    {
      te::stat::StringStatisticalSummary ssStr;
      te::stat::GetStringStatisticalSummary(highValues, ssStr);

      return getDataBasedOnType(ssStr.m_minVal, propType);
    }
    else
    {
      for (std::size_t i = 0; i < highValues.size(); ++i)
      {
        intVec.push_back(boost::lexical_cast<double>(highValues[i]));
      }

      te::stat::NumericStatisticalSummary ssNum;
      te::stat::GetNumericStatisticalSummary(intVec, ssNum);
      std::string strVal = boost::lexical_cast<std::string>(ssNum.m_minVal);
      return getDataBasedOnType(strVal, propType);
    }
  }
  else if (!highValues.empty())
    return getDataBasedOnType(highValues[0], propType);
  else
    return 0;
}

te::dt::AbstractData* te::attributefill::VectorToVectorMemory::getClassWithHighestIntersectionArea(te::da::DataSet* toDs,
                                                                                                   std::size_t toSrid,
                                                                                                   te::da::DataSet* fromDs,
                                                                                                   std::size_t,
                                                                                                   std::vector<std::size_t> dsPos,
                                                                                                   const std::string& propertyName,
                                                                                                   std::vector< std::vector<te::dt::AbstractData*> >& dataValues)
{
  std::size_t toGeomPos =   te::da::GetFirstSpatialPropertyPos(toDs);

  int propIndex = te::da::GetPropertyIndex(fromDs, propertyName);
  int propType = fromDs->getPropertyDataType(propIndex);

  std::auto_ptr<te::gm::Geometry> toGeom = toDs->getGeometry(toGeomPos);
  if(toGeom->getSRID() <= 0)
      toGeom->setSRID((int)toSrid);

  std::map<std::string, double> classAreaMap;
  for(std::size_t i = 0; i < dsPos.size(); ++i)
  {
    //dataValues[i][propIndex];

    if (!dataValues[i][propIndex])
      continue;

    te::gm::Geometry* fromGeom = m_mapGeom[dsPos[i]]; //fromDs->getGeometry(fromGeomPos);

    std::auto_ptr<te::gm::Geometry> interGeom;

    if(!checkGeometries(fromGeom, dsPos[i], toGeom.get()))
    {
      m_hasErrors = true;
      continue;
    }
    
    try
    {
      interGeom.reset(toGeom->intersection(fromGeom));
    }
    catch(const std::exception &e)
    {
#ifdef TERRALIB_LOGGER_ENABLED
      std::string ex = e.what();
      te::common::Logger::logDebug("attributefill", ex.c_str());
#endif //TERRALIB_LOGGER_ENABLED
      m_hasErrors = true;
      continue;
    }

    std::string value = dataValues[i][propIndex]->toString();// fromDs->getAsString(propertyName);

    double area = getArea(interGeom.get());

    if(classAreaMap.find(value) == classAreaMap.end())
    {
      classAreaMap[value] = area;
    }
    else
    {
      double aux = classAreaMap[value];
      classAreaMap[value] = aux + area;
    }
  }

  std::map<std::string, double>::iterator it = classAreaMap.begin();
  std::string value;
  double aux = 0;
  while(it != classAreaMap.end())
  {
    if(aux < it->second)
    {
      aux = it->second;
      value = it->first;
    }

    ++it;
  }

  te::dt::AbstractData* data = getDataBasedOnType(value, propType);

  return data;
}

std::map<std::string, double> te::attributefill::VectorToVectorMemory::getPercentagePerClass(te::da::DataSet* fromDs,
                                                                                             std::vector<std::size_t> dsPos,
                                                                                             const std::string& propertyName,
                                                                                             std::vector< std::vector<te::dt::AbstractData*> >& dataValues)
{
  int propIndex = te::da::GetPropertyIndex(fromDs, propertyName);

  std::map<std::string, double> result;

  std::map<std::string, std::size_t> aux;
  for(std::size_t i = 0; i < dsPos.size(); ++i)
  {
    //dataValues[i][propIndex];//fromDs->move(dsPos[i]);

    std::string value = dataValues[i][propIndex]->toString();//fromDs->getAsString(propertyName);

    if(aux.find(value) == aux.end())
    {
      aux[value] = 1;
    }
    else
    {
      aux[value] += 1;
    }
  }

  std::size_t total = 0;
  std::map<std::string, std::size_t>::iterator it = aux.begin();
  while(it != aux.end())
  {
    total += it->second;

    ++it;
  }

  it = aux.begin();
  while(it != aux.end())
  {
    std::string normName = it->first;
    normalizeClassName(normName);
    result[normName] = ((double)it->second / total);

    ++it;
  }

  return result;
}

double te::attributefill::VectorToVectorMemory::getPercentageOfTotalArea(te::da::DataSet* toDs,
                                                                         std::size_t toSrid,
                                                                         te::da::DataSet*,
                                                                         std::size_t,
                                                                         std::vector<std::size_t> dsPos,
                                                                         const std::string&,
                                                                         std::vector< std::vector<te::dt::AbstractData*> >&)
{
  std::size_t toGeomPos =   te::da::GetFirstSpatialPropertyPos(toDs);

  std::auto_ptr<te::gm::Geometry> toGeom = toDs->getGeometry(toGeomPos);
  if(toGeom->getSRID() <= 0)
    toGeom->setSRID((int)toSrid);

  double classArea = 0;
  for(std::size_t i = 0; i < dsPos.size(); ++i)
  {
    //dataValues[i][propIndex];//fromDs->move(dsPos[i]);

    te::gm::Geometry* fromGeom = m_mapGeom[dsPos[i]];//fromDs->getGeometry(fromGeomPos);

    if(!checkGeometries(fromGeom, dsPos[i], toGeom.get()))
    {
      m_hasErrors = true;
      continue;
    }

    std::auto_ptr<te::gm::Geometry> interGeom(toGeom->intersection(fromGeom));

    classArea += getArea(interGeom.get());
  }

  double polArea = getArea(toGeom.get());

  return classArea/polArea;
}

std::map<std::string, double> te::attributefill::VectorToVectorMemory::getPercentageOfEachClassByArea(te::da::DataSet* toDs,
                                                                                                      std::size_t toSrid,
                                                                                                      te::da::DataSet* fromDs,
                                                                                                      std::size_t,
                                                                                                      std::vector<std::size_t> dsPos,
                                                                                                      const std::string& propertyName,
                                                                                                      std::vector< std::vector<te::dt::AbstractData*> >& dataValues)
{
  std::map<std::string, double> result;

  std::size_t toGeomPos =   te::da::GetFirstSpatialPropertyPos(toDs);

  int propIndex = te::da::GetPropertyIndex(fromDs, propertyName);

  std::auto_ptr<te::gm::Geometry> toGeom = toDs->getGeometry(toGeomPos);
  if(toGeom->getSRID() <= 0)
    toGeom->setSRID((int)toSrid);

  double toGeomArea = getArea(toGeom.get());

  for(std::size_t i = 0; i < dsPos.size(); ++i)
  {
    //dataValues[i][propIndex];//fromDs->move(dsPos[i]);

    te::gm::Geometry* fromGeom = m_mapGeom[dsPos[i]];//fromDs->getGeometry(fromGeomPos);

    if(!checkGeometries(fromGeom, dsPos[i], toGeom.get()))
    {
      m_hasErrors = true;
      continue;
    }

    std::auto_ptr<te::gm::Geometry> interGeom(toGeom->intersection(fromGeom));

    std::string value = dataValues[i][propIndex]->toString();//fromDs->getAsString(propertyName);

    double area = getArea(interGeom.get());

    if(result.find(value) == result.end())
    {
      result[value] = area/toGeomArea;
    }
    else
    {
      result[value] += area/toGeomArea;
    }
  }

  return result;
}

double te::attributefill::VectorToVectorMemory::getWeightedByArea(te::da::DataSet* toDs,
                                                                  std::size_t toSrid,
                                                                  te::da::DataSet* fromDs,
                                                                  std::size_t,
                                                                  std::vector<std::size_t> dsPos,
                                                                  const std::string& propertyName,
                                                                  std::vector< std::vector<te::dt::AbstractData*> >& dataValues)
{
  std::size_t toGeomPos =   te::da::GetFirstSpatialPropertyPos(toDs);

  int propIndex = te::da::GetPropertyIndex(fromDs, propertyName);

  std::auto_ptr<te::gm::Geometry> toGeom = toDs->getGeometry(toGeomPos);
  if(toGeom->getSRID() <= 0)
    toGeom->setSRID((int)toSrid);

  double toGeomArea = getArea(toGeom.get());

  double weigh = 0;

  for(std::size_t i = 0; i < dsPos.size(); ++i)
  {
    //fromDs->move(dsPos[i]);

    te::gm::Geometry* fromGeom = m_mapGeom[dsPos[i]];//fromDs->getGeometry(fromGeomPos);

    if(!checkGeometries(fromGeom, dsPos[i], toGeom.get()))
    {
      m_hasErrors = true;
      continue;
    }

    std::auto_ptr<te::gm::Geometry> interGeom(toGeom->intersection(fromGeom));

    double value_num = 0;

    if(!fromDs->isNull(propertyName))
    {
      std::string value = dataValues[i][propIndex]->toString();//fromDs->getAsString(propertyName);
      value_num = boost::lexical_cast<double>(value);
    }

    double intersectionArea = getArea(interGeom.get());

    weigh += value_num*(intersectionArea/toGeomArea);
  }

  return weigh;
}

double te::attributefill::VectorToVectorMemory::getWeightedSumByArea(te::da::DataSet* toDs,
                                                                     std::size_t toSrid,
                                                                     te::da::DataSet* fromDs,
                                                                     std::size_t,
                                                                     std::vector<std::size_t> dsPos,
                                                                     const std::string& propertyName,
                                                                     std::vector< std::vector<te::dt::AbstractData*> >& dataValues)
{
  std::size_t toGeomPos =   te::da::GetFirstSpatialPropertyPos(toDs);

  int propIndex = te::da::GetPropertyIndex(fromDs, propertyName);

  std::auto_ptr<te::gm::Geometry> toGeom = toDs->getGeometry(toGeomPos);
  if(toGeom->getSRID() <= 0)
    toGeom->setSRID((int)toSrid);

  double weigh = 0;

  for(std::size_t i = 0; i < dsPos.size(); ++i)
  {
    //fromDs->move(dsPos[i]);

    te::gm::Geometry* fromGeom = m_mapGeom[dsPos[i]];//fromDs->getGeometry(fromGeomPos);

    double fromGeomArea = getArea(fromGeom);

    if(!checkGeometries(fromGeom, dsPos[i], toGeom.get()))
    {
      m_hasErrors = true;
      continue;
    }

    std::auto_ptr<te::gm::Geometry> interGeom(toGeom->intersection(fromGeom));

    double value_num = 0;

    if (!dataValues[i][propIndex])
    {
      std::string value = dataValues[i][propIndex]->toString();//fromDs->getAsString(propertyName);
      value_num = boost::lexical_cast<double>(value);
    }

    double intersectionArea = getArea(interGeom.get());

    weigh += value_num*(intersectionArea/fromGeomArea);
  }

  return weigh;
}

bool te::attributefill::VectorToVectorMemory::isPolygon(te::gm::GeomType type)
{
  if(type == te::gm::PolygonType ||
     type == te::gm::PolygonZType ||
     type == te::gm::PolygonMType ||
     type == te::gm::PolygonZMType)
     return true;

  return false;
}

bool te::attributefill::VectorToVectorMemory::isLine(te::gm::GeomType type)
{
  if(type == te::gm::LineStringType ||
     type == te::gm::LineStringMType ||
     type == te::gm::LineStringZMType ||
     type == te::gm::LineStringZType)
     return true;

  return false;
}

bool te::attributefill::VectorToVectorMemory::isPoint(te::gm::GeomType type)
{
  if(type == te::gm::PointType ||
     type == te::gm::PointZType ||
     type == te::gm::PointMType ||
     type == te::gm::PointZMType)
     return true;

  return false;
}

bool te::attributefill::VectorToVectorMemory::isMultiPolygon(te::gm::GeomType type)
{
  if(type == te::gm::MultiPolygonType ||
     type == te::gm::MultiPolygonZType ||
     type == te::gm::MultiPolygonMType ||
     type == te::gm::MultiPolygonZMType)
     return true;

  return false;
}

bool te::attributefill::VectorToVectorMemory::isMultiLine(te::gm::GeomType type)
{
  if(type == te::gm::MultiLineStringMType ||
     type == te::gm::MultiLineStringType ||
     type == te::gm::MultiLineStringZMType ||
     type == te::gm::MultiLineStringZType)
     return true;

  return false;
}

bool te::attributefill::VectorToVectorMemory::isMultiPoint(te::gm::GeomType type)
{
  if(type == te::gm::MultiPointType ||
     type == te::gm::MultiPointZType ||
     type == te::gm::MultiPointMType ||
     type == te::gm::MultiPointZMType)
     return true;

  return false;
}

double te::attributefill::VectorToVectorMemory::getArea(te::gm::Geometry* geom)
{
  te::gm::GeomType geomType = geom->getGeomTypeId();

  double area = 0;

  switch(geomType)
  {
    case te::gm::PolygonType:
    case te::gm::PolygonZType:
    case te::gm::PolygonMType:
    case te::gm::PolygonZMType:
    {
      te::gm::Polygon* g = dynamic_cast<te::gm::Polygon*>(geom);
      area = g->getArea();
      break;
    }

    case te::gm::MultiPolygonType:
    case te::gm::MultiPolygonZType:
    case te::gm::MultiPolygonMType:
    case te::gm::MultiPolygonZMType:
    {
      te::gm::MultiPolygon* g = dynamic_cast<te::gm::MultiPolygon*>(geom);
      area = g->getArea();
      break;
    }

    case te::gm::MultiSurfaceType:
    case te::gm::MultiSurfaceZType:
    case te::gm::MultiSurfaceMType:
    case te::gm::MultiSurfaceZMType:
    {
      te::gm::MultiSurface* col = dynamic_cast<te::gm::MultiSurface*>(geom);
      for (std::size_t j = 0; j < col->getNumGeometries(); ++j)
      {
        te::gm::Geometry* auxGeom = col->getGeometryN(j);
        if (isPolygon(auxGeom->getGeomTypeId()))
        {
          area += dynamic_cast<te::gm::Polygon*>(auxGeom)->getArea();
        }
        else if (isMultiPolygon(auxGeom->getGeomTypeId()))
        {
          area += dynamic_cast<te::gm::MultiPolygon*>(auxGeom)->getArea();
        }
      }

      break;
    }
    case te::gm::GeometryCollectionType:
    case te::gm::GeometryCollectionZType:
    case te::gm::GeometryCollectionMType:
    case te::gm::GeometryCollectionZMType:
    {
      te::gm::GeometryCollection* col = dynamic_cast<te::gm::GeometryCollection*>(geom);
      for(std::size_t j = 0; j < col->getNumGeometries(); ++j)
      {
        te::gm::Geometry* auxGeom = col->getGeometryN(j);
        if(isPolygon(auxGeom->getGeomTypeId()))
        {
          area += dynamic_cast<te::gm::Polygon*>(auxGeom)->getArea();
        }
        else if(isMultiPolygon(auxGeom->getGeomTypeId()))
        {
          area += dynamic_cast<te::gm::MultiPolygon*>(auxGeom)->getArea();
        }
      }

      break;
    }
    default:
    {
      throw te::common::Exception(TE_TR("Unexpected geometry type!"));
    }
  }

  return area;
}

te::dt::AbstractData* te::attributefill::VectorToVectorMemory::getDataBasedOnType(const std::string& strValue, const int type)
{
  te::dt::AbstractData* data = 0;

  switch(type)
  {
    case te::dt::STRING_TYPE:
    {
      data = new te::dt::SimpleData<std::string, te::dt::STRING_TYPE>(strValue);
      break;
    }
    case te::dt::INT16_TYPE:
    {
      int16_t v = boost::lexical_cast<int16_t>(strValue);
      data = new te::dt::SimpleData<int16_t, te::dt::INT16_TYPE>(v);
      break;
    }
    case te::dt::INT32_TYPE:
    {
      int32_t v = boost::lexical_cast<int32_t>(strValue);
      data = new te::dt::SimpleData<int32_t, te::dt::INT32_TYPE>(v);
      break;
    }
    case te::dt::INT64_TYPE:
    {
      int64_t v = boost::lexical_cast<int64_t>(strValue);
      data = new te::dt::SimpleData<int64_t, te::dt::INT64_TYPE>(v);
      break;
    }
    case te::dt::UINT16_TYPE:
    {
      uint16_t v = boost::lexical_cast<uint16_t>(strValue);
      data = new te::dt::SimpleData<uint16_t, te::dt::INT16_TYPE>(v);
      break;
    }
    case te::dt::UINT32_TYPE:
    {
      uint32_t v = boost::lexical_cast<uint32_t>(strValue);
      data = new te::dt::SimpleData<uint32_t, te::dt::INT32_TYPE>(v);
      break;
    }
    case te::dt::UINT64_TYPE:
    {
      uint64_t v = boost::lexical_cast<uint64_t>(strValue);
      data = new te::dt::SimpleData<uint64_t, te::dt::INT64_TYPE>(v);
      break;
    }
    default:
    {
      throw te::common::Exception(TE_TR("Unexpected data type!"));
    }
  }

  return data;
}

KD_ADAPTATIVE_TREE* te::attributefill::VectorToVectorMemory::getKDtree(te::da::DataSet* data, std::size_t toSrid)
{
  std::size_t geomPos = te::da::GetFirstSpatialPropertyPos(data);

  KD_ADAPTATIVE_TREE* kdtree = new KD_ADAPTATIVE_TREE(*data->getExtent(geomPos).release(), 5);

  std::vector<std::pair<te::gm::Coord2D, te::gm::Point> > kdset;

  data->moveBeforeFirst();

  while(data->moveNext())
  {
    std::auto_ptr<te::gm::Geometry> geom = data->getGeometry(geomPos);

    std::vector<te::gm::Point*> allPoints = getAllPointsOfGeometry(geom.get());

    for(std::size_t i = 0; i < allPoints.size(); ++i)
    {
      te::gm::Point* p = allPoints[i];

      if(p->getSRID() != toSrid)
      {
        p->transform((int)toSrid);
      }

      te::gm::Coord2D coord(p->getX(), p->getY());
      kdset.push_back(std::pair<te::gm::Coord2D, te::gm::Point>(coord, *p));
    }
  }

  kdtree->build(kdset);

  return kdtree;
}

double te::attributefill::VectorToVectorMemory::getMinimumDistance(te::da::DataSet* toDs,
                                                                  std::size_t,
                                                                  te::da::DataSet*,
                                                                  std::size_t,
                                                                  KD_ADAPTATIVE_TREE* kdtree)
{
  std::size_t toGeomPos = te::da::GetFirstSpatialPropertyPos(toDs);
  std::auto_ptr<te::gm::Geometry> toGeom = toDs->getGeometry(toGeomPos);

  std::vector<te::gm::Point*> allPoints = getAllPointsOfGeometry(toGeom.get());

  std::vector<double> distances;

  for(std::size_t i = 0; i < allPoints.size(); ++i)
  {
    te::gm::Point* p = allPoints[i];
    te::gm::Coord2D key = te::gm::Coord2D(p->getX(), p->getY());
    std::vector<te::gm::Point> points;
    points.push_back(te::gm::Point(std::numeric_limits<double>::max(), std::numeric_limits<double>::max()));
    std::vector<double> sqrDists;

    kdtree->nearestNeighborSearch(key, points, sqrDists, 1);

    distances.push_back(sqrDists[0]);
  }

  double finalResult = std::numeric_limits<double>::max();
  for(std::size_t i = 0; i < distances.size(); ++i)
  {
    if(distances[i] < finalResult)
      finalResult = distances[i];
  }

  return sqrt(finalResult);
}

std::vector<te::gm::Point*> te::attributefill::VectorToVectorMemory::getAllPointsOfGeometry(te::gm::Geometry* geom)
{
  std::size_t geomType = geom->getGeomTypeId();

  std::vector<te::gm::Point*> result;

  switch(geomType)
  {
    case te::gm::PointType:
    case te::gm::PointZType:
    case te::gm::PointMType:
    case te::gm::PointZMType:
    {
      te::gm::Point* g = dynamic_cast<te::gm::Point*>(geom);
      result.push_back(g);
      break;
    }
    case te::gm::MultiPointType:
    case te::gm::MultiPointZType:
    case te::gm::MultiPointMType:
    case te::gm::MultiPointZMType:
    {
      te::gm::MultiPoint* g = dynamic_cast<te::gm::MultiPoint*>(geom);
      for(std::size_t i = 0; i < g->getNumGeometries(); ++i)
      {
        te::gm::Geometry* gAux = g->getGeometryN(i);

        std::vector<te::gm::Point*> vec = getAllPointsOfGeometry(gAux);
        result.insert(result.end(), vec.begin(), vec.end());
      }
      break;
    }
    case te::gm::PolygonType:
    case te::gm::PolygonZType:
    case te::gm::PolygonMType:
    case te::gm::PolygonZMType:
    {
      te::gm::Polygon* g = dynamic_cast<te::gm::Polygon*>(geom);

      for(std::size_t i = 0; i < g->getNumRings(); ++i)
      {
        
        te::gm::Curve* c = g->getRingN(i);

        te::gm::LinearRing* lr = dynamic_cast<te::gm::LinearRing*>(c);

        for(std::size_t j = 0; j < lr->getNPoints(); ++j)
        {
          result.push_back(lr->getPointN(j));
        }
      }

      break;
    }
    case te::gm::MultiPolygonType:
    case te::gm::MultiPolygonZType:
    case te::gm::MultiPolygonMType:
    case te::gm::MultiPolygonZMType:
    {
      te::gm::MultiPolygon* g = dynamic_cast<te::gm::MultiPolygon*>(geom);

      for(std::size_t i = 0; i < g->getNumGeometries(); ++i)
      {
        te::gm::Geometry* gAux = g->getGeometryN(i);

        std::vector<te::gm::Point*> vec = getAllPointsOfGeometry(gAux);
        result.insert(result.end(), vec.begin(), vec.end());
      }
      break;
    }
    case te::gm::LineStringType:
    case te::gm::LineStringZType:
    case te::gm::LineStringMType:
    case te::gm::LineStringZMType:
    {
      te::gm::LineString* g = dynamic_cast<te::gm::LineString*>(geom);

      for(std::size_t i = 0; i < g->getNPoints(); ++i)
      {
        result.push_back(g->getPointN(i));
      }

      break;
    }
    case te::gm::MultiLineStringType:
    case te::gm::MultiLineStringZType:
    case te::gm::MultiLineStringMType:
    case te::gm::MultiLineStringZMType:
    {
      te::gm::MultiLineString* g = dynamic_cast<te::gm::MultiLineString*>(geom);

      for(std::size_t i = 0; i < g->getNumGeometries(); ++i)
      {
        te::gm::Geometry* gAux = g->getGeometryN(i);

        std::vector<te::gm::Point*> vec = getAllPointsOfGeometry(gAux);
        result.insert(result.end(), vec.begin(), vec.end());
      }

      break;
    }
    default:
    {
      return std::vector<te::gm::Point*>();
    }
  }

  return result;
}

bool te::attributefill::VectorToVectorMemory::hasNoIntersectionOperations()
{
  std::map<std::string, std::vector<te::attributefill::OperationType> >::iterator it = m_options.begin();

  while(it != m_options.end())
  {
    std::vector<te::attributefill::OperationType> ops = it->second;

    for(std::size_t i = 0; i < ops.size(); ++i)
    {
      if(ops[i] == te::attributefill::MIN_DISTANCE ||
         ops[i] == te::attributefill::PRESENCE)
        return true;
    }
    ++it;
  }

  return false;
}

bool te::attributefill::VectorToVectorMemory::checkGeometries(te::gm::Geometry* fromGeom, std::size_t fromPos, te::gm::Geometry* toGeom)
{
  if(!fromGeom->isValid())
  {
#ifdef TERRALIB_LOGGER_ENABLED
    std::string ex = TE_TR("\"From\" layer geometry at position ");
    ex += boost::lexical_cast<std::string>(fromPos);
    ex += TE_TR(" is invalid.");
    te::common::Logger::logDebug("attributefill", ex.c_str());
#endif //TERRALIB_LOGGER_ENABLED
    
    return false;
  }
  else if(!toGeom->isValid())
  {
#ifdef TERRALIB_LOGGER_ENABLED
    std::string ex = TE_TR("\"To\" layer geometry is invalid.");
    te::common::Logger::logDebug("attributefill", ex.c_str());
#endif //TERRALIB_LOGGER_ENABLED

    return false;
  }

  return true;
}