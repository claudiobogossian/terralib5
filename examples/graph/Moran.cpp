
#include <terralib/datatype/Enums.h>
#include <terralib/datatype/SimpleData.h>
#include <terralib/graph/core/AbstractGraph.h>
#include <terralib/graph/core/Edge.h>
#include <terralib/graph/core/GraphMetadata.h>
#include <terralib/graph/core/Vertex.h>
#include <terralib/graph/graphs/DirectedGraph.h>
#include <terralib/graph/iterator/MemoryIterator.h>
#include "GraphExamples.h"

double CalculateMoranIndex(boost::shared_ptr<te::graph::DirectedGraph> graph, int stdDevZIdx, int localMeanWZIdx);

int CreateStdDevZProperty(boost::shared_ptr<te::graph::DirectedGraph> graph, int attrIdx);

int CreateLocalMeanProperty(boost::shared_ptr<te::graph::DirectedGraph> graph, int attrIdx, int stdDevZIdx, int weigthIdx);

int CreateWeigthProperty(boost::shared_ptr<te::graph::DirectedGraph> graph);

double CalculateMean(boost::shared_ptr<te::graph::DirectedGraph> graph, int attrIdx);

double CalculateVariance(boost::shared_ptr<te::graph::DirectedGraph> graph, int stdDevZIdx, double mean = 0.);

void Moran()
{
  //create graph
  boost::shared_ptr<te::graph::AbstractGraph> graph = CreateGPMDistanceGraph(false);
  boost::shared_ptr<te::graph::DirectedGraph> dGraph(dynamic_cast<te::graph::DirectedGraph*>(graph.get()));

  assert(dGraph.get());

  //associate dataset attribute from a data source
  std::auto_ptr<te::da::DataSource> ds = OpenOGRDataSource(TERRALIB_EXAMPLES_DATA_DIR "/data/graph/UP_pol.shp");
  std::string dataSetName = "UP_pol";
  int columnId = 1; //"COD_UP";
  int attrId = 6; //"HOM_03";
  std::string graphAttrName = "hom_03";

  //associate the graph to desired dataset attribute
  int attrIdx = AssociateGraphVertexAttribute(ds.get(), dataSetName, columnId, attrId, dGraph, te::dt::DOUBLE_TYPE, graphAttrName);

  //create the weigth edge attribute
  int weigthIdx = CreateWeigthProperty(dGraph);

  //create the standard deviation Z property
  int stdDevZIdx = CreateStdDevZProperty(dGraph, attrIdx);

  //create the local mean WZ
  int localMeanWZIdx = CreateLocalMeanProperty(dGraph, attrIdx, stdDevZIdx, weigthIdx);

  //calculate Moran Index
  double moranIdx = CalculateMoranIndex(dGraph, stdDevZIdx, localMeanWZIdx);

}

double CalculateMoranIndex(boost::shared_ptr<te::graph::DirectedGraph> graph, int stdDevZIdx, int localMeanWZIdx)
{
  //add std dev z attr into graph
  int moranIdx = AddGraphVertexAttribute(graph, "MoranIdx", te::dt::DOUBLE_TYPE);

  //get the number of attributes from graph
  int attrSize = graph->getMetadata()->getVertexPropertySize();

  //calculate variance
  double variance = CalculateVariance(graph, stdDevZIdx, 0); // Mean 0 ????

 //create graph vertex iterator
  std::auto_ptr<te::graph::MemoryIterator> it(new te::graph::MemoryIterator(graph.get()));

  te::graph::Vertex* v = it->getFirstVertex();

  double sum = 0;
  int count = 0;

  while(!it->isVertexIteratorAfterEnd())
  {
    te::dt::SimpleData<double, te::dt::DOUBLE_TYPE>* sdStdDevZ = dynamic_cast< te::dt::SimpleData<double, te::dt::DOUBLE_TYPE>* >(v->getAttributes()[stdDevZIdx]);
    double stdDevZ = sdStdDevZ->getValue();

    te::dt::SimpleData<double, te::dt::DOUBLE_TYPE>* sdLocalMeanWZ = dynamic_cast< te::dt::SimpleData<double, te::dt::DOUBLE_TYPE>* >(v->getAttributes()[localMeanWZIdx]);
    double localMeanWZ = sdLocalMeanWZ->getValue();

    double ZxWZ = 0.;

    if(variance != 0.)
      ZxWZ = (stdDevZ * localMeanWZ) / variance;

    te::dt::AbstractData* adZxWZ = new te::dt::SimpleData<double, te::dt::DOUBLE_TYPE>(ZxWZ);

    //resize attr vector
    v->setAttributeVecSize(attrSize);

    //add attribute
    v->addAttribute(moranIdx, adZxWZ);

    ++count;
    sum += ZxWZ;

    v = it->getNextVertex();
  }

  double moranIndexValue = sum / count;

  return moranIndexValue;
}

int CreateStdDevZProperty(boost::shared_ptr<te::graph::DirectedGraph> graph, int attrIdx)
{
  //add std dev z attr into graph
  int stdDevZIdx = AddGraphVertexAttribute(graph, "StdDevZ", te::dt::DOUBLE_TYPE);

  //get the number of attributes from graph
  int attrSize = graph->getMetadata()->getVertexPropertySize();

  //calculate mean
  double mean = CalculateMean(graph, attrIdx);

  //create graph vertex iterator
  std::auto_ptr<te::graph::MemoryIterator> it(new te::graph::MemoryIterator(graph.get()));

  te::graph::Vertex* v = it->getFirstVertex();

  while(!it->isVertexIteratorAfterEnd())
  {
    te::dt::SimpleData<double, te::dt::DOUBLE_TYPE>* sd = dynamic_cast< te::dt::SimpleData<double, te::dt::DOUBLE_TYPE>* >(v->getAttributes()[attrIdx]);

    double stdDevZ = sd->getValue() - mean; // ????

    te::dt::AbstractData* ad = new te::dt::SimpleData<double, te::dt::DOUBLE_TYPE>(stdDevZ);

    //resize attr vector
    v->setAttributeVecSize(attrSize);

    //add attribute
    v->addAttribute(stdDevZIdx, ad);

    v = it->getNextVertex();
  }

  return stdDevZIdx;
}

