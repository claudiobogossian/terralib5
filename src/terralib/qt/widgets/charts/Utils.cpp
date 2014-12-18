/*  Copyright (C) 2010-2013 National Institute For Space Research (INPE) - Brazil.

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
  \file Utils.cpp

  \brief This file contains a set of utility chart functions
*/

// TerraLib
#include "../../../color/RGBAColor.h"
#include "../../../common/progress/TaskProgress.h"
#include "../../../dataaccess/dataset/DataSet.h"
#include "../../../dataaccess/dataset/DataSetType.h"
#include "../../../dataaccess/dataset/ObjectIdSet.h"
#include "../../../dataaccess/utils/Utils.h"
#include "../../../datatype.h"
#include "../../../raster.h"
#include "../../../maptools/ExternalGraphicRendererManager.h"
#include "../../../maptools/MarkRendererManager.h"
#include "../../../qt/widgets/Utils.h"
#include "../../../se/Utils.h"
#include "../../../se.h"
#include "../../../statistics/core/Enums.h"
#include "../../../statistics/core/NumericStatisticalSummary.h"
#include "../../../statistics/core/StringStatisticalSummary.h"
#include "../../../statistics/core/SummaryFunctions.h"
#include "ChartDisplay.h"
#include "ChartDisplayWidget.h"
#include "ChartStyle.h"
#include "Histogram.h"
#include "HistogramChart.h"
#include "Scatter.h"
#include "ScatterChart.h"
#include "Utils.h"

//Boost
#include <boost/lexical_cast.hpp>
#include <boost/ptr_container/ptr_vector.hpp>

//QT
#include <QPen>

//STL
#include <memory>

double getDouble(const std::string& value, std::vector<std::string>& sVector)
{
  //verify if it exists 
  for(std::size_t i=0;i<sVector.size();++i)
  {
    if(value==sVector[i])
      return (double)i;
  }

  sVector.push_back(value);
  return (double)sVector.size()-1;
}

double getDouble(te::dt::DateTime* dateTime)
{
  if(dateTime->getTypeCode() == te::dt::TIME_INSTANT)
  {
    std::auto_ptr<te::dt::TimeInstant> ti ((te::dt::TimeInstant*)dateTime);
    boost::gregorian::date basedate(1400, 01, 01);
    boost::gregorian::date_duration days = ti->getDate().getDate() - basedate;
    long long int seconds = ti->getTime().getTimeDuration().total_seconds();
    long long int dias = days.days();
    double v = (double) dias * 86400 + seconds;
    return v;
  }
  else if(dateTime->getTypeCode() == te::dt::DATE)
  {
    std::auto_ptr<te::dt::Date> d ((te::dt::Date*)dateTime);
    boost::gregorian::date basedate(1400, 01, 01);
    boost::gregorian::date_duration days = d->getDate() - basedate;
    double v = days.days();
    return v;
  }
  return 0.;
}

/*
  Auxiliary function used to acquire the current value of the dataset as a double regardless of it's (numeric) type

  param dataset The dataset being used, must be given at the position of interest
  param propId The id of the property of interest;
  note Will return 0 if the type of the property was not numeric.
*/
double getDouble(te::da::DataSet* dataset, int propId)
{
  double res = 0.;

  int propType = dataset->getPropertyDataType(propId);
  switch (propType)
    {
      case(te::dt::INT16_TYPE):
        res = dataset->getInt16(propId);
        break;
      case(te::dt::UINT16_TYPE):
        res = dataset->getInt16(propId);
        break;
      case(te::dt::INT32_TYPE):
        res = dataset->getInt32(propId);
        break;
      case(te::dt::UINT32_TYPE):
        res = dataset->getInt32(propId);
        break;
      case(te::dt::INT64_TYPE):
        res = dataset->getInt64(propId);
        break;
      case(te::dt::UINT64_TYPE):
        res = dataset->getInt64(propId);
        break;
      case(te::dt::FLOAT_TYPE):
        res = dataset->getFloat(propId);
        break;
      case(te::dt::NUMERIC_TYPE):
        res = boost::lexical_cast<double>(dataset->getNumeric(propId));
        break;
      default:
        res = dataset->getDouble(propId);
        break;
    }

 return res;
}

