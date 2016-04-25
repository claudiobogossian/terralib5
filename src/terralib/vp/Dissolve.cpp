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
 \file Dissolve.cpp
 */

#include "../common/StringUtils.h"
#include "../common/Translator.h"

#include "../dataaccess/dataset/DataSet.h"
#include "../dataaccess/dataset/DataSetAdapter.h"
#include "../dataaccess/dataset/DataSetType.h"
#include "../dataaccess/dataset/DataSetTypeConverter.h"
#include "../dataaccess/dataset/ObjectIdSet.h"
#include "../dataaccess/datasource/DataSource.h"
#include "../dataaccess/datasource/DataSourceInfo.h"
#include "../dataaccess/datasource/DataSourceInfoManager.h"
#include "../dataaccess/datasource/DataSourceCapabilities.h"
#include "../dataaccess/datasource/DataSourceTransactor.h"

#include "../dataaccess/query/Avg.h"
#include "../dataaccess/query/Cast.h"
#include "../dataaccess/query/Count.h"
#include "../dataaccess/query/Field.h"
#include "../dataaccess/query/Fields.h"
#include "../dataaccess/query/GroupBy.h"
#include "../dataaccess/query/GroupByItem.h"
#include "../dataaccess/query/Insert.h"
#include "../dataaccess/query/LiteralInt32.h"
#include "../dataaccess/query/Max.h"
#include "../dataaccess/query/Min.h"
#include "../dataaccess/query/PropertyName.h"
#include "../dataaccess/query/Select.h"
#include "../dataaccess/query/Sub.h"
#include "../dataaccess/query/SubSelect.h"
#include "../dataaccess/query/Sum.h"
#include "../dataaccess/query/ST_Dump.h"
#include "../dataaccess/query/ST_Multi.h"
#include "../dataaccess/query/ST_Union.h"
#include "../dataaccess/query/StdDev.h"
#include "../dataaccess/query/Variance.h"

#include "../dataaccess/utils/Utils.h"

#include "../datatype/Property.h"
#include "../datatype/SimpleData.h"
#include "../datatype/StringProperty.h"

#include "../geometry/Geometry.h"
#include "../geometry/GeometryCollection.h"
#include "../geometry/GeometryProperty.h"
#include "../geometry/MultiLineString.h"
#include "../geometry/MultiPoint.h"
#include "../geometry/MultiPolygon.h"
#include "../geometry/Utils.h"

#include "../memory/DataSet.h"
#include "../memory/DataSetItem.h"

#include "../statistics/core/NumericStatisticalSummary.h"
#include "../statistics/core/SummaryFunctions.h"
#include "../statistics/core/StringStatisticalSummary.h"
#include "../statistics/core/Utils.h"

#include "AlgorithmParams.h"
#include "Dissolve.h"
#include "ComplexData.h"
#include "Utils.h"

// BOOST
#include <boost/algorithm/string.hpp>

// STL 
#include <iostream>

te::vp::Dissolve::Dissolve()
{
}

