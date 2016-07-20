/*
  Copyright (C) 2008 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/core/plugin.h

  \brief Include files for Core Plugin Library.
*/

#ifndef __TERRALIB_CORE_TERRALIB_H__
#define __TERRALIB_CORE_TERRALIB_H__

// TerraLib

namespace te
{
  namespace core
  {

    /*!
      \class TerraLib

      \brief An utility class to control the startup and cleanup of the TerraLib Platform and its resources.
     */
    class TerraLib
    {
      public:

        /*!
          \brief It initializes the TerraLib Platform.
         */
        void initialize();

        /*!
          \brief It finalizes the TerraLib Platform.
         */
        void finalize();

        /*!
          \brief It returns a reference to the singleton instance.

          \return A reference to the singleton instance.
         */
        static TerraLib& instance();

      protected:

        /*! \brief Singleton constructor must be private or protected. */
        TerraLib(){}

        /*! \brief Singleton destructor must be private or protected. */
        ~TerraLib(){}

        /*! \brief Singleton copy constructor must be private or protected. */
        TerraLib(const TerraLib&); // no copy allowed

        /*! \brief Singleton copy assignment operator must be private or protected. */
        TerraLib& operator=(const TerraLib&); // no copy allowed
    };
  }
}

#endif // __TERRALIB_CORE_TERRALIB_H__
