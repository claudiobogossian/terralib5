#include "FlowGraphBuilder.h"

#include "AbstractGraphFactory.h"
#include "Config.h"
#include "Edge.h"
#include "Exception.h"
#include "Graph.h"
#include "Vertex.h"
#include "VertexProperty.h"

// TerraLib
#include "../dataaccess.h"
#include "../datatype.h"
#include "../geometry.h"
#include "../common/StringUtils.h"

#include "../common/Translator.h"

// STL
#include <iosfwd>


te::graph::FlowGraphBuilder::FlowGraphBuilder() : AbstractGraphBuilder()
{
  m_edgeId = 0;
}

te::graph::FlowGraphBuilder::~FlowGraphBuilder()
{
}

bool te::graph::FlowGraphBuilder::build(const std::string& shapeFileName, const std::string& linkColumn, const int& srid, const std::string& csvFileName, const int& fromIdx, const int& toIdx, const int& weightIdx,
  const std::map<std::string, std::string>& dsInfo, const std::string& graphType, const std::map<std::string, std::string>& gInfo)
{
  //create output graph
  m_graph = te::graph::AbstractGraphFactory::make(graphType, dsInfo, gInfo);

  assert(m_graph);

  if(createVertexObjects(shapeFileName, linkColumn, srid) == false)
  {
    return false;
  }

  if(createEdgeObjects(csvFileName, fromIdx, toIdx, weightIdx) == false)
  {
    return false;
  }

  return true;
}

int  te::graph::FlowGraphBuilder::getEdgeId()
{
  int id = m_edgeId;

  m_edgeId++;

  return id;
}

te::da::DataSource* te::graph::FlowGraphBuilder::getDataSource(const std::string fileName)
{
  // Creates and connects data source
  std::map<std::string, std::string> connInfo;
  connInfo["path"] = fileName;
  te::da::DataSource* ds = te::da::DataSourceFactory::make("OGR");
  ds->open(connInfo);

  return ds;
}

te::da::DataSet* te::graph::FlowGraphBuilder::getDataSet(te::da::DataSource* ds)
{
   // Gets a transactor
  te::da::DataSourceTransactor* t = ds->getTransactor();
  assert(t);

  // Get the number of data set types that belongs to the data source
  boost::ptr_vector<std::string> datasets;
  t->getCatalogLoader()->getDataSets(datasets);
  assert(!datasets.empty());

  // Gets DataSet Type
  std::string dsName = datasets[0];

  te::da::DataSet* dataset = t->getDataSet(dsName);

  return dataset;
}

std::vector<te::dt::Property*>& te::graph::FlowGraphBuilder::getProperties(te::da::DataSource* ds)
{
// Transactor and catalog loader
  te::da::DataSourceTransactor* transactor = ds->getTransactor();
  te::da::DataSourceCatalogLoader* cl = transactor->getCatalogLoader();
  cl->loadCatalog();

  // Get the number of data set types that belongs to the data source
  boost::ptr_vector<std::string> datasets;
  transactor->getCatalogLoader()->getDataSets(datasets);
  assert(!datasets.empty());

  // Gets DataSet Type
  std::string dsName = datasets[0];
  te::da::DataSetType* dt = cl->getDataSetType(dsName);

  delete transactor;

  // get properties
  return dt->getProperties();
}

