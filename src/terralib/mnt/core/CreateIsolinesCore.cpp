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
  \file terralib/mnt/core/CreateIsolines.cpp

  \brief This file contains a class that represents the create isolines.

 */

//TerraLib

#include "../../../../src/terralib/common/Translator.h"
#include "../../../../src/terralib/common/progress/TaskProgress.h"
#include "../../../../src/terralib/common/Logger.h"

#include "../../../../src/terralib/dataaccess/dataset/DataSetTypeConverter.h"
#include "../../../../src/terralib/dataaccess/dataset/DataSetTypeCapabilities.h"

#include "../../../../src/terralib/dataaccess/datasource/DataSource.h"
#include "../../../../src/terralib/dataaccess/datasource/DataSourceFactory.h"
#include "../../../../src/terralib/dataaccess/datasource/DataSourceInfo.h"
#include "../../../../src/terralib/dataaccess/datasource/DataSourceManager.h"
#include "../../../../src/terralib/dataaccess/query/EqualTo.h"

#include "../../../../src/terralib/datatype/Property.h"
#include "../../../../src/terralib/datatype/SimpleProperty.h"
#include "../../../../src/terralib/datatype/StringProperty.h"

#include "../../../../src/terralib/geometry/Geometry.h"
#include "../../../../src/terralib/geometry/GeometryProperty.h"

#include "../../../../src/terralib/memory/DataSet.h"
#include "../../../../src/terralib/memory/DataSetItem.h"

#include "../../../../src/terralib/raster/RasterProperty.h"
#include "../../../../src/terralib/raster/RasterFactory.h" 
#include "../../../../src/terralib/raster/Utils.h"
#include "../../../../src/terralib/raster/BandProperty.h"
#include "../../../../src/terralib/geometry/PointZ.h"
#include "../../../../src/terralib/common/UnitsOfMeasureManager.h"
#include "../../../../src/terralib/srs/SpatialReferenceSystemManager.h"
#include "../../../../src/terralib/statistics/core/Enums.h"

#include "../../mnt/core/Utils.h"
#include "CreateIsolinesCore.h"

//STL
#include <cassert>

//Boost
#include <boost/lexical_cast.hpp>


double te::mnt::CreateIsolines::m_vmax = 0.;
double te::mnt::CreateIsolines::m_vmin = 0.;
double te::mnt::CreateIsolines::m_dummy = 0.;
bool te::mnt::CreateIsolines::m_hasDummy;

te::mnt::CreateIsolines::CreateIsolines()
{
}

te::mnt::CreateIsolines::~CreateIsolines()
{
}

std::auto_ptr<te::mem::DataSet> te::mnt::CreateIsolines::createDataSet(te::da::DataSet* inputDataSet, te::da::DataSetType* dsType)
{
  std::auto_ptr<te::mem::DataSet> outDataset(new te::mem::DataSet(dsType));

  std::size_t nProp = inputDataSet->getNumProperties();

  inputDataSet->moveBeforeFirst();

  while(inputDataSet->moveNext())
  {
    //create dataset item
    te::mem::DataSetItem* outDSetItem = new te::mem::DataSetItem(outDataset.get());

    for(std::size_t t = 0; t < nProp; ++t)
    {
      te::dt::AbstractData* ad = inputDataSet->getValue(t).release();

      outDSetItem->setValue(t, ad);
    }

    //set kernel default value
    outDSetItem->setDouble("TE_MNT_CREATEISOLINES_ATTR_NAME", 0.);

    //add item into dataset
    outDataset->add(outDSetItem);
  }

  return outDataset;
}

void te::mnt::CreateIsolines::setInput(te::da::DataSourcePtr inRasterDsrc, std::string inRasterName, std::auto_ptr<te::da::DataSetType> inDsetType)
{
  m_inRasterDsrc = inRasterDsrc;
  m_inRasterName = inRasterName;
  m_inRasterDsType = inDsetType;
}

void te::mnt::CreateIsolines::setParams(std::vector<double> &nval, std::vector<double> &gval, double vmax, double vmin, double dummy, bool hasDummy)
{
  m_values = nval;
  m_guidevalues = gval;
  m_vmax = vmax;
  m_vmin = vmin;
  m_dummy = dummy;
  m_hasDummy = hasDummy;
}

