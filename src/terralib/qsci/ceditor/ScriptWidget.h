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
  \file terralib/qsci/ceditor/ScriptWidget.h

  \brief A widget that can be used to show and control he execution of a script.
*/

#ifndef __TERRALIB_QSCI_CEDITOR_SCRIPTWIDGET_H__
#define __TERRALIB_QSCI_CEDITOR_SCRIPTWIDGET_H__

// TerraLib
#include "../Config.h"

// Qt
#include <QWidget>

// Forward declaration
class QString;

// Forward declaration
class QsciLexer;
class QsciScintilla;

namespace te
{
  namespace ce
  {
    /*!
      \class ScriptWidget

      \brief A widget that can be used to show and control the execution of a script.
     */
    class TECEDITOREXPORT ScriptWidget : public QWidget
    {
        Q_OBJECT

      public:

        /*!
          \brief ScriptWidget

          \param parent
         */
        ScriptWidget(QWidget* parent);

        /*! \brief Destructor. */
        ~ScriptWidget();

        /*!
          \brief Returns the file name of the script.

          \return File name.
         */
        QString getFileName() const { return m_fileName; }

        /*!
          \brief getScriptType

          \return Returns the type of the script based on its extension.
         */
        QString getScriptType() const;
        
        /*!
          \brief getScriptName

          \return Return the script name.
         */
        QString getScriptName() const;

        /*!
          \brief Open the code file and presents it on the screen.

          \param fileName File name.
         */
        void open(const QString& fileName);

        /*!
          \brief Save the contents code in a file.

          \param fileName Name of the file.
         */
        void save(const QString& fileName);

        /*!
          \brief Returns the flag that signals if the code was edited.

          \return Returns true or false.
         */
        bool hasChanged() const { return m_hasChanged; }

      public slots:

        /*!
          \brief Saves the current file.
         */
        void save();

        /*!
          \brief saveAs Saves the current file in a different file.
         */
        void saveAs();

        /*! \brief Closes the current script. */
        void close();

        /*! \brief Executes the current script. */
        void execute();

        /*! \brief Stops the execution of the script. */
        void stop();

        /*! \brief Pauses the execution. */
        void pause();

        /*! \brief Zoom in. */
        void zoomIn();

        /*! \brief Zoom out. */
        void zoomOut();

        /*! \brief Sets to true the textChanged flag. */
        void setTextChanged();

      protected:

        /*!
          \brief Saves the name of the file.

          \param fileName The complete path of the script.
         */
        void saveFile(const QString& fileName);

        /*! \brief setLexer. */
        void setLexer();

      private:
        
        QsciScintilla* m_txtEditor;   //!< The editor with grammatical highlighted marks.
        QsciLexer* m_lexer;           //!< The text editor owns the lexer.
        QString m_fileName;           //!< File name.
        bool m_hasChanged;            //!< Code has changed?
    };
  
  } // end namespace ce
}   // end namespace te

#endif  // __TERRALIB_QSCI_CEDITOR_SCRIPTWIDGET_H__
