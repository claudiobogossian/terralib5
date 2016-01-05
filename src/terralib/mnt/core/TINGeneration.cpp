/*!
\file terralib/mnt/core/TINGeneration.cpp

\brief This file contains a class to generate TIN.

*/

#include "TINGeneration.h"
#include "Utils.h"

//.. /..
#include "../../common/progress/TaskProgress.h"
#include "../../dataaccess/utils/Utils.h"

#include "../../geometry/Envelope.h"
#include "../../geometry/Geometry.h"
#include "../../geometry/LineString.h"
#include "../../geometry/Point.h"
#include "../../geometry/TIN.h"
#include "../../geometry/Triangle.h"

#include "../../sam.h"

#include <limits>


te::mnt::TINGeneration::TINGeneration()
{
  m_srid = 0;
  m_tolerance = 0.;
  m_maxdist = 0.;
  m_minedgesize = 0.;
  m_inDsetName_sample = "";
  m_inDsetName_point = "";
  m_atrZ_sample = "";
  m_atrZ_point = "";
}

te::mnt::TINGeneration::~TINGeneration()
{
}

void te::mnt::TINGeneration::setInput(te::da::DataSourcePtr inDsrc,
  std::string inDsetName,
  std::auto_ptr<te::da::DataSetType> inDsetType, InputType type)
{
  if (type == Isolines)
  {
    m_inDsrc_sample = inDsrc;
    m_inDsetName_sample = inDsetName;
    m_inDsetType_sample = inDsetType;
  }
  else
  {
    m_inDsrc_point = inDsrc;
    m_inDsetName_point = inDsetName;
    m_inDsetType_point = inDsetType;
  }
}

void te::mnt::TINGeneration::setBreakLine(te::da::DataSourcePtr inDsrc, std::string inDsetName, std::auto_ptr<te::da::DataSetType> inDsetType, double tol)
{
    m_inDsrc_break = inDsrc;
    m_inDsetName_break = inDsetName;
    m_inDsetType_break = inDsetType;
    m_tolerance_break = tol;
}

void te::mnt::TINGeneration::setOutput(te::da::DataSourcePtr outDsrc, std::string dsname)
{
  m_outDsrc = outDsrc;
  m_outDsetName = dsname;
}

void  te::mnt::TINGeneration::setParams(const double& tolerance,
  const double &maxdist,
  const double &minedgesize,
  const std::string &atrz_iso,
  const std::string &atrz_pt)
{
  setTolerance(tolerance);
  setMaxdist(maxdist);
  setMinedgesize(minedgesize);
  m_atrZ_sample = atrz_iso;
  m_atrZ_point = atrz_pt;
}

bool te::mnt::TINGeneration::run()
{
  te::gm::MultiPoint mpt(0, te::gm::MultiPointZType, m_srid);
  te::gm::MultiPoint mptiso(0, te::gm::MultiPointZType, m_srid);
  te::gm::MultiLineString isolines_simp(0, te::gm::MultiLineStringZType, m_srid);
  std::string geostype;

  // Get samples
  size_t nsamples;
  te::gm::Envelope env;

  nsamples = ReadPoints(m_inDsetName_point, m_inDsrc_point, m_atrZ_point, m_tolerance, mpt, geostype, env);
  setEnvelope(env);

  nsamples += ReadSamples(m_inDsetName_sample, m_inDsrc_sample, m_atrZ_sample, m_tolerance, m_maxdist, AccumulatedDistance, mptiso, isolines_simp, geostype, env);
  setEnvelope(env);

  // Initialize triangulation process
  CreateInitialTriangles(nsamples);
  // Insert points in initial triangulation
  InsertNodes(mpt, isolines_simp);
  // Modify initial triangulation generating Delaunay
  if (!CreateDelaunay())
    return false;
  // Modify Delaunay triangulation to a achieve convex hull 
  if (!ModifyBoundTriangles())
    return false;

  //Test isolines
  if (m_method == 0) //Delanay
  {
    if (isolines_simp.getNumGeometries())
      IsolinesConstrained();
  }
  else //Smaller Angle
    CreateMinAngleTriangulation();

  if (m_inDsrc_break.get())
  {
    InsertBreakLines();
  }

  //if (!borderUp())
  //  return false;

  //TestFlatTriangles();

  // Save triangulation to datasource 
  SaveTin();

  return true;
}

bool te::mnt::TINGeneration::SaveTin()
{
  return te::mnt::Tin::SaveTin(m_outDsrc, m_outDsetName);
}

bool te::mnt::TINGeneration::CreateInitialTriangles(size_t nsamples)
{
  m_nodesize = nsamples + 6;
  m_triangsize = 2 * (nsamples + 6) - 5;  // ntri = 2n-5
  m_linesize = (3 * (nsamples + 5));    // nlin = (n-1)*3
  m_ltriang = 0;
  m_lnode = -1;
  m_lline = 0;

  size_t i;
  for (i = 0; i < m_nodesize; i++)
  {
    TinNode tn;
    m_node.push_back(tn);
  }

  for (i = 0; i < m_triangsize; i++)
  {
    TinTriang tt;
    m_triang.push_back(tt);
  }

  for (i = 0; i < m_linesize; i++)
  {
    TinLine tl;
    m_line.push_back(tl);
  }

  int32_t nodelist[4], linlist[5];

  //  Create the two initial triangles of the Tin
  size_t t1 = (size_t)m_ltriang++;
  size_t t2 = (size_t)m_ltriang++;
  if (t1 > m_triangsize)
    return false;
  if (t2 > m_triangsize)
    return false;

  //  Create and insert initial nodes of the Tin
  std::vector<te::gm::PointZ> pointaux;
  pointaux.push_back(te::gm::PointZ(m_env.getLowerLeftX(), m_env.getLowerLeftY(), m_nodatavalue));
  pointaux.push_back(te::gm::PointZ(m_env.getLowerLeftX(), m_env.getUpperRightY(), m_nodatavalue));
  pointaux.push_back(te::gm::PointZ(m_env.getUpperRightX(), m_env.getUpperRightY(), m_nodatavalue));
  pointaux.push_back(te::gm::PointZ(m_env.getUpperRightX(), m_env.getLowerLeftY(), m_nodatavalue));

  int32_t node;
  for (i = 0; i < 4; i++)
  {
    node = ++m_lnode;
    nodelist[i] = node;
    if (nodelist[i] >(int32_t)m_nodesize)
      return false;
    m_node[(unsigned int)nodelist[i]].Init(pointaux[i]);
    m_node[(unsigned int)nodelist[i]].setType(Box);
  }

  //  Create and insert initial lines of the Tin
  int32_t lin;
  for (i = 0; i < 5; i++)
  {
    lin = m_lline++;
    linlist[i] = lin;
    if (linlist[i] >(int32_t)m_linesize){
      return false;
    }

    if (i < 4)
    {
      m_line[(unsigned int)linlist[i]].Nodes(nodelist[i], nodelist[((i + 1) % 4)]);
      m_node[nodelist[i]].setEdge(linlist[i]);
      m_node[nodelist[((i + 1) % 4)]].setEdge(linlist[i]);
    }

    if (i < 2)
    {
      m_line[(unsigned int)linlist[i]].Polygons(-1, (int32_t)t1);
    }
    else if (i < 4)
    {
      m_line[(unsigned int)linlist[i]].Polygons(-1, (int32_t)t2);
    }
    else
    {
      m_line[(unsigned int)linlist[i]].Nodes(nodelist[2], nodelist[0]);
      m_node[nodelist[2]].setEdge(linlist[i]);
      m_node[nodelist[0]].setEdge(linlist[i]);
      m_line[(unsigned int)linlist[i]].Polygons((int32_t)t2, (int32_t)t1);
    }
  }

  //  Update triangles edges
  m_triang[t1].setEdges(linlist[0], linlist[1], linlist[4]);

  m_triang[t2].setEdges(linlist[4], linlist[2], linlist[3]);

  m_node[(unsigned int)nodelist[0]].setEdge(linlist[0]);
  m_node[(unsigned int)nodelist[1]].setEdge(linlist[1]);
  m_node[(unsigned int)nodelist[2]].setEdge(linlist[2]);
  m_node[(unsigned int)nodelist[3]].setEdge(linlist[3]);

  return true;
}


bool te::mnt::TINGeneration::InsertNodes(const te::gm::MultiPoint &mpt, const te::gm::MultiLineString &mls)
{
  te::common::TaskProgress task("Inserting Nodes...", te::common::TaskProgress::UNDEFINED, (int)m_nodesize-6);

  int32_t node = 0;
  //  Create nodes and insert on triangulation 
  for (size_t id = 0; id < mpt.getNumGeometries(); ++id)
  {
    te::gm::PointZ* pto3d = dynamic_cast<te::gm::PointZ*>(mpt.getGeometryN(id));
    node = ++m_lnode;
    if (node >  (int32_t)m_nodesize)
      return false;
    m_node[(unsigned int)node].Init(*pto3d, Sample);
    InsertNode(node, 1);
    task.pulse();
  }

  bool nflag;
  double PRECISAO = 0.000001;

  typedef te::sam::kdtree::Node<te::gm::Coord2D, int32_t, int32_t> KD_NODE;
  typedef te::sam::kdtree::Index<KD_NODE> KD_TREE;
  te::gm::Envelope e;
  KD_TREE nodetree(e);
  std::vector<KD_NODE*> reportsnode;

  size_t mlsize = mls.getNumGeometries();
  for (unsigned int id = 0; id < mlsize; ++id)
  {
    te::gm::LineString* gout = dynamic_cast<te::gm::LineString*>(mls.getGeometryN(id));
    nflag = true;

    size_t gnp = gout->getNPoints();
    for (size_t j = 0; j < gnp; ++j)
    {
      te::gm::Point *p = gout->getPointN(j);
      te::gm::PointZ pz(p->getX(), p->getY(), p->getZ());
      // Test if new point is equal to inserted
      te::gm::Envelope ept(*p->getMBR());
      nodetree.search(ept, reportsnode);
      if (reportsnode.size())
      {
        reportsnode.clear();
        continue;
      }

      // for each point, create and insert node
      node = ++m_lnode;
      if (node > (int32_t)m_nodesize)
        return false;

      m_node[(unsigned int)node].Init(pz);

      te::gm::Coord2D coord(p->getX(), p->getY());
      nodetree.insert(coord, node);

      // Set node type
      if (nflag)
      {
        m_node[(unsigned int)node].setType(First);
        InsertNode(node, 1);
        if (m_node[(unsigned int)node].getType() == Deletednode)
          nflag = true;
        else
          nflag = false;
      }
      else
      {
        m_node[(unsigned int)node].setType(Normalnode);
        InsertNode(node, 1);
      } //nflag
      task.pulse();
    } //for (std::size_t j = 0; j < gout->getNPoints(); ++j)
  
    if (m_node[(unsigned int)node].getType() == Normalnode)
      m_node[(unsigned int)node].setType(Last);
    else if (m_node[(unsigned int)node].getType() == First)
      m_node[(unsigned int)node].setType(Sample);
    else if ((m_node[(unsigned int)node].getType() != Sample) && (m_node[(unsigned int)node].getType() != Last))
        return false;
  } //for (unsigned int id = 0; id < mls.getNumGeometries(); ++id)

  m_fbnode = 0;
  nodetree.clear();

  return true;
}

