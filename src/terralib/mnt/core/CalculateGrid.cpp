/*!
\file terralib/mnt/core/CalculateGrid.cpp

\brief This file contains a class to calculate a grid from Sample.

*/

#include "CalculateGrid.h"
#include "Utils.h"

#include "../../common/progress/TaskProgress.h"
#include "../../raster.h"
#include "../../raster/BandProperty.h"
#include "../../raster/Grid.h"
#include "../../raster/RasterFactory.h"

#include <algorithm>
#include <fstream>

#define THRESHOLD 48

te::mnt::CalculateGrid::~CalculateGrid()
{
  delete m_rst;
  delete m_adaptativeTree;
}

te::mnt::CalculateGrid::CalculateGrid()
{
  m_tolerance = 0.;
  m_nodatavalue = std::numeric_limits< double >::max();
}

std::auto_ptr<te::rst::Raster> te::mnt::CalculateGrid::Initialize(bool spline, unsigned int &nro_neighb, double &rx1, double &ry2, unsigned int &outputWidth, unsigned int &outputHeight)
{
  te::gm::MultiPoint mpt(0, te::gm::MultiPointZType, m_srid);
  te::gm::MultiLineString isolines_simp(0, te::gm::MultiLineStringZType, m_srid);
  std::string geostype;
  Simplify alg;

  if (spline)
    alg = Spline;
  else
    alg = AccumulatedDistance;

  // Get samples
  size_t nsamples;
  te::gm::Envelope env;

  nsamples = ReadPoints(m_inDsetName, m_inDsrc, m_attrZ, m_tolerance, mpt, geostype, env);
  if (!nsamples)
    nsamples = ReadSamples(m_inDsetName, m_inDsrc, m_attrZ, m_tolerance, m_tolerance, alg, mpt, isolines_simp, geostype, env);
  setEnvelope(env);

  //Create greater box 1/2 resolution
  rx1 = m_env.getLowerLeftX() - m_resx / 2.;
  ry2 = m_env.getUpperRightY() + m_resy / 2.;

  outputWidth = (unsigned int)ceil(m_env.getWidth() / m_resx);
  outputHeight = (unsigned int)ceil(m_env.getHeight() / m_resy);

  te::gm::Coord2D ulc(rx1, ry2);

  te::rst::Grid* grid = new te::rst::Grid(outputWidth, outputHeight, m_resx, m_resy, &ulc, m_srid);

  std::vector<te::rst::BandProperty*> bands;

  bands.push_back(new te::rst::BandProperty(0, te::dt::DOUBLE_TYPE, "DTM GRID"));
  bands[0]->m_nblocksx = 1;
  bands[0]->m_nblocksy = (int)outputHeight;
  bands[0]->m_blkw = (int)outputWidth;
  bands[0]->m_blkh = 1;
  bands[0]->m_colorInterp = te::rst::GrayIdxCInt;
  bands[0]->m_noDataValue = m_nodatavalue;

  // create raster
  std::auto_ptr<te::rst::Raster> rst(te::rst::RasterFactory::make("GDAL", grid, bands, m_dsinfo));
  m_rst = rst.get();

  std::vector<std::pair<te::gm::Coord2D, te::gm::PointZ> > dataset1;

  for (size_t i = 0; i < mpt.getNumGeometries(); i++)
  {
    te::gm::Point* gout = dynamic_cast<te::gm::Point*>(mpt.getGeometryN(i));
    te::gm::PointZ pz(gout->getX(), gout->getY(), gout->getZ());
    te::gm::Coord2D co(gout->getX(), gout->getY());
    m_dataset.push_back(std::pair<te::gm::Coord2D, te::gm::PointZ>(co, pz));
    dataset1.push_back(std::pair<te::gm::Coord2D, te::gm::PointZ>(co, pz));
  }

  std::size_t datasize = m_dataset.size();

  m_adaptativeTree = new KD_ADAPTATIVE_TREE(m_env, THRESHOLD);
  m_adaptativeTree->build(dataset1);

  nro_neighb = THRESHOLD;
  if (datasize < THRESHOLD)
    nro_neighb = (unsigned int)datasize;

  if (m_inter == Vizinho)  //Nearest Neighbor
    nro_neighb = 1;

  return rst;
}

