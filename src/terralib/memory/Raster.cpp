/*  Copyright (C) 2008-2013 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/dataaccess/memory/Raster.cpp

  \brief A raster class for the In-Memory driver.
*/

// TerraLib
#include "../common/STLUtils.h"
#include "../common/StringUtils.h"
#include "../common/Translator.h"
#include "../geometry/Envelope.h"
#include "../raster/Grid.h"
#include "../raster/RasterFactory.h"
#include "../raster/Utils.h"
#include "Band.h"
#include "Exception.h"
#include "Raster.h"
#include "TiledBand.h"

// STL
#include <cassert>
#include <memory>

// Boost
#include <boost/lexical_cast.hpp>

te::mem::Raster::Raster()
  : te::rst::Raster(),
    m_deleter(0),
    m_externalBuffer(0)
{
}

te::mem::Raster::Raster(const Raster& rhs)
  : te::rst::Raster(rhs),
    m_deleter(0),
    m_externalBuffer(0)
{
  const std::size_t nbands = rhs.m_bands.size();

  for(std::size_t i = 0; i < nbands; ++i)
  {
    const TiledBand* tband = dynamic_cast<const TiledBand*>(rhs.m_bands[i]);

    if(tband)
    {
      TiledBand* ntband = new TiledBand(*tband);
      ntband->setRaster(this);
      m_bands.push_back(ntband);
    }
    else
    {
      const Band* band = static_cast<const Band*>(rhs.m_bands[i]);
      Band* nband = new Band(*band);
      nband->setRaster(this);
      m_bands.push_back(nband);
    }
  }
}

te::mem::Raster::~Raster()
{
  close();
}

void te::mem::Raster::open(const std::map<std::string, std::string>& rinfo, te::common::AccessPolicy p)
{
// assure we are in a closed state
  close();

// open input
  std::map<std::string, std::string>::const_iterator it = rinfo.find("MEM_SRC_RASTER_DRIVER_TYPE");
  std::map<std::string, std::string>::const_iterator itend = rinfo.end();

  std::auto_ptr<te::rst::Raster> iraster(0);

// see if input raster comes from default driver or a specific one
  if(it != itend)
  {
    iraster.reset(te::rst::RasterFactory::open(it->second, rinfo, p));
  }
  else
  {
    iraster.reset(te::rst::RasterFactory::open(rinfo, p));
  }

// see if we must create a tiled raster or not
  bool isTiled = false;

  it = rinfo.find("MEM_TILED_RASTER");

  if((it != itend) &&
     (te::common::Convert2UCase(it->second) == "TRUE"))
    isTiled = true;

// determine the block data width, height and the number of data blocks in each dimension
  int blkw = iraster->getNumberOfColumns();
  int blkh = iraster->getNumberOfRows();
  int nblksx = 1;
  int nblksy = 1;

  if(isTiled)
  {
    it = rinfo.find("MEM_TILE_WIDTH");

    if(it != itend)
    {
      blkw = boost::lexical_cast<int>(it->second);

      it = rinfo.find("MEM_TILE_HEIGHT");

      if(it == itend)
        throw Exception(TE_TR("You must provide the MEM_TILE_HEIGHT parameter"));

      blkh = boost::lexical_cast<int>(it->second);

      nblksx = (iraster->getNumberOfColumns() + blkw - 1) / blkw;
      nblksy = (iraster->getNumberOfRows() + blkh - 1) / blkh;
    }
    else
    {
      it = rinfo.find("MEM_TILE_HEIGHT");

      if(it != itend)
      {
        throw Exception(TE_TR("You must provide the MEM_TILE_WIDTH parameter"));
      }
      else
      {
        if((iraster->getBand(0) == 0) || (iraster->getBand(0)->getProperty() == 0))
          throw Exception(TE_TR("It is not possible to determine the in-memory raster tile dimension because the input raster doesn't have enough information!"));

        blkw = iraster->getBand(0)->getProperty()->m_blkw;
        blkh = iraster->getBand(0)->getProperty()->m_blkh;
        nblksx = iraster->getBand(0)->getProperty()->m_nblocksx;
        nblksy = iraster->getBand(0)->getProperty()->m_nblocksy;
      }
    }
  }

// clone the grid from the input raster
  m_grid = new te::rst::Grid(*iraster->getGrid());

// create the raster bands
  const std::size_t nbands = iraster->getNumberOfBands();

  for(std::size_t b = 0; b < nbands; ++b)
  {
    te::rst::Band* iband = iraster->getBand(b);
    te::rst::BandProperty* ibprop = iband->getProperty();

    std::auto_ptr<te::rst::BandProperty> bprop(new te::rst::BandProperty(*ibprop));
    bprop->m_blkh = blkh;
    bprop->m_blkw = blkw;
    bprop->m_nblocksx = nblksx;
    bprop->m_nblocksy = nblksy;

    if(isTiled)
    {
// things may have changed since we found out it is a tiled raster!
      m_bands.push_back(new te::mem::TiledBand(this, bprop.release(), b));
    }
    else
    {
      m_bands.push_back(new te::mem::Band(this, bprop.release(), b));
    }
  }

// set the access policy
  m_policy = p;

// copy band data from input raster
  te::rst::Copy(*iraster, *this);
}