bool te::vp::Dissolve::executeMemory(te::vp::AlgorithmParams* mainParams)
{
// Validating parameters
  std::vector<te::vp::InputParams> inputParams = mainParams->getInputParams();

// Get DataSetType and Geometry Property of InputLayer Layer.
  if (!inputParams[0].m_inputDataSetType)
    throw te::common::Exception(TE_TR("It is necessary to set the DataSetType from Input Layer."));

  std::auto_ptr<te::da::DataSetType> dsType_input(inputParams[0].m_inputDataSetType);

  te::gm::GeometryProperty* inputGeomProp = te::da::GetFirstGeomProperty(dsType_input.get());


// Verify if the operation has DataSet.
  if (!inputParams[0].m_inputDataSet)
    throw te::common::Exception(TE_TR("It is necessary to set the Input DataSet."));

  std::auto_ptr<te::da::DataSet>inputDataSet(inputParams[0].m_inputDataSet);


// Get Output DataSource.
  if (!mainParams->getOutputDataSource())
    throw te::common::Exception(TE_TR("It is necessary to set the Output DataSource."));

  te::da::DataSourcePtr outputDataSource = mainParams->getOutputDataSource();

// Build output dataset type
  std::auto_ptr<te::da::DataSetType> outputDataSetType(getOutputDataSetType(mainParams));

// Create output dataset in memory.
  std::auto_ptr<te::mem::DataSet> outputDataSet(new te::mem::DataSet(outputDataSetType.get()));

// Get the first geometry property from output datasettype.
  te::gm::GeometryProperty* geomProp = te::da::GetFirstGeomProperty(outputDataSetType.get());


//Get specific parameters.
  std::map<std::string, te::dt::AbstractData*> specificParams = mainParams->getSpecificParams();

// Get geometry output type (Multi/Single).
  bool isCollection = this->isCollection(specificParams);

// Get attributes that composes the dissolve operation 
  std::vector<std::string> dissolveProps = this->getDissolveProps(specificParams);
  if (dissolveProps.empty())
    throw te::common::Exception(TE_TR("Select at least one grouping attribute."));

// Get the positions of the dissolve properties
  std::vector<size_t> groupPropIdxs;
  
  for (std::size_t i = 0; i < dissolveProps.size(); ++i)
  {
    std::size_t position = dsType_input->getPropertyPosition(dissolveProps[i]);
    if (position >= 0 && position < dsType_input->size())
      groupPropIdxs.push_back(position);
  }

// Creates groups to dissolve the geometries and calculate the statistical summary.
  std::map<std::string, std::vector<int> > groups;
  std::map<std::string, std::vector<int> >::iterator it_groups;

  int dataSetPos = 0;
  inputDataSet->moveBeforeFirst();

  while (inputDataSet->moveNext())
  {
    std::string key = "";

    for (std::size_t i = 0; i < groupPropIdxs.size(); ++i)
    {
      if (inputDataSet->isNull(groupPropIdxs[i]))
      {
        //-------------------------IMPLEMENTAR WARNING------------------------------------//
        //std::string message = "The selected attribute to aggregate has null values.";

        //std::vector<std::string>::iterator it;

        //it = std::find(m_warnings.begin(), m_warnings.end(), message);
        //if (it == m_warnings.end())
        //  m_warnings.push_back(message);
        //-------------------------IMPLEMENTAR WARNING------------------------------------//
      }
      else
      {
        if (!key.empty())
          key += "_";

        key += inputDataSet->getAsString(groupPropIdxs[i]);
      }
    }

    it_groups = groups.find(key);
    if (it_groups == groups.end())
    {
      std::vector<int> dataSetPosVector;
      dataSetPosVector.push_back(dataSetPos);
      groups.insert(std::pair<std::string, std::vector<int> >(key, dataSetPosVector));
    }
    else
    {
      it_groups->second.push_back(dataSetPos);
    }

    ++dataSetPos;
  }

  it_groups = groups.begin();
  while (it_groups != groups.end())
  {
    //std::string value = it_groups->first;

    std::vector<te::gm::Geometry*> geomVec;
    for (std::size_t i = 0; i < it_groups->second.size(); ++i)
    {
      inputDataSet->move(it_groups->second[i]);
      std::auto_ptr<te::gm::Geometry> inGeometry = inputDataSet->getGeometry(inputGeomProp->getName());

      if (inGeometry->isValid())
        geomVec.push_back(inGeometry.release());
    }

// Output geometry vector if the output is not collection
    std::vector<te::gm::Geometry*> outGeomVec;

// Output geometry.
    te::gm::Geometry* outGeometry = te::vp::GetGeometryUnion(geomVec);

    if (!outGeometry->isValid())
    {
      //----------Emit MSG - Create Log - INVALID GEOMETRY.----------//
      ++it_groups;
      continue;
    }

// Add geometry result in a vector, with the correct geometry type.
    if (isCollection)
    {
      if (!te::vp::IsMultiType(outGeometry->getGeomTypeId()))
      {
        te::dt::AbstractData* abs = outGeometry->clone();
        te::gm::Geometry* singleGeom = static_cast<te::gm::Geometry*>(abs);
        outGeometry = this->setGeomAsMulti(singleGeom);
      }

      outGeomVec.push_back(outGeometry);
    }
    else
    {
      if (te::vp::IsMultiType(outGeometry->getGeomTypeId()))
      {
        te::gm::GeometryCollection* gcIn = dynamic_cast<te::gm::GeometryCollection*>(outGeometry);
        if (!gcIn->isValid())
        {
          //----------Emit MSG - Create Log - INVALID GEOMETRY.----------//
          outGeomVec.clear();
          ++it_groups;
          continue;
        }

        te::gm::GeometryCollection* gcOut = new te::gm::GeometryCollection(0, geomProp->getGeometryType(), outGeometry->getSRID());

        te::vp::SplitGeometryCollection(gcIn, gcOut);

        outGeomVec = gcOut->getGeometries();

        //delete gcOut;
      }
      else
      {
        outGeomVec.push_back(outGeometry);
      }
    }

// Create output dataset item.
    std::auto_ptr<te::mem::DataSetItem> outDSetItem(new te::mem::DataSetItem(outputDataSet.get()));
    std::size_t numProperties = outDSetItem->getNumProperties();

    std::vector<std::string> propNames = this->getDissolveProps(specificParams);

    for (std::size_t i = 0; i < propNames.size(); ++i)
    {
      te::dt::Property* prop = dsType_input->getProperty(propNames[i]);

      if (!prop)
        continue;

      std::string propName = prop->getName();

      inputDataSet->move(it_groups->second[0]);
      outDSetItem->setValue(propName, inputDataSet->getValue(propName)->clone());
    }
    

    if (isCollection)
    {
// Set the number of items grouped.
      outDSetItem->setInt32("NUM_OBJ", (int)it_groups->second.size());

// Get statistical summarization for the chosen attributes
      std::map<te::dt::Property*, std::vector<te::stat::StatisticalSummary> > summaryProps = this->getSummaryProps(specificParams);
      std::map<te::dt::Property*, std::vector<te::stat::StatisticalSummary> >::iterator summaryPropsIt = summaryProps.begin();

      std::vector<te::dt::Property*> properties = outputDataSetType->getProperties();

      while (summaryPropsIt != summaryProps.end())
      {
        te::dt::Property* currentProp = summaryPropsIt->first;
        std::string propertyName = currentProp->getName();

        if (summaryPropsIt->first->getType() == te::dt::STRING_TYPE)
        {
          std::vector<std::string> inputValueStatSummary;

          for (std::size_t i = 0; i < it_groups->second.size(); ++i)
          {
            inputDataSet->move(it_groups->second[i]);

            if (inputDataSet->isNull(currentProp->getId()))
              continue;

            std::string valueString = inputDataSet->getString(propertyName);
            inputValueStatSummary.push_back(valueString);
          }

          te::stat::StringStatisticalSummary ss;
          te::stat::GetStringStatisticalSummary(inputValueStatSummary, ss);

          std::size_t position = outputDataSetType->getPropertyPosition(propertyName + "_MIN_VALUE");
          if (position >= 0 && position < properties.size())
            outDSetItem->setString(position, ss.m_minVal);

          position = outputDataSetType->getPropertyPosition(propertyName + "_MAX_VALUE");
          if (position >= 0 && position < properties.size())
            outDSetItem->setString(position, ss.m_maxVal);

          position = outputDataSetType->getPropertyPosition(propertyName + "_COUNT");
          if (position >= 0 && position < properties.size())
            outDSetItem->setString(position, boost::lexical_cast<std::string>(ss.m_count));

          position = outputDataSetType->getPropertyPosition(propertyName + "_VALID_COUNT");
          if (position >= 0 && position < properties.size())
            outDSetItem->setString(position, boost::lexical_cast<std::string>(ss.m_validCount));

        }
        else
        {
          std::vector<double> inputValueStatSummary;

          for (std::size_t i = 0; i < it_groups->second.size(); ++i)
          {
            inputDataSet->move(it_groups->second[i]);

            if (inputDataSet->isNull(currentProp->getId()))
              continue;

            double valueDouble;

            switch (currentProp->getType())
            {
            case te::dt::INT16_TYPE:
              valueDouble = inputDataSet->getInt16(propertyName);
              break;
            case te::dt::INT32_TYPE:
              valueDouble = inputDataSet->getInt32(propertyName);
              break;
            case te::dt::INT64_TYPE:
              valueDouble = (double)inputDataSet->getInt64(propertyName);
              break;
            case te::dt::FLOAT_TYPE:
              valueDouble = inputDataSet->getFloat(propertyName);
              break;
            case te::dt::DOUBLE_TYPE:
              valueDouble = inputDataSet->getDouble(propertyName);
              break;
            default:
              continue;
              break;
            }

            inputValueStatSummary.push_back(valueDouble);
          }

          te::stat::NumericStatisticalSummary ss;
          te::stat::GetNumericStatisticalSummary(inputValueStatSummary, ss);

          std::size_t position = outputDataSetType->getPropertyPosition(propertyName + "_MIN_VALUE");
          if (position >= 0 && position < properties.size())
            outDSetItem->setDouble(position, ss.m_minVal);

          position = outputDataSetType->getPropertyPosition(propertyName + "_MAX_VALUE");
          if (position >= 0 && position < properties.size())
            outDSetItem->setDouble(position, ss.m_maxVal);

          position = outputDataSetType->getPropertyPosition(propertyName + "_COUNT");
          if (position >= 0 && position < properties.size())
            outDSetItem->setDouble(position, ss.m_count);

          position = outputDataSetType->getPropertyPosition(propertyName + "_VALID_COUNT");
          if (position >= 0 && position < properties.size())
            outDSetItem->setDouble(position, ss.m_validCount);

          position = outputDataSetType->getPropertyPosition(propertyName + "_MEAN");
          if (position >= 0 && position < properties.size())
            outDSetItem->setDouble(position, ss.m_mean);

          position = outputDataSetType->getPropertyPosition(propertyName + "_SUM");
          if (position >= 0 && position < properties.size())
            outDSetItem->setDouble(position, ss.m_sum);

          position = outputDataSetType->getPropertyPosition(propertyName + "_STANDARD_DEVIATION");
          if (position >= 0 && position < properties.size())
            outDSetItem->setDouble(position, ss.m_stdDeviation);

          position = outputDataSetType->getPropertyPosition(propertyName + "_VARIANCE");
          if (position >= 0 && position < properties.size())
            outDSetItem->setDouble(position, ss.m_variance);

          position = outputDataSetType->getPropertyPosition(propertyName + "_SKEWNESS");
          if (position >= 0 && position < properties.size())
            outDSetItem->setDouble(position, ss.m_skewness);

          position = outputDataSetType->getPropertyPosition(propertyName + "_KURTOSIS");
          if (position >= 0 && position < properties.size())
            outDSetItem->setDouble(position, ss.m_kurtosis);

          position = outputDataSetType->getPropertyPosition(propertyName + "_AMPLITUDE");
          if (position >= 0 && position < properties.size())
            outDSetItem->setDouble(position, ss.m_amplitude);

          position = outputDataSetType->getPropertyPosition(propertyName + "_MEDIAN");
          if (position >= 0 && position < properties.size())
            outDSetItem->setDouble(position, ss.m_median);

          position = outputDataSetType->getPropertyPosition(propertyName + "_VAR_COEFF");
          if (position >= 0 && position < properties.size())
            outDSetItem->setDouble(position, ss.m_varCoeff);

          position = outputDataSetType->getPropertyPosition(propertyName + "_MODE");
          if (position >= 0 && position < properties.size())
          {
            std::string modeValues = "";

            if (!ss.m_mode.empty())
            {
              modeValues = boost::lexical_cast<std::string>(ss.m_mode[0]);
              for (std::size_t i = 1; i < ss.m_mode.size(); ++i)
              {
                modeValues += ",";
                modeValues += boost::lexical_cast<std::string>(ss.m_mode[i]);
              }
            }
            
            outDSetItem->setString(position, modeValues);
          }
        }

        ++summaryPropsIt;
      }
    }

    for (std::size_t g = 0; g < outGeomVec.size(); ++g)
    {
      std::auto_ptr<te::mem::DataSetItem> item = outDSetItem->clone();
      if (outGeomVec[g]->isValid())
      {
        item->setGeometry("geom", outGeomVec[g]);
        outputDataSet->add(item.release());
      }
    }

    ++it_groups;
  }

// Persiste
  std::auto_ptr<te::da::DataSet> dataSetPrepared = PrepareAdd(outputDataSet.release(), outputDataSetType.get());

  if (!dataSetPrepared.get())
    throw te::common::Exception(TE_TR("Output DataSet was not prepared to save."));

  if (dataSetPrepared->size() == 0)
    throw te::common::Exception("The resultant layer is empty!");

  Save(outputDataSource.get(), dataSetPrepared.get(), outputDataSetType.get());

  return true;
}

