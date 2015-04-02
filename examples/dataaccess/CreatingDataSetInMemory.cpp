// Examples
#include "DataAccessExamples.h"


// TerraLib
#include <terralib/datatype.h>
//#include <terralib/dataaccess.h>
#include <terralib/memory.h>
//#include <terralib/geometry.h>


// STL
#include <iostream>

#include "boost/date_time/gregorian/gregorian.hpp"

te::da::DataSet* CreatingDataSetInMemory(const std::string& datasetName)
{
  try
  {  
    //============= (1) Create a new data set type
    
    //te::da::DataSetType* dt = new te::da::DataSetType("SoilMeasures");
    te::da::DataSetType* dt = new te::da::DataSetType(datasetName);
       
    //first property: measure_id
    te::dt::SimpleProperty* prop01 = new te::dt::SimpleProperty("id", te::dt::INT32_TYPE, true);
    prop01->setAutoNumber(true);
    
    //second property: sensor_id
    te::dt::SimpleProperty* prop02 = new te::dt::SimpleProperty("sensor", te::dt::INT32_TYPE, true);
    
    //third property: location
    te::gm::GeometryProperty* prop03 = new te::gm::GeometryProperty("location", 0, te::gm::PointType, true);
    prop03->setSRID(4326);
    
    //fourth property: measure_date
    te::dt::DateTimeProperty* prop04 = new te::dt::DateTimeProperty( "date", te::dt::DATE, true);
    
    //fifth property: measure_value
    te::dt::NumericProperty* prop05 = new te::dt::NumericProperty("value", 7, 4, true);
    
    //add properties into data set type
    dt->add(prop01);
    dt->add(prop02);
    dt->add(prop03);
    //dt->setDefaultGeomProperty(prop03);
    dt->add(prop04);
    dt->add(prop05);
    
    //define a primary key
    std::string namepk = datasetName+ "_pk";
    te::da::PrimaryKey* pk = new te::da::PrimaryKey(namepk, dt);
    pk->add(prop01);    
    
    //define a check constraint
    te::da::CheckConstraint* cc = new te::da::CheckConstraint("measurevaluecc", dt);
    cc->setExpression("measure_value>0");
    
    //create index
    te::da::Index* index01 = new te::da::Index("measure_date_idx", te::da::B_TREE_TYPE, dt); 
    index01->add(prop04);
    
    //create spatial index
    te::da::Index* index02 = new te::da::Index("measure_location_idx", te::da::R_TREE_TYPE, dt); 
    index02->add(prop03);     
    
    //============= (2) Create a new data set with 9 measures 
    
    te::mem::DataSet* ds = new te::mem::DataSet(dt);
    
    te::gm::Point* auxPoint1 = new te::gm::Point( 23.5, 78.6, 4326);
    te::mem::DataSetItem* dsItem01 = new te::mem::DataSetItem(ds);
    dsItem01->setInt32(1, 233); //sensor_id
    dsItem01->setGeometry(2, auxPoint1); //location

    boost::gregorian::date d1(boost::gregorian::greg_year(2010),boost::gregorian::greg_month(1),15);
    te::dt::DateTime* datetime1 = new te::dt::Date(d1);

    dsItem01->setDateTime(3, datetime1); //measure_date
    dsItem01->setNumeric(4, "65.89"); //measure_value
       
    te::gm::Point* auxPoint2 = new te::gm::Point( 25.7, 80.5, 4326 );  
    te::mem::DataSetItem* dsItem02 = new te::mem::DataSetItem(ds);
    dsItem02->setInt32(1, 245); //sensor_id
    dsItem02->setGeometry(2, auxPoint2); //location
    dsItem02->setDateTime(3, static_cast<te::dt::DateTime*>(datetime1->clone())); //measure_date
    dsItem02->setNumeric(4, "80.90"); //measure_value
       
    te::gm::Point* auxPoint3 = new te::gm::Point( 24.6, 83.5, 4326 );   
    te::mem::DataSetItem* dsItem03 = new te::mem::DataSetItem(ds);
    dsItem03->setInt32(1, 255); //sensor_id
    dsItem03->setGeometry(2, auxPoint3); //location
    dsItem03->setDateTime(3, static_cast<te::dt::DateTime*>(datetime1->clone())); //measure_date
    dsItem03->setNumeric(4, "87.90"); //measure_value

    boost::gregorian::date d2(boost::gregorian::greg_year(2010),boost::gregorian::greg_month(2),15);
    te::dt::DateTime* datetime2 = new te::dt::Date(d2);
    
    te::gm::Point* auxPoint4 = new te::gm::Point( 23.5, 78.6, 4326 );
    te::mem::DataSetItem* dsItem04 = new te::mem::DataSetItem(ds);
    dsItem04->setInt32(1, 233); //sensor_id
    dsItem04->setGeometry(2, auxPoint4); //location
    dsItem04->setDateTime(3, datetime2); //measure_date
    dsItem04->setNumeric(4, "90.89"); //measure_value
       
    te::gm::Point* auxPoint5 = new te::gm::Point( 25.7, 80.5, 4326 ); 
    te::mem::DataSetItem* dsItem05 = new te::mem::DataSetItem(ds);
    dsItem05->setInt32(1, 245); //sensor_id
    dsItem05->setGeometry(2, auxPoint5); //location
    dsItem05->setDateTime(3, static_cast<te::dt::DateTime*>(datetime2->clone())); //measure_date
    dsItem05->setNumeric(4, "73.90"); //measure_value
       
    te::gm::Point* auxPoint6 = new te::gm::Point( 24.6, 83.5, 4326 );  
    te::mem::DataSetItem* dsItem06 = new te::mem::DataSetItem(ds);
    dsItem06->setInt32(1, 255); //sensor_id
    dsItem06->setGeometry(2, auxPoint6); //location
    dsItem06->setDateTime(3, static_cast<te::dt::DateTime*>(datetime2->clone())); //measure_date
    dsItem06->setNumeric(4, "45.90"); //measure_value

    boost::gregorian::date d3(boost::gregorian::greg_year(2010),boost::gregorian::greg_month(3),15);
    te::dt::DateTime* datetime3 = new te::dt::Date(d3);
    
    te::gm::Point* auxPoint7 = new te::gm::Point( 23.5, 78.6, 4326 ); 
    te::mem::DataSetItem* dsItem07 = new te::mem::DataSetItem(ds);
    dsItem07->setInt32(1, 233); //sensor_id
    dsItem07->setGeometry(2, auxPoint7); //location
    dsItem07->setDateTime(3, datetime3); //measure_date
    dsItem07->setNumeric(4, "76.89"); //measure_value
       
    te::gm::Point* auxPoint8 = new te::gm::Point( 25.7, 80.5, 4326 );  
    te::mem::DataSetItem* dsItem08 = new te::mem::DataSetItem(ds);
    dsItem08->setInt32(1, 245); //sensor_id
    dsItem08->setGeometry(2, auxPoint8); //location
    dsItem08->setDateTime(3,static_cast<te::dt::DateTime*>(datetime3->clone())); //measure_date
    dsItem08->setNumeric(4, "78.15"); //measure_value
       
    te::gm::Point* auxPoint9 = new te::gm::Point( 24.6, 83.5, 4326 );  
    te::mem::DataSetItem* dsItem09 = new te::mem::DataSetItem(ds);
    dsItem09->setInt32(1, 255); //sensor_id
    dsItem09->setGeometry(2, auxPoint9); //location
    dsItem09->setDateTime(3, static_cast<te::dt::DateTime*>(datetime3->clone())); //measure_date
    dsItem09->setNumeric(4, "50.32"); //measure_value
    
    //acrescentar esse metodo no data set em memoria!!!!    
    ds->add(dsItem01); 
    ds->add(dsItem02);
    ds->add(dsItem03);
    ds->add(dsItem04);
    ds->add(dsItem05);
    ds->add(dsItem06);
    ds->add(dsItem07);
    ds->add(dsItem08);
    ds->add(dsItem09);
   
    return ds;
      
  }
  catch(const std::exception& e)
  {
    std::cout << std::endl << "An exception has occurred when Creating DataSet In Memory Example: " << e.what() << std::endl;
    return 0;
  }
  catch(...)
  {
    std::cout << std::endl << "An unexpected exception has occurred when Creating DataSet In Memory Example!" << std::endl;
    return 0;
  }
}


