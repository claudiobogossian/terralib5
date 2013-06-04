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
#include "../../../common/progress/TaskProgress.h"

#include "../../../dataaccess/dataset/DataSet.h"
#include "../../../dataaccess/dataset/DataSetType.h"
#include "../../../dataaccess/dataset/ObjectId.h"
#include "../../../dataaccess/dataset/ObjectIdSet.h"
#include "../../../dataaccess/utils/Utils.h"
#include "../../../datatype.h"
#include "Histogram.h"
#include "../../../raster.h"
#include "../../../raster/RasterSummary.h"
#include "../../../raster/RasterSummaryManager.h"
#include "Scatter.h"
#include "../../../se.h"
#include "Utils.h"
#include "../../../qt/widgets/se/Utils.h"

//QT
#include <QPen>

#include <memory>

double te::qt::widgets::getDouble(const std::string& value, std::vector<std::string>& sVector)
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

double te::qt::widgets::getDouble(te::dt::DateTime* dateTime)
{
  if(dateTime->getTypeCode() == te::dt::TIME_INSTANT)
  {
    te::dt::TimeInstant* ti = (te::dt::TimeInstant*)dateTime;
    boost::gregorian::date basedate(1400, 01, 01);
    boost::gregorian::date_duration days = ti->getDate().getDate() - basedate;
    long long int seconds = ti->getTime().getTimeDuration().total_seconds();
    long long int dias = days.days();
    double v = (double) dias * 86400 + seconds;
    return v;
  }
  else if(dateTime->getTypeCode() == te::dt::DATE)
  {
    te::dt::Date* d = (te::dt::Date*)dateTime;
    boost::gregorian::date basedate(1400, 01, 01);
    boost::gregorian::date_duration days = d->getDate() - basedate;
    double v = days.days();
    return v;
  }
  return 0.;
}

void te::qt::widgets::getObjectIds (te::da::DataSet* dataset, std::vector<std::size_t> pkeys, std::vector<te::da::ObjectId*>& valuesOIDs)
{
  te::da::ObjectIdSet* oids = new te::da::ObjectIdSet;

  // Mounting oidset
  std::vector<size_t>::iterator it;

  for(it=pkeys.begin(); it!=pkeys.end(); ++it)
    oids->addProperty(dataset->getPropertyName(*it), *it, dataset->getPropertyDataType(*it));

  te::da::ObjectId* oid = te::da::GenerateOID(dataset, oids->getPropertyNames());
  valuesOIDs.push_back(oid);
  //oids->add(te::da::GenerateOID(dataset, oids->getPropertyNames()));
}

te::qt::widgets::Scatter* te::qt::widgets::createScatter(te::da::DataSet* dataset, te::da::DataSetType* dataType, int propX, int propY)
{
  te::qt::widgets::Scatter* newScatter = new te::qt::widgets::Scatter();

  std::vector<std::size_t> objIdIdx;
  te::da::GetOIDPropertyPos(dataType, objIdIdx);

  std::map<double, std::vector<te::da::ObjectId*>> valuesIdsByinterval;
  std::vector<te::da::ObjectId*> valuesOIds;

  std::size_t rpos = te::da::GetFirstPropertyPos(dataset, te::dt::RASTER_TYPE);
  if(rpos != std::string::npos)
  {
    std::auto_ptr<te::rst::Raster> raster(dataset->getRaster(rpos));  

     unsigned int nCol = raster->getNumberOfColumns(); 
     unsigned int nLin = raster->getNumberOfRows();

    te::common::TaskProgress task;
    task.setTotalSteps(nCol * nLin);
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

              //calculate range
              if(val1<newScatter->getMinX())
                newScatter->setMinX(val1);
              if(val1>newScatter->getMaxX())
                newScatter->setMaxX(val1);

              if(val2<newScatter->getMinY())
                newScatter->setMinY(val2);
              if(val2>newScatter->getMaxY())
                newScatter->setMaxY(val2);

              task.pulse();
        }
      }
  }
  else
  {
    int xType = dataset->getPropertyDataType(propX);
    int yType = dataset->getPropertyDataType(propY);

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

      //====== treat the X value
      if(xType == te::dt::STRING_TYPE)
      {
        std::string sValue;
        if(dataset->isNull(propX))
          sValue = " null value";
        else
          sValue = dataset->getString(propX);
        x_doubleValue = getDouble(sValue, newScatter->getXString());  
      }
      else if((xType >= te::dt::INT16_TYPE && xType <= te::dt::UINT64_TYPE) || 
        xType == te::dt::FLOAT_TYPE || xType == te::dt::DOUBLE_TYPE || 
        xType == te::dt::NUMERIC_TYPE)
      {
        if(dataset->isNull(propX))
          continue;

        x_doubleValue = dataset->getDouble(propX);
      }
      else if(xType == te::dt::DATETIME_TYPE)
      {
        if(dataset->isNull(propX))
          continue;

        te::dt::DateTime* dateTime = dataset->getDateTime(propX);
        x_doubleValue = getDouble(dateTime);
        delete dateTime;
      }

      //======treat the Y value
      if(yType == te::dt::STRING_TYPE)
      {
        std::string sValue;
        if(dataset->isNull(propY) == false)
          sValue = dataset->getString(propY);
        else
          sValue = " null value";
        y_doubleValue = getDouble(sValue, newScatter->getYString());   
      }
      else if((yType >= te::dt::INT16_TYPE && yType <= te::dt::UINT64_TYPE) || 
        yType == te::dt::FLOAT_TYPE || yType == te::dt::DOUBLE_TYPE || 
        yType == te::dt::NUMERIC_TYPE)
      {
        if(dataset->isNull(propY))
          continue;
        y_doubleValue = dataset->getDouble(propY);
      }
      else if(yType == te::dt::DATETIME_TYPE)
      {
        if(dataset->isNull(propY))
          continue;

        te::dt::DateTime* dateTime = dataset->getDateTime(propY);
        y_doubleValue = getDouble(dateTime);
        delete dateTime;
      }

      //insert values into the vectors

      newScatter->addX(x_doubleValue);
      newScatter->addY(y_doubleValue);

      //calculate range
      if(x_doubleValue<newScatter->getMinX())
        newScatter->setMinX(x_doubleValue);
      if(x_doubleValue>newScatter->getMaxX())
        newScatter->setMaxX(x_doubleValue);

      if(y_doubleValue<newScatter->getMinY())
        newScatter->setMinY(y_doubleValue);
      if(y_doubleValue>newScatter->getMaxY())
        newScatter->setMaxY(y_doubleValue);
      task.pulse();
    } //end of the data set
  }
  return newScatter;
}

