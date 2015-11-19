/*!
\file terralib/mnt/core/Tin.cpp

\brief This file contains a class to define a TIN.

*/

#include "Tin.h"
#include "Utils.h"

#include "../../dataaccess/utils/Utils.h"

#include "../../datatype/Property.h"
#include "../../datatype/SimpleProperty.h"
#include "../../datatype/StringProperty.h"

#include "../../geometry/GeometryProperty.h"
#include "../../geometry/Line.h"
#include "../../geometry/MultiPolygon.h"

#include "../../memory/DataSet.h"
#include "../../memory/DataSetItem.h"

#include "../../raster/Grid.h"
#include "../../raster/Utils.h"

#include "../../sam.h"

#include <limits>
#include <math.h>

bool te::mnt::TinLine::operator== (const TinLine &rhs) const
{
  if ((this->m_nodefrom != rhs.m_nodefrom) ||
    (this->m_nodeto != rhs.m_nodeto) ||
    (this->m_leftpoly != rhs.m_leftpoly) ||
    (this->m_rightpoly != rhs.m_rightpoly)
    )
    return false;
  else
    return true;
}

bool te::mnt::TinLine::operator > (const TinLine &rhs) const
{
  if ((this->m_nodefrom <= rhs.m_nodefrom) ||
    (this->m_nodeto <= rhs.m_nodeto) ||
    (this->m_leftpoly <= rhs.m_leftpoly) ||
    (this->m_rightpoly <= rhs.m_rightpoly)
    )
    return false;
  else
    return true;
}

bool te::mnt::TinLine::operator < (const TinLine &rhs) const
{
  if ((this->m_nodefrom >= rhs.m_nodefrom) ||
    (this->m_nodeto >= rhs.m_nodeto) ||
    (this->m_leftpoly >= rhs.m_leftpoly) ||
    (this->m_rightpoly >= rhs.m_rightpoly)
    )
    return false;
  else
    return true;
}

bool te::mnt::TinLine::ExchangePolygon(int32_t oldPolyId, int32_t newPolyId)
{
  if (m_rightpoly == oldPolyId)
    m_rightpoly = newPolyId;
  else if (m_leftpoly == oldPolyId)
    m_leftpoly = newPolyId;
  else
    return false;

  return true;
}

bool te::mnt::TinLine::ExchangeNode(int32_t oldNodeId, int32_t newNodeId)
{
  if (m_nodefrom == oldNodeId)
    m_nodefrom = newNodeId;
  else if (m_nodeto == oldNodeId)
    m_nodeto = newNodeId;
  else
    return false;

  return true;
}

bool te::mnt::TinLine::SwapPolygon()
{
  int32_t aux;

  aux = m_rightpoly;
  m_rightpoly = m_leftpoly;
  m_leftpoly = aux;
  return true;
}

bool te::mnt::TinLine::SwapNode()
{
  int32_t aux;

  aux = m_nodefrom;
  m_nodefrom = m_nodeto;
  m_nodeto = aux;

  return true;
}

bool te::mnt::TinLine::SwapNodePolygon()
{
  SwapNode();
  SwapPolygon();
  return true;
}

bool te::mnt::TinNode::operator== (const TinNode &rhs) const
{
  if ((this->m_point.getX() == rhs.m_point.getX()) &&
    (this->m_point.getY() == rhs.m_point.getY()))
    return true;
  else
    return false;
}

bool te::mnt::TinNode::operator> (const TinNode &rhs) const
{
  if ((this->m_point.getX() > rhs.m_point.getX()) &&
    (this->m_point.getY() > rhs.m_point.getY()))
    return true;
  else
    return false;
}

bool te::mnt::TinNode::operator< (const TinNode &rhs) const
{
  if ((this->m_point.getX() < rhs.m_point.getX()) &&
    (this->m_point.getY() < rhs.m_point.getY()))
    return true;
  else
    return false;
}

bool te::mnt::TinNode::setEdge(int32_t edge) 
{
  if (std::find(m_edge.begin(), m_edge.end(), edge) == m_edge.end()) {
    m_edge.push_back(edge);
    return true;
  }
  return false;
}

void te::mnt::Tin::setSRID(int srid)
{
  m_srid = srid;
}

void te::mnt::Tin::setEnvelope(te::gm::Envelope& env)
{
  if (m_env.getWidth())
    m_env.Union(env);
  else
    m_env = env;
}

int32_t te::mnt::Tin::FindTriangle(te::gm::PointZ &ptr1)
{
  int32_t v = -1;
  int i;

  if (m_lline == 0)
    return -1;

  for (i = (int32_t)m_lline - 1; i >= 0; i--)
  {
    if ((v = m_line[(unsigned int)i].getNodeFrom()) != -1)
      break;
  }
  if (v == -1)
    return -1;

  double tol = m_env.getWidth() / 1000000.;
  double px = ptr1.getX();
  double py = ptr1.getY();

  for (;;)
  {
    //1. Set the segment line r whose endpoints point p and the vertice v
    te::gm::PointZ ptr2 = m_node[(unsigned int)v].getNPoint();

    //2. Set the edge assist aaux as null 
    int32_t aaux = -1;

    //3. Be m be the number of edges opposite to v . Set the set A = { a1, . , Am } the opposite edges to v ,
    std::vector<int32_t> aam;
    if (!NodeOppositeLines(v, aam))
      return -1;

    //4 . Set the vertice assist Vaux as the vertice v ,
    int32_t vaux = (int32_t) v;

    //5. n is the number of neighboring vertices to v. Defines the set V = {v1,...,vn} of all neighboring vertices to v,
    std::vector<int32_t> vvn;
    if (!NodeNodes(v, vvn))
      return -1;

    int32_t vi;
    for (;;)
    {
      bool flag = false;
      //6. For each vertice vi (i Î  {1,  .,n}) of V, do:
      size_t iiv;
      for (iiv = 0; iiv < vvn.size(); iiv++)
      {
        vi = vvn[iiv];
        if ((fabs(px - m_node[(unsigned int)vi].getX()) < tol) && (fabs(py - m_node[(unsigned int)vi].getY()) < tol))
        {
          //6.1. If vi is equla p, the triangle that contains p is one of the triangles that sharing vi.
          // choose eiher of trianglesand finish the algorithm.
          return NodeTriangle(vi);
        }

        //6.2. If vi is not equal vaux and is on r, do:
        if ((vi == vaux) ||
          ((fabs(m_node[(unsigned int)vaux].getX() - m_node[(unsigned int)vi].getX()) < tol) &&
          (fabs(m_node[(unsigned int)vaux].getY() - m_node[(unsigned int)vi].getY()) < tol)))
          continue;

        te::gm::PointZ ptvi = m_node[(unsigned int)vi].getNPoint();

        double dist = pointToSegmentDistance(ptr1, ptr2, ptvi, NULL);

        if (dist < tol)
        {
          //6.2.1. Define v as vi,
          v = vi;
          //6.2.2. Return to 2.;
          flag = true;
          break;
        }
      }
      if (iiv != vvn.size())
        break;

      flag = false;
      //7. For each edge ai (i Î  {1,  .,m}) of A, do:
      size_t iia;
      for (iia = 0; iia < aam.size(); iia++)
      {
        int32_t ai = aam[iia];
        if (ai != aaux)
        {
          //7.1. If ai is different aaux
          te::gm::PointZ lfr = m_node[(unsigned int)m_line[(unsigned int)ai].getNodeFrom()].getNPoint();
          te::gm::PointZ lto = m_node[(unsigned int)m_line[(unsigned int)ai].getNodeTo()].getNPoint();
          if (segIntersect(ptr1, ptr2, lfr, lto))
          {
            //  and intersects r, do:
            int32_t t;
            int32_t taux = m_line[(unsigned int)ai].getLeftPolygon();
            if (taux == -1)
              continue;
              //  7.1.1. If vaux is not NULL, do:
              //   7.1.1.1. Defines the t triangle that sharing ai 
              //    with the  taux triangle that contains the vaux vertex ,
            if (OppositeNode(taux, ai) == vaux)
              t = m_line[(unsigned int)ai].getRightPolygon();
            else
              t = m_line[(unsigned int)ai].getLeftPolygon();
            if (aaux != -1)
            {
              // 7.1.2. Else, aaux is not NULL, do:
              //  7.1.2.1. Defines the t triangle that sharing ai with 
              //    the taux triangle that constins aaux.
              int32_t lidsaux[3];
              t = m_line[(unsigned int)ai].getLeftPolygon();
              if (t == -1)
                return -1;
              if (!m_triang[(unsigned int)t].LinesId(lidsaux))
                return -1;

              for (unsigned int j = 0; j < 3; j++)
              {
                if (lidsaux[j] == aaux)
                {
                  t = m_line[(unsigned int)ai].getRightPolygon();
                  break;
                }
              }
            }
            else
            {
              return -1;
            }

            //7.1.3. If the t traingle contains the p point, finish the algorihm.
            if (ContainsPoint(t, ptr1))
              return t;

            //7.1.4. Redefines the set A={a1, a2}.
            aam.clear();
            int32_t lids[3];
            if (t == -1)
              return -1;
            if (!m_triang[(unsigned int)t].LinesId(lids))
              return -1;

            for (unsigned int j = 0; j < 3; j++)
            {
              // The edges a1 e a2 are different edges of ai of t triangle,
              if (lids[j] != ai)
              {
                if (std::find(aam.begin(), aam.end(), lids[j]) == aam.end())
                  aam.push_back(lids[j]);
              }
            }

            //7.1.5. Redefines the set V={v1}.
            vvn.clear();
            // The v1 vertice is the t triangle vertice that is not in any border of ai,
            int32_t v1 = OppositeNode(t, ai);
            if (v1 > 0)
              if ((fabs(ptr2.getX() - m_node[(unsigned int)v1].getX()) < tol) && (fabs(ptr2.getY() - m_node[(unsigned int)v1].getY()) < tol))
              {
                flag = true;
                break;
              }

            vvn.push_back(v1);

            //7.1.6. Defines aaux as ai,
            aaux = ai;
            //7.1.7. Defines vaux auxiliar vertice as null
            vaux = -1;
            //7.1.8. Returns to 6.;
            flag = true;
            break;
          }
        }
      }
      if (vvn.empty())
      {
        for (i = i - 1; i >= 0; i--)
        {
          if (m_line[(unsigned int)i].getNodeFrom() != -1)
          {
            v = m_line[(unsigned int)i].getNodeFrom();
            break;
          }
          else
            return -1;
        }
        break;
      }
      if (iia != aam.size())
        continue;
      //8. If no more edges in A, then:
      //8.1. Be k the triangles number that sharing v. Defines the set T={t1,   ., tk} of all triangles that sharing v,
      std::vector<int32_t> ttk;
      if (!NodeTriangles(v, ttk))
        return -1;

      //8.2. For each ti triangle (i Î  {1,  .,k}) of T, do:
      for (size_t iit = 0; iit < ttk.size(); iit++)
      {
        int32_t ti = ttk[iit];

        //8.2.1. If ti triangle contains p point, finish the algorithm.
        if (ContainsPoint(ti, ptr1))
          return ti;
      }
      for (i = i - 1; i >= 0; i--)
      {
        if ((v = m_line[(unsigned int)i].getNodeFrom()) != -1)
          break;
      }
      if (v == -1)
        return -1;
      break;
    }
  }
}

