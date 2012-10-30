/*  Copyright (C) 2011-2011 National Institute For Space Research (INPE) - Brazil.

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
  \file ScriptWidget.h

  \brief A widget that can be used to show and control he execution of a script.
*/

#ifndef __TERRALIB_QT_WIDGETS_INTERNAL_SCRIPTWIDGET_H
#define __TERRALIB_QT_WIDGETS_INTERNAL_SCRIPTWIDGET_H

// TerraLib
#include "../Config.h"

// Qt
#include <QtGui/QWidget>

// Forward declaration
class QString;

// Forward declaration
class QsciLexer;
class QsciScintilla;

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      /*!
        \class ScriptWidget

        \brief A widget that can be used to show and control he execution of a script.
      */
      class TEQTWIDGETSEXPORT ScriptWidget : public QWidget
      {
        Q_OBJECT

        public:      

          ScriptWidget(QWidget* parent);

          ~ScriptWidget();

          QsciScintilla* getTxtEditor() { return m_txtEditor; }

          const QsciScintilla* getTxtEditor() const  { return m_txtEditor; }

          QString* getFileName() { return m_fileName; }

          const QString* getFileName() const { return m_fileName; }

          QString getScriptType() const;

          QString getScriptName() const;

          void open(const QString& fileName);

          bool hasChanged() const { return m_hasChanged; }

        public slots:      

          void save();

          void saveAs(); 

          void close();

          void execute();

          void stop();

          void pause();

          void zoomIn();

          void zoomOut();

          void setTextChanged();

        protected:

          void saveFile(const QString& fileName);

          void setLexer();

        private:

          QsciScintilla* m_txtEditor;
          QsciLexer* m_lexer;           //!< The text editor owns the lexer.
          QString* m_fileName;
          bool m_hasChanged;
      };

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_INTERNAL_SCRIPTWIDGET_H

