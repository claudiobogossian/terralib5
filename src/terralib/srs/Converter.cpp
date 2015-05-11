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
  \file terralib/srs/Converter.cpp
*/

// TerraLib
#include "terralib_config.h"
#include "../common/Translator.h"
#include "Config.h"
#include "Converter.h"
#include "Exception.h"
#include "Module.h"
#include "SpatialReferenceSystemManager.h"

#ifdef TERRALIB_PROJ4_ENABLED
// proj4
#include <proj_api.h>
#endif

// STL
#include <cassert>
#include <cstring>

te::srs::Converter::Converter():
  m_targetSRID(TE_UNKNOWN_SRS),
  m_sourceSRID(TE_UNKNOWN_SRS),
  m_sourcePj4Handler(0),
  m_targetPj4Handler(0)
{
}

te::srs::Converter::Converter(int sourceSRID, int targetSRID):
  m_targetSRID(targetSRID),
  m_sourceSRID(sourceSRID),
  m_sourcePj4Handler(0),
  m_targetPj4Handler(0)
{
#ifdef TERRALIB_PROJ4_ENABLED  
  std::string description = te::srs::SpatialReferenceSystemManager::getInstance().getP4Txt(sourceSRID);
  if (description.empty())
    throw te::srs::Exception(TE_TR("Source SRS ID not recognized."));

  m_sourcePj4Handler = pj_init_plus(description.c_str());
  if (!m_sourcePj4Handler)
  {
    std::string exceptionTxt = TE_TR("Source SRS description is not valid: ");
    char* pjError = pj_strerrno(*(pj_get_errno_ref()));
    exceptionTxt += std::string(pjError);
    throw te::srs::Exception(exceptionTxt);
  }  
  
  description = te::srs::SpatialReferenceSystemManager::getInstance().getP4Txt(targetSRID);
  if ( description.empty())
      throw te::srs::Exception(TE_TR("Target SRS ID not recognized."));

  m_targetPj4Handler = pj_init_plus(description.c_str());
  if (!m_targetPj4Handler)
  {
    std::string exceptionTxt = TE_TR("Target SRS description is not valid: ");
    char* pjError = pj_strerrno(*(pj_get_errno_ref()));
    exceptionTxt += std::string(pjError);
    throw te::srs::Exception(exceptionTxt);
  }
#endif
}

te::srs::Converter::~Converter()
{
  m_sourceSRID = TE_UNKNOWN_SRS;
  m_targetSRID= TE_UNKNOWN_SRS;

#ifdef TERRALIB_PROJ4_ENABLED 
  if (m_sourcePj4Handler)
    pj_free(m_sourcePj4Handler);
  
  if (m_targetPj4Handler)
    pj_free(m_targetPj4Handler);
#endif  
  m_sourcePj4Handler = 0;
  m_targetPj4Handler = 0;
}

void 
te::srs::Converter::setSourceSRID(int sourceSRID)
{
#ifdef TERRALIB_PROJ4_ENABLED 
  if (m_sourcePj4Handler)
  {
    pj_free(m_sourcePj4Handler);
    m_sourcePj4Handler = 0;
  }

  std::string description = te::srs::SpatialReferenceSystemManager::getInstance().getP4Txt(sourceSRID);
  if ( description.empty())
      throw te::srs::Exception(TE_TR("Source SRS ID not recognized."));

  m_sourcePj4Handler = pj_init_plus(description.c_str());
  if (!m_sourcePj4Handler)
  {
    std::string exceptionTxt = TE_TR("Source SRS description is not valid: ");
    char* pjError = pj_strerrno(*(pj_get_errno_ref()));
    exceptionTxt += std::string(pjError);
    throw te::srs::Exception(exceptionTxt);
  }
#endif
  m_sourceSRID = sourceSRID;  
}

void te::srs::Converter::setSourcePJ4txt(const std::string& pj4txt)
{
  assert(!pj4txt.empty());
  
#ifdef TERRALIB_PROJ4_ENABLED 
  if (m_sourcePj4Handler)
  {
    pj_free(m_sourcePj4Handler);
    m_sourcePj4Handler = 0;
  }
  
  m_sourcePj4Handler = pj_init_plus(pj4txt.c_str());
  if (!m_sourcePj4Handler)
  {
    std::string exceptionTxt = TE_TR("Source SRS PROJ4 description is not valid: ");
    char* pjError = pj_strerrno(*(pj_get_errno_ref()));
    exceptionTxt += std::string(pjError);
    throw te::srs::Exception(exceptionTxt);
  }
#endif
  m_sourceSRID = TE_UNKNOWN_SRS;
}

