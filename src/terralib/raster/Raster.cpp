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
  \file terralib/raster/Raster.cpp

  \brief An abstract class for raster data strucutures.
*/

// TerraLib
#include "../common/progress/TaskProgress.h"
#include "../common/STLUtils.h"
#include "../geometry/Coord2D.h"
#include "../geometry/Envelope.h"
#include "../geometry/Polygon.h"
#include "../srs/Converter.h"
#include "Band.h"
#include "BandProperty.h"
#include "Exception.h"
#include "Grid.h"
#include "Interpolator.h"
#include "PositionIterator.h"
#include "Raster.h"
#include "RasterFactory.h"
#include "RasterProperty.h"
#include "Reprojection.h"

// STL
#include <cassert>
#include <cmath>
#include <limits>
#include <memory>

te::rst::Raster::Raster()
  : m_grid(0),
    m_policy(te::common::RAccess)
{
}

te::rst::Raster::Raster(Grid* grid, te::common::AccessPolicy p)
  : m_grid(grid),
    m_policy(p)
{
}

te::rst::Raster::Raster(const Raster& rhs)
  : m_name(rhs.m_name),
    m_grid(0),
    m_policy(rhs.m_policy)
{
  m_grid = rhs.m_grid ? new Grid(*rhs.m_grid) : 0;
}

te::rst::Raster::~Raster()
{
  delete m_grid;
}

void te::rst::Raster::setName(const std::string name)
{
  m_name = name;
}

const std::string& te::rst::Raster::getName() const
{
  return m_name;
}

void te::rst::Raster::setAccessPolicy(te::common::AccessPolicy p)
{
  m_policy = p;
}

te::common::AccessPolicy te::rst::Raster::getAccessPolicy() const
{
  return m_policy;
}

te::rst::Grid* te::rst::Raster::getGrid()
{
  return m_grid;
}

const te::rst::Grid* te::rst::Raster::getGrid() const
{
  return m_grid;
}

te::gm::Envelope* te::rst::Raster::getExtent()
{
  return m_grid->getExtent();
}

const te::gm::Envelope* te::rst::Raster::getExtent() const
{
  return m_grid->getExtent();
}

te::gm::Envelope* te::rst::Raster::getExtent(int srid, te::gm::Envelope* roi) const
{
  if (srid==getSRID())
    return new te::gm::Envelope(*getExtent());

  std::auto_ptr<te::srs::Converter> converter(new te::srs::Converter());
  converter->setSourceSRID(getSRID());
  converter->setTargetSRID(srid);

  unsigned int li=0, lf=getNumberOfRows()-1;
  unsigned int ci=0, cf=getNumberOfColumns()-1;

  te::gm::Coord2D aux;
  if (roi)
  {
    aux=m_grid->geoToGrid(roi->getLowerLeftX(),roi->getLowerLeftY());
    li=static_cast<unsigned int>(aux.y);
    ci=static_cast<unsigned int>(aux.x);

    aux=m_grid->geoToGrid(roi->getUpperRightX(),roi->getUpperRightY());
    lf=static_cast<unsigned int>(aux.y);
    cf=static_cast<unsigned int>(aux.x);
  }

  aux=m_grid->gridToGeo(ci,lf);
  converter->convert(aux.x,aux.y);
  double llx=aux.x,
         lly=aux.y;

  aux=m_grid->gridToGeo(cf,li);
  converter->convert(aux.x,aux.y);
  double urx=aux.x,
         ury=aux.y;

  // follow the upper horizontal edge
  for (unsigned int c=ci; c<cf; ++c)
  {
    aux=m_grid->gridToGeo(c,li);
    if (converter->convert(aux.x,aux.y))
    {
      if(llx > aux.x) llx = aux.x;
      if(lly > aux.y) lly = aux.y;
      if(urx < aux.x) urx = aux.x;
      if(ury < aux.y) ury = aux.y;
    }
  }

  // follow the lower horizontal edge
  for (unsigned int c=ci; c<cf; ++c)
  {
    aux=m_grid->gridToGeo(c,lf);
    if (converter->convert(aux.x,aux.y))
    {
      if(llx > aux.x) llx = aux.x;
      if(lly > aux.y) lly = aux.y;
      if(urx < aux.x) urx = aux.x;
      if(ury < aux.y) ury = aux.y;
    }
  }

  // follow the left vertical edge
  for (unsigned int l=li; l<lf; ++l)
  {
    aux=m_grid->gridToGeo(ci,l);
    if (converter->convert(aux.x,aux.y))
    {
      if(llx > aux.x) llx = aux.x;
      if(lly > aux.y) lly = aux.y;
      if(urx < aux.x) urx = aux.x;
      if(ury < aux.y) ury = aux.y;
    }
  }

  // follow the right vertical edge
  for (unsigned int l=li; l<lf; ++l)
  {
    aux=m_grid->gridToGeo(cf,l);
    if (converter->convert(aux.x,aux.y))
    {
      if(llx > aux.x) llx = aux.x;
      if(lly > aux.y) lly = aux.y;
      if(urx < aux.x) urx = aux.x;
      if(ury < aux.y) ury = aux.y;
    }
  }

  return new te::gm::Envelope(llx,lly,urx,ury);
}

