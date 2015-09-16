/*!
\file terralib/mnt/core/Tin.cpp

\brief This file contains a class to define a TIN.

*/

#include "Tin.h"
#include "Utils.h"

#include "../../dataaccess/datasource/DataSourceTransactor.h"
#include "../../dataaccess/utils/Utils.h"

#include "../../datatype/Property.h"
#include "../../datatype/SimpleProperty.h"
#include "../../datatype/StringProperty.h"

#include "../../geometry/GeometryProperty.h"
#include "../../geometry/MultiPolygon.h"

#include "../../memory/DataSet.h"
#include "../../memory/DataSetItem.h"

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
  if ((this->m_point.getX() != rhs.m_point.getX()) ||
    (this->m_point.getY() != rhs.m_point.getY()))
    return false;
  else
    return true;
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
  //Be a vertex v T triangulation and p any point to which you want to know which triangle contains .
  int32_t v = -1;
  int32_t i;

  for (i = m_lline - 1; i >= 0; i--)
  {
    if ((v = m_line[i].getNodeFrom()) != -1)
      break;
  }
  if (v == -1)
    return -1;

  double tol = m_env.getWidth() / 1000000.;
  double px = ptr1.getX();
  double py = ptr1.getY();

  for (;;)
  {
    //1. Defina o segmento de reta r que tem como pontos extremos o ponto p e o vértice v
    te::gm::PointZ ptr2 = m_node[v].getNPoint();

    //2. Defina a aresta auxiliar aaux como sendo nula,
    int32_t aaux = -1;

    //3. Seja m o número de arestas opostas a v. Defina o conjunto A={a1,   ., am} das arestas opostas a v,
    std::vector<int32_t> aam;
    if (!NodeOppositeLines(v, aam))
      return -1;

    //4. Defina o vértice auxiliar vaux como sendo o vértice v,
    int32_t vaux = v;

    //5. Seja n o número de vértices vizinhos a v. Defina o conjunto V={v1,   ., vn} de todos os vértices
    //  vizinhos a v,
    std::vector<int32_t> vvn;
    if (!NodeNodes(v, vvn))
      return -1;

    int32_t vi;
    for (;;)
    {
      bool flag = false;
      //6. Para cada vértice vi (i Î  {1,  .,n}) de V, faça:
      size_t iiv;
      for (iiv = 0; iiv < vvn.size(); iiv++)
      {
        vi = vvn[iiv];
        if ((fabs(px - m_node[vi].getX()) < tol) && (fabs(py - m_node[vi].getY()) < tol))
        {
          //6.1. Se vi é igual a p, o triângulo que contém p é um dos triângulos que compartilham vi.
          // Escolha qualquer um dos triângulos e termine o algoritmo.
          return NodeTriangle(vi);
        }

        //6.2. Se vi é diferente de vaux e está sobre r, faça:
        if (vaux != -1)
        {
          if ((vi == vaux) ||
            ((fabs(m_node[vaux].getX() - m_node[vi].getX()) < tol) &&
            (fabs(m_node[vaux].getY() - m_node[vi].getY()) < tol)))
            continue;
        }

        te::gm::PointZ ptvi = m_node[vi].getNPoint();

        double dist = pointToSegmentDistance(ptr1, ptr2, ptvi, NULL);

        if (dist < tol)
        {
          //6.2.1. Defina v como sendo vi,
          v = vi;
          //6.2.2. Retorne a 2.;
          flag = true;
          break;
        }
      }
      if (iiv != vvn.size())
        break;

      flag = false;
      //7. Para cada aresta ai (i Î  {1,  .,m}) de A, faça:
      size_t iia;
      for (iia = 0; iia < aam.size(); iia++)
      {
        int32_t ai = aam[iia];
        if (ai != aaux)
        {
          //7.1. Se ai é diferente de aaux
          te::gm::PointZ lfr = m_node[m_line[ai].getNodeFrom()].getNPoint();
          te::gm::PointZ lto = m_node[m_line[ai].getNodeTo()].getNPoint();
          if (segIntersect(ptr1, ptr2, lfr, lto))
          {
            //  e intersecta r, faça:
            int32_t t;
            int32_t taux = m_line[ai].getLeftPolygon();
            if (vaux != -1)
            {
              //  7.1.1. Se vaux não é nulo, faça:
              //   7.1.1.1. Defina o triângulo t que compartilha ai 
              //    com o triângulo taux que contém o vértice vaux,
              if (OppositeNode(taux, ai) == vaux)
                t = m_line[ai].getRightPolygon();
              else
                t = m_line[ai].getLeftPolygon();
            }
            else if (aaux != -1)
            {
              // 7.1.2. Senão, aaux não é nula, faça:
              //  7.1.2.1. Defina o triângulo t que compartilha ai com 
              //    o triângulo taux que contém aaux.
              int32_t lidsaux[3];
              t = m_line[ai].getLeftPolygon();
              m_triang[t].LinesId(lidsaux);
              for (short j = 0; j < 3; j++)
              {
                if (lidsaux[j] == aaux)
                {
                  t = m_line[ai].getRightPolygon();
                  break;
                }
              }
            }
            else
            {
              return -1;
            }

            //7.1.3. Se o triângulo t contém o ponto p, termine o algoritmo.
            if (ContainsPoint(t, ptr1))
              return t;

            //7.1.4. Redefina o conjunto A={a1, a2}.
            aam.clear();
            int32_t lids[3];
            m_triang[t].LinesId(lids);
            for (short j = 0; j < 3; j++)
            {
              // As arestas a1 e a2 são as arestas diferentes de ai do triângulo t,
              if (lids[j] != ai)
              {
                aam.push_back(lids[j]);
              }
            }

            //7.1.5. Redefina o conjunto V={v1}.
            vvn.clear();
            // O vértice v1 é o vértice do triângulo t que não está em nenhuma extremidade de ai,
            int32_t v1 = OppositeNode(t, ai);

            if ((fabs(ptr2.getX() - m_node[v1].getX()) < tol) && (fabs(ptr2.getY() - m_node[v1].getY()) < tol))
            {
              flag = true;
              break;
            }

            vvn.push_back(v1);

            //7.1.6. Defina aaux como sendo ai,
            aaux = ai;
            //7.1.7. Defina o vértice auxiliar vaux como sendo nulo
            vaux = -1;
            //7.1.8. Retorne a 6.;
            flag = true;
            break;
          }
        }
      }
      if (vvn.empty())
      {
        for (i = i - 1; i >= 0; i--)
        {
          if ((v = m_line[i].getNodeFrom()) != -1)
            break;
        }
        if (v == -1)
          return -1;
        break;
      }
      if (iia != aam.size())
        continue;
      //8. Se não há mais arestas em A, então:
      //8.1. Seja k o número de triângulos que compartilham v. Defina o conjunto T={t1,   ., tk} de todos os triângulos que compartilham v,
      std::vector<int32_t> ttk;
      if (!NodeTriangles(v, ttk))
        return -1;

      //8.2. Para cada triângulo ti (i Î  {1,  .,k}) de T, faça:
      for (size_t iit = 0; iit < ttk.size(); iit++)
      {
        int32_t ti = ttk[iit];

        //8.2.1. Se o triângulo ti contém o ponto p, termine o algoritmo.
        if (ContainsPoint(ti, ptr1))
          return ti;
      }
      for (i = i - 1; i >= 0; i--)
      {
        if ((v = m_line[i].getNodeFrom()) != -1)
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
  int32_t  linesid[3];

  if (!m_triang[triangId].LinesId(linesid))
    return false;
  if (m_line[linesid[0]].getNodeTo() == m_line[linesid[1]].getNodeTo())
  {
    vertex[0].setX(m_node[m_line[linesid[0]].getNodeFrom()].getX());
    vertex[0].setY(m_node[m_line[linesid[0]].getNodeFrom()].getY());
    vertex[0].setZ(m_node[m_line[linesid[0]].getNodeFrom()].getZ());
    vertex[1].setX(m_node[m_line[linesid[0]].getNodeTo()].getX());
    vertex[1].setY(m_node[m_line[linesid[0]].getNodeTo()].getY());
    vertex[1].setZ(m_node[m_line[linesid[0]].getNodeTo()].getZ());
    vertex[2].setX(m_node[m_line[linesid[1]].getNodeFrom()].getX());
    vertex[2].setY(m_node[m_line[linesid[1]].getNodeFrom()].getY());
    vertex[2].setZ(m_node[m_line[linesid[1]].getNodeFrom()].getZ());
  }
  else if (m_line[linesid[0]].getNodeTo() == m_line[linesid[1]].getNodeFrom())
  {
    vertex[0].setX(m_node[m_line[linesid[0]].getNodeFrom()].getX());
    vertex[0].setY(m_node[m_line[linesid[0]].getNodeFrom()].getY());
    vertex[0].setZ(m_node[m_line[linesid[0]].getNodeFrom()].getZ());
    vertex[1].setX(m_node[m_line[linesid[0]].getNodeTo()].getX());
    vertex[1].setY(m_node[m_line[linesid[0]].getNodeTo()].getY());
    vertex[1].setZ(m_node[m_line[linesid[0]].getNodeTo()].getZ());
    vertex[2].setX(m_node[m_line[linesid[1]].getNodeTo()].getX());
    vertex[2].setY(m_node[m_line[linesid[1]].getNodeTo()].getY());
    vertex[2].setZ(m_node[m_line[linesid[1]].getNodeTo()].getZ());
  }
  else if (m_line[linesid[0]].getNodeFrom() == m_line[linesid[1]].getNodeFrom())
  {
    vertex[0].setX(m_node[m_line[linesid[0]].getNodeTo()].getX());
    vertex[0].setY(m_node[m_line[linesid[0]].getNodeTo()].getY());
    vertex[0].setZ(m_node[m_line[linesid[0]].getNodeTo()].getZ());
    vertex[1].setX(m_node[m_line[linesid[0]].getNodeFrom()].getX());
    vertex[1].setY(m_node[m_line[linesid[0]].getNodeFrom()].getY());
    vertex[1].setZ(m_node[m_line[linesid[0]].getNodeFrom()].getZ());
    vertex[2].setX(m_node[m_line[linesid[1]].getNodeTo()].getX());
    vertex[2].setY(m_node[m_line[linesid[1]].getNodeTo()].getY());
    vertex[2].setZ(m_node[m_line[linesid[1]].getNodeTo()].getZ());
  }
  else if (m_line[linesid[0]].getNodeFrom() == m_line[linesid[1]].getNodeTo())
  {
    vertex[0].setX(m_node[m_line[linesid[0]].getNodeTo()].getX());
    vertex[0].setY(m_node[m_line[linesid[0]].getNodeTo()].getY());
    vertex[0].setZ(m_node[m_line[linesid[0]].getNodeTo()].getZ());
    vertex[1].setX(m_node[m_line[linesid[0]].getNodeFrom()].getX());
    vertex[1].setY(m_node[m_line[linesid[0]].getNodeFrom()].getY());
    vertex[1].setZ(m_node[m_line[linesid[0]].getNodeFrom()].getZ());
    vertex[2].setX(m_node[m_line[linesid[1]].getNodeFrom()].getX());
    vertex[2].setY(m_node[m_line[linesid[1]].getNodeFrom()].getY());
    vertex[2].setZ(m_node[m_line[linesid[1]].getNodeFrom()].getZ());
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
  triangleArea = fabs(((vert[1].getX() - vert[0].getX()) *
    (vert[2].getY() - vert[0].getY())) -
    ((vert[2].getX() - vert[0].getX()) *
    (vert[1].getY() - vert[0].getY())));
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
  int32_t td, te, a, ai, ti, aaux, taux,
    ao,
    lids[3];
  short j;

  //  Find one line that contains node
  a = FindLine(v);
  if (a == -1)
    return false;

  // 1. Defina td como sendo o triângulo que está à direita da 
  // aresta a e te como sendo o triângulo que está à esquerda de a,
  td = m_line[a].getRightPolygon();
  te = m_line[a].getLeftPolygon();

  // 2. Defina ai como sendo aresta a e ti como sendo o triângulo td,
  ai = a;
  ti = td;

  // 3. Se o triângulo ti não for nulo, insira aresta ao de ti 
  // que não é diretamente conectado a v no conjunto A,
  if (ti != -1)
  {
    ao = OppositeEdge(ti, v);
    if (ao == -1)
      return false;
    linids.push_back(ao); // A = lines
  }

  // 4. Enquanto ti for diferente do triângulo te,
  while (ti != te)
  {
    // 4.1. Se o triângulo ti é nulo (esta' na borda da triangulação) faça:
    if (ti == -1)
    {
      // 4.1.1. Defina ti como sendo o triângulo te,
      ti = te;
      // 4.1.2. Defina te como sendo nulo,
      te = -1;
      // 4.1.3. Defina ai como sendo aresta a,
      ai = a;
      // 4.1.4. Se o triângulo ti não for nulo, insira aresta ao 
      // de ti que não é diretamente conectado a v no conjunto A,
      if (ti != -1)
      {
        ao = OppositeEdge(ti, v);
        if (ao == -1)
          return false;
        linids.push_back(ao); // A = lines
      }

      // 4.1.5. Retorne a 4.
      continue;
    }

    // 4.2. Defina a aresta aaux do triângulo ti que conecta o vértice v 
    // e é diferente de ai,
    m_triang[ti].LinesId(lids);
    for (j = 0; j < 3; j++)
    {
      if (lids[j] == ai)
        continue;
      if ((m_line[lids[j]].getNodeFrom() == v) ||
        (m_line[lids[j]].getNodeTo() == v))
        break;
    }
    if (j == 3){
      return false;
    }

    aaux = lids[j];

    // 4.3. Defina taux como sendo o triângulo que compartilha a 
    // aresta aaux com ti,
    if (m_line[aaux].getRightPolygon() == ti)
      taux = m_line[aaux].getLeftPolygon();
    else if (m_line[aaux].getLeftPolygon() == ti)
      taux = m_line[aaux].getRightPolygon();
    else{
      return false;
    }

    // 4.4. Defina ti como sendo o triângulo taux e ai como sendo
    // aresta aaux,
    ti = taux;
    ai = aaux;

    // 4.5. Se o triângulo ti não for nulo, insira aresta ao de ti que 
    // não é diretamente conectado a v no conjunto A,
    if (ti != -1)
    {
      ao = OppositeEdge(ti, v);
      if (ao == -1)
        return false;
      linids.push_back(ao); // A = lines
    }

    // 4.6. Retorne a 4.
  }
  return true;
}


int32_t te::mnt::Tin::OppositeEdge(int32_t triangId, int32_t nodeId)
{
  int32_t lids[3];
  short j;

  m_triang[triangId].LinesId(lids);
  for (j = 0; j < 3; j++)
  {
    if ((m_line[lids[j]].getNodeFrom() != nodeId) &&
      (m_line[lids[j]].getNodeTo() != nodeId))
      break;
  }
  if (j == 3)
  {
    return -1;
  }

  return lids[j];
}

int32_t te::mnt::Tin::FindLine(int32_t nid)
{
  int32_t linid = m_node[nid].getEdge();

  // Test to make sure there is no wrong index
  if (linid != -1)
    if ((m_line[linid].getNodeTo() == nid) ||
      (m_line[linid].getNodeFrom() == nid))
      return linid;

  static int32_t oldtri = 1;
  int32_t i, j, k, lids[3];
  short m;

  if ((oldtri < 0) || (oldtri >= m_ltriang))
    oldtri = 0;
  if (oldtri == 0)
  {
    for (i = 0; i < m_ltriang; i++)
    {
      m_triang[i].LinesId(lids);
      for (m = 0; m < 3; m++)
      {
        if ((m_line[lids[m]].getNodeFrom() == nid) ||
          (m_line[lids[m]].getNodeTo() == nid))
        {
          oldtri = i;
          m_node[nid].setEdge(lids[m]);
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
      m_triang[j].LinesId(lids);
      for (m = 0; m < 3; m++)
      {
        if ((m_line[lids[m]].getNodeFrom() == nid) ||
          (m_line[lids[m]].getNodeTo() == nid))
        {
          oldtri = j;
          m_node[nid].setEdge(lids[m]);
          return lids[m];
        }
      }
      j--;
    }
    if (k < m_ltriang)
    {
      m_triang[k].LinesId(lids);
      for (m = 0; m < 3; m++)
      {
        if ((m_line[lids[m]].getNodeFrom() == nid) ||
          (m_line[lids[m]].getNodeTo() == nid))
        {
          oldtri = k;
          m_node[nid].setEdge(lids[m]);
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
  int32_t td, te, a, ai, ti, aaux, taux = 0,
    lids[3];
  short j;

  // Find one line that contains node
  a = FindLine(v);
  if (a == -1)
    return false;

  // 1. Defina td como sendo o triângulo que está à direita da aresta a 
  // e te como sendo o triângulo que está à esquerda de a,
  td = m_line[a].getRightPolygon();
  te = m_line[a].getLeftPolygon();

  // 2. Defina ai como sendo aresta a e ti como sendo o triângulo td,
  ai = a;
  ti = td;

  // 3. Insira a aresta ai no conjunto A,
  linids.push_back(ai); // A = linids

  // 4. Enquanto ti for diferente do triângulo te,
  while (ti != te)
  {
    //    4.1. Se o triângulo ti é nulo (esta' na borda da triangulação) faça:
    if (ti == -1)
    {
      //      4.1.1. Defina ti como sendo o triângulo te,
      ti = te;
      //      4.1.2. Defina te como sendo nulo,
      te = -1;
      //      4.1.3. Defina ai como sendo aresta a,
      ai = a;
      //      4.1.4. Retorne a 4.
      continue;
    }

    //    4.2. Defina a aresta aaux do triângulo ti que conecta o 
    //    vértice v e é diferente de ai,
    m_triang[ti].LinesId(lids);
    for (j = 0; j < 3; j++)
    {
      if (lids[j] == ai)
        continue;
      if ((m_line[lids[j]].getNodeFrom() == v) ||
        (m_line[lids[j]].getNodeTo() == v))
        break;
    }
    if (j == 3){
      return false;
    }

    aaux = lids[j];

    //    4.3. Defina taux como sendo o triângulo que compartilha a 
    //    aresta aaux com ti,
    if (m_line[aaux].getRightPolygon() == ti)
      taux = m_line[aaux].getLeftPolygon();
    else if (m_line[aaux].getLeftPolygon() == ti)
      taux = m_line[aaux].getRightPolygon();
    else{
      return false;
    }

    //    4.4. Defina ti como sendo o triângulo taux e ai como 
    //    sendo aresta aaux,
    ti = taux;
    ai = aaux;

    //    4.5. 3. Insira a aresta ai no conjunto A,
    linids.push_back(ai); // A = linids

    //  4.6. Retorne a 4.
  }
  return true;
}


int32_t te::mnt::Tin::NodeId(int32_t triangId, short vertex)
{
  int32_t  nodeids[3];

  NodesId(triangId, nodeids);

  return nodeids[vertex];
}

bool te::mnt::Tin::NodesId(int32_t triangId, int32_t *nodeIds)
{
  int32_t  linesid[3];

  if (!m_triang[triangId].LinesId(linesid))
    return false;
  if (m_line[linesid[0]].getNodeTo() == m_line[linesid[1]].getNodeTo())
  {
    nodeIds[0] = m_line[linesid[0]].getNodeFrom();
    nodeIds[1] = m_line[linesid[0]].getNodeTo();
    nodeIds[2] = m_line[linesid[1]].getNodeFrom();
  }
  else if (m_line[linesid[0]].getNodeTo() == m_line[linesid[1]].getNodeFrom())
  {
    nodeIds[0] = m_line[linesid[0]].getNodeFrom();
    nodeIds[1] = m_line[linesid[0]].getNodeTo();
    nodeIds[2] = m_line[linesid[1]].getNodeTo();
  }
  else if (m_line[linesid[0]].getNodeFrom() == m_line[linesid[1]].getNodeFrom())
  {
    nodeIds[0] = m_line[linesid[0]].getNodeTo();
    nodeIds[1] = m_line[linesid[0]].getNodeFrom();
    nodeIds[2] = m_line[linesid[1]].getNodeTo();
  }
  else if (m_line[linesid[0]].getNodeFrom() == m_line[linesid[1]].getNodeTo())
  {
    nodeIds[0] = m_line[linesid[0]].getNodeTo();
    nodeIds[1] = m_line[linesid[0]].getNodeFrom();
    nodeIds[2] = m_line[linesid[1]].getNodeFrom();
  }
  else
    return false;
  return true;
}



bool te::mnt::Tin::NeighborsId(int32_t triangId, int32_t *neighsId)
{
  int32_t  linesid[3];
  short  i;

  m_triang[triangId].LinesId(linesid);
  for (i = 0; i < 3; i++)
  {
    if (linesid[i] >(int32_t)m_linesize)
      return false;
    if (m_line[linesid[i]].getLeftPolygon() == triangId)
      neighsId[i] = m_line[linesid[i]].getRightPolygon();
    else if (m_line[linesid[i]].getRightPolygon() == triangId)
      neighsId[i] = m_line[linesid[i]].getLeftPolygon();
    else
      return false;
  }

  return true;
}

int32_t te::mnt::Tin::OppositeNode(int32_t triangId, int32_t linId)
{
  int32_t  nodtoid, nodfrid, nodeids[3];
  short  i;

  nodtoid = m_line[linId].getNodeTo();
  nodfrid = m_line[linId].getNodeFrom();
  NodesId(triangId, nodeids);
  for (i = 0; i < 3; i++)
  {
    if ((nodtoid == nodeids[i]) ||
      (nodfrid == nodeids[i]))
      continue;
    break;
  }
  if (i == 3){
    return 0;
  }

  return nodeids[i];
}


int32_t te::mnt::Tin::NextNode(int32_t nodeId)
{
  int32_t  j;

  if (m_node[nodeId + 1].getType() != Deletednode)
    return nodeId + 1;

  //  Search next non-deleted point
  for (j = 2; nodeId + j < m_lnode; j++)
    if (m_node[nodeId + j].getType() != Deletednode)
      break;
  if (nodeId + j > m_lnode)
    return -1;

  return nodeId + j;
}


int32_t te::mnt::Tin::PreviousNode(int32_t nodeId)
{
  int32_t  j;

  if (m_node[nodeId - 1].getType() != Deletednode)
    return nodeId - 1;

  //  Search next non-deleted point
  for (j = 2; nodeId - j >= 0; j++)
    if (m_node[nodeId - j].getType() != Deletednode)
      break;
  if (nodeId - j < 0)
    return -1;

  return nodeId - j;
}


bool te::mnt::Tin::NodeNodes(int32_t v, std::vector<int32_t>& nodids)
{
  int32_t  td, te, a, ai, ti, aaux, taux = 0,
    nodeid,
    lids[3];
  short  j;

  //  Find one line that contains node
  a = FindLine(v);
  if (a == -1)
    return false;

  //  1. Defina td como sendo o triângulo que está à direita da aresta a 
  //    e te como sendo o triângulo que está à esquerda de a,
  td = m_line[a].getRightPolygon();
  te = m_line[a].getLeftPolygon();

  //  2. Defina ai como sendo aresta a e ti como sendo o triângulo td,
  ai = a;
  ti = td;

  //  3. Insira o vértice diferente de v conectado à aresta ai
  //    no conjunto V,
  nodeid = m_line[ai].getNodeFrom();
  if (nodeid == v)
    nodeid = m_line[ai].getNodeTo();
  nodids.push_back(nodeid); // V = nodids

  //  4. Enquanto ti for diferente do triângulo te,
  while (ti != te)
  {
    //    4.1. Se o triângulo ti é nulo (esta' na borda da triangulação) faça:
    if (ti == -1)
    {
      //      4.1.1. Defina ti como sendo o triângulo te,
      ti = te;
      //      4.1.2. Defina te como sendo nulo,
      te = -1;
      //      4.1.3. Defina ai como sendo aresta a,
      ai = a;
      //      4.1.4. Retorne a 4.
      continue;
    }

    //    4.2. Defina a aresta aaux do triângulo ti que conecta o 
    //    vértice v e é diferente de ai,
    m_triang[ti].LinesId(lids);
    for (j = 0; j < 3; j++)
    {
      if (lids[j] == ai)
        continue;
      if ((m_line[lids[j]].getNodeFrom() == v) ||
        (m_line[lids[j]].getNodeTo() == v))
        break;
    }
    if (j == 3){
      return false;
    }

    aaux = lids[j];

    //    4.3. Defina taux como sendo o triângulo que compartilha a 
    //    aresta aaux com ti,
    if (m_line[aaux].getRightPolygon() == ti)
      taux = m_line[aaux].getLeftPolygon();
    else if (m_line[aaux].getLeftPolygon() == ti)
      taux = m_line[aaux].getRightPolygon();
    else{
      return false;
    }

    //    4.4. Defina ti como sendo o triângulo taux e ai como 
    //    sendo aresta aaux,
    ti = taux;
    ai = aaux;

    //    4.5. Insira o vértice diferente de v conectado à aresta ai
    //    no conjunto V,
    nodeid = m_line[ai].getNodeFrom();
    if (nodeid == v)
      nodeid = m_line[ai].getNodeTo();
    nodids.push_back(nodeid);

    //  4.6. Retorne a 4.
  }
  return true;
}



int32_t te::mnt::Tin::NodeTriangle(int32_t v)
{
  //  Find one line that contains node
  int32_t a = FindLine(v);
  if (a == -1)
    return -1;

  int32_t td = m_line[a].getRightPolygon();
  if (td == -1)
    return m_line[a].getLeftPolygon();
  else
    return td;
}



bool te::mnt::Tin::NodeTriangles(int32_t v, std::vector<int32_t> &triangles)
{
  int32_t  td, te, a, ai, ti, aaux, taux = 0,
    lids[3];
  short  j;

  //  Find one line that contains node
  a = FindLine(v);
  if (a == -1)
    return false;

  // 1. Defina td como sendo o triângulo que está à direita da 
  //  aresta a e te como sendo o triângulo que está à esquerda de a,
  td = m_line[a].getRightPolygon();
  te = m_line[a].getLeftPolygon();

  // 2. Defina ai como sendo aresta a e ti como sendo o triângulo td,
  ai = a;
  ti = td;

  // 3. Insira o triângulo ti no conjunto C se ele não for nulo,
  if (ti != -1)
  {
    triangles.push_back(ti); // C = triangles
  }

  // 4. Enquanto ti for diferente do triângulo te,
  while (ti != te)
  {
    //    4.1. Se o triângulo ti é nulo (esta' na borda da triangulação) faça:
    if (ti == -1)
    {
      //      4.1.1. Defina ti como sendo o triângulo te,
      ti = te;
      //      4.1.2. Defina te como sendo nulo,
      te = -1;
      //      4.1.3. Defina ai como sendo aresta a,
      ai = a;
      //      4.1.4. Insira o triângulo ti no conjunto C se 
      //      ele não for nulo,
      if (ti != -1)
      {
        triangles.push_back(ti); // C = triangles
      }

      //      4.1.5. Retorne a 4.
      continue;
    }

    // 4.2. Defina a aresta aaux do triângulo ti que conecta o vértice v 
    //    e é diferente de ai,
    m_triang[ti].LinesId(lids);
    for (j = 0; j < 3; j++)
    {
      if (lids[j] == ai)
        continue;
      if ((m_line[lids[j]].getNodeFrom() == v) ||
        (m_line[lids[j]].getNodeTo() == v))
        break;
    }
    if (j == 3){
      return false;
    }

    aaux = lids[j];

    // 4.3. Defina taux como sendo o triângulo que compartilha a 
    //    aresta aaux com ti,
    if (m_line[aaux].getRightPolygon() == ti)
      taux = m_line[aaux].getLeftPolygon();
    else if (m_line[aaux].getLeftPolygon() == ti)
      taux = m_line[aaux].getRightPolygon();
    else{
      return false;
    }

    // 4.4. Defina ti como sendo o triângulo taux e ai como sendo
    //    aresta aaux,
    ti = taux;
    ai = aaux;

    // 4.5. Insira o triângulo ti no conjunto C se ele não for nulo,
    if (ti != -1)
    {
      triangles.push_back(ti); // C = triangles
    }

    // 4.6. Retorne a 4.
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
  te::dt::SimpleProperty* pkProperty = new te::dt::SimpleProperty(outDsetName + "_id", te::dt::INT32_TYPE);
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

  te::gm::GeometryProperty* geomProp = te::da::GetFirstGeomProperty(outDSType.get());

  te::gm::PointZ vert[3];
  int32_t tEdges[3];
  int32_t left[3];
  int32_t right[3];
  Ntype type[3];
  double value[3];

  for (size_t tri = 0; tri < m_triangsize; tri++)
  {
    te::mem::DataSetItem* dataSetItem = new te::mem::DataSetItem(outDSet.get());
    if (!m_triang[tri].LinesId(tEdges))
      continue;
    TrianglePoints((int32_t)tri, vert);
    for (size_t e = 0; e < 3; e++)
    {
      value[e] = vert[e].getZ();
      left[e] = m_line[tEdges[e]].getLeftPolygon();
      right[e] = m_line[tEdges[e]].getRightPolygon();
      type[e] = m_node[m_line[tEdges[e]].getNodeFrom()].getType();
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


void te::mnt::Tin::Save(te::da::DataSource* source, te::da::DataSet* result, te::da::DataSetType* outDsType)
{
  // do any adaptation necessary to persist the output dataset
  //te::da::DataSetTypeConverter* converter = new te::da::DataSetTypeConverter(outDsType, source->getCapabilities());
  //te::da::DataSetType* dsTypeResult = converter->getResult();

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
      source->add(outDsType->getName(), result, options);
    }
    else
    {
      t->begin();

      // create the dataset
      t->createDataSet(outDsType, options);

      // copy from memory to output datasource
      result->moveBeforeFirst();
      std::string name = outDsType->getName();
      t->add(outDsType->getName(), result, options);

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

  const std::size_t np = inDset->getNumProperties();
  m_triangsize = inDset->size();
  int32_t i;

  // Create and set tin triangles vector
  for (i = 0; i < m_triangsize + 2; i++)
    m_triang.push_back(TinTriang());

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
  bool first = true;
  std::map<te::mnt::TinLine, int32_t> linemap;
  std::map<te::mnt::TinLine, int32_t>::iterator itline;
  std::map<te::mnt::TinNode, int32_t> nodemap;
  std::map<te::mnt::TinNode, int32_t>::iterator itnode;

  while (inDset->moveNext())
  {
    id = inDset->getInt32("FID");
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
      val[j] = val[j] > zmax || val[j] < zmin || val[j] > FLT_MAX ? m_nodatavalue : val[j];

      if (val[j] != m_nodatavalue)
      {
        if (val[j] < m_min) 
          m_min = val[j];
        if (val[j] > m_max) 
          m_max = val[j];
      }
      nd.Init(p->getX(), p->getY(), val[j], type[j]);
      itnode = nodemap.find(nd);
      if (itnode != nodemap.end())
      {
        if ((*itnode).first == nd)
          no[j] = (*itnode).second;
        else
        {
          nodemap.insert(std::pair<TinNode, int32_t>(nd, (int32_t)m_node.size()));
          no[j] = (int32_t)m_node.size();
          m_node.push_back(nd);
        }
      }
      else
      {
        nodemap.insert(std::pair<TinNode, int32_t>(nd, (int32_t)m_node.size()));
        no[j] =(int32_t) m_node.size();
        m_node.push_back(nd);
      }
    }

    if (first)
    {
      m_env = *lr->getMBR();
      first = false;
    }
    else
      m_env.Union(*lr->getMBR());

    TinLine tl0(no[0], no[1], left[0], right[0], Normalline);
    TinLine tl1(no[1], no[2], left[1], right[1], Normalline);
    TinLine tl2(no[2], no[0], left[2], right[2], Normalline);

    itline = linemap.find(tl0);
    if (itline != linemap.end())
    {
      if ((*itline).first == tl0)
        lid[0] = (*itline).second;
      else
      {
        linemap.insert(std::pair<TinLine, int32_t>(tl0, (int32_t)m_line.size()));
        lid[0] = (int32_t)m_line.size();
        m_line.push_back(tl0);
      }
    }
    else
    {
      linemap.insert(std::pair<TinLine, int32_t>(tl0, (int32_t)m_line.size()));
      lid[0] = (int32_t)m_line.size();
      m_line.push_back(tl0);
    }

    itline = linemap.find(tl1);
    if (itline != linemap.end())
    {
      if ((*itline).first == tl1)
        lid[1] = (*itline).second;
      else
      {
        linemap.insert(std::pair<TinLine, int32_t>(tl1, (int32_t)m_line.size()));
        lid[1] = (int32_t)m_line.size();
        m_line.push_back(tl1);
      }
    }
    else
    {
      linemap.insert(std::pair<TinLine, int32_t>(tl1, (int32_t)m_line.size()));
      lid[1] = (int32_t)m_line.size();
      m_line.push_back(tl1);
    }

    itline = linemap.find(tl2);
    if (itline != linemap.end())
    {
      if ((*itline).first == tl2)
        lid[2] = (*itline).second;
      else
      {
        linemap.insert(std::pair<TinLine, int32_t>(tl2, (int32_t)m_line.size()));
        lid[2] = (int32_t)m_line.size();
        m_line.push_back(tl2);
      }
    }
    else
    {
      linemap.insert(std::pair<TinLine, int32_t>(tl2, (int32_t)m_line.size()));
      lid[2] = (int32_t)m_line.size();
      m_line.push_back(tl2);
    }


    m_triang[id].setEdges(lid[0], lid[1], lid[2]);
  }

  m_nodesize = m_node.size();
  m_linesize = m_line.size();

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

  for (i = 0; i < m_ltriang; i++)
  {
    NodesId((int32_t)i, nodesid);
    for (j = 0; j < 3; j++)
    {
      p3da[j].setX(m_node[nodesid[j]].getNPoint().getX());
      p3da[j].setY(m_node[nodesid[j]].getNPoint().getY());
      p3da[j].setZ(m_node[nodesid[j]].getZ());
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

  for (i = 0; i < m_ltriang; i++)
  {
    NodesId((int32_t)i, nodesid);

    // Special case
    if ((m_nfderiv[nodesid[0]].getX() >= m_nodatavalue) ||
      (m_nfderiv[nodesid[1]].getX() >= m_nodatavalue) ||
      (m_nfderiv[nodesid[2]].getX() >= m_nodatavalue) ||
      (m_node[nodesid[0]].getZ() >= m_nodatavalue) ||
      (m_node[nodesid[1]].getZ() >= m_nodatavalue) ||
      (m_node[nodesid[2]].getZ() >= m_nodatavalue))
    {
      // Triangle with DUMMY Value
      m_tsderiv[i].setZ(m_nodatavalue);
      continue;
    }

    m1 = m2 = m_nodatavalue;

    if ((m_nfderiv[nodesid[1]].getY() - m_nfderiv[nodesid[0]].getY()) != 0.0)
      m1 = (m_nfderiv[nodesid[1]].getX() - m_nfderiv[nodesid[0]].getX()) /
      (m_nfderiv[nodesid[1]].getY() - m_nfderiv[nodesid[0]].getY());

    if ((m_nfderiv[nodesid[2]].getY() - m_nfderiv[nodesid[0]].getY()) != 0.0)
      m2 = (m_nfderiv[nodesid[2]].getX() - m_nfderiv[nodesid[0]].getX()) /
      (m_nfderiv[nodesid[2]].getY() - m_nfderiv[nodesid[0]].getY());

    if (fabs(m1 - m2) < tol)
    {
      // Triangle with DUMMY Value
      m_tsderiv[i].setZ(m_nodatavalue);
      continue;
    }

    //		Calculate using dx
    for (short j = 0; j < 3; j++)
    {
      p3da[j].setX(m_node[nodesid[j]].getNPoint().getX());
      p3da[j].setY(m_node[nodesid[j]].getNPoint().getY());
      p3da[j].setZ(m_nfderiv[nodesid[j]].getX());
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
    for (short j = 0; j < 3; j++)
      p3da[j].setZ(m_nfderiv[nodesid[j]].getY());

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
  for (i = 1; i < m_lnode; i++)
  {
    // Special cases
    if (m_node[i].getZ() >= m_nodatavalue)
      continue;
    if (m_node[i].getType() == Deletednode)
      // If deleted node
      continue;

    // Look for closest points of the node
    NodeClosestPoints((int32_t)i, clstnids);
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

  for (i = 1; i < m_lnode; i++)
  {
    // Special cases
    if (m_node[i].getZ() >= m_nodatavalue)
      // Node with DUMMY value
      continue;
    if (m_node[i].getType() == Deletednode)
      // Deleted node 
      continue;

    // Look for closest point of the node
    NodeClosestPoints((int32_t)i, clstnids);
    sderiv = CalcNodeSecondDeriv((int32_t)i, clstnids);
    m_nsderiv[i].setX(sderiv.getX());
    m_nsderiv[i].setY(sderiv.getY());
    m_nsderiv[i].setZ(sderiv.getZ());
  }

  return true;
}

bool te::mnt::Tin::NodeClosestPoints(int32_t nid, int32_t *clstNids, bool useBrNode)
{
  int32_t ltri, rtri,
    flin,
    lineid, lids[3];
  size_t j, k;
  double dist, distv[CLNODES];
  int32_t nodeid;

  // Find one line that contains node
  lineid = FindLine(nid);
  if (lineid == -1)
    return false;
  flin = lineid;

  for (j = 0; j < CLNODES; j++)
  {
    distv[j] = m_nodatavalue;
    clstNids[j] = -1;
  }

  // Find right and left triangle
  rtri = m_line[lineid].getRightPolygon();
  ltri = m_line[lineid].getLeftPolygon();
  if (rtri == -1L)
  {
    rtri = ltri;
    ltri = -1L;
  }
  while (rtri != ltri)
  {
    m_triang[rtri].LinesId(lids);
    for (j = 0; j < 3; j++)
    {
      // Find line that contains node
      if (lids[j] == lineid)
        continue;
      if ((m_line[lids[j]].getNodeFrom() == nid) ||
        (m_line[lids[j]].getNodeTo() == nid))
        break;
    }
    if (j == 3){
      return false;
    }

    lineid = lids[j];
    if (m_line[lineid].getNodeFrom() == nid)
      nodeid = m_line[lineid].getNodeTo();
    else
      nodeid = m_line[lineid].getNodeFrom();

    if ((m_node[nodeid].getZ() < m_nodatavalue) && ((useBrNode) ||
      ((useBrNode) && (nodeid < m_fbnode))))
    {
      dist = (m_node[nid].getX() - m_node[nodeid].getX()) *
        (m_node[nid].getX() - m_node[nodeid].getX()) +
        (m_node[nid].getY() - m_node[nodeid].getY()) *
        (m_node[nid].getY() - m_node[nodeid].getY());
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

    // Find new right triangle
    if (m_line[lineid].getRightPolygon() == rtri)
      rtri = m_line[lineid].getLeftPolygon();
    else if (m_line[lineid].getLeftPolygon() == rtri)
      rtri = m_line[lineid].getRightPolygon();
    else
      return false;

    if ((rtri == -1) && (ltri != -1))
    {
      rtri = ltri;
      ltri = -1L;
      lineid = flin;
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

  p3da[0].setX(m_node[nodeId].getNPoint().getX());
  p3da[0].setY(m_node[nodeId].getNPoint().getY());
  p3da[0].setZ(m_node[nodeId].getZ());

  tnx = 0.;
  tny = 0.;
  tnz = 0.;
  for (j = 0; j < CLNODES; j++)
  {
    if (clstNodes[j] == -1L)
      break;
    p3da[1].setX(m_node[clstNodes[j]].getNPoint().getX());
    p3da[1].setY(m_node[clstNodes[j]].getNPoint().getY());
    p3da[1].setZ(m_node[clstNodes[j]].getZ());
    for (k = j + 1; k < CLNODES; k++)
    {
      if (clstNodes[k] == -1)
        break;
      p3da[2].setX(m_node[clstNodes[k]].getNPoint().getX());
      p3da[2].setY(m_node[clstNodes[k]].getNPoint().getY());
      p3da[2].setZ(m_node[clstNodes[k]].getZ());

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
  short	j, k;
  TinNode	sderiv;

  p3da[0].setX(m_node[nodeId].getNPoint().getX());
  p3da[0].setY(m_node[nodeId].getNPoint().getY());
  p3da[0].setZ(m_nfderiv[nodeId].getX());

  tnxx = 0.;
  tnxy = 0.;
  tnxz = 0.;
  tnyx = 0.;
  tnyy = 0.;
  tnyz = 0.;

  for (j = 0; j < CLNODES; j++)
  {
    if (clstNIds[j] == -1L)
      break;
    p3da[1].setX(m_node[clstNIds[j]].getNPoint().getX());
    p3da[1].setY(m_node[clstNIds[j]].getNPoint().getY());
    p3da[1].setZ(m_nfderiv[clstNIds[j]].getX());
    for (k = j + 1; k < CLNODES; k++)
    {
      if (clstNIds[k] == -1L)
        break;
      p3da[2].setX(m_node[clstNIds[k]].getNPoint().getX());
      p3da[2].setY(m_node[clstNIds[k]].getNPoint().getY());
      p3da[2].setZ(m_nfderiv[clstNIds[k]].getX());

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
  p3da[0].setZ(m_nfderiv[nodeId].getY());
  for (j = 0; j < CLNODES; j++)
  {
    if (clstNIds[j] == -1L)
      break;
    p3da[1].setX(m_node[clstNIds[j]].getNPoint().getX());
    p3da[1].setY(m_node[clstNIds[j]].getNPoint().getY());
    p3da[1].setZ(m_nfderiv[clstNIds[j]].getY());
    for (k = j + 1; k < CLNODES; k++)
    {
      if (clstNIds[k] == -1L)
        break;
      p3da[2].setX(m_node[clstNIds[k]].getNPoint().getX());
      p3da[2].setY(m_node[clstNIds[k]].getNPoint().getY());
      p3da[2].setZ(m_nfderiv[clstNIds[k]].getY());

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
  bnodesize = m_lnode - m_fbnode;
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
  for (i = m_fbnode + 1; i < m_lnode - 1; i++)
  {
    // Special cases
    if ((m_node[i].getZ() >= m_nodatavalue) ||
      (m_node[i].getType() == Deletednode) ||
      (m_node[i].getType() > Breaklinenormal))
      continue;

    // Look for triangles that share the node
    BreakNodeClosestPoints((int32_t)i, rclstnids, lclstnids);
    rderiv = CalcNodeFirstDeriv((int32_t)i, rclstnids);
    lderiv = CalcNodeFirstDeriv((int32_t)i, lclstnids);

    node1 = NextNode((int32_t)i);
    node2 = PreviousNode((int32_t)i);

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
  for (i = m_fbnode + 1; i < m_lnode - 1; i++)
  {
    if (m_node[i].getZ() >- m_nodatavalue)
      continue;

    // If first or last point of a breakline
    if ((m_node[i].getType() == Breaklinefirst) ||
      (m_node[i].getType() == Breaklinelast) ||
      (m_node[i].getType() == Deletednode))
      continue;

    // Look for triangles that share the node
    NodeTriangles((int32_t)i, rightri, leftri);

    // Calculate second derivative of node using right side triangles
    CalcTriangleSecondDeriv((int32_t)i, rightri, m_nbrfderiv);

    // Calculate second derivative of node using left side triangles
    CalcTriangleSecondDeriv((int32_t)i, leftri, m_nblfderiv);

    rightri.clear();
    leftri.clear();
  }

  return true;
}

bool te::mnt::Tin::NodeTriangles(int32_t nodeid, std::vector<int32_t> &rightri, std::vector<int32_t> &leftri)
{
  int32_t fline, linid, lids[3], rtri, ltri;
  short k;

  // Find one line that contains node
  fline = FindLine(nodeid);
  if (fline == -1)
    return false;
  linid = fline;

  ltri = m_line[linid].getLeftPolygon();
  rtri = m_line[linid].getRightPolygon();
  if (rtri == -1)
  {
    rtri = ltri;
    ltri = -1;
  }

  // While right side triangle different from left side<br>
  while (rtri != ltri)
  {
    if (m_line[linid].getNodeTo() == nodeid)
    {
      rightri.push_back(m_line[linid].getRightPolygon());
    }
    else if (m_line[linid].getNodeFrom() == nodeid)
    {
      leftri.push_back(m_line[linid].getLeftPolygon());
    }

    // Find line that contains node in the right triangle
    m_triang[rtri].LinesId(lids);
    for (k = 0; k < 3; k++)
    {
      if (lids[k] == linid)
        continue;
      if ((m_line[lids[k]].getNodeFrom() == nodeid) ||
        (m_line[lids[k]].getNodeTo() == nodeid))
        break;
    }
    if (k == 3){
      return false;
    }

    // Make right triangle equal to the triangle at the
    // other side
    linid = lids[k];
    if (m_line[linid].getRightPolygon() == rtri)
      rtri = m_line[linid].getLeftPolygon();
    else if (m_line[linid].getLeftPolygon() == rtri)
      rtri = m_line[linid].getRightPolygon();
    else
      return false;
    if (rtri == -1)
    {
      rtri = ltri;
      ltri = -1;
      linid = fline;
    }
  }
  //	Insert left side triangle in triangle list
  if (ltri != -1)
  {
    if (m_line[linid].getNodeTo() == nodeid)
    {
      rightri.push_back(m_line[linid].getRightPolygon());
    }
    if (m_line[linid].getNodeFrom() == nodeid)
    {
      leftri.push_back(m_line[linid].getLeftPolygon());
    }
  }

  return true;
}



bool te::mnt::Tin::BreakNodeClosestPoints(int32_t nid, int32_t *rClstNids, int32_t *lClstNids)
{
  int32_t ltri, rtri,
    flin,
    lineid, lids[3];
  short j, k;
  double dist, rdistv[CLNODES], ldistv[CLNODES];
  int32_t nodeid;

  // Find one line that contains node
  lineid = FindLine(nid);
  if (lineid == -1)
    return false;
  flin = lineid;

  for (j = 0; j < CLNODES; j++)
  {
    rdistv[j] = m_nodatavalue;
    rClstNids[j] = -1;

    ldistv[j] = m_nodatavalue;
    lClstNids[j] = -1;
  }

  // Find right and left triangle
  rtri = m_line[lineid].getRightPolygon();
  ltri = m_line[lineid].getLeftPolygon();
  if (rtri == -1L)
  {
    rtri = ltri;
    ltri = -1L;
  }
  while (rtri != ltri)
  {
    m_triang[rtri].LinesId(lids);
    for (j = 0; j < 3; j++)
    {
      // Find line that contains node
      if (lids[j] == lineid)
        continue;
      if ((m_line[lids[j]].getNodeFrom() == nid) ||
        (m_line[lids[j]].getNodeTo() == nid))
        break;
    }
    if (j == 3){
      return false;
    }

    lineid = lids[j];
    if (m_line[lineid].getNodeFrom() == nid)
    {
      nodeid = m_line[lineid].getNodeTo();
      if (nodeid < m_fbnode)
      { //Node is a breakline node at right side of it
        dist = (m_node[nid].getX() - m_node[nodeid].getX()) *
          (m_node[nid].getX() - m_node[nodeid].getX()) +
          (m_node[nid].getY() - m_node[nodeid].getY()) *
          (m_node[nid].getY() - m_node[nodeid].getY());
        for (j = 0; j < CLNODES; j++)
        {
          if (dist < rdistv[j])
          {
            for (k = CLNODES - 1; k > j; k--)
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
      nodeid = m_line[lineid].getNodeFrom();
      if (nodeid < m_fbnode)
      { //Node is a breakline node at left side of it
        dist = (m_node[nid].getX() - m_node[nodeid].getX()) *
          (m_node[nid].getX() - m_node[nodeid].getX()) +
          (m_node[nid].getY() - m_node[nodeid].getY()) *
          (m_node[nid].getY() - m_node[nodeid].getY());
        for (j = 0; j < CLNODES; j++)
        {
          if (dist < ldistv[j])
          {
            for (k = CLNODES - 1; k > j; k--)
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
    if (m_line[lineid].getRightPolygon() == rtri)
      rtri = m_line[lineid].getLeftPolygon();
    else if (m_line[lineid].getLeftPolygon() == rtri)
      rtri = m_line[lineid].getRightPolygon();
    else
      return false;
    if ((rtri == -1) && (ltri != -1))
    {
      rtri = ltri;
      ltri = -1L;
      lineid = flin;
    }
  }

  return true;
}

bool te::mnt::Tin::CalcTriangleSecondDeriv(int32_t nodeid, std::vector<int32_t> &triangles, std::vector<te::gm::PointZ> &fderiv)
{
  short j;
  int32_t triid, nodesid[3];
  te::gm::PointZ p3da[3];
  double nvector[3];
  double dxy, dyx;
  double m1, m2;
  double tol = (double).01;

  for (size_t i = 0; i < triangles.size(); i++)
  {
    triid = triangles[i];
    NodesId(triid, nodesid);

    // Special case
    if ((m_node[nodesid[0]].getZ() >= m_nodatavalue) ||
      (m_node[nodesid[1]].getZ() >= m_nodatavalue) ||
      (m_node[nodesid[2]].getZ() >= m_nodatavalue))
    {
      //			Triangle with DUMMY Value
      m_tsderiv[triid].setX(m_nodatavalue);
      m_tsderiv[triid].setY(m_nodatavalue);
      m_tsderiv[triid].setZ(m_nodatavalue);
      continue;
    }
    m1 = m2 = m_nodatavalue;
    if ((m_node[nodesid[1]].getY() - m_node[nodesid[0]].getY()) != 0.0)
      m1 = (m_node[nodesid[1]].getX() - m_node[nodesid[0]].getX()) /
      (m_node[nodesid[1]].getY() - m_node[nodesid[0]].getY());
    if ((m_node[nodesid[2]].getY() - m_node[nodesid[0]].getY()) != 0.0)
      m2 = (m_node[nodesid[2]].getX() - m_node[nodesid[0]].getX()) /
      (m_node[nodesid[2]].getY() - m_node[nodesid[0]].getY());

    if (fabs(m1 - m2) < tol)
    {
      // Triangle with DUMMY Value
      m_tsderiv[triid].setX(m_nodatavalue);
      m_tsderiv[triid].setY(m_nodatavalue);
      m_tsderiv[triid].setZ(m_nodatavalue);
      continue;
    }
    
    // Calculate using dx
    for (j = 0; j < 3; j++)
    {
      p3da[j].setX(m_node[nodesid[j]].getNPoint().getX());
      p3da[j].setY(m_node[nodesid[j]].getNPoint().getY());
      if (m_node[nodesid[j]].getType() > Last && m_node[nodesid[j]].getType() < Sample)
        // If breakline node
        p3da[j].setZ(fderiv[nodesid[j] - m_fbnode].getX());
      else
        p3da[j].setZ(m_nfderiv[nodesid[j]].getX());
    }

    if ((p3da[0].getZ() == p3da[1].getZ()) && (p3da[0].getZ() == p3da[2].getZ()))
    {
      m_tsderiv[triid].setX(0.);
      dxy = 0.;
    }
    else
    {
      triangleNormalVector(p3da, nvector);
      m_tsderiv[triid].setX(-nvector[0] / nvector[2]);
      dxy = (-nvector[1] / nvector[2]);
    }

    // Calculate using dy
    for (j = 0; j < 3; j++)
      if (m_node[nodesid[j]].getType() > Last && m_node[nodesid[j]].getType() < Sample)
        // If breakline node
        p3da[j].setZ(fderiv[nodesid[j] - m_fbnode].getY());
      else
        p3da[j].setZ(m_nfderiv[nodesid[j]].getY());

    if ((p3da[0].getZ() == p3da[1].getZ()) && (p3da[0].getZ() == p3da[2].getZ()))
    {
      m_tsderiv[triid].setY(0.);
      dyx = 0.;
    }
    else
    {
      triangleNormalVector(p3da, nvector);
      m_tsderiv[triid].setY(-nvector[1] / (double)nvector[2]);
      dyx = (-nvector[0] / (double)nvector[2]);
    }
    m_tsderiv[triid].setZ((float)((dxy + dyx) / 2.));
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
  for (i = 0; i < bnodesize + 1; i++)
  {
    m_nbrsderiv.push_back(m_nsderiv[i + m_fbnode]);
    m_nblsderiv.push_back(m_nsderiv[i + m_fbnode]);
  }

  // To each break node
  for (i = m_fbnode + 1; i < m_lnode - 1; i++)
  {
    // Special cases
    if ((m_node[i].getZ() >= m_nodatavalue) ||
      (m_node[i].getType() == Deletednode) ||
      (m_node[i].getType() > Breaklinenormal))
      continue;

    // Look for triangles that share the node
    BreakNodeClosestPoints((int32_t)i, rclstnids, lclstnids);
    rsderiv = CalcNodeSecondDeriv((int32_t)i, rclstnids);
    lsderiv = CalcNodeSecondDeriv((int32_t)i, lclstnids);

    node1 = NextNode((int32_t)i);
    node2 = PreviousNode((int32_t)i);

    deltax = m_node[node1].getX() - m_node[node2].getX();
    deltay = m_node[node1].getY() - m_node[node2].getY();
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

  if ((trid == -1) || (trid > m_ltriang))
    return false;

  m_triang[trid].LinesId(lids);
  NodesId(trid, nids);

  a = m_node[nids[1]].getX() - m_node[nids[0]].getX();
  b = m_node[nids[1]].getY() - m_node[nids[0]].getY();
  c = m_node[nids[2]].getX() - m_node[nids[0]].getX();
  d = m_node[nids[2]].getY() - m_node[nids[0]].getY();
  s = a*d - b*c;

  // To all triangle lines
  for (i = 0; i < 3; i++)
  {
    // Make sure that triangle is inside
    if (s > 0.)
    {
      if (((m_line[lids[i]].getNodeFrom() == nids[i]) &&
        (m_line[lids[i]].getRightPolygon() == trid)) ||
        ((m_line[lids[i]].getNodeTo() == nids[i]) &&
        (m_line[lids[i]].getLeftPolygon() == trid)))
        m_line[lids[i]].SwapPolygon();
    }
    else
    {
      if (((m_line[lids[i]].getNodeTo() == nids[i]) &&
        (m_line[lids[i]].getRightPolygon() == trid)) ||
        ((m_line[lids[i]].getNodeFrom() == nids[i]) &&
        (m_line[lids[i]].getLeftPolygon() == trid)))
        m_line[lids[i]].SwapPolygon();
    }
  }

  return true;
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
      m_triang[i].LinesId(lids);
      for (m = 0; m < 3; m++)
      {
        if ((m_line[lids[m]].getNodeFrom() == fnid) &&
          (m_line[lids[m]].getNodeTo() == snid))
        {
          oldtri = i;
          return lids[m];
        }
        if ((m_line[lids[m]].getNodeTo() == fnid) &&
          (m_line[lids[m]].getNodeFrom() == snid))
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
      m_triang[j].LinesId(lids);
      for (m = 0; m < 3; m++)
      {
        if ((m_line[lids[m]].getNodeFrom() == fnid) &&
          (m_line[lids[m]].getNodeTo() == snid))
        {
          oldtri = j;
          return lids[m];
        }
        if ((m_line[lids[m]].getNodeTo() == fnid) &&
          (m_line[lids[m]].getNodeFrom() == snid))
        {
          oldtri = j;
          return lids[m];
        }
      }
      j--;
    }
    if (k < m_ltriang)
    {
      m_triang[k].LinesId(lids);
      for (m = 0; m < 3; m++)
      {
        if ((m_line[lids[m]].getNodeFrom() == fnid) &&
          (m_line[lids[m]].getNodeTo() == snid))
        {
          oldtri = k;
          return lids[m];
        }
        if ((m_line[lids[m]].getNodeTo() == fnid) &&
          (m_line[lids[m]].getNodeFrom() == snid))
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
