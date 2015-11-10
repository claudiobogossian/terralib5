/*!
\file terralib/mnt/core/TINGeneration.cpp

\brief This file contains a class to generate TIN.

*/

#include "TINGeneration.h"
#include "Utils.h"

//.. /..
#include "../../dataaccess/utils/Utils.h"

#include "../../geometry/Envelope.h"
#include "../../geometry/Geometry.h"
#include "../../geometry/LineString.h"
#include "../../geometry/Point.h"
#include "../../geometry/TIN.h"
#include "../../geometry/Triangle.h"

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
  size_t t1 = m_ltriang++;
  size_t t2 = m_ltriang++;
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
    m_node[nodelist[i]].Init(pointaux[i]);
    m_node[nodelist[i]].setType(Box);
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

    if (i<4) m_line[linlist[i]].Nodes(nodelist[i], nodelist[((i + 1) % 4)]);

    if (i < 2)
    {
      m_line[linlist[i]].Polygons(-1, (int32_t)t1);
    }
    else if (i < 4)
    {
      m_line[linlist[i]].Polygons(-1, (int32_t)t2);
    }
    else
    {
      m_line[linlist[i]].Nodes(nodelist[2], nodelist[0]);
      m_line[linlist[i]].Polygons((int32_t)t2, (int32_t)t1);
    }
  }

  //  Update triangles edges
  m_triang[t1].setEdges(linlist[0], linlist[1], linlist[4]);

  m_triang[t2].setEdges(linlist[4], linlist[2], linlist[3]);

  m_node[nodelist[0]].setEdge(linlist[0]);
  m_node[nodelist[1]].setEdge(linlist[1]);
  m_node[nodelist[2]].setEdge(linlist[2]);
  m_node[nodelist[3]].setEdge(linlist[3]);

  return true;
}


bool te::mnt::TINGeneration::InsertNodes(const te::gm::MultiPoint &mpt, const te::gm::MultiLineString &mls)
{
  int32_t node=0;
  //  Create nodes and insert on triangulation 
  for (size_t id = 0; id < mpt.getNumGeometries(); ++id)
  {
    te::gm::PointZ* pto3d = dynamic_cast<te::gm::PointZ*>(mpt.getGeometryN(id));
    node = ++m_lnode;
    if (node > (int32_t) m_nodesize)
      return false;
    m_node[node].Init(*pto3d, Sample);
    InsertNode(node, 1);
  }

  bool nflag;
  int32_t i;

  double PRECISAO = 0.000001;

  size_t mlsize = mls.getNumGeometries();
  for (unsigned int id = 0; id < mlsize; ++id)
  {
    te::gm::LineString* gout = dynamic_cast<te::gm::LineString*>(mls.getGeometryN(id));
    double *zvalue = gout->getZ();
    nflag = true;

    size_t gnp = gout->getNPoints();
    for (size_t j = 0; j < gnp; ++j)
    {
      te::gm::Point *p = gout->getPointN(j);
      te::gm::PointZ pz(p->getX(), p->getY(), p->getZ());
      // Test if new point is equal to inserted
      for (i = 0; i <m_lnode; i++)
      {
        if (Equal(m_node[i].getNPoint(), pz, PRECISAO))
          break;
      }
      if (i != m_lnode)
        continue;

      // for each point, create and insert node
      node = ++m_lnode;
      if (node > (int32_t)m_nodesize)
        return false;

      m_node[node].Init(pz);

      // Set node type
      if (nflag)
      {
        m_node[node].setType(First);
        InsertNode(node, 1);
        if (m_node[node].getType() == Deletednode)
          nflag = true;
        else
          nflag = false;
      }
      else
      {
        m_node[node].setType(Normalnode);
        InsertNode(node, 1);
      } //nflag
    } //for (std::size_t j = 0; j < gout->getNPoints(); ++j)
  
    if (m_node[node].getType() == Normalnode)
        m_node[node].setType(Last);
      else if (m_node[node].getType() == First)
        m_node[node].setType(Sample);
      else if ((m_node[node].getType() != Sample) && (m_node[node].getType() != Last))
        return false;
  } //for (unsigned int id = 0; id < mls.getNumGeometries(); ++id)

  m_fbnode = 0;

  return true;
}

