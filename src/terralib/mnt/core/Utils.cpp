
/*!
\file terralib/mnt/core/Utils.cpp

\brief Utility functions for MNT support.
*/

#include "Utils.h"
#include "CalculateGrid.h"

#include "../../dataaccess/utils/Utils.h"
#include "../../geometry/Point.h"

#include <cmath>
#include <limits>
#include <stdint.h>


size_t te::mnt::ReadPoints(std::string &inDsetName, te::da::DataSourcePtr &inDsrc, std::string &atrZ, double tol, 
  te::gm::MultiPoint &mpt, std::string &geostype, te::gm::Envelope &env)
{
  if (inDsetName.empty())
    return 0;

  std::auto_ptr<te::da::DataSet> inDset;
  size_t nsamples = 0;

  inDset = inDsrc->getDataSet(inDsetName);

  std::size_t geo_pos = te::da::GetFirstPropertyPos(inDset.get(), te::dt::GEOMETRY_TYPE);

  inDset->moveBeforeFirst();
  std::size_t pos = 0;
  double value;
  while (inDset->moveNext())
  {
    std::auto_ptr<te::gm::Geometry> gin = inDset->getGeometry(geo_pos);
    geostype = gin.get()->getGeometryType();

    if (geostype == "MultiPoint")
    {
      te::gm::MultiPoint *g = dynamic_cast<te::gm::MultiPoint*>(gin.get());
      std::size_t np = g->getNumGeometries();
      if (!atrZ.empty())
        value = inDset->getDouble(atrZ);
      for (std::size_t i = 0; i < np; ++i)
      {
        te::gm::Point *p = dynamic_cast<te::gm::Point*>(g->getGeometryN(i));
        if (atrZ.empty())
          value = p->getZ();
        te::gm::PointZ pz(p->getX(), p->getY(), value);
        mpt.add(dynamic_cast<te::gm::Geometry*>(pz.clone()));
        nsamples++;
      }
    }
    else if (geostype == "Point")
    {
      te::gm::Point *p = dynamic_cast<te::gm::Point*>(gin.get());
      if (atrZ.empty())
        value = p->getZ();
      else
        value = inDset->getDouble(atrZ);

      te::gm::PointZ pz(p->getX(), p->getY(), value);
      mpt.add(dynamic_cast<te::gm::Geometry*>(pz.clone()));
      nsamples++;
    }
    else
      return 0;
  }

  std::auto_ptr<te::gm::Envelope> e = inDset->getExtent(geo_pos);
  env.init((e->getLowerLeftX() - tol), (e->getLowerLeftY() - tol), (e->getUpperRightX() + tol), (e->getUpperRightY() + tol));

  return nsamples;
}

size_t te::mnt::ReadSamples(std::string &inDsetName, te::da::DataSourcePtr &inDsrc, std::string &atrZ, double tol, double max, bool spline,
  te::gm::MultiPoint &mpt, te::gm::MultiLineString &isolines, std::string &geostype, te::gm::Envelope &env)
{

  if (inDsetName.empty())
    return 0;

  std::auto_ptr<te::da::DataSet> inDset;
  size_t nsamples = mpt.getNumGeometries();

  inDset = inDsrc->getDataSet(inDsetName);

  const std::size_t np = inDset->getNumProperties();
  const std::size_t ng = inDset->size();

  //Read attributes
  std::vector<std::string>pnames;
  std::vector<int> ptypes;
  for (std::size_t i = 0; i != np; ++i)
  {
    pnames.push_back(inDset->getPropertyName(i));
    ptypes.push_back(inDset->getPropertyDataType(i));
  }

  std::size_t geo_pos = te::da::GetFirstPropertyPos(inDset.get(), te::dt::GEOMETRY_TYPE);

  inDset->moveBeforeFirst();
  std::size_t pos = 0;
  double value;

  while (inDset->moveNext())
  {
    std::auto_ptr<te::gm::Geometry> gin = inDset->getGeometry(geo_pos);
    geostype = gin.get()->getGeometryType();

    if (geostype == "LineString")
    {
      te::gm::LineString *l = dynamic_cast<te::gm::LineString*>(gin.get());
      if (atrZ.empty())
        value = *l->getZ();
      else
        value = inDset->getDouble(atrZ);

      te::gm::LineString *ls;
      if (spline)
        ls = te::mnt::SplineInterpolationGrass::pointListSimplify(l, tol, max, value);
      else
        ls = pointListSimplify(l, tol, max, value); 
      isolines.add(dynamic_cast<te::gm::Geometry*>(ls));
      for (std::size_t p = 0; p < ls->size(); p++)
      {
        mpt.add(ls->getPointN(p));
      }
      nsamples += ls->size();
    }
    if (geostype == "MultiLineString")
    {
      te::gm::MultiLineString *g = dynamic_cast<te::gm::MultiLineString*>(gin.get());
      std::size_t np = g->getNumGeometries();
      if (!atrZ.empty())
        value = inDset->getDouble(atrZ);
      for (std::size_t i = 0; i < np; ++i)
      {
        te::gm::LineString *l = dynamic_cast<te::gm::LineString*>(g->getGeometryN(i));
        te::gm::LineString *lz = new te::gm::LineString(l->size(), te::gm::LineStringZType, isolines.getSRID());
        if (atrZ.empty())
          value = *l->getZ();

        for (std::size_t il = 0; il < l->size(); il++)
          lz->setPointZ(il, l->getX(il), l->getY(il), value);
        l->setSRID(isolines.getSRID());
        te::gm::LineString *ls;
        if (spline)
          ls = te::mnt::SplineInterpolationGrass::pointListSimplify(l, tol, max, value);
        else
          ls = pointListSimplify(l, tol, max, value);
        if (ls->size())
        {
          isolines.add(dynamic_cast<te::gm::Geometry*>(ls));
          nsamples += ls->size();

          for (std::size_t p = 0; p < ls->size(); p++)
          {
            mpt.add(ls->getPointN(p));
          }
        }
      }
    }
  }

  std::auto_ptr<te::gm::Envelope> e = inDset->getExtent(geo_pos);
  env.init((e->getLowerLeftX() - tol), (e->getLowerLeftY() - tol), (e->getUpperRightX() + tol), (e->getUpperRightY() + tol));

  return nsamples;
}

