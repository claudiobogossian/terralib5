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
  \file terralib/dataaccess/dataset/DataSetCapabilities.h

  \brief A class that informs what the dataset implementation of a given data source can perform.
*/

#ifndef __TERRALIB_DATAACCESS_INTERNAL_DATASETCAPABILITIES_H
#define __TERRALIB_DATAACCESS_INTERNAL_DATASETCAPABILITIES_H

// TerraLib
#include "../Config.h"

namespace te
{
  namespace da
  {
    /*!
      \class DataSetCapabilities

      \brief A class that informs what the dataset implementation of a given data source can perform.

      \ingroup dataaccess
    */
    class TEDATAACCESSEXPORT DataSetCapabilities
    {
      public:

        /*! \brief Constructor. */
        DataSetCapabilities();

        /*! \brief Destructor. */
        ~DataSetCapabilities();

      public:

        bool supportsBidirectionalTraversing() const;

        void setSupportBidirectionalTraversing(const bool& support);

        bool supportsRandomTraversing() const;

        void setSupportRandomTraversing(const bool& support);

        bool supportsIndexedTraversing() const;

        void setSupportIndexedTraversing(const bool& support);

        bool supportsEfficientMovePrevious() const;

        void setSupportEfficientMovePrevious(const bool& support);

        bool supportsEfficientMoveBeforeFirst() const;

        void setSupportEfficientMoveBeforeFirst(const bool& support);

        bool supportsEfficientMoveLast() const;

        void setSupportEfficientMoveLast(const bool& support);

        bool supportsEfficientMoveAfterLast() const;

        void setSupportEfficientMoveAfterLast(const bool& support);

        bool supportsEfficientMove() const;

        void setSupportEfficientMove(const bool& support);

        bool supportsEfficientDataSetSize() const;

        void setSupportEfficientDataSetSize(const bool& support);

        void setSupportAll();

      private:

        bool m_bidirectional;            //!< A flag that indicates if the data set supports traversing in a bidirectional way.
        bool m_random;                   //!< A flag that indicates if the data set supports traversing in a random way.
        bool m_indexed;                  //!< A flag that indicates if the data set supports traversing using a given key.
        bool m_efficientMovePrevious;    //!< A flag that indicates if the performance of move previous operation has no penality.
        bool m_efficientMoveBeforeFirst; //!< A flag that indicates if the performance of move before first operation has no penality.
        bool m_efficientMoveLast;        //!< A flag that indicates if the performance of move last operation has no penality.
        bool m_efficientMoveAfterLast;   //!< A flag that indicates if the performance of move after last operation has no penality.
        bool m_efficientMove;            //!< A flag that indicates if the performance of move operation has no penality.
        bool m_efficientSize;            //!< A flag that indicates if the performance of getting data set size operation has no penality.
     };

  } // end namespace da
}   // end namespace te

#endif  // __TERRALIB_DATAACCESS_INTERNAL_DATASETCAPABILITIES_H
