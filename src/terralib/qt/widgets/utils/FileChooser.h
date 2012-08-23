/*  Copyright (C) 2011-2012 National Institute For Space Research (INPE) - Brazil.

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
  \details This class is used for choosing a file. Its also possible to inform the file extensions to be searched. This way the
  client can change the filter for the files to be listed when searching. The component is composed of a line edit containing the
  complete path of the file selected and a tool button used to open de choose file dialog.
  \version 5.0
  \author Frederico Augusto Bed&ecirc; &lt;frederico.bede@funcate.org.br&gt;
  \date 2001-2012
  \copyright GNU Lesser General Public License.
*/
#ifndef __TERRALIB_QT_WIDGETS_INTERNAL_FILECHOOSER_H
#define __TERRALIB_QT_WIDGETS_INTERNAL_FILECHOOSER_H

//TerraLib include files.
#include <terralib/qt/widgets/Config.h>

//Qt include files.
#include <QWidget>

//Forward declarations
namespace Ui
{
  class FileChooser;
}

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      /*!
        \class FileChooser
        \copydoc FileChooser.h
      */
      class TEQTWIDGETSEXPORT FileChooser : public QWidget
      {
        Q_OBJECT

      public:

        /*!
          \brief Constructor.
          \param parent The QWidget parent argumnt.
        */
        FileChooser(QWidget* parent=0);

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
          The must use something like "Images(*.png *.bmp) Text files(*.txt)"

          Example:
          \code{.cpp}
          FileChooser ch;

          QString filter = tr("Images(*.png *.bmp) Text files(*.txt)");
          ch.setFilterPattern(filter);
          \endcode
          In the above example two filters are created, for images and text files.
          \param filter The filter pattern. For details of syntax, see QFileDialog documentation.
          \note An filter for all kinds of files are always created as the last one.
        */
        void setFilterPattern(const QString& filter);

        /*!
          \brief Returns the text presented in line edit.
          \return The complete path to the selected file.
        */
        QString getSelectedFile() const;

      protected slots:
        
        /*!
          \brief Shows the file selection dialog.
        */
        void onChooseFileToolButtonClicked();

      protected:

        QString m_filterPattern;  //!< Filter patterns to be used.
        QString m_searchPath;     //!< Initial path for searching.

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