bool te::mnt::CalculateGrid::run()
{
  try
  {
    unsigned int nro_neighb;
    double rx1, ry2;
    unsigned int outputWidth;
    unsigned int outputHeight;

    std::auto_ptr<te::rst::Raster> rst = Initialize(false, nro_neighb, rx1, ry2, outputWidth, outputHeight);

    std::vector<double> distneighb;
    std::vector<te::gm::PointZ> points;
    for (unsigned int t = 0; t < nro_neighb; ++t)
      points.push_back(te::gm::PointZ(std::numeric_limits<double>::max(), std::numeric_limits<double>::max(), std::numeric_limits<double>::max()));
    double zvalue;

    te::common::TaskProgress task("Calculating DTM...", te::common::TaskProgress::UNDEFINED, (int)(outputHeight*outputWidth));

    te::gm::Coord2D pg;
    for (unsigned int l = 0; l < outputHeight; l++)
    {
      for (unsigned int c = 0; c < outputWidth; c++)
      {
        task.pulse();
        te::gm::Coord2D pg(rx1 + (c * m_resx /*+ m_resx / 2.*/), ry2 - (l * m_resy/* + m_resy / 2.*/));
        te::gm::PointZ pgz(pg.getX(), pg.getY(), m_nodatavalue);
        m_adaptativeTree->nearestNeighborSearch(pg, points, distneighb, nro_neighb);

        // Filter elements by raio_max distance
        size_t j = 0;
        for (size_t i = 0; i < points.size(); i++)
        {
          if (distneighb[i] <= (m_radius*m_radius))
          {
            points[j] = points[i];
            distneighb[j] = distneighb[i];
            j++;
          }
        }
        if (j > 0){
          points.resize(j);
          Interpolation(pgz, points, distneighb);
        }

        zvalue = pgz.getZ();
        m_rst->setValue(c, l, zvalue);
      }
    }
    rst.release();
  }
  catch (te::common::Exception& e)
  {
    throw e;
  }

  return true;
}

void te::mnt::CalculateGrid::setInput(te::da::DataSourcePtr inDsrc,
  std::string inDsetName,
  std::auto_ptr<te::da::DataSetType> inDsetType)
{
  m_inDsrc = inDsrc;
  m_inDsetName = inDsetName;
  m_inDsetType = inDsetType;
}

void te::mnt::CalculateGrid::setOutput(std::map<std::string, std::string> &dsinfo)
{
  m_dsinfo = dsinfo;
}

void te::mnt::CalculateGrid::setParams(const std::string &attr, double resx, double resy, Interpolator gt, double rd, int pow)
{
  m_resx = resx;
  m_resy = resy;
  m_inter = gt;
  m_attrZ = attr;
  m_radius = rd;
  m_power = pow;
}

void te::mnt::CalculateGrid::setSRID(int srid)
{
  m_srid = srid;
}

void te::mnt::CalculateGrid::setEnvelope(te::gm::Envelope& env)
{
  if (m_env.getWidth())
    m_env.Union(env);
  else
    m_env = env;
}

void te::mnt::CalculateGrid::Interpolation(te::gm::PointZ& pg, std::vector<te::gm::PointZ>& points, std::vector<double>& distq)
{
  double Ztotal = 0.;
  double Wi, Wtotal = 0.;
  te::gm::PointZ pgtemp;

  if (distq[0] < 1.0e-5)
    pg.setZ(points[0].getZ());
  else
  {
    switch (m_inter)
    {
    case 0:
      Ztotal = Interpwqz(pg, points, distq);
      Wtotal = 1.0;
      break;

    case 1:
      Ztotal = Interpwq(pg, points, distq);
      Wtotal = 1.0;
      break;

    case 2: // Average of Z neighbours values weighted by inverse distance powered by potencia
      for (unsigned int i = 0; i<points.size(); i++){
        Wi = 1. / pow(distq[i], m_power / 2.);
        Ztotal += (points[i].getZ()*Wi);
        Wtotal += Wi;
      }
      break;

    case 3: // Average of Z values of the neighbours 
      for (unsigned int i = 0; i< points.size(); i++){
        Ztotal += points[i].getZ();
        Wtotal += 1.0;
      }
      break;

    case 4: // Nearest neighbour
      Ztotal = points[0].getZ();
      Wtotal = 1.0;
      break;

    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
    default:
      break;
    }

    if (Wtotal > 0.0)
      pg.setZ(Ztotal / Wtotal);
    else
      pg.setZ(m_nodatavalue);
  }

}

