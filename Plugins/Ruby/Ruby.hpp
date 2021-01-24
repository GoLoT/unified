#pragma once

#include "Plugin.hpp"
#include "Events.hpp"

namespace Ruby {

class Ruby : public NWNXLib::Plugin
{
public:
    Ruby(NWNXLib::Services::ProxyServiceList* services);
    virtual ~Ruby();

    Events::ArgumentStack Evaluate(Events::ArgumentStack&& args);

private:
    bool m_evaluateMetrics;
    int32_t m_nextEvaluationId;

    void SafeRequire(const std::string& script);
    void HandleError(const int errCode);
};

}