/*
  Auxiliary function used to acquire the summarized string value of the dataset using the selected statistical function

  param stat The selected satistical function, pass -1 if no function is to be used;
  param sss The StringStatisticalSummary that will make the relevant calculations and return the result
*/
std::string getStatisticalValue(int stat, te::stat::StringStatisticalSummary& sss)
{
  std::string res;

  switch (stat)
  {
    case(te::stat::MIN_VALUE):
      res = sss.m_minVal;
      break;
    case(te::stat::MAX_VALUE):
      res = sss.m_maxVal;
      break;
    //case(te::stat::COUNT):
    //  res = dataset->size();
    //  break;
    //case(te::stat::VALID_COUNT):
    //  break;
    default:
      break;
  }
  return res;
}

/*
  Auxiliary function used to acquire the summarized double value of the dataset using the selected statistical function

  param stat The selected satistical function, pass -1 if no function is to be used;
  param sss The NumericStatisticalSummary that will make the relevant calculations and return the result
*/
double getStatisticalValue(int stat, te::stat::NumericStatisticalSummary& nss)
{
  double res;

  switch (stat)
  {
    case(te::stat::MIN_VALUE):
      res = nss.m_minVal;
      break;
    case(te::stat::MAX_VALUE):
      res = nss.m_maxVal;
      break;
    //case(te::stat::COUNT):
    //  res = dataset->size();
    //  break;
    //case(te::stat::VALID_COUNT):
    //  res = getDouble(dataset, propId);
    //  break;
    case(te::stat::MEAN):
      res = nss.m_mean;
      break;
    case(te::stat::SUM):
      res = nss.m_sum;
      break;
    case(te::stat::STANDARD_DEVIATION):
      res = nss.m_stdDeviation;
      break;
    case(te::stat::VARIANCE):
      res = nss.m_variance;
      break;
    case(te::stat::SKEWNESS):
      res = nss.m_skewness;
      break;
    case(te::stat::KURTOSIS):
      res = nss.m_kurtosis;
      break;
    case(te::stat::AMPLITUDE):
      res = nss.m_amplitude;
      break;
    case(te::stat::MEDIAN):
      res = nss.m_median;
      break;
    case(te::stat::VAR_COEFF):
      res = nss.m_varCoeff;
      break;
    //case(te::stat::MODE):
    //  res = nss.m_mode[0];
      break;
    default:
      res = 0.0;
      break;
  }
  return res;
}

/*
  Auxiliary function used to a scatter with summarized values

  param stat The selected satistical function, pass -1 if no function is to be used;
  param oidsToSummarize A map containing all the  objectIds mapped by the oid of the base dataset.
  param valuesToSummarize A map containing all the  values9points) mapped by the oid of the base dataset.
  param scatter Output parameter, the scatter to be populated.

  note This function is only going to populate the sacatter's data if it a statistical funcion has been selected,
         otherwise it will return the scatter as it received it.
*/
void buildSummarizedScatter(int stat, std::map<std::string, std::vector<te::da::ObjectId*> > oidsToSummarize,
                            std::map<std::string, std::vector<std::pair<double, double> > > valuesToSummarize,
                            te::qt::widgets::Scatter* scatter)
{
  //Containers used to hold informations temporarily, used to organize them prior to inserting them on the histogram.
  std::map<std::string, std::vector<te::da::ObjectId*> >::iterator oidsIt;
  std::map<std::string, std::vector<std::pair<double, double> > > ::iterator valuesIt;
  te::stat::NumericStatisticalSummary ss;

  //Acquiring the summarized values
  for(valuesIt = valuesToSummarize.begin(); valuesIt !=  valuesToSummarize.end(); ++valuesIt)
  {
    if((*valuesIt).second.size() > 1 && (stat != -1))
    {
      std::vector<double> xValues;
      std::vector<double> yValues;
      std::vector<te::da::ObjectId*> oids;

      for(size_t i = 0; i < (*valuesIt).second.size(); ++i)
      {
        xValues.push_back((*valuesIt).second[i].first);
        yValues.push_back((*valuesIt).second[i].second);
        oids = oidsToSummarize[(*valuesIt).first];
      }

      double summarizedXValue, summarizedYValue;

      te::stat::GetNumericStatisticalSummary(xValues, ss);
      summarizedXValue = getStatisticalValue(stat, ss);

      te::stat::GetNumericStatisticalSummary(yValues, ss);
      summarizedYValue = getStatisticalValue(stat, ss);

      for(size_t j = 0; j < oids.size(); ++j)
        scatter->addData(summarizedXValue, summarizedYValue, oids[j]);
    }
    else
    {
      //A summary was requested, but it was not needed due to the fact the there is only one value for each base oid

      double xValue = (*valuesIt).second[0].first;
      double yValue =  (*valuesIt).second[0].second;

      std::vector<te::da::ObjectId*> oids;
      oids = oidsToSummarize[(*valuesIt).first];

      scatter->addData(xValue, yValue, oids[0]);
    }
  }
}