bool te::mnt::Tin::TrianglePoints(int32_t triangId, te::gm::PointZ *vertex)
{
  int32_t linesid[3];
  if (triangId == -1)
    return false;

  if (!m_triang[(unsigned int)triangId].LinesId(linesid))
    return false;
  if (m_line[(unsigned int)linesid[0]].getNodeTo() == m_line[(unsigned int)linesid[1]].getNodeTo())
  {
    vertex[0].setX(m_node[(unsigned int)m_line[(unsigned int)linesid[0]].getNodeFrom()].getX());
    vertex[0].setY(m_node[(unsigned int)m_line[(unsigned int)linesid[0]].getNodeFrom()].getY());
    vertex[0].setZ(m_node[(unsigned int)m_line[(unsigned int)linesid[0]].getNodeFrom()].getZ());
    vertex[1].setX(m_node[(unsigned int)m_line[(unsigned int)linesid[0]].getNodeTo()].getX());
    vertex[1].setY(m_node[(unsigned int)m_line[(unsigned int)linesid[0]].getNodeTo()].getY());
    vertex[1].setZ(m_node[(unsigned int)m_line[(unsigned int)linesid[0]].getNodeTo()].getZ());
    vertex[2].setX(m_node[(unsigned int)m_line[(unsigned int)linesid[1]].getNodeFrom()].getX());
    vertex[2].setY(m_node[(unsigned int)m_line[(unsigned int)linesid[1]].getNodeFrom()].getY());
    vertex[2].setZ(m_node[(unsigned int)m_line[(unsigned int)linesid[1]].getNodeFrom()].getZ());
  }
  else if (m_line[(unsigned int)linesid[0]].getNodeTo() == m_line[(unsigned int)linesid[1]].getNodeFrom())
  {
    vertex[0].setX(m_node[(unsigned int)m_line[(unsigned int)linesid[0]].getNodeFrom()].getX());
    vertex[0].setY(m_node[(unsigned int)m_line[(unsigned int)linesid[0]].getNodeFrom()].getY());
    vertex[0].setZ(m_node[(unsigned int)m_line[(unsigned int)linesid[0]].getNodeFrom()].getZ());
    vertex[1].setX(m_node[(unsigned int)m_line[(unsigned int)linesid[0]].getNodeTo()].getX());
    vertex[1].setY(m_node[(unsigned int)m_line[(unsigned int)linesid[0]].getNodeTo()].getY());
    vertex[1].setZ(m_node[(unsigned int)m_line[(unsigned int)linesid[0]].getNodeTo()].getZ());
    vertex[2].setX(m_node[(unsigned int)m_line[(unsigned int)linesid[1]].getNodeTo()].getX());
    vertex[2].setY(m_node[(unsigned int)m_line[(unsigned int)linesid[1]].getNodeTo()].getY());
    vertex[2].setZ(m_node[(unsigned int)m_line[(unsigned int)linesid[1]].getNodeTo()].getZ());
  }
  else if (m_line[(unsigned int)linesid[0]].getNodeFrom() == m_line[(unsigned int)linesid[1]].getNodeFrom())
  {
    vertex[0].setX(m_node[(unsigned int)m_line[(unsigned int)linesid[0]].getNodeTo()].getX());
    vertex[0].setY(m_node[(unsigned int)m_line[(unsigned int)linesid[0]].getNodeTo()].getY());
    vertex[0].setZ(m_node[(unsigned int)m_line[(unsigned int)linesid[0]].getNodeTo()].getZ());
    vertex[1].setX(m_node[(unsigned int)m_line[(unsigned int)linesid[0]].getNodeFrom()].getX());
    vertex[1].setY(m_node[(unsigned int)m_line[(unsigned int)linesid[0]].getNodeFrom()].getY());
    vertex[1].setZ(m_node[(unsigned int)m_line[(unsigned int)linesid[0]].getNodeFrom()].getZ());
    vertex[2].setX(m_node[(unsigned int)m_line[(unsigned int)linesid[1]].getNodeTo()].getX());
    vertex[2].setY(m_node[(unsigned int)m_line[(unsigned int)linesid[1]].getNodeTo()].getY());
    vertex[2].setZ(m_node[(unsigned int)m_line[(unsigned int)linesid[1]].getNodeTo()].getZ());
  }
  else if (m_line[(unsigned int)linesid[0]].getNodeFrom() == m_line[(unsigned int)linesid[1]].getNodeTo())
  {
    vertex[0].setX(m_node[(unsigned int)m_line[(unsigned int)linesid[0]].getNodeTo()].getX());
    vertex[0].setY(m_node[(unsigned int)m_line[(unsigned int)linesid[0]].getNodeTo()].getY());
    vertex[0].setZ(m_node[(unsigned int)m_line[(unsigned int)linesid[0]].getNodeTo()].getZ());
    vertex[1].setX(m_node[(unsigned int)m_line[(unsigned int)linesid[0]].getNodeFrom()].getX());
    vertex[1].setY(m_node[(unsigned int)m_line[(unsigned int)linesid[0]].getNodeFrom()].getY());
    vertex[1].setZ(m_node[(unsigned int)m_line[(unsigned int)linesid[0]].getNodeFrom()].getZ());
    vertex[2].setX(m_node[(unsigned int)m_line[(unsigned int)linesid[1]].getNodeFrom()].getX());
    vertex[2].setY(m_node[(unsigned int)m_line[(unsigned int)linesid[1]].getNodeFrom()].getY());
    vertex[2].setZ(m_node[(unsigned int)m_line[(unsigned int)linesid[1]].getNodeFrom()].getZ());
  }
  else
    return false;
  return true;
}

bool te::mnt::Tin::ContainsPoint(int32_t triangId, te::gm::PointZ &pt)
{
  double  totalArea, triangleArea;
  te::gm::PointZ vert[3];

  TrianglePoints(triangId, vert);

  //  Calculate the base triangle area
  triangleArea = fabs(((vert[1].getX() - vert[0].getX()) * (vert[2].getY() - vert[0].getY())) -
    ((vert[2].getX() - vert[0].getX()) * (vert[1].getY() - vert[0].getY())));
  triangleArea *= 1.00001;
  totalArea = fabs(((vert[0].getX() - pt.getX()) * (vert[1].getY() - pt.getY())) -
    ((vert[1].getX() - pt.getX()) * (vert[0].getY() - pt.getY())));
  if (totalArea > triangleArea)
    return false;

  totalArea += fabs(((vert[1].getX() - pt.getX()) * (vert[2].getY() - pt.getY())) -
    ((vert[2].getX() - pt.getX()) * (vert[1].getY() - pt.getY())));
  if (totalArea > triangleArea)
    return false;

  totalArea += fabs(((vert[0].getX() - pt.getX()) * (vert[2].getY() - pt.getY())) -
    ((vert[2].getX() - pt.getX()) * (vert[0].getY() - pt.getY())));
  if (totalArea > triangleArea)
    return false;

  return true;
}

bool te::mnt::Tin::NodeOppositeLines(int32_t v, std::vector<int32_t> &linids)
{
  int32_t td, te, taux, ti;
  int32_t  ai,
    ao,
    lids[3];
  int32_t aaux;
  unsigned short j;
  std::vector<int32_t> a;

  //  Find one line that contains node
  a = FindLine(v);
  if (!a.size())
    return false;

  for (size_t i = 0; i < a.size(); i++)
  {
    if (a[i] == -1) continue;
    // 1. Defines td as triangle that is right of 
    // edge a and te as triangle is left of a,
    td = m_line[(unsigned int)a[i]].getRightPolygon();
    te = m_line[(unsigned int)a[i]].getLeftPolygon();

    // 2. Defines ai as edge a and ti as traingle td,
    ai = a[i];
    ti = td;

    // 3. If the triangle ti is not null, insert the edge ao of ti 
    // that is not directly connected to v in set A, 
    if (ti != -1)
    {
      ao = OppositeEdge(ti, v);
      if (ao == -1)
        //return false;
        continue;
      if (std::find(linids.begin(), linids.end(), ao) == linids.end())
        linids.push_back(ao); // A = lines
      else
        continue;
    }

    // 4. While ti is different of triangle te,
    while (ti != te)
    {
      // 4.1. If the triangle ti is null (is in border triangulation) do:
      if (ti == -1)
      {
        // 4.1.1. Defines ti as triangle te,
        ti = te;
        // 4.1.2. Defines te as null,
        te = -1;
        // 4.1.3. Defines ai as edge a,
        ai = a[i];
        // 4.1.4. If triangle ti is not null, insert edge ao 
        // of ti is not directly connected to v in set A,
        if (ti != -1)
        {
          ao = OppositeEdge(ti, v);
          if (ao == -1)
            //return false;
            break;
          if (std::find(linids.begin(), linids.end(), ao) == linids.end())
            linids.push_back(ao); // A = lines
          else
            break;
        }

        // 4.1.5. Returns to 4.
        continue;
      }

      // 4.2. Defines edge aaux of ti triangle that conects the v vertice and is different of ai 
      if (!m_triang[(unsigned int)ti].LinesId(lids))
        //return false;
        break;

      for (j = 0; j < 3; j++)
      {
        if (lids[j] == ai)
          continue;
        if ((m_line[(unsigned int)lids[j]].getNodeFrom() == v) ||
          (m_line[(unsigned int)lids[j]].getNodeTo() == v))
          break;
      }
      if (j == 3){
        //return false;
        break;
      }

      aaux = lids[j];

      // 4.3. Defines taux as the triangle that sharing the aaux edge with ti 
      if (m_line[(unsigned int)aaux].getRightPolygon() == ti)
        taux = m_line[(unsigned int)aaux].getLeftPolygon();
      else if (m_line[(unsigned int)aaux].getLeftPolygon() == ti)
        taux = m_line[(unsigned int)aaux].getRightPolygon();
      else{
       // return false;
        break;
      }

      // 4.4. Defines ti as taux triangle and ai as aaux edge
      ti = taux;
      ai = aaux;

      // 4.5. If ti triangle isn't NULL, insert ao edge of ti that isn't directly connected to v in A set 
      if (ti != -1)
      {
        ao = OppositeEdge(ti, v);
        if (ao == -1)
          //return false;
          break;
        if (std::find(linids.begin(), linids.end(), ao) == linids.end())
          linids.push_back(ao); // A = lines
        else
          break;
      }

      // 4.6. Returns to 4.
    }
  }
  if (!linids.size())
    return false;

  return true;
}

int32_t te::mnt::Tin::OppositeEdge(int32_t triangId, int32_t nodeId)
{
  int32_t lids[3];
  unsigned short j;

  if (!m_triang[(unsigned int)triangId].LinesId(lids))
    return -1;

  for (j = 0; j < 3; j++)
  {
    if ((m_line[(unsigned int)lids[j]].getNodeFrom() != nodeId) &&
      (m_line[(unsigned int)lids[j]].getNodeTo() != nodeId))
      break;
  }
  if (j == 3)
  {
    return -1;
  }

  return lids[j];
}

std::vector<int32_t> te::mnt::Tin::FindLine(int32_t nid)
{
  std::vector<int32_t> linid = m_node[(unsigned int)nid].getEdge();

  // Test to make sure there is no wrong index
  for (unsigned int i = 0; i < linid.size(); i++)
  {
    if ((m_line[(unsigned int)linid[i]].getNodeTo() != nid) &&
      (m_line[(unsigned int)linid[i]].getNodeFrom() != nid))
      linid.erase(linid.begin()+i);
  }
  if (linid.size())
    return linid;

  static int32_t oldtri = 1;
  int32_t j, k, lids[3];
  unsigned short m;
  unsigned int i;

  if ((oldtri < 0) || (oldtri >= m_ltriang))
    oldtri = 0;
  if (oldtri == 0)
  {
    for (i = 0; i < (unsigned int)m_ltriang; i++)
    {
      if (!m_triang[i].LinesId(lids))
        return linid;
      for (m = 0; m < 3; m++)
      {
        if ((m_line[(unsigned int)lids[m]].getNodeFrom() == nid) ||
          (m_line[(unsigned int)lids[m]].getNodeTo() == nid))
        {
          oldtri = (int32_t)i;
          if (m_node[(unsigned int)nid].setEdge(lids[m]))
            linid.push_back(lids[m]);
        }
      }
    }
    return linid;
  }

  j = oldtri;
  k = oldtri + 1;
  while ((j > 0) || (k < m_ltriang))
  {
    if (j > 0)
    {
      if (!m_triang[(unsigned int)j].LinesId(lids))
        return linid;
      for (m = 0; m < 3; m++)
      {
        if ((m_line[(unsigned int)lids[m]].getNodeFrom() == nid) ||
          (m_line[(unsigned int)lids[m]].getNodeTo() == nid))
        {
          oldtri = j;
          if (m_node[(unsigned int)nid].setEdge(lids[m]))
            linid.push_back(lids[m]);
        }
      }
      j--;
    }
    if (k < m_ltriang)
    {
      if (!m_triang[(unsigned int)k].LinesId(lids))
        return linid;
      for (m = 0; m < 3; m++)
      {
        if ((m_line[(unsigned int)lids[m]].getNodeFrom() == nid) ||
          (m_line[(unsigned int)lids[m]].getNodeTo() == nid))
        {
          oldtri = k;
          if (m_node[(unsigned int)nid].setEdge(lids[m]))
            linid.push_back(lids[m]);
        }
      }
      k++;
    }
  }
  oldtri = 1;
  return linid;

}

int32_t te::mnt::Tin::FindLine(int32_t fnid, int32_t snid)
{
  static int32_t oldtri = 1;
  int32_t i, j, k, lids[3];
  short m;

  if ((oldtri < 0) || (oldtri >= m_ltriang))
    oldtri = 0;
  if (oldtri == 0)
  {
    for (i = 0; i < m_ltriang; i++)
    {
      if (!m_triang[(unsigned int)i].LinesId(lids))
        return -1;

      for (m = 0; m < 3; m++)
      {
        if ((m_line[(unsigned int)lids[m]].getNodeFrom() == fnid) &&
          (m_line[(unsigned int)lids[m]].getNodeTo() == snid))
        {
          oldtri = i;
          return lids[m];
        }
        if ((m_line[(unsigned int)lids[m]].getNodeTo() == fnid) &&
          (m_line[(unsigned int)lids[m]].getNodeFrom() == snid))
        {
          oldtri = i;
          return lids[m];
        }
      }
    }
    return -1;
  }
  j = oldtri;
  k = oldtri + 1;
  while ((j > 0) || (k < m_ltriang))
  {
    if (j > 0)
    {
      if (!m_triang[(unsigned int)j].LinesId(lids))
        return -1;

      for (m = 0; m < 3; m++)
      {
        if ((m_line[(unsigned int)lids[m]].getNodeFrom() == fnid) &&
          (m_line[(unsigned int)lids[m]].getNodeTo() == snid))
        {
          oldtri = j;
          return lids[m];
        }
        if ((m_line[(unsigned int)lids[m]].getNodeTo() == fnid) &&
          (m_line[(unsigned int)lids[m]].getNodeFrom() == snid))
        {
          oldtri = j;
          return lids[m];
        }
      }
      j--;
    }
    if (k < m_ltriang)
    {
      if (!m_triang[(unsigned int)k].LinesId(lids))
        return -1;

      for (m = 0; m < 3; m++)
      {
        if ((m_line[(unsigned int)lids[m]].getNodeFrom() == fnid) &&
          (m_line[(unsigned int)lids[m]].getNodeTo() == snid))
        {
          oldtri = k;
          return lids[m];
        }
        if ((m_line[(unsigned int)lids[m]].getNodeTo() == fnid) &&
          (m_line[(unsigned int)lids[m]].getNodeFrom() == snid))
        {
          oldtri = k;
          return lids[m];
        }
      }
      k++;
    }
  }
  oldtri = 1;
  return -1;
}