int CreateLocalMeanProperty(boost::shared_ptr<te::graph::DirectedGraph> graph, int attrIdx, int stdDevZIdx, int weigthIdx)
{
  //add local mean attr into graph
  int localMeanIdx = AddGraphVertexAttribute(graph, "LocalMeanWZ", te::dt::DOUBLE_TYPE);

  //get the number of attributes from graph
  int attrSize = graph->getMetadata()->getVertexPropertySize();

  //create graph vertex iterator
  std::auto_ptr<te::graph::MemoryIterator> it(new te::graph::MemoryIterator(graph.get()));

  te::graph::Vertex* v = it->getFirstVertex();

  while(!it->isVertexIteratorAfterEnd())
  {
    double localMeanWZ = 0.;

    std::set<int> outputEdges = v->getSuccessors();
    std::set<int>::iterator itEdges = outputEdges.begin();

    while(itEdges != outputEdges.end())
    {
      te::graph::Edge* e = graph->getEdge(*itEdges);

      if(e)
      {
        te::graph::Vertex* vTo = graph->getVertex(e->getIdTo());

        if(vTo)
        {
          te::dt::SimpleData<double, te::dt::DOUBLE_TYPE>* sdStdDevZ = dynamic_cast< te::dt::SimpleData<double, te::dt::DOUBLE_TYPE>* >(vTo->getAttributes()[stdDevZIdx]);
          double stdDevZ = sdStdDevZ->getValue();

          te::dt::SimpleData<double, te::dt::DOUBLE_TYPE>* sdWeigth = dynamic_cast< te::dt::SimpleData<double, te::dt::DOUBLE_TYPE>* >(e->getAttributes()[weigthIdx]);
          double weigth = sdWeigth->getValue();

          localMeanWZ += weigth * stdDevZ;
        }
      }

      ++itEdges;
    }

    te::dt::AbstractData* adLocalMean = new te::dt::SimpleData<double, te::dt::DOUBLE_TYPE>(localMeanWZ);

    //resize attr vector
    v->setAttributeVecSize(attrSize);

    //add attribute
    v->addAttribute(localMeanIdx, adLocalMean);

    v = it->getNextVertex();
  }

  return localMeanIdx;
}

int CreateWeigthProperty(boost::shared_ptr<te::graph::DirectedGraph> graph)
{
  //add weigth attr into graph
  int weigthIdx = AddGraphEdgeAttribute(graph, "Weigth", te::dt::DOUBLE_TYPE);

  //get the number of attributes from graph
  int attrSize = graph->getMetadata()->getEdgePropertySize();

  //create graph vertex iterator
  std::auto_ptr<te::graph::MemoryIterator> it(new te::graph::MemoryIterator(graph.get()));

  te::graph::Vertex* v = it->getFirstVertex();

  while(!it->isVertexIteratorAfterEnd())
  {
    std::set<int> outputEdges = v->getSuccessors();
    std::set<int>::iterator itEdges = outputEdges.begin();

    double weigth = 1./ (double)outputEdges.size();

    while(itEdges != outputEdges.end())
    {
      te::graph::Edge* e = graph->getEdge(*itEdges);

      if(e)
      {
        te::dt::AbstractData* adWeigth = new te::dt::SimpleData<double, te::dt::DOUBLE_TYPE>(weigth);

        //resize attr vector
        e->setAttributeVecSize(attrSize);

        //add attribute
        e->addAttribute(weigthIdx, adWeigth);
      }

      ++itEdges;
    }

    v = it->getNextVertex();
  }

  return weigthIdx;
}

double CalculateMean(boost::shared_ptr<te::graph::DirectedGraph> graph, int attrIdx)
{
  double mean = 0.;
  double sum = 0.;
  int count = 0;

  std::auto_ptr<te::graph::MemoryIterator> it(new te::graph::MemoryIterator(graph.get()));

  te::graph::Vertex* v = it->getFirstVertex();

  while(!it->isVertexIteratorAfterEnd())
  {
    std::string strValue = v->getAttributes()[attrIdx]->toString();

    ++count;
    sum += atof(strValue.c_str());

    v = it->getNextVertex();
  }

  mean = sum / count;

  return mean;
}

double CalculateVariance(boost::shared_ptr<te::graph::DirectedGraph> graph, int stdDevZIdx, double mean)
{
  double ssd = 0.; //sum of squares of desviation
  int count = 0;

  std::auto_ptr<te::graph::MemoryIterator> it(new te::graph::MemoryIterator(graph.get()));

  te::graph::Vertex* v = it->getFirstVertex();

  while(!it->isVertexIteratorAfterEnd())
  {
    te::dt::SimpleData<double, te::dt::DOUBLE_TYPE>* sd = dynamic_cast< te::dt::SimpleData<double, te::dt::DOUBLE_TYPE>* >(v->getAttributes()[stdDevZIdx]);

    double stdDevZ = sd->getValue();

    ssd += pow((stdDevZ - mean), 2);

    ++count;

    v = it->getNextVertex();
  }

  double variance = ssd / count;

  return variance;
}