int te::rst::Raster::getSRID() const
{
  return m_grid->getSRID();
}

unsigned int te::rst::Raster::getNumberOfRows() const
{
  return m_grid->getNumberOfRows();
}

unsigned int te::rst::Raster::getNumberOfColumns() const
{
  return m_grid->getNumberOfColumns();
}

double te::rst::Raster::getResolutionX() const
{
  return m_grid->getResolutionX();
}

double te::rst::Raster::getResolutionY() const
{
  return m_grid->getResolutionY();
}

void te::rst::Raster::getValue(unsigned int c, unsigned int r, double& value, std::size_t b) const
{
  getBand(b)->getValue(c, r, value);
}

void te::rst::Raster::setValue(unsigned int c, unsigned int r, const double value, std::size_t b)
{
  getBand(b)->setValue(c, r, value);
}

void te::rst::Raster::getIValue(unsigned int c, unsigned int r, double& value, std::size_t b) const
{
  getBand(b)->getIValue(c, r, value);
}

void te::rst::Raster::setIValue(unsigned int c, unsigned int r, const double value, std::size_t b)
{
  getBand(b)->setIValue(c, r, value);
}

void te::rst::Raster::getValue(unsigned int c, unsigned int r, std::complex<double>& value, std::size_t b) const
{
  getBand(b)->getValue(c, r, value);
}

void te::rst::Raster::setValue(unsigned int c, unsigned int r, const std::complex<double>& value, std::size_t b)
{
  getBand(b)->setValue(c, r, value);
}

void te::rst::Raster::getValues(unsigned int c, unsigned int r, std::vector<double>& values) const
{
  values.clear();

  double v;

  for(std::size_t b = 0; b < getNumberOfBands(); b++)
  {
    getBand(b)->getValue(c, r, v);

    values.push_back(v);
  }
}

void te::rst::Raster::getValues(unsigned int c, unsigned int r, std::vector<std::complex<double> >& values) const
{
  values.clear();

  std::complex<double> v;

  for(std::size_t b = 0; b < getNumberOfBands(); b++)
  {
    getBand(b)->getValue(c, r, v);

    values.push_back(v);
  }
}

void te::rst::Raster::setValues(unsigned int c, unsigned int r, const std::vector<double>& values)
{
  assert(values.size() == getNumberOfBands());

  for(std::size_t b = 0; b < getNumberOfBands(); b++)
    getBand(b)->setValue(c, r, values[b]);
}

void te::rst::Raster::setValues(unsigned int c, unsigned int r, const std::vector<std::complex<double> >& values)
{
  assert(values.size() == getNumberOfBands());

  for(std::size_t b = 0; b < getNumberOfBands(); b++)
    getBand(b)->setValue(c, r, values[b]);
}

std::string te::rst::Raster::toString(void) const
{
  std::ostringstream output;

  output << std::endl;
  output << "Raster Name......: " << m_name << std::endl;
  output << "Number of Columns: " << getNumberOfColumns() << std::endl;
  output << "Number of Rows...: " << getNumberOfRows() << std::endl;
  output << "Number of Bands..: " << getNumberOfBands() << std::endl;
  output << "SRID.............: " << getSRID() << std::endl;
  output << "Resolution in X..: " << getResolutionX() << std::endl;
  output << "Resolution in Y..: " << getResolutionY() << std::endl;
  output << "Extent UR........: " << m_grid->getExtent()->getUpperRightX() << ", " << m_grid->getExtent()->getUpperRightY() << std::endl;
  output << "Extent LL........: " << m_grid->getExtent()->getLowerLeftX() << ", " << m_grid->getExtent()->getLowerLeftY() << std::endl;

  for (std::size_t b = 0; b < getNumberOfBands(); b++)
  {
    output << std::endl;
    output << "Band: " << b << " " << getBand(b)->getProperty()->m_description << std::endl;
    output << "  Min Values...: " << getBand(b)->getMinValue() << std::endl;
    output << "  Max Values...: " << getBand(b)->getMaxValue() << std::endl;
    output << "  Mean Values..: " << getBand(b)->getMeanValue() << std::endl;
    output << "  Std Values...: " << getBand(b)->getStdValue() << std::endl;
    output << "  Gain values..: " << getBand(b)->getScaleValue() << std::endl;
    output << "  Offset values: " << getBand(b)->getOffsetValue() << std::endl;
  }

  output << std::endl;

  return output.str();
}

