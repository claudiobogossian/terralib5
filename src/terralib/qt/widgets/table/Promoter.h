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
  \file terralib/qt/widgets/table/Promoter.h

  \brief Defines an mechanism for logical ordering of rows.
*/
#ifndef __TERRALIB_QT_WIDGETS_TABLE_INTERNAL_PROMOTER_H
#define __TERRALIB_QT_WIDGETS_TABLE_INTERNAL_PROMOTER_H

// TerraLib
#include "../../../common/Comparators.h"

// STL
#include <map>
#include <vector>
#include <string>

// Forward declarations
namespace te
{
  namespace da
  {
    class ObjectId;
    class ObjectIdSet;
    class DataSet;
  }
}


struct ObjectIdComparator
{
  bool operator () (te::da::ObjectId* lhs, te::da::ObjectId* rhs);
};

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      /*!
        \class Promoter

        \brief A class used for logical ordering of rows.

        The promotion just group a set of ObjectIds on the begining of table. When enable promotion requires some time to preproccess the primary
        keys of each data on the DataSet. Next time promotion is instantaneous.

        \note When enabled, promotion consumes additional memory, because each primary key is stored.
      */
      class Promoter
      {
        public:

          /*!
            \brief Constructor.
          */
          Promoter ();

          /*!
            \brief Destructor.
          */
          ~Promoter();

          /*!
            \brief Returns the rows to its original positions.
          */
          void resetPromotion();

          /*!
            \breif Cleans pre processed keys and vector of mapping logical rows.
          */
          void cleanLogRowsAndProcessKeys();

          /*!
            \brief Proccess primary keys and stores it.

            \param dset Dataset to be used.

            \param pkeys Positions of the primary keys columns.

            \note This operation DOES NOT take the ownership of \a dset.
          */
          void preProcessKeys(te::da::DataSet* dset, const std::vector<size_t>& pkeys);

          /*!
            \brief Returns the logical position of the row \a visualRow.

            \param visualRow The visible row.
          */
          size_t getLogicalRow(const size_t& visualRow);

          /*!
            \brief Cleans the storage of the primary keys. Next time enabled, will be necessary to proccess all primary keys again.
          */
          void cleanPreproccessKeys();

          /*!
            \brief Promotes the rows identified by \a oids primary keys.

            \param oids Set of object ids to be promoted.
          */
          void promote(const te::da::ObjectIdSet* oids);

          /*!
            \brief Given an object id returns its row.

            \param oid Identifier of some row.

            \return Position of the row identified by \a oid.
          */
          size_t map2Row(te::da::ObjectId* oid);

        protected:

          std::map<std::string, size_t> m_PkeysRows; //!< Map object id to its position in DataSet.

          std::vector<size_t> m_logicalRows;                                                        //!< Map original row to logical postion.
      };
    }
  }
}

#endif //__TERRALIB_QT_WIDGETS_TABLE_INTERNAL_PROMOTER_H
