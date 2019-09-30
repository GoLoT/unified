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
    std::unique_ptr<Combat> m_combatChanges;
};
}