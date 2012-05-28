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
  \file Scatter.cpp

  \brief A class to represent a scatter. 
*/

// TerraLib
#include "../../../dataaccess/dataset/DataSet.h"
#include "../../../dataaccess/dataset/DataSetType.h"
#include "../../../datatype.h"

#include "Scatter.h"

double getDouble(const std::string& value, std::vector<std::string>& sVector)
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

double getDouble(te::dt::DateTime* dateTime)
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

te::qt::widgets::Scatter::Scatter(te::da::DataSet* dataset, int propX, int propY)
  : m_minX(std::numeric_limits<double>::max()),
    m_maxX(-std::numeric_limits<double>::max()),
    m_minY(std::numeric_limits<double>::max()),
    m_maxY(-std::numeric_limits<double>::max())
{
   createScatter(dataset, propX, propY);
}

te::qt::widgets::Scatter::Scatter(const std::vector<double>& axisX, const std::vector<double>& axisY)
  : m_minX(std::numeric_limits<double>::max()),
    m_maxX(-std::numeric_limits<double>::max()),
    m_minY(std::numeric_limits<double>::max()),
    m_maxY(-std::numeric_limits<double>::max())
{
  std::vector<double>::const_iterator itx = axisX.begin();
  std::vector<double>::const_iterator ity = axisY.begin();
  while(itx!=axisX.end())
  {
    if(*itx < m_minX)
      m_minX = *itx;
    if(*itx > m_maxX)
      m_maxX = *itx; 
    m_xValues.push_back(*itx);
    ++itx;
  }

  while(ity!=axisY.end())
  {
    if(*ity < m_minY)
      m_minY = *ity;
    if(*ity > m_maxY)
      m_maxY = *ity; 
    m_yValues.push_back(*ity);
    ++ity;
  }
}

void
te::qt::widgets::Scatter::createScatter(te::da::DataSet* dataset, int propX, int propY)
{
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
        x_doubleValue = getDouble(sValue, m_xString);  
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
        y_doubleValue = getDouble(sValue, m_yString);   
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
      m_xValues.push_back(x_doubleValue);
      m_yValues.push_back(y_doubleValue);

      //calculate range
      if(x_doubleValue<m_minX)
        m_minX=x_doubleValue;
      if(x_doubleValue>m_maxX)
        m_maxX=x_doubleValue;

      if(y_doubleValue<m_minY)
        m_minY=y_doubleValue;
      if(y_doubleValue>m_maxY)
        m_maxY=y_doubleValue;
   } //end of the data set
}

std::size_t te::qt::widgets::Scatter::size()
{
  return m_xValues.size(); // equal to m_yValues.size();
}

double te::qt::widgets::Scatter::getX(unsigned int idx)
{
  return m_xValues[idx];
}

double* te::qt::widgets::Scatter::getX()
{
  return &m_xValues[0];
}

double te::qt::widgets::Scatter::getY(unsigned int idx)
{
  return m_yValues[idx];
}

double* te::qt::widgets::Scatter::getY()
{
  return &m_yValues[0];
}

double te::qt::widgets::Scatter::getMinX()
{
  return m_minX;
}

double te::qt::widgets::Scatter::getMaxX()
{
  return m_maxX;
}

double te::qt::widgets::Scatter::getMinY()
{
  return m_minY;
}

double te::qt::widgets::Scatter::getMaxY()
{
  return m_maxY;
}

te::qt::widgets::Scatter::~Scatter()
{
  
}

