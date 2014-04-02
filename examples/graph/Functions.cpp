//TerraLib
#include <terralib/dataaccess/datasource/DataSource.h>
#include <terralib/dataaccess/datasource/DataSourceFactory.h>
#include <terralib/dataaccess/utils/Utils.h>
#include <terralib/datatype/SimpleProperty.h>
#include <terralib/graph/core/AbstractGraph.h>
#include <terralib/graph/core/GraphMetadata.h>
#include <terralib/graph/core/Vertex.h>
#include <terralib/raster/Grid.h>
#include <terralib/raster/RasterFactory.h>
#include "GraphExamples.h"

// STL Includes
#include <iostream>



std::auto_ptr<te::rst::Raster> OpenRaster(const std::string& pathName, const int& srid)
{
  std::cout << std::endl << "Open Raster: " << pathName << std::endl;

  //set connection info
  std::map<std::string, std::string> rinfo;
  rinfo["URI"] = pathName;

  // open input raster
  te::rst::Raster* raster = te::rst::RasterFactory::open(rinfo);
  raster->getGrid()->setSRID(srid);

  std::auto_ptr<te::rst::Raster> rst(raster);

  return rst;
}

std::auto_ptr<te::da::DataSource> OpenDataSource(std::map<std::string, std::string> connInfo, std::string dsType)
{
  std::auto_ptr<te::da::DataSource> ds = te::da::DataSourceFactory::make(dsType);
  ds->setConnectionInfo(connInfo);
  ds->open();

  return ds;
}

std::auto_ptr<te::da::DataSource> OpenOGRDataSource(const std::string& pathName)
{
  std::map<std::string, std::string> connInfo;
  connInfo["URI"] = pathName;

  return OpenDataSource(connInfo, "OGR");
}

std::auto_ptr<te::gm::Envelope> getDataSetExtent(te::da::DataSource* ds, std::string dataSetName)
{
  std::auto_ptr<te::da::DataSet> dataSet = ds->getDataSet(dataSetName);

  std::size_t geomPos = te::da::GetFirstSpatialPropertyPos(dataSet.get());

  return dataSet->getExtent(geomPos);
}

int AssociateGraphVertexAttribute(te::da::DataSource* ds, std::string dataSetName, int idIdx, int attrIdx, boost::shared_ptr<te::graph::AbstractGraph> graph, int dataType, std::string attrName)
{
  assert(ds);

  //add graph attr
  int attrGraphIdx = AddGraphVertexAttribute(graph, attrName, dataType);

  //get the number of attributes from graph
  int attrSize = graph->getMetadata()->getVertexPropertySize();

  //dataset iterator
  std::auto_ptr<te::da::DataSet> dataSet = ds->getDataSet(dataSetName);

  dataSet->moveBeforeFirst();

  while(dataSet->moveNext())
  {
    te::dt::AbstractData* ad = dataSet->getValue(attrIdx).release();

    int idx = dataSet->getInt32(idIdx);

    te::graph::Vertex* v = graph->getVertex(idx);

    if(v)
    {
      //resize attr vector
      v->setAttributeVecSize(attrSize);

      //add attribute
      v->addAttribute(attrGraphIdx, ad);
    }
  }

  return attrGraphIdx;
}

int AddGraphVertexAttribute(boost::shared_ptr<te::graph::AbstractGraph> graph, std::string attrName, int dataType)
{
  //add new attribute
  te::dt::SimpleProperty* p = new te::dt::SimpleProperty(attrName, dataType);
  p->setParent(0);
  p->setId(0);

  graph->addVertexProperty(p);

  // verify what the index of the new property
  int idx = 0;

  for(int i = 0; i < graph->getVertexPropertySize(); ++ i)
  {
    if(graph->getVertexProperty(i)->getName() == attrName)
    {
      idx = i;
      break;
    }
  }

  return idx;
}

int AddGraphEdgeAttribute(boost::shared_ptr<te::graph::AbstractGraph> graph, std::string attrName, int dataType)
{
  //add new attribute
  te::dt::SimpleProperty* p = new te::dt::SimpleProperty(attrName, dataType);
  p->setParent(0);
  p->setId(0);

  graph->addEdgeProperty(p);

  // verify what the index of the new property
  int idx = 0;

  for(int i = 0; i < graph->getEdgePropertySize(); ++ i)
  {
    if(graph->getEdgeProperty(i)->getName() == attrName)
    {
      idx = i;
      break;
    }
  }

  return idx;
}