/*
  Auxiliary function used to populate the frequencies, interval and their respective objectIds based on a statistical function

  param slices The number of slices for the histogram, used to calculate the number of intervals;
  param stat The selected satistical function, pass -1 if no function is to be used;
  param valuestoSummarize a map containing all the values and their objectIds mapped by the oid of the base dataset,
        defaults to a one-to-one relationship between value and objectId unless the histogram is based on a linked layer;
  param intervals Output parameter, the vector containing the intervals.
  param intervalToOIds Output parameter, the map that associates every interval to their objectIds.
  param frequencies Output parameter, the vector containing the frequencies.
  param minValue Output parameter, the minimum value of the histogram.
  param interval Output parameter, the interval of the histogram.
*/
void buildNumericFrequencies(int slices, int stat, std::map<std::string, std::vector<std::pair<double, te::da::ObjectId*> > > valuestoSummarize,
                             std::vector<double>& intervals, std::map<double, std::vector<te::da::ObjectId*> >& intervalToOIds,
                             std::vector< unsigned int>& frequencies, double& minValue, double& interval)
{
  //Containers used to hold informations temporarily, used to organize them prior to inserting them on the histogram.
  std::map<std::string, std::vector<std::pair<double, te::da::ObjectId*> > >::iterator valuesIt;
  std::vector<std::pair<double, std::vector<te::da::ObjectId*> > > summarizedValuesToOId;
  te::stat::NumericStatisticalSummary ss;

  //Acquiring the summarized values
  for(valuesIt = valuestoSummarize.begin(); valuesIt !=  valuestoSummarize.end(); ++valuesIt)
  {
    if((*valuesIt).second.size() > 1 && stat != -1)
    {
      std::vector<double> values;
      std::vector<te::da::ObjectId*> oids;
      for(size_t i = 0; i < (*valuesIt).second.size(); ++i)
      {
        values.push_back((*valuesIt).second[i].first);
        oids.push_back((*valuesIt).second[i].second);
      }

      double summarizedValue;
      te::stat::GetNumericStatisticalSummary(values, ss);
      summarizedValue = getStatisticalValue(stat, ss);
      summarizedValuesToOId.push_back(std::make_pair(summarizedValue, oids));
    }
    else
    {
      for(size_t i = 0; i < (*valuesIt).second.size(); ++i)
      {
        std::vector<te::da::ObjectId*> oids;
        oids.push_back((*valuesIt).second[i].second);
        summarizedValuesToOId.push_back(std::make_pair((*valuesIt).second[i].first, oids));
      }
    }
  }

  double maxValue = -std::numeric_limits<double>::max();

  for(size_t i = 0; i < summarizedValuesToOId.size(); ++i)
  {
    double currentValue = summarizedValuesToOId[i].first;

    //calculate range
    if(minValue > currentValue)
      minValue = currentValue;
    if(maxValue < currentValue)
      maxValue = currentValue;
  }

  //Adjusting the interval to the user-defined number of slices.
  interval = ((maxValue - minValue)/slices);

  //Adjusting the histogram's intervals
  for (double i = minValue; i <(maxValue+interval); i+=interval)
  {
    intervals.push_back(i);
    std::vector<te::da::ObjectId*> valuesOIds;
    intervalToOIds.insert(std::make_pair(i, valuesOIds));
  }

  frequencies.resize(intervals.size(), 0);

  //Adjusting the frequencies on each interval
  for(size_t i = 0; i < summarizedValuesToOId.size(); ++i)
  {
    double currentValue = summarizedValuesToOId[i].first;
    for (size_t j = 0; j<intervals.size(); ++j)
    {
      if((currentValue >= intervals[j]) && (currentValue <= intervals[j+1]))
      {
        for(size_t k= 0; k < summarizedValuesToOId[i].second.size(); ++k)
          intervalToOIds.at(intervals[j]).push_back(summarizedValuesToOId[i].second[k]);

        frequencies[j] =  frequencies[j]++;
        break;
      }
    }
  }
}