double te::mnt::CalculateGrid::Interpwq(te::gm::PointZ& pg, std::vector<te::gm::PointZ>& points, std::vector<double>& distq)
{
  double Ztotal = 0;
  double Wi, Wtotal = 0.0;
  int q1 = 0, q2 = 0, q3 = 0, q4 = 0; // quadrants
  unsigned int  npts = 0;

  for (unsigned int i = 0; i < points.size(); i++)
  {
    //Filter the point by quadrant
    if ((q1<1) && (points[i].getX() > pg.getX()) && (points[i].getY() > pg.getY()))
      q1++;
    else
      if ((q2 < 1) && (points[i].getX() > pg.getX()) && (points[i].getY()<pg.getY()))
        q2++;
      else
        if ((q3 < 1) && (points[i].getX() < pg.getX()) && (points[i].getY() < pg.getY()))
          q3++;
        else
          if ((q4 < 1) && (points[i].getX() < pg.getX()) && (points[i].getY() > pg.getY()))
            q4++;
          else continue;

          points[npts] = points[i];
          npts++;
  }

  for (unsigned int i = 0; i<npts; i++)
  {
    Wi = 1. / pow(distq[i], (double)m_power / 2.);
    Ztotal += (points[i].getZ()*Wi);
    Wtotal += Wi;
  }

  if (Wtotal >0.0)
    return(Ztotal / Wtotal);
  else
    return (m_nodatavalue);
}

double te::mnt::CalculateGrid::Interpwqz(te::gm::PointZ& pg, std::vector<te::gm::PointZ>& points, std::vector<double>& distq)
{
  double Ztotal = 0;
  double Wi, Wtotal = 0;
  int q1 = 0, q2 = 0, q3 = 0, q4 = 0; // quadrants
  int j;

  int use_point = 1;
  for (unsigned int i = 0; i < points.size(); i++)
  {
    for (j = (int)i - 1; j >= 0; j--)
      if (points[(unsigned int)j].getZ() == points[i].getZ())
        break; // Points with equal Z coordinates

    use_point = 1; // Use the point
    if ((i == 0) || (j == -1)) // did not find an equal value
    {
      //          Filter the point by quadrants
      if ((q1 < 1) && (points[i].getX() > pg.getX()) && (points[i].getY() > pg.getY()))
        q1++;
      else
        if ((q2 < 1) && (points[i].getX() > pg.getX()) && (points[i].getY() < pg.getY()))
          q2++;
        else
          if ((q3 < 1) && (points[i].getX() < pg.getX()) && (points[i].getY() < pg.getY()))
            q3++;
          else
            if ((q4 < 1) && (points[i].getX() < pg.getX()) && (points[i].getY() > pg.getY()))
              q4++;
            else
              use_point = 0; // do not use the point

      if (use_point == 1)
      {
        Wi = 1. / pow(distq[i], m_power / 2.);
        Ztotal += (points[i].getZ()*Wi);
        Wtotal += Wi;
      }
    }
  }

  if (Wtotal > 0.0)
    return(Ztotal / Wtotal);
  else
    return (m_nodatavalue);
}

bool te::mnt::CalculateGrid::GenerateGridSplineGrass()
{
  return true;
}

bool te::mnt::CalculateGrid::GenerateGridSplineMitasova()
{
  return true;
}

