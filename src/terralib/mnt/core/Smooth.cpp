/*!
\file terralib/mnt/core/Smooth.cpp

\brief This file contains a class to isolines smooth.

*/

#include "Smooth.h"
#include "Utils.h"

//terralib
#include "../../dataaccess/utils/Utils.h"
#include "../../geometry/GeometryProperty.h"
#include "../../srs/SpatialReferenceSystemManager.h"

te::mnt::Smooth::Smooth()
{
}

te::mnt::Smooth::~Smooth()
{
}

void te::mnt::Smooth::setInput(te::da::DataSourcePtr inDsrc,
  std::string inDsetName,
  std::auto_ptr<te::da::DataSetType> inDsetType)
{
  m_inDsrc = inDsrc;
  m_inDsetName = inDsetName;
  m_inDsetType = inDsetType;

}

void te::mnt::Smooth::setOutput(te::da::DataSourcePtr outDsrc, std::string dsname)
{
  m_outDsrc = outDsrc;
  m_outDsetName = dsname;
}

void te::mnt::Smooth::setParams(double factor, double maxdist, bool simpl_out, std::string &attr)
{
  m_factor = factor;
  m_maxdist = maxdist;
  m_simpl_out = simpl_out;
  m_Zattr = attr;
}

bool te::mnt::Smooth::run()
{
  te::gm::MultiPoint mpt(0, te::gm::MultiPointZType, m_srid);
  te::gm::MultiLineString isolines_simp(0, te::gm::MultiLineStringZType, m_srid);
  std::vector<te::gm::LineString> isolines_suavizado;

  std::string geostype;
  size_t nsamples;
  te::gm::Envelope env;

  nsamples = ReadSamples(m_inDsetName, m_inDsrc, m_Zattr, m_factor, m_maxdist, DouglasPeucker, mpt, isolines_simp, geostype, env);

  for (size_t i = 0; i < isolines_simp.getNumGeometries(); i++)
  {
    te::gm::LineString iso = *dynamic_cast<te::gm::LineString*>(isolines_simp.getGeometryN(i));
    if (AdjustCatmullRom(iso, m_maxdist))
    {
      if (m_simpl_out)
      {
        te::gm::LineString *iso_sim = pointListSimplify(&iso, m_factor, m_maxdist, iso.getZ(0));
        isolines_suavizado.push_back(*iso_sim);
      }
      else
        isolines_suavizado.push_back(iso);
    }
  }

  std::vector<double> guidevalues;
  SaveIso(m_outDsetName, m_outDsrc, isolines_suavizado, guidevalues, m_srid);

  return true;
}