bool te::vp::Dissolve::executeQuery(te::vp::AlgorithmParams* mainParams)
{
// Validating parameters
  std::vector<te::vp::InputParams> inputParams = mainParams->getInputParams();


// Get DataSetType and Geometry Property of InputLayer Layer.
  if (!inputParams[0].m_inputDataSetType)
    throw te::common::Exception(TE_TR("It is necessary to set the DataSetType from Input Layer."));
    
  std::auto_ptr<te::da::DataSetType> dsType_input(inputParams[0].m_inputDataSetType);

  te::gm::GeometryProperty* geom_input = te::da::GetFirstGeomProperty(dsType_input.get());
  
  std::string aliasInput = dsType_input->getName();


// Verify if the operation has Query.
  if (!inputParams[0].m_inputQuery)
    throw te::common::Exception(TE_TR("It is necessary to set the Input Query."));

  te::da::Select* selectInput = inputParams[0].m_inputQuery;
  te::da::SubSelect* subSelectInput = new te::da::SubSelect(selectInput, "inputLayer");
  aliasInput = subSelectInput->getAlias();


// Fields represents the properties from layer to compose the select query. 
  te::da::Fields fields;

// Get specift parameters.
  const std::map<std::string, te::dt::AbstractData*> specificParams = mainParams->getSpecificParams();

// Get attributes to dissolve.
  std::vector<std::string> dissolveAttributes = this->getDissolveProps(specificParams);

  if (dissolveAttributes.size() < 1)
    throw te::common::Exception(TE_TR("It is necessary to set at least one attribute to dissolve."));

  for (std::size_t d = 0; d < dissolveAttributes.size(); ++d)
  {
    te::da::Field* f_dissolveAtt = new te::da::Field(dissolveAttributes[d]);
    fields.push_back(f_dissolveAtt);
  }

  // Get geometry to spatial operation
  bool isCollection = this->isCollection(specificParams);

  std::map<te::dt::Property*, std::vector<te::stat::StatisticalSummary> > summaryAttributes;

  if (isCollection)
  {
    // number of objects in each group (mandatory)
    te::da::Expression* e_aggCount = new te::da::Count(new te::da::PropertyName(dissolveAttributes[0]));
    te::da::Expression* e_literalInt32 = new te::da::LiteralInt32(te::dt::INT32_TYPE);

    te::da::Expression* e_cast = new te::da::Cast(e_aggCount, e_literalInt32);
    te::da::Field* f_aggCount = new te::da::Field(*e_cast, "NUM_OBJ");
    fields.push_back(f_aggCount);


    // Get attributes to summary.
    summaryAttributes = this->getSummaryProps(specificParams);
    std::map<te::dt::Property*, std::vector<te::stat::StatisticalSummary> >::const_iterator itSummaryAtt = summaryAttributes.begin();

    while (itSummaryAtt != summaryAttributes.end())
    {
      std::vector<te::stat::StatisticalSummary>::const_iterator itFunc = itSummaryAtt->second.begin();
      while (itFunc != itSummaryAtt->second.end())
      {
        te::da::PropertyName* p_name = new te::da::PropertyName(itSummaryAtt->first->getName());
        int p_type = itSummaryAtt->first->getType();
        te::dt::Property* newProp = 0;

        te::da::Expression *s_exp, *e_max, *e_min;
        te::da::Field*  s_field;
        switch (*itFunc)
        {
        case te::stat::StatisticalSummary::MIN_VALUE:
        {
                                                      s_exp = new te::da::Min(p_name);
                                                      s_field = new te::da::Field(*s_exp, p_name->getName() + "_MIN_VALUE");
        }
          break;

        case te::stat::StatisticalSummary::MAX_VALUE:
        {
                                                      s_exp = new te::da::Max(p_name);
                                                      s_field = new te::da::Field(*s_exp, p_name->getName() + "_MAX_VALUE");
        }
          break;

        case te::stat::StatisticalSummary::MEAN:
        {
                                                 s_exp = new te::da::Avg(p_name);
                                                 s_field = new te::da::Field(*s_exp, p_name->getName() + "_MEAN");
        }
          break;

        case te::stat::StatisticalSummary::SUM:
        {
                                                s_exp = new te::da::Sum(p_name);
                                                s_field = new te::da::Field(*s_exp, p_name->getName() + "_SUM");
        }

          break;

        case te::stat::StatisticalSummary::COUNT:
        {
                                                  te::da::Expression* countName = new te::da::PropertyName("*");
                                                  te::da::Expression* s_count = new te::da::Count(countName);
                                                  te::da::Expression* s_literalInt32 = new te::da::LiteralInt32(te::dt::INT32_TYPE);
                                                  s_exp = new te::da::Cast(s_count, s_literalInt32);
                                                  s_field = new te::da::Field(*s_exp, p_name->getName() + "_COUNT");
        }
          break;

        case te::stat::StatisticalSummary::VALID_COUNT:
        {
                                                  te::da::Expression* s_vaidCount = new te::da::Count(p_name);
                                                  te::da::Expression* s_literalInt32 = new te::da::LiteralInt32(te::dt::INT32_TYPE);
                                                  s_exp = new te::da::Cast(s_vaidCount, s_literalInt32);
                                                  s_field = new te::da::Field(*s_exp, p_name->getName() + "_VALID_COUNT");
        }
          break;

        case te::stat::StatisticalSummary::STANDARD_DEVIATION:
        {
                                                               s_exp = new te::da::StdDev(p_name);
                                                               s_field = new te::da::Field(*s_exp, p_name->getName() + "_STANDARD_DEVIATION");
        }
          break;

        case te::stat::StatisticalSummary::VARIANCE:
        {
                                                     s_exp = new te::da::Variance(p_name);
                                                     s_field = new te::da::Field(*s_exp, p_name->getName() + "_VARIANCE");
        }
          break;

        case te::stat::StatisticalSummary::AMPLITUDE:
        {
                                                      e_min = new te::da::Min(p_name);
                                                      e_max = new te::da::Max(p_name);
                                                      s_exp = new te::da::Sub(*e_max, *e_min);
                                                      s_field = new te::da::Field(*s_exp, p_name->getName() + "_AMPLITUDE");
        }
          break;

        default:
          ++itFunc;
          continue;
        }

        fields.push_back(s_field);
        ++itFunc;
      }

      ++itSummaryAtt;
    }
  }

  te::da::Expression* e_union = new te::da::ST_Union(te::da::PropertyName(geom_input->getName()));
  te::da::Expression* e_collection = 0;

  if (isCollection)
    e_collection = new te::da::ST_Multi(*e_union);
  else
    e_collection = new te::da::ST_Dump(*e_union);

  te::da::Field* f_union = new te::da::Field(*e_collection, "geom");
  fields.push_back(f_union);


// FROM clause - This from clause is for input layer.
  if (!subSelectInput)
    throw te::common::Exception(TE_TR("A problem was found. SubSelect Input with problem."));

  te::da::From fromDissolve;
  fromDissolve.push_back(subSelectInput);

  te::da::Select select(fields, fromDissolve);

// Group by attribute to dissolve.
  te::da::GroupBy* groupBy = new te::da::GroupBy();

  for (std::size_t i = 0; i < dissolveAttributes.size(); ++i)
  {
    te::da::GroupByItem* e_groupBy = new te::da::GroupByItem(dissolveAttributes[i]);
    groupBy->push_back(e_groupBy);
  }
  select.setGroupBy(groupBy);


/*Check if the input and output dataSource are the same, if so,
  persists the result of select query into database with insert command.*/
  te::da::DataSourcePtr outputDataSource = mainParams->getOutputDataSource();

  te::da::DataSourceInfoPtr inDataSourceInfoPtr = te::da::DataSourceInfoManager::getInstance().get(inputParams[0].m_inputDataSource->getId());
  te::da::DataSourceInfoPtr outDataSourceInfoPtr = te::da::DataSourceInfoManager::getInstance().get(outputDataSource->getId());

// Create output dataset
  std::auto_ptr<te::da::DataSourceTransactor> t = outputDataSource->getTransactor();
  std::map<std::string, std::string> options;

// Build output dataset type
  std::auto_ptr<te::da::DataSetType> outputDataSetType(this->getOutputDataSetType(mainParams));

  if (outputDataSource->getType() == "OGR")
  {
    outputDataSource->createDataSet(outputDataSetType.get(), options);
  }
  else
  {
    t->begin();
    t->createDataSet(outputDataSetType.get(), options);
    t->commit();

    if (!inDataSourceInfoPtr)
    {
      t->rollBack();
      throw te::common::Exception(TE_TR("Input DataSource ID not found."));
    }

    if (!outDataSourceInfoPtr)
    {
      t->rollBack();
      throw te::common::Exception(TE_TR("Output DataSource ID not found."));
    }
  }

  std::string inputConnection = inDataSourceInfoPtr->getConnInfoAsString();
  std::string outputConnection = outDataSourceInfoPtr->getConnInfoAsString();


// Execute Query
  if (inputConnection == outputConnection)
  {
    te::da::Fields insertFields;

// Add the dissolve attributes.
    for (std::size_t p = 0; p < dissolveAttributes.size(); ++p)
    {
      te::da::Field* f_att = new te::da::Field(dissolveAttributes[p]);
      insertFields.push_back(f_att);
    }

    if (isCollection)
    {
      te::da::Field* f_numObj = new te::da::Field("NUM_OBJ");
      insertFields.push_back(f_numObj);

      // Add summary attributes.
      std::map<te::dt::Property*, std::vector<te::stat::StatisticalSummary> >::const_iterator itInsert = summaryAttributes.begin();

      while (itInsert != summaryAttributes.end())
      {
        std::vector<te::stat::StatisticalSummary>::const_iterator itFunc = itInsert->second.begin();
        while (itFunc != itInsert->second.end())
        {
          te::da::PropertyName* p_name = new te::da::PropertyName(itInsert->first->getName());
          int p_type = itInsert->first->getType();
          te::dt::Property* newProp = 0;

          te::da::Field*  s_field;
          switch (*itFunc)
          {
          case te::stat::StatisticalSummary::MIN_VALUE:
            s_field = new te::da::Field(p_name->getName() + "_MIN_VALUE");
            break;

          case te::stat::StatisticalSummary::MAX_VALUE:
            s_field = new te::da::Field(p_name->getName() + "_MAX_VALUE");
            break;

          case te::stat::StatisticalSummary::MEAN:
            s_field = new te::da::Field(p_name->getName() + "_MEAN");
            break;

          case te::stat::StatisticalSummary::SUM:
            s_field = new te::da::Field(p_name->getName() + "_SUM");
            break;

          case te::stat::StatisticalSummary::COUNT:
            s_field = new te::da::Field(p_name->getName() + "_COUNT");
            break;

          case te::stat::StatisticalSummary::VALID_COUNT:
            s_field = new te::da::Field(p_name->getName() + "_VALID_COUNT");
            break;

          case te::stat::StatisticalSummary::STANDARD_DEVIATION:
            s_field = new te::da::Field(p_name->getName() + "_STANDARD_DEVIATION");
            break;

          case te::stat::StatisticalSummary::VARIANCE:
            s_field = new te::da::Field(p_name->getName() + "_VARIANCE");
            break;

          case te::stat::StatisticalSummary::AMPLITUDE:
            s_field = new te::da::Field(p_name->getName() + "_AMPLITUDE");
            break;

          default:
            ++itFunc;
            continue;
          }

          insertFields.push_back(s_field);
          ++itFunc;
        }

        ++itInsert;
      }
    }

    te::da::Field* f_insert = new te::da::Field("geom");
    insertFields.push_back(f_insert);

    te::da::Insert* insert = new te::da::Insert(new te::da::DataSetName(mainParams->getOutputDataSetName()), &insertFields, &select);
    outputDataSource->execute(*insert);
  }
  else
  {
    std::auto_ptr<te::da::DataSet> dsQuery = inputParams[0].m_inputDataSource->query(select);
    dsQuery->moveBeforeFirst();

    if (dsQuery->size() == 0)
      throw te::common::Exception("The resultant layer is empty!");

    std::string outputDsName = mainParams->getOutputDataSetName();

    if (outputDataSource->getType() == "OGR")
    {
      outputDataSource->add(outputDsName, dsQuery.get(), options);
    }
    else
    {
      t->add(outputDsName, dsQuery.get(), options);
      t->commit();
    }
  }

  return true;
}

