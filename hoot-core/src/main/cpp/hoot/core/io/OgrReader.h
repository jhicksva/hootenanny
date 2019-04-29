/*
 * This file is part of Hootenanny.
 *
 * Hootenanny is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * --------------------------------------------------------------------
 *
 * The following copyright notices are generated automatically. If you
 * have a new notice to add, please use the format:
 * " * @copyright Copyright ..."
 * This will properly maintain the copyright information. DigitalGlobe
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2015, 2017, 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */

#ifndef __OGR_READER_H__
#define __OGR_READER_H__

// Hoot
#include <hoot/core/io/PartialOsmMapReader.h>
#include <hoot/core/util/ProgressReporter.h>

// Qt
#include <QHash>
#include <QString>
#include <QStringList>
#include <QXmlDefaultHandler>

#include <boost/shared_ptr.hpp>

#include <ogr_spatialref.h>

// Standard
#include <vector>

namespace hoot
{

class ElementIterator;
class Progress;
class OgrReaderInternal;
class Settings;

/**
 * This class is broken out into an internal and external class to avoid issues with Python's
 * include file approach.
 */
class OgrReader : public PartialOsmMapReader, public ProgressReporter
{
public:

  static std::string className() { return "hoot::OgrReader"; }

  /**
   * Returns true if this appears to be a reasonable path without actually attempting to open the
   * data source.
   */
  static bool isReasonablePath(QString path);

  OgrReader();

  OgrReader(QString path);

  OgrReader(QString path, QString layer);

  ~OgrReader();

  ElementIterator* createIterator(QString path, QString layer) const;

  QStringList getLayerNames(QString path);

  QStringList getFilteredLayerNames(const QString path);

  /**
   * Read all geometry data from the specified path.
   *
   * @param path
   * @param layer Read only from this layer. If no layer is specified then read from all geometry
   *  layers.
   * @param map Put what we read in this map.
   */
  void read(QString path, QString layer, OsmMapPtr map);

  void setDefaultCircularError(Meters circularError);

  void setDefaultStatus(Status s);

  void setLimit(long limit);

  void setTranslationFile(QString translate);

  long getFeatureCount(QString path, QString layer);

  virtual void initializePartial();

  virtual bool hasMoreElements();

  virtual ElementPtr readNextElement();

  //Progress streamGetProgress() const;

  virtual void close();

  virtual bool isSupported(QString url);

  virtual void open(QString url);

  virtual void setUseDataSourceIds(bool useDataSourceIds);

  virtual void finalizePartial();

  /**
   * Returns the bounding box for the specified projection and configuration settings. This is
   * likely only useful in unit tests.
   */
  virtual boost::shared_ptr<geos::geom::Envelope> getBoundingBoxFromConfig(const Settings& s,
    OGRSpatialReference* srs);

  virtual boost::shared_ptr<OGRSpatialReference> getProjection() const;

  //leaving this empty for the time being
  virtual QString supportedFormats() { return ""; }

  virtual void setProgress(Progress progress);
  //TODO: fix
  virtual int getNumSteps() const { return 0; }

protected:

  OgrReaderInternal* _d;

  Progress _progress;
};

}


#endif // __OGR_READER_H__
