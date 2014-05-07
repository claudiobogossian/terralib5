//TerraLib
#include <terralib/graph/builder/GPMGraphBuilder.h>
#include <terralib/graph/core/AbstractGraph.h>
#include <terralib/graph/Globals.h>
#include <terralib/se.h>
#include "GraphExamples.h"
#include "DisplayWindow.h"

// STL Includes
#include <iostream>

// BOOST Includes
#include <boost/shared_ptr.hpp>

// Qt
#include <QtGui/QApplication>

te::se::Style* getGPMGraphStyle();

boost::shared_ptr<te::graph::AbstractGraph> CreateGPMAdjacencyGraph(bool draw)
{
  std::cout << std::endl << "Create GPM Adjacency Graph..." << std::endl;

// graph name
  std::string graphName = "graphGPMAdjacency";

// open data source
  std::auto_ptr<te::da::DataSource> ds = OpenOGRDataSource(TE_DATA_EXAMPLE_DIR "/data/graph/UP_pol.shp");
  std::string dataSetName = "UP_pol";
  std::string columnId = "COD_UP";

// graph type
  std::string graphType = te::graph::Globals::sm_factoryGraphTypeDirectedGraph;

// connection info
  std::map<std::string, std::string> connInfo;

// graph information
  std::map<std::string, std::string> graphInfo;
  graphInfo["GRAPH_DATA_SOURCE_TYPE"] = "MEM";
  graphInfo["GRAPH_NAME"] = graphName;
  graphInfo["GRAPH_DESCRIPTION"] = "Generated by GPM Adjacency Builder.";

  boost::shared_ptr<te::graph::AbstractGraph> graph;

// create graph
  try
  {
    te::graph::GPMGraphBuilder builder;

    if(builder.setGraphInfo(connInfo, graphType, graphInfo))
    {
      if(!builder.buildAdjacency(ds, dataSetName, columnId, true))
      {
        std::cout << std::endl << "An exception has occuried in Graph Example - CreateGPMGraph Adjacency: " << builder.getErrorMessage() << std::endl;
      }
      else
      {
        graph = builder.getGraph();
      }
    }
  }
  catch(const std::exception& e)
  {
    std::cout << std::endl << "An exception has occuried in Graph Example - CreateGPMGraph Adjacency: " << e.what() << std::endl;
  }
  catch(...)
  {
    std::cout << std::endl << "An unexpected exception has occuried in Graph Example - CreateGPMGraph Adjacency!" << std::endl;
  }

  if(draw)
  {
    // set visual
    te::se::Style* style = getGPMGraphStyle();

    std::auto_ptr<te::da::DataSource> ds = OpenOGRDataSource(TE_DATA_EXAMPLE_DIR "/data/graph/BR_Estados.shp");

    std::auto_ptr<te::gm::Envelope> ext = getDataSetExtent(ds.get(), dataSetName);

    //start qApp
    int argc = 0;
    QApplication app(argc, 0);

    DisplayWindow* w = new DisplayWindow();
    w->setPNGPrefix("gpm_adjacency_");
    w->addGraph(graph.get(), *ext.get(), style);
    w->show();

    app.exec();
  }

  return graph;
}

boost::shared_ptr<te::graph::AbstractGraph> CreateGPMDistanceGraph(bool draw)
{
  std::cout << std::endl << "Create GPM Distance Graph..." << std::endl;

// graph name
  std::string graphName = "graphGPMDistance";

// open data source
  std::auto_ptr<te::da::DataSource> ds = OpenOGRDataSource(TE_DATA_EXAMPLE_DIR "/data/graph/UP_pol.shp");
  std::string dataSetName = "UP_pol";
  std::string columnId = "COD_UP";
  double distance = 3000;

// graph type
  std::string graphType = te::graph::Globals::sm_factoryGraphTypeDirectedGraph;

// connection info
  std::map<std::string, std::string> connInfo;

// graph information
  std::map<std::string, std::string> graphInfo;
  graphInfo["GRAPH_DATA_SOURCE_TYPE"] = "MEM";
  graphInfo["GRAPH_NAME"] = graphName;
  graphInfo["GRAPH_DESCRIPTION"] = "Generated by GPM Distance Builder.";

  boost::shared_ptr<te::graph::AbstractGraph> graph;

// create graph
  try
  {
    te::graph::GPMGraphBuilder builder;

    if(builder.setGraphInfo(connInfo, graphType, graphInfo))
    {
      if(!builder.buildDistance(ds, dataSetName, columnId, distance))
      {
        std::cout << std::endl << "An exception has occuried in Graph Example - CreateGPMGraph Distance: " << builder.getErrorMessage() << std::endl;
      }
      else
      {
        graph = builder.getGraph();
      }
    }
  }
  catch(const std::exception& e)
  {
    std::cout << std::endl << "An exception has occuried in Graph Example - CreateGPMGraph Distance: " << e.what() << std::endl;
  }
  catch(...)
  {
    std::cout << std::endl << "An unexpected exception has occuried in Graph Example - CreateGPMGraph Distance!" << std::endl;
  }

  if(draw)
  {
    // set visual
    te::se::Style* style = getGPMGraphStyle();

    std::auto_ptr<te::da::DataSource> ds = OpenOGRDataSource(TE_DATA_EXAMPLE_DIR "/data/graph/BR_Estados.shp");

    std::auto_ptr<te::gm::Envelope> ext = getDataSetExtent(ds.get(), dataSetName);

    //start qApp
    int argc = 0;
    QApplication app(argc, 0);

    DisplayWindow* w = new DisplayWindow();
    w->setPNGPrefix("gpm_distance_");
    w->addGraph(graph.get(), *ext.get(), style);
    w->show();

    app.exec();
  }

  return graph;
}

te::se::Style* getGPMGraphStyle()
{
  te::se::Stroke* stroke = te::se::CreateStroke("#FFFF00", "1.0");
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