std::vector<std::string> te::vp::Dissolve::getDissolveProps(const std::map<std::string, te::dt::AbstractData*>& specificParams)
{
  std::vector<std::string> propNames;
  
  if (specificParams.empty())
    return propNames;
  
  std::map<std::string, te::dt::AbstractData*>::const_iterator it = specificParams.begin();

  while (it != specificParams.end())
  {
    if (it->first != "DISSOLVE")
    {
      ++it;
      continue;
    }

    te::vp::ComplexData<std::vector<std::string> >* cd =
      dynamic_cast<te::vp::ComplexData<std::vector<std::string> >* >(it->second);

    if (cd)
      propNames = cd->getValue();

    ++it;
  }

  return propNames;
}

std::map<te::dt::Property*, std::vector<te::stat::StatisticalSummary> > te::vp::Dissolve::getSummaryProps(const std::map<std::string, te::dt::AbstractData*>& specificParams)
{
  std::map<te::dt::Property*, std::vector<te::stat::StatisticalSummary> > statisticalSummaryMap;
  std::vector<std::string> statisticalSummaryVec;

  if (specificParams.empty())
    return statisticalSummaryMap;

  std::map<std::string, te::dt::AbstractData*>::const_iterator it = specificParams.begin();
  

  while (it != specificParams.end())
  {
    if (it->first != "SUMMARY")
    {
      ++it;
      continue;
    }

    te::vp::ComplexData<std::map<te::dt::Property*, std::vector<te::stat::StatisticalSummary> > >* cd =
      dynamic_cast<te::vp::ComplexData<std::map<te::dt::Property*, std::vector<te::stat::StatisticalSummary> > >* >(it->second);

    if (cd)
      statisticalSummaryMap = cd->getValue();

    ++it;
  }

  return statisticalSummaryMap;
}

