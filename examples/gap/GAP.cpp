
#include "../Config.h"
#include "GAP.h"

#include <geos.h>
#include <geos/geom/Coordinate.h>
#include <geos/geom/LineSegment.h>
#include <geos/geosAlgorithm.h>
#include <geos/simplify/DouglasPeuckerLineSimplifier.h>
#include <geos/algorithm/distance/DiscreteHausdorffDistance.h>

#include <terralib/geometry/Coord2d.h>
#include <terralib/geometry/GEOSWriter.h>


te::gm::Coord2D appGetCenterPointOfPoints(std::vector<te::gm::Coord2D> &pts)
{
  double x = 0;
  double y = 0;
  for (size_t i = 0; i < pts.size(); i++)
  {
    x += pts[i].getX();
    y += pts[i].getY();
  }
  return(te::gm::Coord2D(x / pts.size(), y / pts.size()));
}

class sortpoints
{
public:
  sortpoints(te::gm::Coord2D &pt):c(pt){}

  bool operator()(te::gm::Coord2D& a, te::gm::Coord2D& b)
  {
    if ((a.x - c.x) >= 0 && (b.x - c.x) < 0)
      return true;
    if ((a.x - c.x) < 0 && (b.x - c.x) >= 0)
      return false;
    if ((a.x - c.x) == 0 && (b.x - c.x) == 0) {
      if (a.y - c.y >= 0 || b.y - c.y >= 0)
        return a.y > b.y;
      return b.y > a.y;
    }

    // compute the cross product of vectors (center -> a) x (center -> b)
    double det = (a.x - c.x) * (b.y - c.y) - (b.x - c.x) * (a.y - c.y);
    if (det < 0)
      return true;
    if (det > 0)
      return false;

    // points a and b are on the same line from the center
    // check which point is closer to the center
    double d1 = (a.x - c.x) * (a.x - c.x) + (a.y - c.y) * (a.y - c.y);
    double d2 = (b.x - c.x) * (b.x - c.x) + (b.y - c.y) * (b.y - c.y);
    return d1 > d2;
  }

  te::gm::Coord2D c;
};


double Distance(const double pt1x, const double pt1y, const double pt2x, const double pt2y)
{
  double  dx,
    dy,
    dist;

  dx = pt1x - pt2x;
  dx = dx * dx;
  dy = pt1y - pt2y;
  dy = dy * dy;
  dist = sqrt(dy + dx);
  return(dist);
}


double Distance(const te::gm::Coord2D &pt1, const te::gm::Coord2D &pt2)
{
  return(Distance(pt1.getX(), pt1.getY(), pt2.getX(), pt2.getY()));
}


float coef_angular(double fx, double fy, double lx, double ly)
{
  float dx = (float)fx - (float)lx;
  float coef = std::numeric_limits< float >::max();
  if (dx)
    coef = ((float)fy - (float)ly) / dx;

  return coef;
}

bool isInside(te::gm::Coord2D &first, te::gm::Coord2D &last, te::gm::Coord2D &c, te::gm::Coord2D &cent)
{
  double d0c = Distance(first, cent);
  double d1c = Distance(last, cent);
  double dcc = Distance(c, cent);
  if (dcc > d0c && dcc > d1c)
    return false;

  double m01 = coef_angular(first.x, first.y, last.x, last.y);
  double m0c = coef_angular(first.x, first.y, c.x, c.y);

  if (m01 == std::numeric_limits< float >::max()) //segmento na vertical
  {
    if (m0c == 0)
    {
      if (dcc > d0c)
        return false;
      else
        return true;
    }
    if (m0c > 0)
      return true;
    else
      return false;
  }

  if (m0c == std::numeric_limits< float >::max()) //ponto na vertical
  {
    if (m01 == 0)
    { 
      if (dcc > d0c)
        return false;
      else 
        return true;
    }
    if (m01 < 0)
      return true;
    else
      return false;
  }

  else if (m0c < m01)
    return true;

  return false;
}

bool segInterPoint(te::gm::Coord2D &pfr, te::gm::Coord2D &pto, te::gm::Coord2D &lfr, te::gm::Coord2D &lto, te::gm::Coord2D *pt)
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
      pt->x = (pfr.getX() + (px*s));
      pt->y = (pfr.getY() + (py*s));
    }
  }
  return true;
}

double SegmentDistance(double fx, double fy, double lx, double ly, double ptx, double pty, double *pix, double *piy)
{
  double ux = lx - fx;
  double uy = ly - fy;
  double vx = ptx - fx;
  double vy = pty - fy;
  double uv = (ux*vx + uy*vy);
  if (uv < 0.)
  {
    if (pix && piy)
    {
      *pix = lx;
      *piy = ly;
    }
    return (sqrt(vx*vx + vy*vy));
  }
  else
  {
    ux = fx - lx;
    uy = fy - ly;
    vx = ptx - lx;
    vy = pty - ly;
    uv = (ux*vx + uy*vy);
    if (uv < 0.)
    {
      if (pix && piy)
      {
        *pix = fx;
        *piy = fy;
      }
      return (sqrt(vx*vx + vy*vy));
    }
  }

  double  a, b, c, k, dist, aabb;

  a = ly - fy;
  b = fx - lx;
  c = lx*fy - fx*ly;
  aabb = a*a + b*b;

  dist = fabs((a*ptx + b*pty + c) / (sqrt(aabb)));
  if (pix && piy)
  {
    k = b*ptx - a*pty;
    *pix = (b*k - a*c) / aabb;
    *piy = (-a*k - b*c) / aabb;
  }

  return dist;

}

double coordToSegmentDistance(te::gm::Coord2D &fseg, te::gm::Coord2D &lseg, te::gm::Coord2D &pt, te::gm::Coord2D *pti)
{
  //double pix, piy;
  //double dist = SegmentDistance(fseg.getX(), fseg.getY(), lseg.getX(), lseg.getY(), pt.getX(), pt.getY(), &pix, &piy);
  //if (pti)
  //{
  //  pti->x = pix;
  //  pti->y = piy;
  //}
  //return dist;
  geos::geom::LineSegment ls(fseg.x, fseg.y, lseg.x, lseg.y);
  geos::geom::Coordinate c(pt.x, pt.y);
  geos::geom::Coordinate c_int;
  if (pti)
  {
    ls.closestPoint(c, c_int);
    pti->x = c_int.x;
    pti->y = c_int.y;
  }
  return(ls.distance(c));
}


te::gm::LineString* GEOS_DouglasPeucker(te::gm::LineString *ls, double snap)
{
  std::vector<geos::geom::Coordinate> coordpts;
  for (std::size_t j = 0; j < ls->size(); ++j)
  {
    te::gm::Point *lpt = ls->getPointN(j);
    geos::geom::Coordinate coo(lpt->getX(), lpt->getY());
    coordpts.push_back(coo);
  }
  geos::simplify::DouglasPeuckerLineSimplifier douglas(coordpts);
  douglas.setDistanceTolerance(snap);
  geos::simplify::DouglasPeuckerLineSimplifier::CoordsVectAutoPtr simplified = douglas.simplify();
  if (simplified.get()->at(0) != simplified.get()->at(simplified->size() - 1))
    simplified->insert(simplified->end(), simplified.get()->at(0));

  te::gm::LineString* lsout = new te::gm::LineString(simplified->size(), te::gm::LineStringType);

  for (std::size_t j = 0; j < simplified->size(); ++j)
  {
    geos::geom::Coordinate c1 = simplified.get()->at(j);
    lsout->setPoint(j, c1.x, c1.y);
  }

  return lsout;
}


bool GAP_inter::operator<(const GAP_inter &rhs)
{
  if (m_ptid < rhs.m_ptid)
    return true;
  if (m_ptid == rhs.m_ptid)
  {
    if (m_distance < rhs.m_distance)
      return true;
  }
  return false;
}


bool GAP_inter::operator==(const GAP_inter &rhs)
{
  return (m_xnew == rhs.m_xnew && m_ynew == rhs.m_ynew);
}


bool GAP_segment::isInside(te::gm::Coord2D &pt)
{
  double d0 = Distance(m_seg.getCoordinates()[0], m_centroid);
  double d1 = Distance(m_seg.getCoordinates()[1], m_centroid);
  double dp = Distance(pt, m_centroid);
  if (dp < d0 && dp < d1)
    return true;

  //return false;
  float dx = (float)m_seg.getCoordinates()[1].getX() - (float)m_seg.getCoordinates()[0].getX();
  float dxpt = (float)pt.getX() - (float)m_seg.getCoordinates()[0].getX();
  float coef_seg = std::numeric_limits< float >::max();
  float coef_pt = std::numeric_limits< float >::max();
  if (dx)
    coef_seg = ((float)m_seg.getCoordinates()[1].getY() - (float)m_seg.getCoordinates()[0].getY()) / dx;
  if (dxpt)
    coef_pt = ((float)pt.getY() - (float)m_seg.getCoordinates()[0].getY()) / dxpt;

  if (!dx)//segmento vertical ->
  {
    if (coef_pt > 0)
      return true;
    else
      return false;
  }

  if (!dxpt)
  {
    if (coef_seg < 0)
      return true;
    else
      return false;
  }

  if (coef_pt < coef_seg)
    return true;

  return false;
}

void GAP_segment::insert_inter(GAP_inter *inter)
{
  m_inter.push_back(inter);
}