bool te::mnt::Smooth::AdjustCatmullRom(te::gm::LineString &iso, double snap)
{
  size_t npts = iso.getNPoints();
  double valuez = iso.getZ(0);
  if (npts < 3)
    return false;

  te::gm::Coord2D *vxy = iso.getCoordinates();

  te::gm::Coord2D xyA, xyB, xyC, xyD;
  te::gm::Coord2D pxy;

  int32_t ipts = 0;

  xyA = vxy[ipts];
  xyB = vxy[ipts];
  ipts++;
  xyC = vxy[ipts];

  std::vector<te::gm::Coord2D> vab;
  std::vector<te::gm::Coord2D> vbc;
  std::vector<te::gm::Coord2D> isout;

  short getNext = 1;
  short repeatD = 0;
  while (ipts < npts)
  {
    if (getNext)
    {
      if (repeatD)
        repeatD = 0;
      else
        ipts++;

      if (ipts < npts)
        xyD = vxy[ipts];
      else
        xyD = xyC;
    }
    else
      getNext = 1;

    double a3 = (-xyA.getX() + 3 * (xyB.getX() - xyC.getX()) + xyD.getX()) / 2;
    double a2 = (2 * xyA.getX() - 5 * xyB.getX() + 4 * xyC.getX() - xyD.getX()) / 2;
    double a1 = (xyC.getX() - xyA.getX()) / 2;
    double a0 = xyB.getX();

    double b3 = (-xyA.getY() + 3 * (xyB.getY() - xyC.getY()) + xyD.getY()) / 2;
    double b2 = (2 * xyA.getY() - 5 * xyB.getY() + 4 * xyC.getY() - xyD.getY()) / 2;
    double b1 = (xyC.getY() - xyA.getY()) / 2;
    double b0 = xyB.getY();

    double dx = xyC.getX() - xyB.getX();
    double dy = xyC.getY() - xyB.getY();

    double stsize = sqrt(dx * dx + dy * dy);
    int nsteps = (int)(stsize / m_maxdist);

    for (int j = 0; j < nsteps; j++)
    {
      double t = (double)j / (double)nsteps;

      double x = ((a3 * t + a2) * t + a1) * t + a0;
      double y = ((b3 * t + b2) * t + b1) * t + b0;

      if (j)
      {
        double dx1 = pxy.getX() - x;
        double dy1 = pxy.getY() - y;
        double dx1B = xyA.getX() - x;
        double dy1B = xyA.getY() - y;
        double dx1C = xyC.getX() - x;
        double dy1C = xyC.getY() - y;

        double segSize = sqrt(dx1 * dx1 + dy1 * dy1);
        double segSizeB = sqrt(dx1B * dx1B + dy1B * dy1B);
        double segSizeC = sqrt(dx1C * dx1C + dy1C * dy1C);
        double cost = (dx * dx1 + dy * dy1) / (stsize * segSize);
        double costB = (dx * dx1B + dy * dy1B) / (stsize * segSizeB);
        double costC = (dx * dx1C + dy * dy1C) / (stsize * segSizeC);

        if (fabs(cost) < 0.7 /*|| (((costB > 0 && costC > 0) || (costB < 0 && costC < 0))/* && j > nsteps/2)*/)
        {
          repeatD = 1;
          getNext = 0;

          vbc.clear();

          te::gm::Coord2D pt0, pt1;

          //Finds longest segment
          double segsizeAB = te::mnt::Distance(xyA, xyB);
          double segsizeBC = te::mnt::Distance(xyB, xyC);
          double segsizeCD = te::mnt::Distance(xyC, xyD);

          if ((segsizeAB >= segsizeBC) && (segsizeAB >= segsizeCD))
          {
            //Uses AB to find middle point, uses C for opposite side
            middlePointWithSnap(xyA, xyB, pt0, pt1, m_maxdist);

            if (te::mnt::onSameSide(xyC, pt0, xyA, xyB) == 1)
              xyA = pt1;
            else
              xyA = pt0;
          }
          else if ((segsizeBC >= segsizeAB) && (segsizeBC >= segsizeCD))
          {
            //Uses BC to find middle point, uses D for opposite side
            middlePointWithSnap(xyB, xyC, pt0, pt1, m_maxdist);

            xyD = xyC;

            if (te::mnt::onSameSide(xyD, pt0, xyB, xyC) == 1)
              xyC = pt1;
            else
              xyC = pt0;
          }
          else
          {
            // Uses CD to find middle point, uses B for opposite side
            middlePointWithSnap(xyC, xyD, pt0, pt1, m_maxdist);

            if (te::mnt::onSameSide(xyB, pt0, xyC, xyD) == 1)
              xyD = pt1;
            else
              xyD = pt0;
          }

          break;
        }
      }

      pxy.x = x;
      pxy.y = y;
      vbc.push_back(pxy);
    }

    if ((!nsteps) || (ipts == npts))
    {
      double x = a3 + a2 + a1 + a0;
      double y = b3 + b2 + b1 + b0;

      pxy.x = x;
      pxy.y = y;
      vbc.push_back(pxy);

      if ((!nsteps) && (getNext))
      {
        xyA = xyB;
        xyB = xyC;
        xyC = xyD;
      }
    }
    else if (getNext)
    {
      xyA = xyB;
      xyB = xyC;
      xyC = xyD;
    }

    if (vab.size() > 0)
    {
      for (size_t i = 0; i < vab.size(); i++)
        isout.push_back(vab[i]);
      vab.clear();
    }

    if (ipts == npts)
    {
      if (vbc.size() > 0)
      {
        for (size_t i = 0; i < vbc.size(); i++)
          isout.push_back(vbc[i]);
        vbc.clear();
      }
    }
    else
    {
      if (vbc.size() > 0)
      {
        for (size_t i = 0; i < vbc.size(); i++)
          vab.push_back(vbc[i]);
        vbc.clear();
      }
    }
  }

  iso.makeEmpty();
  iso.setNumCoordinates(isout.size());
  for (size_t i = 0; i < isout.size(); i++)
  {
    iso.setPointZ(i, isout[i].getX(), isout[i].getY(), valuez);
  }
  isout.clear();

  return true;
}


bool te::mnt::Smooth::middlePointWithSnap(te::gm::Coord2D &pfr, te::gm::Coord2D &pto, te::gm::Coord2D &pt0, te::gm::Coord2D &pt1, double d)
{
  //Calculates middle point
  double a = pto.getX() - pfr.getX();
  double b = pto.getY() - pfr.getY();

  double x = (a / 2) + pfr.getX();
  double y = (b / 2) + pfr.getY();

  te::gm::Coord2D ptm(x, y);

  if (a == 0)
  {
    pt0.x = ptm.getX() + d;
    pt0.y = ptm.getY();
    pt1.x = ptm.getX() - d;
    pt1.y = ptm.getY();

    return true;
  }

  if (b == 0)
  {
    pt0.x = ptm.getX();
    pt0.y = ptm.getY() + d;
    pt1.x = ptm.getX();
    pt1.y = ptm.getY() - d;

    return true;
  }

  double bx = pfr.getX() - ptm.getX();
  double by = pfr.getY() - ptm.getY();

  double c = -(bx / by);

  double x0 = sqrt((d * d) / (1 + (c * c)));
  double x1 = -x0;
  double y0 = c * x0;
  double y1 = c * x1;

  pt0.x = +ptm.getX();
  pt0.y = y0 + ptm.getY();
  pt1.x = x1 + ptm.getX();
  pt1.y = y1 + ptm.getY();

  return true;
}
