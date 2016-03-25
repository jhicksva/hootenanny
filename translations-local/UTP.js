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
 * @copyright Copyright (C) 2014 DigitalGlobe (http://www.digitalglobe.com/)
 */

//
// UTP Conversion
//

hoot.require('utp')
hoot.require('utp_rules')
hoot.require('fcode_common')
hoot.require('translate');
hoot.require('config')


/* No output so no need
// Create the output Schema
function getDbSchema()
{
    return utp.getDbSchema();
}
*/

function initialize()
{
    if (typeof utp === 'undefined')
    {
        logError('Please install the UTP Translation scripts.');
    }
}

// IMPORT
// translateAttributes - takes 'attrs' and returns OSM 'tags'
function translateAttributes(attrs, layerName, geometryType)
{
    if (typeof utp !== 'undefined')
    {
        return utp.toOsm(attrs, layerName);
    }
    else
    {
        return attrs;
    }
} // End of Translate Attributes


/* No Export
// EXPORT
// translateToOgr - takes 'tags' + geometry and returns 'attrs' + tableName
function translateToOgr(tags, elementType, geometryType)
{
    return utp.toUtp(tags, elementType, geometryType)
} // End of translateToOgr

*/