bool te::mnt::Tin::NodeLines(int32_t v, std::vector<int32_t> &linids)
{
  int32_t  td, te, ti, taux = 0;
  int32_t ai,
    lids[3];
  int32_t aaux;
  unsigned short j;
  std::vector<int32_t> a;

  // Find one line that contains node
  a = FindLine(v);
  if (!a.size())
    return false;

  for (size_t i = 0; i < a.size(); i++)
  {
    // 1. Defines td as the triangle that is on the right edge a 
    // and te as the triangle that is on the left of a,
    td = m_line[(unsigned int)a[i]].getRightPolygon();
    te = m_line[(unsigned int)a[i]].getLeftPolygon();

    // 2. Defines ai as edge a and ti as triangle td,
    ai = a[i];
    ti = td;

    // 3. Inserts edge ai in set A,
    if (std::find(linids.begin(), linids.end(), ai) == linids.end())
      linids.push_back(ai); // A = linids
    else
      continue;

    // 4. while ti is different of triangle te,
    while (ti != te)
    {
      //    4.1. If triangle ti is null (is on border of triangulation) do:
      if (ti == -1)
      {
        //      4.1.1. Defines ti as triangle te,
        ti = te;
        //      4.1.2. Defines te as null,
        te = -1;
        //      4.1.3. Defines ai as edge a,
        ai = a[i];
        //      4.1.4. Returns to 4.
        continue;
      }

      //    4.2. Defines edge aaux of traingle ti that conects 
      //    vertice v and is different of ai,
      if (!m_triang[(unsigned int)ti].LinesId(lids))
        break;
      for (j = 0; j < 3; j++)
      {
        if (lids[j] == ai)
          continue;
        if ((m_line[(unsigned int)lids[j]].getNodeFrom() == v) ||
          (m_line[(unsigned int)lids[j]].getNodeTo() == v))
          break;
      }
      if (j == 3){
        break;
      }

      aaux = lids[j];

      //    4.3. Defines taux as triangle that sharing edge aaux with ti,
      if (m_line[(unsigned int)aaux].getRightPolygon() == ti)
        taux = m_line[(unsigned int)aaux].getLeftPolygon();
      else if (m_line[(unsigned int)aaux].getLeftPolygon() == ti)
        taux = m_line[(unsigned int)aaux].getRightPolygon();
      else{
        break;
      }

      //    4.4. Defines ti as triangle taux and ai as edge aaux,
      ti = taux;
      ai = aaux;

      //    4.5. 3. Inserta edge ai in set A,
      if (std::find(linids.begin(), linids.end(), ai) == linids.end())
        linids.push_back(ai); // A = linids
      else
        break;

      //  4.6. Returns to 4.
    }
  }
  if (!linids.size())
    return false;

  return true;
}

int32_t te::mnt::Tin::NodeId(int32_t triangId, short vertex)
{
  int32_t nodeids[3];
  if (!NodesId(triangId, nodeids))
    return -1;
  if (vertex == -1)
    return -1;
  return nodeids[(unsigned)vertex];
}

bool te::mnt::Tin::NodesId(int32_t triangId, int32_t *nodeIds)
{
  if (triangId == -1)
    return false;
  int32_t linesid[3];

  if (!m_triang[(unsigned int)triangId].LinesId(linesid))
    return false;
  if (m_line[(unsigned int)linesid[0]].getNodeTo() == m_line[(unsigned int)linesid[1]].getNodeTo())
  {
    nodeIds[0] = m_line[(unsigned int)linesid[0]].getNodeFrom();
    nodeIds[1] = m_line[(unsigned int)linesid[0]].getNodeTo();
    nodeIds[2] = m_line[(unsigned int)linesid[1]].getNodeFrom();
  }
  else if (m_line[(unsigned int)linesid[0]].getNodeTo() == m_line[(unsigned int)linesid[1]].getNodeFrom())
  {
    nodeIds[0] = m_line[(unsigned int)linesid[0]].getNodeFrom();
    nodeIds[1] = m_line[(unsigned int)linesid[0]].getNodeTo();
    nodeIds[2] = m_line[(unsigned int)linesid[1]].getNodeTo();
  }
  else if (m_line[(unsigned int)linesid[0]].getNodeFrom() == m_line[(unsigned int)linesid[1]].getNodeFrom())
  {
    nodeIds[0] = m_line[(unsigned int)linesid[0]].getNodeTo();
    nodeIds[1] = m_line[(unsigned int)linesid[0]].getNodeFrom();
    nodeIds[2] = m_line[(unsigned int)linesid[1]].getNodeTo();
  }
  else if (m_line[(unsigned int)linesid[0]].getNodeFrom() == m_line[(unsigned int)linesid[1]].getNodeTo())
  {
    nodeIds[0] = m_line[(unsigned int)linesid[0]].getNodeTo();
    nodeIds[1] = m_line[(unsigned int)linesid[0]].getNodeFrom();
    nodeIds[2] = m_line[(unsigned int)linesid[1]].getNodeFrom();
  }
  else
    return false;
  return true;
}

bool te::mnt::Tin::NeighborsId(int32_t triangId, int32_t *neighsId)
{
  if (triangId == -1)
    return false;

  int32_t  linesid[3];
  unsigned short  i;

  if (!m_triang[(unsigned int)triangId].LinesId(linesid))
    return false;
  for (i = 0; i < 3; i++)
  {
    if (linesid[i] < 0 || linesid[i] > (int32_t)m_linesize)
      return false;
    if (m_line[(unsigned int)linesid[i]].getLeftPolygon() == triangId)
      neighsId[i] = m_line[(unsigned int)linesid[i]].getRightPolygon();
    else if (m_line[(unsigned int)linesid[i]].getRightPolygon() == triangId)
      neighsId[i] = m_line[(unsigned int)linesid[i]].getLeftPolygon();
    else
      return false;
  }

  return true;
}

int32_t te::mnt::Tin::OppositeNode(int32_t triangId, int32_t linId)
{
  int32_t  nodtoid, nodfrid;
  int32_t  nodeids[3];

  nodtoid = m_line[(unsigned int)linId].getNodeTo();
  nodfrid = m_line[(unsigned int)linId].getNodeFrom();
  if (!NodesId(triangId, nodeids))
    return -1;
  unsigned short i;
  for (i = 0; i < 3; i++)
  {
    if ((nodtoid == nodeids[i]) ||
      (nodfrid == nodeids[i]))
      continue;
    break;
  }
  if (i == 3){
    return -1;
  }

  return nodeids[i];
}

int32_t te::mnt::Tin::NextNode(int32_t nodeId)
{
  if (nodeId == -1)
    return -1;
  int32_t  j;

  if (m_node[(unsigned int)nodeId + 1].getType() != Deletednode)
    return nodeId + 1;

  //  Search next non-deleted point
  for (j = 2; nodeId + j < m_lnode; j++)
    if (m_node[(unsigned int)nodeId + j].getType() != Deletednode)
      break;
  if (nodeId + j > m_lnode)
    return -1;

  return nodeId + j;
}

int32_t te::mnt::Tin::PreviousNode(int32_t nodeId)
{
  if (nodeId <= 0)
    return -1;

  int32_t  j;

  if (m_node[(unsigned int)nodeId - 1].getType() != Deletednode)
    return nodeId - 1;

  //  Search next non-deleted point
  for (j = 2; nodeId - j >= 0; j++)
    if (m_node[(unsigned int)nodeId - j].getType() != Deletednode)
      break;
  if (nodeId - j < 0)
    return -1;

  return nodeId - j;
}

bool te::mnt::Tin::NodeNodes(int32_t v, std::vector<int32_t>& nodids)
{
  int32_t td, te, ti,
    nodeid, taux = 0;
  int32_t ai,
    lids[3];
  int32_t aaux;
  unsigned short  j;
  std::vector<int32_t> a;

  //  Find one line that contains node
  a = FindLine(v);
  if (!a.size())
    return false;

  for (size_t i = 0; i < a.size(); i++)
  {
    // 1. Defines td as triangle that is right of 
    // edge a and te as triangle is left of a,
    td = m_line[(unsigned int)a[i]].getRightPolygon();
    te = m_line[(unsigned int)a[i]].getLeftPolygon();

    // 2. Defines ai as edge a and ti as traingle td,
    ai = a[i];
    ti = td;

    //  3. Inserts the vertice is different of v conected to edge ai
    //    in set V,
    nodeid = m_line[(unsigned int)ai].getNodeFrom();
    if (nodeid > 0)
    {
      if (nodeid == v)
        nodeid = m_line[(unsigned int)ai].getNodeTo();
      if (std::find(nodids.begin(), nodids.end(), nodeid) == nodids.end())
        nodids.push_back(nodeid); // V = nodids
      else
        continue;
    }

    //  4. While ti is different of triangle te,
    while (ti != te)
    {
      //    4.1. If the triangle ti is null (is in border triangulation) do:
      if (ti == -1)
      {
        //      4.1.1. Defines ti as triangle te,
        ti = te;
        //      4.1.2.Defines te as null,
        te = -1;
        //      4.1.3. Defines ai as edge a,
        ai = a[i];
        //      4.1.4. Retorne a 4.
        break;
      }

      //    4.2. Defines edge aaux of triangle ti that connects 
      //   to vertice v and is different of ai,
      m_triang[(unsigned int)ti].LinesId(lids);
      for (j = 0; j < 3; j++)
      {
        if (lids[j] == ai)
          continue;
        if ((m_line[(unsigned int)lids[j]].getNodeFrom() == v) ||
          (m_line[(unsigned int)lids[j]].getNodeTo() == v))
          break;
      }
      if (j == 3){
        break;
      }

      aaux = lids[j];

      //    4.3. Defines taux as triangle that sharing edge a with ti
      if (m_line[(unsigned int)aaux].getRightPolygon() == ti)
        taux = m_line[(unsigned int)aaux].getLeftPolygon();
      else if (m_line[(unsigned int)aaux].getLeftPolygon() == ti)
        taux = m_line[(unsigned int)aaux].getRightPolygon();
      else{
       break;
      }

      //    4.4. Defines ti as triangle taux and ai as edge aaux,
      ti = taux;
      ai = aaux;

      //    4.5. Inserts the vertice is different of v  connected to edge ai in set V,
      nodeid = m_line[(unsigned int)ai].getNodeFrom();
      if (nodeid > 0)
      {
        if (nodeid == v)
          nodeid = m_line[(unsigned int)ai].getNodeTo();
        if (std::find(nodids.begin(), nodids.end(), nodeid) == nodids.end())
          nodids.push_back(nodeid);
        else
          break;
      }
      //  4.6. Returns to 4.
    }
  }
  return true;
}

int32_t te::mnt::Tin::NodeTriangle(int32_t v)
{
  //  Find one line that contains node
  std::vector<int32_t> a = FindLine(v);
  if (!a.size())
    return -1;
  for (size_t i = 0; i < a.size(); i++)
  {
    if (a[i] == -1)
      continue;
    int32_t td = m_line[(unsigned int)a[i]].getRightPolygon();
    if (td == -1)
      return m_line[(unsigned int)a[i]].getLeftPolygon();
    else
      return td;
  }
  return -1;
}

bool te::mnt::Tin::NodeTriangles(int32_t v, std::vector<int32_t> &triangles)
{
  int32_t  td, te, ai, ti, taux = 0,
    lids[3];
  int32_t aaux;
  short  j;
  std::vector<int32_t> a;
  //  Find one line that contains node
  a = FindLine(v);
  if (!a.size())
    return false;

  for (size_t i = 0; i < a.size(); i++)
  {
    if (a[i] == -1)
      continue;
    // 1. Defines td as triangle that is right of 
    //  edge a and te as troiangle that is left of a,
    td = m_line[(unsigned int)a[i]].getRightPolygon();
    te = m_line[(unsigned int)a[i]].getLeftPolygon();

    // 2. Defines ai as a edge and ti as td triangle,
    ai = a[i];
    ti = td;

    // 3. Insert triangle ti in set C if it is not null,
    if (ti != -1)
    {
      if (std::find(triangles.begin(), triangles.end(), ti) == triangles.end())
        triangles.push_back(ti); // C = triangles
      else
        continue;
    }

    // 4. while ti is different of triangle te,
    while (ti != te)
    {
      //    4.1. If triangle ti is null (is on border) do:
      if (ti == -1)
      {
        //      4.1.1. Defines ti as te,
        ti = te;
        //      4.1.2. Defines te as null,
        te = -1;
        //      4.1.3. Defines ai as edge a,
        ai = a[i];
        //      4.1.4. Insert the trinagle ti in set C if 
        //      it is not null,
        if (ti != -1)
        {
          if (std::find(triangles.begin(), triangles.end(), ti) == triangles.end())
            triangles.push_back(ti); // C = triangles
          else
            break;
        }

        //      4.1.5. returns to 4.
        continue;
      }

      // 4.2. Defines the edge aaux of triangle ti that conects the vertice v 
      //    and is different of ai,
      m_triang[(unsigned int)ti].LinesId(lids);
      for (j = 0; j < 3; j++)
      {
        if (lids[j] == ai)
          continue;
        if ((m_line[(unsigned int)lids[j]].getNodeFrom() == v) ||
          (m_line[(unsigned int)lids[j]].getNodeTo() == v))
          break;
      }
      if (j == 3){
        break;
      }

      aaux = lids[j];

      // 4.3. Defines taux as triangle that sharing the 
      //    edge aaux with ti,
      if (m_line[(unsigned int)aaux].getRightPolygon() == ti)
        taux = m_line[(unsigned int)aaux].getLeftPolygon();
      else if (m_line[(unsigned int)aaux].getLeftPolygon() == ti)
        taux = m_line[(unsigned int)aaux].getRightPolygon();
      else{
        break;
      }

      // 4.4. Defines ti as triangle taux and ai as
      //    edge aaux,
      ti = taux;
      ai = aaux;

      // 4.5. Inserts the triangle ti in set C if it is not null,
      if (ti != -1)
      {
        if (std::find(triangles.begin(), triangles.end(), ti) == triangles.end())
          triangles.push_back(ti); // C = triangles
        else
          break;
      }

      // 4.6. Returns to 4.
    }
  }

  return true;
}

