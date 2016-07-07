
#include "../Config.h"
#include "GAP.h"

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
        *pix = lx;
        *piy = ly;
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
  double pix, piy;
  double dist = SegmentDistance(fseg.getX(), fseg.getY(), lseg.getX(), lseg.getY(), pt.getX(), pt.getY(), &pix, &piy);
  if (pti)
  {
    pti->x = pix;
    pti->y = piy;
  }
  return dist;
}

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
  te::gm::LinearRing *lr = dynamic_cast<te::gm::LinearRing*>(pol->getExteriorRing());
  for (size_t i = 0; i < lr->getNPoints() - 1; i++)
  {
    te::gm::Coord2D pt(lr->getPointN(i)->getX(), lr->getPointN(i)->getY());
    m_nodetree->insert(pt, id);
  }
  return true;
}


bool GAP::Polygon2RTree(te::gm::Polygon* pol)
{
  std::vector<te::gm::Line*> reportline;

  te::gm::LinearRing *lr = dynamic_cast<te::gm::LinearRing*>(pol->getExteriorRing());
  for (size_t i = 0; i < lr->getNPoints() - 1; i++)
  {
    te::gm::Point first(*lr->getPointN(i));
    te::gm::Point last(*lr->getPointN(i + 1));
    te::gm::Line *l = new te::gm::Line(first, last, te::gm::LineStringType, pol->getSRID());
    te::gm::Envelope e(*l->getMBR());
    m_rtree.insert(e, l);
  }
  return true;
}