bool te::mnt::TINGeneration::InsertNode(int32_t nodeId, int type)
{
  double tol = m_minedgesize;

  // Get Point to be inserted from vector
  te::gm::PointZ pt(m_node[(unsigned int)nodeId].getNPoint());

  if (!m_env.contains(*pt.getMBR()))
    return false;

  double zvalue = pt.getZ();

  // Find the triangle that owns the point pt
  double px = pt.getX();
  double py = pt.getY();

  int32_t triangid = FindTriangle(pt);

  if (triangid == -1)
    return false;

  te::gm::PointZ vert[3];
  TrianglePoints(triangid, vert);
  int32_t nids[3];

  // Verify if point is too close to one triangle vertex
  for (unsigned short j = 0; j < 3; j++)
  {
    if ((fabs(px - vert[j].getX()) < tol) && (fabs(py - vert[j].getY()) < tol))
    {
      NodesId(triangid, nids);
      NodeExchange(nids[j], nodeId);
      DeleteNode(nids[j]);
      return false;
    }
  }

  int32_t linesid[3];
  int32_t neighids[3];
  m_triang[(unsigned int)triangid].LinesId(linesid);
  if (!NeighborsId(triangid, neighids))
    return false;

  //  Test if the point is on an edge
  short nedge = -1;
  double dmin = std::numeric_limits< float >::max();
  te::gm::PointZ paux, pmin;
  for (unsigned int j = 0; j < 3; j++)
  {
    if ((pt.getX() > vert[j].getX()) &&
      (pt.getX() > vert[(j + 1) % 3].getX()))
      continue;
    if ((pt.getX() < vert[j].getX()) &&
      (pt.getX() < vert[(j + 1) % 3].getX()))
      continue;
    if ((pt.getY() > vert[j].getY()) &&
      (pt.getY() > vert[(j + 1) % 3].getY()))
      continue;
    if ((pt.getY() < vert[j].getY()) &&
      (pt.getY() < vert[(j + 1) % 3].getY()))
      continue;

    double daux = pointToSegmentDistance(vert[j], vert[(j + 1) % 3], pt, &paux);
    if ((daux < tol) && (daux < dmin))
    {
      //      On edge j
      nedge = (short)j;
      dmin = daux;
      paux.setZ(zvalue);
      pmin = paux;
    }
  }

  int32_t ntri1, ntri2;
  if (nedge == -1)
  {
    int32_t testLines[6];
    TwoNewTriangles(triangid, nodeId, testLines);  //If not, split triang
    for (int j = 0; j < 6; j++)
    {
      if (testLines[j] == -1)
        break;
      if (TestDelaunay(testLines[j]))
        j = -1;
    }
    ntri1 = m_ltriang - 1;
    ntri2 = m_ltriang - 2;
    if (type == 0)
      TestDelaunay(triangid, ntri1, ntri2);
    else
    {
      if (!NeighborsId(triangid, neighids))
        return false;
      TestDelaunay(triangid, 0);
      TestDelaunay(triangid, 1);
      TestDelaunay(triangid, 2);

      TestDelaunay(neighids[0], 0);
      TestDelaunay(neighids[0], 1);
      TestDelaunay(neighids[0], 2);
      TestDelaunay(neighids[1], 0);
      TestDelaunay(neighids[1], 1);
      TestDelaunay(neighids[1], 2);
      TestDelaunay(neighids[2], 0);
      TestDelaunay(neighids[2], 1);
      TestDelaunay(neighids[2], 2);
      TestDelaunay(ntri1, 0);
      TestDelaunay(ntri1, 1);
      TestDelaunay(ntri1, 2);
      TestDelaunay(ntri2, 0);
      TestDelaunay(ntri2, 1);
      TestDelaunay(ntri2, 2);
    }
  }
  else
  {
    m_node[(unsigned int)nodeId].setNPoint(pmin);

    for (unsigned int j = 0; j < 3; j++)
    {
      if (neighids[j] == -1)
        continue;
      int32_t oppNode = OppositeNode(neighids[j], linesid[j]);
      if (oppNode == -1)
        continue;
      if ((fabs(px - m_node[(unsigned int)oppNode].getX()) < tol) && (fabs(py - m_node[(unsigned int)oppNode].getY()) < tol))
      {
        NodesId(triangid, nids);
        NodeExchange(nids[j], nodeId);
        DeleteNode(nids[j]);
        return false;
      }
    }

    //    Duplicate triangle and its neighbor on the same edge
    int32_t testLines[5];
    int32_t linid = DuplicateTriangle(triangid, nedge, nodeId, testLines);
    if (neighids[(unsigned int)nedge] != -1)
    {
      int32_t ngtriang = neighids[(unsigned int)nedge];
      if (ngtriang > (int32_t)m_triangsize)
        return false;
      int32_t testLines2[2];
      DupNeighTriangle(ngtriang, linid, nedge, nodeId, testLines2);
      for (int j = 0; j < 2; j++)
      {
        if (testLines2[(unsigned int)j] == -1)
          break;
        if (TestDelaunay(testLines2[(unsigned int)j]))
          j = -1;
      }

      //      Test Delaunay restriction with neighbour 1
      ntri1 = m_ltriang - 1;
      ntri2 = m_ltriang - 2;
      if (type == 0)
        TestDelaunay(triangid, ngtriang, ntri1, ntri2);
      else
      {
        if (!NeighborsId(triangid, neighids))
          return false;
        TestDelaunay(triangid, 0);
        TestDelaunay(triangid, 1);
        TestDelaunay(triangid, 2);
        TestDelaunay(ngtriang, 0);
        TestDelaunay(ngtriang, 1);
        TestDelaunay(ngtriang, 2);

        TestDelaunay(neighids[0], 0);
        TestDelaunay(neighids[0], 1);
        TestDelaunay(neighids[0], 2);
        TestDelaunay(neighids[1], 0);
        TestDelaunay(neighids[1], 1);
        TestDelaunay(neighids[1], 2);
        TestDelaunay(neighids[2], 0);
        TestDelaunay(neighids[2], 1);
        TestDelaunay(neighids[2], 2);
        TestDelaunay(ntri1, 0);
        TestDelaunay(ntri1, 1);
        TestDelaunay(ntri1, 2);
        TestDelaunay(ntri2, 0);
        TestDelaunay(ntri2, 1);
        TestDelaunay(ntri2, 2);
      }
    }
    else
    {
      for (int j = 0; j < 5; j++)
      {
        if (testLines[(unsigned int)j] == -1)
          break;
        if (TestDelaunay(testLines[(unsigned int)j]))
          j = -1;
      }
      if (!NeighborsId(triangid, neighids))
        return false;
      ntri1 = m_ltriang - 1;
      TestDelaunay(triangid, 0);
      TestDelaunay(triangid, 1);
      TestDelaunay(triangid, 2);
      TestDelaunay(neighids[0], 0);
      TestDelaunay(neighids[0], 1);
      TestDelaunay(neighids[0], 2);
      TestDelaunay(neighids[1], 0);
      TestDelaunay(neighids[1], 1);
      TestDelaunay(neighids[1], 2);
      TestDelaunay(neighids[2], 0);
      TestDelaunay(neighids[2], 1);
      TestDelaunay(neighids[2], 2);
      TestDelaunay(ntri1, 0);
      TestDelaunay(ntri1, 1);
      TestDelaunay(ntri1, 2);
    }
  }

  return true;
}

bool te::mnt::TINGeneration::DeleteNode(int32_t node)
{
  int32_t nidaux;

  switch (m_node[(unsigned int)node].getType())
  {
  case Normalnode:  // Isoline normal point
  case Sample:  // Sample point
  case Box:
  case Deletednode:
    break;
  case First:  // Isoline first point
  {
    nidaux = NextNode(node);

    if (m_node[(unsigned int)nidaux].getType() == Normalnode)
      m_node[(unsigned int)nidaux].setType(First);
    else if (m_node[(unsigned int)nidaux].getType() == Last)
      m_node[(unsigned int)nidaux].setType(Sample);
    else
      return false;
    break;
  }
  case Last:  // Isoline last point
  {
    nidaux = PreviousNode(node);

    if (m_node[(unsigned int)nidaux].getType() == Normalnode)
      m_node[(unsigned int)nidaux].setType(Last);
    else if (m_node[(unsigned int)nidaux].getType() == First)
      m_node[(unsigned int)nidaux].setType(Sample);
    else
      return false;
    break;
  }
  case Breaklinenormal:  // Breakline normal point
  {
    nidaux = NextNode(node);

    if (nidaux == m_lnode)
      break;

    if (m_node[(unsigned int)nidaux].getType() == Breaklinenormal)
      m_node[(unsigned int)nidaux].setType(Breaklinefirst);
    else if (m_node[(unsigned int)nidaux].getType() == Breaklinelast)
      m_node[(unsigned int)nidaux].setType(Sample);
    else
      return false;
    nidaux = PreviousNode(node);

    if (m_node[(unsigned int)nidaux].getType() == Breaklinenormal)
      m_node[(unsigned int)nidaux].setType(Breaklinelast);
    else if (m_node[(unsigned int)nidaux].getType() == Breaklinefirst)
      m_node[(unsigned int)nidaux].setType(Sample);
    break;
  }
  case Breaklinefirst:  // Breakline first point
  {
    nidaux = NextNode(node);

    if (nidaux == m_lnode)
    {
      m_node[(unsigned int)nidaux].setType(Breaklinefirst);
      break;
    }
    if (m_node[(unsigned int)nidaux].getType() == Breaklinenormal)
      m_node[(unsigned int)nidaux].setType(Breaklinefirst);
    else if (m_node[(unsigned int)nidaux].getType() == Breaklinelast)
      m_node[(unsigned int)nidaux].setType(Sample);
    else
      return false;
    break;
  }
  case Breaklinelast:  // Breakline last point
  {
    nidaux = PreviousNode(node);

    if (m_node[(unsigned int)nidaux].getType() == Breaklinenormal)
      m_node[(unsigned int)nidaux].setType(Breaklinelast);
    else if (m_node[(unsigned int)nidaux].getType() == Breaklinefirst)
      m_node[(unsigned int)nidaux].setType(Sample);
    else
    {
      return false;
    }
    break;
  }
  }
  m_node[(unsigned int)node].setType(Deletednode);

  return true;
}


bool te::mnt::TINGeneration::NodeExchange(int32_t oldNode, int32_t newNode)
{
  std::vector<int32_t> lids;
  int32_t linid = 0;

  if (!NodeLines(oldNode, lids))
    return false;
  for (size_t i = 0; i < lids.size(); i++)
  {
    linid = lids[i];
    m_line[(unsigned int)linid].ExchangeNode(oldNode, newNode);
    m_node[(unsigned int)oldNode].removeEdge(linid);
    m_node[(unsigned int)newNode].setEdge(linid);
  }
  m_node[(unsigned int)newNode].setX(m_node[(unsigned int)oldNode].getX());
  m_node[(unsigned int)newNode].setY(m_node[(unsigned int)oldNode].getY());
  m_node[(unsigned int)newNode].setZ(m_node[(unsigned int)oldNode].getZ());
  return true;
}


bool te::mnt::TINGeneration::TwoNewTriangles(int32_t t, int32_t nodeId, int32_t* testLines)
{
  //1. Creates the vertex vn with data of sample s,
  int32_t vn = nodeId;

  //2. Creates 2 nwes triangles t1 and t2,
  int32_t t1 = m_ltriang++;
  if (t1 > (int32_t)m_triangsize)
    return false;

  int32_t t2 = m_ltriang++;
  if (t2 > (int32_t)m_triangsize)
    return false;

  //3. Creates a new edge an0 that connects vertex vn and v0 and
  //   has t and t1 as the triangles that shared the edge.
  int32_t an0 = m_lline++;
  m_line[(unsigned int)an0].Nodes(vn, NodeId(t, 0)); //v0 is the vertex 0 of t
  m_node[vn].setEdge(an0);
  m_node[NodeId(t, 0)].setEdge(an0);
  m_line[(unsigned int)an0].Polygons(t, t1);

  //4. Create a new edge an1 that connects the vertex vn and v1 and
  //   has t1 and t2 as triangles thats shares the edge.
  int32_t an1 = m_lline++;
  m_line[(unsigned int)an1].Nodes(vn, NodeId(t, 1)); //v1 is the vertex 1 of t
  m_node[vn].setEdge(an1);
  m_node[NodeId(t, 1)].setEdge(an1);
  m_line[(unsigned int)an1].Polygons(t1, t2);

  //5. Creates a new wdge an2 that connects the vertex vn and v2 and
  //  has t2 and t as triangles that shares the edge.
  int32_t an2 = m_lline++;
  m_line[(unsigned int)an2].Nodes(vn, NodeId(t, 2)); //v2 is the vertex 2 of t
  m_node[vn].setEdge(an2);
  m_node[NodeId(t, 2)].setEdge(an2);
  m_line[(unsigned int)an2].Polygons(t2, t);

  //6. Swap triangle t for t1 on edge a0,
  int32_t a0 = ExchangePolygon(t, t1, 0); // returns edge 0 of t

  //7. Defines the edges of t1 as an0, a0 and an1,
  m_triang[(unsigned int)t1].setEdges(an0, a0, an1);

  //8. Swap the triangle t for t2 in edge a1,
  int32_t a1 = ExchangePolygon(t, t2, 1); // returns edge 1 of t

  //9. Defines edges of t2 as an1, a1 and an2,
  m_triang[(unsigned int)t2].setEdges(an1, a1, an2);

  //10. Defines edges of t as an2, a2 and an0,
  int32_t a2 = m_triang[(unsigned int)t].LineAtEdge(2);
  m_triang[(unsigned int)t].setEdges(an2, a2, an0);

  //Define lines to be tested
  testLines[0] = a0;
  testLines[1] = a1;
  testLines[2] = a2;
  testLines[3] = an0;
  testLines[4] = an1;
  testLines[5] = an2;

  return true;
}


