#include "FullPathFileModel.h"

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      FullPathFileModel::FullPathFileModel(QObject* parent) :
      QFileSystemModel(parent)
      {
      }

      QVariant FullPathFileModel::data(const QModelIndex& index, int role) const
      {
         if (role == Qt::DisplayRole && index.column() == 0) {
             QString path  = QDir::toNativeSeparators(filePath(index));
             if (path.endsWith(QDir::separator()))
                 path.chop(1);
             return path;
         }

         return QFileSystemModel::data(index, role);
      }
    }
  }
}