bool GAP::Polygon2Lines(te::gm::Polygon* pol)
{
  te::gm::LinearRing *lr = dynamic_cast<te::gm::LinearRing*>(pol->getExteriorRing());
  for (size_t i = 0; i < lr->getNPoints() - 1; i++)
  {
    te::gm::Point first(*lr->getPointN(i));
    te::gm::Point last(*lr->getPointN(i+1));
    te::gm::Line *l = new te::gm::Line(first, last, te::gm::LineStringType, pol->getSRID());
    m_line.push_back(l);
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
        if (out == Polygon)
          m_pol.insert(std::pair<size_t, te::gm::Polygon*>(inDset->getInt32(id_pos), p));
        if (out == RTree)
          Polygon2RTree(p);
        if (out == Line)
          Polygon2Lines(p);
        if (out == Point)
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
      if (out == Polygon)
        m_pol.insert(std::pair<size_t, te::gm::Polygon*>(inDset->getInt32(id_pos), p));
      if (out == RTree)
        Polygon2RTree(p);
      if (out == Line)
        Polygon2Lines(p);
      if (out == Point)
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
  m_dist_min = 0.0003;
  m_filename1 = "D:/Dados_GAP/agd_6pol.shp";
  m_inDS1 = "agd_6pol";
  m_filename2 = "D:/Dados_GAP/dsf_pol4674.shp";
  m_inDS2 = "dsf_pol4674";

  te::gm::Envelope e;
  m_nodetree = new KD_TREE(e);
}

bool GAP::run()
{
  if (!LoadPolygons(m_filename1, m_inDS1, Point))
  {
    return false;
  }

  if (!LoadPolygons(m_filename2, m_inDS2, Polygon))
  {
    return false;
  }



  std::string filenameResult("D:/Dados_GAP/pp5.shp");

  std::map<std::string, std::string> tgrInfo;
  tgrInfo["URI"] = filenameResult;
  tgrInfo["DRIVER"] = "ESRI Shapefile";

  //std::auto_ptr<te::da::DataSource> trgDs = te::da::DataSourceFactory::make("OGR");
  te::da::DataSourcePtr trgDs(te::da::DataSourceFactory::make("OGR"));
  trgDs->setConnectionInfo(tgrInfo);
  trgDs->open();

  std::string outDS = "pp5";

  if (trgDs->dataSetExists(outDS))
  {
    std::cout << "A dataset with the same requested output dataset name already exists: " << outDS << std::endl;
    return false;
  }


  std::auto_ptr<te::da::DataSetType> dt(new te::da::DataSetType(outDS));

  te::dt::SimpleProperty* prop0 = new te::dt::SimpleProperty("FID", te::dt::INT32_TYPE);
  prop0->setAutoNumber(true);
  te::dt::SimpleProperty* propid = new te::dt::SimpleProperty("POLID", te::dt::INT32_TYPE);
  te::dt::SimpleProperty* propdist = new te::dt::SimpleProperty("distance", te::dt::DOUBLE_TYPE);
  te::dt::SimpleProperty* prop0x = new te::dt::SimpleProperty("xc", te::dt::DOUBLE_TYPE);
  te::dt::SimpleProperty* prop0y = new te::dt::SimpleProperty("yc", te::dt::DOUBLE_TYPE);
  te::gm::GeometryProperty* prop1 = new te::gm::GeometryProperty("pt", 0, te::gm::PointType, true);
  prop1->setSRID(m_srid);
  dt->add(prop0);
  dt->add(propid);
  dt->add(propdist);
  dt->add(prop0x);
  dt->add(prop0y);
  dt->add(prop1);
  te::mem::DataSet* ds = new te::mem::DataSet(dt.get());

  int id = 0;

  std::vector<KD_NODE*> reportsnode;
  std::vector<te::gm::Point *> pt_out;

  for (std::map<size_t, te::gm::Polygon*>::iterator it = m_pol.begin(); it != m_pol.end(); it++)
  {
    te::gm::Polygon *pol = it->second;
    te::gm::LinearRing *lin = dynamic_cast<te::gm::LinearRing*>(pol->getExteriorRing());

    size_t i;
    for (i = 0; i < lin->getNPoints() - 1; i++)
    {
      te::gm::Point first(*lin->getPointN(i));

      te::mem::DataSetItem* dataSetItem = new te::mem::DataSetItem(ds);
      dataSetItem->setInt32("FID", id++);
      dataSetItem->setInt32("POLID", it->first);
      dataSetItem->setDouble("distance", 0.);
      dataSetItem->setDouble("xc", first.getX());
      dataSetItem->setDouble("yc", first.getY());
      dataSetItem->setGeometry("pt", dynamic_cast<te::gm::Geometry*>(first.clone()));
      ds->add(dataSetItem);

     //// pt_out.push_back(dynamic_cast<te::gm::Point*>(lin->getPointN(i)->clone()));
      te::gm::Point last(*lin->getPointN(i + 1));
      te::gm::Line *line = new te::gm::Line(first, last, te::gm::LineStringType, m_srid);

      te::gm::Envelope ept(line->getMBR()->getLowerLeftX() - m_dist_min, line->getMBR()->getLowerLeftY() - m_dist_min,
        line->getMBR()->getUpperRightX() + m_dist_min, line->getMBR()->getUpperRightY() + m_dist_min);

      m_nodetree->search(ept, reportsnode);
      for (size_t j = 0; j < reportsnode.size(); j++)
      {
        te::gm::Coord2D pint;
        te::gm::Coord2D *coords = line->getCoordinates();
        te::gm::Coord2D c = reportsnode[j]->getKey();
        double dist = coordToSegmentDistance(coords[0], coords[1], c, &pint);
        if (dist < m_dist_min)
        {
          //pt_out.push_back(new te::gm::Point(c.getX(), c.getX()));
          te::mem::DataSetItem* dataSetItem = new te::mem::DataSetItem(ds);
          dataSetItem->setInt32("FID", id++);
          dataSetItem->setInt32("POLID", it->first);
          dataSetItem->setDouble("distance", Distance(coords[0], c));
          dataSetItem->setDouble("xc", c.getX());
          dataSetItem->setDouble("yc", c.getY());
          dataSetItem->setGeometry("pt", new te::gm::Point(c.getX(), c.getY(), m_srid));
          ds->add(dataSetItem);
        }
      }
      reportsnode.clear();
      delete line;
    }
   // pt_out.push_back(dynamic_cast<te::gm::Point*>(lin->getPointN(i)->clone()));
    te::mem::DataSetItem* dataSetItem = new te::mem::DataSetItem(ds);
    dataSetItem->setInt32("FID", id++);
    dataSetItem->setInt32("POLID", it->first);
    dataSetItem->setDouble("distance", 0.);
    dataSetItem->setDouble("xc", lin->getPointN(i)->getX());
    dataSetItem->setDouble("yc", lin->getPointN(i)->getY());
    dataSetItem->setGeometry("pt", dynamic_cast<te::gm::Geometry*>(lin->getPointN(i)->clone()));
    ds->add(dataSetItem);

  //  std::auto_ptr<te::gm::Polygon> p(new te::gm::Polygon(0, te::gm::PolygonType));
  //  std::auto_ptr<te::gm::LinearRing> s(new te::gm::LinearRing(pt_out.size(), te::gm::LineStringType));

  //  for (i = 0; i < pt_out.size(); i++)
  //  {
  //    s->setPointN(i, *pt_out[i]);
  //  }

  //  p->push_back(s.get());
  //  p->setSRID(m_srid);

  //  te::mem::DataSetItem* dataSetItem = new te::mem::DataSetItem(ds);
  //  dataSetItem->setInt32("FID", id++);
  //  dataSetItem->setGeometry("pol", (te::gm::Geometry*)p->clone());
  //  ds->add(dataSetItem);
  //  p.release();
  //  pt_out.clear();
  }


  Save(trgDs.get(), ds, dt.get());

  bool result = true;

  //if (!intersectionOp->paramsAreValid())
  //  result = false;
  //else
  //  result = intersectionOp->run();

  //delete intersectionOp;

  return true;
}

bool GAP::run2()
{
  if (!LoadPolygons(m_filename1, m_inDS1, Point))
  {
    return false;
  }

  if (!LoadPolygons(m_filename2, m_inDS2, Line))
  {
    return false;
  }



  std::string filenameResult("D:/Dados_GAP/resultpt9.shp");

  std::map<std::string, std::string> tgrInfo;
  tgrInfo["URI"] = filenameResult;
  tgrInfo["DRIVER"] = "ESRI Shapefile";

  //std::auto_ptr<te::da::DataSource> trgDs = te::da::DataSourceFactory::make("OGR");
  te::da::DataSourcePtr trgDs(te::da::DataSourceFactory::make("OGR"));
  trgDs->setConnectionInfo(tgrInfo);
  trgDs->open();

  std::string outDS = "resultpt9";

  if (trgDs->dataSetExists(outDS))
  {
    std::cout << "A dataset with the same requested output dataset name already exists: " << outDS << std::endl;
    return false;
  }


  std::auto_ptr<te::da::DataSetType> dt(new te::da::DataSetType(outDS));

  te::dt::SimpleProperty* prop0 = new te::dt::SimpleProperty("FID", te::dt::INT32_TYPE);
  prop0->setAutoNumber(true);
  te::dt::SimpleProperty* prop0d = new te::dt::SimpleProperty("Distancesegc", te::dt::DOUBLE_TYPE);
  te::dt::SimpleProperty* prop0d1 = new te::dt::SimpleProperty("Distancec0int", te::dt::DOUBLE_TYPE);
  te::dt::SimpleProperty* propid = new te::dt::SimpleProperty("AgregadoID", te::dt::INT32_TYPE);
  te::dt::SimpleProperty* prop0x = new te::dt::SimpleProperty("xc", te::dt::DOUBLE_TYPE);
  te::dt::SimpleProperty* prop0y = new te::dt::SimpleProperty("yc", te::dt::DOUBLE_TYPE);
  te::dt::SimpleProperty* prop0x0 = new te::dt::SimpleProperty("xc0", te::dt::DOUBLE_TYPE);
  te::dt::SimpleProperty* prop0y0 = new te::dt::SimpleProperty("yc0", te::dt::DOUBLE_TYPE);
  te::dt::SimpleProperty* prop1x0 = new te::dt::SimpleProperty("xc1", te::dt::DOUBLE_TYPE);
  te::dt::SimpleProperty* prop1y0 = new te::dt::SimpleProperty("yc1", te::dt::DOUBLE_TYPE);
  te::dt::SimpleProperty* prop0x1 = new te::dt::SimpleProperty("xint", te::dt::DOUBLE_TYPE);
  te::dt::SimpleProperty* prop0y1 = new te::dt::SimpleProperty("yint", te::dt::DOUBLE_TYPE);
  te::gm::GeometryProperty* prop1 = new te::gm::GeometryProperty("pt", 0, te::gm::PointType, true);
  prop1->setSRID(m_srid);
  dt->add(prop0);
  dt->add(prop0d);
  dt->add(prop0d1);
  dt->add(propid);
  dt->add(prop0x);
  dt->add(prop0y);
  dt->add(prop0x0);
  dt->add(prop0y0);
  dt->add(prop1x0);
  dt->add(prop1y0);
  dt->add(prop0x1);
  dt->add(prop0y1);
  dt->add(prop1);
  te::mem::DataSet* ds = new te::mem::DataSet(dt.get());

  int id = 0;

  std::vector<KD_NODE*> reportsnode;
  // std::map<double, te::gm::Coord2D*> map_inter;
  std::map<double, te::gm::Coord2D*>::iterator it;

  for (size_t i = 0; i < m_line.size(); i++)
  {
    te::gm::Envelope ept(m_line[i]->getMBR()->getLowerLeftX() - m_dist_min, m_line[i]->getMBR()->getLowerLeftY() - m_dist_min,
      m_line[i]->getMBR()->getUpperRightX() + m_dist_min, m_line[i]->getMBR()->getUpperRightY() + m_dist_min);
    m_nodetree->search(ept, reportsnode);
    for (size_t j = 0; j < reportsnode.size(); j++)
    {
      te::gm::Coord2D pint;
      te::gm::Coord2D *coords = m_line[i]->getCoordinates();
      te::gm::Coord2D c = reportsnode[j]->getKey();
      double dist = coordToSegmentDistance(coords[0], coords[1], c, &pint);
      if (dist < m_dist_min)
      {
        // map_inter.insert(std::pair<double, te::gm::Coord2D*>(Distance(coords[0], c), new te::gm::Coord2D(c)));
        te::mem::DataSetItem* dataSetItem = new te::mem::DataSetItem(ds);
        dataSetItem->setInt32("FID", id++);
        dataSetItem->setInt32("AgregadoID", reportsnode[j]->getData());
        dataSetItem->setDouble("Distancesegc", dist);
        dataSetItem->setDouble("Distancec0int", Distance(coords[0], pint));
        dataSetItem->setDouble("xc", c.getX());
        dataSetItem->setDouble("yc", c.getY());
        dataSetItem->setDouble("xc0", coords[0].getX());
        dataSetItem->setDouble("yc0", coords[0].getY());
        dataSetItem->setDouble("xc1", coords[1].getX());
        dataSetItem->setDouble("yc1", coords[1].getY());
        dataSetItem->setDouble("xint", pint.getX());
        dataSetItem->setDouble("yint", pint.getY());
        dataSetItem->setGeometry("pt", (te::gm::Geometry*)new te::gm::Point(pint.getX(), pint.getY()));
        ds->add(dataSetItem);
      }
    }
    reportsnode.clear();
  }

  //std::vector<te::gm::Point*> pts_result;


  //for (it = map_inter.begin(); it !=  map_inter.end(); it++)
  //{
  //  te::mem::DataSetItem* dataSetItem = new te::mem::DataSetItem(ds);
  //  dataSetItem->setInt32("FID", id++);
  //  dataSetItem->setDouble("Distance", it->first);
  //  dataSetItem->setDouble("x", it->second->getX());
  //  dataSetItem->setDouble("y", it->second->getY());
  //  dataSetItem->setGeometry("pt", (te::gm::Geometry*)new te::gm::Point(it->second->getX(), it->second->getY()));
  //  ds->add(dataSetItem);
  //}

  Save(trgDs.get(), ds, dt.get());

  bool result = true;

  //if (!intersectionOp->paramsAreValid())
  //  result = false;
  //else
  //  result = intersectionOp->run();

  //delete intersectionOp;

  return true;
}


bool GAP::run1()
{
  //te::sam::rtree::Index<te::gm::Line*> agregado;
  std::vector<te::gm::Polygon*> agregado;
  te::sam::rtree::Index<te::gm::Line*> desfl;
  std::vector<te::gm::Line*> reportline;
  std::vector<te::gm::Point*> pts_result;

  for (size_t i1 = 0; i1 < agregado.size(); i1++)
  {
    te::gm::Polygon *p1 = agregado[i1];
    te::gm::LinearRing *lr = dynamic_cast<te::gm::LinearRing*>(p1->getExteriorRing());
    te::gm::Envelope e(*lr->getMBR());
    desfl.search(e, reportline);
    for (size_t i2 = 0; i2 < reportline.size(); i2++)
    {
      te::gm::Line *l2 = reportline[i2];
      l2->setSRID(m_srid);
      double dist;
      //double dist = dynamic_cast<te::gm::Geometry*>(p1)->distance(dynamic_cast<te::gm::Geometry*>(l2));
      std::auto_ptr<geos::geom::Geometry> Geom1(te::gm::GEOSWriter::write(dynamic_cast<te::gm::Geometry*>(p1)));
      std::auto_ptr<geos::geom::Geometry> Geom2(te::gm::GEOSWriter::write(dynamic_cast<te::gm::Geometry*>(l2)));
      geos::operation::distance::DistanceOp distop(Geom1.get(), Geom2.get());
      dist = distop.distance();
      if (dist < m_dist_min)
      {
        geos::geom::CoordinateSequence *pts = distop.closestPoints();
        for (size_t ipt = 0; ipt < pts->getSize(); ipt++)
        {
          geos::geom::Coordinate c = pts->getAt(ipt);
          std::cout << c.x << " " << c.y << std::endl;
          te::gm::Point *cp = new te::gm::Point(c.x, c.y);
          pts_result.push_back(cp);
        }
      }
      Geom1.release();
      Geom2.release();
    }

  }
  //for (size_t i2 = 0; i2 < desfl.size(); i2++)
  //{
  //  te::gm::Polygon *p2 = desfl[i2];
  //  double dist = p1->distance(p2);
  //  if (dist < dist_min)
  //  {
  //  }

  //}



  std::string filenameResult("D:/Dados_GAP/resultpt.shp");

  std::map<std::string, std::string> tgrInfo;
  tgrInfo["URI"] = filenameResult;
  tgrInfo["DRIVER"] = "ESRI Shapefile";

  //std::auto_ptr<te::da::DataSource> trgDs = te::da::DataSourceFactory::make("OGR");
  te::da::DataSourcePtr trgDs(te::da::DataSourceFactory::make("OGR"));
  trgDs->setConnectionInfo(tgrInfo);
  trgDs->open();

  std::string outDS = "resultpt";

  if (trgDs->dataSetExists(outDS))
  {
    std::cout << "A dataset with the same requested output dataset name already exists: " << outDS << std::endl;
    return false;
  }

  std::auto_ptr<te::da::DataSetType> dt(new te::da::DataSetType(outDS));

  te::dt::SimpleProperty* prop0 = new te::dt::SimpleProperty("FID", te::dt::INT32_TYPE);
  prop0->setAutoNumber(true);
  te::dt::SimpleProperty* prop0x = new te::dt::SimpleProperty("x", te::dt::DOUBLE_TYPE);
  te::dt::SimpleProperty* prop0y = new te::dt::SimpleProperty("y", te::dt::DOUBLE_TYPE);
  te::gm::GeometryProperty* prop1 = new te::gm::GeometryProperty("pt", 0, te::gm::PointType, true);
  prop1->setSRID(m_srid);
  dt->add(prop0);
  dt->add(prop0x);
  dt->add(prop0y);
  dt->add(prop1);
  te::mem::DataSet* ds = new te::mem::DataSet(dt.get());

  int id = 0;
  for (size_t pp = 0; pp < pts_result.size(); pp++)
  {
    te::mem::DataSetItem* dataSetItem = new te::mem::DataSetItem(ds);
    dataSetItem->setInt32("FID", id++);
    dataSetItem->setDouble("x", pts_result[pp]->getX());
    dataSetItem->setDouble("y", pts_result[pp]->getY());
    dataSetItem->setGeometry("pt", (te::gm::Geometry*)pts_result[pp]->clone());
    ds->add(dataSetItem);
  }

  Save(trgDs.get(), ds, dt.get());

  bool result = true;

  //if (!intersectionOp->paramsAreValid())
  //  result = false;
  //else
  //  result = intersectionOp->run();

  //delete intersectionOp;

  return true;

  return false;
}