int32_t te::mnt::TINGeneration::DuplicateTriangle(int32_t t, short n, int32_t v, int32_t* testLines)
{
  // Is to triangle containing whose nth edge is an edge
  // next the sample s(n Î {0,1,2}). Then edge connects vn and vj vertices, 
  // aj edge connects the vertices vj and vk and ak edge connects 
  // vk and vn vertices, and j is the rest of division of n+ 1 for 3 
  // and k the rest of division n+2 by 3 .

  int32_t lineids[3];
  m_triang[(unsigned int)t].LinesId(lineids);

  int32_t an = lineids[n];
  int32_t aj = lineids[(n + 1) % 3];
  int32_t ak = lineids[(n + 2) % 3];
  int32_t vn = NodeId(t, n);

  testLines[0] = aj;
  testLines[1] = ak;

  //  1. Create the vertice v with data of sample s,
  //  2. Definen the triangle tv that shares edge an with t,
  int32_t tv;
  if (m_line[(unsigned int)an].getLeftPolygon() == t)
    tv = m_line[(unsigned int)an].getRightPolygon();
  else if (m_line[(unsigned int)an].getRightPolygon() == t)
    tv = m_line[(unsigned int)an].getLeftPolygon();
  else{
    return -1;
  }

  //  3. Defines vertice vop of triangleo t that is not connected to an,
  int32_t vop = NodeId(t, (n + 2) % 3);

  //  4. Create new triangle t1,
  int32_t t1 = m_ltriang++;
  if (t1 > (int32_t)m_triangsize)
    return -1;

  //  5. Create new edge an0 that connects the vertice v and vn and
  //     has t1 and tv as triangles that shares the edge.
  int32_t an0 = m_lline++;
  if (an0 > (int32_t)m_linesize)
    return -1;

  m_line[(unsigned int)an0].Nodes(v, vn);
  m_node[v].setEdge(an0);
  m_node[vn].setEdge(an0);
  m_line[(unsigned int)an0].Polygons(t1, tv);

  testLines[2] = an;
  testLines[3] = an0;

  //  6. Create a new edge an1 that connects the vertices v and vop and
  //     has t and t1 as triangles that shares the edge.
  int32_t an1 = m_lline++;
  if (an1 > (int32_t)m_linesize)
    return -1;

  m_line[(unsigned int)an1].Nodes(v, vop);
  m_node[v].setEdge(an1);
  m_node[vop].setEdge(an1);
  m_line[(unsigned int)an1].Polygons(t, t1);

  testLines[4] = an1;

  //  7. Modifies the edge an to connect vertice v instead of vn,
  m_node[vn].removeEdge(an);
  m_node[v].setEdge(an);
  m_line[(unsigned int)an].ExchangeNode(vn, v);

  //  8. Defines the edges of t as an, aj and an1,
  m_triang[(unsigned int)t].setEdges(an, aj, an1);

  //  9. Modifies edge ak  to shares triangle t1 instead of t,
  m_line[(unsigned int)ak].ExchangePolygon(t, t1);

  //  10. Defines edges of t1 as an0, an1 and ak,
  m_triang[(unsigned int)t1].setEdges(an0, an1, ak);

  return an0;
}

bool te::mnt::TINGeneration::DupNeighTriangle(int32_t tv, int32_t an0, short, int32_t v, int32_t *testLines)
{
  //  11.1. Create new triangle t2,
  int32_t t2 = m_ltriang++;
  if (t2 > (int32_t)m_triangsize)
    return false;
  //  11.2. Defines edge av of triangle tv that contains vertex vn
  //        (obs: there is only an edge because the other was modified),
  int32_t vn = m_line[(unsigned int)an0].getNodeTo(); //Due to assembly, dangerous
  int32_t lineids[3];
  m_triang[(unsigned int)tv].LinesId(lineids);
  unsigned short i;
  for (i = 0; i < 3; i++)
  {
    if (lineids[i] >(int32_t)m_linesize)
      return false;
    if ((m_line[(unsigned int)lineids[i]].getNodeFrom() == vn) ||
      (m_line[(unsigned int)lineids[i]].getNodeTo() == vn))
      break;
  }
  if (i == 3){
    return false;
  }
  int32_t av = lineids[i];

  testLines[0] = av;

  //  11.3. Defines the other edge tv as av1 and av2.
  int32_t av1 = lineids[(i + 1) % 3];
  int32_t av2 = lineids[(i + 2) % 3];

  testLines[1] = av1;

  //  11.4. Defines the vertex vvo connect to vn by edge av,
  int32_t vvo;
  if (m_line[(unsigned int)av].getNodeFrom() == vn)
    vvo = m_line[(unsigned int)av].getNodeTo();
  else
    vvo = m_line[(unsigned int)av].getNodeFrom();

  //  11.5. Create new wdge an2 that connects vertex v  and vvo and
  //        there are t and t2 as triangles that shares the edge.
  int32_t an2 = m_lline++;
  if (an2 > (int32_t)m_linesize)
    return false;
  m_line[(unsigned int)an2].Nodes(v, vvo);
  m_line[(unsigned int)an2].Polygons(tv, t2);

  //  11.6. Swap the triangle tv by t2 in edge av,
  m_line[(unsigned int)av].ExchangePolygon(tv, t2);

  //  11.7. Swap the triangle tv by t2 in edge an0,
  m_line[(unsigned int)an0].ExchangePolygon(tv, t2);

  //  11.8. Defines edges of tv as an2, av1 and av2.
  m_triang[(unsigned int)tv].setEdges(an2, av1, av2);

  //  11.9. Defines edges of t2 as an0, av and an2,
  m_triang[(unsigned int)t2].setEdges(an0, av, an2);

  return true;
}

bool te::mnt::TINGeneration::TestDelaunay(int32_t tri1Id, int32_t tri2Id, int32_t tri3Id)
{
  if (tri1Id == -1)
    return false;

  int32_t  nids[3];
  unsigned short  j;

  if (!NeighborsId(tri1Id, nids))
    return false;
  for (j = 0; j < 3; j++)
    if ((nids[j] != tri2Id) && (nids[j] != tri3Id))
      break;
  if (j == 3)
    return false;
  if (IsNeighborOnIsoOrBreakline(tri1Id, j) == false)
    TestDelaunay(tri1Id, (short)j);

  if (!NeighborsId(tri2Id, nids))
    return false;
  for (j = 0; j < 3; j++)
    if ((nids[j] != tri1Id) && (nids[j] != tri3Id))
      break;
  if (j == 3)
    return false;
  if (IsNeighborOnIsoOrBreakline(tri2Id, j) == false)
    TestDelaunay(tri2Id, (short)j);

  if (!NeighborsId(tri3Id, nids))
    return false;
  for (j = 0; j < 3; j++)
    if ((nids[j] != tri1Id) && (nids[j] != tri2Id))
      break;
  if (j == 3)
    return false;
  if (IsNeighborOnIsoOrBreakline(tri3Id, j) == false)
    TestDelaunay(tri3Id, (short)j);

  return true;
}


bool te::mnt::TINGeneration::TestDelaunay(int32_t tri1Id, int32_t tri2Id, int32_t tri3Id, int32_t tri4Id)
{
  if (tri1Id == -1)
    return false;

  int32_t  nids[3];
  unsigned short  j;

  if (!NeighborsId(tri1Id, nids))
    return false;
  for (j = 0; j < 3; j++)
    if ((nids[j] != tri2Id) && (nids[j] != tri3Id) && (nids[j] != tri4Id))
      break;
  if (j == 3)
    return false;
  if (IsNeighborOnIsoOrBreakline(tri1Id, j) == false)
    TestDelaunay(tri1Id, (short)j);

  if (!NeighborsId(tri2Id, nids))
    return false;
  for (j = 0; j < 3; j++)
    if ((nids[j] != tri1Id) && (nids[j] != tri3Id) && (nids[j] != tri4Id))
      break;
  if (j == 3)
    return false;
  if (IsNeighborOnIsoOrBreakline(tri2Id, j) == false)
    TestDelaunay(tri2Id, (short)j);

  if (!NeighborsId(tri3Id, nids))
    return false;
  for (j = 0; j < 3; j++)
    if ((nids[j] != tri1Id) && (nids[j] != tri2Id) && (nids[j] != tri4Id))
      break;
  if (j == 3)
    return false;
  if (IsNeighborOnIsoOrBreakline(tri3Id, j) == false)
    TestDelaunay(tri3Id, (short)j);

  if (!NeighborsId(tri4Id, nids))
    return false;
  for (j = 0; j < 3; j++)
    if ((nids[j] != tri1Id) && (nids[j] != tri2Id) && (nids[j] != tri3Id))
      break;
  if (j == 3)
    return false;
  if (IsNeighborOnIsoOrBreakline(tri4Id, j) == false)
    TestDelaunay(tri4Id, (short)j);

  return true;
}


bool te::mnt::TINGeneration::TestDelaunay(int32_t linId)
{
  if (linId == -1)
    return false;
  int32_t tv = m_line[(unsigned int)linId].getLeftPolygon();
  if (tv == -1)
    return false;

  int32_t  linids[3];
  if (!m_triang[(unsigned int)tv].LinesId(linids))
    return false;

  unsigned short j;
  for (j = 0; j < 3; j++)
    if (linids[j] == linId)
      break;
  if (j != 3)
    return TestDelaunay(tv, (short)j);
  else
    return false;
}


bool te::mnt::TINGeneration::TestDelaunay(int32_t triId, short nviz)
{
  if (triId == -1)
    return false;
  double  xc, yc, xaux, yaux, raio2, dist2,
    xo, yo;
  int32_t  nodid, neighids[3];
  te::gm::PointZ  vert[3];
  unsigned short  i;
  double  minx = std::numeric_limits< float >::max(),
    miny = std::numeric_limits< float >::max();

  //  Retrieve neighbour triangle (tviz) pointer
  if (!NeighborsId(triId, neighids))
    return false;
  if (neighids[nviz] == -1)
    return false;

  //  Retrieve line of triangle common to neighbor triangle
  int32_t linid = m_triang[(unsigned int)triId].LineAtEdge((unsigned short)nviz);
  if (linid == -1 || linid > (int32_t)m_linesize)
    return false;

  //  Define base triangle (tri) center point
  TrianglePoints(triId, vert);

  //  Find opposite point to base triangle (tri) inside neighbour (tviz)
  nodid = OppositeNode(neighids[nviz], linid);
  if (nodid == -1)
    return false;
  if (nodid > (int32_t) m_nodesize)
    return false;
  for (i = 0; i < 3; i++)
  {
    if (vert[i].getX() < minx)
      minx = vert[i].getX();
    if (vert[i].getY() < miny)
      miny = vert[i].getY();
  }
  if ((minx < std::numeric_limits< float >::max()) && (miny < std::numeric_limits< float >::max()))
  {
    for (i = 0; i < 3; i++)
    {
      vert[i].setX(vert[i].getX() - minx);
      vert[i].setY(vert[i].getY() - miny);
    }
    xo = m_node[(unsigned int)nodid].getNPoint().getX() - minx;
    yo = m_node[(unsigned int)nodid].getNPoint().getY() - miny;
  }
  else
  {
    xo = m_node[(unsigned int)nodid].getNPoint().getX();
    yo = m_node[(unsigned int)nodid].getNPoint().getY();
  }

  if (findCenter(vert, &xc, &yc) == false)
  {
    return false;
  }

  //  Calculate base triangle (tri) radius
  xaux = xc - vert[0].getX();
  yaux = yc - vert[0].getY();
  raio2 = xaux * xaux + yaux * yaux;

  //  Calculate distance from opposite point (tviz) to center point (tri)
  xaux = xc - xo;
  yaux = yc - yo;
  dist2 = xaux * xaux + yaux * yaux;
  float fr = (float)sqrt(raio2);
  float fd = (float)sqrt(dist2);

  if (fr <= fd){
    return false;
  }

  //  If not, change edge between tri and ntri
  return UpdateTriangles(triId, neighids[nviz], linid);
}


