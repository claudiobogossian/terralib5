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
  \file terralib/rp/Filter.cpp
Implements a series of well-known filtering algorithms for images, linear and non-linear.
*/

// TerraLib
#include "../common/progress/TaskProgress.h"
#include "../raster/Band.h"
#include "../raster/BandIterator.h"
#include "../raster/BandProperty.h"
#include "../raster/Grid.h"
#include "../raster/Raster.h"
#include "../raster/BandIteratorWindow.h"
#include "../raster/RasterFactory.h"
#include "../raster/RasterProperty.h"
#include "../raster/RasterSummaryManager.h"
#include "../statistics/core/SummaryFunctions.h"
#include "Filter.h"
#include "Functions.h"
#include "Macros.h"

// STL
#include "float.h"

// Boost
#include <boost/numeric/ublas/io.hpp>
#include <boost/numeric/ublas/matrix.hpp>

te::rp::Filter::InputParameters::InputParameters()
{
  reset();
}

te::rp::Filter::InputParameters::~InputParameters()
{
  reset();
}

void te::rp::Filter::InputParameters::reset() throw(te::rp::Exception)
{
  m_type = InputParameters::InvalidFilterT;
  m_windowH = 3;
  m_windowW = 3;
  m_inRasterPtr = 0;
  m_inRasterBands.clear();
  m_enableProgress = false;
}

const te::rp::Filter::InputParameters& te::rp::Filter::InputParameters::operator=(const te::rp::Filter::InputParameters& params)
{
  reset();

  m_type = params.m_type;
  m_windowH = params.m_windowH;
  m_windowW = params.m_windowW;
  m_inRasterPtr = params.m_inRasterPtr;
  m_inRasterBands = params.m_inRasterBands;
  m_enableProgress = params.m_enableProgress;

  return *this;
}

te::common::AbstractParameters* te::rp::Filter::InputParameters::clone() const
{
  return new InputParameters(*this);
}

te::rp::Filter::OutputParameters::OutputParameters()
{
  reset();
}

te::rp::Filter::OutputParameters::OutputParameters(const OutputParameters& other)
{
  reset();

  operator=(other);
}

te::rp::Filter::OutputParameters::~OutputParameters()
{
  reset();
}

void te::rp::Filter::OutputParameters::reset() throw(te::rp::Exception)
{
  m_outRasterPtr = 0;
  m_createdOutRasterPtr.reset();
  m_outRasterBands.clear();
  m_createdOutRasterDSType.clear();
  m_createdOutRasterInfo.clear();
  m_normalizeOutput = false;
}

const te::rp::Filter::OutputParameters& te::rp::Filter::OutputParameters::operator=(const te::rp::Filter::OutputParameters& params)
{
  reset();

  m_outRasterPtr = params.m_outRasterPtr;
  m_outRasterBands = params.m_outRasterBands;
  m_createdOutRasterDSType = params.m_createdOutRasterDSType;
  m_createdOutRasterInfo = params.m_createdOutRasterInfo;
  m_normalizeOutput = params.m_normalizeOutput;

  return *this;
}

te::common::AbstractParameters* te::rp::Filter::OutputParameters::clone() const
{
  return new OutputParameters(*this);
}

te::rp::Filter::Filter()
{
  reset();
}

te::rp::Filter::~Filter()
{
}

