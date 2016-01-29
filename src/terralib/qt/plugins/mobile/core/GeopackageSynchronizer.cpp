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
\file src/terraMobilePlugin/core/GeoPackageSynchronizer.h

\brief This file is used to Synchronizer operation.
*/

#include "../../../../dataaccess/datasource/DataSourceTransactor.h"
#include "../../../../memory/DataSet.h"
#include "../../../../memory/DataSetItem.h"

#include "GeopackageSynchronizer.h"

te::qt::plugins::terramobile::GeoPackageSynchronizer::GeoPackageSynchronizer() :
  m_inputDataSource(0),
  m_inputDataSet(""),
  m_outputDataSource(0),
  m_outputDataset("")
{

}

te::qt::plugins::terramobile::GeoPackageSynchronizer::~GeoPackageSynchronizer()
{

}

void te::qt::plugins::terramobile::GeoPackageSynchronizer::setInputParameters(te::da::DataSource* inputDataSource, std::string inputDataSet, te::da::DataSource* outputDataSource, std::string outputDataset)
{
  m_inputDataSource = inputDataSource;
  m_inputDataSet = inputDataSet;

  m_outputDataSource = outputDataSource;
  m_outputDataset = outputDataset;
}

void te::qt::plugins::terramobile::GeoPackageSynchronizer::synchronize()
{
  checkParameters();

  //get dataset type
  std::auto_ptr<te::da::DataSetType> originalInputDsType = m_inputDataSource->getDataSetType(m_inputDataSet);

  //create dataset memory
  te::mem::DataSet* newData = new te::mem::DataSet(originalInputDsType.get());

  std::vector<te::dt::Property*> props = originalInputDsType->getProperties();

  //get data from gpkg
  std::auto_ptr<te::da::DataSet> dataSet = m_inputDataSource->getDataSet(m_inputDataSet);

  dataSet->moveBeforeFirst();

  while (dataSet->moveNext())
  {
    //create memory items
    te::mem::DataSetItem* dsItem = new te::mem::DataSetItem(newData);

    for (std::size_t t = 0; t < props.size(); ++t)
    {
      dsItem->setValue(props[t]->getName(), dataSet->getValue(props[t]->getName()).release());
    }

    newData->add(dsItem);
  }

  //save into output datasource
  std::unique_ptr<te::da::DataSourceTransactor> transactor = m_outputDataSource->getTransactor();

  std::map<std::string, std::string> op;

  try
  {
    transactor->add(m_outputDataset, newData, op);
  }
  catch (const te::common::Exception& e)
  {
    transactor->rollBack();

    return;
  }
  catch (const std::exception& e)
  {
    transactor->rollBack();

    return;
  }
  catch (...)
  {
    transactor->rollBack();

    return;
  }

  transactor->commit();
}

void te::qt::plugins::terramobile::GeoPackageSynchronizer::checkParameters()
{
  if (!m_inputDataSource)
    throw;

  if (m_inputDataSet.empty())
    throw;

  if (!m_outputDataSource)
    throw;

  if (m_outputDataset.empty())
    throw;
}