int 
te::srs::Converter::getSourceSRID() const
{
  return m_sourceSRID;
}

void 
te::srs::Converter::setTargetSRID(int targetSRID)
{
#ifdef TERRALIB_PROJ4_ENABLED 
  if (m_targetPj4Handler)
  {
    pj_free(m_targetPj4Handler);
    m_targetPj4Handler = 0;
  }

  std::string description = te::srs::SpatialReferenceSystemManager::getInstance().getP4Txt(targetSRID);
  if ( description.empty())
      throw te::srs::Exception(TE_TR("Target SRS ID not recognized."));

  m_targetPj4Handler = pj_init_plus(description.c_str());
  if (!m_targetPj4Handler)
  {
    std::string exceptionTxt = TE_TR("Target SRS description is not valid: ");
    char* pjError = pj_strerrno(*(pj_get_errno_ref()));
    exceptionTxt += std::string(pjError);
    throw te::srs::Exception(exceptionTxt);
  }
#endif
  m_targetSRID = targetSRID;  
}

void te::srs::Converter::setTargetPJ4txt(const std::string& pj4txt)
{
  assert(!pj4txt.empty());
  
#ifdef TERRALIB_PROJ4_ENABLED 
  if (m_targetPj4Handler)
  {
    pj_free(m_targetPj4Handler);
    m_targetPj4Handler = 0;
  }
  
  m_targetPj4Handler = pj_init_plus(pj4txt.c_str());
  if (!m_targetPj4Handler)
  {
    std::string exceptionTxt = TE_TR("Target SRS PROJ4 description is not valid: ");
    char* pjError = pj_strerrno(*(pj_get_errno_ref()));
    exceptionTxt += std::string(pjError);
    throw te::srs::Exception(exceptionTxt);
  }
#endif
  m_targetSRID = TE_UNKNOWN_SRS;
}

int 
te::srs::Converter::getTargetSRID() const
{
  return m_targetSRID;
}

bool
te::srs::Converter::convert(double *xIn, double *yIn, double *xOut, double* yOut, long numCoord, int coordOffset) const
{
#ifdef TERRALIB_PROJ4_ENABLED 
  assert(m_sourcePj4Handler);
  assert(m_targetPj4Handler);

  memcpy(xOut, xIn, numCoord*sizeof(double));
  memcpy(yOut, yIn, numCoord*sizeof(double));

  if (pj_is_latlong(m_sourcePj4Handler))
    for (long i=0; i<numCoord; xOut[i*coordOffset]*=DEG_TO_RAD, yOut[i*coordOffset]*=DEG_TO_RAD,  ++i);

  int res = pj_transform(m_sourcePj4Handler, m_targetPj4Handler,  numCoord, coordOffset, xOut, yOut, 0);

  if (res==0 && pj_is_latlong(m_targetPj4Handler))
    for (long i=0; i<numCoord; xOut[i*coordOffset]*=RAD_TO_DEG,yOut[i*coordOffset]*=RAD_TO_DEG, ++i);

  return (res == 0);
#else
  throw te::srs::Exception(TE_TR("PROJ4 library has to be enabled in order to support coordinate conversion."));
#endif
}

bool
te::srs::Converter::convert(double *x, double* y, long numCoord, int coordOffset) const
{
#ifdef TERRALIB_PROJ4_ENABLED 
  assert(m_sourcePj4Handler);
  assert(m_targetPj4Handler);

  if (pj_is_latlong(m_sourcePj4Handler))
    for (long i=0; i<numCoord; x[i*coordOffset]*=DEG_TO_RAD, y[i*coordOffset]*=DEG_TO_RAD,  ++i);

  int res = pj_transform(m_sourcePj4Handler, m_targetPj4Handler,  numCoord, coordOffset, x, y, 0);

  if (res==0 && pj_is_latlong(m_targetPj4Handler))
    for (long i=0; i<numCoord; x[i*coordOffset]*=RAD_TO_DEG,y[i*coordOffset]*=RAD_TO_DEG, ++i);

  return (res == 0);
#else
  throw te::srs::Exception(TE_TR("PROJ4 library has to be enabled in order to support coordinate conversion."));
#endif
}

