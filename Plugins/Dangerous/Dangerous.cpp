#include "Dangerous.hpp"
#include "Plugin.hpp"
#include "Services/Config/Config.hpp"

using namespace NWNXLib;
using namespace NWNXLib::API;

static ViewPtr<Dangerous::Dangerous> g_plugin;

NWNX_PLUGIN_ENTRY Plugin::Info* PluginInfo()
{
    return new Plugin::Info
        {
            "Dangerous",
            "Aptly named plugin that is expected to break stuff and will probably break even more stuff than expected",
            "GoLoT",
            "golot@golot.es",
            1,
            true
        };
}

NWNX_PLUGIN_ENTRY Plugin* PluginLoad(Plugin::CreateParams params)
{
g_plugin = new Dangerous::Dangerous(params);
return g_plugin;
}

namespace Dangerous
{

Dangerous::Dangerous(const Plugin::CreateParams &params)
    : Plugin(params)
{
    if(GetServices()->m_config->Get<bool>("MASTER_SERVER", false))
        m_combatChanges = std::make_unique<Combat>(GetServices());
}
}