void GAP::Save(te::da::DataSource* source, te::da::DataSet* result, te::da::DataSetType* outDsType)
{
  std::auto_ptr<te::da::DataSourceTransactor> t = source->getTransactor();

  std::map<std::string, std::string> options;

  try
  {
    if (source->getType() == "OGR")
    {
      // create the dataset
      source->createDataSet(outDsType, options);

      // copy from memory to output datasource
      result->moveBeforeFirst();
      std::string name = outDsType->getName();
      source->add(name, result, options);
    }
    else
    {
      t->begin();

      // create the dataset
      t->createDataSet(outDsType, options);

      // copy from memory to output datasource
      result->moveBeforeFirst();
      std::string name = outDsType->getName();
      t->add(name, result, options);

      t->commit();
    }

  }
  catch (te::common::Exception& e)
  {
    t->rollBack();
    throw e;
  }
  catch (std::exception& e)
  {
    t->rollBack();
    throw e;
  }
}


bool GAP::Polygon2Points(te::gm::Polygon* pol, int32_t id)
{
  for (size_t nlr = 0; nlr < pol->getNumRings(); nlr++)
  {
    te::gm::LinearRing *lr = dynamic_cast<te::gm::LinearRing*>(pol->getRingN(nlr));
    for (size_t i = 0; i < lr->getNPoints() - 1; i++)
    {
      te::gm::Coord2D pt(lr->getPointN(i)->getX(), lr->getPointN(i)->getY());
      m_nodetree->insert(pt, std::pair<int32_t, int32_t>(id, (int32_t)i));
    }
  }
  return true;
}


bool GAP::Polygon2RTree(te::gm::Polygon* pol, te::sam::rtree::Index<te::gm::Line*> &rtree)
{
  std::vector<te::gm::Line*> reportline;

  for (size_t nlr = 0; nlr < pol->getNumRings(); nlr++)
  {
    te::gm::LinearRing *lr = dynamic_cast<te::gm::LinearRing*>(pol->getRingN(nlr));
    for (size_t i = 0; i < lr->getNPoints() - 1; i++)
    {
      te::gm::Point first(*lr->getPointN(i));
      te::gm::Point last(*lr->getPointN(i + 1));
      te::gm::Line *l = new te::gm::Line(first, last, te::gm::LineStringType, pol->getSRID());
      te::gm::Envelope e(*l->getMBR());
      rtree.insert(e, l);
    }
  }
  return true;
}

bool GAP::Polygon2Lines(te::gm::Polygon* pol)
{
  for (size_t nlr = 0; nlr < pol->getNumRings(); nlr++)
  {
    te::gm::LinearRing *lr = dynamic_cast<te::gm::LinearRing*>(pol->getRingN(nlr));
    for (size_t i = 0; i < lr->getNPoints() - 1; i++)
    {
      te::gm::Point first(*lr->getPointN(i));
      te::gm::Point last(*lr->getPointN(i + 1));
      te::gm::Line *l = new te::gm::Line(first, last, te::gm::LineStringType, pol->getSRID());
      m_line.push_back(l);
    }
  }
  return true;
}

bool GAP::LoadPolygons(std::string &filename, std::string &inDsetName, Pol_OUT out)
{
  std::map<std::string, std::string> srcInfo;
  srcInfo["URI"] = filename;
  srcInfo["DRIVER"] = "ESRI Shapefile";

  te::da::DataSourcePtr srcDs(te::da::DataSourceFactory::make("OGR"));
  srcDs->setConnectionInfo(srcInfo);
  srcDs->open();

  if (!srcDs->dataSetExists(inDsetName))
  {
    std::cout << "Input dataset not found: " << inDsetName << std::endl;
    return false;
  }

  std::unique_ptr<te::da::DataSet> inDset = srcDs->getDataSet(inDsetName);
  std::unique_ptr<te::da::DataSetType> inDsetType = srcDs->getDataSetType(inDsetName);
  std::size_t geo_pos = te::da::GetFirstPropertyPos(inDset.get(), te::dt::GEOMETRY_TYPE);
  std::size_t id_pos = te::da::GetPropertyPos(inDset.get(), "FID");

  inDset->moveBeforeFirst();
  while (inDset->moveNext())
  {
    std::unique_ptr<te::gm::Geometry> gin = inDset->getGeometry(geo_pos);
    std::string geostype = gin.get()->getGeometryType();
    if (geostype == "MultiPolygon")
    {
      te::gm::MultiPolygon *g = dynamic_cast<te::gm::MultiPolygon*>(gin.get()->clone());
      std::size_t np = g->getNumGeometries();
      for (std::size_t i = 0; i < np; ++i)
      {
        te::gm::Polygon *p = dynamic_cast<te::gm::Polygon*>(g->getGeometryN(i));
        p->setSRID(g->getSRID());
        m_srid = p->getSRID();
        if (out == Polygon_out)
        {
          m_pol.insert(std::pair<size_t, te::gm::Polygon*>(inDset->getInt32(id_pos), p));
          double area = p->getArea();
          int32_t id = (int32_t)inDset->getInt32(id_pos);
         // m_original_area.insert(std::pair<size_t, double>(id, area));
          m_original_area[i] = area;
        }
        if (out == RTree_out)
          Polygon2RTree(p, m_rtree);
        if (out == Line_out)
          Polygon2Lines(p);
        if (out == Point_out)
        {
          Polygon2Points(p, inDset->getInt32(id_pos));
          m_srid = p->getSRID();
        }
      }
    }
    if (geostype == "Polygon")
    {
      te::gm::Polygon *p = dynamic_cast<te::gm::Polygon*>(gin.get()->clone());
      p->setSRID(gin->getSRID());
      m_srid = p->getSRID();
      if (out == Polygon_out)
        m_pol.insert(std::pair<size_t, te::gm::Polygon*>(inDset->getInt32(id_pos), p));
      if (out == RTree_out)
        Polygon2RTree(p, m_rtree);
      if (out == Line_out)
        Polygon2Lines(p);
      if (out == Point_out)
      {
        Polygon2Points(p, inDset->getInt32(id_pos));
        m_srid = p->getSRID();
      }
    }
  }

  return true;

}

GAP::GAP()
{
  m_dist_min = 0.0005;
  m_tol = m_dist_min / 5;
  m_filename1 = "D:/Dados_GAP/AGD2015_22761_pol.shp";
  m_inDS1 = "AGD2015_22761_pol";
  m_filename2 = "D:/Dados_GAP/desflorestamento4674_dissolve.shp"; //Desflorestamento
  m_inDS2 = "desflorestamento4674_dissolve";
  //m_filename1 = "D:/Dados_GAP/1agreg.shp";
  //m_inDS1 = "1agreg";
 // m_filename2 = "D:/Dados_GAP/desf_1449.shp";
 // m_inDS2 = "desf_1449";
  // m_filename2 = "D:/Dados_GAP/polygons_int0.shp";
 // m_inDS2 = "polygons_int0";

  te::gm::Envelope e;
  m_nodetree = new KD_TREE(e);
  m_nodetree_step2 = new KD_TREE(e);
}

bool GAP::run()
{
 // return run5();

  //m_filename2 = "D:/Dados_GAP/poly163_step2.shp"; //Desflorestamento
  //m_inDS2 = "poly163_step2";
  //if (!LoadPolygons(m_filename2, m_inDS2, Polygon_out)) //Desflorestamento para polygon
  //  return false;
  //m_pols_2 = m_pol;
  if (!LoadPolygons(m_filename1, m_inDS1, RTree_out)) //Agregado para segmentos (rtree)
    return false;

  if (!LoadPolygons(m_filename1, m_inDS1, Point_out)) //Agregado para Point (kdtree)
     return false;

  if (!LoadPolygons(m_filename2, m_inDS2, Polygon_out)) //Desflorestamento para polygon
    return false;

  std::vector< std::pair< size_t, te::gm::Polygon* > > pols_step00; //poligonos gerados no step0
  std::vector< std::pair< size_t, te::gm::Polygon* > > pols_step0; //poligonos gerados no step0
  std::vector< std::pair< size_t, te::gm::Polygon* > > pols_step1; //poligonos gerados no step1
  std::vector< std::pair< size_t, te::gm::Polygon* > > pols_step2; //poligonos gerados no step2
  std::vector< std::pair< size_t, te::gm::Polygon* > > pols_step3; //poligonos gerados no step3
  std::vector< std::pair< size_t, te::gm::Polygon* > > pols_final; //poligonos gerados no step3a

  step00(pols_step00);
  if (step0(pols_step00, pols_step0))
  {
    if (step1(pols_step0, pols_step1))
    {
      if (step2(pols_step1, pols_step2))
      {
        pols_step1.clear();

        if (step3(pols_step2, pols_step3))
        {
          pols_step2.clear();
          SavePol(pols_step3, "step3");
          if (step4(pols_step3, pols_final))
          {
            pols_step3.clear();
            SavePol(pols_final, "final");
            pols_final.clear();
          }
        }
      }
    }
  }

  return false;
}

