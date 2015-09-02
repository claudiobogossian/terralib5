#include "Utils.h"
#include "../../geometry/Point.h"


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
  double	delta = (double)(scale/* * 0.04 / 1000.*/); // point one percent of map tolerance

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
  if (npts <= 3)
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

  // te::gm::Point *plast = ptlist->getEndPoint();
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

  double	a, b, c, k, dist, aabb;

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
  double	m1 = 10., m2 = 10.;	// normais aos segmentos 1 e 2 
  double	xm1, ym1, 		// coords do ponto medio do segm. 1 
    xm2, ym2,		// coords do ponto medio do segm. 2 
    xaux, yaux,
    px[3], py[3];
  bool	perpvert1 = false,	// perpendicular vertical ao segmento 1 
    perpvert2 = false;	// perpendicular vertical ao segmento 2 
  short	i;

  for (i = 0; i < 3; i++)
  {
    px[i] = vert[i].getX();
    py[i] = vert[i].getY();
  }

  //	calcula o coeficiente angular perpendicular a reta 1
  if ((float)py[0] == (float)py[1])
  {
    if ((float)px[0] == (float)px[1])
    {
      //			pontos sao iguais-> retorna
      return false;
    }
    perpvert1 = true;
  }
  else
    m1 = -(px[1] - px[0]) / (py[1] - py[0]);

  //	calcula o coeficiente angular da perpendicular a reta 2
  if ((float)py[1] == (float)py[2])
  {
    if ((float)px[1] == (float)px[2])
    {
      //			pontos sao iguais-> retorna
      return false;
    }
    perpvert2 = false;
  }
  else
    m2 = -(px[2] - px[1]) / (py[2] - py[1]);

  //	Caso as retas sejam coincidentes, uma circunferencia 
  //	 nao eh definida
  if ((float)m1 == (float)m2)
  {
    return false;
  }

  //	passou pelos testes: os pontos definem uma circunferencia
  //	calculo do ponto medio do segmento ponto0-ponto1 (segmento 1)
  xm1 = (px[0] + px[1]) / 2.;
  ym1 = (py[0] + py[1]) / 2.;

  //	calculo do ponto medio do segmento ponto1-ponto2 (segmento 2)
  xm2 = (px[1] + px[2]) / 2.;
  ym2 = (py[1] + py[2]) / 2.;

  //	calculo das coordenadas do centro: ponto de interseccao das mediatrizes
  //	 dos segmentos ponto0-ponto1 e ponto1-ponto2
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
  te::gm::PointZ	pt;

  return segInterPoint(pfr, pto, lfr, lto, &pt);
}

/*!
\brief Function for determines the point of two segment intersections
\param pfr, pto, lfr, lto and pt are pointers to Point objects
\return TRUE if the intersection was calculated with no errors or FALSE otherwise
*/

bool te::mnt::segInterPoint(te::gm::PointZ &pfr, te::gm::PointZ &pto, te::gm::PointZ &lfr, te::gm::PointZ &lto, te::gm::PointZ *pt)
{
  //	Adapted from TWO-DIMENSIONAL CLIPPING: A VECTOR-BASED APPROACH
  //	Hans J.W. Spoelder, Fons H. Ullings in:
  ////	Graphics Gems I, pp.701, 

  double	a, b, c,
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
  double	delta = (float).9999;
  size_t	i;

  for (i = 0; i < 3; i++)
    if (p3da[i].getZ() == isolin)
      p3da[i].setZ(isolin*delta);


  if ((p3da[0].getZ() > BIGFLOAT) ||
    (p3da[1].getZ() > BIGFLOAT) ||
    (p3da[2].getZ() > BIGFLOAT))
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
  double	paux;// Parametric variable

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

  //std::ofstream ofs("d:\\teste\\Extract.txt", std::ofstream::out | std::ofstream::app);
  //ofs.precision(8);

  initLineVector(pline, velin);
  borda = 1;
 /* ofs << "velin.size " << velin.size() << std::endl;
  for (size_t vv = 0; vv < velin.size(); vv++)
    ofs << velin[vv].getX() << " " << velin[vv].getY() << std::endl;*/

  //index pindex to the current point of old line
  it = itf = pline.begin();
  itn = ++it;
  te::gm::PointZ ptf(*itf);
  te::gm::PointZ ptn(*itn);
  //ofs << "pline.size " << pline.size() << std::endl;
  while (it != pline.end())
  {
    ptf = *itf;
    ptn = *itn;
    pt = velin[velin.size()-1];
    //ofs << "ptf " << ptf.getX() << " " << ptf.getY() << " ptn " << ptn.getX() << " " << ptn.getY() << " pt " << pt.getX() << " " << pt.getY() << std::endl;
    if (equalFptSpt(ptf, pt, scale))
    {// conection on ptf
      velin.push_back(ptn);
      --it;
      it = itf = pline.erase(it);
      itn = pline.erase(it);
      //ofs << "velin.size " << velin.size() << std::endl;
      //for (size_t vv = 0; vv < velin.size(); vv++)
      //  ofs << velin[vv].getX() << " " << velin[vv].getY() << std::endl;
    }
    else if (equalFptSpt(ptn, pt, scale))
    {// conection on ptn
      velin.push_back(ptf);
      --it;
      it = itf = pline.erase(it);
      itn = pline.erase(it);
      //ofs << "velin.size " << velin.size() << std::endl;
      //for (size_t vv = 0; vv < velin.size(); vv++)
      //  ofs << velin[vv].getX() << " " << velin[vv].getY() << std::endl;
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
          //ofs << "velin.size " << velin.size() << std::endl;
          //for (size_t vv = 0; vv < velin.size(); vv++)
          //  ofs << velin[vv].getX() << " " << velin[vv].getY() << std::endl;
          borda = 1;
        }
        else
        {// If not on boundary, invert line points
          std::reverse(velin.begin(), velin.end());
          //ofs << " Reverse velin.size " << velin.size() << std::endl;
          //for (size_t vv = 0; vv < velin.size(); vv++)
          //  ofs << velin[vv].getX() << " " << velin[vv].getY() << std::endl;
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
  //ofs.close();

  return true;
}

/*************************************************************
NAME		: initLineVector
AUTHOR		: Laercio Namikawa			aug-95
RESUME		: Initialize Line Vector with points of a line.
INPUT		: pline		line
OUTPUT		: vect		line with first and second points
RETURN		: TRUE		successful
FALSE		failure
*************************************************************/
bool te::mnt::initLineVector(std::vector<te::gm::PointZ> &pline, std::vector<te::gm::PointZ> &vect)
{
  short	c;
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
NAME		: assembLine
AUTHOR		: Laercio Namikawa			aug-95
RESUME		: Assemble a line with Line points and
save in LineList.
INPUT		: vect		line with points
OUTPUT		: linlout	output linelist
RETURN		: TRUE		successful
FALSE		failure
*************************************************************/
bool te::mnt::assembLine(std::vector<te::gm::LineString> &linlout, std::vector<te::gm::PointZ> &vect)
{
  if (vect.size() == 1)
    return true;

  te::gm::LineString linout(vect.size(), te::gm::LineStringZType);

  for (size_t i = 0; i < vect.size(); i++)
    linout.setPointN(i, vect[i]);

  linlout.push_back(linout);

  return true;
}
