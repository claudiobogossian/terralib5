#include "Utils.h"

// Qt
#include <QFileInfo>

QIcon te::ce::getIcon(QString fileName)
{
  QFileInfo in(fileName);

  QString lang(in.suffix()),
      ulang = lang.toUpper();

  if(ulang == "LUA")
    return QIcon(":/imgs/imgs/lang-lua.svg");

  if((ulang == "PY") || (ulang == "PYTHON"))
    return QIcon(":/imgs/imgs/lang-python.svg");

  return QIcon();
}