/*
  Auxiliary function used to populate the frequencies, interval and their respective objectIds based on a statistical function

  param stat The selected satistical function, pass -1 if no function is to be used;
  param valuestoSummarize a map containing all the values and their objectIds mapped by the oid of the base dataset,
        defaults to a one-to-one relationship between value and objectId unless the histogram is based on a linked layer;
  param intervalToOIds Output parameter, the map that associates every interval to their objectIds.
  param frequencies Output parameter, the vector containing the frequencies.

*/
void buildStringFrequencies(int stat, std::map<std::string, std::vector<std::pair<std::string, te::da::ObjectId*> > > valuestoSummarize,
                            std::map<std::string, std::vector<te::da::ObjectId*> >& intervalToOIds,
                            std::vector< unsigned int>& frequencies)
{
  //Containers and iterators used to hold informations temporarily, used to organize them prior to inserting them on the histogram.
  std::map<std::string, std::vector<std::pair<std::string, te::da::ObjectId*> > >::iterator valuesIt;
  std::map<std::string, std::vector<te::da::ObjectId*> >::iterator intervalsIt;
  std::vector<std::pair<std::string, std::vector<te::da::ObjectId*> > > summarizedValuesToOId;
  te::stat::StringStatisticalSummary ss;

  //Acquiring the summarized values
  for(valuesIt = valuestoSummarize.begin(); valuesIt !=  valuestoSummarize.end(); ++valuesIt)
  {
    if((*valuesIt).second.size() > 1 && (stat != -1))
    {
      std::vector<std::string> values;
      std::vector<te::da::ObjectId*> oids;
      for(size_t i = 0; i < (*valuesIt).second.size(); ++i)
      {
        values.push_back((*valuesIt).second[i].first);
        oids.push_back((*valuesIt).second[i].second);
      }

      std::string summarizedValue;
      te::stat::GetStringStatisticalSummary(values, ss);
      summarizedValue = getStatisticalValue(stat, ss);
      summarizedValuesToOId.push_back(std::make_pair(summarizedValue, oids));
    }
    else
    {
      for(size_t i = 0; i < (*valuesIt).second.size(); ++i)
      {
        std::vector<te::da::ObjectId*> oids;
        oids.push_back((*valuesIt).second[i].second);
        summarizedValuesToOId.push_back(std::make_pair((*valuesIt).second[i].first, oids));
      }
    }
  }

  //Adjusting the frequencies on each interval
  for(size_t i = 0; i < summarizedValuesToOId.size(); ++i)
  {
    int j;
    std::string currentValue = summarizedValuesToOId[i].first;
    for (  j= 0, intervalsIt = intervalToOIds.begin(); intervalsIt != intervalToOIds.end(); ++intervalsIt,++j)
    {
      if(currentValue == (*intervalsIt).first)
      {
        for(size_t k= 0; k < summarizedValuesToOId[i].second.size(); ++k)
          intervalToOIds.at(currentValue).push_back(summarizedValuesToOId[i].second[k]);

        frequencies[j] =  frequencies[j]++;
        break;
      }
    }
  }
}

/*
  Auxiliary function used to acquire the current objectId of the dataset

  param dataset The dataset being used, must be given at the position of interest;
  param pkeys A vector containing all the properties that form the primaryKey of the given dataset
*/
te::da::ObjectId* getObjectId(te::da::DataSet* dataset, std::vector<std::size_t> pkeys)
{
  std::vector<size_t>::iterator it;
  std::vector<std::string> propNames;

  for(it=pkeys.begin(); it!=pkeys.end(); ++it)
    propNames.push_back(dataset->getPropertyName(*it));

  //The caller will take ownership of the generated pointer.
  te::da::ObjectId* oid = te::da::GenerateOID(dataset, propNames);
  return oid;
}

void getObjectIds (te::da::DataSet* dataset, std::vector<std::size_t> pkeys, std::vector<te::da::ObjectId*>& valuesOIDs)
{
  te::da::ObjectId* oid = getObjectId(dataset,pkeys); 
  valuesOIDs.push_back(oid);
}

