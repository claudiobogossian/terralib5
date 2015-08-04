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
\file terralib/edit/qt/core/EditionManager.h

\brief
*/

#ifndef __TERRALIB_EDIT_QT_INTERNAL_EDITIONMANAGER_H
#define __TERRALIB_EDIT_QT_INTERNAL_EDITIONMANAGER_H

// TerraLib
#include "../../../geometry/Envelope.h"
#include "../../../maptools/AbstractLayer.h"
#include "../../../qt/widgets/tools/AbstractTool.h"
#include "../../RepositoryManager.h"
#include "../Config.h"

// Qt
#include <QPointF>

class QUndoCommand;
class QUndoStack;

//STL
#include <map>

namespace te
{

  namespace edit
  {
    // Forward declaration
    class Feature;

    /*!
    \class EditionManager

    \brief
    */
    class TEEDITQTEXPORT EditionManager
    {

    public:

      /** @name Initializer Methods
      *  Methods related to instantiation and destruction.
      */
      //@{

      /*!
      \brief 

      \param 

      \note 
      */
      EditionManager();
      /*! \brief Destructor. */
      ~EditionManager();

      /*!
      \brief Method that insert command Undo/Redo of type AddCommand in the Undo/Redo stack.

      \param command command
      */
      virtual void addUndoStack(QUndoCommand* command);

      /*!
      \brief Method that return stack of Undo/Redo.

      \return stack
      */
      virtual QUndoStack* getUndoStack();

      te::edit::RepositoryManager* m_repository;

      std::map<std::string, std::size_t> m_operation;  //!< Map to see which operations.... new, update, merge...

      enum toolsOperation {
        createOp = 1, 
        updateOp, 
        removeOp
      };

    private:

      void updatecursor();

    protected:

      QUndoStack* m_undoStack;                               //!< Undo/Redo stack

    };

  }   // end namespace edit
}     // end namespace te

#endif  // __TERRALIB_EDIT_QT_INTERNAL_EDITIONMANAGER_H