bool te::mnt::Tin::ReallocateVectors(size_t nSize)
{
  if (m_nodesize < nSize)
  {
    while (m_nodesize < nSize)
    {
      m_node.push_back(TinNode());
      m_nodesize = m_node.size();
    }

    m_triangsize = 2 * (m_nodesize)-5;  // ntri = 2n-5

    while (m_triang.size() < m_triangsize)
    {
      m_triang.push_back(TinTriang());
    }

    m_linesize = 3 * m_nodesize;  // nlin = (n-1)*3

    while (m_line.size() < m_linesize)
    {
      m_line.push_back(TinLine());
    }
  }

  return true;
}


te::da::DataSetType* te::mnt::Tin::GetDataSetType(std::string &outDsetName)
{
  te::da::DataSetType* dsType = new te::da::DataSetType(outDsetName);

  //Primary key
  te::dt::SimpleProperty* pkProperty = new te::dt::SimpleProperty("tri_id", te::dt::INT32_TYPE);
  pkProperty->setAutoNumber(true);
  dsType->add(pkProperty);

  te::da::PrimaryKey* pk = new te::da::PrimaryKey(outDsetName + "_pk", dsType);
  pk->add(pkProperty);
  dsType->setPrimaryKey(pk);

  te::dt::SimpleProperty* prop1 = new te::dt::SimpleProperty("val1", te::dt::DOUBLE_TYPE);
  dsType->add(prop1);
  te::dt::SimpleProperty* prop2 = new te::dt::SimpleProperty("val2", te::dt::DOUBLE_TYPE);
  dsType->add(prop2);
  te::dt::SimpleProperty* prop3 = new te::dt::SimpleProperty("val3", te::dt::DOUBLE_TYPE);
  dsType->add(prop3);
  te::dt::SimpleProperty* prop4 = new te::dt::SimpleProperty("type1", te::dt::INT32_TYPE);
  dsType->add(prop4);
  te::dt::SimpleProperty* prop5 = new te::dt::SimpleProperty("type2", te::dt::INT32_TYPE);
  dsType->add(prop5);
  te::dt::SimpleProperty* prop6 = new te::dt::SimpleProperty("type3", te::dt::INT32_TYPE);
  dsType->add(prop6);
  te::dt::SimpleProperty* prop7 = new te::dt::SimpleProperty("right1", te::dt::INT32_TYPE);
  dsType->add(prop7);
  te::dt::SimpleProperty* prop8 = new te::dt::SimpleProperty("left1", te::dt::INT32_TYPE);
  dsType->add(prop8);
  te::dt::SimpleProperty* prop9 = new te::dt::SimpleProperty("right2", te::dt::INT32_TYPE);
  dsType->add(prop9);
  te::dt::SimpleProperty* prop10 = new te::dt::SimpleProperty("left2", te::dt::INT32_TYPE);
  dsType->add(prop10);
  te::dt::SimpleProperty* prop11 = new te::dt::SimpleProperty("right3", te::dt::INT32_TYPE);
  dsType->add(prop11);
  te::dt::SimpleProperty* prop12 = new te::dt::SimpleProperty("left3", te::dt::INT32_TYPE);
  dsType->add(prop12);
  te::gm::GeometryProperty* geometry = new te::gm::GeometryProperty("geometry", 0, te::gm::TINZType, true);
  geometry->setSRID(m_srid);
  dsType->add(geometry);

  return dsType;
}


bool te::mnt::Tin::SaveTin(te::da::DataSourcePtr &outDsrc, std::string &outDsetName)
{
  std::auto_ptr<te::da::DataSetType> outDSType(GetDataSetType(outDsetName));
  std::auto_ptr<te::mem::DataSet> outDSet(new te::mem::DataSet(outDSType.get()));

  te::gm::PointZ vert[3];
  int32_t tEdges[3];
  int32_t nids[3];
  int32_t left[3];
  int32_t right[3];
  Ntype type[3];
  double value[3];

  for (unsigned int tri = 0; tri < m_triangsize; tri++)
  {
    te::mem::DataSetItem* dataSetItem = new te::mem::DataSetItem(outDSet.get());
    if (!m_triang[tri].LinesId(tEdges))
      continue;
    TrianglePoints((int32_t)tri, vert);
    if (!NodesId((int32_t)tri, nids))
      continue;
    for (size_t e = 0; e < 3; e++)
    {
      value[e] = vert[e].getZ();
      left[e] = m_line[(unsigned int)tEdges[e]].getLeftPolygon();
      right[e] = m_line[(unsigned int)tEdges[e]].getRightPolygon();
      type[e] = m_node[(unsigned int)nids[e]].getType();
    }

    std::auto_ptr<te::gm::Polygon> p(new te::gm::Polygon(0, te::gm::PolygonZType));
    std::auto_ptr<te::gm::LinearRing> s(new te::gm::LinearRing(4, te::gm::LineStringZType));
    s->setPointN(0, vert[0]);
    s->setPointN(1, vert[1]);
    s->setPointN(2, vert[2]);
    s->setPointN(3, vert[0]);
    p->push_back(s.get());

    dataSetItem->setInt32(0, (int32_t)tri);
    if (value[0] != m_nodatavalue)
      dataSetItem->setDouble("val1", value[0]);
    if (value[1] != m_nodatavalue)
      dataSetItem->setDouble("val2", value[1]);
    if (value[2] != m_nodatavalue)
      dataSetItem->setDouble("val3", value[2]);
    dataSetItem->setInt32("type1", type[0]);
    dataSetItem->setInt32("type2", type[1]);
    dataSetItem->setInt32("type3", type[2]);
    dataSetItem->setInt32("right1", right[0]);
    dataSetItem->setInt32("right2", right[1]);
    dataSetItem->setInt32("right3", right[2]);
    dataSetItem->setInt32("left1", left[0]);
    dataSetItem->setInt32("left2", left[1]);
    dataSetItem->setInt32("left3", left[2]);
    dataSetItem->setGeometry("geometry", (te::gm::Geometry*)p->clone());

    outDSet->add(dataSetItem);
    p.release();

  }

  Save(outDsrc.get(), outDSet.get(), outDSType.get());

  outDSet.release();
  outDSType.release();

  return true;
}

bool fncomp(te::mnt::TinNode lhs, te::mnt::TinNode rhs) { return lhs<rhs; }

struct nodecomp {
  bool operator() (const te::mnt::TinNode& lhs, const te::mnt::TinNode& rhs) const
  {
    return lhs<rhs;
  }
};


/*!
\brief Method used to load a triangular network (TIN)
\return TRUE if the TIN is loaded with no errors or FALSE otherwise
*/
bool te::mnt::Tin::LoadTin(te::da::DataSourcePtr &inDsrc, std::string &inDsetName, double zmin, double zmax)
{
  double val[3];
  te::mnt::Ntype type[3];
  int32_t right[3];
  int32_t left[3];
  int32_t id;
  int32_t no[3];
  int32_t lid[3];

  std::auto_ptr<te::da::DataSet> inDset = inDsrc->getDataSet(inDsetName);
  std::string geo_attr("tri_id");
  const std::size_t np = inDset->getNumProperties();
  m_triangsize = inDset->size();

  // Open tin nodes file for nodes data load
  m_fbnode = 0;

  std::vector<std::string>pnames;
  std::vector<int> ptypes;
  for (std::size_t i = 0; i != np; ++i)
  {
    pnames.push_back(inDset->getPropertyName(i));
    ptypes.push_back(inDset->getPropertyDataType(i));
  }

  std::size_t geo_pos = te::da::GetFirstPropertyPos(inDset.get(), te::dt::GEOMETRY_TYPE);

  inDset->moveBeforeFirst();

  bool first = true;

  typedef te::sam::kdtree::Node<te::gm::Coord2D, int32_t, int32_t> KD_NODE;
  typedef te::sam::kdtree::Index<KD_NODE> KD_TREE;
  te::gm::Envelope e;
  KD_TREE nodetree(e);
  std::vector<KD_NODE*> reportsnode;
  te::sam::rtree::Index<std::size_t> linetree;
  std::vector<std::size_t> reportline;

  while (inDset->moveNext())
  {
    id = inDset->getInt32(1);
    if (inDset->isNull("val1"))
      val[0] = m_nodatavalue;
    else
      val[0] = inDset->getDouble("val1");
    if (inDset->isNull("val2"))
      val[1] = m_nodatavalue;
    else
      val[1] = inDset->getDouble("val2");
    if (inDset->isNull("val3"))
      val[2] = m_nodatavalue;
    else
      val[2] = inDset->getDouble("val3");
    type[0] = (Ntype)inDset->getInt32("type1");
    type[1] = (Ntype)inDset->getInt32("type2");
    type[2] = (Ntype)inDset->getInt32("type3");
    right[0] = inDset->getInt32("right1");
    right[1] = inDset->getInt32("right2");
    right[2] = inDset->getInt32("right3");
    left[0] = inDset->getInt32("left1");
    left[1] = inDset->getInt32("left2");
    left[2] = inDset->getInt32("left3");

    std::auto_ptr<te::gm::Geometry> gin = inDset->getGeometry(geo_pos);
    te::gm::Polygon *g = dynamic_cast<te::gm::Polygon*>(gin.get()->clone());
    te::gm::Curve* c = g->getRingN(0);
    te::gm::LinearRing* lr = dynamic_cast<te::gm::LinearRing*>(c);
    for (std::size_t j = 0; j < 3; ++j)
    {
      TinNode nd;
      te::gm::Point *p = lr->getPointN(j);
      val[j] = val[j] > zmax || val[j] < zmin || val[j] > std::numeric_limits< float >::max() ? m_nodatavalue : val[j];

      if (val[j] != m_nodatavalue)
      {
        if (val[j] < m_min) 
          m_min = val[j];
        if (val[j] > m_max) 
          m_max = val[j];
      }
      nd.Init(p->getX(), p->getY(), val[j], type[j]);

      te::gm::Envelope ept(*p->getMBR());
      nodetree.search(ept, reportsnode);
      size_t kn = 0;
      for (kn = 0; kn < reportsnode.size(); kn++)
      {
        te::gm::Coord2D ind = reportsnode[kn]->getKey();
        if (ind.getX() == p->getX() && ind.getY() == p->getY())
        {
          no[j] = reportsnode[kn]->getData();
          break;
        }
      }
      if (kn == reportsnode.size())
      {
        no[j] = (int32_t)m_node.size();
        m_node.push_back(nd);
        te::gm::Coord2D coord(p->getX(), p->getY());
        nodetree.insert(coord, no[j]);
      }
      reportsnode.clear();

      if (m_fbnode == 0 && nd.getType() == Breaklinefirst)
        m_fbnode = no[j];
    } //nodes

    if (first)
    {
      m_env = *lr->getMBR();
      first = false;
    }
    else
      m_env.Union(*lr->getMBR());

    TinLine tl[3];
    tl[0] = TinLine((int32_t)no[0], (int32_t)no[1], left[0], right[0], Normalline);
    tl[1] = TinLine((int32_t)no[1], (int32_t)no[2], left[1], right[1], Normalline);
    tl[2] = TinLine((int32_t)no[2], (int32_t)no[0], left[2], right[2], Normalline);
    lid[0] = lid[1] = lid[2] = -1;

    for (unsigned int j = 0; j < 3; j++)
    {
      double x0 = m_node[(unsigned int)tl[j].getNodeFrom()].getX();
      double y0 = m_node[(unsigned int)tl[j].getNodeFrom()].getY();
      double x1 = m_node[(unsigned int)tl[j].getNodeTo()].getX();
      double y1 = m_node[(unsigned int)tl[j].getNodeTo()].getY();
      te::gm::Envelope e(std::min(x0, x1), std::min(y0, y1), std::max(x0, x1), std::max(y0, y1));
      linetree.search(e, reportline);
      size_t kl = 0;
      for (kl = 0; kl < reportline.size(); kl++)
      {
        int32_t nd0 = m_line[reportline[kl]].getNodeFrom();
        int32_t nd1 = m_line[reportline[kl]].getNodeTo();
        if ((nd0 == tl[j].getNodeFrom() && nd1 == tl[j].getNodeTo()) || (nd1 == tl[j].getNodeFrom() && nd0 == tl[j].getNodeTo()))
        {
         // lid[j] = (int32_t)reportline[kl];
          m_node[(unsigned int)tl[j].getNodeFrom()].setEdge((int32_t)reportline[kl]);
          m_node[(unsigned int)tl[j].getNodeTo()].setEdge((int32_t)reportline[kl]);
          break;
        }
      }
      if (kl == reportline.size())
      {
        lid[j] = (int32_t)m_line.size();
        m_node[(unsigned int)tl[j].getNodeFrom()].setEdge((int32_t)m_line.size());
        m_node[(unsigned int)tl[j].getNodeTo()].setEdge((int32_t)m_line.size());
        m_line.push_back(tl[j]);
        linetree.insert(e, m_line.size());
      }
      reportline.clear();
    }

    while (id >= (int32_t)m_triang.size())
      m_triang.push_back(TinTriang());
    m_triang[(unsigned int)id].setEdges(lid[0], lid[1], lid[2]);
  }

  m_nodesize = m_node.size();
  m_lnode = (int32_t)m_nodesize;
  m_linesize = m_line.size();
  m_lline = (int32_t)m_linesize;
  m_triangsize = m_triang.size();
  m_ltriang = (int32_t)m_triangsize;

  return true;
}


bool te::mnt::Tin::NodeDerivatives()
{
  // Calculate first derivatives on triangles
  if (!TriangleFirstDeriv())
    return false;

  //Calculate first derivatives on nodes
  if (!NodeFirstDeriv())
    return false;

  // Calculate second derivatives on triangles
  if (!TriangleSecondDeriv())
    return false;

  // Calculate second derivatives on nodes
  if (!NodeSecondDeriv())
    return false;

  if (m_fbnode > 0)
  {
    // If there are breaklines
    // Calculate first derivatives on breaklines nodes
    if (!BreakNodeFirstDeriv())
      return false;

    // Calculate second derivatives on triangles that touch breaklines
    if (!BreakTriangleSecondDeriv())
      return false;

    // Calculate second derivatives on breaklines nodes
    if (!BreakNodeSecondDeriv())
      return false;
  }

  return true;
}

