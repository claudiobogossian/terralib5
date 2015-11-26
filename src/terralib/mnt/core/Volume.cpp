/*!
\file terralib/mnt/core/Volume.cpp

\brief This file contains a class to isolines smooth.

*/

#include "Volume.h"
#include "Utils.h"

//terralib
#include "../../common/StringUtils.h"
#include "../../dataaccess/utils/Utils.h"
#include "../../dataaccess/dataset/DataSetAdapter.h"
#include "../../dataaccess/dataset/DataSetTypeConverter.h"
#include "../../geometry/GeometryProperty.h"
#include "../../geometry/GEOSWriter.h"
#include "../../raster.h"
#include "../../srs/SpatialReferenceSystemManager.h"

#include <geos/geom/Point.h>
#include <geos/geom/Polygon.h>
#include <geos/geosAlgorithm.h>



te::mnt::Volume::Volume()
{
}

te::mnt::Volume::~Volume()
{
}

void te::mnt::Volume::setInput(te::da::DataSourcePtr inrstDsrc,
  std::string inrstDsetName,
  std::auto_ptr<te::da::DataSetType> inrstDsetType,
  te::da::DataSourcePtr invecDsrc,
  std::string invecDsetName,
  std::auto_ptr<te::da::DataSetType> invecDsetType,
  const te::da::ObjectIdSet* OidSet)
{
  m_inrstDsrc = inrstDsrc;
  m_inrstDsetName = inrstDsetName;
  m_inrstDsetType = inrstDsetType;

  m_invecDsrc = invecDsrc;
  m_invecDsetName = invecDsetName;
  m_invecDsetType = invecDsetType;

  m_OidSet = OidSet;

}

void te::mnt::Volume::setParams(double quota, std::string &attr, double dummy)
{ 
  m_quota = quota;
  m_attr = attr;
  m_dummy = dummy;
}

bool te::mnt::Volume::run()
{
  te::rst::RasterProperty* rasterProp = te::da::GetFirstRasterProperty(m_inrstDsetType.get());
  std::auto_ptr<te::da::DataSet> dsRaster = m_inrstDsrc->getDataSet(m_inrstDsetName);
  m_raster = dsRaster->getRaster(rasterProp->getName());
  m_raster->getBand(0)->getProperty()->m_noDataValue = m_dummy;

  std::auto_ptr<te::da::DataSet> inDsetSrc;

  if (m_OidSet == 0)
    inDsetSrc = m_invecDsrc->getDataSet(m_invecDsetName);
  else
    inDsetSrc = m_invecDsrc->getDataSet(m_invecDsetName, m_OidSet);

  std::size_t geo_pos = te::da::GetFirstPropertyPos(inDsetSrc.get(), te::dt::GEOMETRY_TYPE);
  te::gm::GeomType gtype = (te::da::GetFirstGeomProperty(m_invecDsetType.get()))->getGeometryType();

  inDsetSrc->moveBeforeFirst();

  int flin, llin, fcol, lcol;
  std::vector <te::gm::Polygon *> pols;

  while (inDsetSrc->moveNext())
  {
    std::auto_ptr<te::gm::Geometry> gin = inDsetSrc->getGeometry(geo_pos);
    if (gtype == te::gm::PolygonType)
    {
      pols.push_back(dynamic_cast<te::gm::Polygon*>(gin.get()->clone()));
    }
    if (gtype == te::gm::MultiPolygonType)
    {
      te::gm::MultiPolygon* mp = dynamic_cast<te::gm::MultiPolygon*>(gin.get());
      for (size_t p = 0; p < mp->getNumGeometries(); p++)
      {
        pols.push_back(dynamic_cast<te::gm::Polygon*>(mp->getGeometryN(p)->clone()));
      }
    }

    for (size_t p = 0; p < pols.size(); p++)
    if (DefLC(pols[p], flin, llin, fcol, lcol))
    {
      //Calculate the volumes.
      if (CVGrd(pols[p], flin, llin, fcol, lcol))
      {
        m_polyvec.push_back(inDsetSrc->getString(m_attr));
      }
    }
    pols.clear();
  }

  return true;
}