te::rst::Raster& te::rst::Raster::operator+=(te::rst::Raster& rhs)
{
  assert(getNumberOfBands() == rhs.getNumberOfBands());

  for (std::size_t b = 0; b < getNumberOfBands(); b++)
    this->operator[](b) += rhs.operator[](b);

  return *this;
}

te::rst::Raster& te::rst::Raster::operator+=(std::complex<double>& cvalue)
{
  for (std::size_t b = 0; b < getNumberOfBands(); b++)
    this->operator[](b) += cvalue;

  return *this;
}

te::rst::Raster& te::rst::Raster::operator-=(te::rst::Raster& rhs)
{
  assert(getNumberOfBands() == rhs.getNumberOfBands());

  for (std::size_t b = 0; b < getNumberOfBands(); b++)
    this->operator[](b) -= rhs.operator[](b);

  return *this;
}

te::rst::Raster& te::rst::Raster::operator-=(std::complex<double>& cvalue)
{
  for (std::size_t b = 0; b < getNumberOfBands(); b++)
    this->operator[](b) -= cvalue;

  return *this;
}

te::rst::Raster& te::rst::Raster::operator*=(te::rst::Raster& rhs)
{
  assert(getNumberOfBands() == rhs.getNumberOfBands());

  for (std::size_t b = 0; b < getNumberOfBands(); b++)
    this->operator[](b) *= rhs.operator[](b);

  return *this;
}

te::rst::Raster& te::rst::Raster::operator*=(std::complex<double>& cvalue)
{
  for (std::size_t b = 0; b < getNumberOfBands(); b++)
    this->operator[](b) *= cvalue;

  return *this;
}

te::rst::Raster& te::rst::Raster::operator/=(te::rst::Raster& rhs)
{
  assert(getNumberOfBands() == rhs.getNumberOfBands());

  for (std::size_t b = 0; b < getNumberOfBands(); b++)
    this->operator[](b) /= rhs.operator[](b);

  return *this;
}

te::rst::Raster& te::rst::Raster::operator/=(std::complex<double>& cvalue)
{
  for (std::size_t b = 0; b < getNumberOfBands(); b++)
    this->operator[](b) /= cvalue;

  return *this;
}

te::rst::Raster& te::rst::Raster::operator=(const te::rst::Raster& rhs)
{
  assert(m_policy == te::common::RWAccess || m_policy == te::common::WAccess);

  if(this != &rhs)
  {
    m_name = rhs.m_name;

    delete m_grid;

    m_grid = rhs.m_grid ? new Grid(*rhs.m_grid) : 0;
  }

  return *this;
}

