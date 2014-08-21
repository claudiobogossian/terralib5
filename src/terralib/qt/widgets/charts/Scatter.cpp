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
  \file Scatter.cpp

  \brief A class to represent a scatter. 
*/

//Terralib
#include "Scatter.h"
#include "../../../dataaccess/dataset/ObjectIdSet.h"

//STL
#include <limits>

te::qt::widgets::Scatter::Scatter(const std::vector<double>& axisX, const std::vector<double>& axisY)
  : m_xValues(axisX),
    m_yValues(axisY)
{
  calculateMinMaxValues();
}

te::qt::widgets::Scatter::Scatter()
  : m_minX(std::numeric_limits<double>::max()),
  m_maxX(-std::numeric_limits<double>::max()),
  m_minY(std::numeric_limits<double>::max()),
  m_maxY(-std::numeric_limits<double>::max())
{
}

te::qt::widgets::Scatter::~Scatter()
{
  
}

void te::qt::widgets::Scatter::calculateMinMaxValues()
{
  std::vector<double>::const_iterator itx = m_xValues.begin();
  std::vector<double>::const_iterator ity = m_yValues.begin();
  while(itx!=m_xValues.end())
  {
    if(*itx < m_minX)
      m_minX = *itx;
    if(*itx > m_maxX)
      m_maxX = *itx; 
    ++itx;
  }

  while(ity!=m_yValues.end())
  {
    if(*ity < m_minY)
      m_minY = *ity;
    if(*ity > m_maxY)
      m_maxY = *ity; 
    ++ity;
  }
}

std::size_t te::qt::widgets::Scatter::sizeX()
{
  return m_xValues.size();
}

std::size_t te::qt::widgets::Scatter::sizeY()
{
  return m_yValues.size();
}

std::vector<double> te::qt::widgets::Scatter::getXValues()
{
  return m_xValues;
}

double te::qt::widgets::Scatter::getX(unsigned int idx)
{
  return m_xValues[idx];
}

double* te::qt::widgets::Scatter::getX()
{
  return &m_xValues[0];
}

std::vector<double> te::qt::widgets::Scatter::getYValues()
{
  return m_yValues;
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

void te::qt::widgets::Scatter::setXValues(std::vector<double> xValues)
{
  m_xValues = xValues;
}

void te::qt::widgets::Scatter::setMinX(double& new_minX)
{
   m_minX = new_minX;
}

void te::qt::widgets::Scatter::setMaxX(double& new_maxX)
{
  m_maxX = new_maxX;
}

void te::qt::widgets::Scatter::setYValues(std::vector<double> yValues)
{
  m_yValues = yValues;
}

void te::qt::widgets::Scatter::setMinY(double& new_minY)
{
  m_minY = new_minY;
}

void te::qt::widgets::Scatter::setMaxY(double& new_maxY)
{
  m_maxY = new_maxY;
}

void te::qt::widgets::Scatter::addX(double& xValue)
{
  m_xValues.push_back(xValue);
}

void te::qt::widgets::Scatter::addY(double& yValue)
{
  m_yValues.push_back(yValue);
}

void te::qt::widgets::Scatter::addData(double& xValue, double& yValue, te::da::ObjectId* oid)
{
  m_xValues.push_back(xValue);
  m_yValues.push_back(yValue);
  m_valuesOids.insert(te::qt::widgets::PointToObjectId(xValue, yValue, oid));
}

te::da::ObjectIdSet* te::qt::widgets::Scatter::find(double& xValue, double& yValue)
{
  typedef te::qt::widgets::PointToObjectIdSet::nth_index<0>::type::iterator itPointToObjectIdSet;
  PointToObjectId aux(xValue, yValue, 0);

  std::pair<itPointToObjectIdSet, itPointToObjectIdSet> res = m_valuesOids.equal_range(aux);
  itPointToObjectIdSet it0 = res.first; //begin
  itPointToObjectIdSet it1 = res.second; //end

  te::da::ObjectIdSet* oids = new te::da::ObjectIdSet;

  while(it0 != it1) 
  {
    if(it0->y == yValue)
    {
      te::da::ObjectId* oid = new te::da::ObjectId(); 

      for(boost::ptr_vector<te::dt::AbstractData>::const_iterator it = it0->oid->getValue().begin(); it != it0->oid->getValue().end(); ++it)
      {
        oid->addValue((it)->clone());
      }

      oids->add(oid);
    }

    ++it0;
  }
  return oids;
}

te::da::ObjectIdSet* te::qt::widgets::Scatter::find(std::vector<QPointF> selectedPoints)
{
  typedef te::qt::widgets::PointToObjectIdSet::nth_index<0>::type::iterator itPointToObjectIdSet;
  itPointToObjectIdSet it0, it1;
  std::pair<itPointToObjectIdSet, itPointToObjectIdSet> res;
  te::da::ObjectIdSet* oids = new te::da::ObjectIdSet;

  //Checking all the selected points:
  for(size_t i = 0; i < selectedPoints.size(); ++i)
  {
    double x = selectedPoints.at(i).x();
    double y = selectedPoints.at(i).y();
    PointToObjectId aux(x, y, 0);
    res = m_valuesOids.equal_range(aux);
    it0 = res.first;
    it1 = res.second;

    while(it0 != it1) 
    {
      if(it0->y == y)
      {
        te::da::ObjectId* oid = new te::da::ObjectId(); 

        for(boost::ptr_vector<te::dt::AbstractData>::const_iterator it = it0->oid->getValue().begin(); it != it0->oid->getValue().end(); ++it)
        {
          oid->addValue((it)->clone());
        }

        oids->add(oid);
      }

      ++it0;
    }
  }

  return oids;
}

const std::pair<double, double> te::qt::widgets::Scatter::find(const te::da::ObjectId* oid)
{
  te::qt::widgets::PointToObjectIdSet::nth_index<1>::type::iterator it= m_valuesOids.get<1>().find(oid->getValueAsString());
  return std::make_pair(it->x, it->y);
}