bool GAP::step00(std::vector< std::pair< size_t, te::gm::Polygon* > > &polsout)
{
  for (std::map<size_t, te::gm::Polygon*>::iterator it = m_pol.begin(); it != m_pol.end(); it++)  // Para cada poligono do desflorestamento
  {
    te::gm::Polygon *pol = it->second;
    int32_t polid = (int32_t)it->first;
    te::gm::Coord2D *cent = pol->getCentroidCoord();
    double area = pol->getArea();
    if (area < (m_dist_min*m_dist_min) || !cent) //No conseguiu gerar centroide e area menor que tolerancia
    {
      std::cout << "Polygon ERROR 0" << polid << " area " << area << std::endl;
      continue;
    }

    te::gm::LineString *lin = GEOS_DouglasPeucker(dynamic_cast<te::gm::LineString*>(pol->getExteriorRing()), m_tol);
    //  te::gm::LineString *lin = dynamic_cast<te::gm::LineString*>(pol->getExteriorRing());

    if (lin->size() > 4)
    {
      te::gm::Polygon *pol_s = new te::gm::Polygon(0, te::gm::PolygonType, m_srid);
      size_t npts = lin->getNPoints();
      if (!lin->isClosed())
        npts++;
      te::gm::LinearRing *pol_ring = new te::gm::LinearRing( npts, te::gm::LineStringType, m_srid);
      size_t i;
      for (i = 0; i < lin->getNPoints(); i++)
      {
        te::gm::Coord2D p_d(lin->getCoordinates()[i]); //ponto do desflorestamento
        m_pol_points.push_back(GAP_inter(polid, (int32_t)i, p_d.getX(), p_d.getY(), p_d.getX(), p_d.getY(), p_d.getX(), p_d.getY(),
          0., 0., "desf", true));
        pol_ring->setPointN(i, te::gm::Point(p_d.getX(), p_d.getY(), m_srid));
      }
      if (!pol_ring->isClosed())
        pol_ring->setPointN(i, te::gm::Point(lin->getCoordinates()[0].getX(), lin->getCoordinates()[0].getY(), m_srid));
      if (pol_ring->isClosed())
      {
        pol_s->push_back(pol_ring);
        polsout.push_back(std::pair<int32_t, te::gm::Polygon *>(polid, pol_s));
      }
      delete lin;
    }
    else
    {
      std::cout << "ERROR 00 " << polid << std::endl;
      delete lin;
      continue;
    }
  }

  SavePol(polsout, "step00");

  m_pol_points.clear();
  return true;

}

bool GAP::step0(std::vector< std::pair< size_t, te::gm::Polygon* > > &pols, std::vector< std::pair< size_t, te::gm::Polygon* > > &polsout)
{
  std::vector<KD_NODE*> reportsnode;
  std::map<double, te::gm::Coord2D> p_agreg;
 // std::map< const te::gm::Coord2D, std::pair<size_t, double> > p_agreg_sel;
 // std::map< const te::gm::Coord2D, std::pair<size_t, double> >::iterator it_agreg;
  std::map< const te::gm::Coord2D, std::pair<size_t, double> > p_agreg_sel;
  std::map< const te::gm::Coord2D, std::pair<size_t, double> >::iterator it_agreg;
  std::vector<te::gm::Coord2D> pol_points;

  for (std::vector< std::pair<size_t, te::gm::Polygon*> >::iterator it = pols.begin(); it != pols.end(); it++)  // Para cada poligono do desflorestamento
  {
    te::gm::Polygon *pol = it->second;
    int32_t polid = (int32_t)it->first;
    te::gm::Coord2D *cent = pol->getCentroidCoord();
    double area = pol->getArea();
    if (area < (m_dist_min*m_dist_min) || !cent) //No conseguiu gerar centroide e area menor que tolerancia
    {
      std::cout << "Polygon ERROR " << polid << " area " << area << std::endl;
      continue;
    }

    te::gm::LineString *lin = dynamic_cast<te::gm::LineString*>(pol->getExteriorRing());

    te::sam::rtree::Index<te::gm::Line*> rtree_pol; //rtree com os segmentos deste poligono
    Polygon2RTree(pol, rtree_pol);

    //Procurar pontos do agregado que estejam na proximidade e externos aos pontos do desflorestamento
    for (size_t i = 0; i < lin->getNPoints()-1; i++)
    {
      te::gm::Coord2D p_d(lin->getCoordinates()[i]); //ponto do desflorestamento

      te::gm::Envelope ept(p_d.x - m_dist_min, p_d.y - m_dist_min,
        p_d.x + m_dist_min, p_d.y + m_dist_min);
      m_nodetree->search(ept, reportsnode);

      for (size_t j = 0; j < reportsnode.size(); j++)
      {
        te::gm::Coord2D c = reportsnode[j]->getKey();
        double dist = Distance(c, p_d);
        if (dist < m_dist_min)
        {
          if (pointLocate(c, pol) != geos::geom::Location::INTERIOR)
          {
            p_agreg.insert(std::pair<double, te::gm::Coord2D>(dist, c)); //Ordena os pontos pela proximidade ao ponto do desflorestamento
          }
        }
      }
      if (p_agreg.size())
      {
       // const geos::geom::Coordinate c(p_agreg.begin()->second.x, p_agreg.begin()->second.y);
        const te::gm::Coord2D c(p_agreg.begin()->second.x, p_agreg.begin()->second.y);
        double d = Distance(p_agreg.begin()->second, p_d);
       // it_agreg = std::find(p_agreg_sel.begin(), p_agreg_sel.end(), c);
        for (it_agreg = p_agreg_sel.begin(); it_agreg != p_agreg_sel.end(); it_agreg++)
        {
          if ((*it_agreg).first == c)
            break;
        }

        if (it_agreg != p_agreg_sel.end())
        {
          if ((*it_agreg).second.first == 0 && i == lin->getNPoints() - 2) //first point and penultimate point
          {
            //ignore
            p_agreg_sel.insert(std::pair<te::gm::Coord2D, std::pair<size_t, double> >(p_d, std::pair<size_t, double>(i, 0)));
          }
          else
          if (d < (*it_agreg).second.second)
          {
            (*it_agreg).second.first = i;
            (*it_agreg).second.second = d;
          }
        }
        else 
          p_agreg_sel.insert(std::pair<te::gm::Coord2D, std::pair<size_t, double> >(c, std::pair<size_t, double>(i, d)));
      }
      p_agreg.clear();
      reportsnode.clear();
    }

    for (size_t i = 0; i < lin->getNPoints()-1; i++)
    {
      te::gm::Coord2D p_d(lin->getCoordinates()[i]); //ponto do desflorestamento

      for (it_agreg = p_agreg_sel.begin(); it_agreg != p_agreg_sel.end(); it_agreg++)
      {
        if ((*it_agreg).second.first == i)
          break;
      }
      if (it_agreg != p_agreg_sel.end()) //
      {
        if ((*it_agreg).second.second == 0) //ignore
        {
          m_pol_points.push_back(GAP_inter(polid, (int32_t)i, p_d.getX(), p_d.getY(), p_d.getX(), p_d.getY(), p_d.getX(), p_d.getY(),
            0., 0., "not_used", true));
          continue;
        }

        te::gm::Coord2D pp(it_agreg->first.x, it_agreg->first.y);
        if (!verifyIntersections(p_d, pp, rtree_pol))
        {
          pol_points.push_back(pp); //Pega o ponto mais proximo
          m_pol_points.push_back(GAP_inter(polid, (int32_t)i, p_d.getX(), p_d.getY(), p_d.getX(), p_d.getY(), pp.x, pp.y,
          it_agreg->second.second, it_agreg->second.second, "agreg", true));
        }
        else
        {
          pol_points.push_back(p_d);
          m_pol_points.push_back(GAP_inter(polid, (int32_t)i, p_d.getX(), p_d.getY(), p_d.getX(), p_d.getY(), p_d.getX(), p_d.getY(),
            0., 0., "desf", true));
        }
      }
      else
      {
        pol_points.push_back(p_d);
        m_pol_points.push_back(GAP_inter(polid, (int32_t)i, p_d.getX(), p_d.getY(), p_d.getX(), p_d.getY(), p_d.getX(), p_d.getY(),
          0., 0., "desf", true));
      }
    } //dos pontos do poligono

    p_agreg_sel.clear();

    if (pol_points.size() < 3)
    {
      std::cout << "Step0 - out Polygon ERROR " << polid << std::endl;
      pol_points.clear();
      continue;
    }

    te::gm::Polygon *pol_new = new te::gm::Polygon(0, te::gm::PolygonType, m_srid);
    te::gm::LinearRing *pol_ring = new te::gm::LinearRing(pol_points.size() + 1, te::gm::LineStringType, m_srid);
    size_t pp;
    for (pp = 0; pp < pol_points.size(); pp++)
      pol_ring->setPointN(pp, te::gm::Point(pol_points[pp].getX(), pol_points[pp].getY(), m_srid));

    pol_ring->setPointN(pp, te::gm::Point(pol_points[0].getX(), pol_points[0].getY(), m_srid));

    pol_points.clear();

    pol_new->push_back(pol_ring);
    if (pol_new->getArea() <= 0)
    {
      std::cout << "Step0 - ERROR " << polid << std::endl;
      delete pol_new;
    }
    else
      polsout.push_back(std::pair<int32_t, te::gm::Polygon *>(polid, pol_new));
  } //dos poligonos


  SavePol(polsout, "step0");

  m_pol_points.clear();
  return true;
}