bool te::rp::Filter::execute(AlgorithmOutputParameters& outputParams) throw(te::rp::Exception)
{
  TERP_TRUE_OR_RETURN_FALSE(m_isInitialized, "Algoritm not initialized");

  // Initializing the output raster
  m_outputParametersPtr = dynamic_cast<te::rp::Filter::OutputParameters* >(&outputParams);
  TERP_TRUE_OR_RETURN_FALSE(m_outputParametersPtr, "Invalid parameters");

  if(m_outputParametersPtr->m_outRasterPtr == 0)
  {
    m_outputParametersPtr->m_outRasterBands.clear();

    std::vector<te::rst::BandProperty*> bandsProperties;
    for(unsigned int i = 0 ; i < m_inputParameters.m_inRasterBands.size() ; i++)
    {
      assert(m_inputParameters.m_inRasterBands[i] < m_inputParameters.m_inRasterPtr->getNumberOfBands());

      bandsProperties.push_back(new te::rst::BandProperty(*(
        m_inputParameters.m_inRasterPtr->getBand(
          m_inputParameters.m_inRasterBands[i])->getProperty())));

      m_outputParametersPtr->m_outRasterBands.push_back(i);
    }

    m_outputParametersPtr->m_createdOutRasterPtr.reset(
      te::rst::RasterFactory::make(
        m_outputParametersPtr->m_createdOutRasterDSType,
        new te::rst::Grid(*(m_inputParameters.m_inRasterPtr->getGrid())),
        bandsProperties,
        m_outputParametersPtr->m_createdOutRasterInfo,
        0,
        0));
    TERP_TRUE_OR_RETURN_FALSE(m_outputParametersPtr->m_createdOutRasterPtr.get(), "Output raster creation error");

    m_outputParametersPtr->m_outRasterPtr = m_outputParametersPtr->m_createdOutRasterPtr.get();
  }
  else
  {
    if ((m_outputParametersPtr->m_outRasterPtr->getAccessPolicy() & te::common::WAccess)
      && (m_outputParametersPtr->m_outRasterPtr->getNumberOfColumns() == m_inputParameters.m_inRasterPtr->getNumberOfColumns())
      && (m_outputParametersPtr->m_outRasterPtr->getNumberOfRows() == m_inputParameters.m_inRasterPtr->getNumberOfRows()))
    {
      for(unsigned int i = 0 ; i < m_inputParameters.m_inRasterBands.size() ; i++)
      {
        TERP_TRUE_OR_RETURN_FALSE(m_outputParametersPtr->m_outRasterBands[i] < m_outputParametersPtr->m_outRasterPtr->getNumberOfBands(),
          "Invalid output raster band")
      }
    }
    else
    {
      TERP_LOG_AND_RETURN_FALSE("Invalid output raster");
    }
  }

  // Executing the filter on the selected bands
  switch(m_inputParameters.m_type)
  {
    case InputParameters::SobelFilterT:
    {
      return execSobelFilter();
      break;
    }
    case InputParameters::MeanFilterT:
    {
      return execMeanFilter();
      break;
    }
    case InputParameters::ModeFilterT:
    {
      return execModeFilter();
      break;
    }
    case InputParameters::MedianFilterT:
    {
      return execMedianFilter();
      break;
    }
    case InputParameters::DilationFilterT:
    {
      return execDilationFilter();
      break;
    }
    case InputParameters::ErosionFilterT:
    {
      return execErosionFilter();
      break;
    }
    default :
    {
      TERP_LOG_AND_THROW("Invalid filter type");
      break;
    }
  }
}

void te::rp::Filter::reset() throw(te::rp::Exception)
{
  m_inputParameters.reset();
  m_outputParametersPtr = 0;
  m_isInitialized = false;
}

