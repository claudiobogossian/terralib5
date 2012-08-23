#include "FileChooser.h"
#include <ui_FileChooser.h>

//Qt include files
#include <QFileDialog>
#include <QLineEdit>

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      FileChooser::FileChooser(QWidget* parent) :
      QWidget(parent),
        m_ui(new Ui::FileChooser)
      {
        m_ui->setupUi(this);
        m_ui->m_ChooseFileToolButton->setIcon(QIcon(":terralib/png/32x32/folder.png"));
      }

      FileChooser::~FileChooser()
      {
        delete m_ui;
      }

      void FileChooser::setInitialPath(const QString& path)
      {
        m_searchPath = path;
      }

      void FileChooser::setFilterPattern(const QString& filter)
      {
        m_filterPattern = filter;
        if(!m_filterPattern.contains("*.*"))
          m_filterPattern += tr(";;All files (*.*)");
      }

      QString FileChooser::getSelectedFile() const
      {
        return m_ui->m_fileName->text();
      }

      void FileChooser::onChooseFileToolButtonClicked()
      {
        QString fName = QFileDialog::getOpenFileName(parentWidget(), tr("Open file"), m_searchPath, m_filterPattern );

        if(!fName.isEmpty())
          m_ui->m_fileName->setText(fName);
      }
    }
  }
}
