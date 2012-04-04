// Examples
#include "DataAccessExamples.h"

// TerraLib
#include <terralib/dataaccess.h>
#include <terralib/geometry.h>

// STL
#include <iostream>

void DataSetGetExtent(te::da::DataSourceTransactor* transactor)
{
// Let's get a DataSet called 'br_munic_2001' in the 'public' schema
// and retrieve its extent
  {
    te::da::DataSet* dataset = transactor->getDataSet("public.br_munic_2001");

    te::gm::Envelope* extent = dataset->getExtent();

    std::cout << std::endl
              << "public.br_munic_2001 - extent = "
              << extent->m_llx << ", "
              << extent->m_lly << ", "
              << extent->m_urx << ", "
              << extent->m_ury << std::endl;

    delete dataset;

    delete extent;
  }

// now we will retrieve all cities that contains a given point and the retrieves its extent
  {
    te::gm::Point pt( -43.6107606714293, -20.3913548070123, 4291, 0);

    te::da::DataSet* dataset = transactor->getDataSet("public.br_munic_2001", &pt, te::gm::INTERSECTS);

    te::gm::Envelope* extent = dataset->getExtent();

    std::cout << std::endl
              << "public.br_munic_2001 with pt spatial filter - extent = "
              << extent->m_llx << ", "
              << extent->m_lly << ", "
              << extent->m_urx << ", "
              << extent->m_ury << std::endl;

    delete dataset;

    delete extent;
  }

// now we will retrieve all cities and then we will refine with cities that contains a given point
  {
    te::da::DataSet* dataset = transactor->getDataSet("public.br_munic_2001");

    te::gm::Point pt(-43.6107606714293, -20.3913548070123, 4291, 0 );

    dataset->setFilter(&pt, te::gm::INTERSECTS);

    te::gm::Envelope* extent = dataset->getExtent();

    std::cout << std::endl
              << "public.br_munic_2001 refined with pt spatial filter - extent = "
              << extent->m_llx << ", "
              << extent->m_lly << ", "
              << extent->m_urx << ", "
              << extent->m_ury << std::endl;

    delete dataset;

    delete extent;
  }   

// now we will retrieve all cities that intersects a given box
  {
    te::gm::Envelope box(-43.9329795837402, -20.6328010559082, -43.4036407470703, -20.1612071990967);

    te::da::DataSet* dataset = transactor->getDataSet("public.br_munic_2001", &box, te::gm::INTERSECTS);

    te::gm::Envelope* extent = dataset->getExtent();

    std::cout << std::endl
              << "public.br_munic_2001 box spatial filter - extent = "
              << extent->m_llx << ", "
              << extent->m_lly << ", "
              << extent->m_urx << ", "
              << extent->m_ury << std::endl;

    delete dataset;

    delete extent;
  }   

// now we will retrieve all cities and then refine with a box filter
  {
    te::da::DataSet* dataset = transactor->getDataSet("public.br_munic_2001");

    te::gm::Envelope box(-43.9329795837402, -20.6328010559082, -43.4036407470703, -20.1612071990967);

    dataset->setFilter(&box, te::gm::INTERSECTS);

    te::gm::Envelope* extent = dataset->getExtent();

    std::cout << std::endl
              << "public.br_munic_2001 refined by a box spatial filter - extent = "
              << extent->m_llx << ", "
              << extent->m_lly << ", "
              << extent->m_urx << ", "
              << extent->m_ury << std::endl;

    delete dataset;

    delete extent;
  }
}

