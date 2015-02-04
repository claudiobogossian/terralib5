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
  \file HistogramChart.cpp

  \brief A class to represent a histogram chart.
*/

//Terralib
#include "../../../dataaccess/dataset/ObjectId.h"
#include "../../../dataaccess/dataset/ObjectIdSet.h"
#include "../../../dataaccess/utils/Utils.h"
#include "../../../qt/widgets/se/Utils.h"
#include "../../../se/Stroke.h"
#include "Enums.h"
#include "Histogram.h"
#include "HistogramChart.h"
#include "HistogramStyle.h"
#include "StringScaleDraw.h"
#include "Utils.h"


//QT
#include <qbrush.h>
#include <qpen.h>

//QWT
#include <qwt_column_symbol.h>
#include <qwt_plot.h>

//STL
#include <limits>

te::qt::widgets::HistogramChart::HistogramChart(Histogram* histogram, te::qt::widgets::HistogramStyle* style) :
  QwtPlotHistogram(),
  m_histogram(histogram),
  m_histogramStyle(style)
{
  if(!m_histogramStyle)
  {
    m_histogramStyle = new te::qt::widgets::HistogramStyle();
    QPen barPen;
    QBrush barBrush;

    te::qt::widgets::Config(barPen, m_histogramStyle->getStroke());
    te::qt::widgets::Config(barBrush, m_histogramStyle->getFill());
    barBrush.setStyle(Qt::SolidPattern);

    setPen(barPen);
    setBrush(barBrush);
  }
  
  setData();
  
  m_selection = new QwtPlotHistogram();
  m_selection->setStyle(QwtPlotHistogram::Columns);

  //The default selection color is green
  m_selection->setBrush(QColor("#00FF00"));
  m_selection->attach(plot());

}

void te::qt::widgets::HistogramChart::setData()
{
  //Vector that will be populated by the histogram's data
  QVector<QwtIntervalSample> samples;
  if((m_histogram->getType() >= te::dt::INT16_TYPE && m_histogram->getType() <= te::dt::UINT64_TYPE) || 
    m_histogram->getType() == te::dt::FLOAT_TYPE || m_histogram->getType() == te::dt::DOUBLE_TYPE || 
    m_histogram->getType() == te::dt::NUMERIC_TYPE)
  {
    std::map<double, unsigned int> values;
    values = m_histogram->getValues();

    std::map<double,  unsigned int>::const_iterator it;
    it = values.begin();

    std::map<int,  unsigned int> vmap;

    int i = 0;

    double ini = m_histogram->getMinValue();

    double vx = ini + m_histogram->getInterval();

    while(vx <= values.rbegin()->first)
    {
      vmap[i] = 0;
      if(fabs(vx) < 0.000000000001)
        vx = 0.;
      while(it != values.end())
      {
        if(it->first >= ini && it->first < vx)
          vmap[i] += it->second;
        else
          break;
        ++it;
      }

      QwtInterval qinterval(ini, vx);
      qinterval.setBorderFlags(QwtInterval::ExcludeMaximum);  
      samples.push_back(QwtIntervalSample(vmap[i], qinterval));

      ini = vx;
      vx += m_histogram->getInterval();
      ++i;
    }
    setSamples(samples);
  }

  else if (m_histogram->getType() == te::dt::DATETIME_TYPE || m_histogram->getType() == te::dt::STRING_TYPE)
  {
    std::map<std::string, unsigned int> values;
    values = m_histogram->getStringValues();

    std::map<std::string,  unsigned int>::iterator it;
    it  = values.begin();

    m_histogramScaleDraw = new StringScaleDraw(m_histogram->getStringInterval());
    QVector<QwtIntervalSample> samples(values.size());
    double LabelInterval = 0.0;

    while (it != values.end())
    {
      QwtInterval qwtInterval(LabelInterval, LabelInterval+1);
      qwtInterval.setBorderFlags(QwtInterval::ExcludeMaximum);
      samples[LabelInterval] = QwtIntervalSample(it->second, qwtInterval);
      LabelInterval++;
       it++;
    }
    setSamples(samples);
  }
  else
  {
    std::map<double,  unsigned int> values;
    std::map<double,  unsigned int>::const_iterator it;
    values = m_histogram->getValues();
    it = values.begin();

    while (it != values.end())
    {
      double interval = it->first;
      QwtInterval qwtInterval(interval, interval+1);
      samples.push_back(QwtIntervalSample(it->second, qwtInterval));
      it++;
    }

    QPen blankPen = QPen(Qt::transparent);
    setPen(blankPen);

    te::se::Stroke* blankStroke = new te::se::Stroke();
    blankStroke->setOpacity(QString::number(0, 'g', 2).toStdString());
    m_histogramStyle->setStroke(blankStroke);
    setSamples(samples);
  }
}

