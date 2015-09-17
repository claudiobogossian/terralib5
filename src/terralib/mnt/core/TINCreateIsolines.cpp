/*!
\file terralib/mnt/core/TINCreateIsolines.cpp

\brief This file contains a class to generate isolines from TIN.

*/

#include "TINCreateIsolines.h"
#include "Utils.h"

#include "../../datatype/SimpleProperty.h"
#include "../../geometry/GeometryProperty.h"
#include "../../memory/DataSet.h"
#include "../../memory/DataSetItem.h"


bool te::mnt::TINCreateIsolines::run()
{
  if (!m_values.size())
    return false;

  int32_t nodesid[3];
  te::gm::PointZ p3da[3];
  std::vector<te::gm::PointZ> ptline;
  te::gm::LineString isoline(te::gm::LineStringZType, m_srid);
  std::vector<te::gm::PointZ> pts;

  double cvalue;

  std::sort(m_values.begin(), m_values.end());

  double zmin = m_values[0];
  double zmax = m_values[m_values.size() - 1];

  LoadTin(m_inDsrc, m_inDsetName, zmin, zmax);

  for (size_t v = 0; v < m_values.size(); v++)
  {
    cvalue = m_values[v];

    for (size_t i = 0; i < m_triang.size(); i++)
    {
      if (!NodesId((int32_t)i, nodesid))
        continue;
      for (size_t j = 0; j < 3; j++)
      {
        p3da[j].setX(m_node[nodesid[j]].getNPoint().getX());
        p3da[j].setY(m_node[nodesid[j]].getNPoint().getY());
        p3da[j].setZ(m_node[nodesid[j]].getZ());
      }
      if (!testVertexValues(cvalue, p3da))
        continue;

      // Define intersection between contour and triangle
      defineInterTriangle(pts, p3da, cvalue);
    }

    if (pts.size() > 2)
    {
      if (!extractLines(pts, m_isolist, m_tol))
      {
        return false;
      }
       
      pts.clear();
    }
  }

  SaveIso();

  return true;
}

void te::mnt::TINCreateIsolines::setInput(te::da::DataSourcePtr inDsrc,
  std::string inDsetName,
  std::auto_ptr<te::da::DataSetType> inDsetType)
{
  m_inDsrc = inDsrc;
  m_inDsetName = inDsetName;
  m_inDsetType = inDsetType;
}

void te::mnt::TINCreateIsolines::setOutput(te::da::DataSourcePtr outDsrc, std::string dsname)
{
  m_outDsrc = outDsrc;
  m_outDsetName = dsname;

}

void te::mnt::TINCreateIsolines::setParams(std::vector<double> &val, double tol)
{
  m_values = val;
  m_tol = tol;
}

bool te::mnt::TINCreateIsolines::SaveIso()
{
  std::auto_ptr<te::da::DataSetType> dt(new te::da::DataSetType(m_outDsetName));

  //Primary key
  te::dt::SimpleProperty* prop0 = new te::dt::SimpleProperty("ID", te::dt::INT32_TYPE);
  prop0->setAutoNumber(true);
  te::dt::SimpleProperty* prop1 = new te::dt::SimpleProperty("Z", te::dt::DOUBLE_TYPE);
  te::gm::GeometryProperty* prop2 = new te::gm::GeometryProperty("iso", 0, te::gm::LineStringType, true);
  prop2->setSRID(m_srid);
  dt->add(prop0);
  dt->add(prop1);
  dt->add(prop2);

  te::mem::DataSet* ds = new te::mem::DataSet(dt.get());
  te::mem::DataSetItem* dataSetItem = new te::mem::DataSetItem(ds);

  std::size_t niso = m_isolist.size();
  int id = 0;

  for (unsigned int Idx = 0; Idx < m_isolist.size(); ++Idx)
  {
    te::mem::DataSetItem* dataSetItem = new te::mem::DataSetItem(ds);
    te::gm::LineString gout = m_isolist[Idx];
    double *zvalue = gout.getZ();
    dataSetItem->setInt32("ID", id++);
    if (zvalue)
      dataSetItem->setDouble("Z", zvalue[0]);
    dataSetItem->setGeometry("iso", (te::gm::Geometry*)gout.clone());

    ds->add(dataSetItem);
  }

  Save(m_outDsrc.get(), ds, dt.get());

  return true;

}