double te::mnt::Distance(const te::gm::Coord2D &pt1, const te::gm::Coord2D &pt2)
{
  double       dx,          /* Diferenca dos X's do ponto a e b        */
    dy,          /* Diferenca dos Y's do ponto a e b        */
    dist;        /* Distancia dos pontos                    */

  dx = pt1.getX() - pt2.getX();
  dx = dx * dx;
  dy = pt1.getY() - pt2.getY();
  dy = dy * dy;
  dist = sqrt(dy + dx);
  return(dist);
}


te::gm::Point te::mnt::Max(te::gm::Point& p1, te::gm::Point& p2) 
{
  return te::gm::Point(MAX(p1.getX(), p2.getX()), MAX(p1.getY(), p2.getY()));
}

te::gm::Point te::mnt::Min(te::gm::Point& p1, te::gm::Point& p2)
{
  return te::gm::Point(MIN(p1.getX(), p2.getX()), MIN(p1.getY(), p2.getY()));
}


bool te::mnt::Equal(te::gm::PointZ &p1, te::gm::PointZ &p2, double &tol)
{
  return (std::fabs(p1.getX() - p2.getX()) < tol && std::fabs(p1.getY() - p2.getY()) < tol && std::fabs(p1.getZ() - p2.getZ()) < tol);
}

bool te::mnt::equalFptSpt(te::gm::PointZ & fpt, te::gm::PointZ &spt, double scale)
{
  double  delta = (double)(scale/* * 0.04 / 1000.*/); // point one percent of map tolerance

  if ((fpt.getX() == spt.getX()) && (fpt.getY() == spt.getY())) return true;
  if (fabs(fpt.getX() - spt.getX()) > delta) return false;
  if (fabs(fpt.getY() - spt.getY()) < delta) return true;
  return false;
}

te::gm::LineString* te::mnt::pointListSimplify(te::gm::LineString *ls, double snap, double maxdist, double Zvalue)
{
  size_t npts = ls->getNPoints();
  size_t npte = npts;

  te::gm::Coord2D* vxy = ls->getCoordinates();

  //  If line is too short do nothing
  if (npts <= 3 || snap == 0.)
  {
    te::gm::LineString *ptlist = new te::gm::LineString(npts, te::gm::LineStringZType);
    for (size_t i = 0; i < npts; i++)
    {
      ptlist->setPointZ(i, vxy[i].getX(), vxy[i].getY(), Zvalue);
    }
    return ptlist;
  }

  //  snap2 = 400 * snap*snap;
  double snap2 = maxdist*maxdist;

  size_t npt;
  bool island = false;
  //  Check for islands before defining number of points to be used
  if (vxy[0] == vxy[npte - 1])
  {
    npt = npte - 1;
    island = true;
  }
  else
    npt = npte;

  //  initialize variables
  size_t i = 0;
  size_t numa = 0;
  size_t numpf = npt - 1;

  //  define anchor
  te::gm::Coord2D axy = vxy[numa];

  //  define floating point
  te::gm::Coord2D pfxy = vxy[numpf];

  bool retv;
  double dmax, d,
    a = 0, b = 0,
    aa1 = 0;
  size_t numdmax;

  while (numa < (npt - 1))
  {
    //    Compute coeficients of straight line y=ax+b
    if (pfxy.getX() == axy.getX())
      retv = true;
    else
    {
      retv = false;
      a = (pfxy.getY() - axy.getY()) / (pfxy.getX() - axy.getX());
      b = pfxy.getY() - a * pfxy.getX();
      aa1 = sqrt(a * a + 1.);
    }

    dmax = 0;
    numdmax = numpf;
    size_t k;

    for (k = numa + 1; k < numpf; k++)
    {
      //      Distance between point and line
      if (retv)
        d = fabs(axy.getX() - vxy[k].getX());
      else
        d = fabs(vxy[k].getY() - a*vxy[k].getX() - b) / aa1;

      if (d > dmax)
      {
        dmax = d;
        numdmax = k;
      }
    }

    if (dmax <= snap)
    {
      //      Store selected point
      if (i > (npt - 1))
      {
        throw;
      }
      vxy[i++] = axy;

      double axbx = pfxy.getX() - axy.getX();
      double ayby = pfxy.getY() - axy.getY();
      double dist2 = axbx*axbx + ayby*ayby;
      if (dist2 > snap2)
      {
        for (k = numpf; k > numa + 1; k--)
        {
          axbx = vxy[k].getX() - axy.getX();
          ayby = vxy[k].getY() - axy.getY();
          dist2 = axbx*axbx + ayby*ayby;
          if (dist2 < snap2)
            break;
        }
        //        Shift anchor
        numa = k;
        axy = vxy[k];
      }
      else
      {
        //        Shift anchor
        numa = numpf;
        axy = vxy[numpf];
      }
      numpf = npt - 1;
    }
    else
      //      Shift floating point
      numpf = numdmax;

    pfxy = vxy[numpf];
  }

  //  Store results
  vxy[i] = vxy[numa];
  npts = i + 1;

  if ((Distance(vxy[i], vxy[i - 1]) < snap) || (Distance(vxy[i], vxy[0]) < snap))
    npts = i;

  size_t newpts;
  if (island)
    newpts = npts + 1;
  else
    newpts = npts;

  te::gm::LineString *ptlist = new te::gm::LineString(newpts, te::gm::LineStringZType, ls->getSRID());

  for (i = 0; i < npts; i++)
  {
    ptlist->setPointZ(i, vxy[i].getX(), vxy[i].getY(), Zvalue);
  }

  if (island)
    ptlist->setPointZ(npts, vxy[0].getX(), vxy[0].getY(), Zvalue);

  return ptlist;
}

