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
 * @copyright Copyright (C) 2016, 2017, 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */
package hoot.services.controllers.grail;

import java.io.File;
import java.util.HashMap;
import java.util.Map;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import hoot.services.command.CommandResult;
import hoot.services.command.ExternalCommand;
import hoot.services.geo.BoundingBox;


class PullOSMDataCommand extends GrailCommand {
    private static final Logger logger = LoggerFactory.getLogger(PullOSMDataCommand.class);

    // PullOSMDataCommand(String jobId, BoundingBox bbox, String apiUrl, File outputFile, Class<?> caller) {
    PullOSMDataCommand(String jobId, GrailParams params, String debugLevel, Class<?> caller) {
        super(jobId,params);

        // logger.info("PullOSMCommand: " + params.toString());

        BoundingBox boundingBox = new BoundingBox(params.getBounds());
        double bboxArea = boundingBox.getArea();

        // TODO:  Pull <area maximum="0.25"> from the capabilities of the OSM API DB's
        // Also, pull the server status as well. Throw errors if they are not available.
        double maxBboxArea = 0.25;

        if (bboxArea > maxBboxArea) {
            throw new IllegalArgumentException("The bounding box area (" + bboxArea + ") is too large. It must be less than " + maxBboxArea + " degrees");
        }

        String fullUrl = params.getPullUrl() + "/map?bbox=" + boundingBox.toServicesString();

        Map<String, Object> substitutionMap = new HashMap<>();
        // substitutionMap.put("OUTPUT_FILE", outputFile.getAbsolutePath());
        substitutionMap.put("OUTPUT_FILE", params.getOutput());
        substitutionMap.put("API_URL", fullUrl);

        // wget -O <output.osm> "http://api.openstreetmap.org/api/0.6/map?bbox=$EXTENT"
        String command = "wget -O ${OUTPUT_FILE} ${API_URL}";

        super.configureCommand(command, substitutionMap, caller);
    }
}