bool te::mnt::TINGeneration::UpdateTriangles(int32_t t, int32_t tv, int32_t ai)
{
  //Be the triangle t whose ith edge there is the edge
  //that you want to change (i Î { 0,1,2 } ). The edge ai connects the
  //vertices vi and vj , the aj edge connects the vertices vj and vk and 
  //ak edge connects vk vertices and vi , and j is the rest of the division
  //of i+1 for 3 and k the rest of the i+2 division by 3.

  if (tv == -1)
    return false;

  int32_t tEdges[3];
  if (!m_triang[(unsigned int)t].LinesId(tEdges))
    return false;

  int32_t nodeIds[3];
  if (!NodesId(t, nodeIds))
    return false;

  unsigned short i;
  for (i = 0; i < 3; i++)
    if (tEdges[i] == ai)
      break;

  if (i == 3) return false;

  int32_t aj = tEdges[(i + 1) % 3];
  int32_t ak = tEdges[(i + 2) % 3];

  int32_t vi = nodeIds[i];
  int32_t vj = nodeIds[(i + 1) % 3];
  int32_t vk = nodeIds[(i + 2) % 3];

  // Be the triangle tv that sharing the edge ai with t. The vertice of
  //  tv that is not connected by edge ai is the vertice vn. Others
  //  edges of triangle tv are am that connects edge vi and vn and the
  //  edge an connects vertices vn and vj.

  int32_t vn = OppositeNode(tv, ai);
  if (tv == -1)
    return false;

  int32_t tvEdges[3];
  if (!m_triang[(unsigned int)tv].LinesId(tvEdges))
    return false;

  int32_t am = 0, an = 0;
  unsigned short j;
  for (j = 0; j < 3; j++)
  {
    if (tvEdges[j] == ai)
      continue;

    if (m_line[(unsigned int)tvEdges[j]].getNodeFrom() == vn)
    {
      if (m_line[(unsigned int)tvEdges[j]].getNodeTo() == vi)
        am = tvEdges[j];
      else if (m_line[(unsigned int)tvEdges[j]].getNodeTo() == vj)
        an = tvEdges[j];
      else{
        return false;
      }
    }
    else if (m_line[(unsigned int)tvEdges[j]].getNodeTo() == vn)
    {
      if (m_line[(unsigned int)tvEdges[j]].getNodeFrom() == vi)
        am = tvEdges[j];
      else if (m_line[(unsigned int)tvEdges[j]].getNodeFrom() == vj)
        an = tvEdges[j];
      else{
        return false;
      }
    }
  }

  //  1. If the line segment formed by connecting vertice vk of
  //     triangle t to vertice vn of tv intersects the edge ai, do:
  te::gm::PointZ ptvk = m_node[(unsigned int)vk].getNPoint();
  te::gm::PointZ ptvn = m_node[(unsigned int)vn].getNPoint();
  te::gm::PointZ ptvi = m_node[(unsigned int)vi].getNPoint();
  te::gm::PointZ ptvj = m_node[(unsigned int)vj].getNPoint();

  if (!segIntersect(ptvi, ptvj, ptvk, ptvn))
    return false;

  //    1.1. Swap in edge ai the vertice vi by vertice vk and the
  //       vertice vj pelo vertice vn.
  if (m_line[(unsigned int)ai].getNodeTo() == vi)
  {
    m_node[vi].removeEdge(ai);
    m_node[vk].setEdge(ai);
    m_node[vn].setEdge(ai);
    m_line[(unsigned int)ai].setNodeTo(vk);
    m_line[(unsigned int)ai].setNodeFrom(vn); // this is vj
  }
  else if (m_line[(unsigned int)ai].getNodeTo() == vj)
  {
    m_node[vj].removeEdge(ai);
    m_node[vn].setEdge(ai);
    m_node[vk].setEdge(ai);
    m_line[(unsigned int)ai].setNodeTo(vn);
    m_line[(unsigned int)ai].setNodeFrom(vk); // this is vi
  }

  std::vector<int32_t>::iterator it;
  it = std::find(m_node[(unsigned int)vi].m_edge.begin(), m_node[(unsigned int)vi].m_edge.end(), ai);
  if (it != m_node[(unsigned int)vi].m_edge.end())
  {
    m_node[(unsigned int)vi].m_edge.erase(it);
    m_node[(unsigned int)vi].setEdge(ak);
  }

  it = std::find(m_node[(unsigned int)vj].m_edge.begin(), m_node[(unsigned int)vj].m_edge.end(), ai);
  if (it != m_node[(unsigned int)vj].m_edge.end())
  {
    m_node[(unsigned int)vj].m_edge.erase(it);
    m_node[(unsigned int)vj].setEdge(aj);
  }

  //    1.2. Swap in edge an the triangle tv by triangle t.
  if (m_line[(unsigned int)an].getRightPolygon() == tv)
    m_line[(unsigned int)an].setRightPolygon(t);
  else
    m_line[(unsigned int)an].setLeftPolygon(t);

  //    1.3. Swap in edge ak the triangle t by triangle tv.
  if (m_line[(unsigned int)ak].getRightPolygon() == t)
    m_line[(unsigned int)ak].setRightPolygon(tv);
  else
    m_line[(unsigned int)ak].setLeftPolygon(tv);

  //    1.4. Swap in triangle t the edge ai by edge an and the
  //       edge ak by edge ai.
  for (i = 0; i < 3; i++)
    if (tEdges[i] == ai)
      break;
  if (i < 3) tEdges[i] = an;
  tEdges[(i + 2) % 3] = ai;
  m_triang[(unsigned int)t].setEdges((int32_t)tEdges[0], (int32_t)tEdges[1], (int32_t)tEdges[2]);

  //    1.5. Swap in triangle tv the edge aiby edge ak
  for (i = 0; i < 3; i++)
    if (tvEdges[i] == ai)
      break;
  if (i < 3) tvEdges[i] = ak;

  //     and the edge an by edge ai.
  for (i = 0; i < 3; i++)
    if (tvEdges[i] == an)
      break;
  if (i < 3) tvEdges[i] = ai;

  m_triang[(unsigned int)tv].setEdges((int32_t)tvEdges[0], (int32_t)tvEdges[1], (int32_t)tvEdges[2]);

  return true;
}

int32_t te::mnt::TINGeneration::ExchangePolygon(int32_t triangId, int32_t newPolyId, unsigned short edge)
{
  int32_t  linId;

  linId = m_triang[(unsigned int)triangId].LineAtEdge(edge);
  if (linId == -1)
    return false;
  if (linId > (int32_t)m_linesize)
    return false;
  m_line[(unsigned int)linId].ExchangePolygon(triangId, newPolyId);

  return linId;
}


bool te::mnt::TINGeneration::IsIsolineSegment(int32_t linid)
{
  int32_t  nid0, nid1;

  if ((m_node[(unsigned int)m_line[(unsigned int)linid].getNodeTo()].getType() == Sample) ||
    (m_node[(unsigned int)m_line[(unsigned int)linid].getNodeFrom()].getType() == Sample))
    //      If sample point
    return false;

  if ((m_node[(unsigned int)m_line[(unsigned int)linid].getNodeTo()].getType() > Last) ||
    (m_node[(unsigned int)m_line[(unsigned int)linid].getNodeFrom()].getType() > Last))
    //      If breakline point
    return false;

  nid0 = m_line[(unsigned int)linid].getNodeFrom();
  if (nid0 > m_line[(unsigned int)linid].getNodeTo())
  {
    nid0 = m_line[(unsigned int)linid].getNodeTo();
    nid1 = m_line[(unsigned int)linid].getNodeFrom();
  }
  else
    nid1 = m_line[(unsigned int)linid].getNodeTo();

  if (m_node[(unsigned int)nid0].getType() == Last)
    //    If first point is last point of line
    return false;

  if (m_node[(unsigned int)nid1].getType() == First)
    //    If last point is first point of line
    return false;

  //  Search next non-deleted point
  if (NextNode(nid0) != nid1)
    return false;

  return true;
}


bool te::mnt::TINGeneration::IsBreaklineSegment(int32_t linid)
{
  int32_t  nid0, nid1;

  if ((m_node[(unsigned int)m_line[(unsigned int)linid].getNodeTo()].getType() == Sample) ||
    (m_node[(unsigned int)m_line[(unsigned int)linid].getNodeFrom()].getType() == Sample))
    //      If sample point
    return false;

  if ((m_node[(unsigned int)m_line[(unsigned int)linid].getNodeTo()].getType() < Breaklinenormal) ||
    (m_node[(unsigned int)m_line[(unsigned int)linid].getNodeFrom()].getType() < Breaklinenormal))
    //      If breakline point
    return false;

  nid0 = m_line[(unsigned int)linid].getNodeFrom();
  if (nid0 > m_line[(unsigned int)linid].getNodeTo())
  {
    nid0 = m_line[(unsigned int)linid].getNodeTo();
    nid1 = m_line[(unsigned int)linid].getNodeFrom();
  }
  else
    nid1 = m_line[(unsigned int)linid].getNodeTo();

  if (m_node[(unsigned int)nid0].getType() == Breaklinelast)
    //    If first point is last point of line
    return false;

  if (m_node[(unsigned int)nid1].getType() == Breaklinefirst)
    //    If last point is first point of line
    return false;

  //  Search next non-deleted point
  if (NextNode(nid0) != nid1)
    return false;

  return true;
}



bool te::mnt::TINGeneration::IsNeighborOnIsoOrBreakline(int32_t triId, unsigned short nviz)
{
  int32_t  linid;

  //  Retrieve line of triangle common to neighbor triangle
  linid = m_triang[(unsigned int)triId].LineAtEdge(nviz);
  if (IsIsolineSegment(linid))
    return true;
  if (IsBreaklineSegment(linid))
    return true;
  return false;
}

bool te::mnt::TINGeneration::CreateDelaunay()
{
  int32_t triangid, contr = 0, npoly = -1;

  te::common::TaskProgress task("Creating Delaunay...", te::common::TaskProgress::UNDEFINED, (int)m_ltriang);

  for (triangid = 0; triangid < m_ltriang; triangid++)
  {
    task.pulse();

    if (triangid > npoly)
      npoly = triangid;
    else
      return false;

    if (!GenerateDelaunay(npoly, npoly, contr))
    {
      return false;
    }
  }

  return true;
}


bool te::mnt::TINGeneration::GenerateDelaunay(int32_t nt, int32_t ntbase, int32_t contr)
{
  int32_t  aux, neighids[3];
  short  j;

  contr++;
  if (contr > m_ltriang)
  {
    return false;
  }

  for (j = 0; j < 3; j++)
  {
    NeighborsId(nt, neighids);
    aux = neighids[(unsigned int)j];
    if (aux == nt)
      continue;

    // Test with each neighbor, if it exists
    if (aux == -1)
      continue;
    if (TestDelaunay(nt, j))
    {
      // If changed,
      if (ntbase > aux)
        // Se numero menor que base
        GenerateDelaunay(aux, ntbase, contr);

      //Return current neighbor
      j = -1;
    }
  }

  return true;
}


bool te::mnt::TINGeneration::ModifyBoundTriangles()
{
  std::vector<int32_t> lids;
  int32_t nid, linid;
  int32_t rtri, ltri;
  int32_t nids[4] = { 0, 0, 0, 0 }, tnids[3] = { 0, 0, 0 };
  int32_t   trids[4] = {0, 0, 0, 0};
  unsigned short j, modified;

  int i = 0;
  for (nid = 1; nid < (int32_t)m_node.size(); nid++)
  {
    if (m_node[(unsigned int)nid].getType() != Box &&
      m_node[(unsigned int)nid].getType() != Deletednode &&
      m_node[(unsigned int)nid].getType() != Sample)
      continue;
    if (m_node[(unsigned int)nid].getZ() < m_nodatavalue)
      //      If not fake sample point
      continue;
    nids[i++] = nid;
    if (i == 4)
      break;
  }

  modified = 0;
  for (i = 0; i < 4; i++)
  {
    if (!NodeLines(nids[(unsigned int)i], lids))
      continue;
    for (size_t ii = 0; ii < lids.size(); ii++)
    {
      linid = lids[ii];
      rtri = m_line[(unsigned int)linid].getRightPolygon();
      ltri = m_line[(unsigned int)linid].getLeftPolygon();
      if ((rtri != -1) && (ltri != -1))
      {
        NeighborsId(rtri, tnids);
        for (j = 0; j < 3; j++)
          if (tnids[j] == -1)
            break;
        if (j != 3)
          continue;
        if (UpdateTriangles(rtri, ltri, linid))
          modified = 1;
      }
      else
      {
        if (((m_line[(unsigned int)linid].getNodeFrom() == nids[(unsigned int)i]) &&
          ((m_line[(unsigned int)linid].getNodeTo() == nids[(unsigned int)(i + 1) % 4]) ||
          (m_line[(unsigned int)linid].getNodeTo() == nids[(unsigned int)(i + 2) % 4]) ||
          (m_line[(unsigned int)linid].getNodeTo() == nids[(unsigned int)(i + 3) % 4]))) ||
          ((m_line[(unsigned int)linid].getNodeTo() == nids[(unsigned int)i]) &&
          ((m_line[(unsigned int)linid].getNodeFrom() == nids[(unsigned int)(i + 1) % 4]) ||
          (m_line[(unsigned int)linid].getNodeFrom() == nids[(unsigned int)(i + 2) % 4]) ||
          (m_line[(unsigned int)linid].getNodeFrom() == nids[(unsigned int)(i + 3) % 4]))))
        {
          if (rtri == -1)
            trids[(unsigned int)i] = ltri;
          else
            trids[(unsigned int)i] = rtri;
        }
      }
    }
    lids.clear();
    if ((i == 3) && (modified))
    {
      i = -1;
      modified = 0;
    }
  }
  for (i = 0; i < 4; i++)
  {
    m_triang[(unsigned int)trids[(unsigned int)i]].LinesId(tnids);
    for (j = 0; j < 3; j++)
    {
      rtri = m_line[(unsigned int)tnids[j]].getRightPolygon();
      ltri = m_line[(unsigned int)tnids[j]].getLeftPolygon();
      if ((rtri != -1) && (ltri != -1))
        UpdateTriangles(rtri, ltri, tnids[j]);
    }
  }
  return true;
}


