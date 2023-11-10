#include "ReanimatedMacros.h"
#ifndef DEBUG

#include <memory>
#include "JSLogger.h"

namespace reanimated {

void JSLogger::warnOnJS(const std::string &warning) const {
  jsScheduler_->scheduleOnJS([warning](jsi::Runtime &rt) {
    auto console = rt.global().getPropertyAsObject(rt, "console");
    auto warn = console.getPropertyAsFunction(rt, "warn");
    warn.call(rt, jsi::String::createFromUtf8(rt, warning));
  });
}

} // namespace reanimated

#endif // REANIMATED_NDEBUG