te::qt::widgets::Scatter* te::qt::widgets::createScatter(te::da::DataSet* dataset, te::da::DataSetType* dataType, int propX, int propY, int stat)
{
  te::qt::widgets::Scatter* newScatter = new te::qt::widgets::Scatter();

  std::vector<std::size_t> objIdIdx;
  te::da::GetOIDPropertyPos(dataType, objIdIdx);

  std::map<double, std::vector<te::da::ObjectId*> > valuesIdsByinterval;
  std::vector<te::da::ObjectId*> valuesOIds;

  std::size_t rpos = te::da::GetFirstPropertyPos(dataset, te::dt::RASTER_TYPE);
  if(rpos != std::string::npos)
  {
    std::auto_ptr<te::rst::Raster> raster(dataset->getRaster(rpos));

     unsigned int nCol = raster->getNumberOfColumns();
     unsigned int nLin = raster->getNumberOfRows();

    te::common::TaskProgress task;
    task.setTotalSteps(nCol);
    task.setMessage("Scatter creation");

      for (unsigned int c=0; c < nCol; ++c)
      {
        if(!task.isActive())
        {
          break;
        }

        for (unsigned int r=0;  r <nLin; ++r)
        {
              double val1, val2;

              raster->getValue(c, r, val1, propX);
              raster->getValue(c, r, val2, propY);

              newScatter->addX(val1);
              newScatter->addY(val2);
        }

        task.pulse();
      }
  }
  else
  {
    int xType = dataset->getPropertyDataType(propX);
    int yType = dataset->getPropertyDataType(propY);

    //Acquiring the name of the base dataset and how many properties are included in it's primary key
    std::pair<std::string, int> dsProps;
    te::da::GetOIDDatasetProps(dataType, dsProps);

    //A map containg the summarized values used to buil d the histogram
    std::map<std::string, std::vector<te::da::ObjectId*> > oidsToSummarize;
    std::map<std::string, std::vector<std::pair<double, double> > > valuesToSummarize;

    te::common::TaskProgress task;
    task.setTotalSteps(dataset->getNumProperties());
    task.setMessage("Scatter creation");

    while(dataset->moveNext())
    {

      if(!task.isActive())
      {
        break;
      }

      double x_doubleValue = 0.;
      double y_doubleValue = 0.;

      if((xType >= te::dt::INT16_TYPE && xType <= te::dt::UINT64_TYPE) || 
        xType == te::dt::FLOAT_TYPE || xType == te::dt::DOUBLE_TYPE || 
        xType == te::dt::NUMERIC_TYPE)
      {
        if(dataset->isNull(propX))
          continue;

        x_doubleValue = getDouble(dataset, propX);
      }
      else if(xType == te::dt::DATETIME_TYPE)
      {
        if(dataset->isNull(propX))
          continue;

        std::auto_ptr<te::dt::DateTime> dateTime = dataset->getDateTime(propX);
        x_doubleValue = getDouble(dateTime.get());
      }

      //======treat the Y value
      if((yType >= te::dt::INT16_TYPE && yType <= te::dt::UINT64_TYPE) || 
        yType == te::dt::FLOAT_TYPE || yType == te::dt::DOUBLE_TYPE || 
        yType == te::dt::NUMERIC_TYPE)
      {
        if(dataset->isNull(propY))
          continue;
        y_doubleValue = getDouble(dataset, propY);
      }
      else if(yType == te::dt::DATETIME_TYPE)
      {
        if(dataset->isNull(propY))
          continue;

        std::auto_ptr<te::dt::DateTime> dateTime = dataset->getDateTime(propY);
        y_doubleValue = getDouble(dateTime.get());
      }

      //insert values into the vectors
      te::da::ObjectId* currentOid = getObjectId(dataset, objIdIdx);

      if(stat == -1)
      {
        newScatter->addData(x_doubleValue, y_doubleValue, currentOid);
      }
      else
      {
        oidsToSummarize[te::da::getBasePkey(currentOid, dsProps)].push_back(currentOid);
        valuesToSummarize[te::da::getBasePkey(currentOid, dsProps)].push_back(std::make_pair(x_doubleValue, y_doubleValue));
      }
      task.pulse();
    } //end of the data set
    if(stat != -1)
      buildSummarizedScatter(stat, oidsToSummarize, valuesToSummarize, newScatter);
  }
  newScatter->calculateMinMaxValues();
  return newScatter;
}

te::qt::widgets::ChartDisplayWidget* te::qt::widgets::createScatterDisplay(te::da::DataSet* dataset, te::da::DataSetType* dataType, int propX, int propY, int stat)
{
  //Creating the scatter and it's chart with the given dataset
  te::qt::widgets::ScatterChart* chart = new te::qt::widgets::ScatterChart(te::qt::widgets::createScatter(dataset, dataType, propX, propY, stat));

  //Creating and adjusting the chart Display's style.
  te::qt::widgets::ChartStyle* chartStyle = new te::qt::widgets::ChartStyle();
  chartStyle->setTitle(QString::fromStdString("Scatter"));
  chartStyle->setAxisX(QString::fromStdString(dataset->getPropertyName(propX)));
  chartStyle->setAxisY(QString::fromStdString(dataset->getPropertyName(propY)));

  //Creating and adjusting the chart Display
  te::qt::widgets::ChartDisplay* chartDisplay = new te::qt::widgets::ChartDisplay(0, QString::fromStdString("Scatter"), chartStyle);
  chartDisplay->adjustDisplay();
  chart->attach(chartDisplay);

  //Adjusting the chart widget
  te::qt::widgets::ChartDisplayWidget* displayWidget = new te::qt::widgets::ChartDisplayWidget(chart, te::qt::widgets::SCATTER_CHART, chartDisplay);
  displayWidget->show();
  displayWidget->setWindowTitle("Scatter");
  return displayWidget;
}