bool te::mnt::TINGeneration::IsolinesConstrained()
{
  int iter = 0;
  for (;;)
    if (!TestIsolines(++iter))
      break;

  return true;
}


bool te::mnt::TINGeneration::TestIsolines(int iter)
{
  int32_t linid1 = 0, lidaux, oldline,
    ntri,
    tnodid, vnodid;
  int32_t nid0, nid1, node, nidaux, nidaux1, 
    vlins[3];
  unsigned int i;
  std::vector<int32_t> lids;
  te::gm::PointZ pt, ptf, npt0, npt1;
  bool modified = false;
  static size_t count = 0;
  std::vector<te::mnt::TinNode> p3dl;
  te::mnt::TinNode ptaux;
  std::vector<int32_t> snode;

  for (nid0 = 0; nid0 < m_lnode; nid0++)
    snode.push_back(0);

  std::string msg("Testing Isolines(");
  std::stringstream ss;
  ss << iter;
  msg += ss.str() + ")...";
  te::common::TaskProgress task(msg, te::common::TaskProgress::UNDEFINED, (int)m_linesize);

  for (i = 0; i < m_linesize; i++)
  {
    task.pulse();

    if (m_line[i].getNodeFrom() == -1)
      continue;
    nid0 = m_line[i].getNodeFrom();
    if (nid0 > m_line[i].getNodeTo())
    {
      nid0 = m_line[i].getNodeTo();
      nid1 = m_line[i].getNodeFrom();
    }
    else
      nid1 = m_line[i].getNodeTo();
    if ((m_node[(unsigned int)nid0].getType() == Deletednode) || (m_node[(unsigned int)nid0].getType() > First))
    {
      //If deleted, last or sample point
      snode[(unsigned int)nid0] = 1;
      continue;
    }
    node = NextNode(nid0);
    if (node == nid1)
      snode[(unsigned int)nid0] = 1;
  }

  modified = false;
  for (nid0 = 0; nid0 < m_lnode; nid0++)
  {
    if ((m_node[(unsigned int)nid0].getType() == Deletednode) ||
      (m_node[(unsigned int)nid0].getType() > First))
      // If deleted, last or sample point
      continue;

    if (snode[(unsigned int)nid0] == 1)
      continue;

    // Find next non-deleted node
    nid1 = NextNode(nid0);
    if (nid1 == -1)
      continue;
    if ((m_node[(unsigned int)nid1].getType() != Normalnode) &&
      (m_node[(unsigned int)nid1].getType() != Last))
    {
      continue;
    }

    // Find triangle edge that intersects isoline segment
    NodeOppositeLines(nid0, lids);
    te::gm::PointZ pt0(m_node[(unsigned int)nid0].getNPoint());
    te::gm::PointZ pt1(m_node[(unsigned int)nid1].getNPoint());
    lidaux = -1;
    for (size_t ii = 0; ii < lids.size(); ii++)
    {
      linid1 = lids[ii];
      te::gm::PointZ ptl0(m_node[(unsigned int)m_line[(unsigned int)linid1].getNodeTo()].getNPoint());
      te::gm::PointZ ptl1(m_node[(unsigned int)m_line[(unsigned int)linid1].getNodeFrom()].getNPoint());
      if (segInterPoint(pt0, pt1, ptl0, ptl1, &ptf))
      {
        lids.clear();
        // Save first edge of triangulation that intersects isoline segment
        lidaux = linid1;
        break;
      }
    }
    if (lidaux == -1)
    {
      lids.clear();
      continue;
    }

    // Find triangle at opposite side of node, compared to one line
    ntri = -1;
    for (i = 0; i < 3; i++)
    {
      if (NodeId(m_line[(unsigned int)lidaux].getRightPolygon(), (short)i) == nid0)
      {
        ntri = m_line[(unsigned int)lidaux].getLeftPolygon();
        break;
      }
      if (NodeId(m_line[(unsigned int)lidaux].getLeftPolygon(), (short)i) == nid0)
      {
        ntri = m_line[(unsigned int)lidaux].getRightPolygon();
        break;
      }
    }
    if (ntri == -1)
      return false;

    tnodid = OppositeNode(m_line[(unsigned int)lidaux].getRightPolygon(), lidaux);
    vnodid = OppositeNode(m_line[(unsigned int)lidaux].getLeftPolygon(), lidaux);
    node = m_line[(unsigned int)lidaux].getNodeFrom();
    if (node > m_line[(unsigned int)lidaux].getNodeTo())
    {
      node = m_line[(unsigned int)lidaux].getNodeTo();
      nidaux = m_line[(unsigned int)lidaux].getNodeFrom();
    }
    else
      nidaux = m_line[(unsigned int)lidaux].getNodeTo();
    nidaux1 = NextNode(node);

    if ((tnodid == nid1) || (vnodid == nid1))
    {
      if (((m_node[(unsigned int)node].getType() != First) && (m_node[(unsigned int)node].getType() != Normalnode)) ||
        (nidaux1 != nidaux))
      {
        if (UpdateTriangles(m_line[(unsigned int)lidaux].getLeftPolygon(), m_line[(unsigned int)lidaux].getRightPolygon(), lidaux))
        {
          modified = true;
          continue;
        }
       }
    }

    ptaux.setNPoint(ptf);
    ptaux.setZ(m_node[(unsigned int)nid0].getZ());
    ptaux.setType(First);
    p3dl.push_back(ptaux);

    nidaux = nid0;
    oldline = lidaux;
    while (nidaux != nid1)
    {
      // Test if reached final isoline node
      m_triang[(unsigned int)ntri].LinesId(vlins);
      for (i = 0; i < 3; i++)
      {
        if (vlins[i] == oldline)
          continue;
        if ((m_line[(unsigned int)vlins[i]].getNodeTo() == nid1) ||
          (m_line[(unsigned int)vlins[i]].getNodeFrom() == nid1))
        {
          nidaux = nid1;
          break;
        }
      }
      if (nidaux == nid1)
        break;

      // Find intersection point between triangle edge and isoline segment
      for (i = 0; i < 3; i++)
      {
        if (vlins[i] == oldline)
          // If previous edge
          continue;
        linid1 = vlins[i];
        if ((m_line[(unsigned int)linid1].getNodeTo() == nid0) ||
          (m_line[(unsigned int)linid1].getNodeFrom() == nid0))
          continue;
        te::gm::PointZ ptl0(m_node[(unsigned int)m_line[(unsigned int)linid1].getNodeTo()].getNPoint());
        te::gm::PointZ ptl1(m_node[(unsigned int)m_line[(unsigned int)linid1].getNodeFrom()].getNPoint());
        if (segInterPoint(pt0, pt1, ptl0, ptl1, &pt))
          break;
      }
      if (i == 3)
      {
        break;
      }

      //  Insert intersected point to point list
      ptaux.setX(pt.getX());
      ptaux.setY(pt.getY());
      ptaux.setZ(m_node[(unsigned int)nid0].getZ());
      ptaux.setType(Normalnode);
      p3dl.push_back(ptaux);

      nidaux = m_line[(unsigned int)linid1].getNodeTo();
      oldline = linid1;
      if (m_line[(unsigned int)linid1].getLeftPolygon() == ntri)
        ntri = m_line[(unsigned int)linid1].getRightPolygon();
      else
        ntri = m_line[(unsigned int)linid1].getLeftPolygon();

      modified = true;
    }

    te::mnt::TinNode ptaux1 = p3dl.back();
    if (ptaux1.getType() == Normalnode)
      ptaux1.setType(Last);
    else if (ptaux1.getType() == First)
      ptaux1.setType(Sample);

    if (m_node[(unsigned int)nid0].getType() == First)
      m_node[(unsigned int)nid0].setType(Sample);
    else
      m_node[(unsigned int)nid0].setType(Last);
    if (m_node[(unsigned int)nid1].getType() == Normalnode)
      m_node[(unsigned int)nid1].setType(First);
    else
      m_node[(unsigned int)nid1].setType(Sample);
  }

  size_t npts = p3dl.size();
  if (npts == 0)
  {
    count = 0;
    return modified;
  }

  if ((npts + m_lnode + 6) > m_nodesize)
    ReallocateVectors(npts + m_lnode + 6);

  for (size_t ii = 0; ii < p3dl.size(); ii++)
  {
    ptaux = p3dl[ii];
    node = ++m_lnode;
    if (node > (int32_t)m_nodesize)
      return false;

    m_node[(unsigned int)node].Init(ptaux.getNPoint(), ptaux.getType());
    InsertNode(node, 0);
  }

  return modified;
}

bool te::mnt::TINGeneration::CreateMinAngleTriangulation()
{
  int32_t triangid, neighids[3];
  short j;

  te::common::TaskProgress task("Creating Minimum Angle...", te::common::TaskProgress::UNDEFINED, (int)m_ltriang);

  for (triangid = 0; triangid < m_ltriang; triangid++)
  {
    task.pulse();

    NeighborsId(triangid, neighids);
    for (j = 0; j < 3; j++)
    {
      if (TestAngleBetweenNormals(triangid, j))
        if (neighids[j] < triangid)
        {
          triangid = neighids[j] - 1;
          j = 3;
        }
        else
          j = -1;
    }
  }

  return true;
}

bool te::mnt::TINGeneration::TestAngleBetweenNormals(int32_t triId, short nviz)
{
  double nvector1[3], nvector2[3], cos1, cos2;
  int32_t nodid1, nodid2, linid, neighids[3], i;
  te::gm::PointZ vert1[3], vert2[3];

  if (triId == -1)
    return false;

  // Retrieve neighbour triangle (tviz) pointer
  NeighborsId(triId, neighids);
  if (neighids[nviz] == -1)
    return false;

  // Retrieve line of triangle common to neighbor triangle
  linid = m_triang[(unsigned int)triId].LineAtEdge((unsigned short)nviz);
  if (linid > (int32_t)m_linesize)
    return false;
  nodid1 = m_line[(unsigned int)linid].getNodeTo();
  if (nodid1 > m_line[(unsigned int)linid].getNodeFrom())
  {
    nodid1 = m_line[(unsigned int)linid].getNodeFrom();
    nodid2 = m_line[(unsigned int)linid].getNodeTo();
  }
  else
    nodid2 = m_line[(unsigned int)linid].getNodeFrom();
  if ((m_node[(unsigned int)nodid1].getZ() >= m_nodatavalue) || (m_node[(unsigned int)nodid2].getZ() >= m_nodatavalue))
    return false;

  TrianglePoints(triId, vert1);
  TrianglePoints(neighids[nviz], vert2);
  for (i = 0; i < 3; i++)
    if ((vert1[i].getZ() >= m_nodatavalue) || (vert2[i].getZ() >= m_nodatavalue))
      return false;

  // Define base triangle (tri) normal vector
  triangleNormalVector(vert1, nvector1);
  normalizeVector(nvector1);

  // Define opposite triangle (nviz) normal vector
  triangleNormalVector(vert2, nvector2);
  normalizeVector(nvector2);

  // Calculate cosine between triangles
  cos1 = nvector1[0] * nvector2[0] + nvector1[1] * nvector2[1] + nvector1[2] * nvector2[2];

  // Find opposite point to common edge on base triangle
  nodid1 = OppositeNode(triId, linid);

  // Find opposite point to common edge on neighbor triangle
  nodid2 = OppositeNode(neighids[nviz], linid);

  // Fill in first possible triangle points
  vert1[0].setX(m_node[(unsigned int)nodid1].getX());
  vert1[0].setY(m_node[(unsigned int)nodid1].getY());
  vert1[0].setZ(m_node[(unsigned int)nodid1].getZ());
  vert1[1].setX(m_node[(unsigned int)nodid2].getX());
  vert1[1].setY(m_node[(unsigned int)nodid2].getY());
  vert1[1].setZ(m_node[(unsigned int)nodid2].getZ());
  vert1[2].setX(m_node[(unsigned int)m_line[(unsigned int)linid].getNodeFrom()].getX());
  vert1[2].setY(m_node[(unsigned int)m_line[(unsigned int)linid].getNodeFrom()].getY());
  vert1[2].setZ(m_node[(unsigned int)m_line[(unsigned int)linid].getNodeFrom()].getZ());

  // Fill in second possible triangle points
  vert2[0].setX(m_node[(unsigned int)nodid1].getX());
  vert2[0].setY(m_node[(unsigned int)nodid1].getY());
  vert2[0].setZ(m_node[(unsigned int)nodid1].getZ());
  vert2[1].setX(m_node[(unsigned int)nodid2].getX());
  vert2[1].setY(m_node[(unsigned int)nodid2].getY());
  vert2[1].setZ(m_node[(unsigned int)nodid2].getZ());
  vert2[2].setX(m_node[(unsigned int)m_line[(unsigned int)linid].getNodeTo()].getX());
  vert2[2].setY(m_node[(unsigned int)m_line[(unsigned int)linid].getNodeTo()].getY());
  vert2[2].setZ(m_node[(unsigned int)m_line[(unsigned int)linid].getNodeTo()].getZ());

  // Define first possible triangle normal vector
  triangleNormalVector(vert1, nvector1);
  normalizeVector(nvector1);

  // Define second possible triangle normal vector
  triangleNormalVector(vert2, nvector2);
  normalizeVector(nvector2);

  //	Calculate cosine between triangles
  cos2 = nvector1[0] * nvector2[0] + nvector1[1] * nvector2[1] + nvector1[2] * nvector2[2];

  // If new triangles normal vector angle difference smaller than old triangle's
  if (cos2 > cos1)
    return UpdateTriangles(triId, neighids[nviz], linid);

  return false;
}


