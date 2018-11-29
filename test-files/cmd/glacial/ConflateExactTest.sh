#!/bin/bash
set -e

mkdir -p test-output/cmd/ConflateExactTest
STATS_OUT=test-output/cmd/ConflateExactTest/boston-road-building-out

HOOT_OPTS="-D writer.include.debug.tags=true -D poi.polygon.address.match.enabled=true"

hoot conflate $HOOT_OPTS test-files/BostonSubsetRoadBuilding_FromShp.osm test-files/BostonSubsetRoadBuilding_FromOsm.osm $STATS_OUT.osm > $STATS_OUT

hoot diff $STATS_OUT.osm test-files/cmd/glacial/ConflateExactTest/output.osm
