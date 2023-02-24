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

RISD_BINARY_PATH="/usr/local/bin/risd"
SYSTEMD_SCRIPT="/etc/systemd/system/risd.service"
RISD_URL="https://github.com/sofakng/risd"

if [ ! -f "${RISD_BINARY_PATH}" ]
then
	echo "Downloading risd binary and risd.service"
	curl -L "$RISD_URL/blob/main/binaries/risd?raw=true" -o "${RISD_BINARY_PATH}"
	case $? in
		0)
			curl -L "$RISD_URL/blob/main/scripts/systemd_risd.service?raw=true" -o "${SYSTEMD_SCRIPT}"
			;;
		60)
			if ! curl -kL "$RISD_URL/blob/main/binaries/risd?raw=true" -o "${RISD_BINARY_PATH}"
			then
				echo "No Internet connection"
				exit 2
			fi
			curl -kL "$RISD_URL/blob/main/scripts/systemd_risd.service?raw=true" -o "${SYSTEMD_SCRIPT}"
			;;
		*)
			echo "No Internet connection"
			exit 2
			;;
	esac
fi

chmod +x "${RISD_BINARY_PATH}"

/usr/bin/systemctl daemon-reload
/usr/bin/systemctl enable risd
/usr/bin/systemctl start risd

echo "risd service is on and"
echo "active at startup."
echo "Done!"
exit 0