void te::mnt::CreateIsolines::setOutput(te::da::DataSourcePtr outDsrc, std::string dsname)
{
  m_outDsrc = outDsrc;
  m_outDsetName = dsname;
}
std::auto_ptr<te::rst::Raster> te::mnt::CreateIsolines::getPrepareRaster()
{
  std::auto_ptr<te::da::DataSetType>dsTypeRaster = m_inRasterDsrc->getDataSetType(m_inRasterName);
  //prepare raster
  te::rst::RasterProperty* rasterProp = te::da::GetFirstRasterProperty(dsTypeRaster.get());
  std::auto_ptr<te::da::DataSet> dsRaster = m_inRasterDsrc->getDataSet(m_inRasterName);
  std::auto_ptr<te::rst::Raster> raster = dsRaster->getRaster(rasterProp->getName());
  return raster;
}

bool te::mnt::CreateIsolines::run(std::auto_ptr<te::rst::Raster> raster)
{
  std::string timeResult = "Create Isolines Grid - Start.";
#ifdef TERRALIB_LOGGER_ENABLED
  te::common::Logger::logDebug("mnt", timeResult.c_str());
#endif

  te::rst::RasterProperty* rstProp = te::da::GetFirstRasterProperty(m_inRasterDsType.get());
  te::rst::Grid* grd = rstProp->getGrid();
  m_srid = grd->getSRID();

  std::vector<std::vector<te::gm::LineString*> >   vecSegments;

  for (unsigned int i = 0; i < m_values.size(); i++)
  {
    std::vector<te::gm::LineString*> vecLine;
    vecSegments.push_back(vecLine);
  } 

  unsigned int numRows = raster->getNumberOfRows();
  unsigned int numThreads = 8;
  std::vector<RasterBlockSize> vecBlocks = calculateBlocks(numRows, numThreads);

  std::vector<GenerateSegmentsParams*> vecGenerateParams;

  for (unsigned int i = 0; i < numThreads; ++i)
  {
    vecGenerateParams.push_back(new GenerateSegmentsParams());
  }

  rstMemoryBlock(raster, vecBlocks, vecGenerateParams);
 

  for (unsigned int vg = 0; vg < vecGenerateParams.size(); ++vg)
  {
    GenerateSegmentsParams* currentBlock = vecGenerateParams[vg];
    const std::vector<std::vector<te::gm::LineString*> >& allQuotas = currentBlock->m_vecSegments;
    
    for (size_t quota = 0; quota < allQuotas.size(); ++quota)
    {
      const std::vector<te::gm::LineString*>& allSegments = allQuotas[quota];
      for (size_t seg = 0; seg < allSegments.size(); ++seg)
      {
        vecSegments[quota].push_back(allSegments[seg]);
      }
    }
  }

  for (unsigned int i = 0; i < numThreads; ++i)
  {
    delete vecGenerateParams[i];
  }
  vecGenerateParams.clear();

  std::vector<ConnectLinesParams> vecParams;
  vecParams.resize(m_values.size());
  boost::thread_group thread;

  for (unsigned int idQuota = 0; idQuota < vecParams.size(); ++idQuota)
  {
    vecParams[idQuota].m_quota = m_values[idQuota];
    vecParams[idQuota].m_vecSegments = vecSegments[idQuota];
    vecParams[idQuota].m_srid = m_srid;
    thread.add_thread(new boost::thread(connectLinesThreaded, &vecParams[idQuota]));
  }
  thread.join_all();

  std::vector<te::gm::LineString> lsOut;
  for (unsigned int idQuota = 0; idQuota < vecParams.size(); ++idQuota)
  {
    for (unsigned int i = 0; i < vecParams[idQuota].m_lsOut.size(); ++i)
    {
      lsOut.push_back(*vecParams[idQuota].m_lsOut[i]);
    }
  }

  timeResult = "Create Isolines Grid - End.";
#ifdef TERRALIB_LOGGER_ENABLED
  te::common::Logger::logDebug("mnt", timeResult.c_str());
#endif
 
  std::vector<double> guidevalues;
  te::mnt::SaveIso(m_outDsetName, m_outDsrc, lsOut, guidevalues, m_srid);
  
  return true;
}
void te::mnt::CreateIsolines::rstMemoryBlock(std::auto_ptr<te::rst::Raster> raster, std::vector<RasterBlockSize> vecBlocks, std::vector<GenerateSegmentsParams*>& vecGenerateParams)
{
  boost::thread_group threadGenerateSegments;

 
  for (unsigned int vb = 0; vb < vecBlocks.size(); ++vb)
  {
    te::gm::Coord2D coordLowerLeft = raster->getGrid()->gridToGeo(0, vecBlocks[vb].m_finalRow);
    te::gm::Coord2D coordUpperRight = raster->getGrid()->gridToGeo(raster->getNumberOfColumns() - 1, vecBlocks[vb].m_initalRow);

    
    double xmin = coordLowerLeft.getX() - (raster->getResolutionX() / 2);
    double ymin = coordLowerLeft.getY() - (raster->getResolutionY() / 2);

    double xmax = coordUpperRight.getX() + (raster->getResolutionX() / 2);
    double ymax = coordUpperRight.getY() + (raster->getResolutionY() / 2);

   
    std::map<std::string, std::string> rinfo;

    rinfo["MEM_RASTER_NROWS"] = boost::lexical_cast<std::string>(vecBlocks[vb].m_numRows);
    rinfo["MEM_RASTER_NCOLS"] = boost::lexical_cast<std::string>(raster->getNumberOfColumns());
    rinfo["MEM_RASTER_DATATYPE"] = boost::lexical_cast<std::string>(raster->getBandDataType(0));
    rinfo["MEM_RASTER_NBANDS"] = boost::lexical_cast<std::string>(raster->getNumberOfBands());
    rinfo["MEM_RASTER_SRID"] = boost::lexical_cast<std::string>(raster->getSRID());
    rinfo["MEM_RASTER_RES_X"] = boost::lexical_cast<std::string>(raster->getResolutionX());
    rinfo["MEM_RASTER_RES_Y"] = boost::lexical_cast<std::string>(raster->getResolutionY());
    rinfo["MEM_RASTER_MIN_X"] = boost::lexical_cast<std::string>(xmin);
    rinfo["MEM_RASTER_MIN_Y"] = boost::lexical_cast<std::string>(ymin);
    rinfo["MEM_RASTER_MAX_X"] = boost::lexical_cast<std::string>(xmax);
    rinfo["MEM_RASTER_MAX_Y"] = boost::lexical_cast<std::string>(ymax);
    te::rst::Raster* myraster = te::rst::RasterFactory::make("MEM", 0, std::vector<te::rst::BandProperty*>(), rinfo);

    for (unsigned r = 0; r < myraster->getNumberOfRows(); r++)
    {
      for (unsigned c = 0; c < myraster->getNumberOfColumns(); c++)
      {
        double value = 0;
        raster->getValue(c, r + vecBlocks[vb].m_initalRow, value);
        myraster->setValue(c, r, value);
      }
    }
    std::auto_ptr<te::rst::Raster> rasterPtr(myraster);
    vecGenerateParams[vb]->m_nvals = m_values;
    vecGenerateParams[vb]->m_vecSegments.resize(m_values.size());
    vecGenerateParams[vb]->m_rasterPtr = rasterPtr;
    
    threadGenerateSegments.add_thread(new boost::thread(generateSegmentsThreaded, vecGenerateParams[vb]));
  }
  threadGenerateSegments.join_all();
}
std::vector<RasterBlockSize> te::mnt::CreateIsolines::calculateBlocks(unsigned int numRows, unsigned int numThreads)
{
  int calc = (int)(numRows / numThreads) - 1;
  int aux = 0;
  int var = calc;
  struct RasterBlockSize rasterBlock;
  std::vector<RasterBlockSize> rasterBlockSize;

  for (unsigned int i = 1; i < numThreads; i++)
  {
    rasterBlock.m_idThread = (int)i;
    rasterBlock.m_numRows = var + 1;
    rasterBlock.m_initalRow = aux;
    rasterBlock.m_finalRow = calc;
    rasterBlockSize.push_back(rasterBlock);
    aux = calc;
    calc += var;
  }
  int dif = (int)numThreads - 1;
  int sum = dif + 1;
  int allRasters = var * dif;
  int lastRaster = abs(allRasters - (int)numRows);
  calc = 0;
  calc = aux + lastRaster;
  rasterBlock.m_idThread = sum;
  rasterBlock.m_numRows = lastRaster;
  rasterBlock.m_initalRow = aux;
  rasterBlock.m_finalRow = calc - 1;
  rasterBlockSize.push_back(rasterBlock);

  return rasterBlockSize;
}