bool te::vp::Dissolve::isCollection(const std::map<std::string, te::dt::AbstractData*>& specificParams)
{
  bool isCollection = false;

  std::map<std::string, te::dt::AbstractData*>::const_iterator it = specificParams.begin();
  while (it != specificParams.end())
  {
    if (it->first != "IS_COLLECTION")
    {
      ++it;
      continue;
    }

    te::dt::SimpleData<bool, te::dt::BOOLEAN_TYPE>* sd =
      dynamic_cast<te::dt::SimpleData<bool, te::dt::BOOLEAN_TYPE >* >(it->second);

    if (sd)
      isCollection = sd->getValue();

    ++it;
  }

  return isCollection;
}

te::da::DataSetType* te::vp::Dissolve::getOutputDataSetType(te::vp::AlgorithmParams* mainParams)
{
// Get the input parameters
  std::vector<te::vp::InputParams> inputParams = mainParams->getInputParams();

// Get the output dataset name.
  std::string outputDataSetName = mainParams->getOutputDataSetName();
  te::da::DataSetType* outputDataSetType = new te::da::DataSetType(outputDataSetName);


// Set to output datasettype the primary key property.
  if (mainParams->getOutputDataSource()->getType() != "OGR")
  {
    te::dt::SimpleProperty* pkProperty = new te::dt::SimpleProperty(outputDataSetName + "_id", te::dt::INT32_TYPE);
    pkProperty->setAutoNumber(true);
    outputDataSetType->add(pkProperty);

    te::da::PrimaryKey* pk = new te::da::PrimaryKey(outputDataSetName + "_pk", outputDataSetType);
    pk->add(pkProperty);
    outputDataSetType->setPrimaryKey(pk);
  }


// Get specific parameters.
  te::da::DataSetType* dsType = 0;
  if (inputParams[0].m_inputDataSetType)
  {
    dsType = inputParams[0].m_inputDataSetType;
  }
  else
  {
    dsType = te::da::GetDataSetType(inputParams[0].m_inputDataSetName, inputParams[0].m_inputDataSource->getId());
  }


// Set the Attribute to do the dissolve.
  const std::map<std::string, te::dt::AbstractData*> specificParams = mainParams->getSpecificParams();
  std::vector<std::string> propNames = this->getDissolveProps(specificParams);

  for (std::size_t i = 0; i < propNames.size(); ++i)
  {
    te::dt::Property* prop = dsType->getProperty(propNames[i]);

    if (!prop)
      continue;

    te::dt::SimpleProperty* currentProperty = new te::dt::SimpleProperty(propNames[i], prop->getType());
    outputDataSetType->add(currentProperty);
  }

  // Verify if the output geometry type is multi or single geometry.
  bool isCollection = this->isCollection(specificParams);

  // Is not allow to compute summary function if the output geometry is single type.
  if (isCollection)
  {
// The number of dissolved objects.
    te::dt::SimpleProperty* numDissolvedProperty = new te::dt::SimpleProperty("NUM_OBJ", te::dt::INT32_TYPE);
    outputDataSetType->add(numDissolvedProperty);


// Set the Summary Attribute to do the dissolve.
    std::map<te::dt::Property*, std::vector<te::stat::StatisticalSummary> > summaryAtt = this->getSummaryProps(specificParams);
    std::map<te::dt::Property*, std::vector<te::stat::StatisticalSummary> >::const_iterator itSummaryAtt = summaryAtt.begin();

    while (itSummaryAtt != summaryAtt.end())
    {
      std::vector<te::stat::StatisticalSummary> vectorResult = itSummaryAtt->second;
      
      int p_type = itSummaryAtt->first->getType();

      for (std::size_t s = 0; s < vectorResult.size(); ++s)
      {
        std::string attName = itSummaryAtt->first->getName();
        attName += "_" + te::stat::GetStatSummaryShortName(vectorResult[s]);

        if (p_type == te::dt::STRING_TYPE || vectorResult[s] == te::stat::MODE)
        {
          te::dt::SimpleProperty* funcProp = new te::dt::SimpleProperty(attName, te::dt::STRING_TYPE);
          outputDataSetType->add(funcProp);
        }
        else
        {
          te::dt::SimpleProperty* funcProp = new te::dt::SimpleProperty(attName, te::dt::DOUBLE_TYPE);
          outputDataSetType->add(funcProp);
        }
      }

      ++itSummaryAtt;
    }
  }

// Creating the geometry property
  te::gm::GeometryProperty* newGeomProp = new te::gm::GeometryProperty("geom");

  te::gm::GeometryProperty* intputGeomProp = te::da::GetFirstGeomProperty(dsType);

  te::gm::GeomType type = setGeomResultType(intputGeomProp->getGeometryType(), isCollection);
  newGeomProp->setGeometryType(type);
  newGeomProp->setSRID(mainParams->getOutputSRID());

  outputDataSetType->add(newGeomProp);

  return outputDataSetType;
}