/*!
\brief Function that evaluates the distance between a point and a segment
\param fseg is a pointer to the segment first point
\param lseg is a pointer to the segment last point
\param pt is a pointer to the considered point
\param pti is a pointer to the intersection point
\return the distance between the point and the segment
*/
double te::mnt::pointToSegmentDistance(te::gm::PointZ &fseg, te::gm::PointZ &lseg, te::gm::PointZ &pt, te::gm::PointZ *pti)
{
  double ux = lseg.getX() - fseg.getX();
  double uy = lseg.getY() - fseg.getY();
  double vx = pt.getX() - fseg.getX();
  double vy = pt.getY() - fseg.getY();
  double uv = (ux*vx + uy*vy);
  if (uv < 0.)
  {
    if (pti)
    {
      pti->setX(lseg.getX());
      pti->setY(lseg.getY());
    }
    return (sqrt(vx*vx + vy*vy));
  }
  else
  {
    ux = fseg.getX() - lseg.getX();
    uy = fseg.getY() - lseg.getY();
    vx = pt.getX() - lseg.getX();
    vy = pt.getY() - lseg.getY();
    uv = (ux*vx + uy*vy);
    if (uv < 0.)
    {
      if (pti)
      {
        pti->setX(lseg.getX());
        pti->setY(lseg.getY());
      }
      return (sqrt(vx*vx + vy*vy));
    }
  }

  double  a, b, c, k, dist, aabb;

  a = lseg.getY() - fseg.getY();
  b = fseg.getX() - lseg.getX();
  c = lseg.getX()*fseg.getY() - fseg.getX()*lseg.getY();
  aabb = a*a + b*b;

  dist = fabs((a*pt.getX() + b*pt.getY() + c) / (sqrt(aabb)));
  if (pti)
  {
    k = b*pt.getX() - a*pt.getY();
    pti->setX((b*k - a*c) / aabb);
    pti->setY((-a*k - b*c) / aabb);
  }
  return dist;
}

/*!
\brief Function that finds the center of the vertices of a triangle
\param vert is pointer to the triangle vertices
\param pcx and pcy are pointers to coordinates x and y of the vertex center points
\return TRUE if the centers are evaluated with no errors or FALSE otherwise
*/

