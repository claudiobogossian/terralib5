#include "FileChooser.h"
#include <ui_FileChooser.h>
#include "FullPathFileModel.h"

//Qt include files
#include <QFileDialog>
#include <QLineEdit>
#include <QCompleter>


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
        m_ui->m_ChooseFileToolButton->setIcon(QIcon(":terralib/png/32x32/folder.png"));

        QCompleter* cmp = new QCompleter(this);
        m_fp_model = new FullPathFileModel(cmp);
        cmp->setCompletionMode(QCompleter::UnfilteredPopupCompletion);
        m_fp_model->setNameFilterDisables(false);

        cmp->setModel(m_fp_model);
        m_ui->m_fileName->setCompleter(cmp);

        setInitialPath("");
        setResourceType(type);

        connect(cmp, SIGNAL(activated(const QModelIndex&)), SLOT(showIndex(const QModelIndex&)));
        connect(m_ui->m_fileName, SIGNAL(textChanged(const QString&)), SLOT(showPath(const QString&)));
      }

      FileChooser::~FileChooser()
      {
        delete m_ui;
      }

      void FileChooser::setInitialPath(const QString& path)
      {
        m_fp_model->setRootPath(path);
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
        QString fName = QFileDialog::getOpenFileName(parentWidget(), tr("Open file"), getSelectedResource(), m_filter);

        if(!fName.isEmpty())
          m_ui->m_fileName->setText(fName);
      }

      void FileChooser::showIndex(const QModelIndex& idx)
      {
        m_fp_model->setRootPath(idx.data(Qt::DisplayRole).toString());
      }

      void FileChooser::showPath(const QString& path)
      {
        if(path.endsWith(QDir::separator()))
        {
          QModelIndex idx = m_fp_model->index(path);
          if(idx.isValid())
            showIndex(idx);
        }
      }
    }
  }
}