bool te::graph::FlowGraphBuilder::createVertexObjects(const std::string& shapeFileName, const std::string& linkColumn, const int& srid)
{
 //get data source
  te::da::DataSource* ds = getDataSource(shapeFileName);

  if(ds == 0)
  {
    return false;
  }

  //get data set
  te::da::DataSet* dataSet = getDataSet(ds);

  if(dataSet == 0)
  {
    return false;
  }

  //get properties
  std::vector<te::dt::Property*> properties = getProperties(ds);

  if(properties.empty())
  {
    return false;
  }
//create graph vertex attrs
  for(size_t t = 0; t < properties.size(); ++t)
  {
    if(te::common::Convert2UCase(properties[t]->getName()) == te::common::Convert2UCase(linkColumn))
    {
      continue;
    }

    te::dt::Property* p;

    if(properties[t]->getType() == te::dt::GEOMETRY_TYPE)
    {
      //create graph attrs
      te::gm::GeometryProperty* gProp = new te::gm::GeometryProperty("coords");
      gProp->setId(0);
      gProp->setGeometryType(te::gm::PointType);
      gProp->setSRID(srid);

      p = gProp;
    }
    else
    {
      p = properties[t]->clone();
      p->setParent(0);
      p->setId(t);
    }

    m_graph->addVertexProperty(p);
  }

  //create vertex objects
  while(dataSet->moveNext())
  {
    int id = dataSet->getInt32(linkColumn);

    Vertex* v = new Vertex(id);
    
    v->setAttributeVecSize(properties.size() - 1);

    int shift = 0;

    for(size_t t = 0; t < properties.size(); ++t)
    {
      if(te::common::Convert2UCase(properties[t]->getName()) == te::common::Convert2UCase(linkColumn))
      {
        shift = 1;
        continue;
      }

      te::dt::AbstractData* ad = 0;

      if(properties[t]->getType() == te::dt::GEOMETRY_TYPE)
      {
        te::gm::Geometry* g = dataSet->getGeometry(properties[t]->getName());
        g->setSRID(srid);

        if(g->getGeomTypeId() == te::gm::PointType)
        {
          ad = g;
        }
        else if(g->getGeomTypeId() == te::gm::PolygonType)
        {
          te::gm::Point* p = ((te::gm::Polygon*)g)->getCentroid();
          p->setSRID(srid);

          ad = p;
        }
        else if(g->getGeomTypeId() == te::gm::MultiPolygonType)
        {
          te::gm::Polygon* poly = (te::gm::Polygon*)((te::gm::MultiPolygon*)g)->getGeometryN(0);

          te::gm::Point* p = poly->getCentroid();
          p->setSRID(srid);

          ad = p;
        }
      }
      else
      {
        ad = dataSet->getValue(properties[t]->getName());
      }
      
      v->addAttribute(t - shift, ad);
    }

    m_graph->add(v);
  }

  delete dataSet;
  delete ds;

  return true;
}

bool te::graph::FlowGraphBuilder::createEdgeObjects(const std::string& csvFileName, const int& fromIdx, const int& toIdx, const int& weightIdx)
{
  //open file
  std::ifstream in(csvFileName.c_str());

  if(in.is_open() == false)
  {
    return false;
  }

  te::dt::SimpleProperty* p = new te::dt::SimpleProperty("weight", te::dt::INT32_TYPE);
  p->setParent(0);
  p->setId(0);

  m_graph->addEdgeProperty(p);

  //create boost tokenizer
  typedef boost::tokenizer< boost::escaped_list_separator<char> > Tokenizer;
  boost::escaped_list_separator<char> sep('\\', ';', '\"');
  
  std::vector<std::string> line;
  std::string buffer;

  //access each line of the csv file
  while(std::getline(in, buffer))
  {
    line.clear();

    Tokenizer tok(buffer, sep);

    line.assign(tok.begin(), tok.end());

    std::string fromStr, toStr, weightStr;
    try
    {
      fromStr = line[fromIdx];
      toStr = line[toIdx];
      weightStr = line[weightIdx];
    }
    catch(...)
    {
      return false;
    }

    //create edge
    int id = getEdgeId();
    int from = atoi(fromStr.c_str());
    int to = atoi(toStr.c_str());
    int weight = atoi(weightStr.c_str());

    Edge* e = new Edge(id, from, to);

    e->setAttributeVecSize(1); //weight attribute
    e->addAttribute(0, new te::dt::SimpleData<int, te::dt::INT32_TYPE>(weight));

    m_graph->add(e);
  }

  return true;
}
