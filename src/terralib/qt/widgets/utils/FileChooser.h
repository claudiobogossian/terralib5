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
  \file FileChooser.h

  \brief Defines a component for choose a file.
*/
#ifndef __TERRALIB_QT_WIDGETS_INTERNAL_FILECHOOSER_H
#define __TERRALIB_QT_WIDGETS_INTERNAL_FILECHOOSER_H

//TerraLib include files.
#include <terralib/qt/widgets/Config.h>

//Qt include files.
#include <QWidget>
#include <QModelIndex>

//Forward declarations
namespace Ui
{
  class FileChooser;
}

class QFileSystemModel;

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      /*!
        \class FileChooser

        \brief Defines a component for choose a file.

        This class is used for choosing a file. Its also possible to inform the file extensions to be searched. This way the
        client can change the filter for the files to be listed when searching. The component is composed of a line edit containing the
        complete path of the file selected and a tool button used to open de choose file dialog.

        \ingroup widgets
      */
      class TEQTWIDGETSEXPORT FileChooser : public QWidget
      {
        Q_OBJECT

      public:
        /*!
          \enum ResourceType
          \brief Type of resource required.
        */
        enum ResourceType
        {
          FILE,   //!< Search files.
          FOLDER  //!< Search folders.
        };

        /*!
          \brief Constructor.
          \param parent The QWidget parent argumnt.
        */
        FileChooser(QWidget* parent=0, const ResourceType& type=FILE);

        /*!
          \brief Destructor.
        */
        ~FileChooser();

        /*!
          \brief This will open the dialog of file selection in the given \a path.
          \param path Path to open when initing a file search.
        */
        void setInitialPath(const QString& path);

        /*!
          \brief Defines the filter for files of interest.

          This can be used to restrict the seach for different kinds of files.  
          The client must use something like "Images(*.png *.bmp) Text files(*.txt)".

          Example:
          \code{.cpp}
          FileChooser ch;

          QString filter(tr("Images(*.png *.bmp) Text files(*.txt)");
          ch.setFilterPattern(filter);
          \endcode
          In the above example two filters are created, for images and text files.
          \param filter The filter pattern. For details of syntax, see QFileDialog documentation.
        */
        void setFilterPattern(const QString& filter);

        /*!
          \brief Returns the text presented in line edit.
          \return The complete path to the selected file.
        */
        QString getSelectedResource() const;

        /*!
          \brief Updates the resource type of the search to be done.
          \param type Resource type.
        */
        void setResourceType(const ResourceType& type);

        /*!
          \brief Sets the label to be presented.
          \param label The new label.
        */
        void setLabel(const QString& label);

      signals:

        /*!
          \brief Emit a qt signal to inform that a resource was selected or defined.
          \param s Resource Name.
        */
        void resourceSelected(const QString&);

      protected slots:
        
        /*!
          \brief Shows the file selection dialog.
        */
        void onChooseFileToolButtonClicked();

        /*!
          \brief Event on return pressed over the resource line edit.
        */
        void onReturnPressed();

      protected:

        QFileSystemModel* m_fp_model;  //!< Model to show full path.
        QString m_filter;               //!< Files filter.

      private:

        Ui::FileChooser* m_ui;    //!< Qt ui pointer.

        /*! 
          \name Copy methods.
          No copy allowed.
        */
        //@{

        /*!
          \brief Copy constructor.
        */
        FileChooser(const FileChooser&);

        /*!
          \brief Copy operator.
        */
        FileChooser& operator=(const FileChooser&);
        //@}
      };
    }
  }
}

#endif //__TERRALIB_QT_WIDGETS_INTERNAL_FILECHOOSER_H