GenerateSegmentsParams::GenerateSegmentsParams()
{
}

GenerateSegmentsParams::~GenerateSegmentsParams()
{
}

bool te::mnt::CreateIsolines::generateSegmentsThreaded(GenerateSegmentsParams* params)
{
  generateSegments(params->m_rasterPtr, params->m_nvals, params->m_vecSegments);
  return true;
}
bool te::mnt::CreateIsolines::generateSegments(std::auto_ptr<te::rst::Raster> raster, std::vector<double> nvals, std::vector< std::vector<te::gm::LineString*> >& vecSegments)
{
  double quota;

  double resX = raster->getResolutionX();
  double resY = raster->getResolutionY();

  double xmin = raster->getExtent()->getLowerLeftX() + (resX / 2);
  double ymax = raster->getExtent()->getUpperRightY() - (resY / 2);
  double xlg_pos = xmin;
  double ylg_inf = ymax;

  te::gm::LineString* line = new te::gm::LineString(0, te::gm::LineStringZType);
  double lineSupLeft = 0, lineSupRigth = 0, lineInfLeft = 0, lineInfRigth = 0;
  unsigned int numberColumns = raster->getNumberOfColumns();
  unsigned int numberRows = raster->getNumberOfRows();

  for (unsigned int nr = 1; nr < numberRows; ++nr)
  {
    double ylg_sup = ylg_inf;
    ylg_inf = ymax - nr * resY;
    xlg_pos = xmin;

    for (unsigned int nc = 0; nc < numberColumns - 1; ++nc)
    {
      double xlg_ant = xlg_pos;
      xlg_pos = xmin + (nc + 1) * resX;
      
      raster->getValue(nc, nr - 1, lineSupLeft);
      raster->getValue(nc + 1, nr - 1, lineSupRigth);
      raster->getValue(nc, nr, lineInfLeft);
      raster->getValue(nc + 1, nr, lineInfRigth);
      if (m_hasDummy == true && ((lineSupRigth > m_vmax) ||
        (lineSupLeft > m_vmax) ||
        (lineInfRigth > m_vmax) ||
        (lineInfLeft > m_vmax) ||
        (lineSupRigth < m_vmin) ||
        (lineSupLeft < m_vmin) ||
        (lineInfRigth < m_vmin) ||
        (lineInfLeft < m_vmin)))
      {

        continue;

      }
      else
      {
        for (unsigned int idQuota = 0; idQuota < nvals.size(); ++idQuota)
        {
          quota = nvals[idQuota];
          double delta = 0.0001;

          bool changed = true;

          while (changed)
          {
            changed = false;
            if (fabs(quota - lineSupRigth) < delta)
            {
              lineSupRigth += delta;
              changed = true;
            }
            if (fabs(quota - lineInfRigth) < delta)
            {
              lineInfRigth += delta;
              changed = true;
            }
            if (fabs(quota - lineSupLeft) < delta)
            {
              lineSupLeft += delta;
              changed = true;
            }
            if (fabs(quota - lineInfLeft) < delta)
            {
              lineInfLeft += delta;
              changed = true;
            }
          }

          if (((quota > lineSupLeft) && (quota > lineSupRigth) &&
            (quota > lineInfLeft) && (quota > lineInfRigth)) ||
            ((quota < lineSupLeft) && (quota < lineSupRigth) &&
            (quota < lineInfLeft) && (quota < lineInfRigth)))
          {

          }
          else
          {
            if (((quota < lineSupLeft) && (quota > lineSupRigth) &&
              (quota > lineInfLeft) && (quota < lineInfRigth)) ||
              ((quota > lineSupLeft) && (quota < lineSupRigth) &&
              (quota < lineInfLeft) && (quota > lineInfRigth)))
            {
              double zmeio = (lineSupLeft + lineSupRigth + lineInfLeft + lineInfRigth) / 4;

              while (quota == zmeio)
              {
                if (zmeio == 0)
                  zmeio += 0.0001;
                else
                  zmeio *= 1.001;
              }

              if (((quota > zmeio) && (quota > lineSupLeft)) ||
                ((quota < zmeio) && (quota < lineSupLeft)))
              {
                interpolacao(1, line, quota, ylg_sup, xlg_ant, xlg_pos, lineSupLeft, lineSupRigth);
                interpolacao(0, line, quota, xlg_pos, ylg_inf, ylg_sup, lineInfRigth, lineSupRigth);
                interpolacao(0, line, quota, xlg_ant, ylg_inf, ylg_sup, lineInfLeft, lineSupLeft);
                interpolacao(1, line, quota, ylg_inf, xlg_ant, xlg_pos, lineInfLeft, lineInfRigth);
              }
              else
              {
                interpolacao(0, line, quota, xlg_ant, ylg_inf, ylg_sup, lineInfLeft, lineSupLeft);
                interpolacao(1, line, quota, ylg_sup, xlg_ant, xlg_pos, lineSupLeft, lineSupRigth);
                interpolacao(1, line, quota, ylg_inf, xlg_ant, xlg_pos, lineInfLeft, lineInfRigth);
                interpolacao(0, line, quota, xlg_pos, ylg_inf, ylg_sup, lineInfRigth, lineSupRigth);
              }
            }
            else
            {
              if (((quota < lineSupLeft) && (quota > lineSupRigth)) ||
                ((quota > lineSupLeft) && (quota < lineSupRigth)))
              {
                interpolacao(1, line, quota, ylg_sup, xlg_ant, xlg_pos, lineSupLeft, lineSupRigth);
              }
              if (((quota < lineSupRigth) && (quota > lineInfRigth)) ||
                ((quota > lineSupRigth) && (quota < lineInfRigth)))
              {
                interpolacao(0, line, quota, xlg_pos, ylg_inf, ylg_sup, lineInfRigth, lineSupRigth);
              }
              if (((quota < lineInfLeft) && (quota > lineInfRigth)) ||
                ((quota > lineInfLeft) && (quota < lineInfRigth)))
              {
                interpolacao(1, line, quota, ylg_inf, xlg_ant, xlg_pos, lineInfLeft, lineInfRigth);
              }
              if (((quota < lineSupLeft) && (quota > lineInfLeft)) ||
                ((quota > lineSupLeft) && (quota < lineInfLeft)))
              {
                interpolacao(0, line, quota, xlg_ant, ylg_inf, ylg_sup, lineInfLeft, lineSupLeft);
              }
            }
            if (line->size() > 1)
            {
              if (line->size() == 4)
              {
                te::gm::LineString* line1 = new te::gm::LineString(2, te::gm::LineStringZType);
                size_t n = line->size();
                line1->setX(0, line->getPointN(0)->getX());
                line1->setY(0, line->getPointN(0)->getY());
                line1->setZ(0, line->getPointN(n - 1)->getZ());

                line1->setX(1, line->getPointN(1)->getX());
                line1->setY(1, line->getPointN(1)->getY());
                line1->setZ(1, line->getPointN(n - 1)->getZ());

                te::gm::LineString* line2 = new te::gm::LineString(2, te::gm::LineStringZType);

                line2->setX(0, line->getPointN(2)->getX());
                line2->setY(0, line->getPointN(2)->getY());
                line2->setZ(0, line->getPointN(n - 1)->getZ());

                line2->setX(1, line->getPointN(3)->getX());
                line2->setY(1, line->getPointN(3)->getY());
                line2->setZ(1, line->getPointN(n - 1)->getZ());

                vecSegments[idQuota].push_back(line1);
                vecSegments[idQuota].push_back(line2);
                delete line;
              }
              else if (line->size())
              {
                vecSegments[idQuota].push_back(line);
              }
              line = new te::gm::LineString(0, te::gm::LineStringZType);
            }
          }
        }
      }
    }
  }
  return true;
}