bool
te::srs::Converter::convert(const double xIn, const double yIn, double &xOut, double &yOut) const
{
#ifdef TERRALIB_PROJ4_ENABLED 
  assert(m_sourcePj4Handler);
  assert(m_targetPj4Handler);

  xOut = xIn;
  yOut = yIn;

  if (pj_is_latlong(m_sourcePj4Handler))
  {
    xOut *= DEG_TO_RAD;
    yOut *= DEG_TO_RAD;  
  }

  int res = pj_transform(m_sourcePj4Handler, m_targetPj4Handler,  1, 1, &xOut, &yOut, 0);

  if (pj_is_latlong(m_targetPj4Handler))
  {
    xOut *= RAD_TO_DEG;
    yOut *= RAD_TO_DEG;  
  }
  return (res == 0);
#else
  throw te::srs::Exception(TE_TR("PROJ4 library has to be enabled in order to support coordinate conversion."));
#endif
}

bool
te::srs::Converter::convert(double &x, double &y) const
{
#ifdef TERRALIB_PROJ4_ENABLED 
  assert(m_sourcePj4Handler);
  assert(m_targetPj4Handler);

  if (pj_is_latlong(m_sourcePj4Handler))
  {
    x *= DEG_TO_RAD;
    y *= DEG_TO_RAD;  
  }

  int res = pj_transform(m_sourcePj4Handler, m_targetPj4Handler,  1, 1, &x, &y, 0);

  if (pj_is_latlong(m_targetPj4Handler))
  {
    x *= RAD_TO_DEG;
    y *= RAD_TO_DEG;  
  }
  return (res == 0);
#else
  throw te::srs::Exception(TE_TR("PROJ4 library has to be enabled in order to support coordinate conversion."));
#endif
}

bool
te::srs::Converter::invert(double *xIn, double *yIn, double *xOut, double* yOut, long numCoord, int coordOffset) const
{
#ifdef TERRALIB_PROJ4_ENABLED 
  assert(m_sourcePj4Handler);
  assert(m_targetPj4Handler);

  memcpy(xOut, xIn, numCoord*sizeof(double));
  memcpy(yOut, yIn, numCoord*sizeof(double));

  if (pj_is_latlong(m_targetPj4Handler))
    for (long i=0; i<numCoord; xOut[i*coordOffset]*=DEG_TO_RAD, yOut[i*coordOffset]*=DEG_TO_RAD, ++i);

  int res = pj_transform(m_targetPj4Handler, m_sourcePj4Handler,  numCoord, coordOffset, xOut, yOut, 0);

  if (res==0 && pj_is_latlong(m_sourcePj4Handler))
    for (long i=0; i<numCoord; xOut[i*coordOffset]*=RAD_TO_DEG, yOut[i*coordOffset]*=RAD_TO_DEG, ++i);

  return (res == 0);
#else
  throw te::srs::Exception(TE_TR("PROJ4 library has to be enabled in order to support coordinate conversion."));
#endif
}

bool
te::srs::Converter::invert(double *x, double* y, long numCoord, int coordOffset) const
{
#ifdef TERRALIB_PROJ4_ENABLED 
  assert(m_sourcePj4Handler);
  assert(m_targetPj4Handler);

  if (pj_is_latlong(m_targetPj4Handler))
    for (long i=0; i<numCoord; x[i*coordOffset]*=DEG_TO_RAD, y[i]*=DEG_TO_RAD, ++i);

  int res = pj_transform(m_targetPj4Handler, m_sourcePj4Handler,  numCoord, coordOffset, x, y, 0);

  if (res==0 && pj_is_latlong(m_sourcePj4Handler))
    for (long i=0; i<numCoord; x[i*coordOffset]*=RAD_TO_DEG, y[i]*=RAD_TO_DEG, ++i);

  return (res == 0);
#else
  throw te::srs::Exception(TE_TR("PROJ4 library has to be enabled in order to support coordinate conversion."));
#endif
}