bool te::mnt::Volume::DefLC(te::gm::Polygon *pol, int& flin, int& llin, int& fcol, int& lcol)
{
  if (!pol)
    return false;

  te::gm::Coord2D cg;

  const te::gm::Envelope *env = pol->getMBR();

  te::gm::Point llpt(env->getLowerLeftX(), env->getLowerLeftY());
  te::gm::Point urpt(env->getUpperRightX(), env->getUpperRightY());

  //  Calculate lines and coluns intercepted
  cg = m_raster->getGrid()->geoToGrid(llpt.getX(), llpt.getY());
  fcol = te::rst::Round(cg.getX());
  llin = te::rst::Round(cg.getY());
  cg = m_raster->getGrid()->geoToGrid(urpt.getX(), urpt.getY());
  lcol = te::rst::Round(cg.getX());
  flin = te::rst::Round(cg.getY());

  if (((int)m_raster->getNumberOfColumns() <= fcol) || (lcol < 0) ||
    ((int)m_raster->getNumberOfRows() <= flin) || (llin < 0))
    return false;

  if (fcol < 0)
    fcol = 0;
  if (flin < 0)
    flin = 0;
  if ((int)m_raster->getNumberOfColumns() <= lcol)
    lcol = m_raster->getNumberOfColumns() - 1;
  if ((int)m_raster->getNumberOfRows() <= llin)
    llin = m_raster->getNumberOfRows() - 1;

  return true;
}

bool te::mnt::Volume::CVGrd(te::gm::Polygon *pol, int flin, int llin, int fcol, int lcol)
{
  double somazs = 0.0;
  double nrozs = 0.0;
  double areabase;
  double areatotal;
  double volcorte = 0.;
  double volaterro = 0.;

  double rx = m_raster->getResolutionX();
  double ry = m_raster->getResolutionY();

  areabase = rx * ry;
  areatotal = areabase;

  te::gm::Coord2D cg;
  geos::geom::Coordinate pt;
  double value;
  bool contr = true;
  std::auto_ptr<geos::geom::Geometry> polGeom(te::gm::GEOSWriter::write(pol)->clone());
  geos::algorithm::PointLocator loc;

  // Take each line and column of the bounding box.
  for (int i = flin; i <= llin; i++)
  {
    for (int j = fcol; j <= lcol; j++)
    {
      cg = m_raster->getGrid()->gridToGeo(j, i);
      geos::geom::Coordinate pt(cg.getX(), cg.getY());

      if (loc.locate(pt, polGeom.get()) == geos::geom::Location::INTERIOR)
      {
       // Verify ans atualize the profile volume or embankment volume.
       m_raster->getValue(j, i, value);
       if (value >= m_dummy)
       {
          contr = false;
          i = llin + 1;
          j = lcol + 1;
          break;
        }
        else{
          if (value < m_quota)
            volaterro += (m_quota - value);
          else
            volcorte += (value - m_quota);
          areatotal += areabase;
          somazs += value;
          nrozs = nrozs + (double)1.0;
        }
      }
    }
    if (!contr) break;
  }

  polGeom.release();

  volaterro *= areabase;
  volcorte *= areabase;

  // Show the volumes values.
  m_cortevec.push_back(te::common::Convert2String(volcorte));
  m_aterrovec.push_back(te::common::Convert2String(volaterro));
  m_areavec.push_back(te::common::Convert2String(pol->getArea()));

  if (nrozs != 0)
    m_iquotavec.push_back(te::common::Convert2String(somazs / nrozs));
  else
    m_iquotavec.push_back(te::common::Convert2String(m_quota));

  return true;
}

void te::mnt::Volume::getResults(std::vector<std::string> &polyvec,
  std::vector<std::string> &cortevec,
  std::vector<std::string> &aterrovec,
  std::vector<std::string> &areavec,
  std::vector<std::string> &iquotavec)
{
  polyvec = m_polyvec;
  cortevec = m_cortevec;
  aterrovec = m_aterrovec;
  areavec = m_areavec;
  iquotavec = m_iquotavec;
}


