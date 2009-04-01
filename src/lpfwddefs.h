/*****************************************************************************/
/*!
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 *****************************************************************************
 *
 * \file   lpfwddefs.h
 *
 * \brief  Forward definitions of LP classes.
 *
 * Version:
 *      Sri Panyam      05 Dec 2008
 *      Initial version
 *
 *****************************************************************************/

#ifndef _LUNARPROBE_FWD_DEFS_H
#define _LUNARPROBE_FWD_DEFS_H

#include <assert.h>
#include "lpnsdefs.h"

extern "C"
{
    #include "lua.h"
    #include "lauxlib.h"
    #include "lualib.h"
};

LUNARPROBE_NS_BEGIN

class LunarProbe;
class ClientIface;
class BayeuxClientIface;
class TcpClientIface;
class LuaBindings;
class DebugContext;

typedef lua_State* LuaStack;
typedef lua_Debug* LuaDebug;

LUNARPROBE_NS_END

#endif