te::rst::Raster* te::rst::Raster::trim(const te::gm::Envelope* env, const std::map<std::string, std::string>& rinfo) const
{
// calculate output properties
  
  te::gm::Coord2D cllenv(m_grid->geoToGrid(env->getLowerLeftX(), env->getLowerLeftY()));
  te::gm::Coord2D curenv(m_grid->geoToGrid(env->getUpperRightX(), env->getUpperRightY()));
  
  const unsigned int firstInputRow = (unsigned int)
    std::min( 
      (double)( m_grid->getNumberOfRows() - 1 )
      ,
      std::max( 
        0.0
        , 
        std::floor( curenv.getY() ) 
      )
    );
  const unsigned int firstInputCol = (unsigned int)
    std::min( 
      (double)( m_grid->getNumberOfColumns() - 1 )
      ,
      std::max( 
        0.0
        , 
        std::floor( cllenv.getX() ) 
      )
    );
  const unsigned int lastInputRow = (unsigned int)
    std::min( 
      (double)( m_grid->getNumberOfRows() - 1 )
      ,
      std::max( 
        0.0
        , 
        std::ceil( cllenv.getY() ) 
      )
    );
  const unsigned int lastInputCol = (unsigned int)
    std::min( 
      (double)( m_grid->getNumberOfColumns() - 1 )
      ,
      std::max( 
        0.0
        , 
        std::ceil( curenv.getX() ) 
      )
    );    
    
  if( ( lastInputRow <= firstInputRow ) || ( lastInputCol <= firstInputCol ) )
  {
    return 0;
  }
  
  const unsigned int outputWidth = lastInputCol - firstInputCol + 1;
  const unsigned int outputHeight = lastInputRow - firstInputRow + 1;

// create output parameters and raster

  te::gm::Coord2D ulc( m_grid->gridToGeo( ((double)firstInputCol) - 0.5,
    ((double)firstInputRow) - 0.5 ) );
  
  te::rst::Grid* grid = new te::rst::Grid( outputWidth, outputHeight, getResolutionX(),
    getResolutionY(), &ulc, getSRID() );

  std::vector<te::rst::BandProperty*> bands;

  for (std::size_t b = 0; b < getNumberOfBands(); b++)
  {
    bands.push_back(new te::rst::BandProperty(*(getBand(b)->getProperty())));
    bands[ b ]->m_nblocksx = 1;
    bands[ b ]->m_nblocksy = outputHeight;
    bands[ b ]->m_blkw = outputWidth;
    bands[ b ]->m_blkh = 1;
  }

  te::rst::Raster* rout = te::rst::RasterFactory::make(grid, bands, rinfo);

// perform trim
  
  if( rout )
  {
    std::vector<std::complex<double> > values;
    unsigned int ci = 0;
    unsigned int ri = 0;
    unsigned int co = 0;
    unsigned int ro = 0;

    for( ri = firstInputRow, ro = 0; ro < outputHeight; ri++, ro++)
    {
      for( ci = firstInputCol, co = 0; co < outputWidth; ci++, co++)
      {
        getValues(ci, ri, values);

        rout->setValues(co, ro, values);
      }
    }
  }

  return rout;
}

te::rst::Raster* te::rst::Raster::resample(int method, int scale, const std::map<std::string, std::string>& rinfo) const
{
  assert(scale != 0);

// create output parameters and raster
  te::rst::Grid* grid = new te::rst::Grid(*getResampledGrid(scale));

  std::vector<te::rst::BandProperty*> bands;

  for (std::size_t b = 0; b < getNumberOfBands(); b++)
    bands.push_back(new te::rst::BandProperty(*(getBand(b)->getProperty())));

  te::rst::Raster* rout = te::rst::RasterFactory::make(grid, bands, rinfo);

// define variables for interpolation
  std::vector<std::complex<double> > v;

  double ripp = applyScale(scale, 1.0);

  double cipp = applyScale(scale, 1.0);

  double ri = 0.0;

  double ci;

  te::rst::Interpolator* interp = new te::rst::Interpolator(this, method);

// fill output raster
  for (unsigned r = 0; r < rout->getNumberOfRows(); r++, ri+=ripp)
  {
    ci = 0.0;
    for (unsigned c = 0; c < rout->getNumberOfColumns(); c++, ci+=cipp)
    {
      interp->getValues(ci, ri, v);

      rout->setValues(c, r, v);
    }
  }

  return rout;
}

te::rst::Raster* te::rst::Raster::resample(int method, unsigned int drow, 
  unsigned int dcolumn, unsigned int height, unsigned int width, 
  unsigned int newheight, unsigned int newwidth, 
  const std::map<std::string, std::string>& rinfo) const
{
  assert(drow + height <= getNumberOfRows());
  assert(dcolumn + width <= getNumberOfColumns());
  
  te::gm::Coord2D ulc = getGrid()->gridToGeo( ((double)dcolumn) - 0.5, ((double)drow) - 0.5);
  te::gm::Coord2D lrc = getGrid()->gridToGeo( ((double)(dcolumn + width)) - 0.5, 
    ((double)(drow + height)) - 0.5);
  
  te::gm::Envelope* newEnvelopePtr = new te::gm::Envelope( ulc.x, lrc.y, lrc.x,
    ulc.y );  

  // create output parameters and raster
  
  te::rst::Grid* grid = new te::rst::Grid(newwidth, newheight, newEnvelopePtr,
    getSRID());

  std::vector<te::rst::BandProperty*> bands;

  for (std::size_t b = 0; b < getNumberOfBands(); b++)
  {
    bands.push_back(new te::rst::BandProperty(*(getBand(b)->getProperty())));
    bands[ b ]->m_blkh = 1;
    bands[ b ]->m_blkw = newwidth;
    bands[ b ]->m_nblocksx = 1;
    bands[ b ]->m_nblocksy = newheight;
  }

  te::rst::Raster* rout = te::rst::RasterFactory::make(grid, bands, rinfo);

// define variables for interpolation
  std::vector<std::complex<double> > v;

  te::rst::Interpolator* interp = new te::rst::Interpolator(this, method);

// fill output raster
  double ripp = ((double)(height-1)) / ((double)(newheight-1));

  double cipp = ((double)(width-1)) / ((double)(newwidth-1));

  double ri = drow;

  double ci;

  for (unsigned r = 0; r < newheight; r++, ri+=ripp)
  {
    ci = dcolumn;
    for (unsigned c = 0; c < newwidth; c++, ci+=cipp)
    {
      interp->getValues(ci, ri, v);

      rout->setValues(c, r, v);
    }
  }

  return rout;
}

