// TerraLib
#include <terralib/dataaccess.h>
#include <terralib/stmemory.h>
#include <terralib/st.h>
#include <terralib/datatype.h>

// Examples
#include "STExamples.h"

// STL
#include <iostream>

void LoadTrajectoryDataSetFromKML(boost::ptr_vector<te::st::TrajectoryDataSet>& output)
{
  try
  { 
    //Indicates the data source
    te::da::DataSourceInfo dsinfo;

    std::map<std::string, std::string> connInfo;
    connInfo["URI"] = ""TERRALIB_DATA_DIR"/st/trajectory/t_40_41.kml" ; 
    dsinfo.setConnInfo(connInfo);
    dsinfo.setType("OGR");

    //It creates a new Data Source and put it into the manager
    CreateDataSourceAndUpdateManager(dsinfo);

    //Indicates how the trajectories are stored in the data source -> This structure is fixed for OGR driver
    //int phTimeIdx = 3;  /* property name: timestamp */
    std::string phTimeName = "timestamp";
    //int geomIdx = 12;    /* property name: geom */
    std::string geomName = "geom";

    //Use the STDataLoader to create a TrajectoryDataSet with all observations
    te::st::TrajectoryDataSetInfo tjinfo40(dsinfo, "40: locations", phTimeName, geomName, "", "40");
    
    std::auto_ptr<te::st::TrajectoryDataSet> tjDS40 = te::st::STDataLoader::getDataSet(tjinfo40);
    
    //Use the STDataLoader to create a TrajectoryDataSet with all observations
    te::st::TrajectoryDataSetInfo tjinfo41(dsinfo, "41: locations", phTimeName, geomName, "", "41");
    std::auto_ptr<te::st::TrajectoryDataSet> tjDS41 = te::st::STDataLoader::getDataSet(tjinfo41);
    
    //Print the spatial and temporal extent as well as the observations of the loaded trajectories
    PrintTrajectoryDataSet(tjDS40.get());
    PrintTrajectoryDataSet(tjDS41.get());

    //Insert into the result container
    tjDS40->moveBeforeFirst();
    output.push_back(tjDS40);
    tjDS41->moveBeforeFirst();
    output.push_back(tjDS41);
  
    //Use the STDataLoader to create a TrajectoryDataSet with the observations during a given period
    te::dt::TimeInstant time1(te::dt::Date(2008,01,01), te::dt::TimeDuration(0,0,0));
    te::dt::TimeInstant time2(te::dt::Date(2008,03,31), te::dt::TimeDuration(23,59,59));
    te::dt::TimePeriod period(time1, time2); 

    std::auto_ptr<te::st::TrajectoryDataSet> tjDS40period = te::st::STDataLoader::getDataSet(tjinfo40, period, te::dt::DURING);
    std::auto_ptr<te::st::TrajectoryDataSet> tjDS41period = te::st::STDataLoader::getDataSet(tjinfo41, period, te::dt::DURING);

    //Print the spatial and temporal extent as well as the observations of the loaded trajectories
    //PrintTrajectoryDataSet(tjDS40period.get());
    //PrintTrajectoryDataSet(tjDS41period.get()); 
  
    //Insert into the result container
    tjDS40period->moveBeforeFirst();
    output.push_back(tjDS40period);
    tjDS41period->moveBeforeFirst();
    output.push_back(tjDS41period);
  }
  catch(const std::exception& e)
  {
    std::cout << std::endl << "An exception has occurred in TrajectoryExamplesFromKML: " << e.what() << std::endl;
  }
  catch(...)
  {
    std::cout << std::endl << "An unexpected exception has occurred in TrajectoryExamplesFromKML!" << std::endl;
  }
}