size_t te::mnt::TINGeneration::ReadBreakLines(te::gm::MultiPoint &mpt, te::gm::MultiLineString &breaklines, std::string &geostype)
{

  if (m_inDsetName_break.empty())
    return 0;

  std::auto_ptr<te::da::DataSet> inDset;
  size_t nbreak = mpt.getNumGeometries();

  inDset = m_inDsrc_break->getDataSet(m_inDsetName_break);

  std::size_t geo_pos = te::da::GetFirstPropertyPos(inDset.get(), te::dt::GEOMETRY_TYPE);

  inDset->moveBeforeFirst();

  while (inDset->moveNext())
  {
    std::auto_ptr<te::gm::Geometry> gin = inDset->getGeometry(geo_pos);
    geostype = gin.get()->getGeometryType();

    if (geostype == "LineString")
    {
      te::gm::LineString *l = dynamic_cast<te::gm::LineString*>(gin.get());

      te::gm::LineString *ls = pointListSimplify(l, m_tolerance_break, m_maxdist, 0.);
      breaklines.add(dynamic_cast<te::gm::Geometry*>(ls));
      nbreak += ls->size();
    }
    if (geostype == "MultiLineString")
    {
      te::gm::MultiLineString *g = dynamic_cast<te::gm::MultiLineString*>(gin.get());
      std::size_t np = g->getNumGeometries();
      for (std::size_t i = 0; i < np; ++i)
      {
        te::gm::LineString *l = dynamic_cast<te::gm::LineString*>(g->getGeometryN(i));
        te::gm::LineString *lz = new te::gm::LineString(l->size(), te::gm::LineStringType, breaklines.getSRID());

        for (std::size_t il = 0; il < l->size(); il++)
          lz->setPoint(il, l->getX(il), l->getY(il));
        l->setSRID(breaklines.getSRID());
        te::gm::LineString *ls = pointListSimplify(l, m_tolerance, m_maxdist, 0.);
        if (ls->size())
        {
          breaklines.add(dynamic_cast<te::gm::Geometry*>(ls));
          nbreak += ls->size();
        }
      }
    }
  }

  std::auto_ptr<te::gm::Envelope> env = inDset->getExtent(geo_pos);
  env->init((env->getLowerLeftX() - m_tolerance_break), (env->getLowerLeftY() - m_tolerance_break), (env->getUpperRightX() + m_tolerance_break), (env->getUpperRightY() + m_tolerance_break));

  setEnvelope(*env);

  return nbreak;
}

bool te::mnt::TINGeneration::InsertBreakNodes(te::gm::MultiLineString &breaklines)
{
  if (!m_nsderiv.size())
  // Calculate derivatives
    if (!NodeDerivatives())
      return false;

  std::vector<te::gm::PointZ> p3dl;
  std::vector<bool> fixed;

  te::common::TaskProgress task("Inserting BreakLines...", te::common::TaskProgress::UNDEFINED, (int)breaklines.getNumGeometries());

  // To all breaklines
  for (unsigned int id = 0; id < breaklines.getNumGeometries(); ++id)
  {
    task.pulse();

    te::gm::LineString* gout = dynamic_cast<te::gm::LineString*>(breaklines.getGeometryN(id));
    if ((gout->getNPoints() < 2))
    {
      continue;
    }
    for (std::size_t j = 0; j < gout->getNPoints()-1; ++j)
    {
      te::gm::Point *pf = gout->getPointN(j);
      te::gm::Point *pn = gout->getPointN(j + 1);
      te::gm::PointZ pfz(pf->getX(), pf->getY(), 0.);
      te::gm::PointZ pnz(pn->getX(), pn->getY(), 0.);

      // Fill list of points intersecting triangle edges
      if (!FindInterPoints(pfz, pnz, p3dl, fixed))
      {
        return false;
      }
    }

    // Mark last point of a line
    te::gm::PointZ p3d(std::numeric_limits< float >::max(), std::numeric_limits< float >::max(), std::numeric_limits< float >::max());
    p3dl.push_back(p3d);
    fixed.push_back(false);
  }

  RegeneratewithNewPoints(p3dl, fixed);

  return true;
}

bool te::mnt::TINGeneration::InsertBreakLines()
{
  te::gm::MultiPoint mpt(0, te::gm::MultiPointZType, m_srid);
  te::gm::MultiLineString breaklines(0, te::gm::MultiLineStringZType, m_srid);
  std::string geostype;
  ReadBreakLines(mpt, breaklines, geostype);

  InsertBreakNodes(breaklines);
  return true;
}


