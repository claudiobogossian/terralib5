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
  \file ResourceChooser.h
  
  \brief Defines a component for choosing a file or directory.
*/
#ifndef __TERRALIB_QT_WIDGETS_INTERNAL_RESOURCECHOOSER_H
#define __TERRALIB_QT_WIDGETS_INTERNAL_RESOURCECHOOSER_H

#include <terralib/qt/widgets/Config.h>

//! Qt include files
#include <QWidget>

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      //! Forward declarations
      class FileChooser;

      /*!
        \class ResourceChooser
        
        \brief Defines a component for choosing a file or directory.
        
        This class is used for choosing a file or a directory. Its also possible to inform the file extensions to be searched. This way the
        client can change the filter for the files to be listed when searching. The component is composed of a line edit containing the
        complete path of the file or directory selected and a tool button used to open de choose file / directory dialog.
      */
      class TEQTWIDGETSEXPORT ResourceChooser : public QWidget
      {
        Q_OBJECT
        
      public:
        
        /*!
          \brief Constructor.
        */
        ResourceChooser(QWidget* parent=0);

        /*!
          \brief Destructor.
        */
        ~ResourceChooser();

        /*!
          \brief Returns the selected resource.
        */
        QString getSelectedResource();

        /*!
          \brief Setts a pattern for filtering the searched files.
          \details This class uses internally the FileChooser component. For more details about on how define filter pattern see its documentation.
          \param filter Pattern to be applied.
          \sa te::qt::widgets::FileChooser
        */
        void setFilterPattern(const QString& filter);

        /*!
          \brief use the static when the ResourceChooser have to be presented as a Dialog.
          \param initialPath
          \param parent
          \return 
        */
        static QString getResource(const QString& initialPath, const QString& filter, QWidget* parent=0);

      protected slots:

        /*!
          \brief This is called when the user changes the type of resource to search.
          \param id
        */
        void setResource(int id);

      protected:

        FileChooser* m_fchooser;    //!< File chooser component.
      };
    }
  }
}

#endif //! __TERRALIB_QT_WIDGETS_INTERNAL_RESOURCECHOOSER_H