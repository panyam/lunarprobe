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
 *  \file   LunarProbe.cpp
 *
 *  \brief  Implementation of CLunarProbe
 *
 *  \version
 *      - S Panyam   27/10/2008
 *      Initial version.
 */
//*****************************************************************************

#include "LunarProbe.h"
#include "TcpDebugger.h"

LUNARPROBE_NS_BEGIN

std::auto_ptr< Debugger > LunarProbe::pDebugger;
const int HookMask     = LUA_MASKCALL | LUA_MASKRET | LUA_MASKLINE /* | LUA_MASKCOUNT */;
const int HookCount    = 10000;

//*****************************************************************************
/*!
 *  \brief  Callback called by lua when debug events are reached.
 *
 *  \version
 *      - S Panyam  23/10/2008
 *      Initial version.
 */
//*****************************************************************************
void HookFunction(LuaStack pStack, LuaDebug pDebug)
{
    LunarProbe::GetInstance()->GetDebugger()->HandleDebugHook(pStack, pDebug);
}

//*****************************************************************************
/*!
 *  \brief  Destructor
 *
 *  \version
 *      - S Panyam  30/03/2009
 *      Initial version.
 */
//*****************************************************************************
LunarProbe::~LunarProbe()
{
}

//*****************************************************************************
/*!
 *  \brief  Gets the LP instance.
 *
 *  \version
 *      - S Panyam  30/03/2009
 *      Initial version.
 */
//*****************************************************************************
LunarProbe *LunarProbe::GetInstance()
{
    static LunarProbe theProbe;
    return &theProbe;
}

//*****************************************************************************
/*!
 *  \brief  Get the instance of the Debugger.
 *
 *  \version
 *      - S Panyam  27/10/2008
 *      Initial version.
 */
//*****************************************************************************
Debugger *LunarProbe::GetDebugger()
{
    if (pDebugger.get() == NULL)
    {
        pDebugger = std::auto_ptr<Debugger>(new TcpDebugger());
    }
    return pDebugger.get();
}

//*****************************************************************************
/*!
 *  \brief  Set the instance of the Debugger.
 *
 *  \version
 *      - S Panyam  27/10/2008
 *      Initial version.
 */
//*****************************************************************************
void LunarProbe::SetDebugger(Debugger *pDebugger_)
{
    Debugger *pOld = pDebugger.get();
    if (pOld != NULL)
        delete pOld;
    pDebugger = std::auto_ptr<Debugger>(pDebugger_);
}

//*****************************************************************************
/*!
 *  \brief  Starts debugging of a lua stack.
 *
 *  \version
 *      - S Panyam  27/10/2008
 *      Initial version.
 */
//*****************************************************************************
int LunarProbe::Attach(LuaStack pStack, const char *name)
{
    int result = -1;
    if (GetDebugger()->StartDebugging(pStack, name))
        result = lua_sethook(pStack, HookFunction, HookMask, HookCount);
    return result;
}

//*****************************************************************************
/*!
 *  \brief  Stops debugging of a lua stack.
 *
 *  \version
 *      - S Panyam  27/10/2008
 *      Initial version.
 */
//*****************************************************************************
int LunarProbe::Detach(LuaStack pStack)
{
    GetDebugger()->StopDebugging(pStack);
    return lua_sethook(pStack, NULL, 0, 0);
}

LUNARPROBE_NS_END

