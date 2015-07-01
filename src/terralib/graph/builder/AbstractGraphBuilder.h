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
  \file AbstractGraphBuilder.h

  \brief This abstract class provides the common functions for graph
    builder classes. Each builder strategy has to implement only
    a public function called build.

*/

#ifndef __TERRALIB_GRAPH_INTERNAL_ABSTRACTGRAPHBUILDER_H
#define __TERRALIB_GRAPH_INTERNAL_ABSTRACTGRAPHBUILDER_H

// Terralib Includes
#include "../Config.h"

// STL Includes
#include <string>
#include <vector>

// BOOST Includes
#include <boost/shared_ptr.hpp>

namespace te
{
  namespace graph
  {
  
  class AbstractGraph;
    /*!
      \class AbstractGraphBuilder

      \brief This abstract class provides the common functions for graph
             builder classes. Each builder strategy has to implement only
             a public function called build.

    */

    class TEGRAPHEXPORT AbstractGraphBuilder
    {
      public:

        /*! \brief Default constructor. */
        AbstractGraphBuilder();

        /*! \brief Virtual destructor. */
        virtual ~AbstractGraphBuilder();

        /** @name Abstract Methods
        *  Method common to each graph builder
        */
        //@{

        /*! \brief Get error message. */
        std::string getErrorMessage();
       
        /*! \brief Get generated graph. */
        boost::shared_ptr<AbstractGraph> getGraph();

        //@}

      protected:

        boost::shared_ptr<AbstractGraph> m_graph;   //!< Graph object

        std::string m_errorMessage;                 //!< Error message
    };

  } // end namespace graph
} // end namespace te

#endif // __TERRALIB_GRAPH_INTERNAL_ABSTRACTGRAPHBUILDER_H