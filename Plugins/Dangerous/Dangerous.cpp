#include "Dangerous.hpp"
#include "Plugin.hpp"
#include "Services/Config/Config.hpp"
#include "Services/Messaging/Messaging.hpp"

using namespace NWNXLib;
using namespace NWNXLib::API;

static ViewPtr<Dangerous::Dangerous> g_plugin;

NWNX_PLUGIN_ENTRY Plugin::Info *PluginInfo()
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

NWNX_PLUGIN_ENTRY Plugin *PluginLoad(Plugin::CreateParams params)
{
    g_plugin = new Dangerous::Dangerous(params);
    return g_plugin;
}

namespace Dangerous
{

const std::string Dangerous::NWX_EVENT_PREFIX = "NWNX_DANGEROUS_";

Dangerous::Dangerous(const Plugin::CreateParams &params)
    : Plugin(params)
{
    m_combatChanges = std::make_unique<Combat>(this);

    GetServices()->m_messaging->SubscribeMessage("NWNX_EVENT_SIGNAL_EVENT_SKIPPED",
        [this](const std::vector<std::string> message)
        {
            if (message[0].find(NWX_EVENT_PREFIX) == 0)
            {
                this->m_lastEventSkipped = std::strtoul(message[1].c_str(), NULL, 0) == 1;
                LOG_DEBUG("Skipped event: %d (%s)", this->m_lastEventSkipped, message[1].c_str());
            }
        });

    GetServices()->m_messaging->SubscribeMessage("NWNX_EVENT_SIGNAL_EVENT_RESULT",
        [this](const std::vector<std::string> message)
        {
            if (message[0].find(NWX_EVENT_PREFIX) == 0 && !message[1].empty())
            {
                this->m_lastEventReturnVal = std::strtol(message[1].c_str(), NULL, 0);
                LOG_DEBUG("New result value: %d (%s)", this->m_lastEventReturnVal, message[1].c_str());
            }
        });
}

Dangerous::~Dangerous()
{
}
}