bool te::mnt::TINGeneration::FindInterPoints(te::gm::PointZ &pf, te::gm::PointZ &pn, std::vector<te::gm::PointZ> &p3d, std::vector<bool> &fixed)
{
  int32_t ftri, ltri, nids[3], lids[3],
    ledge, ntri;
  double tol = m_minedgesize;
  double dist, mindist;
  size_t i, k;
  bool fixedf = false;
  bool fixedn = false;

  // Search triangle containing first point
  ftri = FindTriangle(pf);

  // Search triangle containing last point
  ltri = FindTriangle(pn);
  if (ftri == ltri)
  {
    // If first triangle equal to last
    m_triang[(unsigned int)ftri].LinesId(lids);
    for (i = 0; i < 3; i++)
      if ((m_node[(unsigned int)m_line[(unsigned int)lids[i]].getNodeFrom()].getZ() >= m_nodatavalue) ||
        (m_node[(unsigned int)m_line[(unsigned int)lids[i]].getNodeTo()].getZ() >= m_nodatavalue))
      {
        pf.setZ(m_nodatavalue);
        pn.setZ(m_nodatavalue);
        break;
      }

    // Calculate zvalue of first and last points
    if (i == 3)
    {
      for (k = 0; k < 3; k++)
        if (OnIsolineSegment(lids[(unsigned int)k], pf, fixedf))
          // If on isoline segment
          break;

      for (k = 0; k < 3; k++)
        if (OnIsolineSegment(lids[(unsigned int)k], pn, fixedn))
          // If on isoline segment
          break;

      // Calculate zvalue of first and last points using quintic surface
      if (!fixedf && !fixedn)
        CalcZvalueAkima(ftri, pf, pn);
      else if (!fixedf)
        CalcZvalueAkima(ftri, pf, pf);
      else if (!fixedn)
        CalcZvalueAkima(ftri, pn, pn);
    }

    // Insert points in list
    p3d.push_back(pf);
    fixed.push_back(fixedf);
    p3d.push_back(pn);
    fixed.push_back(fixedn);

    return true;
  }

  // Search intersecting edge and point between first triangle
  // edges and the segment from first to last point.
  m_triang[(unsigned int)ftri].LinesId(lids);
  NeighborsId(ftri, nids);
  te::gm::PointZ pt(std::numeric_limits< float >::max(), 0., 0.);
  te::gm::PointZ minpt;
  int auxj = -1, j, minj;
  for (j = 0; j < 3; j++)
  {
    te::gm::PointZ pt1 = m_node[(unsigned int)m_line[(unsigned int)lids[j]].getNodeFrom()].getNPoint();
    te::gm::PointZ pt2 = m_node[(unsigned int)m_line[(unsigned int)lids[j]].getNodeTo()].getNPoint();
    if (segInterPoint(pf, pn, pt1, pt2, &pt))
    {
      if (Equal(pt, pt2, m_tolerance) || Equal(pt, pt1, m_tolerance))
      {
        auxj = j;
        minpt = pt;
      }
      else
        break;
    }
  }
  if ((j == 3) && (auxj != -1))
  {
    j = auxj;
    pt = minpt;
  }
  if (j == 3)
  {
    minj = -1;
    mindist = std::numeric_limits< float >::max();
    for (j = 0; j < 3; j++)
    {
      te::gm::PointZ pt1 = m_node[(unsigned int)m_line[(unsigned int)lids[j]].getNodeFrom()].getNPoint();
      te::gm::PointZ pt2 = m_node[(unsigned int)m_line[(unsigned int)lids[j]].getNodeTo()].getNPoint();
      dist = pointToSegmentDistance(pt1, pt2, pf, NULL);
      if (dist < tol)
      {
        if (dist < mindist)
        {
          minj = j;
          mindist = dist;
          minpt.setX(pf.getX());
          minpt.setY(pf.getY());
        }
        if (onSameSide(te::gm::Coord2D(pf.getX(), pf.getY()), te::gm::Coord2D(pn.getX(), pn.getY()), te::gm::Coord2D(pt1.getX(), pt1.getY()), te::gm::Coord2D(pt2.getX(), pt2.getY())) != 1)
        {
          pt.setX(pf.getX());
          pt.setY(pf.getY());
          break;
        }
      }
      dist = pointToSegmentDistance(pt1, pt2, pn, NULL);
      if (dist < tol)
      {
        if (dist < mindist)
        {
          minj = j;
          mindist = dist;
          minpt.setX(pn.getX());
          minpt.setY(pn.getY());
        }
        if (onSameSide(te::gm::Coord2D(pf.getX(), pf.getY()), te::gm::Coord2D(pn.getX(), pn.getY()), te::gm::Coord2D(pt1.getX(), pt1.getY()), te::gm::Coord2D(pt2.getX(), pt2.getY())) != 1)
        {
          pt.setX(pn.getX());
          pt.setY(pn.getY());
          break;
        }
      }
      dist = pointToSegmentDistance(pf, pn, pt1, NULL);
      if (dist < tol)
      {
        if (dist < mindist)
        {
          minj = j;
          mindist = dist;
          minpt.setX(pt1.getX());
          minpt.setY(pt1.getY());
        }
        if (onSameSide(te::gm::Coord2D(pf.getX(), pf.getY()), te::gm::Coord2D(pn.getX(), pn.getY()), te::gm::Coord2D(pt1.getX(), pt1.getY()), te::gm::Coord2D(pt2.getX(), pt2.getY())) != 1)
        {
          pt.setX(pt1.getX());
          pt.setY(pt1.getY());
          break;
        }
      }
      dist = pointToSegmentDistance(pf, pn, pt2, NULL);
      if (dist < tol)
      {
        if (dist < mindist)
        {
          minj = j;
          mindist = dist;
          minpt.setX(pt2.getX());
          minpt.setY(pt2.getY());
        }
        if (onSameSide(te::gm::Coord2D(pf.getX(), pf.getY()), te::gm::Coord2D(pn.getX(), pn.getY()), te::gm::Coord2D(pt1.getX(), pt1.getY()), te::gm::Coord2D(pt2.getX(), pt2.getY())) != 1)
        {
          pt.setX(pt2.getX());
          pt.setY(pt2.getY());
          break;
        }
      }
    }
    if (minj != -1)
    {
      j = minj;
      pt.setX(minpt.getX());
      pt.setY(minpt.getY());
    }
  }
  if (j == 3)
  {
    return false;
  }

  te::gm::PointZ p3t1(pf);
  te::gm::PointZ p3t2(pt);
  bool fixed1 = false;
  bool fixed2 = false;
  for (i = 0; i < 3; i++)
    if ((m_node[(unsigned int)m_line[(unsigned int)lids[i]].getNodeFrom()].getZ() >= m_nodatavalue) ||
      (m_node[(unsigned int)m_line[(unsigned int)lids[i]].getNodeTo()].getZ() >= m_nodatavalue))
    {
      p3t1.setZ(m_nodatavalue);
      p3t2.setZ(m_nodatavalue);
      break;
    }
  // Calculate zvalue of first and intersected points
  if (i == 3)
  {
    for (k = 0; k < 3; k++)
      if (OnIsolineSegment(lids[(unsigned int)k], p3t1, fixed1))
        // If on isoline segment
        break;

    for (k = 0; k < 3; k++)
      if (OnIsolineSegment(lids[(unsigned int)k], p3t2, fixed2))
        // If on isoline segment
        break;

    // Calculate zvalue of first and last points using quintic surface
    if (!fixed1 && !fixed2)
      CalcZvalueAkima(ftri, p3t1, p3t2);
    else if (!fixed1)
      CalcZvalueAkima(ftri, p3t1, p3t1);
    else if (!fixed2)
      CalcZvalueAkima(ftri, p3t2, p3t2);
  }

  // Insert points in list
  p3d.push_back(p3t1);
  fixed.push_back(fixed1);
  p3d.push_back(p3t2);
  fixed.push_back(fixed2);

  k = 0;

  while (nids[j] != ltri)
  {
    ledge = lids[j];
    ntri = nids[j];
    m_triang[(unsigned int)ntri].LinesId(lids);
    NeighborsId(ntri, nids);

    // Search intersecting edge and point in triangle
    pt.setX(m_nodatavalue);
    auxj = -1;
    for (j = 0; j < 3; j++)
    {
      if (lids[j] == ledge)
        continue;
      te::gm::PointZ pt1 = m_node[(unsigned int)m_line[(unsigned int)lids[j]].getNodeFrom()].getNPoint();
      te::gm::PointZ pt2 = m_node[(unsigned int)m_line[(unsigned int)lids[j]].getNodeTo()].getNPoint();
      if (segInterPoint(pf, pn, pt1, pt2, &pt))
      {
        if (Equal(pt,pt2, m_tolerance) || Equal(pt,pt1, m_tolerance))
        {
          auxj = j;
          minpt = pt;
        }
        else
          break;
      }
    }
    if ((j == 3) && (auxj != -1))
    {
      j = auxj;
      pt = minpt;
    }
    if (j == 3)
    {
      minj = -1;
      mindist = std::numeric_limits< float >::max();
      for (j = 0; j < 3; j++)
      {
        if (lids[j] == ledge)
          continue;
        te::gm::PointZ pt1 = m_node[(unsigned int)m_line[(unsigned int)lids[j]].getNodeFrom()].getNPoint();
        te::gm::PointZ pt2 = m_node[(unsigned int)m_line[(unsigned int)lids[j]].getNodeTo()].getNPoint();
        dist = pointToSegmentDistance(pt1, pt2, pf, NULL);
        if (dist < tol)
        {
          if (dist < mindist)
          {
            minj = j;
            mindist = dist;
            minpt.setX(pf.getX());
            minpt.setY(pf.getY());
          }
          if (onSameSide(te::gm::Coord2D(pf.getX(), pf.getY()), te::gm::Coord2D(pn.getX(), pn.getY()), te::gm::Coord2D(pt1.getX(), pt1.getY()), te::gm::Coord2D(pt2.getX(), pt2.getY())) != 1)
          {
            pt.setX(pf.getX());
            pt.setY(pf.getY());
            break;
          }
        }
        dist = pointToSegmentDistance(pt1, pt2, pn, NULL);
        if (dist < tol)
        {
          if (dist < mindist)
          {
            minj = j;
            mindist = dist;
            minpt.setX(pn.getX());
            minpt.setY(pn.getY());
          }
          if (onSameSide(te::gm::Coord2D(pf.getX(), pf.getY()), te::gm::Coord2D(pn.getX(), pn.getY()), te::gm::Coord2D(pt1.getX(), pt1.getY()), te::gm::Coord2D(pt2.getX(), pt2.getY())) != 1)
          {
            pt.setX(pn.getX());
            pt.setY(pn.getY());
            break;
          }
        }
        dist = pointToSegmentDistance(pf, pn, pt1, NULL);
        if (dist < tol)
        {
          if (dist < mindist)
          {
            minj = j;
            mindist = dist;
            minpt.setX(pt1.getX());
            minpt.setY(pt1.getY());
          }
          if (onSameSide(te::gm::Coord2D(pf.getX(), pf.getY()), te::gm::Coord2D(pn.getX(), pn.getY()), te::gm::Coord2D(pt1.getX(), pt1.getY()), te::gm::Coord2D(pt2.getX(), pt2.getY())) != 1)
          {
            pt.setX(pt1.getX());
            pt.setY(pt1.getY());
            break;
          }
        }
        dist = pointToSegmentDistance(pf, pn, pt2, NULL);
        if (dist < tol)
        {
          if (dist < mindist)
          {
            minj = j;
            mindist = dist;
            minpt.setX(pt2.getX());
            minpt.setY(pt2.getY());
          }
          if (onSameSide(te::gm::Coord2D(pf.getX(), pf.getY()), te::gm::Coord2D(pn.getX(), pn.getY()), te::gm::Coord2D(pt1.getX(), pt1.getY()), te::gm::Coord2D(pt2.getX(), pt2.getY())) != 1)
          {
            pt.setX(pt2.getX());
            pt.setY(pt2.getY());
            break;
          }
        }
      }
      if (minj != -1)
      {
        j = minj;
        pt.setX(minpt.getX());
        pt.setY(minpt.getY());
      }
      if (j == 3)
      {
        return false;
      }
    }

    if (k == 0)
    {
      // Search next intersecting point
      p3t1.setX(pt.getX());
      p3t1.setY(pt.getY());
      if (IsIsolineSegment(lids[j]))
      {
        fixed1 = true;
        p3t1.setZ(m_node[(unsigned int)m_line[(unsigned int)lids[j]].getNodeTo()].getZ());
      }
      else
        fixed1 = false;
      k++;
    }
    else
    {
      if (IsIsolineSegment(lids[j]))
      {
        fixed2 = true;
        p3t2.setZ(m_node[(unsigned int)m_line[(unsigned int)lids[j]].getNodeTo()].getZ());
      }
      else
        fixed2 = false;
      p3t2.setX(pt.getX());
      p3t2.setY(pt.getY());
      for (i = 0; i < 3; i++)
        if ((m_node[(unsigned int)m_line[(unsigned int)lids[i]].getNodeFrom()].getZ() >= m_nodatavalue) ||
          (m_node[(unsigned int)m_line[(unsigned int)lids[i]].getNodeTo()].getZ() >= m_nodatavalue))
        {
          p3t1.setZ(m_nodatavalue);
          p3t2.setZ(m_nodatavalue);
          break;
        }

      // Calculate zvalue of intersected point using quintic surface
      if (i == 3)
      {
        // Calculate zvalue of first and last points using quintic surface
        if (!fixed1 && !fixed2)
          CalcZvalueAkima(ntri, p3t1, p3t2);
        else if (!fixed1)
          CalcZvalueAkima(ntri, p3t1, p3t1);
        else if (!fixed2)
          CalcZvalueAkima(ntri, p3t2, p3t2);
      }
      p3d.push_back(p3t1);
      fixed.push_back(fixed1);
      p3d.push_back(p3t2);
      fixed.push_back(fixed2);
      k = 0;
    }
  }
  m_triang[(unsigned int)ltri].LinesId(lids);
  if (k == 0)
  {
    p3t1.setX(pn.getX());
    p3t1.setY(pn.getY());
    for (i = 0; i < 3; i++)
      if ((m_node[(unsigned int)m_line[(unsigned int)lids[i]].getNodeFrom()].getZ() >= m_nodatavalue) ||
        (m_node[(unsigned int)m_line[(unsigned int)lids[i]].getNodeTo()].getZ() >= m_nodatavalue))
      {
        p3t1.setZ(m_nodatavalue);
        break;
      }

    // Calculate zvalue of last point using quintic surface
    if (i == 3)
    {
      for (k = 0; k < 3; k++)
        if (OnIsolineSegment(lids[k], p3t1, fixed1))
          // If on isoline segment
          break;

      // Calculate zvalue of first and last points using quintic surface
      if (!fixed1)
        CalcZvalueAkima(ltri, p3t1, p3t1);
    }
    p3d.push_back(p3t1);
    fixed.push_back(fixed1);
  }
  else
  {
    p3t2.setX(pn.getX());
    p3t2.setY(pn.getY());
    for (i = 0; i < 3; i++)
      if ((m_node[(unsigned int)m_line[(unsigned int)lids[i]].getNodeFrom()].getZ() >= m_nodatavalue) ||
        (m_node[(unsigned int)m_line[(unsigned int)lids[i]].getNodeTo()].getZ() >= m_nodatavalue))
      {
        p3t1.setZ(m_nodatavalue);
        p3t2.setZ(m_nodatavalue);
        break;
      }

    if (i == 3)
    {
      for (k = 0; k < 3; k++)
        if (OnIsolineSegment(lids[k], p3t1, fixed1))
          // If on isoline segment
          break;

      for (k = 0; k < 3; k++)
        if (OnIsolineSegment(lids[k], p3t2, fixed2))
          // If on isoline segment
          break;

      // Calculate zvalue of first and last points using quintic surface
      if (!fixed2)
        CalcZvalueAkima(ltri, p3t1, p3t2);
    }
    p3d.push_back(p3t1);
    fixed.push_back(fixed1);
    p3d.push_back(p3t2);
    fixed.push_back(fixed2);
  }

  return true;
}

bool te::mnt::TINGeneration::OnIsolineSegment(int32_t linid, te::gm::PointZ &pt, bool &fixed)
{
  te::gm::PointZ pt1 = m_node[(unsigned int)m_line[(unsigned int)linid].getNodeFrom()].getNPoint();
  te::gm::PointZ pt2 = m_node[(unsigned int)m_line[(unsigned int)linid].getNodeTo()].getNPoint();

  fixed = false;
  if (onSegment(pt, pt1, pt2, m_minedgesize))
    if (IsIsolineSegment(linid))
    {
      // If on isoline segment
      pt.setZ(m_node[(unsigned int)m_line[(unsigned int)linid].getNodeFrom()].getZ());
      fixed = true; // Fixed Zvalue point
      return true;
    }

  return false;
}

bool te::mnt::TINGeneration::OrderLines()
{
  int32_t i, tri,
    nrtri,
    bline, nline, lline,
    lids[3],
    node1, node2,
    line1, line2;
  short j;

  // To all breakline nodes
  for (i = m_fbnode; i < (int32_t)m_node.size() - 1; i++)
  {
    if ((m_node[(unsigned int)i].getType() > Breaklinefirst) ||
      (m_node[(unsigned int)i].getType() == Deletednode))
      // If node is last or sample point
      continue;

    node1 = NextNode(i);
    if (node1 == -1) continue;

    if (m_node[(unsigned int)node1].getType() == Breaklinelast)
      // If next node is last of a breakline
      continue;

    // Search line from node to node+1
    bline = FindLine(i, node1);
    if (bline == -1)
    {
      if (m_node[(unsigned int)i].getType() == Breaklinefirst)
        m_node[(unsigned int)i].setType(Sample);
      else
        m_node[(unsigned int)i].setType(Breaklinelast);
      m_node[(unsigned int)node1].setType(Breaklinefirst);
      continue;
    }
    line1 = bline;

    // Search line from node+1 to node+2
    node2 = NextNode(node1);
    if (node2 == -1) continue;
    nline = FindLine(node1, node2);
    if (nline == -1)
    {
      m_node[(unsigned int)node1].setType(Breaklinelast);
      if (m_node[(unsigned int)node2].getType() == Breaklinelast)
        m_node[(unsigned int)node2].setType(Sample);
      else
        m_node[(unsigned int)node2].setType(Breaklinefirst);
      continue;
    }
    line2 = nline;

    // Make sure line pointing to next node
    if (m_line[(unsigned int)bline].getNodeTo() == i)
      m_line[(unsigned int)bline].SwapNodePolygon();

    tri = m_line[(unsigned int)bline].getRightPolygon();
    lline = bline;

    // Search lines at right side
    while ((lline != nline) && (tri != -1))
    {
      m_triang[(unsigned int)tri].LinesId(lids);
      for (j = 0; j < 3; j++)
      {
        if (lids[j] == lline)
          continue;
        if (m_line[(unsigned int)lids[j]].getNodeTo() == node1)
          break;
        if (m_line[(unsigned int)lids[j]].getNodeFrom() == node1)
        {
          // Make line pointing to next node
          m_line[(unsigned int)lids[j]].SwapNodePolygon();
          break;
        }
      }
      if (j == 3)
        return false;
      lline = lids[j];
      nrtri = m_line[(unsigned int)lline].getRightPolygon();
      if (nrtri == tri)
        m_line[(unsigned int)lline].SwapPolygon();
      tri = m_line[(unsigned int)lline].getRightPolygon();
    }

    tri = m_line[(unsigned int)bline].getLeftPolygon();
    lline = bline;

    // Search lines at left side
    while ((lline != nline) && (tri != -1))
    {
      m_triang[(unsigned int)tri].LinesId(lids);
      for (j = 0; j < 3; j++)
      {
        if (lids[j] == lline)
          continue;
        if (m_line[(unsigned int)lids[j]].getNodeFrom() == node1)
          break;
        if (m_line[(unsigned int)lids[j]].getNodeTo() == node1)
        {
          // Make line pointing to opposite node of next node
          m_line[(unsigned int)lids[j]].SwapNodePolygon();
          break;
        }
      }
      if (j == 3)
        return false;
      lline = lids[j];
      nrtri = m_line[(unsigned int)lline].getRightPolygon();
      if (nrtri == tri)
        m_line[(unsigned int)lline].SwapPolygon();
      tri = m_line[(unsigned int)lline].getRightPolygon();
    }
  }

  return true;
}