bool te::mnt::Tin::TriangleFirstDeriv()
{
  te::gm::PointZ p3da[3];
  double nvector[3];
  double m1, m2;
  int32_t nodesid[3];
  short j;
  size_t i;
  double tol = .01;

  // Create and Initialize first derivatives vector
  if (m_tfderiv.size())
  {
    m_tfderiv.clear();
  }

  for (i = 0; i < m_triangsize + 1; i++)
  {
    m_tfderiv.push_back(te::gm::PointZ(m_nodatavalue, 0., 0.));
  }

  for (i = 0; i <(unsigned int)m_ltriang; i++)
  {
    if (!NodesId((int32_t)i, nodesid))
    {
      m_tfderiv[i].setY(m_nodatavalue);
      continue;
    }

    for (j = 0; j < 3; j++)
    {
      p3da[j].setX(m_node[(unsigned int)nodesid[j]].getNPoint().getX());
      p3da[j].setY(m_node[(unsigned int)nodesid[j]].getNPoint().getY());
      p3da[j].setZ(m_node[(unsigned int)nodesid[j]].getZ());
    }

    // Special cases
    if ((p3da[0].getZ() >= m_nodatavalue) || (p3da[1].getZ() >= m_nodatavalue) ||
      (p3da[2].getZ() >= m_nodatavalue))
    {
      // Triangle with DUMMY Value
      m_tfderiv[i].setY(m_nodatavalue);
      continue;
    }

    m1 = m2 = m_nodatavalue;

    if ((p3da[1].getY() - p3da[0].getY()) != 0.0)
      m1 = (p3da[1].getX() - p3da[0].getX()) / (p3da[1].getY() - p3da[0].getY());

    if ((p3da[2].getY() - p3da[0].getY()) != 0.0)
      m2 = (p3da[2].getX() - p3da[0].getX()) / (p3da[2].getY() - p3da[0].getY());

    if (fabs(m1 - m2) < tol)
    {
      // Triangle with DUMMY Value
      m_tfderiv[i].setY(m_nodatavalue);
      continue;
    }

    if ((p3da[0].getZ() == p3da[1].getZ()) && (p3da[0].getZ() == p3da[2].getZ()))
    {
      // Triangle parallel to XY plane
      m_tfderiv[i].setX(0.);
      continue;
    }

    // Calculate vector normal to triangle
    triangleNormalVector(p3da, nvector);
    m_tfderiv[i].setX(-nvector[0] / nvector[2]);
    m_tfderiv[i].setY(-nvector[1] / nvector[2]);
  }

  return true;
}

bool te::mnt::Tin::TriangleSecondDeriv()
{
  te::gm::PointZ p3da[3];
  double nvector[3];
  te::gm::PointZ pt;
  double dxy, dyx;
  double m1, m2;
  double tol = (double).01;
  int32_t nodesid[3];

  // Create and Initialize second derivatives vector
  if (!m_nfderiv.size())
    return false;
  if (m_tsderiv.size())
  {
    m_tsderiv.clear();
  }

  size_t i;
  for (i = 0; i < m_triangsize + 1; i++)
  {
    m_tsderiv.push_back(TinNode());
    m_tsderiv[i].setNPoint(te::gm::PointZ(m_nodatavalue, m_nodatavalue, 0.));
  }

  for (i = 0; i < (unsigned int)m_ltriang; i++)
  {
    if (!NodesId((int32_t)i, nodesid))
    {
      m_tsderiv[i].setZ(m_nodatavalue);
      continue;
    }

    // Special case
    if ((m_nfderiv[(unsigned int)nodesid[0]].getX() >= m_nodatavalue) ||
      (m_nfderiv[(unsigned int)nodesid[1]].getX() >= m_nodatavalue) ||
      (m_nfderiv[(unsigned int)nodesid[2]].getX() >= m_nodatavalue) ||
      (m_node[(unsigned int)nodesid[0]].getZ() >= m_nodatavalue) ||
      (m_node[(unsigned int)nodesid[1]].getZ() >= m_nodatavalue) ||
      (m_node[(unsigned int)nodesid[2]].getZ() >= m_nodatavalue))
    {
      // Triangle with DUMMY Value
      m_tsderiv[i].setZ(m_nodatavalue);
      continue;
    }

    m1 = m2 = m_nodatavalue;

    if ((m_nfderiv[(unsigned int)nodesid[1]].getY() - m_nfderiv[(unsigned int)nodesid[0]].getY()) != 0.0)
      m1 = (m_nfderiv[(unsigned int)nodesid[1]].getX() - m_nfderiv[(unsigned int)nodesid[0]].getX()) /
      (m_nfderiv[(unsigned int)nodesid[1]].getY() - m_nfderiv[(unsigned int)nodesid[0]].getY());

    if ((m_nfderiv[(unsigned int)nodesid[2]].getY() - m_nfderiv[(unsigned int)nodesid[0]].getY()) != 0.0)
      m2 = (m_nfderiv[(unsigned int)nodesid[2]].getX() - m_nfderiv[(unsigned int)nodesid[0]].getX()) /
      (m_nfderiv[(unsigned int)nodesid[2]].getY() - m_nfderiv[(unsigned int)nodesid[0]].getY());

    if (fabs(m1 - m2) < tol)
    {
      // Triangle with DUMMY Value
      m_tsderiv[i].setZ(m_nodatavalue);
      continue;
    }

    //		Calculate using dx
    for (unsigned short j = 0; j < 3; j++)
    {
      p3da[j].setX(m_node[(unsigned int)nodesid[j]].getNPoint().getX());
      p3da[j].setY(m_node[(unsigned int)nodesid[j]].getNPoint().getY());
      p3da[j].setZ(m_nfderiv[(unsigned int)nodesid[j]].getX());
    }

    if ((p3da[0].getZ() == p3da[1].getZ()) && (p3da[0].getZ() == p3da[2].getZ()))
    {
      m_tsderiv[i].setX(0.);
      dxy = 0.;
    }
    else
    {
      triangleNormalVector(p3da, nvector);
      m_tsderiv[i].setX(-nvector[0] / nvector[2]);
      dxy = (-nvector[1] / nvector[2]);
    }

    // Calculate using dy
    for (unsigned int j = 0; j < 3; j++)
      p3da[j].setZ(m_nfderiv[(unsigned int)nodesid[j]].getY());

    if ((p3da[0].getZ() == p3da[1].getZ()) && (p3da[0].getZ() == p3da[2].getZ()))
    {
      m_tsderiv[i].setY(0.);
      dyx = 0.;
    }
    else
    {
      triangleNormalVector(p3da, nvector);
      m_tsderiv[i].setY(-nvector[1] / nvector[2]);
      dyx = (-nvector[0] / nvector[2]);
    }
    m_tsderiv[i].setZ((dxy + dyx) / 2.);
  }

  return true;
}


bool te::mnt::Tin::NodeFirstDeriv()
{
  int32_t clstnids[CLNODES];
  size_t i;

  // Create and Initialize first derivatives vector
  if (m_nfderiv.size())
  {
    m_nfderiv.clear();
  }
  for (i = 0; i < m_nodesize + 1; i++)
  {
    m_nfderiv.push_back(te::gm::PointZ(0., 0., 0.));
  }

  // To each node
  for (i = 0; i < m_node.size(); i++)
  {
    // Special cases
    if (m_node[i].getZ() >= m_nodatavalue)
      continue;
    if (m_node[i].getType() == Deletednode)
      // If deleted node
      continue;

    // Look for closest points of the node
    if (!NodeClosestPoints((int32_t)i, clstnids))
      continue;
    m_nfderiv[i] = CalcNodeFirstDeriv((int32_t)i, clstnids);
  }


  return true;
}

bool te::mnt::Tin::NodeSecondDeriv()
{
  size_t i;
  int32_t clstnids[CLNODES];
  TinNode sderiv;

  if (!m_tsderiv.size())
    return false;
  // Create and Initialize second derivatives vector
  if (m_nsderiv.size())
  {
    m_nsderiv.clear();
  }

  for (i = 0; i < m_nodesize + 1; i++)
  {
    m_nsderiv.push_back(TinNode());
    m_nsderiv[i].Init(0., 0., 0.);
  }

  for (i = 0; i < m_node.size(); i++)
  {
    // Special cases
    if (m_node[i].getZ() >= m_nodatavalue)
      // Node with DUMMY value
      continue;
    if (m_node[i].getType() == Deletednode)
      // Deleted node 
      continue;

    // Look for closest point of the node
    if (!NodeClosestPoints((int32_t)i, clstnids))
      continue;
    sderiv = CalcNodeSecondDeriv((int32_t)i, clstnids);
    m_nsderiv[i].setX(sderiv.getX());
    m_nsderiv[i].setY(sderiv.getY());
    m_nsderiv[i].setZ(sderiv.getZ());
  }

  return true;
}

bool te::mnt::Tin::NodeClosestPoints(int32_t nid, int32_t *clstNids, bool useBrNode)
{
  int32_t lineid;
  size_t j, k;
  double dist, distv[CLNODES];
  int32_t nodeid;

  // Find one line that contains node
  std::vector<int32_t> lineids = FindLine(nid);
  if (!lineids.size())
    return false;

  for (j = 0; j < CLNODES; j++)
  {
    distv[j] = m_nodatavalue;
    clstNids[j] = -1;
  }

  for (size_t i = 0; i < lineids.size(); i++)
  {
    if (lineids[i] == -1)
      continue;

    lineid = lineids[i];

    if (m_line[(unsigned int)lineid].getNodeFrom() == nid)
      nodeid = m_line[(unsigned int)lineid].getNodeTo();
    else if (m_line[(unsigned int)lineid].getNodeTo() == nid)
      nodeid = m_line[(unsigned int)lineid].getNodeFrom();
    else
      continue;

    if ((m_node[(unsigned int)nodeid].getZ() < m_nodatavalue) && ((useBrNode) ||
      ((useBrNode) && (nodeid < m_fbnode))))
    {
      dist = (m_node[(unsigned int)nid].getX() - m_node[(unsigned int)nodeid].getX()) *
        (m_node[(unsigned int)nid].getX() - m_node[(unsigned int)nodeid].getX()) +
        (m_node[(unsigned int)nid].getY() - m_node[(unsigned int)nodeid].getY()) *
        (m_node[(unsigned int)nid].getY() - m_node[(unsigned int)nodeid].getY());
        for (j = 0; j < CLNODES; j++)
        {
          if (dist < distv[j])
          {
            for (k = CLNODES - 1; k > j; k--)
            {
              distv[k] = distv[k - 1];
              clstNids[k] = clstNids[k - 1];
            }
            distv[j] = dist;
            clstNids[j] = nodeid;
            break;
          }
        }
      }
  }

  return true;
}

te::gm::PointZ te::mnt::Tin::CalcNodeFirstDeriv(int32_t nodeId, int32_t clstNodes[CLNODES])
{
  size_t j, k;
  te::gm::PointZ deriv;
  te::gm::PointZ p3da[3];
  double	nvector[3], tnx, tny, tnz;
  double	m1, m2;
  double tol = (double).01;

  p3da[0].setX(m_node[(unsigned int)nodeId].getNPoint().getX());
  p3da[0].setY(m_node[(unsigned int)nodeId].getNPoint().getY());
  p3da[0].setZ(m_node[(unsigned int)nodeId].getZ());

  tnx = 0.;
  tny = 0.;
  tnz = 0.;
  for (j = 0; j < CLNODES; j++)
  {
    if (clstNodes[j] == -1)
      break;
    p3da[1].setX(m_node[(unsigned int)clstNodes[j]].getNPoint().getX());
    p3da[1].setY(m_node[(unsigned int)clstNodes[j]].getNPoint().getY());
    p3da[1].setZ(m_node[(unsigned int)clstNodes[j]].getZ());
    for (k = j + 1; k < CLNODES; k++)
    {
      if (clstNodes[k] == -1)
        break;
      p3da[2].setX(m_node[(unsigned int)clstNodes[k]].getNPoint().getX());
      p3da[2].setY(m_node[(unsigned int)clstNodes[k]].getNPoint().getY());
      p3da[2].setZ(m_node[(unsigned int)clstNodes[k]].getZ());

      // Special cases
      m1 = m2 = m_nodatavalue;

      if ((p3da[1].getY() - p3da[0].getY()) != 0.0)
        m1 = (p3da[1].getX() - p3da[0].getX()) / (p3da[1].getY() - p3da[0].getY());

      if ((p3da[2].getY() - p3da[0].getY()) != 0.0)
        m2 = (p3da[2].getX() - p3da[0].getX()) / (p3da[2].getY() - p3da[0].getY());

      if (fabs(m1 - m2) < tol)
        continue;

      if ((p3da[0].getZ() >= m_nodatavalue) || (p3da[1].getZ() >= m_nodatavalue) ||
        (p3da[2].getZ() >= m_nodatavalue))
        continue;

      if ((p3da[0].getZ() == p3da[1].getZ()) &&
        (p3da[0].getZ() == p3da[2].getZ()))
        continue;

      triangleNormalVector(p3da, nvector);
      tnx += nvector[0];
      tny += nvector[1];
      tnz += nvector[2];
    }

  }
  // Calculate node first derivatives
  if (tnz != 0.)
  {
    deriv.setX(-tnx / tnz);
    deriv.setY(-tny / tnz);
  }
  else
  {
    deriv.setX(0.);
    deriv.setY(0.);
  }
  return deriv;
}