te::qt::widgets::Histogram* te::qt::widgets::createHistogram(te::da::DataSet* dataset, te::da::DataSetType* dataType, int propId, int slices, int stat)
{
  if(slices <=1)
    slices = 2;

  te::qt::widgets::Histogram* newHistogram = new te::qt::widgets::Histogram();

  std::size_t rpos = te::da::GetFirstPropertyPos(dataset, te::dt::RASTER_TYPE);
  std::vector<std::size_t> objIdIdx;
  te::da::GetOIDPropertyPos(dataType, objIdIdx);

  if(rpos != std::string::npos)
  {
    std::auto_ptr<te::rst::Raster> rstptr = dataset->getRaster(rpos);
    std::map<double, unsigned int> values =  rstptr->getBand(propId)->getHistogramR(0, 0, 0, 0, slices);

    for(std::map<double, unsigned int>::iterator it = values.begin(); it != values.end(); ++it)
    {
      newHistogram->insert(std::make_pair(new te::dt::Double(it->first), it->second));
    }
    newHistogram->setMinValue(rstptr->getBand(propId)->getMinValue(true).real());
  }
  else
  {
    int propType = dataset->getPropertyDataType(propId);
    newHistogram->setType(propType);

     if((propType >= te::dt::INT16_TYPE && propType <= te::dt::UINT64_TYPE) || 
       propType == te::dt::FLOAT_TYPE || propType == te::dt::DOUBLE_TYPE || propType == te::dt::NUMERIC_TYPE)
     {
      //Acquiring the name of the base dataset and how many of it's properties are included in it's primary key
      std::pair<std::string, int> dsProps;
      te::da::GetOIDDatasetProps(dataType, dsProps);

      //A map containg the summarized values used to buil d the histogram
      std::map<std::string, std::vector<std::pair<double, te::da::ObjectId*> > > valuesToSummarize;

      te::common::TaskProgress task;
      task.setMessage("Histogram creation");
      task.setTotalSteps((dataset->getNumProperties()) * 2);

      dataset->moveBeforeFirst();

     //Adjusting the Histogram's values
      while(dataset->moveNext())
      {

        if(dataset->isNull(propId))
          continue;

        if(!task.isActive())
        {
          break;
        }

        double currentValue = getDouble(dataset, propId);
        te::da::ObjectId* currentOid = getObjectId(dataset, objIdIdx);
        valuesToSummarize[te::da::getBasePkey(currentOid, dsProps)].push_back(std::make_pair(currentValue, currentOid));

        task.pulse();
      }

      //The minimum value
      double minValue = std::numeric_limits<double>::max();

      //The interval (testing)
      double interval = std::numeric_limits<double>::max();

      //A vector containing the intervals
      std::vector<double> intervals;

      //The vector containing the frequency of each interval, will be used to every property type
      std::vector< unsigned int> frequencies;

      //A map containing the intervals and their objecIds
      std::map<double, std::vector<te::da::ObjectId*> > intervalToOIds;

      //Populating the frequencies, intervals and their respective objectIds with the results of the chosen summary function
      buildNumericFrequencies(slices, stat, valuesToSummarize, intervals, intervalToOIds, frequencies, minValue, interval);

       //With both the intervals and frequencies ready, the map can be populated
      for (unsigned int i= 0; i<intervals.size(); ++i)
      {
        te::dt::Double* data = new te::dt::Double(intervals[i]);
        newHistogram->insert(std::make_pair(data, frequencies[i]), intervalToOIds.at(intervals[i]));
      }

      newHistogram->setMinValue(minValue);
      newHistogram->setInterval(interval);
      newHistogram->setSummarized(stat != -1);
      dataset->moveBeforeFirst();
    }
  }
   return newHistogram;
}

