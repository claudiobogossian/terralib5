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
  m_env = env;
}

int32_t te::mnt::Tin::FindTriangle(te::gm::PointZ &ptr1)
{
  //Seja v um vértice qualquer da triangulação T e p o ponto
  //	para o qual se deseja saber qual triângulo t o contém.
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
  //	double tol = Tminedgesize;

  double px = ptr1.getX();
  double py = ptr1.getY();

  for (;;)
  {
    //1. Defina o segmento de reta r que tem como pontos extremos o ponto p e
    //	o vértice v,
    te::gm::PointZ ptr2 = m_node[v].getNPoint();

    //2. Defina a aresta auxiliar aaux como sendo nula,
    int32_t aaux = -1;

    //3. Seja m o número de arestas opostas a v. Defina o conjunto A={a1, ..., am} das arestas opostas a v,
    std::vector<int32_t> aam;
    if (!NodeOppositeLines(v, aam))
      return -1;

    //4. Defina o vértice auxiliar vaux como sendo o vértice v,
    int32_t vaux = v;

    //5. Seja n o número de vértices vizinhos a v. Defina o conjunto V={v1, ..., vn} de todos os vértices
    //	vizinhos a v,
    std::vector<int32_t> vvn;
    if (!NodeNodes(v, vvn))
      return -1;

    int32_t vi;
    for (;;)
    {
      bool flag = false;
      //6. Para cada vértice vi (i Î  {1,...,n}) de V, faça:
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
      //7. Para cada aresta ai (i Î  {1,...,m}) de A, faça:
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
            //	e intersecta r, faça:
            int32_t t;
            int32_t taux = m_line[ai].getLeftPolygon();
            if (vaux != -1)
            {
              //	7.1.1. Se vaux não é nulo, faça:
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
              //	As arestas a1 e a2 são as arestas diferentes de ai do triângulo t,
              if (lids[j] != ai)
              {
                aam.push_back(lids[j]);
              }
            }

            //7.1.5. Redefina o conjunto V={v1}.
            vvn.clear();
            //	O vértice v1 é o vértice do triângulo t que não está em nenhuma extremidade de ai,
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
      //8.1. Seja k o número de triângulos que compartilham v. Defina o conjunto T={t1, ..., tk} de todos os triângulos que compartilham v,
      std::vector<int32_t> ttk;
      if (!NodeTriangles(v, ttk))
        return -1;

      //8.2. Para cada triângulo ti (i Î  {1,...,k}) de T, faça:
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
  int32_t	linesid[3];

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
  double	totalArea, triangleArea;
  te::gm::PointZ	vert[3];

  TrianglePoints(triangId, vert);

  //	Calculate the base triangle area
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
  int32_t	td, te, a, ai, ti, aaux, taux,
    ao,
    lids[3];
  short	j;

  //	Find one line that contains node
  a = FindLine(v);
  if (a == -1)
    return false;

  // 1. Defina td como sendo o triângulo que está à direita da 
  //	aresta a e te como sendo o triângulo que está à esquerda de a,
  td = m_line[a].getRightPolygon();
  te = m_line[a].getLeftPolygon();

  // 2. Defina ai como sendo aresta a e ti como sendo o triângulo td,
  ai = a;
  ti = td;

  // 3. Se o triângulo ti não for nulo, insira aresta ao de ti 
  //	que não é diretamente conectado a v no conjunto A,
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
    //		4.1. Se o triângulo ti é nulo (esta' na borda da triangulação) faça:
    if (ti == -1)
    {
      //			4.1.1. Defina ti como sendo o triângulo te,
      ti = te;
      //			4.1.2. Defina te como sendo nulo,
      te = -1;
      //			4.1.3. Defina ai como sendo aresta a,
      ai = a;
      //			4.1.4. Se o triângulo ti não for nulo, insira aresta ao 
      //			de ti que não é diretamente conectado a v no conjunto A,
      if (ti != -1)
      {
        ao = OppositeEdge(ti, v);
        if (ao == -1)
          return false;
        linids.push_back(ao); // A = lines
      }

      //			4.1.5. Retorne a 4.
      continue;
    }

    // 4.2. Defina a aresta aaux do triângulo ti que conecta o vértice v 
    //		e é diferente de ai,
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
    //		aresta aaux com ti,
    if (m_line[aaux].getRightPolygon() == ti)
      taux = m_line[aaux].getLeftPolygon();
    else if (m_line[aaux].getLeftPolygon() == ti)
      taux = m_line[aaux].getRightPolygon();
    else{
      return false;
    }

    // 4.4. Defina ti como sendo o triângulo taux e ai como sendo
    //		aresta aaux,
    ti = taux;
    ai = aaux;

    // 4.5. Se o triângulo ti não for nulo, insira aresta ao de ti que 
    //		não é diretamente conectado a v no conjunto A,
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
  int32_t	lids[3];
  short	j;

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

  static int32_t	oldtri = 1;
  int32_t		i, j, k, lids[3];
  short		m;

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
  int32_t	td, te, a, ai, ti, aaux, taux = 0,
    lids[3];
  short	j;

  //	Find one line that contains node
  a = FindLine(v);
  if (a == -1)
    return false;

  //	1. Defina td como sendo o triângulo que está à direita da aresta a 
  //		e te como sendo o triângulo que está à esquerda de a,
  td = m_line[a].getRightPolygon();
  te = m_line[a].getLeftPolygon();

  //	2. Defina ai como sendo aresta a e ti como sendo o triângulo td,
  ai = a;
  ti = td;

  //	3. Insira a aresta ai no conjunto A,
  linids.push_back(ai); // A = linids

  //	4. Enquanto ti for diferente do triângulo te,
  while (ti != te)
  {
    //		4.1. Se o triângulo ti é nulo (esta' na borda da triangulação) faça:
    if (ti == -1)
    {
      //			4.1.1. Defina ti como sendo o triângulo te,
      ti = te;
      //			4.1.2. Defina te como sendo nulo,
      te = -1;
      //			4.1.3. Defina ai como sendo aresta a,
      ai = a;
      //			4.1.4. Retorne a 4.
      continue;
    }

    //		4.2. Defina a aresta aaux do triângulo ti que conecta o 
    //		vértice v e é diferente de ai,
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

    //		4.3. Defina taux como sendo o triângulo que compartilha a 
    //		aresta aaux com ti,
    if (m_line[aaux].getRightPolygon() == ti)
      taux = m_line[aaux].getLeftPolygon();
    else if (m_line[aaux].getLeftPolygon() == ti)
      taux = m_line[aaux].getRightPolygon();
    else{
      return false;
    }

    //		4.4. Defina ti como sendo o triângulo taux e ai como 
    //		sendo aresta aaux,
    ti = taux;
    ai = aaux;

    //		4.5. 3. Insira a aresta ai no conjunto A,
    linids.push_back(ai); // A = linids

    //	4.6. Retorne a 4.
  }
  return true;
}


int32_t te::mnt::Tin::NodeId(int32_t triangId, short vertex)
{
  int32_t	nodeids[3];

  NodesId(triangId, nodeids);

  return nodeids[vertex];
}

bool te::mnt::Tin::NodesId(int32_t triangId, int32_t *nodeIds)
{
  int32_t	linesid[3];

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
  int32_t	linesid[3];
  short	i;

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
  int32_t	nodtoid, nodfrid, nodeids[3];
  short	i;

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
  int32_t	j;

  if (m_node[nodeId + 1].getType() != Deletednode)
    return nodeId + 1;

  //	Search next non-deleted point
  for (j = 2; nodeId + j < m_lnode; j++)
    if (m_node[nodeId + j].getType() != Deletednode)
      break;
  if (nodeId + j > m_lnode)
    return -1;

  return nodeId + j;
}


int32_t te::mnt::Tin::PreviousNode(int32_t nodeId)
{
  int32_t	j;

  if (m_node[nodeId - 1].getType() != Deletednode)
    return nodeId - 1;

  //	Search next non-deleted point
  for (j = 2; nodeId - j >= 0; j++)
    if (m_node[nodeId - j].getType() != Deletednode)
      break;
  if (nodeId - j < 0)
    return -1;

  return nodeId - j;
}


bool te::mnt::Tin::NodeNodes(int32_t v, std::vector<int32_t>& nodids)
{
  int32_t	td, te, a, ai, ti, aaux, taux = 0,
    nodeid,
    lids[3];
  short	j;

  //	Find one line that contains node
  a = FindLine(v);
  if (a == -1)
    return false;

  //	1. Defina td como sendo o triângulo que está à direita da aresta a 
  //		e te como sendo o triângulo que está à esquerda de a,
  td = m_line[a].getRightPolygon();
  te = m_line[a].getLeftPolygon();

  //	2. Defina ai como sendo aresta a e ti como sendo o triângulo td,
  ai = a;
  ti = td;

  //	3. Insira o vértice diferente de v conectado à aresta ai
  //  	no conjunto V,
  nodeid = m_line[ai].getNodeFrom();
  if (nodeid == v)
    nodeid = m_line[ai].getNodeTo();
  nodids.push_back(nodeid); // V = nodids

  //	4. Enquanto ti for diferente do triângulo te,
  while (ti != te)
  {
    //		4.1. Se o triângulo ti é nulo (esta' na borda da triangulação) faça:
    if (ti == -1)
    {
      //			4.1.1. Defina ti como sendo o triângulo te,
      ti = te;
      //			4.1.2. Defina te como sendo nulo,
      te = -1;
      //			4.1.3. Defina ai como sendo aresta a,
      ai = a;
      //			4.1.4. Retorne a 4.
      continue;
    }

    //		4.2. Defina a aresta aaux do triângulo ti que conecta o 
    //		vértice v e é diferente de ai,
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

    //		4.3. Defina taux como sendo o triângulo que compartilha a 
    //		aresta aaux com ti,
    if (m_line[aaux].getRightPolygon() == ti)
      taux = m_line[aaux].getLeftPolygon();
    else if (m_line[aaux].getLeftPolygon() == ti)
      taux = m_line[aaux].getRightPolygon();
    else{
      return false;
    }

    //		4.4. Defina ti como sendo o triângulo taux e ai como 
    //		sendo aresta aaux,
    ti = taux;
    ai = aaux;

    //		4.5. Insira o vértice diferente de v conectado à aresta ai
    //		no conjunto V,
    nodeid = m_line[ai].getNodeFrom();
    if (nodeid == v)
      nodeid = m_line[ai].getNodeTo();
    nodids.push_back(nodeid);

    //	4.6. Retorne a 4.
  }
  return true;
}



int32_t te::mnt::Tin::NodeTriangle(int32_t v)
{
  //	Find one line that contains node
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
  int32_t	td, te, a, ai, ti, aaux, taux = 0,
    lids[3];
  short	j;

  //	Find one line that contains node
  a = FindLine(v);
  if (a == -1)
    return false;

  // 1. Defina td como sendo o triângulo que está à direita da 
  //	aresta a e te como sendo o triângulo que está à esquerda de a,
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
    //		4.1. Se o triângulo ti é nulo (esta' na borda da triangulação) faça:
    if (ti == -1)
    {
      //			4.1.1. Defina ti como sendo o triângulo te,
      ti = te;
      //			4.1.2. Defina te como sendo nulo,
      te = -1;
      //			4.1.3. Defina ai como sendo aresta a,
      ai = a;
      //			4.1.4. Insira o triângulo ti no conjunto C se 
      //			ele não for nulo,
      if (ti != -1)
      {
        triangles.push_back(ti); // C = triangles
      }

      //			4.1.5. Retorne a 4.
      continue;
    }

    // 4.2. Defina a aresta aaux do triângulo ti que conecta o vértice v 
    //		e é diferente de ai,
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
    //		aresta aaux com ti,
    if (m_line[aaux].getRightPolygon() == ti)
      taux = m_line[aaux].getLeftPolygon();
    else if (m_line[aaux].getLeftPolygon() == ti)
      taux = m_line[aaux].getRightPolygon();
    else{
      return false;
    }

    // 4.4. Defina ti como sendo o triângulo taux e ai como sendo
    //		aresta aaux,
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

    m_triangsize = 2 * (m_nodesize)-5;	// ntri = 2n-5

    while (m_triang.size() < m_triangsize)
    {
      m_triang.push_back(TinTriang());
    }

    m_linesize = 3 * m_nodesize;	// nlin = (n-1)*3

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
  //  int32_t nto[3];
  //  int32_t nfrom[3];
  double value[3];

  for (size_t tri = 0; tri < m_triangsize; tri++)
  {
    te::mem::DataSetItem* dataSetItem = new te::mem::DataSetItem(outDSet.get());
    if (!m_triang[tri].LinesId(tEdges))
      continue;
    TrianglePoints((int32_t)tri, vert);
    // NodesId((int32_t)tri, nids);
    for (size_t e = 0; e < 3; e++)
    {
      value[e] = vert[e].getZ();
      left[e] = m_line[tEdges[e]].getLeftPolygon();
      right[e] = m_line[tEdges[e]].getRightPolygon();
      type[e] = m_node[m_line[tEdges[e]].getNodeFrom()].getType();
      //      nfrom[e] = m_line[tEdges[e]].getNodeFrom();
      //      nto[e] = m_line[tEdges[e]].getNodeTo();
    }
    //   te::gm::TIN *t = new te::gm::TIN(0, te::gm::TINZType, m_srid);
    std::auto_ptr<te::gm::Polygon> p(new te::gm::Polygon(0, te::gm::PolygonZType));
    std::auto_ptr<te::gm::LinearRing> s(new te::gm::LinearRing(4, te::gm::LineStringZType));
    s->setPointN(0, vert[0]);
    s->setPointN(1, vert[1]);
    s->setPointN(2, vert[2]);
    s->setPointN(3, vert[0]);
    p->push_back(s.get());
    //    t->add(p);

    dataSetItem->setInt32(0, (int32_t)tri);
    if (value[0] < FLT_MAX)
      dataSetItem->setDouble("val1", value[0]);
    if (value[1] < FLT_MAX)
      dataSetItem->setDouble("val2", value[1]);
    if (value[2] < FLT_MAX)
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
    val[0] = inDset->getDouble("val1");
    val[1] = inDset->getDouble("val2");
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
      val[j] = val[j] > zmax || val[j] < zmin ? BIGFLOAT : val[j];
      nd.Init(p->getX(), p->getY(), (float)val[j], type[j]);
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