te::mnt::TinNode te::mnt::Tin::CalcNodeSecondDeriv(int32_t nodeId, int32_t clstNIds[CLNODES])
{
  te::gm::PointZ p3da[3];
  double tnxx, tnxy, tnxz, tnyx, tnyy, tnyz,
    nvector[3], m1, m2;
  double tol = .01;
  unsigned int j, k;
  TinNode sderiv;

  p3da[0].setX(m_node[(unsigned int)nodeId].getNPoint().getX());
  p3da[0].setY(m_node[(unsigned int)nodeId].getNPoint().getY());
  p3da[0].setZ(m_nfderiv[(unsigned int)nodeId].getX());

  tnxx = 0.;
  tnxy = 0.;
  tnxz = 0.;
  tnyx = 0.;
  tnyy = 0.;
  tnyz = 0.;

  for (j = 0; j < (unsigned int)CLNODES; j++)
  {
    if (clstNIds[j] == -1)
      break;
    p3da[1].setX(m_node[(unsigned int)clstNIds[j]].getNPoint().getX());
    p3da[1].setY(m_node[(unsigned int)clstNIds[j]].getNPoint().getY());
    p3da[1].setZ(m_nfderiv[(unsigned int)clstNIds[j]].getX());
    for (k = j + 1; k < CLNODES; k++)
    {
      if (clstNIds[k] == -1)
        break;
      p3da[2].setX(m_node[(unsigned int)clstNIds[k]].getNPoint().getX());
      p3da[2].setY(m_node[(unsigned int)clstNIds[k]].getNPoint().getY());
      p3da[2].setZ(m_nfderiv[(unsigned int)clstNIds[k]].getX());

      m1 = m2 = m_nodatavalue;

      if ((p3da[1].getY() - p3da[0].getY()) != 0.0)
        m1 = (p3da[1].getX() - p3da[0].getX()) / (p3da[1].getY() - p3da[0].getY());

      if ((p3da[2].getY() - p3da[0].getY()) != 0.0)
        m2 = (p3da[2].getX() - p3da[0].getX()) / (p3da[2].getY() - p3da[0].getY());

      if (fabs(m1 - m2) < tol)         continue;

      if ((p3da[0].getZ() >= m_nodatavalue) ||
        (p3da[1].getZ() >= m_nodatavalue) ||
        (p3da[2].getZ() >= m_nodatavalue))
        // Triangle with DUMMY Value
        continue;
      if ((p3da[0].getZ() == p3da[1].getZ()) &&
        (p3da[0].getZ() == p3da[2].getZ()))
        // Triangle parallel to XY plane
        continue;

      triangleNormalVector(p3da, nvector);
      tnxx += nvector[0];
      tnxy += nvector[1];
      tnxz += nvector[2];
    }
  }
  p3da[0].setZ(m_nfderiv[(unsigned int)nodeId].getY());
  for (j = 0; j < CLNODES; j++)
  {
    if (clstNIds[j] == -1)
      break;
    p3da[1].setX(m_node[(unsigned int)clstNIds[j]].getNPoint().getX());
    p3da[1].setY(m_node[(unsigned int)clstNIds[j]].getNPoint().getY());
    p3da[1].setZ(m_nfderiv[(unsigned int)clstNIds[j]].getY());
    for (k = j + 1; k < CLNODES; k++)
    {
      if (clstNIds[k] == -1)
        break;
      p3da[2].setX(m_node[(unsigned int)clstNIds[k]].getNPoint().getX());
      p3da[2].setY(m_node[(unsigned int)clstNIds[k]].getNPoint().getY());
      p3da[2].setZ(m_nfderiv[(unsigned int)clstNIds[k]].getY());

      m1 = m2 = m_nodatavalue;

      if ((p3da[1].getY() - p3da[0].getY()) != 0.0)
        m1 = (p3da[1].getX() - p3da[0].getX()) / (p3da[1].getY() - p3da[0].getY());

      if ((p3da[2].getY() - p3da[0].getY()) != 0.0)
        m2 = (p3da[2].getX() - p3da[0].getX()) / (p3da[2].getY() - p3da[0].getY());

      if (fabs(m1 - m2) < tol)
        continue;

      if ((p3da[0].getZ() >= m_nodatavalue) ||
        (p3da[1].getZ() >= m_nodatavalue) ||
        (p3da[2].getZ() >= m_nodatavalue))
        continue;
      if ((p3da[0].getZ() == p3da[1].getZ()) &&
        (p3da[0].getZ() == p3da[2].getZ()))
        continue;

      triangleNormalVector(p3da, nvector);
      tnyx += nvector[0];
      tnyy += nvector[1];
      tnyz += nvector[2];
    }
  }
  //	Calculate node second derivatives
  if (tnxz != 0.)
  {
    sderiv.setX(-tnxx / tnxz);
    tnxy = -tnxy / tnxz;
  }
  else
    tnxy = 0.;
  if (tnyz != 0.)
  {
    sderiv.setY(-tnyy / tnyz);
    tnyx = -tnyx / tnyz;
  }
  else
    tnyx = 0.;

  sderiv.setZ((tnxy + tnyx) / 2.);

  return sderiv;
}


bool ::te::mnt::Tin::BreakNodeFirstDeriv()
{
  size_t i, bnodesize,
    node1, node2;
  int32_t rclstnids[CLNODES], lclstnids[CLNODES];
  double deltax, deltay,
    sintheta, costheta, modxy,
    dzds, dzdt, dzdx, dzdy;
  te::gm::PointZ rderiv, lderiv;

  if (!m_tfderiv.size())
    return false;
  // Create and Initialize first derivatives vector
  bnodesize = (unsigned int)(m_lnode - m_fbnode);
  if (m_nbrfderiv.size())
  {
    m_nbrfderiv.clear();
  }

  for (i = 0; i < bnodesize + 1; i++)
  {
    m_nbrfderiv.push_back(m_nfderiv[i + m_fbnode]);
    m_nblfderiv.push_back(m_nfderiv[i + m_fbnode]);
  }

  // To each break node
  for (i = (unsigned int)m_fbnode; i < m_node.size() - 1; i++)
  {
    // Special cases
    if ((m_node[i].getZ() >= m_nodatavalue) ||
      (m_node[i].getType() == Deletednode) ||
      (m_node[i].getType() > Breaklinenormal))
      continue;

    // Look for triangles that sharing the node
    BreakNodeClosestPoints((int32_t)i, rclstnids, lclstnids);
    rderiv = CalcNodeFirstDeriv((int32_t)i, rclstnids);
    lderiv = CalcNodeFirstDeriv((int32_t)i, lclstnids);

    node1 = (unsigned int)NextNode((int32_t)i);
    node2 = (unsigned int)PreviousNode((int32_t)i);

    deltax = m_node[node1].getX() - m_node[node2].getX();
    deltay = m_node[node1].getY() - m_node[node2].getY();
    modxy = sqrt(deltax*deltax + deltay*deltay);
    costheta = deltax / modxy;
    sintheta = deltay / modxy;

    if (m_nfderiv[i].getX() >= m_nodatavalue)
      dzds = 0.;
    else
      dzds = costheta*m_nfderiv[i].getX() -
      sintheta*m_nfderiv[i].getY();

    if (rderiv.getX() >= m_nodatavalue)
      dzdt = 0.;
    else
      dzdt = sintheta*rderiv.getX() +
      costheta*rderiv.getY();
    dzdx = costheta*dzds + sintheta*dzdt;
    dzdy = -sintheta*dzds + costheta*dzdt;
    m_nbrfderiv[i - m_fbnode].setX(dzdx);
    m_nbrfderiv[i - m_fbnode].setY(dzdy);

    if (lderiv.getX() >= m_nodatavalue)
      dzdt = 0.;
    else
      dzdt = sintheta*lderiv.getX() +
      costheta*lderiv.getY();
    dzdx = costheta*dzds + sintheta*dzdt;
    dzdy = -sintheta*dzds + costheta*dzdt;
    m_nblfderiv[i - m_fbnode].setX(dzdx);
    m_nblfderiv[i - m_fbnode].setY(dzdy);
  }

  return true;
}

bool te::mnt::Tin::BreakTriangleSecondDeriv()
{
  size_t i;
  std::vector<int32_t> rightri, leftri;

  // Create and Initialize second derivatives vector
  if ((!m_nbrfderiv.size()) || (!m_nblfderiv.size()))
    return false;
  if (!m_tsderiv.size())
    return false;

  // To each break node except first and last
  for (i = (unsigned int)m_fbnode; i < m_node.size() - 1; i++)
  {
    if (m_node[i].getZ() >- m_nodatavalue)
      continue;

    // If first or last point of a breakline
    if ((m_node[i].getType() == Breaklinefirst) ||
      (m_node[i].getType() == Breaklinelast) ||
      (m_node[i].getType() == Deletednode))
      continue;

    // Look for triangles that sharing the node
    NodeTriangles((int32_t)i, rightri, leftri);

    // Calculate second derivative of node using right side triangles
    CalcTriangleSecondDeriv(rightri, m_nbrfderiv);

    // Calculate second derivative of node using left side triangles
    CalcTriangleSecondDeriv(leftri, m_nblfderiv);

    rightri.clear();
    leftri.clear();
  }

  return true;
}

bool te::mnt::Tin::NodeTriangles(int32_t nodeid, std::vector<int32_t> &rightri, std::vector<int32_t> &leftri)
{
  int32_t rtri, ltri;
  int32_t lids[3], linid;
  short k;

  lids[0] = lids[1] = lids[2] = -1;

  // Find one line that contains node
  std::vector<int32_t> fline = FindLine(nodeid);
  if (!fline.size())
    return false;

  for (size_t i = 0; i < fline.size(); i++)
  {
    linid = fline[i];

    ltri = m_line[(unsigned int)linid].getLeftPolygon();
    rtri = m_line[(unsigned int)linid].getRightPolygon();
    if (rtri == -1)
    {
      rtri = ltri;
      ltri = -1;
    }

    // While right side triangle different from left side<br>
    while (rtri != ltri)
    {
      if (m_line[(unsigned int)linid].getNodeTo() == nodeid)
      {
        rightri.push_back(m_line[(unsigned int)linid].getRightPolygon());
      }
      else if (m_line[(unsigned int)linid].getNodeFrom() == nodeid)
      {
        leftri.push_back(m_line[(unsigned int)linid].getLeftPolygon());
      }

      // Find line that contains node in the right triangle
      if (rtri >= 0)
       if (!m_triang[(unsigned int)rtri].LinesId(lids))
        break;

      for (k = 0; k < 3; k++)
      {
        if (lids[k] == linid || lids[k] == -1)
          continue;
        if ((m_line[(unsigned int)lids[k]].getNodeFrom() == nodeid) ||
          (m_line[(unsigned int)lids[k]].getNodeTo() == nodeid))
          break;
      }
      if (k == 3){
        break;
      }

      // Make right triangle equal to the triangle at the
      // other side
      linid = lids[k];
      if (m_line[(unsigned int)linid].getRightPolygon() == rtri)
        rtri = m_line[(unsigned int)linid].getLeftPolygon();
      else if (m_line[(unsigned int)linid].getLeftPolygon() == rtri)
        rtri = m_line[(unsigned int)linid].getRightPolygon();
      else
        break;
      if (rtri == -1)
      {
        rtri = ltri;
        ltri = -1;
        linid = fline[i];
      }
    }
    //	Insert left side triangle in triangle list
    if (ltri != -1)
    {
      if (m_line[(unsigned int)linid].getNodeTo() == nodeid)
      {
        rightri.push_back(m_line[(unsigned int)linid].getRightPolygon());
      }
      if (m_line[(unsigned int)linid].getNodeFrom() == nodeid)
      {
        leftri.push_back(m_line[(unsigned int)linid].getLeftPolygon());
      }
    }
  }

  return true;
}

bool te::mnt::Tin::BreakNodeClosestPoints(int32_t nid, int32_t *rClstNids, int32_t *lClstNids)
{
  int32_t ltri, rtri;
  int32_t lids[3],
    flin;
  double dist, rdistv[CLNODES], ldistv[CLNODES];
  int32_t nodeid;

  lids[0] = lids[1] = lids[2] = -1;
  // Find one line that contains node
  std::vector<int32_t>lineid = FindLine(nid);
  if (!lineid.size())
    return false;

  for (size_t i = 0; i < lineid.size(); i++)
  {
    flin = lineid[i];

    for (unsigned int j = 0; j < CLNODES; j++)
    {
      rdistv[j] = m_nodatavalue;
      rClstNids[j] = -1;

      ldistv[j] = m_nodatavalue;
      lClstNids[j] = -1;
    }

    // Find right and left triangle
    rtri = m_line[(unsigned int)lineid[i]].getRightPolygon();
    ltri = m_line[(unsigned int)lineid[i]].getLeftPolygon();
    if (rtri == -1)
    {
      rtri = ltri;
      ltri = -1;
    }
    while (rtri != ltri)
    {
      if (rtri >= 0)
        if (!m_triang[(unsigned int)rtri].LinesId(lids))
          break;

      unsigned int j;
      for (j = 0; j < 3; j++)
      {
        // Find line that contains node
        if (lids[j] == lineid[i] || lids[j] == -1)
          continue;
        if ((m_line[(unsigned int)lids[j]].getNodeFrom() == nid) ||
          (m_line[(unsigned int)lids[j]].getNodeTo() == nid))
          break;
      }
      if (j == 3){
        break;
      }

      lineid[i] = lids[j];
      if (m_line[(unsigned int)lineid[i]].getNodeFrom() == nid)
      {
        nodeid = m_line[(unsigned int)lineid[i]].getNodeTo();
        if (nodeid < m_fbnode)
        { //Node is a breakline node at right side of it
          dist = (m_node[(unsigned int)nid].getX() - m_node[(unsigned int)nodeid].getX()) *
            (m_node[(unsigned int)nid].getX() - m_node[(unsigned int)nodeid].getX()) +
            (m_node[(unsigned int)nid].getY() - m_node[(unsigned int)nodeid].getY()) *
            (m_node[(unsigned int)nid].getY() - m_node[(unsigned int)nodeid].getY());
          for (j = 0; j < CLNODES; j++)
          {
            if (dist < rdistv[j])
            {
              for (unsigned int k = (unsigned int)CLNODES - 1; k > j; k--)
              {
                rdistv[k] = rdistv[k - 1];
                rClstNids[k] = rClstNids[k - 1];
              }
              rdistv[j] = dist;
              rClstNids[j] = nodeid;
              break;
            }
          }
        }
      }
      else
      {
        nodeid = m_line[(unsigned int)lineid[i]].getNodeFrom();
        if (nodeid < m_fbnode)
        { //Node is a breakline node at left side of it
          dist = (m_node[(unsigned int)nid].getX() - m_node[(unsigned int)nodeid].getX()) *
            (m_node[(unsigned int)nid].getX() - m_node[(unsigned int)nodeid].getX()) +
            (m_node[(unsigned int)nid].getY() - m_node[(unsigned int)nodeid].getY()) *
            (m_node[(unsigned int)nid].getY() - m_node[(unsigned int)nodeid].getY());
          for (j = 0; j < CLNODES; j++)
          {
            if (dist < ldistv[j])
            {
              for (unsigned int k = (unsigned int)CLNODES - 1; k > j; k--)
              {
                ldistv[k] = ldistv[k - 1];
                lClstNids[k] = lClstNids[k - 1];
              }
              ldistv[j] = dist;
              lClstNids[j] = nodeid;
              break;
            }
          }
        }
      }

      // Find new right triangle
      if (m_line[(unsigned int)lineid[i]].getRightPolygon() == rtri)
        rtri = m_line[(unsigned int)lineid[i]].getLeftPolygon();
      else if (m_line[(unsigned int)lineid[i]].getLeftPolygon() == rtri)
        rtri = m_line[(unsigned int)lineid[i]].getRightPolygon();
      else
        break;
      if ((rtri == -1) && (ltri != -1))
      {
        rtri = ltri;
        ltri = -1;
        lineid[i] = flin;
      }
    }
  }
  return true;
}