short te::mnt::findCenter(te::gm::PointZ* vert, double* pcx, double* pcy)
{
  double  m1 = 10., m2 = 10.;  // normais aos segmentos 1 e 2 
  double  xm1, ym1,     // coords do ponto medio do segm. 1 
    xm2, ym2,    // coords do ponto medio do segm. 2 
    xaux, yaux,
    px[3], py[3];
  bool  perpvert1 = false,  // perpendicular vertical ao segmento 1 
    perpvert2 = false;  // perpendicular vertical ao segmento 2 
  short  i;

  for (i = 0; i < 3; i++)
  {
    px[i] = vert[i].getX();
    py[i] = vert[i].getY();
  }

  //  calcula o coeficiente angular perpendicular a reta 1
  if ((float)py[0] == (float)py[1])
  {
    if ((float)px[0] == (float)px[1])
    {
      //      pontos sao iguais-> retorna
      return false;
    }
    perpvert1 = true;
  }
  else
    m1 = -(px[1] - px[0]) / (py[1] - py[0]);

  //  calcula o coeficiente angular da perpendicular a reta 2
  if ((float)py[1] == (float)py[2])
  {
    if ((float)px[1] == (float)px[2])
    {
      //      pontos sao iguais-> retorna
      return false;
    }
    perpvert2 = false;
  }
  else
    m2 = -(px[2] - px[1]) / (py[2] - py[1]);

  //  Caso as retas sejam coincidentes, uma circunferencia 
  //   nao eh definida
  if ((float)m1 == (float)m2)
  {
    return false;
  }

  //  passou pelos testes: os pontos definem uma circunferencia
  //  calculo do ponto medio do segmento ponto0-ponto1 (segmento 1)
  xm1 = (px[0] + px[1]) / 2.;
  ym1 = (py[0] + py[1]) / 2.;

  //  calculo do ponto medio do segmento ponto1-ponto2 (segmento 2)
  xm2 = (px[1] + px[2]) / 2.;
  ym2 = (py[1] + py[2]) / 2.;

  //  calculo das coordenadas do centro: ponto de interseccao das mediatrizes
  //   dos segmentos ponto0-ponto1 e ponto1-ponto2
  if (perpvert1 == true)
  {
    xaux = xm1;
    yaux = (ym2 + m2 * (xaux - xm2));
  }
  else if (perpvert2 == true)
  {
    xaux = xm2;
    yaux = (ym1 + m1 * (xaux - xm1));
  }
  else
  {
    xaux = (m1*xm1 - m2*xm2 - ym1 + ym2) / (m1 - m2);
    yaux = (ym1 + m1 * (xaux - xm1));
  }
  *pcx = xaux;
  *pcy = yaux;

  return true;
}
/*!
\brief Function for check segment intersection
\param pfr, pto, lfr, lto are pointers to Point objects
\return point of intersection
*/
short te::mnt::segIntersect(te::gm::PointZ &pfr, te::gm::PointZ &pto, te::gm::PointZ &lfr, te::gm::PointZ &lto)
{
  te::gm::PointZ  pt;

  return segInterPoint(pfr, pto, lfr, lto, &pt);
}

/*!
\brief Function for determines the point of two segment intersections
\param pfr, pto, lfr, lto and pt are pointers to Point objects
\return TRUE if the intersection was calculated with no errors or FALSE otherwise
*/

bool te::mnt::segInterPoint(te::gm::PointZ &pfr, te::gm::PointZ &pto, te::gm::PointZ &lfr, te::gm::PointZ &lto, te::gm::PointZ *pt)
{
  //  Adapted from TWO-DIMENSIONAL CLIPPING: A VECTOR-BASED APPROACH
  //  Hans J.W. Spoelder, Fons H. Ullings in:
  ////  Graphics Gems I, pp.701, 

  double  a, b, c,
    px, py, lx, ly, lpx, lpy, s;

  px = pto.getX() - pfr.getX();
  py = pto.getY() - pfr.getY();
  lx = lto.getX() - lfr.getX();
  ly = lto.getY() - lfr.getY();
  lpx = pfr.getX() - lfr.getX();
  lpy = pfr.getY() - lfr.getY();

  a = py * lx - px * ly;
  b = lpx * ly - lpy * lx;
  c = lpx * py - lpy * px;

  if (a == 0) // Linhas paralelas
    return false;
  else
  {
    if (a > 0)
    {
      if ((b < 0) || (b > a) || (c < 0) || (c > a))
        return false;
    }
    else
    {
      if ((b > 0) || (b < a) || (c > 0) || (c < a))
        return false;
    }
    if (pt)
    {
      s = b / a;
      pt->setX(pfr.getX() + (px*s));
      pt->setY(pfr.getY() + (py*s));
    }
  }
  return true;
}


/*!
\brief Function that test the vertex values
\param isolin is the isolin value of a given isolin
\param p3da is a pointer to a vector with Point3d objects
\return TRUE if the vertex values are in the same isolin or FALSE otherwise
*/
bool te::mnt::testVertexValues(double isolin, te::gm::PointZ *p3da)
{
  double  delta = .9999;
  size_t  i;

  for (i = 0; i < 3; i++)
    if (p3da[i].getZ() == isolin)
      p3da[i].setZ(isolin*delta);


  if ((p3da[0].getZ() == std::numeric_limits<double>::max()) ||
    (p3da[1].getZ() == std::numeric_limits<double>::max()) ||
    (p3da[2].getZ() == std::numeric_limits<double>::max()))
    return false;

  if ((p3da[0].getZ() < isolin) &&
    (p3da[1].getZ() < isolin) &&
    (p3da[2].getZ() < isolin))
    return false;

  if ((p3da[0].getZ() > isolin) &&
    (p3da[1].getZ() > isolin) &&
    (p3da[2].getZ() > isolin))
    return false;

  for (i = 0; i < 3; i++)
  {
    if ((p3da[i].getZ() == isolin) &&
      (p3da[(i + 1) % 3].getZ() > isolin) &&
      (p3da[(i + 2) % 3].getZ() > isolin))
      return false;
    if ((p3da[i].getZ() == isolin) &&
      (p3da[(i + 1) % 3].getZ() < isolin) &&
      (p3da[(i + 2) % 3].getZ() < isolin))
      return false;
  }

  return true;
}


/*!
\brief Function that defines the intersections between an isoline and a triangle
\param ptline is a pointer to a Line object
\param p3da is a pointer to a Point3d object
\return TRUE if the intersections are defined or FALSE otherwise
*/

