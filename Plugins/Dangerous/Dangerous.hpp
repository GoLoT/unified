#pragma once

#include "Plugin.hpp"
#include "Combat/Combat.hpp"

namespace Dangerous
{

class Dangerous : public NWNXLib::Plugin
{
public:
    Dangerous(const Plugin::CreateParams &params);
    virtual ~Dangerous();

private:
    static const std::string NWNX_EVENT_PREFIX;

    bool m_lastEventSkipped;
    int m_lastEventReturnVal;
    std::unique_ptr<Combat> m_combatChanges;

    friend class Combat;
};
}