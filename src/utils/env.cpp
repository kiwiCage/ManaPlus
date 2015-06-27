/*
 *  The ManaPlus Client
 *  Copyright (C) 2011-2015  The ManaPlus Developers
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

#include "utils/env.h"

#include "configuration.h"
#include "logger.h"
#include "settings.h"

#include <stdlib.h>

#include "debug.h"

void updateEnv()
{
#if defined(WIN32) || defined(__APPLE__)
    if (config.getBoolValue("centerwindow"))
        setEnv("SDL_VIDEO_CENTERED", "1");
    else
        setEnv("SDL_VIDEO_CENTERED", "0");
#endif

    if (config.getBoolValue("allowscreensaver"))
        setEnv("SDL_VIDEO_ALLOW_SCREENSAVER", "1");
    else
        setEnv("SDL_VIDEO_ALLOW_SCREENSAVER", "0");

#ifndef WIN32
    const int vsync = settings.options.test.empty()
        ? config.getIntValue("vsync") : 1;
    // __GL_SYNC_TO_VBLANK is nvidia variable.
    // vblank_mode is MESA variable.
    switch (vsync)
    {
        case 1:
            setEnv("__GL_SYNC_TO_VBLANK", "0");
            setEnv("vblank_mode", "0");
            break;
        case 2:
            setEnv("__GL_SYNC_TO_VBLANK", "1");
            setEnv("vblank_mode", "1");
            break;
        default:
            break;
    }
#endif
}

void setEnv(const char *const name, const char *const value)
{
    if (!name || !value)
        return;
#ifdef WIN32
    if (putenv(const_cast<char*>((std::string(name)
        + "=" + value).c_str())))
#else
    if (setenv(name, value, 1))
#endif
    {
        logger->log("setenv failed: %s=%s", name, value);
    }
}