bool te::mnt::defineInterTriangle(std::vector<te::gm::PointZ> &ptline, te::gm::PointZ *p3da, double isolin)
{
  size_t contrl = 0;

  // Verify if contour intercepts triangle
  if ((isolin < p3da[0].getZ()) && (isolin < p3da[1].getZ()) &&
    (isolin < p3da[2].getZ()))
    return false;

  if ((isolin > p3da[0].getZ()) && (isolin > p3da[1].getZ()) &&
    (isolin > p3da[2].getZ()))
    return false;

  if (((isolin <= p3da[0].getZ()) && (isolin > p3da[1].getZ())) ||
    ((isolin > p3da[0].getZ()) && (isolin <= p3da[1].getZ())))
  {// Calculate intersection with edge 0
    if (!defineInterEdge(ptline, p3da[0], p3da[1], isolin))
      return false;
    contrl++;
  }

  if (((isolin <= p3da[1].getZ()) && (isolin > p3da[2].getZ())) ||
    ((isolin > p3da[1].getZ()) && (isolin <= p3da[2].getZ())))
  {// Calculate intersection with edge 0
    if (!defineInterEdge(ptline, p3da[1], p3da[2], isolin))
      return false;
    contrl++;
  }

  if (((isolin <= p3da[2].getZ()) && (isolin > p3da[0].getZ())) ||
    ((isolin > p3da[2].getZ()) && (isolin <= p3da[0].getZ())))
  {// Calculate intersection with edge 0
    if (!defineInterEdge(ptline, p3da[2], p3da[0], isolin))
      return false;
    contrl++;
  }
  if (contrl != 2)
    return false;

  return true;
}


/*!
\brief Function that defines the intersection with an edge of a triangle
\param ptline is a pointer to a Line object
\param fpt and spt are the first and second point of the edge
\return TRUE if the intersection is defined or FALSE otherwise
*/

bool te::mnt::defineInterEdge(std::vector<te::gm::PointZ> &ptline, te::gm::PointZ &fpt, te::gm::PointZ &spt, double isolin)
{
  te::gm::PointZ dvect; // Vector from fpt to spt
  te::gm::PointZ pt;
  double  paux;// Parametric variable

  if (spt.getZ() == fpt.getZ())
    return false;

  dvect.setX(spt.getX() - fpt.getX());
  dvect.setY(spt.getY() - fpt.getY());
  dvect.setZ(spt.getZ() - fpt.getZ());

  paux = (isolin - fpt.getZ()) / dvect.getZ(); // t = (Z - Z0) / C

  pt.setX((fpt.getX() + dvect.getX() * paux)); // X = X0 + A * t
  pt.setY((fpt.getY() + dvect.getY() * paux)); // Y = Y0 + B * t
  pt.setZ(isolin);

  ptline.push_back(pt);
  return true;
}

#include <fstream>      // std::ofstream

bool te::mnt::extractLines(std::vector<te::gm::PointZ> &pline, std::vector<te::gm::LineString> &clinlist, double scale)
{
  std::vector<te::gm::PointZ> velin;
  te::gm::PointZ pt;

  std::vector<te::gm::PointZ>::iterator it, itf, itn;
  short borda; // borda = 1 no boundary reached, 2 one boundary reached.

  std::ofstream ofs("d:\\teste\\Extract.txt", std::ofstream::out | std::ofstream::app);
  ofs.precision(8);

  initLineVector(pline, velin);
  borda = 1;
  ofs << "velin.size " << velin.size() << std::endl;
  for (size_t vv = 0; vv < velin.size(); vv++)
    ofs << velin[vv].getX() << " " << velin[vv].getY() << std::endl;

  //index pindex to the current point of old line
  it = itf = pline.begin();
  itn = ++it;
  te::gm::PointZ ptf(*itf);
  te::gm::PointZ ptn(*itn);
  ofs << "pline.size " << pline.size() << std::endl;
  while (it != pline.end())
  {
    ptf = *itf;
    ptn = *itn;
    pt = velin[velin.size()-1];
    ofs << "ptf " << ptf.getX() << " " << ptf.getY() << " ptn " << ptn.getX() << " " << ptn.getY() << " pt " << pt.getX() << " " << pt.getY() << std::endl;
    if (equalFptSpt(ptf, pt, scale))
    {// conection on ptf
      velin.push_back(ptn);
      --it;
      it = itf = pline.erase(it);
      itn = pline.erase(it);
      ofs << "velin.size " << velin.size() << std::endl;
      for (size_t vv = 0; vv < velin.size(); vv++)
        ofs << velin[vv].getX() << " " << velin[vv].getY() << std::endl;
    }
    else if (equalFptSpt(ptn, pt, scale))
    {// conection on ptn
      velin.push_back(ptf);
      --it;
      it = itf = pline.erase(it);
      itn = pline.erase(it);
      ofs << "velin.size " << velin.size() << std::endl;
      for (size_t vv = 0; vv < velin.size(); vv++)
        ofs << velin[vv].getX() << " " << velin[vv].getY() << std::endl;
    }
    else
    {
      itf = ++it;;
      if (it != pline.end())
        itn = ++it;
      if (itf == pline.end() || itn == pline.end())
      {// There are no points to match
        if (borda == 2)
        {// If first point is on boundary -> open contour
          assembLine(clinlist, velin);
          initLineVector(pline, velin); //SSL0296
          ofs << "velin.size " << velin.size() << std::endl;
          for (size_t vv = 0; vv < velin.size(); vv++)
            ofs << velin[vv].getX() << " " << velin[vv].getY() << std::endl;
          borda = 1;
        }
        else
        {// If not on boundary, invert line points
          std::reverse(velin.begin(), velin.end());
          ofs << " Reverse velin.size " << velin.size() << std::endl;
          for (size_t vv = 0; vv < velin.size(); vv++)
            ofs << velin[vv].getX() << " " << velin[vv].getY() << std::endl;
          borda = 2;
        }
        it = itf = pline.begin();
        if (it == pline.end())
          break;
        itn = ++it;
      }
      continue;
    }

    ptf = velin[0];
    ptn = velin[velin.size()-1];
    if (Equal(ptf, ptn, scale))
    {// If closed contour
      assembLine(clinlist, velin);
      initLineVector(pline, velin);
    }
    it= itf = pline.begin();
    if (it == pline.end())
      break;
    itn = ++it;
  }

  assembLine(clinlist, velin);
  ofs.close();

  return true;
}