// Move os pontos do desflorestamento para a borda do agregado (junta pelos vertices)
bool GAP::step1(std::vector< std::pair< size_t, te::gm::Polygon* > > &pols, std::vector< std::pair< size_t, te::gm::Polygon* > > &polsout)
{
  std::vector<te::gm::Line*> reportline;
  std::vector<te::gm::Coord2D> pol_points;
  std::map< double, std::pair<size_t, te::gm::Coord2D> > map_seg;
  std::map< double, std::pair<size_t, te::gm::Coord2D> > :: iterator it_seg;
  std::map<const te::gm::Coord2D, std::pair<size_t, double> > map_pts_used;
  std::map<const te::gm::Coord2D, std::pair<size_t, double> >::iterator it_pts_used;

  for (std::vector< std::pair<size_t, te::gm::Polygon*> >::iterator it = pols.begin(); it != pols.end(); it++)  // Para cada poligono do desflorestamento
  {
    te::gm::Polygon *pol = it->second;
    int32_t polid = (int32_t)it->first;
    te::gm::LinearRing *lin = dynamic_cast<te::gm::LinearRing*>(pol->getExteriorRing());

    te::sam::rtree::Index<te::gm::Line*> rtree_pol; //rtree com os segmentos deste poligono
    Polygon2RTree(pol, rtree_pol);

    for (size_t i = 0; i < lin->getNPoints()-1; i++) //Para cada ponto do desflorestamento
    {
      te::gm::Coord2D pt0 = lin->getCoordinates()[i];

      te::gm::Envelope ept(pt0.x - m_dist_min, pt0.y - m_dist_min,
        pt0.x + m_dist_min, pt0.y + m_dist_min);
      m_rtree.search(ept, reportline); //procura segmentos do agregado

      double dist;
      if (!reportline.size()) //nao tem agregado proximo --> nao mexe no vertice
      {
        //pol_points.push_back(pt0);
        //m_pol_points.push_back(GAP_inter(polid, (int32_t)i, pt0.getX(), pt0.getY(), pt0.getX(), pt0.getY(), pt0.getX(), pt0.getY(),
        //  0., 0., "desf", true));
        //continue;
      }
      else
      {
        bool addpt = false;
        te::gm::Coord2D ptint;
        size_t seg_sel = 0;

        //Pegar os segmentos do agregado  e ordena-los pela distancia do p0 do desflorestamento
        for (size_t rl = 0; rl < reportline.size(); rl++)
        {
          dist = coordToSegmentDistance(reportline[rl]->getCoordinates()[0], reportline[rl]->getCoordinates()[1], pt0, &ptint);
          map_seg.insert(std::pair< double, std::pair<size_t, te::gm::Coord2D> >(dist, std::pair<size_t, te::gm::Coord2D>(rl, ptint)));
        }

        for (it_seg = map_seg.begin(); it_seg != map_seg.end(); it_seg++)
        {
          dist = it_seg->first;
          seg_sel = it_seg->second.first;
          ptint = it_seg->second.second;
          te::gm::Line *l_agreg = reportline[seg_sel];
          bool achou = false;

          if (dist <= m_dist_min)
          {
            if (pointLocate(ptint, pol) != geos::geom::Location::INTERIOR)
            {
              if (!verifyIntersections(pt0, ptint, rtree_pol))
              {
                for (it_pts_used = map_pts_used.begin(); it_pts_used != map_pts_used.end(); it_pts_used++)
                {
                  if ((*it_pts_used).first == ptint)
                  {
                    achou = true;
                    if (it_pts_used != map_pts_used.end()) //Tem outro ponto do desflorestametno com esse mesmo ponto de intersecção, usa o mais proximo
                    {
                      if (dist < it_pts_used->second.second)
                        it_pts_used->second.first = i;
                    }
                  }
                } //for (it_pts_used = map_pts_used.begin(); it_pts_used != map_pts_used.end(); it_pts_used++)
                if (!achou)
                {
                  map_pts_used.insert(std::pair<te::gm::Coord2D, std::pair<size_t, double>>(ptint, std::pair<size_t, double>(i, dist)));
                }
                //else
                //{
                //  pol_points.push_back(ptint);
                //  m_pol_points.push_back(GAP_inter(polid, (int32_t)i, pt0.getX(), pt0.getY(), ptint.getX(), ptint.getY(), ptint.getX(), ptint.getY(),
                //    dist, 0., "inter", true));
                //  addpt = true;
                //  break;
                //}
              }
            }
          }// if (dist <= m_dist_min)
        } //for (it_seg = map_seg.begin(); it_seg != map_seg.end(); it_seg++)

        //if (!addpt)
        //{
        //  pol_points.push_back(pt0);
        //  m_pol_points.push_back(GAP_inter(polid, (int32_t)i, pt0.getX(), pt0.getY(), pt0.getX(), pt0.getY(), pt0.getX(), pt0.getY(),
        //    0., 0., "desf", true));
        //}
        map_seg.clear();
        reportline.clear();
      } //if (!reportline.size()) ... else
    } //Para cada segmento do desflorestamento

    for (size_t i = 0; i < lin->getNPoints()-1; i++) //Para cada ponto do desflorestamento
    {
      bool addpt = false;
      te::gm::Coord2D pt0 = lin->getCoordinates()[i];
      for (it_pts_used = map_pts_used.begin(); it_pts_used != map_pts_used.end(); it_pts_used++)
      {
        if ((*it_pts_used).second.first == i)
        {
          pol_points.push_back(it_pts_used->first);
          m_pol_points.push_back(GAP_inter(polid, (int32_t)i, pt0.getX(), pt0.getY(), pt0.getX(), pt0.getY(), it_pts_used->first.getX(), it_pts_used->first.getY(),
            (*it_pts_used).second.second, 0., "agreg", true));
          addpt = true;
          break;
        }
      }
      if (!addpt)
      {
        pol_points.push_back(pt0);
        m_pol_points.push_back(GAP_inter(polid, (int32_t)i, pt0.getX(), pt0.getY(), pt0.getX(), pt0.getY(), pt0.getX(), pt0.getY(),
          0., 0., "desf", true));
      }
    }
    map_pts_used.clear();


    if (pol_points.size() < 3)
    {
      std::cout << "Step1 - out Polygon ERROR " << polid << std::endl;
      pol_points.clear();
      continue;
    }

    te::gm::Polygon *pol_new = new te::gm::Polygon(0, te::gm::PolygonType, m_srid);
    te::gm::LinearRing *pol_ring = new te::gm::LinearRing(pol_points.size() + 1, te::gm::LineStringType, m_srid);
    size_t pp;
    for (pp = 0; pp < pol_points.size(); pp++)
      pol_ring->setPointN(pp, te::gm::Point(pol_points[pp].getX(), pol_points[pp].getY(), m_srid));

    pol_ring->setPointN(pp, te::gm::Point(pol_points[0].getX(), pol_points[0].getY(), m_srid));

    pol_points.clear();

    pol_new->push_back(pol_ring);
    if (pol_new->getArea() <= 0)
    {
      std::cout << "Step1 - ERROR " << polid << std::endl;
      delete pol_new;
    }
    else
      polsout.push_back(std::pair<int32_t, te::gm::Polygon *>(polid, pol_new));
  } // Para cada poligono do desflorestamento

  SavePol(polsout, "step1");

  m_pol_points.clear();
  return true;
}


//Add intersection points from agregado
bool GAP::step2(std::vector< std::pair< size_t, te::gm::Polygon* > > &pols, std::vector< std::pair< size_t, te::gm::Polygon* > > &polsout)
{
  std::vector<te::gm::Coord2D> pt_out;
  std::vector<te::gm::Line*> reportline;
  std::map<double, te::gm::Coord2D> map_int;

  for (std::vector< std::pair<size_t, te::gm::Polygon*> >::iterator it = pols.begin(); it != pols.end(); it++)  // Para cada poligono do desflorestamento
  {
    te::gm::Polygon *pol = it->second;
    int32_t polid = (int32_t)it->first;
    te::gm::LinearRing *lin = dynamic_cast<te::gm::LinearRing*>(pol->getExteriorRing());

    size_t i;
    for (i = 0; i < lin->getNPoints() - 1; i++) //Cria segmento com os pontos da borda (i ~ i+1)
    {
      te::gm::Coord2D first(lin->getCoordinates()[i]);

      //if (std::find(pt_out.begin(), pt_out.end(), first) == pt_out.end()) //Não coloca pontos repetidos
      {
        pt_out.push_back(te::gm::Coord2D(first));
        m_pol_points.push_back(GAP_inter(polid, (int32_t)i, first.getX(), first.getY(), first.getX(), first.getY(), first.getX(), first.getY(),
          0., 0., "desf", true));
      }
      te::gm::Coord2D last(lin->getCoordinates()[i + 1]);
      te::gm::Line *line = new te::gm::Line(te::gm::LineStringType, m_srid);
      line->setCoord(0, first.getX(), first.getY());
      line->setCoord(1, last.getX(), last.getY());
      double dist01 = Distance(first, last);
      te::gm::Envelope ept(line->getMBR()->getLowerLeftX() - m_dist_min, line->getMBR()->getLowerLeftY() - m_dist_min,
        line->getMBR()->getUpperRightX() + m_dist_min, line->getMBR()->getUpperRightY() + m_dist_min);
      m_rtree.search(ept, reportline); //procura segmentos do agregado
      if (!reportline.size()) //não tem agregado próximo
        continue;

      te::gm::Coord2D ptint;
      for (size_t rl = 0; rl < reportline.size(); rl++)
      {
        if (segInterPoint(reportline[rl]->getCoordinates()[0], reportline[rl]->getCoordinates()[1], first, last, &ptint))
        {
          double dist = Distance(first, ptint);
          map_int.insert(std::pair<double, te::gm::Coord2D>(dist, te::gm::Coord2D(ptint)));
        }
      }
      for (std::map<double, te::gm::Coord2D>::iterator it = map_int.begin(); it != map_int.end(); it++)
      //if (std::find(pt_out.begin(), pt_out.end(), (*it).second) == pt_out.end()) //Não coloca pontos repetidos
      {
          pt_out.push_back(te::gm::Coord2D((*it).second));
          m_pol_points.push_back(GAP_inter(polid, (int32_t)i, (*it).second.getX(), (*it).second.getY(), (*it).second.getX(), (*it).second.getY(), (*it).second.getX(), (*it).second.getY(),
            (*it).first, 0., "agreg", true));
      }

      reportline.clear();
      map_int.clear();
    }
    pt_out.push_back(te::gm::Coord2D(lin->getCoordinates()[i])); //ultimo ponto para fechar poligono

    if (pt_out.size() < 3)
    {
      std::cout << "Step2 - out Polygon ERROR " << polid << std::endl;
      pt_out.clear();
      continue;
    }

    te::gm::Polygon *pol_new = new te::gm::Polygon(0, te::gm::PolygonType, m_srid);
    te::gm::LinearRing *pol_ring = new te::gm::LinearRing(pt_out.size() + 1, te::gm::LineStringType, m_srid);
    size_t pp;
    for (pp = 0; pp < pt_out.size(); pp++)
    {
      pol_ring->setPointN(pp, te::gm::Point(pt_out[pp].getX(), pt_out[pp].getY(), m_srid));
      m_nodetree_step2->insert(te::gm::Coord2D(pt_out[pp].getX(), pt_out[pp].getY()), std::pair<int32_t, int32_t>(polid, (int32_t)pp));
    }

    pol_ring->setPointN(pp, te::gm::Point(pt_out[0].getX(), pt_out[0].getY(), m_srid));

    pt_out.clear();

    pol_new->push_back(pol_ring);
    polsout.push_back(std::pair<int32_t, te::gm::Polygon *>(polid, pol_new));
  } // Para cada poligono do desflorestamento

  SavePol(polsout, "step2");

  m_pol_points.clear();

  return true;
}