te::qt::widgets::Histogram* te::qt::widgets::createHistogram(te::da::DataSet* dataset, te::da::DataSetType* dataType, int propId, int slices)
{
  te::qt::widgets::Histogram* newHistogram = new te::qt::widgets::Histogram();

  int propType = dataset->getPropertyDataType(propId);

  newHistogram->setType(propType);

  std::vector<std::size_t> objIdIdx;
  te::da::GetOIDPropertyPos(dataType, objIdIdx);

  //The vector containing the frequency of each interval, will be used to every property type
  std::vector< unsigned int> values;

   if((propType >= te::dt::INT16_TYPE && propType <= te::dt::UINT64_TYPE) || 
     propType == te::dt::FLOAT_TYPE || propType == te::dt::DOUBLE_TYPE || 
     propType == te::dt::NUMERIC_TYPE)
   {

     std::map<double, unsigned int>* histogramValues = new std::map<double,  unsigned int>;
     std::map<double, std::vector<te::da::ObjectId*>> valuesIdsByinterval;
     std::vector<te::da::ObjectId*> valuesOIds;

     double minValue, maxValue;
     minValue = std::numeric_limits<double>::max();
     maxValue = -std::numeric_limits<double>::max();

     std::vector<double> intervals;

    te::common::TaskProgress task;
    task.setTotalSteps((dataset->getNumProperties()) * 2);
    task.setMessage("Histogram creation");

     //Calculating the minimum and maximum values of the given property and adjusting the Histogram's interval.
     while(dataset->moveNext())
     {

        if(!task.isActive())
        {
          break;
        }

       //calculate range
       if(minValue>dataset->getDouble(propId))
          minValue = dataset->getDouble(propId);
       if(maxValue<dataset->getDouble(propId))
         maxValue = dataset->getDouble(propId);

       double interval = maxValue - minValue;

       //Adjusting the interval to the user-defined number of slices.
       newHistogram->setInterval(interval/slices);
       task.pulse();
     }

     //Adjusting the histogram's intervals
     for (double i = minValue; i <(maxValue+newHistogram->getInterval()); i+=newHistogram->getInterval())
     {
       intervals.push_back(i);
       valuesIdsByinterval.insert(make_pair(i, valuesOIds));
     }

     values.resize(intervals.size(), 0);

     dataset->moveBeforeFirst();

     //Adjusting the Histogram's values
     while(dataset->moveNext())
     {

        if(!task.isActive())
        {
          break;
        }

       double currentValue = dataset->getDouble(propId);

       for (unsigned int i= 0; i<intervals.size(); ++i)
       {
         if((currentValue >= intervals[i]) && (currentValue <= intervals[i+1]))
         {
            values[i] =  values[i]+1;
            te::qt::widgets::getObjectIds(dataset, objIdIdx, valuesIdsByinterval.at(intervals[i]));
            break;
         }

       }
      task.pulse();
      }

     //With both the intervals and values ready, the map can be populated
     for (unsigned int i= 0; i<intervals.size(); ++i)
     {
       std::pair<double,  unsigned int> new_pair(intervals[i], values[i]);
       histogramValues->insert(new_pair);
     }

     newHistogram->setValues(histogramValues);
     newHistogram->setValuesOIDs(valuesIdsByinterval);
     newHistogram->setMinValue(minValue);
     dataset->moveBeforeFirst();

   }

   return newHistogram;
}