bool te::mnt::Tin::CalcTriangleSecondDeriv(std::vector<int32_t> &triangles, std::vector<te::gm::PointZ> &fderiv)
{
  int32_t triid, nodesid[3];
  te::gm::PointZ p3da[3];
  double nvector[3];
  double dxy, dyx;
  double m1, m2;
  double tol = .01;

  for (size_t i = 0; i < triangles.size(); i++)
  {
    if (triangles[i] == -1)
      continue;
    triid = triangles[i];
    if (!NodesId(triid, nodesid))
      continue;

    // Special case
    if ((m_node[(unsigned int)nodesid[0]].getZ() >= m_nodatavalue) ||
      (m_node[(unsigned int)nodesid[1]].getZ() >= m_nodatavalue) ||
      (m_node[(unsigned int)nodesid[2]].getZ() >= m_nodatavalue))
    {
      //			Triangle with DUMMY Value
      m_tsderiv[(unsigned int)triid].setX(m_nodatavalue);
      m_tsderiv[(unsigned int)triid].setY(m_nodatavalue);
      m_tsderiv[(unsigned int)triid].setZ(m_nodatavalue);
      continue;
    }
    m1 = m2 = m_nodatavalue;
    if ((m_node[(unsigned int)nodesid[1]].getY() - m_node[(unsigned int)nodesid[0]].getY()) != 0.0)
      m1 = (m_node[(unsigned int)nodesid[1]].getX() - m_node[(unsigned int)nodesid[0]].getX()) /
      (m_node[(unsigned int)nodesid[1]].getY() - m_node[(unsigned int)nodesid[0]].getY());
    if ((m_node[(unsigned int)nodesid[2]].getY() - m_node[(unsigned int)nodesid[0]].getY()) != 0.0)
      m2 = (m_node[(unsigned int)nodesid[2]].getX() - m_node[(unsigned int)nodesid[0]].getX()) /
      (m_node[(unsigned int)nodesid[2]].getY() - m_node[(unsigned int)nodesid[0]].getY());

    if (fabs(m1 - m2) < tol)
    {
      // Triangle with DUMMY Value
      m_tsderiv[(unsigned int)triid].setX(m_nodatavalue);
      m_tsderiv[(unsigned int)triid].setY(m_nodatavalue);
      m_tsderiv[(unsigned int)triid].setZ(m_nodatavalue);
      continue;
    }
    
    // Calculate using dx
    for (unsigned int j = 0; j < 3; j++)
    {
      p3da[j].setX(m_node[(unsigned int)nodesid[j]].getNPoint().getX());
      p3da[j].setY(m_node[(unsigned int)nodesid[j]].getNPoint().getY());
      if (m_node[(unsigned int)nodesid[j]].getType() > Last && m_node[(unsigned int)nodesid[j]].getType() < Sample)
        // If breakline node
        p3da[j].setZ(fderiv[(unsigned int)(nodesid[j] - m_fbnode)].getX());
      else
        p3da[j].setZ(m_nfderiv[(unsigned int)nodesid[j]].getX());
    }

    if ((p3da[0].getZ() == p3da[1].getZ()) && (p3da[0].getZ() == p3da[2].getZ()))
    {
      m_tsderiv[(unsigned int)triid].setX(0.);
      dxy = 0.;
    }
    else
    {
      triangleNormalVector(p3da, nvector);
      m_tsderiv[(unsigned int)triid].setX(-nvector[0] / nvector[2]);
      dxy = (-nvector[1] / nvector[2]);
    }

    // Calculate using dy
    for (unsigned int j = 0; j < 3; j++)
      if (m_node[(unsigned int)nodesid[j]].getType() > Last && m_node[(unsigned int)nodesid[j]].getType() < Sample)
        // If breakline node
        p3da[j].setZ(fderiv[(unsigned int)(nodesid[j] - m_fbnode)].getY());
      else
        p3da[j].setZ(m_nfderiv[(unsigned int)nodesid[j]].getY());

    if ((p3da[0].getZ() == p3da[1].getZ()) && (p3da[0].getZ() == p3da[2].getZ()))
    {
      m_tsderiv[(unsigned int)triid].setY(0.);
      dyx = 0.;
    }
    else
    {
      triangleNormalVector(p3da, nvector);
      m_tsderiv[(unsigned int)triid].setY(-nvector[1] / nvector[2]);
      dyx = (-nvector[0] / nvector[2]);
    }
    m_tsderiv[(unsigned int)triid].setZ((dxy + dyx) / 2.);
  }

  return true;
}

bool te::mnt::Tin::BreakNodeSecondDeriv()
{
  int32_t bnodesize,
    node1, node2,
    rclstnids[CLNODES], lclstnids[CLNODES];
  TinNode rsderiv, lsderiv;
  double deltax, deltay, modxy,
    costheta, sintheta,
    cos2theta, sin2theta, sincostheta,
    dzdss, dzdtt, dzdst,
    dzdxx, dzdyy, dzdxy;

  if (!m_tsderiv.size())
    return false;

  // Create and Initialize second derivatives vector
  bnodesize = m_lnode - m_fbnode;
  if (m_nbrsderiv.size())
  {
    m_nbrsderiv.clear();
  }

  size_t i;
  for (i = 0; i < (unsigned int)bnodesize + 1; i++)
  {
    m_nbrsderiv.push_back(m_nsderiv[i + (unsigned int)m_fbnode]);
    m_nblsderiv.push_back(m_nsderiv[i + (unsigned int)m_fbnode]);
  }

  // To each break node
  for (i = (unsigned int)m_fbnode; i < m_node.size() - 1; i++)
  {
    // Special cases
    if ((m_node[i].getZ() >= m_nodatavalue) ||
      (m_node[i].getType() == Deletednode) ||
      (m_node[i].getType() > Breaklinenormal))
      continue;

    // Look for triangles that sharing the node
    BreakNodeClosestPoints((int32_t)i, rclstnids, lclstnids);
    rsderiv = CalcNodeSecondDeriv((int32_t)i, rclstnids);
    lsderiv = CalcNodeSecondDeriv((int32_t)i, lclstnids);

    node1 = NextNode((int32_t)i);
    node2 = PreviousNode((int32_t)i);

    deltax = m_node[(unsigned int)node1].getX() - m_node[(unsigned int)node2].getX();
    deltay = m_node[(unsigned int)node1].getY() - m_node[(unsigned int)node2].getY();
    modxy = sqrt(deltax*deltax + deltay*deltay);
    costheta = deltax / modxy;
    sintheta = deltay / modxy;
    cos2theta = costheta*costheta;
    sin2theta = sintheta*sintheta;
    sincostheta = sintheta*costheta;

    dzdss = cos2theta*m_nsderiv[i].getX() -
      2 * sincostheta*m_nsderiv[i].getZ() +
      sin2theta*m_nsderiv[i].getY();

    dzdtt = sin2theta*rsderiv.getX() +
      2 * sincostheta*rsderiv.getZ() +
      cos2theta*rsderiv.getY();
    dzdst = sincostheta*rsderiv.getX() +
      (cos2theta - sin2theta)*rsderiv.getZ() -
      sincostheta*rsderiv.getY();

    dzdxx = cos2theta*dzdss +
      2 * sincostheta*dzdst +
      sin2theta*dzdtt;
    dzdyy = sin2theta*dzdss -
      2 * sincostheta*dzdst +
      cos2theta*dzdtt;
    dzdxy = -sincostheta*dzdss +
      (cos2theta - sin2theta)*dzdst +
      sincostheta*dzdtt;

    m_nbrsderiv[i - m_fbnode].setX(dzdxx);
    m_nbrsderiv[i - m_fbnode].setY(dzdyy);
    m_nbrsderiv[i - m_fbnode].setZ(dzdxy);

    dzdtt = sin2theta*lsderiv.getX() +
      2 * sincostheta*lsderiv.getZ() +
      cos2theta*lsderiv.getY();
    dzdst = sincostheta*lsderiv.getX() +
      (cos2theta - sin2theta)*lsderiv.getZ() -
      sincostheta*lsderiv.getY();

    dzdxx = cos2theta*dzdss +
      2 * sincostheta*dzdst +
      sin2theta*dzdtt;
    dzdyy = sin2theta*dzdss -
      2 * sincostheta*dzdst +
      cos2theta*dzdtt;
    dzdxy = -sincostheta*dzdss +
      (cos2theta - sin2theta)*dzdst +
      sincostheta*dzdtt;

    m_nblsderiv[i - m_fbnode].setX(dzdxx);
    m_nblsderiv[i - m_fbnode].setY(dzdyy);
    m_nblsderiv[i - m_fbnode].setZ(dzdxy);
  }

  return true;
}

bool te::mnt::Tin::CheckTopology()
{
  int32_t triangid;

  for (triangid = 0; triangid < m_ltriang; triangid++)
  {
    CheckLines(triangid);
  }

  return true;
}


bool te::mnt::Tin::CheckLines(int32_t trid)
{
  int32_t lids[3], nids[3];
  double a, b, c, d, s;
  short i;

  if (trid > m_ltriang)
    return false;

  if (!m_triang[(unsigned int)trid].LinesId(lids))
    return false;

  if (!NodesId(trid, nids))
    return false;

  a = m_node[(unsigned int)nids[1]].getX() - m_node[(unsigned int)nids[0]].getX();
  b = m_node[(unsigned int)nids[1]].getY() - m_node[(unsigned int)nids[0]].getY();
  c = m_node[(unsigned int)nids[2]].getX() - m_node[(unsigned int)nids[0]].getX();
  d = m_node[(unsigned int)nids[2]].getY() - m_node[(unsigned int)nids[0]].getY();
  s = a*d - b*c;

  // To all triangle lines
  for (i = 0; i < 3; i++)
  {
    // Make sure that triangle is inside
    if (s > 0.)
    {
      if (((m_line[(unsigned int)lids[i]].getNodeFrom() == nids[i]) &&
        (m_line[(unsigned int)lids[i]].getRightPolygon() == trid)) ||
        ((m_line[(unsigned int)lids[i]].getNodeTo() == nids[i]) &&
        (m_line[(unsigned int)lids[i]].getLeftPolygon() == trid)))
        m_line[(unsigned int)lids[i]].SwapPolygon();
    }
    else
    {
      if (((m_line[(unsigned int)lids[i]].getNodeTo() == nids[i]) &&
        (m_line[(unsigned int)lids[i]].getRightPolygon() == trid)) ||
        ((m_line[(unsigned int)lids[i]].getNodeFrom() == nids[i]) &&
        (m_line[(unsigned int)lids[i]].getLeftPolygon() == trid)))
        m_line[(unsigned int)lids[i]].SwapPolygon();
    }
  }

  return true;
}