bool GAP::step3(std::vector< std::pair< size_t, te::gm::Polygon* > > &pols, std::vector< std::pair< size_t, te::gm::Polygon* > > &polsout)
{
  std::vector<KD_NODE*> reportsnode;
  std::vector<te::gm::Coord2D> pt_out;
  std::map< double, std::pair<te::gm::Coord2D, te::gm::Coord2D> > map_inter;

  te::sam::kdtree::Index< te::sam::kdtree::Node< te::gm::Coord2D, GAP_pt_extern, GAP_pt_extern > > *kdtree_int;
  std::vector< te::sam::kdtree::Node< te::gm::Coord2D, GAP_pt_extern, GAP_pt_extern > *> reportsnode_int;

  std::vector<int32_t> pts_to_remove;

  for (std::vector< std::pair<size_t, te::gm::Polygon*> >::iterator it = pols.begin(); it != pols.end(); it++)  // Para cada poligono do desflorestamento
  {
    te::gm::Polygon *pol = it->second;
    int32_t polid = (int32_t)it->first;
    te::gm::LinearRing *lin = dynamic_cast<te::gm::LinearRing*>(pol->getExteriorRing());
    if (lin->getNPoints() < 4)
    {
      for (size_t i = 0; i < lin->getNPoints() - 1; i++)
        std::cout << "ERROR " << polid << " " << lin->getPointN(i)->getX() << " " << lin->getPointN(i)->getY() << std::endl;
      continue;
    }

    te::gm::Coord2D *cent = pol->getCentroidCoord();
    if (!cent) //poligono bixado
    {
      for (size_t i = 0; i < lin->getNPoints() - 1; i++)
        std::cout << "ERROR " << polid << " " << lin->getPointN(i)->getX() << " " << lin->getPointN(i)->getY() << std::endl;
      continue;
    }

    double area = pol->getArea();
    if (area <= 0)
    {
      std::cout << "ERROR " << polid << " area " <<  area << std::endl;
      continue;
    }

    te::gm::Envelope e;
    kdtree_int = new te::sam::kdtree::Index< te::sam::kdtree::Node< te::gm::Coord2D, GAP_pt_extern, GAP_pt_extern > >(e);
    size_t i;
    //Procurar pontos do agregado que estejam na proximidade e externos ao desflorestamento
    for (i = 0; i < lin->getNPoints() - 1; i++) //Cria segmento com os pontos da borda (i ~ i+1)
    {
      te::gm::Coord2D first(lin->getCoordinates()[i]);
      te::gm::Coord2D last(lin->getCoordinates()[i + 1]);
      te::gm::Line *line = new te::gm::Line(te::gm::LineStringType, m_srid);
      line->setCoord(0, first.getX(), first.getY());
      line->setCoord(1, last.getX(), last.getY());

      double dist01 = Distance(first, last);
      if (dist01 == 0)
        continue;

      te::gm::Envelope ept(line->getMBR()->getLowerLeftX() - m_dist_min, line->getMBR()->getLowerLeftY() - m_dist_min,
        line->getMBR()->getUpperRightX() + m_dist_min, line->getMBR()->getUpperRightY() + m_dist_min);
      m_nodetree->search(ept, reportsnode);

      for (size_t j = 0; j < reportsnode.size(); j++)
      {
        te::gm::Coord2D pint;
        te::gm::Coord2D c = reportsnode[j]->getKey();

        double distsc = coordToSegmentDistance(first, last, c, &pint); //Distance agregado point to desflorestamento segment
        if (distsc == 0.) continue;

        if (distsc < m_dist_min)
        {
          kdtree_int->search(te::gm::Envelope(c.x, c.y, c.x, c.y), reportsnode_int);
          if (reportsnode_int.size())
          {
            if (distsc <= reportsnode_int[0]->getData().m_dist)
            {
              if (!(Distance(pint, first) == 0. || Distance(pint, last) == 0.)) //Ponto de interseccao é o proprio ponto do segmento - ignora
              if (fabs(distsc - reportsnode_int[0]->getData().m_dist) < m_tol
                && (i - reportsnode_int[0]->getData().m_seg_id) == 1)
                //ponto do agregado exatamente entre dois segmentos consecutivos -> junta esses segmentos
              {
                pts_to_remove.push_back((int32_t)i);
              }
              else
                kdtree_int->insert(c, GAP_pt_extern(pint, (int32_t)i, distsc));
            }
            reportsnode_int.clear();
            continue;
          }

         // double dist0int = Distance(first, pint);
          double dist0c = Distance(first, c);
          if (/*dist01 > dist0c && */dist0c != 0) //pega só os pontos dentro da área de abrangencia do segmento (se o ponto estiver além do last não usa
          {
            if (Distance(pint, first) == 0. || Distance(pint, last) == 0.) //Ponto de interseccao é o proprio ponto do segmento - ignora
              continue;

           // if (isInside(first, last, c, *cent)) //So usa pontos fora do pol
            te::gm::Point pc(c.x, c.y, m_srid);
            if (pol->disjoint(dynamic_cast<te::gm::Geometry*>(&pc)))
              kdtree_int->insert(c, GAP_pt_extern(pint, (int32_t)i, distsc));

            reportsnode_int.clear();
          }
        }
      } // for (size_t j = 0; j < reportsnode.size(); j++)
      reportsnode.clear();
    } //for (i = 0; i < lin->getNPoints() - 1; i++)

    //Adds intersection points
    for (i = 0; i < lin->getNPoints() - 1; i++) //Cria segmento com os pontos da borda (i ~ i+1)
    {
      if (std::find(pts_to_remove.begin(), pts_to_remove.end(), i) != pts_to_remove.end()) //Este ponto não entra -> vide caso de juntar 2 segmentos
        continue;

      te::gm::Coord2D first(lin->getCoordinates()[i]);
      te::gm::Coord2D last(lin->getCoordinates()[i + 1]);
      te::gm::Line *line = new te::gm::Line(te::gm::LineStringType, m_srid);
      line->setCoord(0, first.getX(), first.getY());
      line->setCoord(1, last.getX(), last.getY());

      //if (std::find(pt_out.begin(), pt_out.end(), first) == pt_out.end()) //Não coloca pontos repetidos
      {
        m_pol_points.push_back(GAP_inter(polid, (int32_t)i, first.getX(), first.getY(), first.getX(), first.getY(), lin->getPointN(i)->getX(), lin->getPointN(i)->getY(),
          0., 0., "desf", true));
        pt_out.push_back(te::gm::Coord2D(first));
      }

      double dist01 = Distance(first, last);

      te::gm::Envelope ept(line->getMBR()->getLowerLeftX() - m_dist_min, line->getMBR()->getLowerLeftY() - m_dist_min,
        line->getMBR()->getUpperRightX() + m_dist_min, line->getMBR()->getUpperRightY() + m_dist_min);
      kdtree_int->search(ept, reportsnode_int);

      if (reportsnode_int.size())
      {
        te::gm::Coord2D k_pint = reportsnode_int[0]->getData().m_ptint;
        double k_dist = reportsnode_int[0]->getData().m_dist;
        std::vector<size_t> k_sel;
        size_t j_sel;
        //Procurar pontos de intersecao repetidos,se achar usar o pont do agregado mais distante
        for (size_t j = 0; j < reportsnode_int.size(); j++)
        {
          if (reportsnode_int[j]->getData().m_seg_id == (int32_t)i)
          {
            te::gm::Coord2D k_pint = reportsnode_int[j]->getData().m_ptint;
            double k_dist = reportsnode_int[j]->getData().m_dist;
            j_sel = j;

            for (size_t jj = j; jj < reportsnode_int.size(); jj++)
            {
              if (Distance(k_pint, reportsnode_int[jj]->getData().m_ptint) < m_tol) //Achou mesmo ponto de interseccao
              {
                if (reportsnode_int[jj]->getData().m_dist > k_dist)
                {
                  k_dist = reportsnode_int[jj]->getData().m_dist;
                  j_sel = jj;
                }
              }
            }
            k_sel.push_back(j_sel);
          }
        }

        for (size_t j = 0; j < k_sel.size(); j++)
        {
          if (reportsnode_int[k_sel[j]]->getData().m_dist != 0)
          {
            double dist0int = Distance(first, reportsnode_int[k_sel[j]]->getData().m_ptint);
            if (dist0int < dist01)
            {
              map_inter.insert(std::pair< double, std::pair<te::gm::Coord2D, te::gm::Coord2D> >
              (dist0int, std::pair<te::gm::Coord2D, te::gm::Coord2D>(reportsnode_int[k_sel[j]]->getKey(), reportsnode_int[k_sel[j]]->getData().m_ptint)));
            }
          }
        }
      }

      for (std::map< double, std::pair<te::gm::Coord2D, te::gm::Coord2D> >::iterator it = map_inter.begin(); it != map_inter.end(); it++)
      {
        te::gm::Coord2D c = (*it).second.first;
        te::gm::Coord2D pint = (*it).second.second;
        bool inside = false;
        if (Distance(*cent, c) < Distance(*cent, pint))
          inside = true;
       // if (std::find(pt_out.begin(), pt_out.end(), c) == pt_out.end()) //Não coloca pontos repetidos
        {
          pt_out.push_back(te::gm::Coord2D(c));
          m_pol_points.push_back(GAP_inter(polid, (int32_t)i, first.getX(), first.getY(), pint.getX(), pint.getY(), c.getX(), c.getY(),
            Distance(first, c), dist01, "agreg", inside));
        }
      }
      map_inter.clear();
      reportsnode_int.clear();
    }

    pt_out.push_back(te::gm::Coord2D(lin->getCoordinates()[i]));

    delete kdtree_int;
    pts_to_remove.clear();

    if (pt_out.size() < 3 || (pt_out.size() == 3 && pt_out[0] == pt_out[2]))
    {
      pt_out.clear();
      std::cout << "ERROR " << polid << std::endl;
      continue;
    }

    te::gm::Polygon *pol_new = new te::gm::Polygon(0, te::gm::PolygonType, m_srid);
    te::gm::LinearRing *pol_ring = new te::gm::LinearRing(pt_out.size() + 1, te::gm::LineStringType, m_srid);
    size_t pp;
    for (pp = 0; pp < pt_out.size(); pp++)
    {
     // std::cout << polid << " " << pp << " " << pt_out[pp].getX() << " " << pt_out[pp].getY() << std::endl;
      pol_ring->setPointN(pp, te::gm::Point(pt_out[pp].getX(), pt_out[pp].getY(), m_srid));
    }

    pol_ring->setPointN(pp, te::gm::Point(pt_out[0].getX(), pt_out[0].getY(), m_srid));
    pt_out.clear();

    if (pol_ring->size() <= 3)
    {
      std::cout << "ERROR " << polid << std::endl;
      delete pol_ring;
      continue;
    }
    pol_new->push_back(pol_ring);
    if (pol_new->getArea() <= 0)
    {
      std::cout << "ERROR sem area " << polid << std::endl;
      delete pol_new;
      continue;
    }

    te::gm::Geometry* result = te::gm::Validate((dynamic_cast<te::gm::Geometry*>(pol_new)));

    if (result->getGeomTypeId() == te::gm::MultiPolygonType)
    {
      std::cout << "Step3 - Divide " << polid << std::endl;
      te::gm::MultiPolygon* mp = dynamic_cast<te::gm::MultiPolygon*>(result);
      for (size_t i = 0; i < mp->getNumGeometries(); i++)
        polsout.push_back(std::pair<size_t, te::gm::Polygon*>(polid, dynamic_cast<te::gm::Polygon*>(mp->getGeometryN(i))));
    }
    else
      polsout.push_back(std::pair<size_t, te::gm::Polygon*>(polid, pol_new));

  } //for (size_t p = 0; p < m_pols_2.size(); p++)


  return true;

}


