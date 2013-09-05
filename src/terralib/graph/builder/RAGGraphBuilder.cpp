#include "RAGGraphBuilder.h"

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
#include "../sam.h"

#include "../common/Translator.h"
#include "../common/progress/TaskProgress.h"

#include "../common/Translator.h"

te::graph::RAGGraphBuilder::RAGGraphBuilder() : AbstractGraphBuilder()
{
  m_edgeId = 0;
}

te::graph::RAGGraphBuilder::~RAGGraphBuilder()
{
}

bool te::graph::RAGGraphBuilder::build(const std::string& shapeFileName, const std::string& linkColumn, const int& srid, 
  const std::map<std::string, std::string>& dsInfo, const std::string& graphType, const std::map<std::string, std::string>& gInfo)
{
  //create output graph
  m_graph = te::graph::AbstractGraphFactory::make(graphType, dsInfo, gInfo);

  assert(m_graph);

  if(createVertexObjects(shapeFileName, linkColumn, srid) == false)
  {
    return false;
  }

  if(createEdgeObjects(shapeFileName, linkColumn) == false)
  {
    return false;
  }


  return true;
}

int  te::graph::RAGGraphBuilder::getEdgeId()
{
  int id = m_edgeId;

  m_edgeId++;

  return id;
}


te::da::DataSource* te::graph::RAGGraphBuilder::getDataSource(const std::string fileName)
{
  // Creates and connects data source
  std::map<std::string, std::string> connInfo;
  connInfo["path"] = fileName;
  te::da::DataSource* ds = te::da::DataSourceFactory::make("OGR");
  ds->open(connInfo);

  return ds;
}

te::da::DataSet* te::graph::RAGGraphBuilder::getDataSet(te::da::DataSource* ds)
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

std::vector<te::dt::Property*>& te::graph::RAGGraphBuilder::getProperties(te::da::DataSource* ds)
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

bool te::graph::RAGGraphBuilder::createVertexObjects(const std::string& shapeFileName, const std::string& linkColumn, const int& srid)
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

  //create graph vertex attrs
  te::gm::GeometryProperty* gProp = new te::gm::GeometryProperty("coords");
  gProp->setId(0);
  gProp->setGeometryType(te::gm::PointType);
  gProp->setSRID(srid);

  m_graph->addVertexProperty(gProp);

  //get geometry column
  std::string geomColumn = "";
  if(getGeometryColumn(shapeFileName, geomColumn) == false)
  {
    return false;
  }

  //create vertex objects
  while(dataSet->moveNext())
  {
    int id = dataSet->getInt32(linkColumn);

    Vertex* v = new Vertex(id);
    
    v->setAttributeVecSize(1);

    te::gm::Geometry* g = dataSet->getGeometry(geomColumn);
    g->setSRID(srid);

    te::dt::AbstractData* ad = 0;

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

    v->addAttribute(0, ad);

    m_graph->add(v);
  }

  delete dataSet;
  delete ds;

  return true;
}

bool te::graph::RAGGraphBuilder::createEdgeObjects(const std::string& shapeFileName, const std::string& linkColumn)
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

  //get geometry column
  std::string geomColumn = "";
  if(getGeometryColumn(shapeFileName, geomColumn) == false)
  {
    return false;
  }

  te::sam::rtree::Index<int> rtree;
  std::map<int, te::gm::Geometry*> geomMap;
  //create tree
  while(dataSet->moveNext())
  {
    int id = dataSet->getInt32(linkColumn);
    te::gm::Geometry* g = dataSet->getGeometry(geomColumn);
    const te::gm::Envelope* box = g->getMBR();

    rtree.insert(*box, id);

    geomMap.insert(std::map<int, te::gm::Geometry*>::value_type(id, g));
  }

  dataSet->moveBeforeFirst();

  te::common::TaskProgress task;
  
  task.setTotalSteps(dataSet->size());
  task.setMessage("RAG Builder - Extracting Edges");

  //create vertex objects
  while(dataSet->moveNext())
  {
    int vFromId = dataSet->getInt32(linkColumn);

    te::gm::Geometry* g = dataSet->getGeometry(geomColumn);

    std::vector<int> results;

    rtree.search(*g->getMBR(), results);

    for(size_t t = 0; t < results.size(); ++t)
    {
      std::map<int, te::gm::Geometry*>::iterator it = geomMap.find(results[t]);

      if(it != geomMap.end())
      {
        if(g->touches(it->second))
        {
          int edgeId = getEdgeId();

          int vToId = results[t];

          Edge* e = new Edge(edgeId, vFromId, vToId);

          m_graph->add(e);
        }
      }
    }

    task.pulse();
  }

  delete dataSet;
  delete ds;

  return true;
}

bool te::graph::RAGGraphBuilder::getGeometryColumn(const std::string& shapeFileName, std::string& columnName)
{
//get data source
  te::da::DataSource* ds = getDataSource(shapeFileName);

  if(ds == 0)
  {
    return false;
  }

  //get properties
  std::vector<te::dt::Property*> properties = getProperties(ds);

  if(properties.empty())
  {
    delete ds;
    return false;
  }

  for(size_t t = 0; t < properties.size(); ++t)
  {
    if(properties[t]->getType() == te::dt::GEOMETRY_TYPE)
    {
      columnName = properties[t]->getName();
    }
  }

  if(columnName.empty())
  {
    delete ds;
    return false;
  }

  return true;
}