/*************************************************************
NAME    : initLineVector
AUTHOR    : Laercio Namikawa      aug-95
RESUME    : Initialize Line Vector with points of a line.
INPUT    : pline    line
OUTPUT    : vect    line with first and second points
RETURN    : TRUE    successful
FALSE    failure
*************************************************************/
bool te::mnt::initLineVector(std::vector<te::gm::PointZ> &pline, std::vector<te::gm::PointZ> &vect)
{
  short  c;
  std::vector<te::gm::PointZ>::iterator it;

  if (vect.size() != 0)
    vect.clear();

  if (pline.size() < 2)
    return true;

  for (c = 0; c < 2; c++)
  {
    vect.push_back(*pline.begin());
    pline.erase(pline.begin());
  }

  if (vect.size() != 2)
    return false;
  return true;
}


/*************************************************************
NAME    : assembLine
AUTHOR    : Laercio Namikawa      aug-95
RESUME    : Assemble a line with Line points and
save in LineList.
INPUT    : vect    line with points
OUTPUT    : linlout  output linelist
RETURN    : TRUE    successful
FALSE    failure
*************************************************************/
bool te::mnt::assembLine(std::vector<te::gm::LineString> &linlout, std::vector<te::gm::PointZ> &vect)
{
  if (vect.size() <= 1)
    return true;

  te::gm::LineString linout(vect.size(), te::gm::LineStringZType);

  for (size_t i = 0; i < vect.size(); i++)
    linout.setPointN(i, vect[i]);

  linlout.push_back(linout);

  return true;
}

/*!
\brief Function that defines the triangle normal vector
\param p3da are pointers to objets Point3d representing the triangle vertices
\param nvector are the coordinates of the normal vector
\return TRUE always
*/

bool te::mnt::triangleNormalVector(te::gm::PointZ *p3da, double *nvector)
{
  double	ux, vx, uy, vy, uz, vz;

  if ((p3da == NULL) || (nvector == NULL)){
    return false;
  }

  nvector[0] = 0.;
  nvector[1] = 0.;

  // Define normal vector (uvx,uvy,uvz)
  ux = p3da[1].getX() - p3da[0].getX();
  vx = p3da[2].getX() - p3da[0].getX();
  uy = p3da[1].getY() - p3da[0].getY();
  vy = p3da[2].getY() - p3da[0].getY();
  uz = p3da[1].getZ() - p3da[0].getZ();
  vz = p3da[2].getZ() - p3da[0].getZ();

  if ((ux == 0) && (vx == 0))
  {
    nvector[0] = 1.;
    nvector[1] = 0.;
    nvector[2] = 0.;
    return true;
  }
  if ((uy == 0) && (vy == 0))
  {
    nvector[0] = 0.;
    nvector[1] = 1.;
    nvector[2] = 0.;
    return true;
  }
  if ((uz == 0) && (vz == 0))
  {
    nvector[0] = 0.;
    nvector[1] = 0.;
    nvector[2] = 1.;
    return true;
  }

  nvector[2] = ux * vy - vx * uy;
  if (nvector[2] < 0.)
  {
    // Make sure that normal vector is always positive
    nvector[2] = -nvector[2];
    nvector[0] = vy * uz - uy * vz;
    nvector[1] = ux * vz - vx * uz;
  }
  else
  {
    nvector[0] = uy * vz - vy * uz;
    nvector[1] = vx * uz - ux * vz;
  }
  return true;
}

/*!
\brief Function that normalize a vector by its size
\param nvector is a pointer to the vector coordinates x, y and z
\return TRUE always
*/

bool te::mnt::normalizeVector(double *nvector)
{
  double	vectorSize;

  if (nvector == NULL){
    return false;
  }

  vectorSize = sqrt(nvector[0] * nvector[0] + nvector[1] * nvector[1] + nvector[2] * nvector[2]);

  if (vectorSize != 0.)
  {
    nvector[0] = nvector[0] / vectorSize;
    nvector[1] = nvector[1] / vectorSize;
    nvector[2] = nvector[2] / vectorSize;
  }
  return true;
}