// Move os pontos do desflorestamento para a borda do agregado (junta pelos vertices)
bool GAP::step4(std::vector< std::pair< size_t, te::gm::Polygon* > > &pols, std::vector< std::pair< size_t, te::gm::Polygon* > > &polsout)
{
  std::vector<te::gm::Line*> reportline, reportline_pol;
  std::vector<te::gm::Coord2D> pol_points;
  std::map<double, te::gm::Coord2D> map_inter;

  for (std::vector< std::pair<size_t, te::gm::Polygon*> >::iterator it = pols.begin(); it != pols.end(); it++)  // Para cada poligono do desflorestamento
  {
    te::gm::Polygon *pol = it->second;
    int32_t polid = (int32_t)it->first;
    te::gm::LinearRing *lin = dynamic_cast<te::gm::LinearRing*>(pol->getExteriorRing());

    te::gm::Coord2D *cent = pol->getCentroidCoord();
    double area = pol->getArea();
    if (area < (m_dist_min*m_dist_min) || !cent) //No conseguiu gerar centroide e area menor que tolerancia
    {
      std::cout << "Polygon ERROR " << polid << " area " << area << std::endl;
      continue;
    }

    te::sam::rtree::Index<te::gm::Line*> rtree_pol; //rtree com os segmentos deste poligono
    Polygon2RTree(pol, rtree_pol);

    for (size_t i = 0; i < lin->getNPoints() - 1; i++) //Para cada segmento do desflorestamento
    {
      te::gm::Coord2D pt0 = lin->getCoordinates()[i];
      te::gm::Coord2D pt1 = lin->getCoordinates()[i + 1];
      double dist01 = Distance(pt0, pt1);  //comprimento do segmento do desflorestamento
      if (dist01 == 0)
        continue;

      te::gm::Line *line_desf = new te::gm::Line(*lin->getPointN(i), *lin->getPointN(i + 1), te::gm::LineStringType, m_srid);
      te::gm::Envelope ept(line_desf->getMBR()->getLowerLeftX() - m_dist_min, line_desf->getMBR()->getLowerLeftY() - m_dist_min,
        line_desf->getMBR()->getUpperRightX() + m_dist_min, line_desf->getMBR()->getUpperRightY() + m_dist_min);
      m_rtree.search(ept, reportline); //procura segmentos do agregado

      double dist, dist_aux;
      if (!reportline.size()) //nao tem agregado proximo --> nao mexe no vertice
      {
        pol_points.push_back(pt0);
        m_pol_points.push_back(GAP_inter(polid, (int32_t)i, pt0.getX(), pt0.getY(), pt0.getX(), pt0.getY(), pt0.getX(), pt0.getY(),
          0., 0., "desf", true));
        continue;
      }
      else
      {
        bool addpt = false;
        te::gm::Coord2D ptint, ptint_aux;
        size_t seg_sel = 0;
        dist = coordToSegmentDistance(reportline[0]->getCoordinates()[0], reportline[0]->getCoordinates()[1], pt0, &ptint);

        //Pegar apenas o segmento do agregado mais proximo do p0 do desflorestamento e que naõ tenha sido usado anteriormente
        for (size_t rl = 1; rl < reportline.size(); rl++)
        {
          dist_aux = coordToSegmentDistance(reportline[rl]->getCoordinates()[0], reportline[rl]->getCoordinates()[1], pt0, &ptint_aux);
          if (dist_aux < dist)
          {
            dist = dist_aux;
            seg_sel = rl;
            ptint = ptint_aux;
          }
        }

        te::gm::Line *l_agreg = reportline[seg_sel];

        if (dist <= m_dist_min)
        {
          double distpt0 = Distance(pt0, reportline[seg_sel]->getCoordinates()[0]);  //distancia de p0 do segmento do agregado do p0 do segmento do desflorestamento
          double distpt1 = Distance(pt0, reportline[seg_sel]->getCoordinates()[1]);  //distancia de p1 do segmento do agregado do p0 do segmento do desflorestamento
          if (distpt0 < distpt1 && distpt0 < m_dist_min) //p0 mais proximo;
          {
            te::gm::Point pc(reportline[seg_sel]->getCoordinates()[0].x, reportline[seg_sel]->getCoordinates()[0].y, m_srid);
            if (pol->disjoint(dynamic_cast<te::gm::Geometry*>(&pc)))
            {
              te::gm::Line segp0pint(te::gm::Point(pt0.x, pt0.y), pc, te::gm::LineStringType, m_srid);
              te::gm::Envelope ept(segp0pint.getMBR()->getLowerLeftX() - m_tol, segp0pint.getMBR()->getLowerLeftY() - m_tol,
                segp0pint.getMBR()->getUpperRightX() + m_tol, segp0pint.getMBR()->getUpperRightY() + m_tol);
              rtree_pol.search(ept, reportline_pol); //procura segmentos do poligono
              bool temoutroseg = false;
              for (size_t pl = 0; pl < reportline_pol.size(); pl++)
              {
                if (!(reportline_pol[pl]->getCoordinates()[0] == pt0) && !(reportline_pol[pl]->getCoordinates()[1] == pt0)) //Não testa com ele mesmo
                {
                  if (!reportline_pol[pl]->disjoint(dynamic_cast<te::gm::Geometry*>(&segp0pint)))
                    temoutroseg = true;
                }
              }

              reportline_pol.clear();
              if (!temoutroseg)
              {
                pol_points.push_back(reportline[seg_sel]->getCoordinates()[0]);
                m_pol_points.push_back(GAP_inter(polid, (int32_t)i, pt0.getX(), pt0.getY(), ptint.getX(), ptint.getY(), reportline[seg_sel]->getCoordinates()[0].getX(), reportline[seg_sel]->getCoordinates()[0].getY(),
                  dist, distpt0, "agreg0", true));
                addpt = true;
              }
            }
          }
          else if (distpt1 < distpt0 && distpt1 < m_dist_min) //p1 mais proximo;
          {
            te::gm::Point pc(reportline[seg_sel]->getCoordinates()[1].x, reportline[seg_sel]->getCoordinates()[1].y, m_srid);
            if (pol->disjoint(dynamic_cast<te::gm::Geometry*>(&pc)))
            {
              te::gm::Line segp0pint(te::gm::Point(pt0.x, pt0.y), pc, te::gm::LineStringType, m_srid);
              te::gm::Envelope ept(segp0pint.getMBR()->getLowerLeftX() - m_tol, segp0pint.getMBR()->getLowerLeftY() - m_tol,
                segp0pint.getMBR()->getUpperRightX() + m_tol, segp0pint.getMBR()->getUpperRightY() + m_tol);
              rtree_pol.search(ept, reportline_pol); //procura segmentos do poligono
              bool temoutroseg = false;
              for (size_t pl = 0; pl < reportline_pol.size(); pl++)
              {
                if (!(reportline_pol[pl]->getCoordinates()[0] == pt0) && !(reportline_pol[pl]->getCoordinates()[1] == pt0)) //Não testa com ele mesmo
                {
                  if (!reportline_pol[pl]->disjoint(dynamic_cast<te::gm::Geometry*>(&segp0pint)))
                    temoutroseg = true;
                }
              }

              reportline_pol.clear();
              if (!temoutroseg)
              {
                pol_points.push_back(reportline[seg_sel]->getCoordinates()[1]);
                m_pol_points.push_back(GAP_inter(polid, (int32_t)i, pt0.getX(), pt0.getY(), ptint.getX(), ptint.getY(), reportline[seg_sel]->getCoordinates()[1].getX(), reportline[seg_sel]->getCoordinates()[1].getY(),
                  dist, distpt1, "agreg1", true));
                addpt = true;
              }
            }
          }
          if (!addpt)
          {
            double distpt0_int = Distance(pt0, ptint);
            if (distpt0_int < m_dist_min)
            {
              te::gm::Point pc(ptint.x, ptint.y, m_srid);

              if (pointLocate(ptint, pol) != geos::geom::Location::INTERIOR)
                //double distlin_pint = coordToSegmentDistance(pt0, pt1, ptint, 0);
              //if (!pol->contains(dynamic_cast<te::gm::Geometry*>(&pc)) || distlin_pint <= m_tol) //Inserts if point is not inside or is boundary
                // if (!isInside(pt0, pt1, ptint, *cent))
                //if (pol->disjoint(dynamic_cast<te::gm::Geometry*>(&pc)))
              {
                te::gm::Line segp0pint(te::gm::Point(pt0.x, pt0.y), pc, te::gm::LineStringType, m_srid);
                te::gm::Envelope ept(segp0pint.getMBR()->getLowerLeftX() - m_tol, segp0pint.getMBR()->getLowerLeftY() - m_tol,
                  segp0pint.getMBR()->getUpperRightX() + m_tol, segp0pint.getMBR()->getUpperRightY() + m_tol);
                rtree_pol.search(ept, reportline_pol); //procura segmentos do poligono
                bool temoutroseg = false;
                for (size_t pl = 0; pl < reportline_pol.size(); pl++)
                {
                  if (!(reportline_pol[pl]->getCoordinates()[0] == pt0) && !(reportline_pol[pl]->getCoordinates()[1] == pt0)) //Não testa com ele mesmo
                  {
                    if (reportline_pol[pl]->intersects(dynamic_cast<te::gm::Geometry*>(&segp0pint)))
                      temoutroseg = true;
                  }
                }

                reportline_pol.clear();
                if (!temoutroseg)
                {
                  pol_points.push_back(ptint);
                  m_pol_points.push_back(GAP_inter(polid, (int32_t)i, pt0.getX(), pt0.getY(), ptint.getX(), ptint.getY(), ptint.getX(), ptint.getY(),
                    dist, 0., "inter", true));
                  addpt = true;
                }
              }
              else //calcula intersecção dos segmentos
              {
                if (segInterPoint(reportline[seg_sel]->getCoordinates()[0], reportline[seg_sel]->getCoordinates()[1], pt0, pt1, &ptint))
                {
                  te::gm::Line segp0pint(te::gm::Point(pt0.x, pt0.y), pc, te::gm::LineStringType, m_srid);
                  rtree_pol.search(*segp0pint.getMBR(), reportline_pol); //procura segmentos do poligono
                  bool temoutroseg = false;
                  for (size_t pl = 0; pl < reportline_pol.size(); pl++)
                  {
                    if (!(reportline_pol[pl]->getCoordinates()[0] == pt0) && !(reportline_pol[pl]->getCoordinates()[1] == pt0)) //Não testa com ele mesmo
                    {
                      if (!reportline_pol[pl]->disjoint(dynamic_cast<te::gm::Geometry*>(&segp0pint)))
                        temoutroseg = true;
                    }
                  }

                  reportline_pol.clear();
                  if (!temoutroseg)
                  {
                    pol_points.push_back(ptint);
                    m_pol_points.push_back(GAP_inter(polid, (int32_t)i, pt0.getX(), pt0.getY(), ptint.getX(), ptint.getY(), ptint.getX(), ptint.getY(),
                      dist, 0., "inter", true));
                    addpt = true;
                  }
                }
              }
            }
          }
        } // if (dist <= m_dist_min)
        if (!addpt)
        {
          pol_points.push_back(pt0);
          m_pol_points.push_back(GAP_inter(polid, (int32_t)i, pt0.getX(), pt0.getY(), pt0.getX(), pt0.getY(), pt0.getX(), pt0.getY(),
            0., 0., "desf", true));
        }
        reportline.clear();
      } //if (!reportline.size()) ... else
    } //Para cada segmento do desflorestamento

    //std::unique(pol_points.begin(), pol_points.end());

    if (pol_points.size() < 3)
    {
      std::cout << "Step4 - out Polygon ERROR " << polid << std::endl;
      pol_points.clear();
      continue;
    }

    te::gm::Polygon *pol_new = new te::gm::Polygon(0, te::gm::PolygonType, m_srid);
    te::gm::LinearRing *pol_ring = new te::gm::LinearRing(pol_points.size() + 1, te::gm::LineStringType, m_srid);
    size_t pp;
    for (pp = 0; pp < pol_points.size(); pp++)
      pol_ring->setPointN(pp, te::gm::Point(pol_points[pp].getX(), pol_points[pp].getY(), m_srid));

    pol_ring->setPointN(pp, te::gm::Point(pol_points[0].getX(), pol_points[0].getY(), m_srid));

    pol_points.clear();

    pol_new->push_back(pol_ring);
    if (pol_new->getArea() <= 0)
    {
      std::cout << "Step4 - ERROR " << polid << std::endl;
      delete pol_new;
    }
    else
      polsout.push_back(std::pair<int32_t, te::gm::Polygon *>(polid, pol_new));
  } // Para cada poligono do desflorestamento

  return true;
}