std::map<std::string, std::string> te::mem::Raster::getInfo() const
{
  return std::map<std::string, std::string>();
}

std::size_t te::mem::Raster::getNumberOfBands() const
{
  return m_bands.size();
}

int te::mem::Raster::getBandDataType(std::size_t i) const
{
  assert(i < getNumberOfBands());

  return m_bands[i]->getProperty()->getType();
}

const te::rst::Band* te::mem::Raster::getBand(std::size_t i) const
{
  return m_bands[i];
}

te::rst::Band* te::mem::Raster::getBand(std::size_t i)
{
  return m_bands[i];
}

const te::rst::Band& te::mem::Raster::operator[](std::size_t i) const
{
  return *m_bands[i];
}

te::rst::Band& te::mem::Raster::operator[](std::size_t i)
{
  return *m_bands[i];
}

te::dt::AbstractData* te::mem::Raster::clone() const
{
  return new Raster(*this);
}

void te::mem::Raster::close()
{
  te::common::FreeContents(m_bands);

  m_bands.clear();

  if(m_deleter)
  {
    m_deleter(m_externalBuffer);
    m_externalBuffer = 0;
    m_deleter = 0;
  }

  delete m_grid;
  m_grid = 0;

  m_name.clear();
}

void te::mem::Raster::create(te::rst::Grid* g,
                             const std::vector<te::rst::BandProperty*> bands,
                             const std::map<std::string, std::string>& rinfo,
                             void* h, void (*deleter)(void*))
{
// assure we are in a closed state
  close();

  std::map<std::string, std::string>::const_iterator it;
  std::map<std::string, std::string>::const_iterator itend = rinfo.end();

  m_deleter = deleter;

// how to create the raster?
  if(g != 0 && !bands.empty())
  {
// define grid information
    m_grid = g;

// define policy
    m_policy = te::common::RWAccess;

    if(h)
    {
      bool isRaster = true;

      it = rinfo.find("MEM_IS_DATA_BUFFER");

      if((it != itend) &&
         (te::common::Convert2UCase(it->second) == "TRUE"))
        isRaster = false;

      if(isRaster)
      {
        te::rst::Raster* iraster = static_cast<te::rst::Raster*> (h);

        if (iraster == 0)
        {
          delete m_grid;
          m_grid = 0;

          te::common::FreeContents(bands);

          throw Exception(TE_TR("You must provide a valid input raster in the 'h' parameter"));
        }

// copy raster bands
        const std::size_t nbands = bands.size();

        bool isTiled = false;

        if (bands[0]->m_nblocksx > 1 || bands[0]->m_nblocksy > 1)
          isTiled = true;

        for(std::size_t b = 0; b < nbands; ++b)
        {
          if(isTiled)
            m_bands.push_back(new te::mem::TiledBand(this, bands[b], b));
          else
            m_bands.push_back(new te::mem::Band(this, bands[b], b));
        }

        te::rst::Copy(*iraster, *this);
      }
      else
      {
// define policy
        m_policy = te::common::RWAccess;

// create bands
        for(std::size_t b = 0; b < bands.size(); ++b)
        {
          te::rst::BandProperty* ibprop = new te::rst::BandProperty(b, bands[b]->getType());

          ibprop->m_blkh = m_grid->getNumberOfRows();
          ibprop->m_blkw = m_grid->getNumberOfColumns();
          ibprop->m_nblocksx = 1;
          ibprop->m_nblocksy = 1;

          unsigned blksize = m_grid->getNumberOfRows() * m_grid->getNumberOfColumns() * te::rst::GetPixelSize(bands[b]->getType());

          void* externalBuffer = (unsigned char*) h + (b * blksize);

          m_bands.push_back(new te::mem::Band(this, ibprop, b, externalBuffer));
        }
      }
    }
    else
    {
// create bands
      if (bands[0]->m_nblocksx > 1 || bands[0]->m_nblocksy > 1)
      {
// is tiled
        for(std::size_t b = 0; b < bands.size(); ++b)
        {
          if (bands[b]->m_blkw == 0 || bands[b]->m_blkh == 0)
          {
            delete m_grid;
            m_grid = 0;

            te::common::FreeContents(bands);

            throw Exception(TE_TR("You must provide the parameters of block height and width"));
          }

          m_bands.push_back(new te::mem::TiledBand(this, bands[b], b));
        }
      }
      else
      {
// is not tiled
        for(std::size_t b = 0; b < bands.size(); ++b)
        {
          bands[b]->m_blkh = m_grid->getNumberOfRows();
          bands[b]->m_blkw = m_grid->getNumberOfColumns();
          bands[b]->m_nblocksx = 1;
          bands[b]->m_nblocksy = 1;

          m_bands.push_back(new te::mem::Band(this, bands[b], b));
        }
      }
    }
  }
  else if(h != 0)
  {
    bool isRaster = true;

    it = rinfo.find("MEM_IS_DATA_BUFFER");

    if((it != itend) &&
       (te::common::Convert2UCase(it->second) == "TRUE"))
      isRaster = false;

    if(isRaster)
    {
      te::rst::Raster* iraster = static_cast<te::rst::Raster*> (h);

      if (iraster == 0)
        throw Exception(TE_TR("You must provide a valid input raster in the 'h' parameter"));

// clone the grid from the input raster
      m_grid = new te::rst::Grid(*iraster->getGrid());

// create the raster bands
      const std::size_t nbands = iraster->getNumberOfBands();

      bool isTiled = false;

      if (iraster->getBand(0)->getProperty()->m_nblocksx > 1 || iraster->getBand(0)->getProperty()->m_nblocksy > 1)
        isTiled = true;

      for(std::size_t b = 0; b < nbands; ++b)
      {
        te::rst::Band* iband = iraster->getBand(b);
        te::rst::BandProperty* ibprop = iband->getProperty();

        std::auto_ptr<te::rst::BandProperty> bprop(new te::rst::BandProperty(*ibprop));

        if(isTiled)
          m_bands.push_back(new te::mem::TiledBand(this, bprop.release(), b));
        else
          m_bands.push_back(new te::mem::Band(this, bprop.release(), b));
      }

      te::rst::Copy(*iraster, *this);
    }
    else
    {
// h is a buffer

// get number of rows
      it = rinfo.find("MEM_BUFFER_NROWS");

      if(it == itend)
        throw Exception(TE_TR("You must provide the MEM_BUFFER_NROWS parameter"));

      unsigned int nrows = boost::lexical_cast<unsigned>(it->second);

// get number of columns
      it = rinfo.find("MEM_BUFFER_NCOLS");

      if(it == itend)
        throw Exception(TE_TR("You must provide the MEM_BUFFER_NCOLS parameter"));

      unsigned int ncols = boost::lexical_cast<unsigned>(it->second);

// get bands data type
      it = rinfo.find("MEM_BUFFER_DATATYPE");

      if(it == itend)
        throw Exception(TE_TR("You must provide the MEM_BUFFER_DATATYPE parameter"));

      unsigned int dt = boost::lexical_cast<unsigned>(it->second);

// get number of bands
      it = rinfo.find("MEM_BUFFER_NBANDS");

      if(it == itend)
        throw Exception(TE_TR("You must provide the MEM_BUFFER_NBANDS parameter"));

      unsigned int nbands = boost::lexical_cast<unsigned>(it->second);

// get the srid
      it = rinfo.find("MEM_BUFFER_SRID");

      int srid = -1;

      if(it != itend)
        srid = boost::lexical_cast<unsigned>(it->second);

// get bouding box
      double minx = 0.0,
             miny = 0.0,
             maxx = nrows - 1,
             maxy = ncols - 1,
             resx = 1.0,
             resy = 1.0;

// get min-x bounding box
      it = rinfo.find("MEM_BUFFER_MIN_X");

      if(it != itend)
      {
        minx = boost::lexical_cast<double>(it->second);

// get min-y bounding box
        it = rinfo.find("MEM_BUFFER_MIN_Y");

        if(it == itend)
          throw Exception(TE_TR("You must provide the MEM_BUFFER_MIN_Y parameter"));

        miny = boost::lexical_cast<double>(it->second);

// get max-x bounding box
        it = rinfo.find("MEM_BUFFER_MAX_X");

        if(it == itend)
          throw Exception(TE_TR("You must provide the MEM_BUFFER_MAX_X parameter"));

        maxx = boost::lexical_cast<double>(it->second);

// get max-y bounding box
        it = rinfo.find("MEM_BUFFER_MAX_Y");

        if(it == itend)
          throw Exception(TE_TR("You must provide the MEM_BUFFER_MAX_Y parameter"));

        maxy = boost::lexical_cast<double>(it->second);

// get x resolution
        it = rinfo.find("MEM_BUFFER_RES_X");

        if(it == itend)
          throw Exception(TE_TR("You must provide the MEM_BUFFER_RES_X parameter"));

        resx = boost::lexical_cast<double>(it->second);

// get y resolution
        it = rinfo.find("MEM_BUFFER_RES_Y");

        if(it == itend)
          throw Exception(TE_TR("You must provide the MEM_BUFFER_RES_Y parameter"));

        resy = boost::lexical_cast<double>(it->second);
      }

// define grid information
      te::gm::Envelope* mbr = new te::gm::Envelope(minx, miny, maxx, maxy);
      m_grid = new te::rst::Grid(ncols, nrows, resx, resy, mbr, srid);

// define policy
      m_policy = te::common::RWAccess;

// create bands
      for(std::size_t b = 0; b < nbands; ++b)
      {
        te::rst::BandProperty* ibprop = new te::rst::BandProperty(b, dt);

        ibprop->m_blkh = nrows;
        ibprop->m_blkw = ncols;
        ibprop->m_nblocksx = 1;
        ibprop->m_nblocksy = 1;

        unsigned blksize = ncols * nrows * te::rst::GetPixelSize(dt);

        void* externalBuffer = (unsigned char*) h + (b * blksize);

        m_bands.push_back(new te::mem::Band(this, ibprop, b, externalBuffer));
      }
    }
  }
// grid and band paramteres were omitted, and no external buffer was provided
  else if(!rinfo.empty())
  {
    it = rinfo.find("MEM_RASTER_NROWS");

    if(it != itend)
    {
// mandatory parameters

// get number of rows
      unsigned int nrows = boost::lexical_cast<unsigned>(it->second);

// get number of columns
      it = rinfo.find("MEM_RASTER_NCOLS");

      if(it == itend)
        throw Exception(TE_TR("You must provide the MEM_RASTER_NCOLS parameter"));

      unsigned int ncols = boost::lexical_cast<unsigned>(it->second);

// get the raster data type
      it = rinfo.find("MEM_RASTER_DATATYPE");

      if(it == itend)
        throw Exception(TE_TR("You must provide the MEM_RASTER_DATATYPE parameter"));

      int dt  = boost::lexical_cast<int>(it->second);

// get number of bands
      it = rinfo.find("MEM_RASTER_NBANDS");

      if(it == itend)
        throw Exception(TE_TR("You must provide the MEM_RASTER_NBANDS parameter"));

      unsigned int nbands = boost::lexical_cast<unsigned>(it->second);

// optional parameters

// get srid
      it = rinfo.find("MEM_RASTER_SRID");

      int srid = -1;

      if(it != itend)
        srid = boost::lexical_cast<int>(it->second);

// get bouding box
      double minx = 0.0,
             miny = 0.0,
             maxx = nrows - 1.0,
             maxy = ncols - 1.0,
             resx = 1.0,
             resy = 1.0;

// get min-x bounding box
      it = rinfo.find("MEM_RASTER_MIN_X");

      if(it != itend)
      {
        minx = boost::lexical_cast<double>(it->second);

// get min-y bounding box
        it = rinfo.find("MEM_RASTER_MIN_Y");

        if(it == itend)
          throw Exception(TE_TR("You must provide the MEM_RASTER_MIN_Y parameter"));

        miny = boost::lexical_cast<double>(it->second);

// get max-x bounding box
        it = rinfo.find("MEM_RASTER_MAX_X");

        if(it == itend)
          throw Exception(TE_TR("You must provide the MEM_RASTER_MAX_X parameter"));

        maxx = boost::lexical_cast<double>(it->second);

// get max-y bounding box
        it = rinfo.find("MEM_RASTER_MAX_Y");

        if(it == itend)
          throw Exception(TE_TR("You must provide the MEM_RASTER_MAX_Y parameter"));

        maxy = boost::lexical_cast<double>(it->second);

// get x resolution
        it = rinfo.find("MEM_RASTER_RES_X");

        if(it == itend)
          throw Exception(TE_TR("You must provide the MEM_RASTER_RES_X parameter"));

        resx = boost::lexical_cast<double>(it->second);

// get y resolution
        it = rinfo.find("MEM_RASTER_RES_Y");

        if(it == itend)
          throw Exception(TE_TR("You must provide the MEM_RASTER_RES_Y parameter"));

        resy = boost::lexical_cast<double>(it->second);
      }

// define grid information
      te::gm::Envelope* mbr = new te::gm::Envelope(minx, miny, maxx, maxy);
      m_grid = new te::rst::Grid(ncols, nrows, resx, resy, mbr, srid);

// define policy
      m_policy = te::common::RWAccess;

// create bands

// see if we must create a tiled raster or not
      bool isTiled = false;

      it = rinfo.find("MEM_TILED_RASTER");

      if((it != itend) &&
         (te::common::Convert2UCase(it->second) == "TRUE"))
        isTiled = true;

// determine the block data width, height and the number of data blocks in each dimension
      int blkw = ncols;
      int blkh = nrows;
      int nblksx = 1;
      int nblksy = 1;

      if(isTiled)
      {
        it = rinfo.find("MEM_TILE_WIDTH");

        if(it == itend)
          throw Exception(TE_TR("You must provide the MEM_TILE_WIDTH parameter"));

        blkw = boost::lexical_cast<int>(it->second);

        it = rinfo.find("MEM_TILE_HEIGHT");

        if(it == itend)
          throw Exception(TE_TR("You must provide the MEM_TILE_HEIGHT parameter"));

        blkh = boost::lexical_cast<int>(it->second);

        nblksx = (ncols + blkw - 1) / blkw;
        nblksy = (nrows  + blkh - 1) / blkh;
      }

      for(std::size_t b = 0; b < nbands; ++b)
      {
        te::rst::BandProperty* ibprop = new te::rst::BandProperty(b, dt);

        ibprop->m_blkh = blkh;
        ibprop->m_blkw = blkw;
        ibprop->m_nblocksx = nblksx;
        ibprop->m_nblocksy = nblksy;

        if(isTiled)
          m_bands.push_back(new te::mem::TiledBand(this, ibprop, b));
        else
          m_bands.push_back(new te::mem::Band(this, ibprop, b));
      }
    }
  }
  else
  {
    throw Exception(TE_TR("You must provide enough parameters"));
  }
}
