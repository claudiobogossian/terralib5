//TerraLib
#include "../Config.h"
#include "GraphExamples.h"
#include "DisplayWindow.h"

#include <terralib/dataaccess/utils/Utils.h>
#include <terralib/datatype/SimpleData.h>
#include <terralib/graph/core/AbstractGraph.h>
#include <terralib/graph/core/Edge.h>
#include <terralib/graph/core/GraphMetadata.h>
#include <terralib/graph/core/Vertex.h>
#include <terralib/graph/iterator/MemoryIterator.h>
#include <terralib/graph/Globals.h>
#include <terralib/se.h>
#include <terralib/sa/core/GeneralizedProximityMatrix.h>
#include <terralib/sa/core/MinimumSpanningTree.h>
#include <terralib/sa/core/SpatialWeightsExchanger.h>
#include <terralib/sa/core/Utils.h>


// STL Includes
#include <iostream>

// BOOST Includes
#include <boost/shared_ptr.hpp>

// Qt
#include <QApplication>

te::se::Style* getMSTGraphStyle();
void CreateWeightAttribute(te::graph::AbstractGraph* graph, int weightAttrIdx, std::vector<int> attrsIdx);
double CalculateWeight(std::vector<int> attrsIdx, te::graph::Vertex* vFrom, te::graph::Vertex* vTo);

void CreateMSTGraph(bool draw)
{
  std::cout << std::endl << "Create MST Graph..." << std::endl;

  //load data set
  std::string data_dir = TERRALIB_DATA_DIR;
  std::auto_ptr<te::da::DataSource> ds = OpenOGRDataSource(data_dir + "/graph/up_pol_rec_pol.shp");

  std::string dataSetName = "up_pol_rec_pol";
  std::auto_ptr<te::da::DataSet> dataSet = ds->getDataSet(dataSetName);
  std::auto_ptr<te::da::DataSetType> dataSetType = ds->getDataSetType(dataSetName);

  std::size_t geomPos = te::da::GetFirstSpatialPropertyPos(dataSet.get());

  //load gpm
  std::auto_ptr<te::sa::GeneralizedProximityMatrix> gpm;

  te::sa::SpatialWeightsExchanger swe;

  gpm.reset(swe.importFromGAL(data_dir + "/graph/up_pol_rec_gal.gal", ds.get()));

  gpm->getGraph()->getMetadata()->setSRID(29193);
  gpm->getGraph()->getMetadata()->setEnvelope(*dataSet->getExtent(geomPos));


  //calculate the mst
  std::vector<int> attrsIdx;

  std::string attrName = "Attr1";

  std::size_t idx = dataSetType->getPropertyPosition(attrName);
  int type = dataSet->getPropertyDataType(idx);
  int gpmIdx = te::sa::AssociateGPMVertexAttribute(gpm.get(), dataSet.get(), "objet_id_1", attrName, type);

  attrsIdx.push_back(gpmIdx);

  int edgeWeightIdx = te::sa::AddGraphEdgeAttribute(gpm->getGraph(), TE_SA_SKATER_ATTR_WEIGHT_NAME, te::dt::DOUBLE_TYPE);

  CreateWeightAttribute(gpm->getGraph(), edgeWeightIdx, attrsIdx);

  te::sa::MinimumSpanningTree mst(gpm->getGraph());

  te::graph::AbstractGraph* graph = mst.kruskal(edgeWeightIdx);

  graph->getMetadata()->setSRID(29193);
  graph->getMetadata()->setEnvelope(*dataSet->getExtent(geomPos));

  if(draw)
  {
    // set visual
    te::se::Style* style = getMSTGraphStyle();

    //start qApp
    int argc = 0;
    QApplication app(argc, 0);

    DisplayWindow* w = new DisplayWindow();
    w->addVectorialLayer(data_dir + "/graph/up_pol_rec_pol.shp", 29193);
    w->addGraph(graph, *dataSet->getExtent(geomPos), style);
    w->show();

    app.exec();
  }

  delete graph;
}

te::se::Style* getMSTGraphStyle()
{
  te::se::Stroke* stroke = te::se::CreateStroke("#000000", "1.0");
  te::se::LineSymbolizer* lineSymbolizer = te::se::CreateLineSymbolizer(stroke);

  te::se::Fill* markFill = te::se::CreateFill("#009900", "1.0");
  te::se::Stroke* markStroke = te::se::CreateStroke("#000000", "1");
  te::se::Mark* mark = te::se::CreateMark("circle", markStroke, markFill);
  te::se::Graphic* graphic = te::se::CreateGraphic(mark, "4", "", "");
  te::se::PointSymbolizer* pointSymbolizer = te::se::CreatePointSymbolizer(graphic);
  
  te::se::Rule* rule = new te::se::Rule;
  rule->push_back(lineSymbolizer);
  rule->push_back(pointSymbolizer);

  te::se::FeatureTypeStyle* style = new te::se::FeatureTypeStyle;
  style->push_back(rule);

  return style;
}

void CreateWeightAttribute(te::graph::AbstractGraph* graph, int weightAttrIdx, std::vector<int> attrsIdx)
{
  int size = graph->getMetadata()->getEdgePropertySize();

  te::graph::MemoryIterator* iterator = new te::graph::MemoryIterator(graph);

  te::graph::Edge* edge = iterator->getFirstEdge();

  while(edge)
  {
    te::graph::Vertex* vFrom = graph->getVertex(edge->getIdFrom());
    te::graph::Vertex* vTo = graph->getVertex(edge->getIdTo());

    if(vFrom && vTo)
    {
      double weight = CalculateWeight(attrsIdx, vFrom, vTo);

      edge->setAttributeVecSize(size);
      edge->addAttribute(weightAttrIdx, new te::dt::SimpleData<double, te::dt::DOUBLE_TYPE>(weight));
    }

    edge = iterator->getNextEdge();
  }
}

double CalculateWeight(std::vector<int> attrsIdx, te::graph::Vertex* vFrom, te::graph::Vertex* vTo)
{
  double weight = 0.;

  for(std::size_t t = 0; t < attrsIdx.size(); ++t)
  {
    double valueFrom = te::sa::GetDataValue(vFrom->getAttributes()[attrsIdx[t]]);
    double valueTo = te::sa::GetDataValue(vTo->getAttributes()[attrsIdx[t]]);

    weight += (valueTo - valueFrom) * (valueTo - valueFrom);
  }

  return sqrt(weight);
}
