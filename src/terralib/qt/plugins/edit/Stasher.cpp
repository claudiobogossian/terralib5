
#include "Stasher.h"

#include "GeometryFile.h"

#include "../../../edit/Utils.h"
#include "../../../maptools/AbstractLayer.h"

// Qt
#include <QDir>
#include <QStandardPaths>


void StashGeometries(const te::map::AbstractLayer* layer, const std::map<std::string, te::gm::Geometry*>& geoms, const std::map<std::string, int>& ops)
{
  QString userDataDir = QStandardPaths::writableLocation(QStandardPaths::DataLocation);

  QDir dir(userDataDir + "/edition/" +layer->getTitle().c_str());

  if(!dir.exists())
    dir.mkpath(dir.path());

  QString f = dir.path() + "/" + QString::fromUtf8(layer->getTitle().c_str());
  QString f2 = f + "_r";

  std::vector<te::gm::Geometry*> gs;
  std::vector<te::gm::Geometry*> gsR;

  std::string fName = (f + ".ids").toUtf8().data();

  std::ofstream idsFile(fName.c_str(), std::ios::out);
  std::ofstream idsRFile((fName + "r").c_str(), std::ios::out);

  for(std::map<std::string, te::gm::Geometry*>::const_iterator it = geoms.begin(); it != geoms.end(); ++it)
  {
    std::string id = it->first;

    if(ops.at(id) == te::edit::GEOMETRY_DELETE)
    {
      gsR.push_back(it->second);
      idsRFile << id << "\n";
    }
    else
    {
      gs.push_back(it->second);
      idsFile << id << "\n";
    }
  }

  idsFile.close();
  idsRFile.close();

  GeometryFile::writeGeometries(f.toUtf8().data(), gs);
  GeometryFile::writeGeometries(f2.toUtf8().data(), gsR);
}


void GetStashedGeometries(const te::map::AbstractLayer* layer, std::map<std::string, te::gm::Geometry*>& geoms, std::map<std::string, int>& ops)
{
  QString userDataDir = QStandardPaths::writableLocation(QStandardPaths::DataLocation);

  QDir dir(userDataDir + "/edition/" + QString::fromUtf8(layer->getTitle().c_str()));

  if(dir.exists())
  {
    GeometryFile f;

    QString fName = dir.path() + "/" + QString::fromUtf8(layer->getTitle().c_str()) + ".geom";

    f.openFile(fName.toUtf8().data());

    QFile qf(dir.path() + "/" + QString::fromUtf8(layer->getTitle().c_str()) + ".ids");

    if(!qf.open(QIODevice::ReadOnly | QIODevice::Text))
      return;

    while(te::gm::Geometry* gm = f.next())
    {
      QString id(qf.readLine().data());

      if(id.isEmpty())
        continue;

      id.remove("\n");

      bool ok;

      id.toInt(&ok);
      
      ops[id.toUtf8().data()] = ((ok) ? te::edit::GEOMETRY_UPDATE : te::edit::GEOMETRY_CREATE);

      gm->setSRID(layer->getSRID());

      geoms[id.toUtf8().data()] = gm;
    }

    fName = dir.path() + "/" + QString::fromUtf8(layer->getTitle().c_str()) + "_r.geom";

    GeometryFile f2;

    f2.openFile(fName.toUtf8().data());

    QFile qf2(dir.path() + "/" + QString::fromUtf8(layer->getTitle().c_str()) + ".idsr");

    if(!qf2.open(QIODevice::ReadOnly | QIODevice::Text))
      return;

    while(te::gm::Geometry* gm = f2.next())
    {
      QString id(qf2.readLine().data());

      if(id.isEmpty())
        continue;

      id.remove("\n");

      bool ok;

      id.toInt(&ok);

      ops[id.toUtf8().data()] = te::edit::GEOMETRY_DELETE;

      gm->setSRID(layer->getSRID());

      geoms[id.toUtf8().data()] = gm;
    }
  }
}

std::set<std::string> GetStashedLayers()
{
  std::set<std::string> res;

  QString userDataDir = QStandardPaths::writableLocation(QStandardPaths::DataLocation);

  QDir dir(userDataDir + "/edition");

  QStringList lst = dir.entryList(QDir::Dirs);

  for(QStringList::iterator it = lst.begin(); it != lst.end(); ++it)
    res.insert((*it).toUtf8().data());

  return res;
}

void RemoveStash(const te::map::AbstractLayer* layer)
{
  QString userDataDir = QStandardPaths::writableLocation(QStandardPaths::DataLocation);

  QDir dir(userDataDir + "/edition/" + QString::fromUtf8(layer->getTitle().c_str()));

  if(dir.exists())
    if(!dir.removeRecursively())
      throw;
}
