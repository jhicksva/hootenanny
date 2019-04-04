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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */

#ifndef GEOMETRYMODIFIEROP_H
#define GEOMETRYMODIFIEROP_H

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/ops/OsmMapOperation.h>
#include <hoot/core/info/OperationStatusInfo.h>
#include <hoot/core/visitors/GeometryModifierVisitor.h>

namespace hoot
{

  class GeometryModifierOp : public OsmMapOperation, public OperationStatusInfo
  {
  public:

    GeometryModifierOp();

    // OsmMapOperation
    static std::string className() { return "hoot::GeometryModifierOp"; }
    void apply(boost::shared_ptr<OsmMap>& map);
    QString getDescription() const { return "Modifies map geometry as specified"; }

    // OperationStatusInfo
    virtual QString getInitStatusMessage() const { return "Modifying geometry..."; }
    virtual QString getCompletedStatusMessage() const { return "Modified " + QString::number(_numAffected) + " elements"; }

  private:
    QString _rulesFileName;
    GeometryModifierVisitor _geometryModifierVisitor;
  };

}

#endif // GEOMETRYMODIFIEROP_H
