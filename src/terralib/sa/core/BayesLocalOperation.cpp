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
  \file terralib/sa/core/BayesLocalOperation.cpp

  \brief This file contains a class that represents the bayes local operation.

  \reference Adapted from TerraLib4.
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
#include "../../graph/core/AbstractGraph.h"
#include "../../graph/core/Edge.h"
#include "../../graph/core/Vertex.h"
#include "../../memory/DataSet.h"
#include "../../memory/DataSetItem.h"
#include "BayesLocalOperation.h"
#include "Utils.h"

//STL
#include <cassert>

te::sa::BayesLocalOperation::BayesLocalOperation()
{
}

te::sa::BayesLocalOperation::~BayesLocalOperation()
{
}

void te::sa::BayesLocalOperation::execute()
{
  assert(m_inputParams->m_ds.get());
  assert(m_inputParams->m_dsType.get());

  //associate gpm event attribute
  std::size_t eventIdx = m_inputParams->m_dsType->getPropertyPosition(m_inputParams->m_eventAttrName);
  int eventType = m_inputParams->m_ds->getPropertyDataType(eventIdx);
  int gpmEventIdx = te::sa::AssociateGPMVertexAttribute(m_inputParams->m_gpm.get(), m_inputParams->m_ds.get(), m_inputParams->m_gpmAttrLink, m_inputParams->m_eventAttrName, eventType);

  //associate gpm population attribute
  std::size_t popIdx = m_inputParams->m_dsType->getPropertyPosition(m_inputParams->m_populationAttrName);
  int popType = m_inputParams->m_ds->getPropertyDataType(popIdx);
  int gpmPopIdx = te::sa::AssociateGPMVertexAttribute(m_inputParams->m_gpm.get(), m_inputParams->m_ds.get(), m_inputParams->m_gpmAttrLink, m_inputParams->m_populationAttrName, popType);

  //create output data
  std::auto_ptr<te::da::DataSetType> outDsType = createDataSetType(m_inputParams->m_dsType.get());
  std::auto_ptr<te::mem::DataSet> outDs = createDataSet(m_inputParams->m_ds.get(), outDsType.get());

  //run bayes
  eventIdx = outDsType->getPropertyPosition(m_inputParams->m_eventAttrName);
  popIdx = outDsType->getPropertyPosition(m_inputParams->m_populationAttrName);

  std::size_t idIdx = outDsType->getPropertyPosition(m_inputParams->m_gpmAttrLink);
  std::size_t neighEventIdx = outDsType->getPropertyPosition(TE_SA_BAYESEVENT_ATTR_NAME);
  std::size_t neighPopIdx = outDsType->getPropertyPosition(TE_SA_BAYESPOP_ATTR_NAME);
  std::size_t bayesIdx = outDsType->getPropertyPosition(TE_SA_BAYES_ATTR_NAME);

  runBayesLocal(outDs.get(), idIdx, eventIdx, popIdx, neighEventIdx, neighPopIdx, bayesIdx, gpmEventIdx, gpmPopIdx);

  //save data
  saveDataSet(outDs.get(), outDsType.get());
}

void te::sa::BayesLocalOperation::setParameters(te::sa::BayesInputParams* inParams, te::sa::BayesOutputParams* outParams)
{
  m_inputParams.reset(inParams);
  m_outputParams.reset(outParams);
}

void te::sa::BayesLocalOperation::saveDataSet(te::da::DataSet* dataSet, te::da::DataSetType* dsType)
{
  //save dataset
  dataSet->moveBeforeFirst();

  std::map<std::string, std::string> options;

  m_outputParams->m_ds->createDataSet(dsType, options);

  m_outputParams->m_ds->add(m_outputParams->m_outputDataSetName, dataSet, options);
}

std::auto_ptr<te::da::DataSetType> te::sa::BayesLocalOperation::createDataSetType(te::da::DataSetType* dsType)
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

  //create neighbour event property
  te::dt::SimpleProperty* neighEvent = new te::dt::SimpleProperty(TE_SA_BAYESEVENT_ATTR_NAME, te::dt::DOUBLE_TYPE);
  dataSetType->add(neighEvent);

  //create neighbour population property
  te::dt::SimpleProperty* neighPop = new te::dt::SimpleProperty(TE_SA_BAYESPOP_ATTR_NAME, te::dt::DOUBLE_TYPE);
  dataSetType->add(neighPop);

  //create bayes property
  te::dt::SimpleProperty* bayesProperty = new te::dt::SimpleProperty(TE_SA_BAYES_ATTR_NAME, te::dt::DOUBLE_TYPE);
  dataSetType->add(bayesProperty);

  return dataSetType;
}