bool te::rp::Filter::initialize(const AlgorithmInputParameters& inputParams) throw(te::rp::Exception)
{
  reset();

  Filter::InputParameters const* inputParamsPtr = dynamic_cast<Filter::InputParameters const*> (&inputParams);
  TERP_TRUE_OR_RETURN_FALSE(inputParamsPtr, "Invalid parameters");

  TERP_TRUE_OR_RETURN_FALSE(inputParamsPtr->m_inRasterPtr, "Invalid raster pointer");
  TERP_TRUE_OR_RETURN_FALSE(inputParamsPtr->m_inRasterPtr->getAccessPolicy() & te::common::RAccess, "Invalid raster");

  for(unsigned int i = 0 ; i < inputParamsPtr->m_inRasterBands.size(); i++)
  {
    TERP_TRUE_OR_RETURN_FALSE(inputParamsPtr->m_inRasterBands[i] < inputParamsPtr->m_inRasterPtr->getNumberOfBands(), 
                              "Invalid input raster band");
  }

// checking window size
  if (inputParamsPtr->m_type == InputParameters::SobelFilterT)
    TERP_TRUE_OR_RETURN_FALSE(inputParamsPtr->m_windowH == 3 || inputParamsPtr->m_windowW ==3, 
                              "Invalid window size for Sobel Filter, correct is 3x3.");

  TERP_TRUE_OR_RETURN_FALSE(inputParamsPtr->m_windowH > 0 && inputParamsPtr->m_windowH < inputParamsPtr->m_inRasterPtr->getNumberOfRows() / 2,
                            "Window height must be greater than 0, and less than half of the number of rows of the image");
  TERP_TRUE_OR_RETURN_FALSE(inputParamsPtr->m_windowW > 0 && inputParamsPtr->m_windowW < inputParamsPtr->m_inRasterPtr->getNumberOfColumns() / 2,
                            "Window width must be greater than 0, and less than half of the number of columns of the image");

  m_inputParameters = *inputParamsPtr;
  m_isInitialized = true;

  return true;
}

bool te::rp::Filter::isInitialized() const
{
  return m_isInitialized;
}

bool te::rp::Filter::execSobelFilter()
{
  assert(m_inputParameters.m_inRasterPtr);
  assert(m_outputParametersPtr->m_outRasterPtr);
  
  te::common::TaskProgress task(TR_RP("Sobel Filter"),
                                te::common::TaskProgress::UNDEFINED,
                                m_inputParameters.m_inRasterBands.size());

  const int H = m_inputParameters.m_windowH;
  const int W = m_inputParameters.m_windowW;

  // create weight matrices
  boost::numeric::ublas::matrix<double> window_X(W, H);
  boost::numeric::ublas::matrix<double> window_Y(W, H);
  for (unsigned int i = 0; i < W; i++)
    for (unsigned int j = 0; j < H; j++)
    {
      window_X(i, j) = 0.0;
      window_Y(i, j) = 0.0;
    }
  window_X(0, 0) = 1; window_Y(0, 0) = -1;
  window_X(0, 1) = 2; window_Y(1, 0) = -2;
  window_X(0, 2) = 1; window_Y(2, 0) = -1;
  window_X(2, 0) = -1; window_Y(0, 2) = 1;
  window_X(2, 1) = -2; window_Y(1, 2) = 2;
  window_X(2, 2) = -1; window_Y(2, 2) = 1;

  double pixels_X;
  double pixels_Y;
  double pixel_sobel;

  unsigned int R;
  unsigned int C;
  const unsigned int MR = m_inputParameters.m_inRasterPtr->getNumberOfRows();
  const unsigned int MC = m_inputParameters.m_inRasterPtr->getNumberOfColumns();
  
  for (unsigned int b = 0; b < m_inputParameters.m_inRasterBands.size(); b++)
  {
    unsigned int nband = m_inputParameters.m_inRasterBands[b];
    te::rst::Band* band = m_inputParameters.m_inRasterPtr->getBand(nband);
    te::rst::BandIteratorWindow<unsigned char> it = te::rst::BandIteratorWindow<unsigned char>::begin(band, W, H);
    te::rst::BandIteratorWindow<unsigned char> itend = te::rst::BandIteratorWindow<unsigned char>::end(band, W, H);

    while (it != itend)
    {
      R = it.getRow();
      C = it.getColumn();
      if ((R >= (unsigned)(H / 2) && R < MR - (H / 2)) &&
          (C >= (unsigned)(W / 2) && C < MC - (W / 2)))
      {
        pixels_X = 0.0;
        pixels_Y = 0.0;
        for (int r = -1, rw = 0; r <= 1; r++, rw++)
          for (int c = -1, cw = 0; c <= 1; c++, cw++)
          {
            pixels_X += window_X(rw, cw) * it.getValue(c, r);
            pixels_Y += window_Y(rw, cw) * it.getValue(c, r);
          }
        pixel_sobel = sqrt(pixels_X * pixels_X + pixels_Y * pixels_Y);
      }
      else
        pixel_sobel = 0.0;

      m_outputParametersPtr->m_outRasterPtr->setValue(C, R, pixel_sobel, b);
      ++it;
    }
    task.pulse();
  }
  if (m_outputParametersPtr->m_normalizeOutput)
    NormalizeRaster(*m_outputParametersPtr->m_outRasterPtr);

  return true;
}

