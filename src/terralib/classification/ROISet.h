/*  Copyright (C) 2008-2013 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/classification/ROISet.h

  \brief A ROISet is a set of ROI's.
*/

#ifndef __TERRALIB_CLASSIFICATION_INTERNAL_ROISET_H
#define __TERRALIB_CLASSIFICATION_INTERNAL_ROISET_H

// TerraLib
#include "ROI.h"
#include "Config.h"

// STL
#include <map>
#include <string>

namespace te
{
  namespace da
  {
    class DataSet;
    class DataSetType;
  }

  namespace cl
  {
    /*!
      \brief A ROISet is a set of ROI's.

      \sa ROI
    */
    class TECLEXPORT ROISet
    {
      public:

        /*! \brief Default constructor. */
        ROISet();

        /*! \brief Virtual destructor. */
        virtual ~ROISet();

      public:

        /*! 
          \brief Gets a ROI from this set.

          \param label The label of the ROI to be selected.
        */
        te::cl::ROI* getROI(std::string label);

        /*! 
          \brief Add a new ROI to this set.

          \param roi  A pointer to a ROI object.
        */
        void addROI(te::cl::ROI* roi);

        /*! 
          \brief Removes a ROI from this set.

          \param label The label of the ROI to be removed.
        */
        void removeROI(std::string label);

        /*! \brief Get the roi set map. */
        std::map<std::string, te::cl::ROI*>& getROISet();

         /*! 
          \brief Export the ROISet to a shapefile

          \param fileName The file name path.

          \param srid The spatial reference system used to export the ROI's.
        */
        void exportToFile(std::string fileName, int srid);

         /*! 
          \brief Imports the ROISet from a dataset

          \param ds The dataset with the ROISet information

          \return If the dataset has a valid ROISet information, a valid ROISet pointer will be returned,
                  in other case, a NULL pointer will be returned.
        */
        static te::cl::ROISet* createROISet(std::auto_ptr<te::da::DataSet> ds);

      protected:

        /*! \brief Creates a dataset type that defines a roi. */
        std::auto_ptr<te::da::DataSetType> getDataSetType();

        /*! \brief Creates a dataset that with the roi set information. */
        std::auto_ptr<te::da::DataSet> getDataSet();

      private:

        std::map<std::string, te::cl::ROI*> m_roiMap; //!< The map of ROI's.
    };
  } // end namespace cl
}   // end namespace te


#endif  // __TERRALIB_CLASSIFICATION_INTERNAL_ROISET_H
