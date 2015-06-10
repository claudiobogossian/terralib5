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
  \file terralib/sa/core/SamplePointsGeneratorStratified.h

  \brief This file contains a class to generate samples points using stratified strategy.

  \reference Adapted from TerraLib4.
*/

#ifndef __TERRALIB_SA_INTERNAL_SAMPLEPOINTSGENERATORSTRATIFIED_H
#define __TERRALIB_SA_INTERNAL_SAMPLEPOINTSGENERATORSTRATIFIED_H

// Terralib Includes
#include "../../geometry/Geometry.h"
#include "SamplePointsGeneratorAbstract.h"

// STL Includes
#include <map>
#include <memory>

//BOOST
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>

namespace te
{
  //forward declarations
  namespace da  { class DataSet; }
  namespace da  { class DataSetType; }
  namespace mem { class DataSet; }

  namespace sa
  {
    /*!
      \class SamplePointsGeneratorStratified

      \brief Class to generate samples points using stratified strategy.

    */
    class TESAEXPORT SamplePointsGeneratorStratified : public te::sa::SamplePointsGeneratorAbstract
    {
      public:

        /*! \brief Default constructor. */
        SamplePointsGeneratorStratified();

        /*! \brief Virtual destructor. */
        virtual ~SamplePointsGeneratorStratified();

      public:

        /*! Function used to set the number of points to be generated */
        void setNumberOfPoints(int nPoints);

        /*! Function used to set the input dataset */
        void setInputDataSet(std::auto_ptr<te::da::DataSet> dataSet);

        /*! Function used to set attribute from input data selected to generate the samples */
        void setInputAttributeName(std::string attrName);

        /*! Function used to set if the number of samples has to be proportional to the area */
        void isProportionalToArea(bool isProp);

        /*! Function used to get the names for the generated classes */
        std::vector<std::string> getClassNames();

      protected:

        /*! Function used to create the output dataset type */
        virtual std::auto_ptr<te::da::DataSetType> createDataSetType();

        /*! Function used to create the output data */
        virtual std::auto_ptr<te::mem::DataSet> generateSamples(te::da::DataSetType* dsType);

        /*! Function used to create the class map */
        void createClassMap();

        /*! Function used to generate a randon point inside a defined geometry */
        te::gm::Point* getPointInGeometry(te::gm::Geometry* g);

      protected:

        int m_nPoints;                                //!< Attribute used to define the number of points to be generated.

        std::auto_ptr<te::da::DataSet> m_dataSet;     //!< Attribute with input data.
        
        std::string m_attrName;                       //!< Attribute used to get from input data the selected attribute

        bool m_propToArea;                            //!< Attribute used to indicate if the number of samples has to be proportional to the area.

        std::map<std::string, std::vector<te::gm::Geometry*> > m_classMap; //!< Attribute with polygons for each class from input dataset

        boost::random::uniform_int_distribution<> m_distInt;
    };
  } // end namespace sa
} // end namespace te


#endif // __TERRALIB_SA_INTERNAL_SAMPLEPOINTSGENERATORSTRATIFIED_H