ConnectLinesParams::ConnectLinesParams() : m_quota(0), m_srid(0)
{
}

ConnectLinesParams::~ConnectLinesParams()
{
}

bool te::mnt::CreateIsolines::connectLinesThreaded(ConnectLinesParams* params)
{
  std::vector<te::gm::LineString*> lsOut;
  connectLines(params->m_vecSegments, params->m_srid, lsOut);
  params->m_lsOut = lsOut;
  return true;
}

bool te::mnt::CreateIsolines::connectLines(std::vector<te::gm::LineString*>  vecSegments, int srid, std::vector<te::gm::LineString*>& lsOut)
{
  std::vector<te::gm::PointZ> vecPoints;
  std::vector<int> candidate;
  std::set<unsigned int> lineRemoved;
  te::sam::rtree::Index<int> segmentsTree;
  bool borda;
  bool newiso = false;

  for (unsigned int i = 0; i < vecSegments.size(); ++i)
    segmentsTree.insert(*(vecSegments[i]->getMBR()), (int)i);

  
  te::common::UnitOfMeasurePtr unitin = te::srs::SpatialReferenceSystemManager::getInstance().getUnit((unsigned int)srid);
  double tol=0;
  if (unitin->getId() == te::common::UOM_Metre)
  {
    tol = 0.000001;
  }
  else if (unitin->getId() == te::common::UOM_Degree)
  {
    tol = 0.000001 / 1000;
  }

  std::vector<te::gm::Envelope> vecEnvelope;

  for (unsigned int l = 0; l < vecSegments.size(); ++l)
  {
    std::set<unsigned int>::iterator id = lineRemoved.find(l);
    if (id != lineRemoved.end())
      continue;

    if (std::find(vecEnvelope.begin(), vecEnvelope.end(), *vecSegments[l]->getMBR()) != vecEnvelope.end())
    {
      continue;
    }

    borda = false;
    te::gm::LineString* currentSegment = new te::gm::LineString(0, te::gm::LineStringZType);
    currentSegment->setNumCoordinates(vecSegments[l]->size());
    currentSegment = vecSegments[l];

    segmentsTree.remove(*(currentSegment->getMBR()), (int)l);
    lineRemoved.insert(l);

    vecPoints.clear();
    
    te::gm::PointZ ps(currentSegment->getStartPoint()->getX(), currentSegment->getStartPoint()->getY(), currentSegment->getEndPoint()->getZ());
    vecPoints.push_back(ps);

    te::gm::PointZ pe(currentSegment->getEndPoint()->getX(), currentSegment->getEndPoint()->getY(), currentSegment->getEndPoint()->getZ());
    vecPoints.push_back(pe);

    size_t npts = vecPoints.size();

    while (!segmentsTree.isEmpty())
    {
      bool hasSegment = false;

      te::gm::Envelope currentEnvelope = *currentSegment->getMBR();
      te::gm::Envelope searchEnvelope(currentEnvelope.getLowerLeftX() - tol, currentEnvelope.getLowerLeftY() - tol, currentEnvelope.getUpperRightX() + tol, currentEnvelope.getUpperRightY() + tol);

      segmentsTree.search(searchEnvelope, candidate);

      std::sort(candidate.begin(), candidate.end());

      for (unsigned int i = 0; i < candidate.size(); ++i)
      {
        if (std::find(vecEnvelope.begin(), vecEnvelope.end(), *vecSegments[(unsigned int)candidate[i]]->getMBR()) != vecEnvelope.end())
        {
          continue;
        }

        te::gm::PointZ ptStartC(vecSegments[(unsigned int)candidate[i]]->getX(0), vecSegments[(unsigned int)candidate[i]]->getY(0), vecSegments[(unsigned int)candidate[i]]->getZ(1));
        te::gm::PointZ ptEndC(vecSegments[(unsigned int)candidate[i]]->getX(1), vecSegments[(unsigned int)candidate[i]]->getY(1), vecSegments[(unsigned int)candidate[i]]->getZ(1));
        te::gm::PointZ ptEnd(vecPoints[(unsigned int)npts - 1].getX(), vecPoints[(unsigned int)npts - 1].getY(), vecPoints[(unsigned int)npts - 1].getZ());

        if (equal(ptStartC, ptEnd, tol))
        {
          te::gm::PointZ pt;
          pt.setX(vecSegments[(unsigned int)candidate[i]]->getEndPoint()->getX());
          pt.setY(vecSegments[(unsigned int)candidate[i]]->getEndPoint()->getY());
          pt.setZ(vecSegments[(unsigned int)candidate[i]]->getEndPoint()->getZ());

          vecPoints.push_back(pt);
          ++npts;

          segmentsTree.remove(*(vecSegments[(unsigned int)candidate[i]]->getMBR()), candidate[i]);
          lineRemoved.insert((unsigned int)candidate[i]);

          currentSegment = new te::gm::LineString(0, te::gm::LineStringZType);
          currentSegment->setNumCoordinates(currentSegment->size() + 1);
          currentSegment->setPointZ(currentSegment->size() - 1, vecPoints[npts - 2].getX(), vecPoints[npts - 2].getY(), vecPoints[npts - 2].getZ());
          currentSegment->setNumCoordinates(currentSegment->size() + 1);
          currentSegment->setPointZ(currentSegment->size() - 1, vecPoints[npts - 1].getX(), vecPoints[npts - 1].getY(), vecPoints[npts - 1].getZ());
          hasSegment = true;
          break;
        }
        else if (equal(ptEndC, ptEnd, tol))
        {
          te::gm::PointZ pt;
          pt.setX(vecSegments[(unsigned int)candidate[i]]->getStartPoint()->getX());
          pt.setY(vecSegments[(unsigned int)candidate[i]]->getStartPoint()->getY());
          pt.setZ(vecSegments[(unsigned int)candidate[i]]->getEndPoint()->getZ());

          vecPoints.push_back(pt);
          ++npts;

          segmentsTree.remove(*(vecSegments[(unsigned int)candidate[i]]->getMBR()), candidate[i]);
          lineRemoved.insert((unsigned int)candidate[i]);

          currentSegment = new te::gm::LineString(0, te::gm::LineStringZType);
          currentSegment->setNumCoordinates(currentSegment->size() + 1);
          currentSegment->setPointZ(currentSegment->size() - 1, vecPoints[(unsigned int)npts - 2].getX(), vecPoints[(unsigned int)npts - 2].getY(), vecPoints[(unsigned int)npts - 2].getZ());
          currentSegment->setNumCoordinates(currentSegment->size() + 1);
          currentSegment->setPointZ(currentSegment->size() - 1, vecPoints[(unsigned int)npts - 1].getX(), vecPoints[(unsigned int)npts - 1].getY(), vecPoints[(unsigned int)npts - 1].getZ());

          hasSegment = true;
          break;
        }
      }
      if (!hasSegment)
      {
        if (borda){
          if (vecPoints.size()>2)
            newiso = true;
          break;
        }
        else
        {
          std::reverse(vecPoints.begin(), vecPoints.end());
          borda = true;

          currentSegment = new te::gm::LineString(0, te::gm::LineStringZType);
          currentSegment->setNumCoordinates(currentSegment->size() + 1);
          currentSegment->setPointZ(currentSegment->size() - 1, vecPoints[(unsigned int)npts - 2].getX(), vecPoints[(unsigned int)npts - 2].getY(), vecPoints[(unsigned int)npts - 1].getZ());
          currentSegment->setNumCoordinates(currentSegment->size() + 1);
          currentSegment->setPointZ(currentSegment->size() - 1, vecPoints[(unsigned int)npts - 1].getX(), vecPoints[(unsigned int)npts - 1].getY(), vecPoints[(unsigned int)npts - 1].getZ());
          hasSegment = true;
        }
      }
      te::gm::PointZ ptStart(vecPoints[0].getX(), vecPoints[0].getY(), vecPoints[(unsigned int)npts - 1].getZ());
      te::gm::PointZ ptEnd(vecPoints[(unsigned int)npts - 1].getX(), vecPoints[(unsigned int)npts - 1].getY(), vecPoints[(unsigned int)npts - 1].getZ());
      if (npts > 2 && equal(ptStart, ptEnd, tol))
      {
        newiso = true;
        break;
      }
      candidate.clear();
    }
    if (newiso || segmentsTree.isEmpty())
    {
      te::gm::LineString* lineOut = new te::gm::LineString(vecPoints.size(), te::gm::LineStringZType);
      for (unsigned int l = 0; l < vecPoints.size(); ++l)
      {
        lineOut->setPointZ(l, vecPoints[l].getX(), vecPoints[l].getY(), vecPoints[l].getZ());
      }
      lsOut.push_back(lineOut);
      newiso = false;
    }
  }
  return true;
}

void te::mnt::CreateIsolines::interpolacao(int direction, te::gm::LineString* line, double quota, double coord, double c_inf, double c_sup, double z_inf, double z_sup)
{
  double aux = c_inf + ((quota - z_inf) * (c_sup - c_inf) / (z_sup - z_inf));
  
  line->setNumCoordinates(line->size() + 1);

  if (direction == 0)
  {
    line->setPointZ(line->size()-1, coord, aux, quota);
  }
  else
  {
    line->setPointZ(line->size()-1, aux, coord, quota);
  }
}

bool te::mnt::CreateIsolines::equal(te::gm::PointZ &p1, te::gm::PointZ &p2, double &tol)
{
  return (std::fabs(p1.getX() - p2.getX()) < tol && std::fabs(p1.getY() - p2.getY()) < tol/* && std::fabs(p1.getZ() - p2.getZ()) < tol*/);
}