te::rst::Grid* te::rst::Raster::getResampledGrid(int scale) const
{
  assert(scale != 0);

  te::gm::Coord2D* ulc = new te::gm::Coord2D(getExtent()->getLowerLeftX(), getExtent()->getUpperRightY());

  return new te::rst::Grid((unsigned) std::ceil(applyScale(-scale, getNumberOfColumns())),
                           (unsigned) std::ceil(applyScale(-scale, getNumberOfRows())),
                           applyScale(scale, getResolutionX()), applyScale(scale, getResolutionY()),
                           ulc, getSRID());
}

double te::rst::Raster::applyScale(int i, const double& v) const
{
  if (i > 0)
    return (v / i);

  return (v * -i);
}

te::rst::Raster* te::rst::Raster::transform(int srid, const std::map<std::string, std::string>& rinfo, int m) const
{
  te::gm::Envelope transformedEnvelope( *getExtent() );
  transformedEnvelope.transform( getSRID(), srid );

  double transformedResolutionX = transformedEnvelope.getWidth() /
    ((double)getNumberOfColumns());
  double transformedResolutionY = transformedEnvelope.getHeight() /
    ((double)getNumberOfRows());

  return te::rst::Reproject(
    this,
    srid,
    getExtent()->getLowerLeftX(),
    getExtent()->getLowerLeftY(),
    getExtent()->getUpperRightX(),
    getExtent()->getUpperRightY(),
    transformedResolutionX,
    transformedResolutionY,
    rinfo,
    m);
}

te::rst::Raster* te::rst::Raster::transform(int srid, double llx, double lly, double urx, double ury, const std::map<std::string, std::string>& rinfo, int m) const
{
  return te::rst::Reproject(this, srid, llx, lly, urx, ury, 0, 0, rinfo, m);
}

te::rst::Raster* te::rst::Raster::transform(int srid, double llx, double lly, double urx, double ury, double resx, double resy, const std::map<std::string, std::string>& rinfo, int m) const
{
  return te::rst::Reproject(this, srid, llx, lly, urx, ury, resx, resy, rinfo, m);
}

void te::rst::Raster::vectorize(std::vector<te::gm::Geometry*>& g, std::size_t b, unsigned int mp)
{
  g.clear();

  te::rst::Vectorizer vectorizer(this, b, mp);

  vectorizer.run(g);
}

void te::rst::Raster::rasterize(std::vector<te::gm::Geometry*> g, std::vector<double> vp, std::size_t b)
{
  assert(b < getNumberOfBands());

  te::rst::Band* band = getBand(b);

// if vp is empty, create a vector of contrastand pixel values for neighboring polygons
  if (vp.size() == 0)
  {
    int bvalue = 254;
    for (unsigned int i = 0; i < g.size(); i++)
    {
      vp.push_back(bvalue % 255);

      bvalue = bvalue >= 127? bvalue - 126: bvalue > 255? 0: bvalue + 127;
    }
  }

  te::common::TaskProgress task;
  task.setTotalSteps(g.size());

  for (unsigned int i = 0; i < g.size(); i++)
  {
    te::gm::Polygon* polygon = static_cast<te::gm::Polygon*> (g[i]);

    te::rst::PolygonIterator<double> it = te::rst::PolygonIterator<double>::begin(band->getRaster(), polygon);
    te::rst::PolygonIterator<double> itend = te::rst::PolygonIterator<double>::end(band->getRaster(), polygon);

    while (it != itend)
    {
      setValue(it.getColumn(), it.getRow(), vp[i], b);

      ++it;
    }

    task.pulse();
  }
}
