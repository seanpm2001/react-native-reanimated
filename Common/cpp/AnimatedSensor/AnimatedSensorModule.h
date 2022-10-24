#pragma once

#include <jsi/jsi.h>
#include <unordered_set>

#include "PlatformDepMethodsHolder.h"
#include "RuntimeManager.h"

namespace reanimated {

using namespace facebook;

class JSRuntimeHelper;

enum SensorType {
  ACCELEROMETER = 1,
  GYROSCOPE = 2,
  GRAVITY = 3,
  MAGNETIC_FIELD = 4,
  ROTATION_VECTOR = 5,
};

class AnimatedSensorModule {
  std::unordered_set<int> sensorsIds_;
  RegisterSensorFunction platformRegisterSensorFunction_;
  UnregisterSensorFunction platformUnregisterSensorFunction_;

 public:
  AnimatedSensorModule(const PlatformDepMethodsHolder &platformDepMethodsHolder);
  ~AnimatedSensorModule();

  jsi::Value registerSensor(
      jsi::Runtime &rt,
      JSRuntimeHelper *runtimeHelper,
      const jsi::Value &sensorType,
      const jsi::Value &interval,
      const jsi::Value &sensorDataContainer);
  void unregisterSensor(const jsi::Value &sensorId);
};

} // namespace reanimated