te::qt::widgets::HistogramChart::~HistogramChart()
{  
  delete m_histogram;
  delete m_histogramStyle;
  delete m_selection;
  if(m_histogram->getType() == te::dt::DATETIME_TYPE || m_histogram->getType() == te::dt::STRING_TYPE)
    delete m_histogramScaleDraw;
}

int  te::qt::widgets::HistogramChart::rtti() const
{
  return te::qt::widgets::HISTOGRAM_CHART;
}

te::qt::widgets::StringScaleDraw* te::qt::widgets::HistogramChart::getScaleDraw()
{
  return m_histogramScaleDraw;
}

void te::qt::widgets::HistogramChart::setScaleDraw( StringScaleDraw* newScaleDraw)
{
  delete m_histogramScaleDraw;
  m_histogramScaleDraw = newScaleDraw;
}

void te::qt::widgets::HistogramChart::attach(QwtPlot* plot)
{
  if (m_histogram->getType() == te::dt::DATETIME_TYPE || m_histogram->getType() == te::dt::STRING_TYPE)
  {
    plot->setAxisScaleDraw(QwtPlot::xBottom, m_histogramScaleDraw);
    plot->axisScaleDraw(QwtPlot::xBottom)->setLabelAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    plot->axisScaleDraw(QwtPlot::xBottom)->setLabelRotation(-60);
  }

  QwtPlotHistogram::attach(plot);
}

te::qt::widgets::Histogram* te::qt::widgets::HistogramChart::getHistogram()
{
 return m_histogram;
}

void te::qt::widgets::HistogramChart::setHistogram(te::qt::widgets::Histogram* newHistogram)
{
  delete m_histogram;
  m_histogram = newHistogram;
}

te::qt::widgets::HistogramStyle* te::qt::widgets::HistogramChart::getHistogramStyle()
{
 return m_histogramStyle->clone();
}

void te::qt::widgets::HistogramChart::setHistogramStyle(te::qt::widgets::HistogramStyle* newStyle)
{
  delete m_histogramStyle;
  m_histogramStyle = newStyle;

  //The pen and brush that will be used
  QPen barPen;
  QBrush barBrush;

  //Configuring the pen and brush based on the current style
  te::qt::widgets::Config(barPen, m_histogramStyle->getStroke());
  te::qt::widgets::Config(barBrush, m_histogramStyle->getFill());
  barBrush.setStyle(Qt::SolidPattern);

  //Updating the chart's pen and brush.
  setPen(barPen);
  setBrush(barBrush);
}

