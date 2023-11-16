#!/bin/bash

# ZuluSCSI™ - Copyright (c) 2022 Rabbit Hole Computing™
# QuokkADB - Copyright (c) 2023 Rabbit Hole Computing™
#
# This file was orignally from the ZuluSCSI™ firmware project and
# adapted for use with the QuokkADB project.
#
# This file is licensed under the GPL version 3 or any later version. 
#
# https://www.gnu.org/licenses/gpl-3.0.html
# ----
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version. 
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details. 
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.


# This script renames the built binaries according to version
# number and platform.

mkdir -p distrib

DATE=$(date +%Y-%m-%d)
COMMIT=$(git describe --always)
VERSION=$(gcc -E utils/version-extractor.cpp -Ilib/QuokkADB/include/ | grep  "version\[\]" | cut -d '"' -f2)
SUFFIX=$(gcc -E utils/version-extractor.cpp -Ilib/QuokkADB/include/ | grep  "suffix\[\]" | cut -d '"' -f2)

for file in $(ls .pio/build/quokkadb/*.bin .pio/build/quokkadb/*.elf .pio/build/quokkadb/*.uf2)
do
    NEWNAME=$(echo $file | sed 's|.pio/build/quokkadb/\(.*\)\.\(.*\)|QuokkADB-\1-v'$VERSION'-'$SUFFIX'_'$DATE'_'$COMMIT'.\2|')
    echo $file to distrib/$NEWNAME
    cp $file distrib/$NEWNAME
done

