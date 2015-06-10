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
  \file IDWInterpolator.cpp

  \brief  This file contains an inverse distance weighted (IDW) 
          interpolationan function. 
*/

// TerraLib
#include "../../../datatype/AbstractData.h"
#include "../../../datatype/SimpleData.h"
#include "../../../geometry/Point.h"

// ST
#include "IDWInterpolator.h"
#include "../coverage/PointCoverage.h"

te::st::IDWInterpolator::IDWInterpolator()
{
}

te::st::IDWInterpolator::~IDWInterpolator()
{
}                         

std::auto_ptr<te::dt::AbstractData> te::st::IDWInterpolator::estimate(const te::st::PointCoverage& cv, unsigned int p, const te::gm::Point& l)
{
  std::vector<double> di; //inverso da distancia ao quadrado ao ponto i: 1/(distance)2
  std::vector<double> vi; //value associated to point i
  double sumdi = 0.;      //soma do inverso das distancias ao quadrado
  
  PointCoverageIterator it = cv.begin();
  while(it!=cv.end())
  {
    te::gm::Point& point = it.getLocation();
    double distance = point.distance(&l);
    double d = 0.;
    if(distance!=0.)
      d = 1/(distance * distance); // 1/(distance)2
    di.push_back(d); 
    sumdi += d;

    vi.push_back(it.getDouble(p));

    ++it;
  }
  
  double result = 0.;
  for(unsigned int i = 0; i< di.size(); ++i)
  {
    double wi = di[i]/sumdi;
    result += vi[i]*wi;    
  }

  return std::auto_ptr<te::dt::AbstractData>(new te::dt::Double(result));
}