te::gm::GeomType te::vp::Dissolve::setGeomResultType(const te::gm::GeomType& geomType, const bool& isCollection)
{
  if (isCollection)
  {
    if (te::vp::IsMultiType(geomType))
      return geomType; 
    else
      return te::vp::GetMultiType(geomType);
  }
  else
  {
    if (te::vp::IsMultiType(geomType))
      return te::vp::GetSimpleType(geomType);
    else
      return geomType;
  }
}

te::gm::Geometry* te::vp::Dissolve::setGeomAsMulti(te::gm::Geometry* geom)
{

  switch (geom->getGeomTypeId())
  {
    case te::gm::PointType:
    {
                            te::gm::MultiPoint* geomColl = new te::gm::MultiPoint(0, te::gm::MultiPointType, geom->getSRID());
                            geomColl->add(geom);

                            return geomColl;
    }
    case te::gm::LineStringType:
    {
                                 te::gm::MultiLineString* geomColl = new te::gm::MultiLineString(0, te::gm::MultiLineStringType, geom->getSRID());
                                 geomColl->add(geom);

                                 return geomColl;
    }
    case te::gm::PolygonType:
    {
                              te::gm::MultiPolygon* geomColl = new te::gm::MultiPolygon(0, te::gm::MultiPolygonType, geom->getSRID());
                              geomColl->add(geom);

                              return geomColl;
    }
  }

  return geom;
}

