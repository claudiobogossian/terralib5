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
  \file Utils.cpp

  \brief This file contains a set of utility chart functions
*/

// TerraLib
#include "../../../se.h"
#include "../../../dataaccess/dataset/DataSet.h"
#include "../../../dataaccess/dataset/DataSetType.h"
#include "../../../datatype.h"
#include "Utils.h"
#include "Scatter.h"
#include "Histogram.h"

//QT
#include <QPen>

double te::qt::widgets::getDouble(const std::string& value, std::vector<std::string>& sVector)
{
  //verify if it exists 
  double result = 0.;
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

te::qt::widgets::Scatter* te::qt::widgets::createScatter(te::da::DataSet* dataset, int propX, int propY)
{
  te::qt::widgets::Scatter* newScatter = new te::qt::widgets::Scatter();
  int xType = dataset->getType()->getProperty(propX)->getType();
  int yType = dataset->getType()->getProperty(propY)->getType();

  while(dataset->moveNext())
  {
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
  } //end of the data set
  return newScatter;
}

te::qt::widgets::Histogram* te::qt::widgets::createHistogram(te::da::DataSet* dataset, int propId, int slices)
{
   te::qt::widgets::Histogram* newHistogram = new te::qt::widgets::Histogram();
   std::map<double, int> histogramValues;

   double minValue, maxValue;
   minValue = std::numeric_limits<double>::max();
   maxValue = -std::numeric_limits<double>::max();
   
   int propType = dataset->getType()->getProperty(propId)->getType();

   std::vector<double> intervals;
   std::vector<int> values;

   if((propType >= te::dt::INT16_TYPE && propType <= te::dt::UINT64_TYPE) || 
     propType == te::dt::FLOAT_TYPE || propType == te::dt::DOUBLE_TYPE || 
     propType == te::dt::NUMERIC_TYPE)
   {

     //Calculating the minimum and maximum values of the given property and adjusting the Histogram's interval.
     while(dataset->moveNext())
     {
       double teste = dataset->getDouble(propId);
       //calculate range
       if(minValue>dataset->getDouble(propId))
          minValue = dataset->getDouble(propId);
       if(maxValue<dataset->getDouble(propId))
         maxValue = dataset->getDouble(propId);

       double interval = maxValue - minValue;

       //Adjusting the interval to the user-defined number of slices.
       newHistogram->setInterval(interval/slices);
     }

     //Adjusting the histogram's intervals
     for (double i = minValue; i <(maxValue+newHistogram->getInterval()); i+=newHistogram->getInterval())
     {
       intervals.push_back(i);
     }

     values.resize(intervals.size(), 0);
    
     dataset->moveBeforeFirst();

     //Adjusting the Histogram's values
     while(dataset->moveNext())
     {
       double currentValue = dataset->getDouble(propId);

       for (unsigned int i= 0; i<intervals.size(); i++)
       {
         if((currentValue >= intervals[i]) && (currentValue < intervals[i+1]))
           values[i] =  values[i]+1;
       }
      }

     //With both the intervals and values ready, the map can be populated
     for (unsigned int i= 0; i<intervals.size(); i++)
     {
       std::pair<double, int> new_pair(intervals[i], values[i]);
       histogramValues.insert(new_pair);
     }
   }
   newHistogram->setValues(histogramValues);
   newHistogram->setMinValue(minValue);
   dataset->moveBeforeFirst();
   return newHistogram;
}

QwtText* te::qt::widgets::Terralib2Qwt(const std::string& text)
{
  QwtText* result = new QwtText(text.c_str()); 
  result->setBackgroundBrush(QBrush(QColor(0, 255, 0)));
  result->setBackgroundPen(QPen(Qt::red, 3, Qt::SolidLine));
  return result;
}

QwtText* te::qt::widgets::Terralib2Qwt(const std::string& text,  te::color::RGBAColor* color, 
                   te::se::Font*  font, te::se::Fill* backFill, 
                   te::se::Stroke* backStroke)
{
  QwtText* result = new QwtText(QString(text.c_str()));
 
  /*
  QColor color
  if(color==0)
  result->setColor();
  result->setFont();

  result->setBackgroundBrush();
  result->setBackgroundPen();
  */
  return result;
}

