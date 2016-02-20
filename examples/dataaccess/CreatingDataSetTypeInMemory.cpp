// Examples
#include "DataAccessExamples.h"

// TerraLib
#include <terralib/dataaccess.h>
#include <terralib/datatype.h>
#include <terralib/geometry.h>

te::da::DataSetType* CreateDataSetTypeInMemory(const std::string& datasettypename)
{
  //============= (1) Create a new data set type in memory
    
  //te::da::DataSetType* dt = new te::da::DataSetType("SoilMeasures");
  te::da::DataSetType* dt = new te::da::DataSetType(datasettypename);
       
  //first property: measure_id
  te::dt::SimpleProperty* prop01 = new te::dt::SimpleProperty("id", te::dt::INT32_TYPE, true);
  prop01->setAutoNumber(true);
    
  //second property: sensor_id
  te::dt::SimpleProperty* prop02 = new te::dt::SimpleProperty("sensor", te::dt::INT32_TYPE, true);
    
  //third property: location
  te::gm::GeometryProperty* prop03 = new te::gm::GeometryProperty("location", 0, te::gm::PointType, true);
  prop03->setSRID(4326);
    
  //fourth property: measure_date
  te::dt::DateTimeProperty* prop04 = new te::dt::DateTimeProperty("date", te::dt::DATE, true);
    
  //fifth property: measure_value
  te::dt::NumericProperty* prop05 = new te::dt::NumericProperty("value", 7, 4, true);
    
  //add properties into data set type
  dt->add(prop01);
  dt->add(prop02);
  dt->add(prop03);
  dt->add(prop04);
  dt->add(prop05);
    
  //define a primary key
  std::string namepk = datasettypename+ "_pk";
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
    
  return dt;
}