void te::qt::widgets::HistogramChart::highlight(const te::da::ObjectIdSet* oids, te::da::DataSetType* dataType)
{
  //Removing the previous selection, if there was any.
  m_selection->detach();

  std::set<te::da::ObjectId*, te::common::LessCmp<te::da::ObjectId*> >::const_iterator itObjSet; 
  QwtSeriesData<QwtIntervalSample>* values = data();

  //Acquiring all selected intervals:

  if((m_histogram->getType() >= te::dt::INT16_TYPE && m_histogram->getType() <= te::dt::UINT64_TYPE) || 
     m_histogram->getType() == te::dt::FLOAT_TYPE || m_histogram->getType() == te::dt::DOUBLE_TYPE || 
     m_histogram->getType() == te::dt::NUMERIC_TYPE)
  {
    std::map<double, unsigned int> highlightedIntervals;
    std::set<std::string> highlightedPkeys;

    //Acquiring the slected intervals
    for(itObjSet = oids->begin(); itObjSet != oids->end(); ++itObjSet)
    {
      const te::dt::Double* data = static_cast< const te::dt::Double*>(m_histogram->find((*itObjSet)));
      if(data)
      {
        double interval = data->getValue();
        highlightedIntervals.insert(std::make_pair(interval, 0));
      }
    }

    //Acquiring the name of the base dataset and how many of it's properties are included in it's primary key
    std::pair<std::string, int> dsProps;
    te::da::GetOIDDatasetProps(dataType, dsProps);

    //Acquiring the selected values' frequency
    for(itObjSet = oids->begin(); itObjSet != oids->end(); ++itObjSet)
    {
      //Acquiring the value of the base primaryKey on the current objectId;
      std::string pKey = te::da::getBasePkey((*itObjSet),dsProps);
      if(m_histogram->isSummarized())
      {
        if(highlightedPkeys.insert(pKey).second)
        {
          const te::dt::Double* data = static_cast< const te::dt::Double*>(m_histogram->find((*itObjSet)));
          if(data)
          {
            double interval = data->getValue();
            ++highlightedIntervals.at(interval);
          }
        }
      }
      else
      {
        const te::dt::Double* data = static_cast< const te::dt::Double*>(m_histogram->find((*itObjSet)));
        if(data)
        {
          double interval = data->getValue();
          ++highlightedIntervals.at(interval);
        }
      }
    }

    QVector<QwtIntervalSample> highlightedSamples(highlightedIntervals.size());

    //Acquiring all selected samples:
    for(size_t i = 0; i < values->size(); ++i)
    {
      for (std::map<double, unsigned int>::iterator it = highlightedIntervals.begin(); it !=  highlightedIntervals.end(); ++it)
      {
        //Comparing with the minimum value. Our histogram is created based on the exclude maximum policy.
        if(values->sample(i).interval.minValue() == it->first)
        {
          QwtInterval qwtInterval(values->sample(i).interval.minValue(), values->sample(i).interval.maxValue());
          highlightedSamples.push_back(QwtIntervalSample(it->second, qwtInterval));
        }
      }
    }
    m_selection->setData(new QwtIntervalSeriesData(highlightedSamples));
  }

  else if (m_histogram->getType() == te::dt::DATETIME_TYPE || m_histogram->getType() == te::dt::STRING_TYPE)
  {

    //A vector that will contain the selected strings
    std::map<std::string, unsigned int> highlightedIntervals;

    //Acquiring the slected intervals
    for(itObjSet = oids->begin(); itObjSet != oids->end(); ++itObjSet)
    {
      std::string interval = m_histogram->find((*itObjSet))->toString();
      highlightedIntervals.insert(std::make_pair(interval,0));
    }

    //Acquiring the selected values' frequency
    for(itObjSet = oids->begin(); itObjSet != oids->end(); ++itObjSet)
    {
      std::string interval = m_histogram->find((*itObjSet))->toString();
      if(m_histogram->getStringValues().at(interval) > highlightedIntervals.at(interval))
        ++highlightedIntervals.at(interval);
    }

    //A vector containing that will be populated with the samples that match the selected strings
    QVector<QwtIntervalSample> highlightedSamples(highlightedIntervals.size());

    //Acquiring all selected samples:
    for(size_t i = 0; i < values->size(); ++i)
    {
      for (std::map<std::string, unsigned int>::iterator it = highlightedIntervals.begin();  it !=  highlightedIntervals.end(); ++it)
      {
        //Comparing label by label.
        if(m_histogramScaleDraw->label(i).text().toStdString() == it->first)
        {
          QwtInterval qwtInterval(values->sample(i).interval.minValue(), values->sample(i).interval.maxValue());
          highlightedSamples.push_back(QwtIntervalSample(it->second, qwtInterval));
        }
      }
    }
    m_selection->setData(new QwtIntervalSeriesData(highlightedSamples));
  }

  m_selection->attach(plot());
  plot()->replot();
}
 
