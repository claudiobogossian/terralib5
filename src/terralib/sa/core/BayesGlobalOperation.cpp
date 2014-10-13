/*  Copyright (C) 2001-2009 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/sa/core/BayesGlobalOperation.cpp

  \brief This file contains a class that represents the bayes global operation.

  \reference Adapted from TerraLib4.

  \reference Mapping Disease and Mortality Rates Using Empirical Bayes Estimators
             Roger J. Marshall
*/

//TerraLib
#include "../../common/Exception.h"
#include "../../common/Translator.h"
#include "../../common/progress/TaskProgress.h"
#include "../../dataaccess/datasource/DataSource.h"
#include "../../dataaccess/datasource/DataSourceManager.h"
#include "../../dataaccess/utils/Utils.h"
#include "../../datatype/SimpleProperty.h"
#include "../../geometry/Geometry.h"
#include "../../geometry/GeometryProperty.h"
#include "../../memory/DataSet.h"
#include "../../memory/DataSetItem.h"
#include "BayesGlobalOperation.h"
#include "Utils.h"

//STL
#include <cassert>

te::sa::BayesGlobalOperation::BayesGlobalOperation()
{
}

te::sa::BayesGlobalOperation::~BayesGlobalOperation()
{
}

void te::sa::BayesGlobalOperation::execute()
{
  assert(m_inputParams->m_ds.get());
  assert(m_inputParams->m_dsType.get());

  //create output data
  std::auto_ptr<te::da::DataSetType> outDsType = createDataSetType(m_inputParams->m_dsType.get());

  std::auto_ptr<te::mem::DataSet> outDs = createDataSet(m_inputParams->m_ds.get(), outDsType.get());

  //run bayes
  std::size_t eventIdx = outDsType->getPropertyPosition(m_inputParams->m_eventAttrName);
  std::size_t popIdx = outDsType->getPropertyPosition(m_inputParams->m_populationAttrName);
  std::size_t bayesIdx = outDsType->getPropertyPosition(TE_SA_BAYES_ATTR_NAME);

  runBayesGlobal(outDs.get(), eventIdx, popIdx, bayesIdx);

  //save data
  saveDataSet(outDs.get(), outDsType.get());
}

void te::sa::BayesGlobalOperation::setParameters(te::sa::BayesInputParams* inParams, te::sa::BayesOutputParams* outParams)
{
  m_inputParams.reset(inParams);
  m_outputParams.reset(outParams);
}

void te::sa::BayesGlobalOperation::saveDataSet(te::da::DataSet* dataSet, te::da::DataSetType* dsType)
{
  //save dataset
  dataSet->moveBeforeFirst();

  std::map<std::string, std::string> options;

  m_outputParams->m_ds->createDataSet(dsType, options);

  m_outputParams->m_ds->add(m_outputParams->m_outputDataSetName, dataSet, options);
}

std::auto_ptr<te::da::DataSetType> te::sa::BayesGlobalOperation::createDataSetType(te::da::DataSetType* dsType)
{
  std::auto_ptr<te::da::DataSetType> dataSetType(new te::da::DataSetType(m_outputParams->m_outputDataSetName));

  //create all input dataset properties
  std::vector<te::dt::Property*> propertyVec = dsType->getProperties();

  for(std::size_t t = 0; t < propertyVec.size(); ++t)
  {
    te::dt::Property* prop = propertyVec[t];

    te::dt::Property* newProp = prop->clone();
    newProp->setId(0);
    newProp->setParent(0);

    dataSetType->add(newProp);
  }

  //create bayes property
  te::dt::SimpleProperty* bayesProperty = new te::dt::SimpleProperty(TE_SA_BAYES_ATTR_NAME, te::dt::DOUBLE_TYPE);
  dataSetType->add(bayesProperty);

  return dataSetType;
}

std::auto_ptr<te::mem::DataSet> te::sa::BayesGlobalOperation::createDataSet(te::da::DataSet* inputDataSet, te::da::DataSetType* dsType)
{
  std::auto_ptr<te::mem::DataSet> outDataset(new te::mem::DataSet(dsType));

  std::size_t nProp = inputDataSet->getNumProperties();

  inputDataSet->moveBeforeFirst();

  while(inputDataSet->moveNext())
  {
    //create dataset item
    te::mem::DataSetItem* outDSetItem = new te::mem::DataSetItem(outDataset.get());

    for(std::size_t t = 0; t < nProp; ++t)
    {
      te::dt::AbstractData* ad = inputDataSet->getValue(t).release();

      outDSetItem->setValue(t, ad);
    }

    //set kernel default value
    outDSetItem->setDouble(TE_SA_BAYES_ATTR_NAME, 0.);

    //add item into dataset
    outDataset->add(outDSetItem);
  }

  return outDataset;
}

void te::sa::BayesGlobalOperation::runBayesGlobal(te::mem::DataSet* ds, std::size_t eventIdx, std::size_t popIdx, std::size_t bayesIdx)
{
  std::vector<double> eventVecY;
  std::vector<double> popVecN;
  std::vector<double> rateVec;
  double sumPopN = 0.;
  double sumEventY = 0.;
  int count = 0;

  ds->moveBeforeFirst();

  {
    //create task
    te::common::TaskProgress task;

    task.setTotalSteps(ds->size());
    task.setMessage(TE_TR("Getting Event/Population information."));

    while(ds->moveNext())
    {
      ++ count;

      double eventAttr = ds->getDouble(eventIdx);
      eventVecY.push_back(eventAttr);

      double popAttr = ds->getDouble(popIdx);
      popVecN.push_back(popAttr);

      if(popAttr <= 0.)
        throw te::common::Exception(TE_TR("Invalid value (0) for Population Attribute."));

      rateVec.push_back(eventVecY[count-1] / popVecN[count-1]);

      sumEventY += eventVecY[count-1];
      sumPopN += popVecN[count-1];

      if(!task.isActive())
      {
        throw te::common::Exception(TE_TR("Operation canceled by the user."));
      }

      task.pulse();
    }
  }

  double mean = sumEventY / sumPopN;

  double meanPop = sumPopN / (double)count;

  double variance = 0.;

  for(int i = 0; i < count; ++i)
  {
    variance += popVecN[i] * pow((rateVec[i] - mean), 2);
  }

  variance /= sumPopN;

  double aux = variance - (mean / meanPop);

  if(aux < 0.)
    aux = 0.;

  //calculate bayes value
  ds->moveBeforeFirst();

  {
    //create task
    te::common::TaskProgress task;

    task.setTotalSteps(ds->size());
    task.setMessage(TE_TR("Calculating Bayes Value."));

    while(ds->moveNext())
    {
      double eventAttr = ds->getDouble(eventIdx);
      double popAttr = ds->getDouble(popIdx);

      double wI = 1.;

      if(aux != 0. || mean != 0.)
        wI = aux / (aux + (mean / popAttr));

      double thetaI = wI * (eventAttr / popAttr) + ( 1 - wI) * mean;

      ds->setDouble(bayesIdx, thetaI * m_inputParams->m_rate);

      if(!task.isActive())
      {
        throw te::common::Exception(TE_TR("Operation canceled by the user."));
      }

      task.pulse();
    }
  }
}
