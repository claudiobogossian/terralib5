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
  \file terralib/sa/core/SamplePointsGeneratorRandom.h

  \brief This file contains a class to generate samples points using random strategy.

  \reference Adapted from TerraLib4.
*/

#ifndef __TERRALIB_SA_INTERNAL_SAMPLEPOINTSGENERATORRANDOM_H
#define __TERRALIB_SA_INTERNAL_SAMPLEPOINTSGENERATORRANDOM_H

// Terralib Includes
#include "SamplePointsGeneratorAbstract.h"

// STL Includes
#include <memory>

namespace te
{
  //forward declarations
  namespace da  { class DataSetType; }
  namespace mem { class DataSet; }

  namespace sa
  {
    /*!
      \class SamplePointsGeneratorRandom

      \brief Class to generate samples points using random strategy.

    */
    class TESAEXPORT SamplePointsGeneratorRandom : public te::sa::SamplePointsGeneratorAbstract
    {
      public:

        /*! \brief Default constructor. */
        SamplePointsGeneratorRandom();

        /*! \brief Virtual destructor. */
        virtual ~SamplePointsGeneratorRandom();

      public:

        /*! Function used to set the number of points to be generated */
        void setNumberOfPoints(int nPoints);

      protected:

        /*! Function used to create the output dataset type */
        virtual std::auto_ptr<te::da::DataSetType> createDataSetType();

        /*! Function used to create the output data */
        virtual std::auto_ptr<te::mem::DataSet> generateSamples(te::da::DataSetType* dsType);

      protected:

        int m_nPoints;             //!< Attribute used to define the number of points to be generated.
    };
  } // end namespace sa
} // end namespace te


#endif // __TERRALIB_SA_INTERNAL_SAMPLEPOINTSGENERATORRANDOM_H