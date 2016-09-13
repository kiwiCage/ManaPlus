/*
 *  The ManaPlus Client
 *  Copyright (C) 2011-2016  The ManaPlus Developers
 *
 *  This file is part of The ManaPlus Client.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "resources/db/networkdb.h"

#include "configuration.h"
#include "logger.h"

#include "utils/xmlutils.h"

#include "resources/beingcommon.h"

#include "debug.h"

namespace
{
    bool mLoaded = false;
    NetworkInfos mInPackets;
}  // namespace

void NetworkDb::load()
{
    if (mLoaded)
        unload();

    logger->log1("Initializing network database...");
    loadXmlFile(paths.getStringValue("networkFile"), SkipError_false);
    loadXmlFile(paths.getStringValue("networkPatchFile"), SkipError_true);
    loadXmlDir("networkPatchDir", loadXmlFile);
    mLoaded = true;
}

void NetworkDb::loadXmlFile(const std::string &fileName,
                            const SkipError skipError)
{
    readXmlIntMap(fileName,
        "network",
        "inpackets",
        "fakepacket",
        "id",
        "len",
        mInPackets,
        skipError);
}

void NetworkDb::unload()
{
    mInPackets.clear();
    mLoaded = false;
}

const NetworkInfos &NetworkDb::getFakePackets()
{
    return mInPackets;
}