te::qt::widgets::Histogram* te::qt::widgets::createHistogram(te::da::DataSet* dataset, te::da::DataSetType* dataType, int propId, int stat)
{
  te::qt::widgets::Histogram* newHistogram = new te::qt::widgets::Histogram();

  std::size_t rpos = te::da::GetFirstPropertyPos(dataset, te::dt::RASTER_TYPE);
  std::vector<std::size_t> objIdIdx;
  te::da::GetOIDPropertyPos(dataType, objIdIdx);

  te::common::TaskProgress task;
  task.setMessage("Histogram creation");

  if(rpos != std::string::npos)
  {
    std::auto_ptr<te::rst::Raster> rstptr = dataset->getRaster(rpos);
    std::map<double, unsigned int> values =  rstptr->getBand(propId)->getHistogramR();

    for(std::map<double, unsigned int>::iterator it = values.begin(); it != values.end(); ++it)
    {
      task.pulse();
      newHistogram->insert(std::make_pair(new te::dt::Double(it->first), it->second));
    }
    newHistogram->setMinValue(rstptr->getBand(propId)->getMinValue(true).real());
  }
  else
  {
    int propType = dataset->getPropertyDataType(propId);
    newHistogram->setType(propType);

    //The vector containing the frequency of each interval, will be used to every property type
    std::vector< unsigned int> frequencies;

    if(propType == te::dt::DATETIME_TYPE || propType == te::dt::STRING_TYPE)
    {
      std::set <std::string> intervals;
      std::set <std::string>::iterator intervalsIt;
      std::map<std::string, std::vector<te::da::ObjectId*> > valuesIdsByinterval;
      std::vector<te::da::ObjectId*> valuesOIds;

      //Acquiring the name of the base dataset and how many properties are included in it's primary key
      std::pair<std::string, int> dsProps;
      te::da::GetOIDDatasetProps(dataType, dsProps);

      //A map containg the summarized values used to build the histogram
      std::map<std::string, std::vector<std::pair<std::string, te::da::ObjectId*> > > valuesToSummarize;

      //Adjusting the histogram's intervals
      dataset->moveBeforeFirst();
      while(dataset->moveNext())
      {

        if(dataset->isNull(propId))
          continue;

        if(!task.isActive())
        {
          break;
        }

        std::string interval = dataset->getString(propId);

        //Every unique string will be an interval
        intervals.insert(interval);
        task.pulse();
      }

        for (intervalsIt = intervals.begin(); intervalsIt != intervals.end(); ++intervalsIt)
        {
          valuesIdsByinterval.insert(make_pair((*intervalsIt), valuesOIds));
        }

      frequencies.resize(intervals.size(), 0);
      dataset->moveBeforeFirst();
      newHistogram->setStringInterval(intervals);

      //Adjusting the Histogram's values
      while(dataset->moveNext())
      {

        if(dataset->isNull(propId))
          continue;

        if(!task.isActive())
        {
          break;
        }

        std::string currentValue = dataset->getString(propId);
        te::da::ObjectId* currentOid = getObjectId(dataset, objIdIdx);
        valuesToSummarize[te::da::getBasePkey(currentOid, dsProps)].push_back(std::make_pair(currentValue, currentOid));

        task.pulse();
      }

      buildStringFrequencies(stat, valuesToSummarize, valuesIdsByinterval, frequencies);

      //With both the intervals and values ready, the map can be populated
      int i;
      for (i= 0, intervalsIt = intervals.begin(); intervalsIt != intervals.end();  ++intervalsIt,++i)
      {
        te::dt::String* data = new te::dt::String(*intervalsIt);
        newHistogram->insert(std::make_pair(data, frequencies[i]), valuesIdsByinterval.at(*intervalsIt));
      }

      dataset->moveBeforeFirst();
    }
  }
  newHistogram->setSummarized(stat != -1);
  return newHistogram;
}