bool te::rp::Filter::execMeanFilter()
{
  assert(m_inputParameters.m_inRasterPtr);
  assert(m_outputParametersPtr->m_outRasterPtr);
  
  te::common::TaskProgress task(TR_RP("Mean Filter"),
                                te::common::TaskProgress::UNDEFINED,
                                m_inputParameters.m_inRasterBands.size());

  int H = m_inputParameters.m_windowH;
  int W = m_inputParameters.m_windowW;

  boost::numeric::ublas::matrix<double> window_mean(H, W);
  double pixel_mean;
  for (unsigned int i = 0; i < H; i++)
    for (unsigned int j = 0; j < W; j++)
      window_mean(i, j) = 1.0 / (W * H);

  unsigned int R;
  unsigned int C;
  const unsigned int MR = m_inputParameters.m_inRasterPtr->getNumberOfRows();
  const unsigned int MC = m_inputParameters.m_inRasterPtr->getNumberOfColumns();
  
  for (unsigned int b = 0; b < m_inputParameters.m_inRasterBands.size(); b++)
  {
    unsigned int nband = m_inputParameters.m_inRasterBands[b];
    te::rst::Band* band = m_inputParameters.m_inRasterPtr->getBand(nband);
    te::rst::BandIteratorWindow<unsigned char> it = te::rst::BandIteratorWindow<unsigned char>::begin(band, W, H);
    te::rst::BandIteratorWindow<unsigned char> itend = te::rst::BandIteratorWindow<unsigned char>::end(band, W, H);

    while (it != itend)
    {
      R = it.getRow();
      C = it.getColumn();

      if ((R >= (unsigned)(H / 2) && R < MR - (H / 2)) &&
          (C >= (unsigned)(W / 2) && C < MC - (W / 2)))
      {
        pixel_mean = 0.0;
        for (int r = -(H / 2), rw = 0; r <= (H / 2); r++, rw++)
          for (int c = -(W / 2), cw = 0; c <= (W / 2); c++, cw++)
            pixel_mean += window_mean(rw, cw) * it.getValue(c, r);
      }
      else
        pixel_mean = it.getValue();

      m_outputParametersPtr->m_outRasterPtr->setValue(C, R, pixel_mean, b);
      ++it;
    }
    task.pulse();
  }
  if (m_outputParametersPtr->m_normalizeOutput)
    NormalizeRaster(*m_outputParametersPtr->m_outRasterPtr);

  return true;
}

bool te::rp::Filter::execModeFilter()
{
  assert(m_inputParameters.m_inRasterPtr);
  assert(m_outputParametersPtr->m_outRasterPtr);
  
  te::common::TaskProgress task(TR_RP("Mode Filter"),
                                te::common::TaskProgress::UNDEFINED,
                                m_inputParameters.m_inRasterBands.size());

  int H = m_inputParameters.m_windowH;
  int W = m_inputParameters.m_windowW;

  unsigned int R;
  unsigned int C;
  const unsigned int MR = m_inputParameters.m_inRasterPtr->getNumberOfRows();
  const unsigned int MC = m_inputParameters.m_inRasterPtr->getNumberOfColumns();
  
  std::vector<double> pixels_in_window;
  double pixel_mode;
  for (unsigned int b = 0; b < m_inputParameters.m_inRasterBands.size(); b++)
  {
    unsigned int nband = m_inputParameters.m_inRasterBands[b];
    te::rst::Band* band = m_inputParameters.m_inRasterPtr->getBand(nband);
    te::rst::BandIteratorWindow<unsigned char> it = te::rst::BandIteratorWindow<unsigned char>::begin(band, W, H);
    te::rst::BandIteratorWindow<unsigned char> itend = te::rst::BandIteratorWindow<unsigned char>::end(band, W, H);

    while (it != itend)
    {
      R = it.getRow();
      C = it.getColumn();

      if ((R >= (unsigned)(H / 2) && R < MR - (H / 2)) &&
          (C >= (unsigned)(W / 2) && C < MC - (W / 2)))
      {
        pixel_mode = 0.0;
        pixels_in_window.clear();
        for (int r = -(H / 2); r <= (H / 2); r++)
          for (int c = -(W / 2); c <= (W / 2); c++)
            pixels_in_window.push_back(it.getValue(c, r));
        pixel_mode = te::stat::Mode(pixels_in_window);
      }
      else
        pixel_mode = it.getValue();

      m_outputParametersPtr->m_outRasterPtr->setValue(C, R, pixel_mode, b);
      ++it;
    }
    task.pulse();
  }
  if (m_outputParametersPtr->m_normalizeOutput)
    NormalizeRaster(*m_outputParametersPtr->m_outRasterPtr);

  return true;
}