te::qt::widgets::Histogram* te::qt::widgets::createHistogram(te::da::DataSet* dataset, te::da::DataSetType* dataType, int propId)
{
  te::qt::widgets::Histogram* newHistogram = new te::qt::widgets::Histogram();

  std::size_t rpos = te::da::GetFirstPropertyPos(dataset, te::dt::RASTER_TYPE);

  te::common::TaskProgress task;
  task.setTotalSteps((dataset->getNumProperties()) * 2);
  task.setMessage("Histogram creation");

  if(rpos != std::string::npos)
  {
    const te::rst::RasterSummary* rs = te::rst::RasterSummaryManager::getInstance().get(dataset->getRaster(rpos), te::rst::SUMMARY_R_HISTOGRAM);
    newHistogram->setValues(rs->at(propId).m_histogramR);
  }
  else
  {
    int propType = dataset->getPropertyDataType(propId);

    newHistogram->setType(propType);

    std::vector<std::size_t> objIdIdx;
    te::da::GetOIDPropertyPos(dataType, objIdIdx);

    //The vector containing the frequency of each interval, will be used to every property type
    std::vector< unsigned int> values;
    
    if(propType == te::dt::DATETIME_TYPE || propType == te::dt::STRING_TYPE)
    {

      std::set <std::string> intervals;
      std::set <std::string>::iterator intervalsIt;
      std::map<std::string,  unsigned int>* histogramValues = new std::map<std::string,  unsigned int>;
      std::map<std::string, std::vector<te::da::ObjectId*>> valuesIdsByinterval;
      std::vector<te::da::ObjectId*> valuesOIds;

      //Adjusting the histogram's intervals
      while(dataset->moveNext())
      {

        if(!task.isActive())
        {
          break;
        }

        std::string interval = dataset->getString(propId);

        //Every unique string will be an interval
        intervals.insert(interval);
        valuesIdsByinterval.insert(make_pair(interval, valuesOIds));
        task.pulse();
      }

      values.resize(intervals.size(), 0);
      dataset->moveBeforeFirst();
      newHistogram->setStringInterval(intervals);

      //Adjusting the Histogram's values
      while(dataset->moveNext())
      {

        if(!task.isActive())
        {
          break;
        }

        std::string currentValue = dataset->getString(propId);
        int i;

        for (  i= 0, intervalsIt = intervals.begin(); intervalsIt != intervals.end(); ++intervalsIt,++i)
        {
          if(currentValue == *intervalsIt)
          {
            values[i] =  values[i]+1;
            te::qt::widgets::getObjectIds(dataset, objIdIdx, valuesIdsByinterval.at(*intervalsIt));
            break;
          }
        }
      task.pulse();
      }

      //With both the intervals and values ready, the map can be populated
      int i;
      for (i= 0, intervalsIt = intervals.begin(); intervalsIt != intervals.end();  ++intervalsIt,++i)
      {
        std::pair<std::string, unsigned int> new_pair(*intervalsIt, values[i]);
        histogramValues->insert(new_pair);
      }

      newHistogram->setStringValues(histogramValues);
      newHistogram->setStringOIDs(valuesIdsByinterval); 
      dataset->moveBeforeFirst();
    }
  }
  return newHistogram;
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

QwtSymbol* te::qt::widgets::Terralib2Qwt(te::se::Mark* mark)
{
  QwtSymbol* symbol =  new QwtSymbol( QwtSymbol::Ellipse, QBrush( Qt::yellow ), QPen( Qt::red, 2 ), QSize( 8, 8 ));
  QPen symbolPen;
  QBrush symbolBrush;
  te::qt::widgets::Config(symbolPen, mark->getStroke());
  te::qt::widgets::Config(symbolBrush, mark->getFill());
  symbolBrush.setStyle(Qt::SolidPattern);
  symbol->setPen(symbolPen);
  symbol->setBrush(symbolBrush);

  return symbol;
}