te::qt::widgets::ChartDisplayWidget* te::qt::widgets::createHistogramDisplay(te::da::DataSet* dataset, te::da::DataSetType* dataType, int propId, int slices, int stat)
{
  te::qt::widgets::HistogramChart* chart;
  int propType = dataset->getPropertyDataType(propId);

  if(slices <=1)
    slices = 2;

  //Creating the histogram and it's chart with the given dataset
  if(propType == te::dt::DATETIME_TYPE || propType == te::dt::STRING_TYPE)
    chart =  new te::qt::widgets::HistogramChart(te::qt::widgets::createHistogram(dataset, dataType, propId, stat));
  else
    chart =  new te::qt::widgets::HistogramChart(te::qt::widgets::createHistogram(dataset, dataType, propId, slices, stat));

  //Creating and adjusting the chart Display's style.
  te::qt::widgets::ChartStyle* chartStyle = new te::qt::widgets::ChartStyle();
  chartStyle->setTitle(QString::fromStdString("Histogram"));
  chartStyle->setAxisX(QString::fromStdString(dataset->getPropertyName(propId)));
  chartStyle->setAxisY(QString::fromStdString("Frequency"));

  //Creating and adjusting the chart Display
  te::qt::widgets::ChartDisplay* chartDisplay = new te::qt::widgets::ChartDisplay(0, QString::fromStdString("Histogram"), chartStyle);
  chartDisplay->adjustDisplay();
  chart->attach(chartDisplay);

  //Adjusting the chart widget
  te::qt::widgets::ChartDisplayWidget* displayWidget = new te::qt::widgets::ChartDisplayWidget(chart, te::qt::widgets::HISTOGRAM_CHART, chartDisplay);
  displayWidget->show();
  displayWidget->setWindowTitle("Histogram");
  return displayWidget;
}

te::qt::widgets::ChartDisplayWidget* te::qt::widgets::createHistogramDisplay(te::da::DataSet* dataset, te::da::DataSetType* dataType, int propId, Histogram* histogram)
{
  te::qt::widgets::HistogramChart* chart;
  chart =  new te::qt::widgets::HistogramChart(histogram);

  //Creating and adjusting the chart Display's style.
  te::qt::widgets::ChartStyle* chartStyle = new te::qt::widgets::ChartStyle();
  chartStyle->setTitle(QString::fromStdString("Histogram"));
  chartStyle->setAxisX(QString::fromStdString(dataset->getPropertyName(propId)));
  chartStyle->setAxisY(QString::fromStdString("Frequency"));

  //Creating and adjusting the chart Display
  te::qt::widgets::ChartDisplay* chartDisplay = new te::qt::widgets::ChartDisplay(0, QString::fromStdString("Histogram"), chartStyle);
  chartDisplay->adjustDisplay();
  chart->attach(chartDisplay);

  //Adjusting the chart widget
  te::qt::widgets::ChartDisplayWidget* displayWidget = new te::qt::widgets::ChartDisplayWidget(chart, te::qt::widgets::HISTOGRAM_CHART, chartDisplay);
  displayWidget->show();
  displayWidget->setWindowTitle("Histogram");
  return displayWidget;
}

QwtText* te::qt::widgets::Terralib2Qwt(const std::string& text)
{
  QwtText* result = new QwtText(text.c_str()); 
  result->setBackgroundBrush(QBrush(QColor(0, 255, 0)));
  result->setBorderPen(QPen(Qt::red, 3, Qt::SolidLine));
  return result;
}

QwtText* te::qt::widgets::Terralib2Qwt(const std::string& text,  te::color::RGBAColor* color, 
                   te::se::Font*  font, te::se::Fill* backFill, 
                   te::se::Stroke* backStroke)
{
  QwtText* result = new QwtText(QString(text.c_str()));

  return result;
}

QwtSymbol* te::qt::widgets::Terralib2Qwt(te::se::Graphic* graphic)
{
  //Default symbol used in case the Graphic is not completely populated.
  QwtSymbol* symbol =  new QwtSymbol( QwtSymbol::Ellipse, QBrush( Qt::yellow ), QPen( Qt::red, 2 ), QSize( 8, 8 ));

  //Default size and width for the symbols
  size_t height = 8, width = 8;

  //Adjusting the size if the user changed it
  if(graphic->getSize())
  {
    height = te::se::GetInt(graphic->getSize());
    width = height;
  }

  /*Image that will be used to generate the symbol's pixmap,
    it can be either from a mark or from an external graphic, whichever is valid. 
  */
  te::color::RGBAColor** image;

  if(!graphic->getMarks().empty())
  {
    image = te::map::MarkRendererManager::getInstance().render(graphic->getMarks()[0], height);
  }
  else
  {
    image = te::map::ExternalGraphicRendererManager::getInstance().render(graphic->getExternalGraphics()[0], height, width);
  }

  QImage* qimg = te::qt::widgets::GetImage(image, height, width);
  QPixmap pixmap;
  pixmap = QPixmap::fromImage(*qimg);

  //Adjusting the symbol
  symbol->setPixmap(pixmap);
  symbol->setSize(width, height);

  delete image;
  delete qimg;

  return symbol;
}