std::auto_ptr<te::mem::DataSet> te::sa::BayesLocalOperation::createDataSet(te::da::DataSet* inputDataSet, te::da::DataSetType* dsType)
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
    outDSetItem->setDouble(TE_SA_BAYESEVENT_ATTR_NAME, 0.);

    //set kernel default value
    outDSetItem->setDouble(TE_SA_BAYESPOP_ATTR_NAME, 0.);

    //set kernel default value
    outDSetItem->setDouble(TE_SA_BAYES_ATTR_NAME, 0.);

    //add item into dataset
    outDataset->add(outDSetItem);
  }

  return outDataset;
}

void te::sa::BayesLocalOperation::runBayesLocal(te::mem::DataSet* ds, std::size_t idIdx, std::size_t eventIdx, std::size_t popIdx, 
          std::size_t neighEventIdx, std::size_t neighPopIdx, std::size_t bayesIdx, std::size_t gpmEventIdx, std::size_t gpmPopIdx)
{
  assert(m_inputParams->m_gpm.get());

  te::graph::AbstractGraph* graph = m_inputParams->m_gpm->getGraph();

  {
    //create task
    te::common::TaskProgress task;

    task.setTotalSteps(ds->size());
    task.setMessage(TE_TR("Calculating events and population for each element."));

    //calculate neighbour events and population for each element
    ds->moveBeforeFirst();

    while(ds->moveNext())
    {
      std::string strId = ds->getAsString(idIdx);

      int id = atoi(strId.c_str());

      double totEvent = ds->getDouble(eventIdx);
      double totPop = ds->getDouble(popIdx);

      te::graph::Vertex* v = graph->getVertex(id);

      if(v)
      {
        std::set<int> neighbours = v->getSuccessors();
        std::set<int>::iterator itNeighbours = neighbours.begin();
        int nNeighbours = (int)neighbours.size();

        while(itNeighbours != neighbours.end())
        {
          te::graph::Edge* e = graph->getEdge(*itNeighbours);
          te::graph::Vertex* vTo = 0;

          if(e)
          {
            if(e->getIdFrom() == id)
              vTo = graph->getVertex(e->getIdTo());
            else
              vTo = graph->getVertex(e->getIdFrom());
          }

          if(vTo)
          {
            totEvent += te::sa::GetDataValue(vTo->getAttributes()[gpmEventIdx]);
            totPop += te::sa::GetDataValue(vTo->getAttributes()[gpmPopIdx]);
          }

          ++itNeighbours;
        }
      }

      ds->setDouble(neighEventIdx, totEvent);
      ds->setDouble(neighPopIdx, totPop);
    }

    if(!task.isActive())
    {
      throw te::common::Exception(TE_TR("Operation canceled by the user."));
    }

    task.pulse();
  }


  {
    //create task
    te::common::TaskProgress task;

    task.setTotalSteps(ds->size());
    task.setMessage(TE_TR("Calculating Local Bayes."));

    //calculate local bayes values
    ds->moveBeforeFirst();

    while(ds->moveNext())
    {
      std::string strId = ds->getAsString(idIdx);

      int id = atoi(strId.c_str());

      double myEvent = ds->getDouble(eventIdx);
      double myPop = ds->getDouble(popIdx);

      double totEvent = ds->getDouble(neighEventIdx);
      double totPop = ds->getDouble(neighPopIdx);

      if(totPop <= 0.)
        throw;

      double mean = totEvent / totPop;

      double thetaI = (myPop > 0) ? myEvent / myPop : 0.0;

      te::graph::Vertex* v = graph->getVertex(id);

      if(v)
      {
        std::set<int> neighbours = v->getSuccessors();
        std::set<int>::iterator itNeighbours = neighbours.begin();
        int nNeighbours = (int)neighbours.size();

        if(nNeighbours != 0)
        {
          double variance = myPop * pow((myEvent / myPop) - mean, 2);

          while(itNeighbours != neighbours.end())
          {
            te::graph::Edge* e = graph->getEdge(*itNeighbours);
            te::graph::Vertex* vTo = 0;

            if(e)
            {
              if(e->getIdFrom() == id)
                vTo = graph->getVertex(e->getIdTo());
              else
                vTo = graph->getVertex(e->getIdFrom());
            }

            if(vTo)
            {
              double toEvent = te::sa::GetDataValue(vTo->getAttributes()[gpmEventIdx]);
              double toPop = te::sa::GetDataValue(vTo->getAttributes()[gpmPopIdx]);

              variance += toPop * pow((toEvent / toPop) - mean, 2);
            }

            ++itNeighbours;
          }

          variance /= totPop;

          double aux = variance - (mean * ((double)nNeighbours + 1.) / totPop);

          if(aux < 0.) 
            aux = 0.;

          double wI = 1.;

          if(aux != 0. || mean != 0.)
            wI = aux / (aux + (mean / myPop));

          thetaI = wI * (myEvent / myPop) + (1 - wI) * mean;
        }
      }

      ds->setDouble(bayesIdx, thetaI * m_inputParams->m_rate);
    }

    if(!task.isActive())
    {
      throw te::common::Exception(TE_TR("Operation canceled by the user."));
    }

    task.pulse();
  }
}