bool order_function(double i, double j)
{
  return (i < j);
}

bool te::rp::Filter::execMedianFilter()
{
  assert(m_inputParameters.m_inRasterPtr);
  assert(m_outputParametersPtr->m_outRasterPtr);
  
  te::common::TaskProgress task(TR_RP("Median Filter"),
                                te::common::TaskProgress::UNDEFINED,
                                m_inputParameters.m_inRasterBands.size());

  int H = m_inputParameters.m_windowH;
  int W = m_inputParameters.m_windowW;

  unsigned int R;
  unsigned int C;
  const unsigned int MR = m_inputParameters.m_inRasterPtr->getNumberOfRows();
  const unsigned int MC = m_inputParameters.m_inRasterPtr->getNumberOfColumns();
  
  std::vector<double> pixels_in_window;
  double pixel_median;
  for (unsigned int b = 0; b < m_inputParameters.m_inRasterBands.size(); b++)
  {
    unsigned int nband = m_inputParameters.m_inRasterBands[b];
    te::rst::Band* band = m_inputParameters.m_inRasterPtr->getBand(nband);
    te::rst::BandIteratorWindow<unsigned char> it = te::rst::BandIteratorWindow<unsigned char>::begin(band, W, H);
    te::rst::BandIteratorWindow<unsigned char> itend = te::rst::BandIteratorWindow<unsigned char>::end(band, W, H);

    while (it != itend)
    {
      R = it.getRow();
      C = it.getColumn();

      if ((R >= (unsigned)(H / 2) && R < MR - (H / 2)) &&
          (C >= (unsigned)(W / 2) && C < MC - (W / 2)))
      {
        pixel_median = 0.0;
        pixels_in_window.clear();
        for (int r = -(H / 2); r <= (H / 2); r++)
          for (int c = -(W / 2); c <= (W / 2); c++)
            pixels_in_window.push_back(it.getValue(c, r));

        std::sort(pixels_in_window.begin(), pixels_in_window.end(), order_function);
        pixel_median = pixels_in_window[pixels_in_window.size() / 2];
      }
      else
        pixel_median = it.getValue();

      m_outputParametersPtr->m_outRasterPtr->setValue(C, R, pixel_median, b);
      ++it;
    }
    task.pulse();
  }
  if (m_outputParametersPtr->m_normalizeOutput)
    NormalizeRaster(*m_outputParametersPtr->m_outRasterPtr);

  return true;
}

