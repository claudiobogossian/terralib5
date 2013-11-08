// TerraLib
#include <terralib/dataaccess.h>
#include <terralib/stmemory.h>
#include <terralib/st.h>
#include <terralib/datatype.h>

// Boost
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/filesystem.hpp>

// Examples
#include "STExamples.h"

// STL
#include <iostream>

void TrajectoryExamplesFromKML()
{
  try
  { 
    //Indicates the data source
    te::da::DataSourceInfo dsinfo;

    std::map<std::string, std::string> connInfo;
    connInfo["URI"] = ""TE_DATA_EXAMPLE_DIR"/data/kml/t_40_41.kml" ; 
    dsinfo.setConnInfo(connInfo);
    dsinfo.setType("OGR");

    //Generates a randon id to the data source
    boost::uuids::basic_random_generator<boost::mt19937> gen;
    boost::uuids::uuid u = gen();
    std::string id = boost::uuids::to_string(u);
    dsinfo.setId(id);

    //Create the data source and put it into the manager
    te::da::DataSourceManager::getInstance().open(dsinfo.getId(), dsinfo.getType(), dsinfo.getConnInfo());
    
    //Indicates how the trajectories are stored in the data source -> This structure is fixed for OGR driver
    int phTimeIdx = 3;  /* property name: timestamp */
    int geomIdx = 12;    /* property name: geom */

    //It initializes the st data loader support
    te::st::STDataLoader::initialize();

    //Use the STDataLoader to create a TrajectoryDataSet with all observations
    te::st::TrajectoryDataSetInfo tjinfo40(dsinfo, "40: locations", phTimeIdx, geomIdx, -1, "40");
    std::auto_ptr<te::st::TrajectoryDataSet> tjDS40 = te::st::STDataLoader::getDataSet(tjinfo40);

    //Use the STDataLoader to create a TrajectoryDataSet with all observations
    te::st::TrajectoryDataSetInfo tjinfo41(dsinfo, "41: locations", phTimeIdx, geomIdx, -1, "41");
    std::auto_ptr<te::st::TrajectoryDataSet> tjDS41 = te::st::STDataLoader::getDataSet(tjinfo41);

    //Print the spatial and temporal extent as well as the observations of the loaded trajectories
    PrintTrajectoryDataSet(tjDS40.get());
    PrintTrajectoryDataSet(tjDS41.get());

    //Use the STDataLoader to create a TrajectoryDataSet with the observations during a given period
    te::dt::TimeInstant time1(te::dt::Date(2008,01,01), te::dt::TimeDuration(0,0,0));
    te::dt::TimeInstant time2(te::dt::Date(2008,03,31), te::dt::TimeDuration(23,59,59));
    te::dt::TimePeriod period(time1, time2); 

    std::auto_ptr<te::st::TrajectoryDataSet> tjDS40period = te::st::STDataLoader::getDataSet(tjinfo40, period, te::dt::DURING);
    std::auto_ptr<te::st::TrajectoryDataSet> tjDS41period = te::st::STDataLoader::getDataSet(tjinfo41, period, te::dt::DURING);

    //Print the spatial and temporal extent as well as the observations of the loaded trajectories
    PrintTrajectoryDataSet(tjDS40period.get());
    PrintTrajectoryDataSet(tjDS41period.get());   

    //It finalizes the st data loader support
    te::st::STDataLoader::finalize();
  }
  catch(const std::exception& e)
  {
    std::cout << std::endl << "An exception has occurried in TrajectoryExamplesFromKML: " << e.what() << std::endl;
  }
  catch(...)
  {
    std::cout << std::endl << "An unexpected exception has occurried in TrajectoryExamplesFromKML!" << std::endl;
  }
}