/*!
\brief Function that checks if a point pt is on a segment
\param pt, fseg, lseg are pointers to Point objects
\param tol is the tolerance distance between the point and the segment
\return true if the point is on the segment or false otherwise
*/
bool te::mnt::onSegment(te::gm::PointZ& pt, te::gm::PointZ& fseg, te::gm::PointZ& lseg, double tol)
{
  double	pax, pay, bax, bay;
  double	area2, minxy, daux;

  if ((pt.getX() < fseg.getX()) && (pt.getX() < lseg.getX()))
    return false;
  if ((pt.getY() < fseg.getY()) && (pt.getY() < lseg.getY()))
    return false;
  if ((pt.getX() > fseg.getX()) && (pt.getX() > lseg.getX()))
    return false;
  if ((pt.getY() > fseg.getY()) && (pt.getY() > lseg.getY()))
    return false;
  pax = pt.getX() - fseg.getX();
  bax = lseg.getX() - fseg.getX();
  if ((pax == 0.) && (bax == 0.))
    // On segment
    return true;

  pay = pt.getY() - fseg.getY();
  bay = lseg.getY() - fseg.getY();
  if ((pay == 0.) && (bay == 0.))
    // On segment
    return true;

  area2 = fabs((pay * bax) - (pax * bay));
  bax = fabs(bax);
  bay = fabs(bay);
  minxy = bay;
  if (bax < bay)
    minxy = bax;
  daux = area2 / (bax + bay - (minxy / 2));

  if (daux < tol)
    // On segment
    return true;

  return false;
}


int te::mnt::onSameSide(te::gm::PointZ &pt1, te::gm::PointZ &pt2, te::gm::PointZ &fseg, te::gm::PointZ &lseg)
{
  double	a, b, c, ip, ipt;

  a = lseg.getY() - fseg.getY();
  b = fseg.getX() - lseg.getX();
  c = lseg.getX()*fseg.getY() - fseg.getX()*lseg.getY();
  ip = a*pt1.getX() + b*pt1.getY() + c;
  if (ip == 0.)
    // On segment
    return -1;
  ipt = a*pt2.getX() + b*pt2.getY() + c;
  if ((ip > 0.) && (ipt < 0.))
    return 0;
  if ((ip < 0.) && (ipt > 0.))
    return 0;

  // On same side
  return 1;
}

/*!
\brief Function that filters the points that are closer considering a given tolerance
\param p3dl is a pointer to a list of Point3dList objects
\param tol is a tolerance value
\return TRUE always
*/

bool te::mnt::point3dListFilter(std::vector<te::gm::PointZ> &p3dl, std::vector<bool> &fixed, double tol)
{
  te::gm::PointZ p3d;
  int32_t npts, nptsmax, i, j, maxdiffindex = 0;
  double x0 = 0, y0 = 0, z0, coef[5];
  double maxdiff;
  double maxfixptdiff;
  short degree;

  npts = 0;
  nptsmax = 0;

  double vectd[200];
  double vectz[200];
  double vectx[200];
  double vecty[200];
  double fvectz[200];
  short ptype[200];
  te::gm::PointZ p3dlaux[200];

  npts = 0;
  for (size_t pp = 0; pp < p3dl.size(); pp++)
  {
    p3d = p3dl[pp];
    if ((p3d.getX() >= std::numeric_limits< float >::max()) || (npts > 199))
    {
      // If last point of a line
      maxdiff = std::numeric_limits< float >::max();
      maxfixptdiff = std::numeric_limits< float >::max();
      j = 4;
      while ((maxdiff > tol) && (maxfixptdiff > tol) &&
        (j > 3) && (npts > 3))
      {
        j = 0;
        for (i = 0; i < npts; i++)
        {
          if (ptype[i] == 0)
            continue;
          vectx[j] = vectd[i];
          vecty[j++] = vectz[i];
        }
        if (j < 3)
          break;
        if (j < 5)
          degree = 1;
        else if (j < 8)
          degree = 2; //6 is minimum npts for 3rd degree
        else
          degree = 3;	//8 is minimum npts for 3rd degree
        Least_square_fitting(vectx, vecty, (short)j, degree, coef);
        for (i = 0; i < npts; i++)
        {
          fvectz[i] = coef[0];
          for (j = 1; j <= degree; j++)
            fvectz[i] += coef[j] * pow(vectd[i], (double)j);
        }
        maxfixptdiff = 0.;
        for (i = 0; i < npts; i++)
        {
          if ((ptype[i] == 0) || (ptype[i] == 1))
            continue;
          if (fabs(fvectz[i] - vectz[i]) > maxfixptdiff)
            maxfixptdiff = (float)fabs(fvectz[i] - vectz[i]);
        }
        maxdiff = 0.;
        for (i = 0; i < npts; i++)
        {
          if ((ptype[i] == 0) || (ptype[i] == 2))
            continue;
          if (fabs(fvectz[i] - vectz[i]) > maxdiff)
          {
            maxdiff = (float)fabs(fvectz[i] - vectz[i]);
            maxdiffindex = i;
          }
        }
        if (i == 0)
          break;
        ptype[maxdiffindex] = 0;
      }

      for (i = 0; i < npts; i++)
      {
        p3d = p3dlaux[i];
        p3d.setZ(fvectz[i]);
        z0 = p3d.getZ();
      }
      npts = 0;
      continue;
    }
    if (npts == 0)
    {
      x0 = p3d.getX();
      y0 = p3d.getY();
    }

    p3dlaux[npts] = p3d;
    vectd[npts] = sqrt((p3d.getX() - x0)*(p3d.getX() - x0) +
      (p3d.getY() - y0)*(p3d.getY() - y0));
    if (npts > 0)
      vectd[npts] += vectd[npts - 1];
    if (p3d.getZ() < std::numeric_limits< float >::max())
      ptype[npts] = 1;
    else
      ptype[npts] = 0;
    if (!fixed[pp])
      ptype[npts] = 2;
    vectz[npts] = p3d.getZ();
    fvectz[npts++] = p3d.getZ();
    x0 = p3d.getX();
    y0 = p3d.getY();
  }

  return true;
}