bool GAP::SavePol(std::vector< std::pair<size_t, te::gm::Polygon*> >& pols, std::string step)
{
  std::string pol("poly_all");
  std::string pt("point_all");
  std::string dir("D:/Dados_GAP/");
  m_filenameout = dir + pol + step + ".shp";
  m_inDSout = pol + step;
  std::map<std::string, std::string> tgrpol;
  tgrpol["URI"] = m_filenameout;
  tgrpol["DRIVER"] = "ESRI Shapefile";
  te::da::DataSourcePtr Dspol(te::da::DataSourceFactory::make("OGR"));
  Dspol->setConnectionInfo(tgrpol);
  Dspol->open();
  std::string polDS;
  polDS = pol + step;
  if (Dspol->dataSetExists(polDS))
  {
    std::cout << "A dataset with the same requested output dataset name already exists: " << polDS << std::endl;
    return false;
  }

  std::auto_ptr<te::da::DataSetType> dtp(new te::da::DataSetType(polDS));
  te::dt::SimpleProperty* p0 = new te::dt::SimpleProperty("FID", te::dt::INT32_TYPE);
  p0->setAutoNumber(true);
  te::dt::SimpleProperty* pidori = new te::dt::SimpleProperty("FIDoriginal", te::dt::INT32_TYPE);
  te::dt::SimpleProperty* pori = new te::dt::SimpleProperty("Areaoriginal", te::dt::DOUBLE_TYPE);
  te::dt::SimpleProperty* pnew = new te::dt::SimpleProperty("Areanew", te::dt::DOUBLE_TYPE);
  te::gm::GeometryProperty* p1 = new te::gm::GeometryProperty("geometry", 0, te::gm::PolygonType, true);
  p1->setSRID(m_srid);
  dtp->add(p0);
  dtp->add(pidori);
  dtp->add(pori);
  dtp->add(pnew);
  dtp->add(p1);
  te::mem::DataSet* dsp = new te::mem::DataSet(dtp.get());

  std::string filenamept;
  filenamept = dir + pt + step + ".shp";
  std::map<std::string, std::string> tgrpt;
  tgrpt["URI"] = filenamept;
  tgrpt["DRIVER"] = "ESRI Shapefile";
  te::da::DataSourcePtr Dspt(te::da::DataSourceFactory::make("OGR"));
  Dspt->setConnectionInfo(tgrpt);
  Dspt->open();
  std::string ptDS;
  ptDS = pt + step;
  if (Dspt->dataSetExists(ptDS))
  {
    std::cout << "A dataset with the same requested output dataset name already exists: " << ptDS << std::endl;
    return false;
  }

  std::auto_ptr<te::da::DataSetType> dtpt(new te::da::DataSetType(ptDS));
  te::dt::SimpleProperty* prop0 = new te::dt::SimpleProperty("FID", te::dt::INT32_TYPE);
  prop0->setAutoNumber(true);
  te::dt::SimpleProperty* propid = new te::dt::SimpleProperty("POLID", te::dt::INT32_TYPE);
  te::dt::SimpleProperty* propptid = new te::dt::SimpleProperty("ptorder", te::dt::INT32_TYPE);
  te::dt::SimpleProperty* propdist0 = new te::dt::SimpleProperty("dist0", te::dt::DOUBLE_TYPE);
  te::dt::SimpleProperty* propdist00 = new te::dt::SimpleProperty("dist00", te::dt::DOUBLE_TYPE);
  te::dt::SimpleProperty* propx = new te::dt::SimpleProperty("x", te::dt::DOUBLE_TYPE);
  te::dt::SimpleProperty* propy = new te::dt::SimpleProperty("y", te::dt::DOUBLE_TYPE);
  te::dt::SimpleProperty* propxi = new te::dt::SimpleProperty("xint", te::dt::DOUBLE_TYPE);
  te::dt::SimpleProperty* propyi = new te::dt::SimpleProperty("yint", te::dt::DOUBLE_TYPE);
  te::dt::SimpleProperty* propxn = new te::dt::SimpleProperty("newx", te::dt::DOUBLE_TYPE);
  te::dt::SimpleProperty* propyn = new te::dt::SimpleProperty("newy", te::dt::DOUBLE_TYPE);
  te::dt::SimpleProperty* propt = new te::dt::SimpleProperty("tipo", te::dt::STRING_TYPE);
  te::dt::SimpleProperty* propr = new te::dt::SimpleProperty("inside", te::dt::INT32_TYPE);
  te::gm::GeometryProperty* prop1 = new te::gm::GeometryProperty("pt", 0, te::gm::PointType, true);
  prop1->setSRID(m_srid);
  dtpt->add(prop0);
  dtpt->add(propid);
  dtpt->add(propptid);
  dtpt->add(propdist0);
  dtpt->add(propdist00);
  dtpt->add(propx);
  dtpt->add(propy);
  dtpt->add(propxi);
  dtpt->add(propyi);
  dtpt->add(propxn);
  dtpt->add(propyn);
  dtpt->add(propt);
  dtpt->add(propr);
  dtpt->add(prop1);
  te::mem::DataSet* dspt = new te::mem::DataSet(dtpt.get());

  int32_t id = 0;
  for (std::vector< std::pair<size_t, te::gm::Polygon*> >::iterator it = pols.begin(); it != pols.end(); it++)
  {
    te::gm::Polygon *pol = it->second;
    te::mem::DataSetItem* dataSetItem = new te::mem::DataSetItem(dsp);
    dataSetItem->setInt32("FID", id++);
    dataSetItem->setInt32("FIDoriginal", (int32_t)it->first);
    dataSetItem->setDouble("Areaoriginal", m_original_area[(int32_t)it->first]);
    dataSetItem->setDouble("Areanew", pol->getArea());
    dataSetItem->setGeometry("geometry", dynamic_cast<te::gm::Geometry*>(pol->clone()));
    dsp->add(dataSetItem);

 //   te::gm::LinearRing *lin = dynamic_cast<te::gm::LinearRing*>(pols[ii]->getExteriorRing());

  }

    for (size_t pp = 0; pp < m_pol_points.size(); pp++)
    {
      te::mem::DataSetItem* dataSetItem = new te::mem::DataSetItem(dspt);
      dataSetItem->setInt32("FID", (int32_t)pp);
      dataSetItem->setInt32("POLID", m_pol_points[pp].m_polid);
      dataSetItem->setInt32("ptorder", m_pol_points[pp].m_ptid);
      dataSetItem->setDouble("dist0", m_pol_points[pp].m_distance);
      dataSetItem->setDouble("dist00", m_pol_points[pp].m_distance0);
      dataSetItem->setDouble("x", m_pol_points[pp].m_x);
      dataSetItem->setDouble("y", m_pol_points[pp].m_y);
      dataSetItem->setDouble("xint", m_pol_points[pp].m_xint);
      dataSetItem->setDouble("yint", m_pol_points[pp].m_yint);
      dataSetItem->setDouble("newx", m_pol_points[pp].m_xnew);
      dataSetItem->setDouble("newy", m_pol_points[pp].m_ynew);
      dataSetItem->setString("tipo", m_pol_points[pp].m_tipo);
      dataSetItem->setInt32("inside", (m_pol_points[pp].m_inside) ? 0 : 1);
      dataSetItem->setGeometry("pt", dynamic_cast<te::gm::Geometry*>(new te::gm::Point(m_pol_points[pp].m_xnew, m_pol_points[pp].m_ynew)));
      dspt->add(dataSetItem);
    }
  Save(Dspol.get(), dsp, dtp.get());
  Save(Dspt.get(), dspt, dtpt.get());

 // pols.clear();


  return true;

}