bool te::mnt::Tin::CalcZvalueAkima(int32_t triid, te::gm::PointZ &pt1, te::gm::PointZ &pt2)
{
  double	u, v, ap, bp, cp, dp, x0, y0,
    p00, p01, p02, p03, p04, p05,
    p10, p11, p12, p13, p14,
    p20, p21, p22, p23,
    p30, p31, p32,
    p40, p41, p50,
    p0, p1, p2, p3, p4,
    coef[27];

  DefineAkimaCoeficients(triid, coef);

  // Polynomial coefficients
  p00 = coef[0]; p01 = coef[1]; p02 = coef[2]; p03 = coef[3]; p04 = coef[4]; p05 = coef[5];
  p10 = coef[6]; p11 = coef[7]; p12 = coef[8]; p13 = coef[9]; p14 = coef[10];
  p20 = coef[11]; p21 = coef[12]; p22 = coef[13]; p23 = coef[14];
  p30 = coef[15]; p31 = coef[16]; p32 = coef[17];
  p40 = coef[18]; p41 = coef[19];
  p50 = coef[20];

  // Coefficients of conversion from XY to UV coordinates
  ap = coef[21]; bp = coef[22]; cp = coef[23]; dp = coef[24];
  x0 = coef[25]; y0 = coef[26];

  // Converts point from XY to UV
  u = ap*(pt1.getX() - x0) +
    bp*(pt1.getY() - y0);
  v = cp*(pt1.getX() - x0) +
    dp*(pt1.getY() - y0);

  // Evaluates the polynomial
  p0 = p00 + v*(p01 + v*(p02 + v*(p03 + v*(p04 + v*p05))));
  p1 = p10 + v*(p11 + v*(p12 + v*(p13 + v*p14)));
  p2 = p20 + v*(p21 + v*(p22 + v*p23));
  p3 = p30 + v*(p31 + v*p32);
  p4 = p40 + v*p41;

  pt1.setZ((p0 + u*(p1 + u*(p2 + u*(p3 + u*(p4 + u*p50))))));

  // Converts point from XY to UV
  u = ap*(pt2.getX() - x0) +
    bp*(pt2.getY() - y0);
  v = cp*(pt2.getX() - x0) +
    dp*(pt2.getY() - y0);

  // Evaluates the polynomial
  p0 = p00 + v*(p01 + v*(p02 + v*(p03 + v*(p04 + v*p05))));
  p1 = p10 + v*(p11 + v*(p12 + v*(p13 + v*p14)));
  p2 = p20 + v*(p21 + v*(p22 + v*p23));
  p3 = p30 + v*(p31 + v*p32);
  p4 = p40 + v*p41;

  pt2.setZ((p0 + u*(p1 + u*(p2 + u*(p3 + u*(p4 + u*p50))))));

  return true;
}

bool te::mnt::Tin::DefineAkimaCoeficients(int32_t triid, double *coef)
{
  int32_t nodesid[3];
  te::gm::PointZ p3d[3];

  if (!NodesId(triid, nodesid))
    return false;
  for (unsigned int j = 0; j < 3; j++)
  {
    p3d[j].setX(m_node[(unsigned int)nodesid[j]].getNPoint().getX());
    p3d[j].setY(m_node[(unsigned int)nodesid[j]].getNPoint().getY());
    p3d[j].setZ(m_node[(unsigned int)nodesid[j]].getZ());
  }

  DefineAkimaCoeficients(triid, nodesid, p3d, coef);

  return true;
}

bool te::mnt::Tin::DefineAkimaCoeficients(int32_t triid, int32_t *nodesid, te::gm::PointZ *p3d, double *coef)
{
  double a, b, c, d,
    aa, bb, cc, dd,
    ad, bc, det,
    ap, bp, cp, dp,
    lu, lv,
    theta, phi, thxu, thphi,
    cstheta, lusntheta, lvsntheta,
    e, f, g, h,
    p00, p01, p02, p03, p04, p05,
    p10, p11, p12, p13, p14,
    p20, p21, p22, p23,
    p30, p31, p32,
    p40, p41, p50,
    h1, h2, h3, g1, g2,
    fg, eh, ee, gg,
    zu[3], zv[3], zuu[3], zvv[3], zuv[3];
  short bside;
  int32_t lids[3], nodid;
  std::vector<te::gm::PointZ>* fderiv;
  std::vector<TinNode> *sderiv;

  // Coeficients of conversion from UV to XY coordinates
  a = p3d[1].getX() - p3d[0].getX();
  b = p3d[2].getX() - p3d[0].getX();
  c = p3d[1].getY() - p3d[0].getY();
  d = p3d[2].getY() - p3d[0].getY();

  aa = a * a;
  bb = b * b;
  cc = c * c;
  dd = d * d;

  // Coeficients of conversion from XY to UV coordinates
  ad = a * d;
  bc = b * c;
  det = ad - bc;
  ap = d / det;
  bp = -b / det;
  cp = -c / det;
  dp = a / det;

  bside = 0;
  if (m_fbnode > 0)
  {
    // If there are breaklines
    if (!m_triang[(unsigned int)triid].LinesId(lids))
      return false;

    for (unsigned int i = 0; i < 3; i++)
    {
      if ((m_line[(unsigned int)lids[i]].getNodeTo() >= m_fbnode) &&
        (m_line[(unsigned int)lids[i]].getNodeFrom() >= m_fbnode))
      {
        if (m_line[(unsigned int)lids[i]].getRightPolygon() == triid)
          // Triangle at the right side of a breakline
          bside = 1;
        else if (m_line[(unsigned int)lids[i]].getLeftPolygon() == triid)
          // Triangle at the left side of a breakline
          bside = 2;
        else
          return false;
        break;
      }
      if (m_line[(unsigned int)lids[i]].getNodeTo() >= m_fbnode)
      {
        // Triangle at the right side of a breakline
        bside = 1;
        break;
      }
      if (m_line[(unsigned int)lids[i]].getNodeFrom() >= m_fbnode)
      {
        // Triangle at the left side of a breakline
        bside = 2;
        break;
      }
    }
  }

  // Conversion of derivatives from XY to UV coordinates
  for (unsigned int i = 0; i < 3; i++)
  {
    if ((m_fbnode == 0) ||
      ((m_fbnode > 0) && (nodesid[i] < m_fbnode)))
    {
      nodid = nodesid[i];
      fderiv = &m_nfderiv;
      sderiv = &m_nsderiv;
    }
    else if (bside == 1)
    {
      nodid = nodesid[i] - m_fbnode;
      fderiv = &m_nbrfderiv;
      sderiv = &m_nbrsderiv;
    }
    else if (bside == 2)
    {
      nodid = nodesid[i] - m_fbnode;
      fderiv = &m_nblfderiv;
      sderiv = &m_nbrsderiv;
    }
    else{
      return false;
    }

    if ((*fderiv)[(unsigned int)nodid].getY() >= m_nodatavalue)
    {
      zu[i] = 0.; zv[i] = 0.;
    }
    else
    {
      zu[i] = a * (*fderiv)[(unsigned int)nodid].getX() +
        c * (*fderiv)[(unsigned int)nodid].getY();
      zv[i] = b * (*fderiv)[(unsigned int)nodid].getX() +
        d * (*fderiv)[(unsigned int)nodid].getY();
    }
    if ((*sderiv)[(unsigned int)nodid].getZ() >= m_nodatavalue)
    {
      zuu[i] = 0.;
      zuv[i] = 0.;
      zvv[i] = 0.;
    }
    else
    {
      zuu[i] = aa * (*sderiv)[(unsigned int)nodid].getX() +
        2.*a*c * (double)(*sderiv)[(unsigned int)nodid].getZ() +
        cc * (*sderiv)[(unsigned int)nodid].getY();
      zuv[i] = a*b * (*sderiv)[(unsigned int)nodid].getX() +
        (ad + bc) * (double)(*sderiv)[(unsigned int)nodid].getZ() +
        c*d * (*sderiv)[(unsigned int)nodid].getY();
      zvv[i] = bb * (*sderiv)[(unsigned int)nodid].getX() +
        2.*b*d * (double)(*sderiv)[(unsigned int)nodid].getZ() +
        dd * (*sderiv)[(unsigned int)nodid].getY();
    }
  }

  // Calculate U and V side sizes
  lu = sqrt(aa + cc);
  lv = sqrt(bb + dd);

  // Calculate angle between U and V axis and its cosine
  thxu = atan2(c, a);
  theta = atan2(d, b) - thxu;
  cstheta = cos(theta);
  lusntheta = lu * sin(theta);
  lvsntheta = lv * sin(theta);

  // Calculate angle between U and S axis and its cosine
  phi = atan2(d - c, b - a) - thxu;
  thphi = theta - phi;

  // Coeficients of conversion from ST to UV coordinates when
  //  S axis is paralel to v2v3 side
  e = sin(thphi) / lusntheta;
  f = -cos(thphi) / lusntheta;
  g = sin(phi) / lvsntheta;
  h = cos(phi) / lvsntheta;

  // Definition of the polynomial coefficients
  // Using (u,v) = (0,0) -> v1
  p00 = p3d[0].getZ();
  p10 = zu[0];
  p01 = zv[0];
  p11 = zuv[0];
  p20 = zuu[0] / 2.;
  p02 = zvv[0] / 2.;

  // Using (u,v) = (1,0) -> v2 and z(u,v), zu and zuu
  h1 = p3d[1].getZ() - p00 - p10 - p20;
  h2 = zu[1] - p10 - zuu[0];
  h3 = zuu[1] - zuu[0];
  p30 = 10.*h1 - 4.*h2 + h3 / 2.;
  p40 = -15.*h1 + 7.*h2 - h3;
  p50 = 6.*h1 - 3.*h2 + h3 / 2.;

  // Using (u,v) = (0,1) -> v3 and z(u,v), zu and zuu
  h1 = p3d[2].getZ() - p00 - p01 - p02;
  h2 = zv[2] - p01 - zvv[0];
  h3 = zvv[2] - zvv[0];
  p03 = 10.*h1 - 4.*h2 + h3 / 2.;
  p04 = -15.*h1 + 7.*h2 - h3;
  p05 = 6.*h1 - 3.*h2 + h3 / 2.;

  p41 = 5. * lv * cstheta * p50 / lu;
  p14 = 5. * lu * cstheta * p05 / lv;

  // Using (u,v) = (1,0) -> v2 and z(u,v) and zuv
  h1 = zv[1] - p01 - p11 - p41;
  h2 = zuv[1] - p11 - 4.*p41;
  p21 = 3.*h1 - h2;
  p31 = -2.*h1 + h2;

  // Using (u,v) = (1,0) -> v3 and z(u,v) and zuv
  h1 = zu[2] - p10 - p11 - p14;
  h2 = zuv[2] - p11 - 4.*p14;
  p12 = 3.*h1 - h2;
  p13 = -2.*h1 + h2;

  // Using continuity restriction and zvv at v2 and zuu at v3
  fg = f * g;
  eh = e * h;
  ee = e * e;
  gg = g * g;

  g1 = ee*g*(3.*fg + 2.*eh);
  g2 = e*gg*(2.*fg + 3.*eh);

  h1 = -5.*ee*ee*f*p50 -
    ee*e*(fg + eh)*p41 -
    gg*g*(fg + 4 * eh) -
    5.*gg*gg*h*p05;

  h2 = zvv[1] / 2. - p02 - p12;
  h3 = zuu[2] / 2. - p20 - p21;

  p22 = (g1*h2 + g2*h3 - h1) / (g1 + g2);
  p32 = h2 - p22;
  p23 = h3 - p22;

  // Polynomial coefficients
  coef[0] = p00; coef[1] = p01; coef[2] = p02; coef[3] = p03; coef[4] = p04; coef[5] = p05;
  coef[6] = p10; coef[7] = p11; coef[8] = p12; coef[9] = p13; coef[10] = p14;
  coef[11] = p20; coef[12] = p21; coef[13] = p22; coef[14] = p23;
  coef[15] = p30; coef[16] = p31; coef[17] = p32;
  coef[18] = p40; coef[19] = p41;
  coef[20] = p50;

  // Coefficients of conversion from XY to UV coordinates
  coef[21] = ap; coef[22] = bp; coef[23] = cp; coef[24] = dp;
  coef[25] = p3d[0].getX(); coef[26] = p3d[0].getY();

  return true;
}


/*!
\brief Method that fills the grid locations, inside a triangle, with a zvalue
\param triid is the triangle identification number
\param flin and llin are the first and the last lines (rows) of the grid
\param fcol and lcol are the first and the last columns of the grid
\param zvalue is the z value to be stored in the grid inside the triangle region
\return true always
*/
bool te::mnt::Tin::FillGridValue(int32_t triid, int32_t flin, int32_t llin, int32_t fcol, int32_t lcol, double zvalue)
{
  int32_t  nlin, ncol;
  te::gm::PointZ pg;
  te::gm::Coord2D cg;

  for (nlin = flin; nlin <= llin; nlin++)
  {
    for (ncol = fcol; ncol <= lcol; ncol++)
    {
      cg = m_rst->getGrid()->gridToGeo(ncol, nlin);
      pg.setX(cg.getX());
      pg.setY(cg.getY());
      if (!(ContainsPoint(triid, pg)))
        continue;
      m_rst->setValue((unsigned int)ncol, (unsigned int)nlin, zvalue);
    }
  }
  
  return true;
}


bool te::mnt::Tin::DefineInterLinesColumns(int32_t *nodesid, int32_t &flin, int32_t &llin, int32_t &fcol, int32_t &lcol)
{
  te::gm::Coord2D cg;

  te::gm::Point llpt(std::numeric_limits< float >::max(), std::numeric_limits< float >::max());
  te::gm::Point urpt(-std::numeric_limits< float >::max(), -std::numeric_limits< float >::max());
  for (size_t j = 0; j < 3; j++)
  {
    llpt = Min(llpt, m_node[(unsigned int)nodesid[j]].getNPoint());
    urpt = Max(urpt, m_node[(unsigned int)nodesid[j]].getNPoint());
  }

  //  Calculate lines and coluns intercepted
  cg = m_rst->getGrid()->geoToGrid(llpt.getX(), llpt.getY());
  fcol = te::rst::Round(cg.getX());
  llin = te::rst::Round(cg.getY());
  cg = m_rst->getGrid()->geoToGrid(urpt.getX(), urpt.getY());
  lcol = te::rst::Round(cg.getX());
  flin = te::rst::Round(cg.getY());

  if (((int32_t)m_rst->getNumberOfColumns() <= fcol) || (lcol < 0) ||
    ((int32_t)m_rst->getNumberOfRows() <= flin) || (llin < 0))
    return false;

  if (fcol < 0)
    fcol = 0;
  if (flin < 0)
    flin = 0;
  if ((int32_t)m_rst->getNumberOfColumns() <= lcol)
    lcol = (int32_t)m_rst->getNumberOfColumns() - 1;
  if ((int32_t)m_rst->getNumberOfRows() <= llin)
    llin = (int32_t)m_rst->getNumberOfRows() - 1;

  return true;
}

