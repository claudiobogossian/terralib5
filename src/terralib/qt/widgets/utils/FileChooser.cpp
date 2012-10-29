#include "FileChooser.h"
#include <ui_FileChooser.h>
//#include "FullPathFileModel.h"

//Qt include files
#include <QFileDialog>
#include <QLineEdit>
#include <QCompleter>
#include <QFileSystemModel>

QStringList getExtensionFilters(const QString& filter)
{
  QStringList filters;

  QRegExp re("(\\*\\.{1}[^\\.]\\S*[^\\)|^\\) ])");

  int pos = 0;

  while ((pos = re.indexIn(filter, pos)) != -1) 
  {
    filters <<re.cap(1);
    pos += re.matchedLength();
  }

  return filters;
}

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      FileChooser::FileChooser(QWidget* parent, const ResourceType& type) :
      QWidget(parent),
        m_ui(new Ui::FileChooser)
      {
        m_ui->setupUi(this);
        m_ui->m_ChooseFileToolButton->setIcon(QIcon::fromTheme("terralib_logo"));
//        m_ui->m_ChooseFileToolButton->setIcon(QIcon::fromTheme("folder").pixmap(m_ui->m_ChooseFileToolButton->iconSize()));

        connect(m_ui->m_fileName, SIGNAL(returnPressed()), this, SLOT(onReturnPressed()));

        QCompleter* cmp = new QCompleter(this);
        m_fp_model = new QFileSystemModel(cmp);
        m_fp_model->setNameFilterDisables(false);

        cmp->setModel(m_fp_model);
        m_ui->m_fileName->setCompleter(cmp);

        setInitialPath("");
        setResourceType(type);
      }

      FileChooser::~FileChooser()
      {
        delete m_ui;
      }

      void FileChooser::setInitialPath(const QString& path)
      {
        m_fp_model->setRootPath(path);
        m_ui->m_fileName->setText(path);
      }

      void FileChooser::setFilterPattern(const QString& filter)
      {
        m_filter = filter;
        m_fp_model->setNameFilters(getExtensionFilters(m_filter));
      }

      QString FileChooser::getSelectedResource() const
      {
        return m_ui->m_fileName->text();
      }

      void FileChooser::setResourceType(const ResourceType& type)
      {
        QDir::Filters filters;

        switch(type)
        {
          case FILE:
            filters = QDir::AllDirs | QDir::AllEntries | QDir::Drives;
          break;

          case FOLDER:
            filters = QDir::AllDirs | QDir::Dirs | QDir::Drives | QDir::NoDotAndDotDot;
          break;
        };

        m_fp_model->setFilter(filters);
      }

      void FileChooser::onChooseFileToolButtonClicked()
      {
        bool openFile = (m_fp_model->filter() & QDir::AllEntries);
        QString fName = (openFile) ? 
          QFileDialog::getOpenFileName(parentWidget(), tr("Choose file"), getSelectedResource(), m_filter) :
          QFileDialog::getExistingDirectory(parentWidget(), tr("Choose directory"), getSelectedResource());

        if(!fName.isEmpty())
        {
          m_ui->m_fileName->setText(fName);
          emit resourceSelected(fName);
        }
      }

      void FileChooser::onReturnPressed()
      {
        if(!m_ui->m_fileName->text().isEmpty())
          emit resourceSelected(m_ui->m_fileName->text());
      }
    }
  }
}
