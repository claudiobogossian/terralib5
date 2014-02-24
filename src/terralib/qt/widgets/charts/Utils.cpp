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
#include "../../../dataaccess/dataset/ObjectId.h"
#include "../../../dataaccess/dataset/ObjectIdSet.h"
#include "../../../dataaccess/utils/Utils.h"
#include "../../../datatype.h"
#include "../../../raster.h"
#include "../../../maptools/ExternalGraphicRendererManager.h"
#include "../../../maptools/MarkRendererManager.h"
#include "../../../qt/widgets/Utils.h"
#include "../../../se/Utils.h"
#include "../../../se.h"
#include "Histogram.h"
#include "Scatter.h"
#include "Utils.h"

//Boost
#include <boost/lexical_cast.hpp>

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

void getObjectIds (te::da::DataSet* dataset, std::vector<std::size_t> pkeys, std::vector<te::da::ObjectId*>& valuesOIDs)
{
  std::vector<size_t>::iterator it;
  std::vector<std::string> propNames;

  for(it=pkeys.begin(); it!=pkeys.end(); ++it)
    propNames.push_back(dataset->getPropertyName(*it));

  //The caller will take ownership of the generated pointers.
  te::da::ObjectId* oid = te::da::GenerateOID(dataset, propNames); 
  valuesOIDs.push_back(oid);
}

te::da::ObjectId* getObjectIds (te::da::DataSet* dataset, std::vector<std::size_t> pkeys)
{
  std::vector<size_t>::iterator it;
  std::vector<std::string> propNames;

  for(it=pkeys.begin(); it!=pkeys.end(); ++it)
    propNames.push_back(dataset->getPropertyName(*it));

  //The caller will take ownership of the generated pointer.
  te::da::ObjectId* oid = te::da::GenerateOID(dataset, propNames); 
  return oid;
}

te::qt::widgets::Scatter* te::qt::widgets::createScatter(te::da::DataSet* dataset, te::da::DataSetType* dataType, int propX, int propY)
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
      newScatter->addData(x_doubleValue, y_doubleValue, getObjectIds(dataset, objIdIdx));
      task.pulse();
    } //end of the data set
  }
  newScatter->calculateMinMaxValues();
  return newScatter;
}

te::qt::widgets::Histogram* te::qt::widgets::createHistogram(te::da::DataSet* dataset, te::da::DataSetType* dataType, int propId, int slices)
{

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
  }
  else
  {

    int propType = dataset->getPropertyDataType(propId);
    newHistogram->setType(propType);

    //The vector containing the frequency of each interval, will be used to every property type
    std::vector< unsigned int> values;

     if((propType >= te::dt::INT16_TYPE && propType <= te::dt::UINT64_TYPE) || 
       propType == te::dt::FLOAT_TYPE || propType == te::dt::DOUBLE_TYPE || propType == te::dt::NUMERIC_TYPE)
     {

       std::map<double, std::vector<te::da::ObjectId*> > intervalToOIds;
       std::vector<te::da::ObjectId*> valuesOIds;

       double interval, minValue, maxValue;
       minValue = std::numeric_limits<double>::max();
       maxValue = -std::numeric_limits<double>::max();

       std::vector<double> intervals;

      te::common::TaskProgress task;
      task.setMessage("Histogram creation");
      task.setTotalSteps((dataset->getNumProperties()) * 2);

       //Calculating the minimum and maximum values of the given property and adjusting the Histogram's interval.
       while(dataset->moveNext())
       {

          if(!task.isActive())
          {
            break;
          }

         //calculate range
         if(minValue > getDouble(dataset, propId))
            minValue = getDouble(dataset, propId);
         if(maxValue < getDouble(dataset, propId))
            maxValue = getDouble(dataset, propId);

         task.pulse();
       }

        //Adjusting the interval to the user-defined number of slices.
        interval = maxValue - minValue;
        newHistogram->setInterval(interval/slices);

       //Adjusting the histogram's intervals
       for (double i = minValue; i <(maxValue+newHistogram->getInterval()); i+=newHistogram->getInterval())
       {
         intervals.push_back(i);
         intervalToOIds.insert(std::make_pair(i, valuesOIds));
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

         double currentValue = getDouble(dataset, propId);

         for (unsigned int i= 0; i<intervals.size(); ++i)
         {
           if((currentValue >= intervals[i]) && (currentValue <= intervals[i+1]))
           {
              values[i] =  values[i]+1;
              getObjectIds(dataset, objIdIdx, intervalToOIds.at(intervals[i]));
              break;
           }

         }
        task.pulse();
        }

       //With both the intervals and values ready, the map can be populated
       for (unsigned int i= 0; i<intervals.size(); ++i)
       {
         te::dt::Double* data = new te::dt::Double(intervals[i]);
         newHistogram->insert(std::make_pair(data, values[i]), intervalToOIds.at(intervals[i]));
       }

       newHistogram->setMinValue(minValue);
       dataset->moveBeforeFirst();

     }
  }

   return newHistogram;
}

te::qt::widgets::Histogram* te::qt::widgets::createHistogram(te::da::DataSet* dataset, te::da::DataSetType* dataType, int propId)
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
  }
  else
  {
    int propType = dataset->getPropertyDataType(propId);

    newHistogram->setType(propType);

    //The vector containing the frequency of each interval, will be used to every property type
    std::vector< unsigned int> values;

    if(propType == te::dt::DATETIME_TYPE || propType == te::dt::STRING_TYPE)
    {

      std::set <std::string> intervals;
      std::set <std::string>::iterator intervalsIt;
      std::map<std::string, std::vector<te::da::ObjectId*> > valuesIdsByinterval;
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
        task.pulse();
      }

        for (intervalsIt = intervals.begin(); intervalsIt != intervals.end(); ++intervalsIt)
        {
          valuesIdsByinterval.insert(make_pair((*intervalsIt), valuesOIds));
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
            getObjectIds(dataset, objIdIdx, valuesIdsByinterval.at(*intervalsIt));
            break;
          }
        }
      task.pulse();
      }

      //With both the intervals and values ready, the map can be populated
      int i;
      for (i= 0, intervalsIt = intervals.begin(); intervalsIt != intervals.end();  ++intervalsIt,++i)
      {
        te::dt::String* data = new te::dt::String(*intervalsIt);
        newHistogram->insert(std::make_pair(data, values[i]), valuesIdsByinterval.at(*intervalsIt));
      }

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