te::da::ObjectIdSet* te::qt::widgets::HistogramChart::highlight(QPointF point)
{
  QwtSeriesData<QwtIntervalSample>* values = data();

  if (m_histogram->getType() == te::dt::DATETIME_TYPE || m_histogram->getType() == te::dt::STRING_TYPE)
  {
    std::auto_ptr<te::dt::String> data(new te::dt::String(""));
    for(size_t i = 0; i < values->size(); ++i)
    {
      if(values->sample(i).interval.minValue() < point.rx() && values->sample(i).interval.maxValue() > point.rx() &&  values->sample(i).value > point.ry())
        data.reset(new te::dt::String(m_histogramScaleDraw->label(i).text().toStdString()));
    }

    return m_histogram->find(data.get());
  }
  else
  {
    std::auto_ptr<te::dt::Double> data(new te::dt::Double(std::numeric_limits<double>::max()));

    for(size_t i = 0; i < values->size(); ++i)
    {
      if(values->sample(i).interval.minValue() < point.rx() && values->sample(i).interval.maxValue() > point.rx() &&  values->sample(i).value > point.ry())
        data.reset(new te::dt::Double(values->sample(i).interval.minValue()));
    }

    return m_histogram->find(data.get());
  }
}

te::da::ObjectIdSet* te::qt::widgets::HistogramChart::highlight(QRectF rect)
{
  QwtSeriesData<QwtIntervalSample>* values = data();
  std::vector<te::dt::AbstractData*> selected;

  if (m_histogram->getType() == te::dt::DATETIME_TYPE || m_histogram->getType() == te::dt::STRING_TYPE)
  {
    for(size_t i = 0; i < values->size(); ++i)
    {
      int min = values->sample(i).interval.minValue();
      int max = values->sample(i).interval.maxValue();
      int frequency = values->sample(i).value;

      //Checking if the interval is within the rectangle, works when a rectangle is drawn around the intervals
      if(min > rect.x() && (max < rect.x() + rect.width() || min < rect.x() + rect.width())  &&  (frequency > rect.y() && rect.y() + rect.height() > 0))
        selected.push_back(new te::dt::String(m_histogramScaleDraw->label(i).text().toStdString()));

      //Checking if the rectangle is within the interval, works when the user simply clicked on an interval
      else if(min < rect.x() && max > rect.x() &&  (frequency > rect.y() && rect.y() + rect.height() > 0))
        selected.push_back(new te::dt::String(m_histogramScaleDraw->label(i).text().toStdString()));
    }
  }

  else
  {
    for(size_t i = 0; i < values->size(); ++i)
    {

      int min = values->sample(i).interval.minValue();
      int max = values->sample(i).interval.maxValue();
      int frequency = values->sample(i).value;

      //Checking if the interval is within the rectangle, works when a rectangle is drawn around the intervals
      if(min > rect.x() && (max < rect.x() + rect.width() || min < rect.x() + rect.width()) &&  (frequency > rect.y() && rect.y() + rect.height() > 0))
        selected.push_back(new te::dt::Double(values->sample(i).interval.minValue()));

      //Checking if the rectangle is within the interval, works when the user simply clicked on an interval
      else if(min < rect.x() && max > rect.x() &&  (frequency > rect.y() && rect.y() + rect.height() > 0))
        selected.push_back(new te::dt::Double(values->sample(i).interval.minValue()));
    }
  }

  return m_histogram->find(selected);
}

void te::qt::widgets::HistogramChart::setSelectionColor(QColor selColor)
{
  m_selection->setBrush(selColor);
}