//verifing and removing loops in borderline
void GAP::verify_polygon(std::vector<te::gm::Coord2D> &pt_out)
{
  te::gm::Line l0(te::gm::LineStringType, m_srid);
  te::gm::Line l1(te::gm::LineStringType, m_srid);
  te::gm::Point pint;

  double x0 = pt_out[0].x; //Initial point 
  double y0 = pt_out[0].y;

  for (size_t i = 0; i < pt_out.size() - 1; i++)
  {
    l0.setCoord(0, pt_out[i].x, pt_out[i].y);
    l0.setCoord(1, pt_out[i+1].x, pt_out[i+1].y);
    for (size_t ii = i; ii < pt_out.size() - 1; ii++)
    {
      l1.setCoord(0, pt_out[ii].x, pt_out[ii].y);
      l1.setCoord(1, pt_out[ii + 1].x, pt_out[ii + 1].y);
      if (l0.intersection(l1, pint))
      {
        if ((pint.getX() == l0.getCoordinates()[0].getX() && pint.getY() == l0.getCoordinates()[0].getY()) ||
          (pint.getX() == l0.getCoordinates()[1].getX() && pint.getY() == l0.getCoordinates()[1].getY()) ||
          pint.getX() == x0 && pint.getY() == y0)
          continue;
        else //remove points that tracing the loop
        {
          for (size_t j = i + 1; (j < ii && j < pt_out.size()-1); j++)
            pt_out.erase(pt_out.begin() + j);
          break;
        }
      }
    }
  }
}

bool GAP::verifyIntersections(te::gm::Coord2D &pt0, te::gm::Coord2D &ptint, te::sam::rtree::Index<te::gm::Line*> &rtree_pol)
{
  std::vector<te::gm::Line*> reportline_pol;
  bool temoutroseg = false;
  size_t n_pts = 0;

  te::gm::Point pc(ptint.x, ptint.y, m_srid);
  te::gm::Line segp0pint(te::gm::Point(pt0.x, pt0.y), pc, te::gm::LineStringType, m_srid);
  rtree_pol.search(*segp0pint.getMBR(), reportline_pol); //procura segmentos do poligono
  for (size_t pl = 0; pl < reportline_pol.size(); pl++)
  {
    if (!(reportline_pol[pl]->getCoordinates()[0] == pt0) && !(reportline_pol[pl]->getCoordinates()[1] == pt0)) //Não testa com ele mesmo
    {
      if (reportline_pol[pl]->intersects(dynamic_cast<te::gm::Geometry*>(&segp0pint)))
      {
        te::gm::Coord2D pint_agreg;
        te::gm::Coord2D p0 = reportline_pol[pl]->getCoordinates()[0];
        te::gm::Coord2D p1 = reportline_pol[pl]->getCoordinates()[1];
        double dist = coordToSegmentDistance(p0, p1, ptint, &pint_agreg);
        if (pint_agreg == p0 || pint_agreg == p1)
        {
          n_pts++;
          continue;
        }
        if (dist < segp0pint.getLength())
          temoutroseg = true;
      }
    }
  }

  if (n_pts>1)
    temoutroseg = true;

  reportline_pol.clear();
  return temoutroseg;

}


int GAP::pointLocate(te::gm::Coord2D &pt, te::gm::Polygon *pol)
{
  const geos::geom::Coordinate p(pt.x, pt.y);
  std::auto_ptr<geos::geom::Geometry> geos_pol(te::gm::GEOSWriter::write(dynamic_cast<te::gm::Geometry*>(pol)));
  geos::geom::GeometryFactory factory;
  geos::geom::Point* geos_pt = factory.createPoint(p);
  double dist = geos_pol->getBoundary()->distance(geos_pt);
  delete geos_pt;
  if (dist < 0.1e-12)
    return geos::geom::Location::BOUNDARY;

  geos::algorithm::PointLocator loc;
  return loc.locate(p, geos_pol.get());
}