bool te::mnt::TINGeneration::ReCreateDelaunay()
{
  int32_t triangid, contr = 0, npoly = -1;

  te::common::TaskProgress task("Regeneration Delaunay...", te::common::TaskProgress::UNDEFINED, (int)m_ltriang);

  for (triangid = 0; triangid < m_ltriang; triangid++)
  {
    task.pulse();

    if (triangid > npoly)
      npoly = triangid;
    else
    {
      return false;
    }
    if (!ReGenerateDelaunay(npoly, npoly, contr) )
    {
      return false;
    }
  }

  return false;
}

bool te::mnt::TINGeneration::ReGenerateDelaunay(int32_t nt, int32_t ntbase, int32_t contr)
{
  int32_t aux, neighids[3];
  short j;

  contr++;
  if (contr > m_ltriang)
  {
    return false;
  }

  for (j = 0; j < 3; j++)
  {
    NeighborsId(nt, neighids);
    aux = neighids[(unsigned int)j];
    if (aux == nt)
      continue;

    // Test with each neighbor, if it exists
    if (aux == -1)
      continue;
    if (IsNeighborOnIsoOrBreakline(nt, (unsigned short)j))
      continue;
    if (TestDelaunay(nt, j))
    {
      // If changed,
      if (ntbase > aux)
        // If smaller number that base
        ReGenerateDelaunay(aux, ntbase, contr);

      // Returns to current neighbor
      j = -1;
    }
  }
  return true;
}

bool te::mnt::TINGeneration::TestFlatTriangles()
{
  if (!m_nsderiv.size())
    // Calculate derivatives
    if (!NodeDerivatives())
      return false;

  te::gm::PointZ vert[3], vertn[3];
  int32_t neighs[3];
  std::vector<int32_t> used_tri;
  std::vector<te::gm::PointZ> p3dl;
  std::vector<bool> fixed;

  for (int32_t tri = 0; tri <  (int32_t)m_triangsize; tri++)
  {
    if (std::find(used_tri.begin(), used_tri.end(), tri) != used_tri.end())
      continue;

    used_tri.push_back(tri);
    TrianglePoints(tri, vert);
    if (vert[0].getZ() == vert[1].getZ() && vert[0].getZ() == vert[2].getZ())
    {
      NeighborsId(tri, neighs);
      for (int n = 0; n < 3; n++)
      {
        te::gm::PointZ pc;
        TrianglePoints(neighs[n], vertn);

        if (vertn[0].getZ() == vertn[1].getZ() && vertn[0].getZ() == vertn[2].getZ())
        {
          if (std::find(used_tri.begin(), used_tri.end(), neighs[n]) != used_tri.end())
            continue;

          used_tri.push_back(neighs[n]);

          if (n == 0) // first edge
          {
            pc.setX((vert[0].getX() + vert[1].getX()) / 2.);
            pc.setY((vert[0].getY() + vert[1].getY()) / 2.);
          }
          if (n == 1) //second edge
          {
            pc.setX((vert[1].getX() + vert[2].getX()) / 2.);
            pc.setY((vert[1].getY() + vert[2].getY()) / 2.);
          }
          if (n == 2) //third edge
          {
            pc.setX((vert[2].getX() + vert[0].getX()) / 2.);
            pc.setY((vert[2].getY() + vert[0].getY()) / 2.);
          }

          CalcZvalueAkima(tri, pc, pc);
          p3dl.push_back(pc);
          fixed.push_back(false);
          break;
        }
      }
    }
  }

  RegeneratewithNewPoints(p3dl, fixed);

  return true;
}

te::gm::Geometry* te::mnt::TINGeneration::neigh_union(te::gm::Geometry* tri_union, int32_t tri, std::vector<int32_t> &used_tri, std::map<int32_t, te::gm::Polygon> &pol_tri)
{
  te::gm::PointZ vert[3];
  int32_t neighs[3];

  NeighborsId(tri, neighs);
  for (int n = 0; n < 3; n++)
  {
    int tri = neighs[n];
    if (std::find(used_tri.begin(), used_tri.end(), tri) != used_tri.end())
      continue;

    used_tri.push_back(tri);

    TrianglePoints(tri, vert);
    if (vert[0].getZ() == vert[1].getZ() && vert[0].getZ() == vert[2].getZ())
    {
      used_tri.push_back(tri);
      te::gm::Polygon tri_n(1, te::gm::PolygonZType, m_srid, 0);
      te::gm::LinearRing r(4, te::gm::LineStringZType, m_srid, 0);
      for (std::size_t i = 0; i < 4; ++i)
      {
        r.setX(i, vert[i%3].getX());
        r.setY(i, vert[i%3].getY());
        r.setZ(i, vert[i%3].getZ());
      }
      tri_n.setRingN(0, dynamic_cast<te::gm::Curve*>(r.clone()));

      pol_tri.insert(std::pair<int32_t, te::gm::Polygon>(tri, tri_n));

      tri_union = tri_union->Union(dynamic_cast<te::gm::Geometry*>(&tri_n));
      tri_union = neigh_union(tri_union, tri, used_tri, pol_tri);
    }
  }
  return tri_union;
}


bool te::mnt::TINGeneration::RegeneratewithNewPoints(std::vector<te::gm::PointZ> &p3dl, std::vector<bool> &fixed)
{
  point3dListFilter(p3dl, fixed, m_tolerance);
  size_t blnode = p3dl.size();

  size_t newtnodesize = blnode + m_lnode + 6;
  ReallocateVectors(newtnodesize);

  m_fbnode = m_lnode;
  bool frsflag = true;
  int32_t nidaux, node;
  for (size_t pp = 0; pp < p3dl.size(); pp++)
  {
    te::gm::PointZ p3d = p3dl[pp];
    if (p3d.getX() >= m_nodatavalue)
    {
      // If last point of a line, change type of previous node
      frsflag = true;
      nidaux = PreviousNode(m_lnode);

      if (m_node[(unsigned int)nidaux].getType() == Breaklinenormal)
        m_node[(unsigned int)nidaux].setType(Sample);
      else if (m_node[(unsigned int)nidaux].getType() == Last)
        m_node[(unsigned int)nidaux].setType(Breaklinelast);
      else
        return false;
      continue;
    }
    node = ++m_lnode;
    if (node > (int32_t)m_nodesize)
      return false;
    m_node[(unsigned int)node].setNPoint(p3d);
    if (frsflag)
    {
      // If first point of a line, set node type
      frsflag = false;
      m_node[(unsigned int)node].setType(Breaklinenormal);
    }
    else
      m_node[(unsigned int)node].setType(Last);
    InsertNode(node, 0);

  }

  //Check triangulation topology
  if (!CheckTopology())
    return false;

  //Order breaklines
  OrderLines();
  ReCreateDelaunay();
  if (m_nsderiv.size())
  {
    m_nsderiv.clear();
  }

  return true;

}

bool te::mnt::TINGeneration::borderUp()
{
  //Search ONBOXVERTEX vertex
  for (int32_t vii = 0; vii < (int32_t)m_node.size(); ++vii)
  {
    if ((m_node[(unsigned int)vii].getType() == Box)/* || (m_node[(unsigned int)vii].getType() == Deletednode)*/)
    {//if node has Box type
      //Search adjacent edges to the vertex
      std::vector <int32_t> lids;
      NodeLines(vii, lids);

      for (size_t li = 0; li < lids.size(); li++)
      { //Swaps vertex reference in the edges (from and/or to) for NULL
        int32_t lii = lids[li];
        int32_t from = m_line[(unsigned int)lii].getNodeFrom();
        int32_t to = m_line[(unsigned int)lii].getNodeTo();

        if (from == vii)
        {
          m_node[from].removeEdge(lii);
          m_line[(unsigned int)lii].setNodeFrom(-1);
          m_line[(unsigned int)lii].setType(Deletedline);
        }
        if (to == vii)
        {
          m_node[to].removeEdge(lii);
          m_line[(unsigned int)lii].setNodeTo(-1);
          m_line[(unsigned int)lii].setType(Deletedline);
        }

        //Swaps edge reference in the triangles for NULL
        long tleft = m_line[(unsigned int)lii].getLeftPolygon();
        long tright = m_line[(unsigned int)lii].getRightPolygon();

        //Check if triangle edges are equal eii
        int32_t edge[3];
        if (tleft != -1)
        {
          //left triangle
          m_triang[(unsigned int)tleft].LinesId(edge);
          if (edge[0] == lii)
            m_triang[(unsigned int)tleft].setEdges(-1, edge[1], edge[2]);
          if (edge[1] == lii)
            m_triang[(unsigned int)tleft].setEdges(edge[0], -1, edge[2]);
          if (edge[2] == lii)
            m_triang[(unsigned int)tleft].setEdges(edge[0], edge[1], -1);

        }
        if (tright != -1)
        { //right triangle
          m_triang[(unsigned int)tright].LinesId(edge);
          if (edge[0] == lii)
            m_triang[(unsigned int)tright].setEdges(-1, edge[1], edge[2]);
          if (edge[1] == lii)
            m_triang[(unsigned int)tright].setEdges(edge[0], -1, edge[2]);
          if (edge[2] == lii)
            m_triang[(unsigned int)tright].setEdges(edge[0], edge[1], -1);
        }

        std::vector <int32_t> edgesadj;
       //Checks if vertex from and to have reference to this edge in onEdge
        //If, there is the reference, changes it to another adjacent edge.
        if (from != -1)
        {
          std::vector<int32_t> edge = m_node[(unsigned int)from].getEdge();
          for (size_t i_e = 0; i_e < edge.size(); i_e++)
          {
            if (edge[i_e] == lii)//vertex from
            {
              NodeLines(from, edgesadj);
              for (size_t iAdj = 0; iAdj < edgesadj.size(); iAdj++)
              {
                if (edgesadj[iAdj] != -1 && edgesadj[iAdj] != lii)
                {
                  m_node[(unsigned int)from].setEdge(edgesadj[iAdj]);
                  break;
                }
              }
            }
          }
        }

        if (to != -1)
        {
          std::vector<int32_t> edge = m_node[(unsigned int)to].getEdge();
          for (size_t i_e = 0; i_e < edge.size(); i_e++)
          {
            if (edge[i_e] == lii)//vertex to
            {
              NodeLines(to, edgesadj);
              for (size_t iAdj = 0; iAdj < edgesadj.size(); iAdj++)
              {
                if (edgesadj[iAdj] != -1 && edgesadj[iAdj] != lii)
                {
                  m_node[(unsigned int)to].setEdge(edgesadj[iAdj]);
                  break;
                }
              }
            }
          }
        }
      }

//      m_node[(unsigned int)vii].setType(Deletednode);
    }
  }

  //Search triangles with NULL reference
  for (int32_t tit = 0; tit < (int32_t)m_triang.size(); ++tit)
  {
    int32_t edge[3];
    m_triang[(unsigned int)tit].LinesId(edge);
    if (edge[0] == -1 || edge[1] == -1 || edge[2] == -1)
    {//if triangle has NULL edge
      //For each triangle to be removed:
      //Changes triangle reference in the edges (left and right) to -1
      if (edge[0] != -1)
      {
        int32_t left = m_line[(unsigned int)edge[0]].getLeftPolygon();
        int32_t right = m_line[(unsigned int)edge[0]].getRightPolygon();

        if (left == tit)
          m_line[(unsigned int)edge[0]].setLeftPolygon(-1);
        if (right == tit)
          m_line[(unsigned int)edge[0]].setRightPolygon(-1);
      }

      if (edge[1] != -1)
      {
        long left = m_line[(unsigned int)edge[1]].getLeftPolygon();
        long right = m_line[(unsigned int)edge[1]].getRightPolygon();

        if (left == tit)
          m_line[(unsigned int)edge[1]].setLeftPolygon(-1);
        if (right == tit)
          m_line[(unsigned int)edge[1]].setRightPolygon(-1);
      }
      if (edge[2] != -1)
      {
        long left = m_line[(unsigned int)edge[2]].getLeftPolygon();
        long right = m_line[(unsigned int)edge[2]].getRightPolygon();

        if (left == tit)
          m_line[(unsigned int)edge[2]].setLeftPolygon(-1);
        if (right == tit)
          m_line[(unsigned int)edge[2]].setRightPolygon(-1);
      }
    }
  }

  return true;
}

