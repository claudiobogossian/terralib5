/*  Copyright (C) 2008 National Institute For Space Research (INPE) - Brazil.

    This file is part of the TerraLib - a Framework for building GIS enabled applications.

    TerraLib is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License,
    or (at your option) any later version.

    TerraLib is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with TerraLib. See COPYING. If not, write to
    TerraLib Team at <terralib-team@terralib.org>.
 */

/*!
  \file terralib/sa/core/SamplePointsGeneratorAbstract.h

  \brief This file contains a virtual class to generate samples points.

  \reference Adapted from TerraLib4.
*/

#ifndef __TERRALIB_SA_INTERNAL_SAMPLEPOINTSGENERATORABSTRACT_H
#define __TERRALIB_SA_INTERNAL_SAMPLEPOINTSGENERATORABSTRACT_H

// Terralib Includes
#include "../../dataaccess/datasource/DataSource.h"
#include "../Config.h"
#include "../Enums.h"

// STL Includes
#include <memory>

//BOOST
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_real_distribution.hpp>

namespace te
{
  //forward declarations
  namespace da  { class DataSetType; }
  namespace gm  { class Envelope; }
  namespace gm  { class Point; }
  namespace mem { class DataSet; }

  namespace sa
  {
    /*!
      \class SamplePointsGeneratorAbstract

      \brief Virtual class to generate samples points.

    */
    class TESAEXPORT SamplePointsGeneratorAbstract
    {
      public:

        /*! \brief Default constructor. */
        SamplePointsGeneratorAbstract();

        /*! \brief Virtual destructor. */
        virtual ~SamplePointsGeneratorAbstract();

      public:

        /*! \brief Function to execute the kernel operation. */
        void execute();

        /*! Function used to set the envelope parameter */
        void setSRID(int srid);

        /*! Function used to set the envelope parameter */
        void setEnvelope(te::gm::Envelope env);

        /*! Function used to set the output dataset name parameter */
        void setOutputDataSetName(std::string dataSetName);

        /*! Function used to set the output data source parameter */
        void setOutputDataSource(te::da::DataSourcePtr ds);

      protected:

        /*! Function used to create the output dataset type */
        virtual std::auto_ptr<te::da::DataSetType> createDataSetType() = 0;

        /*! Function used to create the output data */
        virtual std::auto_ptr<te::mem::DataSet> generateSamples(te::da::DataSetType* dsType) = 0;

        /*! Function used to save the output dataset */
        void saveDataSet(te::mem::DataSet* dataSet, te::da::DataSetType* dsType);

        /*! Function used to generate a randon point inside a defined envelope */
        te::gm::Point* getPoint(const te::gm::Envelope* env);

      protected:

        int m_srid;                                  //!< Attribute with spatial reference information

        te::gm::Envelope m_env;                      //!< Attribute used to restrict the area to generate the samples.

        te::da::DataSourcePtr m_ds;                  //!< Pointer to the output datasource.

        std::string m_outputDataSetName;             //!< Attribute that defines the output dataset name

        te::sa::SamplePointsGeneratorType m_type;    //!< Generator Type.

        boost::random::mt19937 m_gen;

        boost::random::uniform_real_distribution<> m_distReal;
    };
  } // end namespace sa
} // end namespace te


#endif // __TERRALIB_SA_INTERNAL_SAMPLEPOINTSGENERATORABSTRACT_H