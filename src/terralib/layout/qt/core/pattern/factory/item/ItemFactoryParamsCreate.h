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
  \file ItemFactoryParamsCreate.h
   
  \brief Parameters to create a new item object (MVC graphic Object). 

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_ITEM_FACTORY_PARAMS_CREATE_H 
#define __TERRALIB_LAYOUT_INTERNAL_ITEM_FACTORY_PARAMS_CREATE_H

// TerraLib
#include "terralib/common/AbstractParameters.h"
#include "terralib/geometry/Coord2D.h"
#include "../../../../../core/Config.h"

// STL
#include <string>

namespace te
{
  namespace layout
  {
		class Properties;
    /*!
	  \brief Parameters to create a new item object (MVC graphic Object).  
	  
	  \ingroup layout

	  \sa te::common::AbstractParameters
	  */
		class TELAYOUTEXPORT ItemFactoryParamsCreate : public te::common::AbstractParameters
    {
      public:
				

				/*! \brief Copy constructor. */
				ItemFactoryParamsCreate(const ItemFactoryParamsCreate& rhs);

        /*!
          \brief Constructor

          \param 
					\param
					\param
					\param
        */ 
				ItemFactoryParamsCreate(std::string name, int zValue, int id, te::gm::Coord2D coord = te::gm::Coord2D(), Properties* props = 0);

				/*!
				\brief Constructor

				\param
				\param
				*/
				ItemFactoryParamsCreate(std::string name, int zValue, te::gm::Coord2D coord = te::gm::Coord2D());

				/*!
				\brief Constructor

				\param
				\param
				\param
				\param
				*/
				ItemFactoryParamsCreate(Properties* props);

        /*!
          \brief Destructor
        */ 
				virtual ~ItemFactoryParamsCreate();

				te::common::AbstractParameters* clone() const;

				void reset() throw(te::common::Exception);

				std::string getName();

				Properties* getProperties();

				te::gm::Coord2D getCoord();

				int getZValue();

				int getId();

			protected:

				std::string							m_name;
				Properties*							m_props; //!< Properties of a graphic object.
				te::gm::Coord2D         m_coord; //!< Coordinate of a graphic object.
				int                     m_zValue; //!< Z Value of a graphic object.
				int                     m_id; //!< Id of a graphic object.
    };
  }
}
#endif