bool te::rp::Filter::execDilationFilter()
{
  assert(m_inputParameters.m_inRasterPtr);
  assert(m_outputParametersPtr->m_outRasterPtr);
  
  te::common::TaskProgress task(TR_RP("Dilation Filter"),
                                te::common::TaskProgress::UNDEFINED,
                                m_inputParameters.m_inRasterBands.size());

  int H = m_inputParameters.m_windowH;
  int W = m_inputParameters.m_windowW;

  unsigned int R;
  unsigned int C;
  const unsigned int MR = m_inputParameters.m_inRasterPtr->getNumberOfRows();
  const unsigned int MC = m_inputParameters.m_inRasterPtr->getNumberOfColumns();

  double pixel_dilation;
  double pixel;
  for (unsigned int b = 0; b < m_inputParameters.m_inRasterBands.size(); b++)
  {
    unsigned int nband = m_inputParameters.m_inRasterBands[b];
    te::rst::Band* band = m_inputParameters.m_inRasterPtr->getBand(nband);
    te::rst::BandIteratorWindow<unsigned char> it = te::rst::BandIteratorWindow<unsigned char>::begin(band, W, H);
    te::rst::BandIteratorWindow<unsigned char> itend = te::rst::BandIteratorWindow<unsigned char>::end(band, W, H);

    while (it != itend)
    {
      R = it.getRow();
      C = it.getColumn();

      if ((R >= (unsigned)(H / 2) && R < MR - (H / 2)) &&
          (C >= (unsigned)(W / 2) && C < MC - (W / 2)))
      {
        pixel_dilation = -1.0 * std::numeric_limits<double>::max();
        for (int r = -(H / 2); r <= (H / 2); r++)
          for (int c = -(W / 2); c <= (W / 2); c++)
          {
            pixel = it.getValue(c, r);
            if (pixel > pixel_dilation)
              pixel_dilation = pixel;
          }
      }
      else
        pixel_dilation = it.getValue();

      m_outputParametersPtr->m_outRasterPtr->setValue(C, R, pixel_dilation, b);
      ++it;
    }
    task.pulse();
  }
  if (m_outputParametersPtr->m_normalizeOutput)
    NormalizeRaster(*m_outputParametersPtr->m_outRasterPtr);

  return true;
}

bool te::rp::Filter::execErosionFilter()
{
  assert(m_inputParameters.m_inRasterPtr);
  assert(m_outputParametersPtr->m_outRasterPtr);
  
  te::common::TaskProgress task(TR_RP("Erosion Filter"),
                                te::common::TaskProgress::UNDEFINED,
                                m_inputParameters.m_inRasterBands.size());

  int H = m_inputParameters.m_windowH;
  int W = m_inputParameters.m_windowW;

  unsigned int R;
  unsigned int C;
  const unsigned int MR = m_inputParameters.m_inRasterPtr->getNumberOfRows();
  const unsigned int MC = m_inputParameters.m_inRasterPtr->getNumberOfColumns();

  double pixel_erosion;
  double pixel;
  for (unsigned int b = 0; b < m_inputParameters.m_inRasterBands.size(); b++)
  {
    unsigned int nband = m_inputParameters.m_inRasterBands[b];
    te::rst::Band* band = m_inputParameters.m_inRasterPtr->getBand(nband);
    te::rst::BandIteratorWindow<unsigned char> it = te::rst::BandIteratorWindow<unsigned char>::begin(band, W, H);
    te::rst::BandIteratorWindow<unsigned char> itend = te::rst::BandIteratorWindow<unsigned char>::end(band, W, H);

    while (it != itend)
    {
      R = it.getRow();
      C = it.getColumn();

      if ((R >= (unsigned)(H / 2) && R < MR - (H / 2)) &&
          (C >= (unsigned)(W / 2) && C < MC - (W / 2)))
      {
        pixel_erosion = std::numeric_limits<double>::max();
        for (int r = -(H / 2); r <= (H / 2); r++)
          for (int c = -(W / 2); c <= (W / 2); c++)
          {
            pixel = it.getValue(c, r);
            if (pixel < pixel_erosion)
              pixel_erosion = pixel;
          }
      }
      else
        pixel_erosion = it.getValue();

      m_outputParametersPtr->m_outRasterPtr->setValue(C, R, pixel_erosion, b);
      ++it;
    }
    task.pulse();
  }
  if (m_outputParametersPtr->m_normalizeOutput)
    NormalizeRaster(*m_outputParametersPtr->m_outRasterPtr);

  return true;
}