bool
te::srs::Converter::invert(const double xIn, const double yIn, double &xOut, double &yOut) const
{
#ifdef TERRALIB_PROJ4_ENABLED 
  assert(m_sourcePj4Handler);
  assert(m_targetPj4Handler);

  xOut = xIn;
  yOut = yIn;

  if (pj_is_latlong(m_targetPj4Handler))
  {
    xOut *= DEG_TO_RAD;
    yOut *= DEG_TO_RAD;  
  }

  int res = pj_transform(m_targetPj4Handler, m_sourcePj4Handler,  1, 1, &xOut, &yOut, 0);

  if (res==0 && pj_is_latlong(m_sourcePj4Handler))
  {
    xOut *= RAD_TO_DEG;
    yOut *= RAD_TO_DEG;  
  }
  
  return (res == 0);
#else
  throw te::srs::Exception(TE_TR("PROJ4 library has to be enabled in order to support coordinate conversion."));
#endif
}

bool
te::srs::Converter::invert(double &x, double &y) const
{
#ifdef TERRALIB_PROJ4_ENABLED 
  assert(m_sourcePj4Handler);
  assert(m_targetPj4Handler);

  if (pj_is_latlong(m_targetPj4Handler))
  {
    x *= DEG_TO_RAD;
    y *= DEG_TO_RAD;  
  }

  int res = pj_transform(m_targetPj4Handler, m_sourcePj4Handler,  1, 1, &x, &y, 0);

  if (res==0 && pj_is_latlong(m_sourcePj4Handler))
  {
    x *= RAD_TO_DEG;
    y *= RAD_TO_DEG;  
  }

  return (res == 0);
#else
  throw te::srs::Exception(TE_TR("PROJ4 library has to be enabled in order to support coordinates conversion."));
#endif
}


bool 
te::srs::Converter::convertToGeographic(double &x, double &y, int SRID) const
{
#ifdef TERRALIB_PROJ4_ENABLED 

  std::string description = te::srs::SpatialReferenceSystemManager::getInstance().getP4Txt(SRID);
  if (description.empty())
    throw te::common::Exception(TE_TR("Source SRS ID not recognized."));
  
  projPJ pjhProj = pj_init_plus(description.c_str());
  if (!pjhProj)
  {
    std::string exceptionTxt = TE_TR("srs Source SRS description is not valid: ");
    char* pjError = pj_strerrno(*(pj_get_errno_ref()));
    exceptionTxt += std::string(pjError);
    throw te::common::Exception(exceptionTxt);
  }

  if (pj_is_latlong(pjhProj))
  {
    pj_free(pjhProj);
    return true;
  }
  
  projPJ pjhGeog = pj_latlong_from_proj(pjhProj);
  int res = pj_transform(pjhProj, pjhGeog, 1, 1, &x, &y, 0);
  pj_free(pjhProj);
  pj_free(pjhGeog);

  if (res == 0)
  {
    x *= RAD_TO_DEG;
    y *= RAD_TO_DEG;
    return true;
  }
  return false;
#else
  throw te::srs::Exception(TE_TR("PROJ4 library has to be enabled in order to support coordinate conversion."));
#endif
}

bool te::srs::Converter::convertToProjected(double &lon, double &lat, int SRID) const
{
#ifdef TERRALIB_PROJ4_ENABLED 
  std::string description = te::srs::SpatialReferenceSystemManager::getInstance().getP4Txt(SRID);
  if (description.empty())
    throw te::srs::Exception(TE_TR("Source SRS ID not recognized."));
  
  projPJ pjhProj = pj_init_plus(description.c_str());
  if (!pjhProj)
  {
    std::string exceptionTxt = TE_TR("Source SRS description is not valid: ");
    char* pjError = pj_strerrno(*(pj_get_errno_ref()));
    exceptionTxt += std::string(pjError);
    throw te::common::Exception(exceptionTxt);
  }
  projPJ pjhGeog = pj_latlong_from_proj(pjhProj);

  lon *= DEG_TO_RAD;
  lat *= DEG_TO_RAD;  
  int res = pj_transform(pjhGeog, pjhProj, 1, 1, &lon, &lat, 0);

  pj_free(pjhProj);
  pj_free(pjhGeog);  
  return (res == 0);

#else
  throw te::srs::Exception(TE_TR("PROJ4 library has to be enabled in order to support coordinate conversion."));
#endif

}