bool te::mnt::TINGeneration::InsertNode(int32_t nodeId, int type)
{
  double tol = m_minedgesize;

  // Get Point to be inserted from vector
  te::gm::PointZ pt(m_node[nodeId].getNPoint());

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
  for (short j = 0; j < 3; j++)
  {
    if ((fabs(px - vert[j].getX()) < tol) && (fabs(py - vert[j].getY()) < tol))
    {
      NodesId(triangid, nids);
      NodeExchange(nids[j], nodeId);
      DeleteNode(nids[j]);
      return false;
    }
  }

  int32_t linesid[3], neighids[3];
  m_triang[triangid].LinesId(linesid);
  NeighborsId(triangid, neighids);

  //  Test if the point is on an edge
  int nedge = -1;
  double dmin = std::numeric_limits< float >::max();
  te::gm::PointZ paux, pmin;
  for (int j = 0; j < 3; j++)
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
      nedge = j;
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
      NeighborsId(triangid, neighids);
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
    m_node[nodeId].setNPoint(pmin);

    for (int j = 0; j < 3; j++)
    {
      if (neighids[j] == -1)
        continue;
      int32_t oppNode = OppositeNode(neighids[j], linesid[j]);
      if ((fabs(px - m_node[oppNode].getX()) < tol) && (fabs(py - m_node[oppNode].getY()) < tol))
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
    if (neighids[nedge] != -1)
    {
      int32_t ngtriang = neighids[nedge];
      if (ngtriang > (int32_t)m_triangsize)
        return false;
      int32_t testLines2[2];
      DupNeighTriangle(ngtriang, linid, nedge, nodeId, testLines2);
      for (int j = 0; j < 2; j++)
      {
        if (testLines2[j] == -1)
          break;
        if (TestDelaunay(testLines2[j]))
          j = -1;
      }

      //      Test Delaunay restriction with neighbour 1
      ntri1 = m_ltriang - 1;
      ntri2 = m_ltriang - 2;
      if (type == 0)
        TestDelaunay(triangid, ngtriang, ntri1, ntri2);
      else
      {
        NeighborsId(triangid, neighids);
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
        if (testLines[j] == -1)
          break;
        if (TestDelaunay(testLines[j]))
          j = -1;
      }
      NeighborsId(triangid, neighids);
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
  int32_t  nidaux;

  switch (m_node[node].getType())
  {
  case Normalnode:  // Isoline normal point
  case Sample:  // Sample point
    break;
  case First:  // Isoline first point
  {
    nidaux = NextNode(node);

    if (m_node[nidaux].getType() == Normalnode)
      m_node[nidaux].setType(First);
    else if (m_node[nidaux].getType() == Last)
      m_node[nidaux].setType(Sample);
    else
      return false;
    break;
  }
  case Last:  // Isoline last point
  {
    nidaux = PreviousNode(node);

    if (m_node[nidaux].getType() == Normalnode)
      m_node[nidaux].setType(Last);
    else if (m_node[nidaux].getType() == First)
      m_node[nidaux].setType(Sample);
    else
      return false;
    break;
  }
  case Breaklinenormal:  // Breakline normal point
  {
    nidaux = NextNode(node);

    if (nidaux == m_lnode)
      break;

    if (m_node[nidaux].getType() == Breaklinenormal)
      m_node[nidaux].setType(Breaklinefirst);
    else if (m_node[nidaux].getType() == Breaklinelast)
      m_node[nidaux].setType(Sample);
    else
      return false;
    nidaux = PreviousNode(node);

    if (m_node[nidaux].getType() == Breaklinenormal)
      m_node[nidaux].setType(Breaklinelast);
    else if (m_node[nidaux].getType() == Breaklinefirst)
      m_node[nidaux].setType(Sample);
    break;
  }
  case Breaklinefirst:  // Breakline first point
  {
    nidaux = NextNode(node);

    if (nidaux == m_lnode)
    {
      m_node[nidaux].setType(Breaklinefirst);
      break;
    }
    if (m_node[nidaux].getType() == Breaklinenormal)
      m_node[nidaux].setType(Breaklinefirst);
    else if (m_node[nidaux].getType() == Breaklinelast)
      m_node[nidaux].setType(Sample);
    else
      return false;
    break;
  }
  case Breaklinelast:  // Breakline last point
  {
    nidaux = PreviousNode(node);

    if (m_node[nidaux].getType() == Breaklinenormal)
      m_node[nidaux].setType(Breaklinelast);
    else if (m_node[nidaux].getType() == Breaklinefirst)
      m_node[nidaux].setType(Sample);
    else
    {
      return false;
    }
    break;
  }
  }
  m_node[node].setType(Deletednode);

  return true;
}


bool te::mnt::TINGeneration::NodeExchange(int32_t oldNode, int32_t newNode)
{
  std::vector<int32_t> lids;
  int32_t  linid = 0;

  NodeLines(oldNode, lids);
  for (size_t i = 0; i < lids.size(); i++)
  {
    linid = lids[i];
    m_line[linid].ExchangeNode(oldNode, newNode);
  }
  m_node[newNode].setX(m_node[oldNode].getX());
  m_node[newNode].setY(m_node[oldNode].getY());
  m_node[newNode].setZ(m_node[oldNode].getZ());
  m_node[newNode].setEdge(linid);
  return true;
}


bool te::mnt::TINGeneration::TwoNewTriangles(int32_t t, int32_t nodeId, int32_t* testLines)
{
  //1. Crie o vértice vn com os dados da amostra s,
  int32_t vn = nodeId;

  //2. Crie 2 novos triângulos t1 e t2,
  int32_t t1 = m_ltriang++;
  if (t1 > (int32_t)m_triangsize)
    return false;

  int32_t t2 = m_ltriang++;
  if (t2 > (int32_t)m_triangsize)
    return false;

  //3. Crie a nova aresta an0 que conecta os vértices vn e v0 e
  //   tem t e t1 como os triângulos que compartilham a aresta.
  int32_t an0 = m_lline++;
  m_line[an0].Nodes(vn, NodeId(t, 0)); //v0 e' o vertice 0 de t
  m_line[an0].Polygons(t, t1);

  //4. Crie a nova aresta an1 que conecta os vértices vn e v1 e
  //   tem t1 e t2 como os triângulos que compartilham a aresta.
  int32_t an1 = m_lline++;
  m_line[an1].Nodes(vn, NodeId(t, 1)); //v1 e' o vertice 1 de t
  m_line[an1].Polygons(t1, t2);

  //5. Crie a nova aresta an2 que conecta os vértices vn e v2 e
  //  tem t2 e t como os triângulos que compartilham a aresta.
  int32_t an2 = m_lline++;
  m_line[an2].Nodes(vn, NodeId(t, 2)); //v2 e' o vertice 2 de t
  m_line[an2].Polygons(t2, t);

  //6. Troque o triângulo t por t1 na aresta a0,
  int32_t a0 = ExchangePolygon(t, t1, 0); // retorna aresta 0 de t

  //7. Defina as arestas de t1 como sendo an0, a0 e an1,
  m_triang[t1].setEdges(an0, a0, an1);

  //8. Troque o triângulo t por t2 na aresta a1,
  int32_t a1 = ExchangePolygon(t, t2, 1); // retorna aresta 1 de t

  //9. Defina as arestas de t2 como sendo an1, a1 e an2,
  m_triang[t2].setEdges(an1, a1, an2);


  //10. Defina as arestas de t como sendo an2, a2 e an0,
  int32_t a2 = m_triang[t].LineAtEdge(2);
  m_triang[t].setEdges(an2, a2, an0);

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
  // Seja t o triângulo que contém cuja n-ésima aresta an é a aresta 
  // próxima a amostra s ( n Î {0,1,2} ) . A aresta an conecta os vértices 
  // vn e vj, a aresta aj conecta os vértices vj e vk e a aresta ak conecta
  // os vértices vk e vn, sendo j o resto da divisão de n+1 por 3 e k o resto
  // da divisão de n+2 por 3.
  int32_t lineids[3];
  m_triang[t].LinesId(lineids);

  int32_t an = lineids[n];
  int32_t aj = lineids[(n + 1) % 3];
  int32_t ak = lineids[(n + 2) % 3];
  int32_t vn = NodeId(t, n);

  testLines[0] = aj;
  testLines[1] = ak;

  //  1. Crie o vértice v com os dados da amostra s,
  //  2. Defina o triângulo tv que compartilha a aresta an com t,
  int32_t tv;
  if (m_line[an].getLeftPolygon() == t)
    tv = m_line[an].getRightPolygon();
  else if (m_line[an].getRightPolygon() == t)
    tv = m_line[an].getLeftPolygon();
  else{
    return -1;
  }

  //  3. Defina o vértice vop do triângulo t que não é conectado a aresta an,
  int32_t vop = NodeId(t, (n + 2) % 3);

  //  4. Crie o novo triângulos t1,
  int32_t t1 = m_ltriang++;
  if (t1 > (int32_t)m_triangsize)
    return -1;

  //  5. Crie a nova aresta an0 que conecta os vértices v e vn e
  //     tem t1 e tv como os triângulos que compartilham a aresta.
  int32_t an0 = m_lline++;
  if (an0 > (int32_t)m_linesize)
    return -1;

  m_line[an0].Nodes(v, vn);
  m_line[an0].Polygons(t1, tv);

  testLines[2] = an;
  testLines[3] = an0;

  //  6. Crie a nova aresta an1 que conecta os vértices v e vop e
  //     tem t e t1 como os triângulos que compartilham a aresta.
  int32_t an1 = m_lline++;
  if (an1 > (int32_t)m_linesize)
    return -1;

  m_line[an1].Nodes(v, vop);
  m_line[an1].Polygons(t, t1);

  testLines[4] = an1;

  //  7. Modifique a aresta an para conectar o vértice v ao invés de vn,
  m_line[an].ExchangeNode(vn, v);

  //  8. Defina as arestas de t como sendo an, aj e an1,
  m_triang[t].setEdges(an, aj, an1);

  //  9. Modifique a aresta ak para compartilhar o triângulo t1 ao invés de t,
  m_line[ak].ExchangePolygon(t, t1);

  //  10. Defina as arestas de t1 como sendo an0, an1 e ak,
  m_triang[t1].setEdges(an0, an1, ak);

  return an0;
}


bool te::mnt::TINGeneration::DupNeighTriangle(int32_t tv, int32_t an0, short, int32_t v, int32_t *testLines)
{
  //  11.1. Crie o novo triângulos t2,
  int32_t t2 = m_ltriang++;
  if (t2 > (int32_t)m_triangsize)
    return false;
  //  11.2. Defina a aresta av do triângulo tv que contém o vértice vn
  //        (obs: só há uma aresta porque a outra foi modificada),
  int32_t vn = m_line[an0].getNodeTo(); //Due to assembly, dangerous
  int32_t lineids[3];
  m_triang[tv].LinesId(lineids);
  short i;
  for (i = 0; i < 3; i++)
  {
    if (lineids[i] >(int32_t)m_linesize)
      return false;
    if ((m_line[lineids[i]].getNodeFrom() == vn) ||
      (m_line[lineids[i]].getNodeTo() == vn))
      break;
  }
  if (i == 3){
    return false;
  }
  int32_t av = lineids[i];

  testLines[0] = av;

  //  11.3. Defina as outras arestas de tv como sendo av1 e av2.
  int32_t av1 = lineids[(i + 1) % 3];
  int32_t av2 = lineids[(i + 2) % 3];

  testLines[1] = av1;

  //  11.4. Defina o vértice vvo conectado a vn por meio da aresta av,
  int32_t vvo;
  if (m_line[av].getNodeFrom() == vn)
    vvo = m_line[av].getNodeTo();
  else
    vvo = m_line[av].getNodeFrom();

  //  11.5. Crie a nova aresta an2 que conecta os vértices v e vvo e
  //        tem t e t2 como os triângulos que compartilham a aresta.
  int32_t an2 = m_lline++;
  if (an2 > (int32_t)m_linesize)
    return false;
  m_line[an2].Nodes(v, vvo);
  m_line[an2].Polygons(tv, t2);

  //  11.6. Troque o triângulo tv por t2 na aresta av,
  m_line[av].ExchangePolygon(tv, t2);

  //  11.7. Troque o triângulo tv por t2 na aresta an0,
  m_line[an0].ExchangePolygon(tv, t2);

  //  11.8. Defina as arestas de tv como sendo an2, av1 e av2.
  m_triang[tv].setEdges(an2, av1, av2);

  //  11.9. Defina as arestas de t2 como sendo an0, av e an2,
  m_triang[t2].setEdges(an0, av, an2);

  return true;
}

bool te::mnt::TINGeneration::TestDelaunay(int32_t tri1Id, int32_t tri2Id, int32_t tri3Id)
{
  int32_t  nids[3];
  short  j;

  NeighborsId(tri1Id, nids);
  for (j = 0; j < 3; j++)
    if ((nids[j] != tri2Id) && (nids[j] != tri3Id))
      break;
  if (j == 3)
    return false;
  if (IsNeighborOnIsoOrBreakline(tri1Id, j) == false)
    TestDelaunay(tri1Id, j);

  NeighborsId(tri2Id, nids);
  for (j = 0; j < 3; j++)
    if ((nids[j] != tri1Id) && (nids[j] != tri3Id))
      break;
  if (j == 3)
    return false;
  if (IsNeighborOnIsoOrBreakline(tri2Id, j) == false)
    TestDelaunay(tri2Id, j);

  NeighborsId(tri3Id, nids);
  for (j = 0; j < 3; j++)
    if ((nids[j] != tri1Id) && (nids[j] != tri2Id))
      break;
  if (j == 3)
    return false;
  if (IsNeighborOnIsoOrBreakline(tri3Id, j) == false)
    TestDelaunay(tri3Id, j);

  return true;
}


bool te::mnt::TINGeneration::TestDelaunay(int32_t tri1Id, int32_t tri2Id, int32_t tri3Id, int32_t tri4Id)
{
  int32_t  nids[3];
  short  j;

  NeighborsId(tri1Id, nids);
  for (j = 0; j < 3; j++)
    if ((nids[j] != tri2Id) && (nids[j] != tri3Id) && (nids[j] != tri4Id))
      break;
  if (j == 3)
    return false;
  if (IsNeighborOnIsoOrBreakline(tri1Id, j) == false)
    TestDelaunay(tri1Id, j);

  NeighborsId(tri2Id, nids);
  for (j = 0; j < 3; j++)
    if ((nids[j] != tri1Id) && (nids[j] != tri3Id) && (nids[j] != tri4Id))
      break;
  if (j == 3)
    return false;
  if (IsNeighborOnIsoOrBreakline(tri2Id, j) == false)
    TestDelaunay(tri2Id, j);

  NeighborsId(tri3Id, nids);
  for (j = 0; j < 3; j++)
    if ((nids[j] != tri1Id) && (nids[j] != tri2Id) && (nids[j] != tri4Id))
      break;
  if (j == 3)
    return false;
  if (IsNeighborOnIsoOrBreakline(tri3Id, j) == false)
    TestDelaunay(tri3Id, j);

  NeighborsId(tri4Id, nids);
  for (j = 0; j < 3; j++)
    if ((nids[j] != tri1Id) && (nids[j] != tri2Id) && (nids[j] != tri3Id))
      break;
  if (j == 3)
    return false;
  if (IsNeighborOnIsoOrBreakline(tri4Id, j) == false)
    TestDelaunay(tri4Id, j);

  return true;
}


bool te::mnt::TINGeneration::TestDelaunay(int32_t linId)
{
  int32_t tv = m_line[linId].getLeftPolygon();
  if (tv == -1)
    return false;

  int32_t  linids[3];
  m_triang[tv].LinesId(linids);

  short j;
  for (j = 0; j < 3; j++)
    if (linids[j] == linId)
      break;
  if (j != 3)
    return TestDelaunay(tv, j);
  else
    return false;
}


bool te::mnt::TINGeneration::TestDelaunay(int32_t triId, short nviz)
{
  double  xc, yc, xaux, yaux, raio2, dist2,
    xo, yo;
  int32_t  nodid, linid, neighids[3];
  te::gm::PointZ  vert[3];
  short  i;
  double  minx = std::numeric_limits< float >::max(),
    miny = std::numeric_limits< float >::max();

  if (triId == -1)
    return false;

  //  Retrieve neighbour triangle (tviz) pointer
  NeighborsId(triId, neighids);
  if (neighids[nviz] == -1)
    return false;

  //  Retrieve line of triangle common to neighbor triangle
  linid = m_triang[triId].LineAtEdge(nviz);
  if (linid > (int32_t) m_linesize)
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
    xo = m_node[nodid].getNPoint().getX() - minx;
    yo = m_node[nodid].getNPoint().getY() - miny;
  }
  else
  {
    xo = m_node[nodid].getNPoint().getX();
    yo = m_node[nodid].getNPoint().getY();
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
  // Seja t o triângulo que contém cuja i-ésima aresta ai é a aresta
  //  que se deseja alterar ( i Î {0,1,2} ). A aresta ai conecta os
  //  vértices vi e vj, a aresta aj conecta os vértices vj e vk e a
  //  aresta ak conecta os vértices vk e vi, sendo j o resto da divisão
  //  de i+1 por 3 e k o resto da divisão de i+2 por 3.

  if (tv == -1)
    return false;

  int32_t tEdges[3];
  m_triang[t].LinesId(tEdges);

  int32_t nodeIds[3];
  NodesId(t, nodeIds);

  short i;
  for (i = 0; i < 3; i++)
    if (tEdges[i] == ai)
      break;

  if (i == 3) return false;

  int32_t aj = tEdges[(i + 1) % 3];
  int32_t ak = tEdges[(i + 2) % 3];

  int32_t vi = nodeIds[i];
  int32_t vj = nodeIds[(i + 1) % 3];
  int32_t vk = nodeIds[(i + 2) % 3];

  // Seja tv o triângulo que compartilha a aresta ai com t. O vértice de
  //  tv que não é conectado pela aresta ai é o vértice vn. As outras
  //  arestas do triângulo tv são am que conecta os vértices vi e vn e a
  //  aresta an conecta os vértices vn e vj.

  int32_t vn = OppositeNode(tv, ai);

  int32_t tvEdges[3];
  m_triang[tv].LinesId(tvEdges);

  int32_t am = 0, an = 0;
  short j;
  for (j = 0; j < 3; j++)
  {
    if (tvEdges[j] == ai)
      continue;

    if (m_line[tvEdges[j]].getNodeFrom() == vn)
    {
      if (m_line[tvEdges[j]].getNodeTo() == vi)
        am = tvEdges[j];
      else if (m_line[tvEdges[j]].getNodeTo() == vj)
        an = tvEdges[j];
      else{
        return false;
      }
    }
    else if (m_line[tvEdges[j]].getNodeTo() == vn)
    {
      if (m_line[tvEdges[j]].getNodeFrom() == vi)
        am = tvEdges[j];
      else if (m_line[tvEdges[j]].getNodeFrom() == vj)
        an = tvEdges[j];
      else{
        return false;
      }
    }

  }

  //  1. Se o segmento de reta formado conectando o vértice vk do
  //     triângulo t ao vértice vn de tv intersecta a aresta ai, faça:
  te::gm::PointZ ptvk = m_node[vk].getNPoint();
  te::gm::PointZ ptvn = m_node[vn].getNPoint();
  te::gm::PointZ ptvi = m_node[vi].getNPoint();
  te::gm::PointZ ptvj = m_node[vj].getNPoint();

  if (!segIntersect(ptvi, ptvj, ptvk, ptvn))
    return false;

  //    1.1. Troque na aresta ai o vértice vi pelo vertice vk e o
  //       vértice vj pelo vértice vn.
  if (m_line[ai].getNodeTo() == vi)
  {
    m_line[ai].setNodeTo(vk);
    m_line[ai].setNodeFrom(vn); // this is vj
  }
  else if (m_line[ai].getNodeTo() == vj)
  {
    m_line[ai].setNodeTo(vn);
    m_line[ai].setNodeFrom(vk); // this is vi
  }
  std::vector<int32_t> edge_vi = m_node[vi].getEdge();
  for (size_t i_vi = 0; i_vi < edge_vi.size(); i_vi++)
  {
    if (edge_vi[i_vi] == ai)
      m_node[vi].setEdge(edge_vi[i_vi]);
  }
  std::vector<int32_t> edge_vj = m_node[vj].getEdge();
  for (size_t i_vj = 0; i_vj < edge_vj.size(); i_vj++)
  {
    if (edge_vj[i_vj] == ai)
      m_node[vj].setEdge(aj);
  }

  //    1.2. Troque na aresta an o triângulo tv pelo triângulo t.
  if (m_line[an].getRightPolygon() == tv)
    m_line[an].setRightPolygon(t);
  else
    m_line[an].setLeftPolygon(t);

  //    1.3. Troque na aresta ak o triângulo t pelo triângulo tv.
  if (m_line[ak].getRightPolygon() == t)
    m_line[ak].setRightPolygon(tv);
  else
    m_line[ak].setLeftPolygon(tv);

  //    1.4. Troque no triângulo t a aresta ai pela aresta an e a
  //       aresta ak pela aresta ai.
  for (i = 0; i < 3; i++)
    if (tEdges[i] == ai)
      break;
  if (i < 3) tEdges[i] = an;
  tEdges[(i + 2) % 3] = ai;
  m_triang[t].setEdges(tEdges[0], tEdges[1], tEdges[2]);

  //    1.5. Troque no triângulo tv a aresta ai pela aresta ak
  for (i = 0; i < 3; i++)
    if (tvEdges[i] == ai)
      break;
  if (i < 3) tvEdges[i] = ak;

  //     e a aresta an pela aresta ai.
  for (i = 0; i < 3; i++)
    if (tvEdges[i] == an)
      break;
  if (i < 3) tvEdges[i] = ai;

  m_triang[tv].setEdges(tvEdges[0], tvEdges[1], tvEdges[2]);

  return true;
}


int32_t te::mnt::TINGeneration::ExchangePolygon(int32_t triangId, int32_t newPolyId, short edge)
{
  int32_t  linId;

  linId = m_triang[triangId].LineAtEdge(edge);
  if (linId == -1)
    return false;
  if (linId > (int32_t)m_linesize)
    return false;
  m_line[linId].ExchangePolygon(triangId, newPolyId);

  return linId;
}


bool te::mnt::TINGeneration::IsIsolineSegment(int32_t linid)
{
  int32_t  nid0, nid1;

  if ((m_node[m_line[linid].getNodeTo()].getType() == Sample) ||
    (m_node[m_line[linid].getNodeFrom()].getType() == Sample))
    //      If sample point
    return false;

  if ((m_node[m_line[linid].getNodeTo()].getType() > Last) ||
    (m_node[m_line[linid].getNodeFrom()].getType() > Last))
    //      If breakline point
    return false;

  nid0 = m_line[linid].getNodeFrom();
  if (nid0 > m_line[linid].getNodeTo())
  {
    nid0 = m_line[linid].getNodeTo();
    nid1 = m_line[linid].getNodeFrom();
  }
  else
    nid1 = m_line[linid].getNodeTo();

  if (m_node[nid0].getType() == Last)
    //    If first point is last point of line
    return false;

  if (m_node[nid1].getType() == First)
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

  if ((m_node[m_line[linid].getNodeTo()].getType() == Sample) ||
    (m_node[m_line[linid].getNodeFrom()].getType() == Sample))
    //      If sample point
    return false;

  if ((m_node[m_line[linid].getNodeTo()].getType() < Breaklinenormal) ||
    (m_node[m_line[linid].getNodeFrom()].getType() < Breaklinenormal))
    //      If breakline point
    return false;

  nid0 = m_line[linid].getNodeFrom();
  if (nid0 > m_line[linid].getNodeTo())
  {
    nid0 = m_line[linid].getNodeTo();
    nid1 = m_line[linid].getNodeFrom();
  }
  else
    nid1 = m_line[linid].getNodeTo();

  if (m_node[nid0].getType() == Breaklinelast)
    //    If first point is last point of line
    return false;

  if (m_node[nid1].getType() == Breaklinefirst)
    //    If last point is first point of line
    return false;

  //  Search next non-deleted point
  if (NextNode(nid0) != nid1)
    return false;

  return true;
}



bool te::mnt::TINGeneration::IsNeighborOnIsoOrBreakline(int32_t triId, short nviz)
{
  int32_t  linid;

  //  Retrieve line of triangle common to neighbor triangle
  linid = m_triang[triId].LineAtEdge(nviz);
  if (IsIsolineSegment(linid))
    return true;
  if (IsBreaklineSegment(linid))
    return true;
  return false;
}

bool te::mnt::TINGeneration::CreateDelaunay()
{
  int32_t triangid, contr = 0, npoly = -1;

  for (triangid = 0; triangid < m_ltriang; triangid++)
  {
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
    aux = neighids[j];
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
  int32_t nid, linid, rtri, ltri;
  int32_t nids[4] = { 0, 0, 0, 0 }, tnids[3] = { 0, 0, 0 };
  int32_t   trids[4] = {0, 0, 0, 0};
  short  i, j, modified;

  i = 0;
  for (nid = 1; nid < m_node.size(); nid++)
  {
    if (m_node[nid].getType() != Box &&
      m_node[nid].getType() != Deletednode &&
      m_node[nid].getType() != Sample)
      continue;
    if (m_node[nid].getZ() < m_nodatavalue)
      //      If not fake sample point
      continue;
    nids[i++] = nid;
    if (i == 4)
      break;
  }

  modified = 0;
  for (i = 0; i < 4; i++)
  {
    NodeLines(nids[i], lids);
    for (size_t ii = 0; ii < lids.size(); ii++)
    {
      linid = lids[ii];
      rtri = m_line[linid].getRightPolygon();
      ltri = m_line[linid].getLeftPolygon();
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
        if (((m_line[linid].getNodeFrom() == nids[i]) &&
          ((m_line[linid].getNodeTo() == nids[(i + 1) % 4]) ||
          (m_line[linid].getNodeTo() == nids[(i + 2) % 4]) ||
          (m_line[linid].getNodeTo() == nids[(i + 3) % 4]))) ||
          ((m_line[linid].getNodeTo() == nids[i]) &&
          ((m_line[linid].getNodeFrom() == nids[(i + 1) % 4]) ||
          (m_line[linid].getNodeFrom() == nids[(i + 2) % 4]) ||
          (m_line[linid].getNodeFrom() == nids[(i + 3) % 4]))))
        {
          if (rtri == -1)
            trids[i] = ltri;
          else
            trids[i] = rtri;
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
    m_triang[trids[i]].LinesId(tnids);
    for (j = 0; j < 3; j++)
    {
      rtri = m_line[tnids[j]].getRightPolygon();
      ltri = m_line[tnids[j]].getLeftPolygon();
      if ((rtri != -1) && (ltri != -1))
        UpdateTriangles(rtri, ltri, tnids[j]);
    }
  }
  return true;
}


bool te::mnt::TINGeneration::IsolinesConstrained()
{
  for (;;)
    if (!TestIsolines())
      break;

  return true;
}


bool te::mnt::TINGeneration::TestIsolines()
{
  int32_t nid0, nid1, node, nidaux, nidaux1,
    npts,
    linid1 = 0, lidaux, oldline,
    vlins[3], ntri,
    tnodid, vnodid,
    i;
  std::vector<int32_t> lids;
  te::gm::PointZ pt, ptf, npt0, npt1;
  bool modified = false;
  static size_t count = 0;
  std::vector<te::mnt::TinNode> p3dl;
  te::mnt::TinNode ptaux;
  std::vector<int32_t> snode;

  for (nid0 = 0; nid0 < m_lnode; nid0++)
    snode.push_back(0);
  for (i = 0; i < m_linesize; i++)
  {
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
    if ((m_node[nid0].getType() == Deletednode) || (m_node[nid0].getType() > First))
    {
      //If deleted, last or sample point
      snode[nid0] = 1;
      continue;
    }
    node = NextNode(nid0);
    if (node == nid1)
      snode[nid0] = 1;
  }

  modified = false;
  for (nid0 = 0; nid0 < m_lnode; nid0++)
  {
    if ((m_node[nid0].getType() == Deletednode) ||
      (m_node[nid0].getType() > First))
      // If deleted, last or sample point
      continue;

    if (snode[nid0] == 1)
      continue;

    // Find next non-deleted node
    nid1 = NextNode(nid0);
    if (nid1 == -1)
      continue;
    if ((m_node[nid1].getType() != Normalnode) &&
      (m_node[nid1].getType() != Last))
    {
      continue;
    }

    // Find triangle edge that intersects isoline segment
    NodeOppositeLines(nid0, lids);
    te::gm::PointZ pt0(m_node[nid0].getNPoint());
    te::gm::PointZ pt1(m_node[nid1].getNPoint());
    lidaux = -1;
    for (size_t ii = 0; ii < lids.size(); ii++)
    {
      linid1 = lids[ii];
      te::gm::PointZ ptl0(m_node[m_line[linid1].getNodeTo()].getNPoint());
      te::gm::PointZ ptl1(m_node[m_line[linid1].getNodeFrom()].getNPoint());
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
      if (NodeId(m_line[lidaux].getRightPolygon(), i) == nid0)
      {
        ntri = m_line[lidaux].getLeftPolygon();
        break;
      }
      if (NodeId(m_line[lidaux].getLeftPolygon(), i) == nid0)
      {
        ntri = m_line[lidaux].getRightPolygon();
        break;
      }
    }
    if (ntri == -1)
      return false;

    tnodid = OppositeNode(m_line[lidaux].getRightPolygon(), lidaux);
    vnodid = OppositeNode(m_line[lidaux].getLeftPolygon(), lidaux);
    node = m_line[lidaux].getNodeFrom();
    if (node > m_line[lidaux].getNodeTo())
    {
      node = m_line[lidaux].getNodeTo();
      nidaux = m_line[lidaux].getNodeFrom();
    }
    else
      nidaux = m_line[lidaux].getNodeTo();
    nidaux1 = NextNode(node);

    if ((tnodid == nid1) || (vnodid == nid1))
    {
      if (((m_node[node].getType() != First) && (m_node[node].getType() != Normalnode)) ||
        (nidaux1 != nidaux))
      {
        if (UpdateTriangles(m_line[lidaux].getLeftPolygon(), m_line[lidaux].getRightPolygon(), lidaux))
        {
          modified = true;
          continue;
        }
       }
    }

    ptaux.setNPoint(ptf);
    ptaux.setZ(m_node[nid0].getZ());
    ptaux.setType(First);
    p3dl.push_back(ptaux);

    nidaux = nid0;
    oldline = lidaux;
    while (nidaux != nid1)
    {
      // Test if reached final isoline node
      m_triang[ntri].LinesId(vlins);
      for (i = 0; i < 3; i++)
      {
        if (vlins[i] == oldline)
          continue;
        if ((m_line[vlins[i]].getNodeTo() == nid1) ||
          (m_line[vlins[i]].getNodeFrom() == nid1))
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
        if ((m_line[linid1].getNodeTo() == nid0) ||
          (m_line[linid1].getNodeFrom() == nid0))
          continue;
        te::gm::PointZ ptl0(m_node[m_line[linid1].getNodeTo()].getNPoint());
        te::gm::PointZ ptl1(m_node[m_line[linid1].getNodeFrom()].getNPoint());
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
      ptaux.setZ(m_node[nid0].getZ());
      ptaux.setType(Normalnode);
      p3dl.push_back(ptaux);

      nidaux = m_line[linid1].getNodeTo();
      oldline = linid1;
      if (m_line[linid1].getLeftPolygon() == ntri)
        ntri = m_line[linid1].getRightPolygon();
      else
        ntri = m_line[linid1].getLeftPolygon();

      modified = true;
    }

    te::mnt::TinNode ptaux1 = p3dl.back();
    if (ptaux1.getType() == Normalnode)
      ptaux1.setType(Last);
    else if (ptaux1.getType() == First)
      ptaux1.setType(Sample);

    if (m_node[nid0].getType() == First)
      m_node[nid0].setType(Sample);
    else
      m_node[nid0].setType(Last);
    if (m_node[nid1].getType() == Normalnode)
      m_node[nid1].setType(First);
    else
      m_node[nid1].setType(Sample);
  }


  npts =(int32_t) p3dl.size();
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
    if (node > m_nodesize)
      return false;

    m_node[node].Init(ptaux.getNPoint(), ptaux.getType());
    InsertNode(node, 0);
  }

  return modified;
}

bool te::mnt::TINGeneration::CreateMinAngleTriangulation()
{
  int32_t triangid, neighids[3];
  short j;

  for (triangid = 0; triangid < m_ltriang; triangid++)
  {
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
  linid = m_triang[triId].LineAtEdge(nviz);
  if (linid > m_linesize)
    return false;
  nodid1 = m_line[linid].getNodeTo();
  if (nodid1 > m_line[linid].getNodeFrom())
  {
    nodid1 = m_line[linid].getNodeFrom();
    nodid2 = m_line[linid].getNodeTo();
  }
  else
    nodid2 = m_line[linid].getNodeFrom();
  if ((m_node[nodid1].getZ() >= m_nodatavalue) || (m_node[nodid2].getZ() >= m_nodatavalue))
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
  vert1[0].setX(m_node[nodid1].getX());
  vert1[0].setY(m_node[nodid1].getY());
  vert1[0].setZ(m_node[nodid1].getZ());
  vert1[1].setX(m_node[nodid2].getX());
  vert1[1].setY(m_node[nodid2].getY());
  vert1[1].setZ(m_node[nodid2].getZ());
  vert1[2].setX(m_node[m_line[linid].getNodeFrom()].getX());
  vert1[2].setY(m_node[m_line[linid].getNodeFrom()].getY());
  vert1[2].setZ(m_node[m_line[linid].getNodeFrom()].getZ());

  // Fill in second possible triangle points
  vert2[0].setX(m_node[nodid1].getX());
  vert2[0].setY(m_node[nodid1].getY());
  vert2[0].setZ(m_node[nodid1].getZ());
  vert2[1].setX(m_node[nodid2].getX());
  vert2[1].setY(m_node[nodid2].getY());
  vert2[1].setZ(m_node[nodid2].getZ());
  vert2[2].setX(m_node[m_line[linid].getNodeTo()].getX());
  vert2[2].setY(m_node[m_line[linid].getNodeTo()].getY());
  vert2[2].setZ(m_node[m_line[linid].getNodeTo()].getZ());

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

  const std::size_t np = inDset->getNumProperties();
  const std::size_t ng = inDset->size();

  std::size_t geo_pos = te::da::GetFirstPropertyPos(inDset.get(), te::dt::GEOMETRY_TYPE);

  inDset->moveBeforeFirst();
  std::size_t pos = 0;

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

  // To all breaklines
  for (unsigned int id = 0; id < breaklines.getNumGeometries(); ++id)
  {
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
  size_t nbreaklines = ReadBreakLines(mpt, breaklines, geostype);

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
    m_triang[ftri].LinesId(lids);
    for (i = 0; i < 3; i++)
      if ((m_node[m_line[lids[i]].getNodeFrom()].getZ() >= m_nodatavalue) ||
        (m_node[m_line[lids[i]].getNodeTo()].getZ() >= m_nodatavalue))
      {
        pf.setZ(m_nodatavalue);
        pn.setZ(m_nodatavalue);
        break;
      }

    // Calculate zvalue of first and last points
    if (i == 3)
    {
      for (k = 0; k < 3; k++)
        if (OnIsolineSegment(lids[k], pf, fixedf))
          // If on isoline segment
          break;

      for (k = 0; k < 3; k++)
        if (OnIsolineSegment(lids[k], pn, fixedn))
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
  m_triang[ftri].LinesId(lids);
  NeighborsId(ftri, nids);
  te::gm::PointZ pt(std::numeric_limits< float >::max(), 0., 0.);
  te::gm::PointZ minpt;
  int auxj = -1, j, minj;
  for (j = 0; j < 3; j++)
  {
    te::gm::PointZ pt1 = m_node[m_line[lids[j]].getNodeFrom()].getNPoint();
    te::gm::PointZ pt2 = m_node[m_line[lids[j]].getNodeTo()].getNPoint();
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
      te::gm::PointZ pt1 = m_node[m_line[lids[j]].getNodeFrom()].getNPoint();
      te::gm::PointZ pt2 = m_node[m_line[lids[j]].getNodeTo()].getNPoint();
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
    if ((m_node[m_line[lids[i]].getNodeFrom()].getZ() >= m_nodatavalue) ||
      (m_node[m_line[lids[i]].getNodeTo()].getZ() >= m_nodatavalue))
    {
      p3t1.setZ(m_nodatavalue);
      p3t2.setZ(m_nodatavalue);
      break;
    }
  // Calculate zvalue of first and intersected points
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
    m_triang[ntri].LinesId(lids);
    NeighborsId(ntri, nids);

    // Search intersecting edge and point in triangle
    pt.setX(m_nodatavalue);
    auxj = -1;
    for (j = 0; j < 3; j++)
    {
      if (lids[j] == ledge)
        continue;
      te::gm::PointZ pt1 = m_node[m_line[lids[j]].getNodeFrom()].getNPoint();
      te::gm::PointZ pt2 = m_node[m_line[lids[j]].getNodeTo()].getNPoint();
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
        te::gm::PointZ pt1 = m_node[m_line[lids[j]].getNodeFrom()].getNPoint();
        te::gm::PointZ pt2 = m_node[m_line[lids[j]].getNodeTo()].getNPoint();
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
        p3t1.setZ(m_node[m_line[lids[j]].getNodeTo()].getZ());
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
        p3t2.setZ(m_node[m_line[lids[j]].getNodeTo()].getZ());
      }
      else
        fixed2 = false;
      p3t2.setX(pt.getX());
      p3t2.setY(pt.getY());
      for (i = 0; i < 3; i++)
        if ((m_node[m_line[lids[i]].getNodeFrom()].getZ() >= m_nodatavalue) ||
          (m_node[m_line[lids[i]].getNodeTo()].getZ() >= m_nodatavalue))
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
  m_triang[ltri].LinesId(lids);
  if (k == 0)
  {
    p3t1.setX(pn.getX());
    p3t1.setY(pn.getY());
    for (i = 0; i < 3; i++)
      if ((m_node[m_line[lids[i]].getNodeFrom()].getZ() >= m_nodatavalue) ||
        (m_node[m_line[lids[i]].getNodeTo()].getZ() >= m_nodatavalue))
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
      if ((m_node[m_line[lids[i]].getNodeFrom()].getZ() >= m_nodatavalue) ||
        (m_node[m_line[lids[i]].getNodeTo()].getZ() >= m_nodatavalue))
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
  te::gm::PointZ pt1 = m_node[m_line[linid].getNodeFrom()].getNPoint();
  te::gm::PointZ pt2 = m_node[m_line[linid].getNodeTo()].getNPoint();

  fixed = false;
  if (onSegment(pt, pt1, pt2, m_minedgesize))
    if (IsIsolineSegment(linid))
    {
      // If on isoline segment
      pt.setZ(m_node[m_line[linid].getNodeFrom()].getZ());
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
  for (i = m_fbnode; i < m_node.size() - 1; i++)
  {
    if ((m_node[i].getType() > Breaklinefirst) ||
      (m_node[i].getType() == Deletednode))
      // If node is last or sample point
      continue;

    node1 = NextNode(i);
    if (node1 == -1) continue;

    if (m_node[node1].getType() == Breaklinelast)
      // If next node is last of a breakline
      continue;

    // Search line from node to node+1
    bline = FindLine(i, node1);
    if (bline == -1)
    {
      if (m_node[i].getType() == Breaklinefirst)
        m_node[i].setType(Sample);
      else
        m_node[i].setType(Breaklinelast);
      m_node[node1].setType(Breaklinefirst);
      continue;
    }
    line1 = bline;

    // Search line from node+1 to node+2
    node2 = NextNode(node1);
    if (node2 == -1) continue;
    nline = FindLine(node1, node2);
    if (nline == -1)
    {
      m_node[node1].setType(Breaklinelast);
      if (m_node[node2].getType() == Breaklinelast)
        m_node[node2].setType(Sample);
      else
        m_node[node2].setType(Breaklinefirst);
      continue;
    }
    line2 = nline;

    // Make sure line pointing to next node
    if (m_line[bline].getNodeTo() == i)
      m_line[bline].SwapNodePolygon();

    tri = m_line[bline].getRightPolygon();
    lline = bline;

    // Search lines at right side
    while ((lline != nline) && (tri != -1))
    {
      m_triang[tri].LinesId(lids);
      for (j = 0; j < 3; j++)
      {
        if (lids[j] == lline)
          continue;
        if (m_line[lids[j]].getNodeTo() == node1)
          break;
        if (m_line[lids[j]].getNodeFrom() == node1)
        {
          // Make line pointing to next node
          m_line[lids[j]].SwapNodePolygon();
          break;
        }
      }
      if (j == 3)
        return false;
      lline = lids[j];
      nrtri = m_line[lline].getRightPolygon();
      if (nrtri == tri)
        m_line[lline].SwapPolygon();
      tri = m_line[lline].getRightPolygon();
    }

    tri = m_line[bline].getLeftPolygon();
    lline = bline;

    // Search lines at left side
    while ((lline != nline) && (tri != -1))
    {
      m_triang[tri].LinesId(lids);
      for (j = 0; j < 3; j++)
      {
        if (lids[j] == lline)
          continue;
        if (m_line[lids[j]].getNodeFrom() == node1)
          break;
        if (m_line[lids[j]].getNodeTo() == node1)
        {
          // Make line pointing to opposite node of next node
          m_line[lids[j]].SwapNodePolygon();
          break;
        }
      }
      if (j == 3)
        return false;
      lline = lids[j];
      nrtri = m_line[lline].getRightPolygon();
      if (nrtri == tri)
        m_line[lline].SwapPolygon();
      tri = m_line[lline].getRightPolygon();
    }
  }

  return true;
}

bool te::mnt::TINGeneration::ReCreateDelaunay()
{
  int32_t triangid, contr = 0, npoly = -1;

  for (triangid = 0; triangid < m_ltriang; triangid++)
  {

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
    aux = neighids[j];
    if (aux == nt)
      continue;

    // Test with each neighbor, if it exists
    if (aux == -1)
      continue;
    if (IsNeighborOnIsoOrBreakline(nt, j))
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

      if (m_node[nidaux].getType() == Breaklinenormal)
        m_node[nidaux].setType(Sample);
      else if (m_node[nidaux].getType() == Last)
        m_node[nidaux].setType(Breaklinelast);
      else
        return false;
      continue;
    }
    node = ++m_lnode;
    if (node > m_nodesize)
      return false;
    m_node[node].setNPoint(p3d);
    if (frsflag)
    {
      // If first point of a line, set node type
      frsflag = false;
      m_node[node].setType(Breaklinenormal);
    }
    else
      m_node[node].setType(Last);
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
    if ((m_node[vii].getType() == Box)/* || (m_node[vii].getType() == Deletednode)*/)
    {//if node has Box type
      //Search adjacent edges to the vertex
      std::vector <int32_t> lids;
      NodeLines(vii, lids);

      for (size_t li = 0; li < lids.size(); li++)
      { //Swaps vertex reference in the edges (from and/or to) for NULL
        int32_t lii = lids[li];
        int32_t from = m_line[lii].getNodeFrom();
        int32_t to = m_line[lii].getNodeTo();

        if (from == vii)
        {
          m_line[lii].setNodeFrom(-1);
          m_line[lii].setType(Deletedline);
        }
        if (to == vii)
        {
          m_line[lii].setNodeTo(-1);
          m_line[lii].setType(Deletedline);
        }

        //Swaps edge reference in the triangles for NULL
        long tleft = m_line[lii].getLeftPolygon();
        long tright = m_line[lii].getRightPolygon();

        //Check if triangle edges are equal eii
        int32_t edge[3];
        if (tleft != -1)
        {
          //left triangle
          m_triang[tleft].LinesId(edge);
          if (edge[0] == lii)
            m_triang[tleft].setEdges(-1, edge[1], edge[2]);
          if (edge[1] == lii)
            m_triang[tleft].setEdges(edge[0], -1, edge[2]);
          if (edge[2] == lii)
            m_triang[tleft].setEdges(edge[0], edge[1], -1);

        }
        if (tright != -1)
        { //right triangle
          m_triang[tright].LinesId(edge);
          if (edge[0] == lii)
            m_triang[tright].setEdges(-1, edge[1], edge[2]);
          if (edge[1] == lii)
            m_triang[tright].setEdges(edge[0], -1, edge[2]);
          if (edge[2] == lii)
            m_triang[tright].setEdges(edge[0], edge[1], -1);
        }

        std::vector <int32_t> edgesadj;
       //Checks if vertex from and to have reference to this edge in onEdge
        //If, there is the reference, changes it to another adjacent edge.
        if (from != -1)
        {
          std::vector<int32_t> edge = m_node[from].getEdge();
          for (size_t i_e = 0; i_e < edge.size(); i_e++)
          {
            if (edge[i_e] == lii)//vertex from
            {
              NodeLines(from, edgesadj);
              for (size_t iAdj = 0; iAdj < edgesadj.size(); iAdj++)
              {
                if (edgesadj[iAdj] != -1 && edgesadj[iAdj] != lii)
                {
                  m_node[from].setEdge(edgesadj[iAdj]);
                  break;
                }
              }
            }
          }
        }

        if (to != -1)
        {
          std::vector<int32_t> edge = m_node[to].getEdge();
          for (size_t i_e = 0; i_e < edge.size(); i_e++)
          {
            if (edge[i_e] == lii)//vertex to
            {
              NodeLines(to, edgesadj);
              for (size_t iAdj = 0; iAdj < edgesadj.size(); iAdj++)
              {
                if (edgesadj[iAdj] != -1 && edgesadj[iAdj] != lii)
                {
                  m_node[to].setEdge(edgesadj[iAdj]);
                  break;
                }
              }
            }
          }
        }
      }

//      m_node[vii].setType(Deletednode);
    }
  }

  //Search triangles with NULL reference
  for (int32_t tit = 0; tit < m_triang.size(); ++tit)
  {
    int32_t edge[3];
    m_triang[tit].LinesId(edge);
    if (edge[0] == -1 || edge[1] == -1 || edge[2] == -1)
    {//if triangle has NULL edge
      //For each triangle to be removed:
      //Changes triangle reference in the edges (left and right) to -1
      if (edge[0] != -1)
      {
        int32_t left = m_line[edge[0]].getLeftPolygon();
        int32_t right = m_line[edge[0]].getRightPolygon();

        if (left == tit)
          m_line[edge[0]].setLeftPolygon(-1);
        if (right == tit)
          m_line[edge[0]].setRightPolygon(-1);
      }

      if (edge[1] != -1)
      {
        long left = m_line[edge[1]].getLeftPolygon();
        long right = m_line[edge[1]].getRightPolygon();

        if (left == tit)
          m_line[edge[1]].setLeftPolygon(-1);
        if (right == tit)
          m_line[edge[1]].setRightPolygon(-1);
      }
      if (edge[2] != -1)
      {
        long left = m_line[edge[2]].getLeftPolygon();
        long right = m_line[edge[2]].getRightPolygon();

        if (left == tit)
          m_line[edge[2]].setLeftPolygon(-1);
        if (right == tit)
          m_line[edge[2]].setRightPolygon(-1);
      }
    }
  }

  return true;
}

