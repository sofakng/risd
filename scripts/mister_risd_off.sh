#!/bin/bash

# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

# Copyright 2021 John "sofakng" Klimek

# You can download the latest version of this script from:
# https://github.com/MiSTer-devel/Scripts_MiSTer

# Version 1.0 - 2023-02-24 - First commit

/etc/init.d/S40risd stop /dev/null > /dev/null 2>&1
mount | grep -q "on / .*[(,]ro[,$]" && RO_ROOT="true"
[ "$RO_ROOT" == "true" ] && mount / -o remount,rw
rm /etc/init.d/S40risd > /dev/null 2>&1
sync
[ "$RO_ROOT" == "true" ] && mount / -o remount,ro
sync

echo "risd service is off and"
echo "inactive at startup."
echo "Done!"
exit 0