/*!
\brief Function that performs a leat square fitting in a set of points
\param vectx and vecty are the point coordinates
\param np is the number of points
\param deg is degree of the polynomium used in the fitting
\param coef is a pointer to the the polynomiun coefficients
\return TRUE if the fitting was performed with no errors or FALSE otherwise
*/

bool te::mnt::Least_square_fitting(double *vectx, double *vecty, short np, short deg, double *coef)
{
  short  i, j, m, n;
  double matx[6][6];
  double powx[200],
    sumpow[200],
    fx[200];

  if (np > 200)
    return false;

  // initialization 
  for (i = 0; i<np; i++)
  {
    powx[i] = 1.;
    fx[i] = vecty[i];
  }

  // compute the rigth hand side of the first normal equation 
  matx[0][deg + 1] = 0;
  for (i = 0; i<np; i++)
    matx[0][deg + 1] = matx[0][deg + 1] + vecty[i];

  // compute the rigth hand side of the other normal equation 
  // and also the sums 	
  sumpow[0] = np;
  for (i = 1; i <= deg; i++)
  {
    sumpow[i] = 0;
    matx[i][deg + 1] = 0;

    for (j = 0; j< np; j++)
    {
      powx[j] = powx[j] * vectx[j];
      fx[j] = fx[j] * vectx[j];
      sumpow[i] = sumpow[i] + powx[j];
      matx[i][deg + 1] = matx[i][deg + 1] + fx[j];
    }
  }

  for (i = deg + 1; i< 2 * (deg + 1); i++)
  {
    sumpow[i] = 0;
    for (j = 0; j<np; j++)
    {
      powx[j] = powx[j] * vectx[j];
      sumpow[i] = sumpow[i] + powx[j];
    }
  }

  for (i = 0; i <= deg; i++)
    for (j = 0; j <= deg; j++)
      matx[i][j] = sumpow[i + j];

  // perform Gaussian elimination
  m = deg + 1;
  n = 1;

  if (!Gauss_elimination(m, n, matx))
  {
    return false;
  }

  for (i = 0; i<deg + 1; i++)
    coef[i] = matx[i][m];

  return false;
}


/*********************************************************************
NAME		: Gauss_elimination
AUTHOR		: Jose Claudio Mura			oct - 95
RESUME		: Solve a system of linear equation throught the Gauss
elimination method.
INPUT        	: Number of the variables (m), number of equations (n) and
array with the variables (mat).
OUTPUT       	: Solution in the last column of the (a).
RETURN		: TRUE		successful
FALSE		failure
*********************************************************************/

bool te::mnt::Gauss_elimination(short m, short n, double mat[6][6])
{
  short i, j, k, l, ii, jj, kk;
  double aux, piv, eps;

  /*-- estimated error bound (machine epsilon) --*/
  eps = .000000001;

  if (m > 1)
  {
    for (i = 0; i<m - 1; i++)
    {
      piv = fabs(mat[i][i]);
      ii = i + 1;
      jj = i;

      /*-- search for the index in of maximum pivot value -*/
      for (j = ii; j< m; j++)
      {
        if (fabs(mat[j][i]) > piv)
        {
          piv = fabs(mat[j][i]);
          jj = j;
        }
      }

      if (i != jj)
      {
        /*-- interchange rows i and index jj --*/
        for (j = i; j< m + n; j++)
        {
          aux = mat[i][j];
          mat[i][j] = mat[jj][j];
          mat[jj][j] = aux;
        }
      }

      /*-- check if pivot is too smal --*/
      if (piv < eps)
      {
        return false;
      }

      /*-- forward elimination step --*/
      for (j = ii; j< m; j++)
      {
        for (k = ii; k< m + n; k++)
          mat[j][k] = mat[j][k] - mat[j][i] * mat[i][k] / mat[i][i];
      }
    }

    if (fabs(mat[m - 1][m - 1]) < eps)
    {
      return false;
    }

    /*-- back substitution --*/
    for (i = 0; i<n; i++)
    {
      mat[m - 1][i + m] = mat[m - 1][i + m] / mat[m - 1][m - 1];

      for (j = 0; j<m - 1; j++)
      {
        k = m - j - 2;
        kk = k + 1;

        for (l = kk; l<m; l++)
          mat[k][i + m] = mat[k][i + m] - mat[l][i + m] * mat[k][l];

        mat[k][i + m] = mat[k][i + m] / mat[k][k];
      }
    }
  }
  else
  {
    if (fabs(mat[0][0]) < eps)
    {
      return false;
    }

    for (i = 0; i<n; i++)
      mat[0][m + i] = mat[0][m + i] / mat[0][0];
  }
  
  return true;
}


