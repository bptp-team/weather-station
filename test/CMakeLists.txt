# Builds the firmware logic against a host stub of the Arduino APIs, so it can
# be unit tested on a development machine and in CI without an ESP32 attached.
#
#   cmake -S test -B build
#   cmake --build build
#   ctest --test-dir build --output-on-failure

cmake_minimum_required(VERSION 3.16)

project(weather_station_tests CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

enable_testing()

set(FIRMWARE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../weather_station")

add_library(arduino_stub STATIC
  arduino/Arduino.cpp
  arduino/PubSubClient.cpp
  arduino/WiFi.cpp
)
target_include_directories(arduino_stub PUBLIC arduino)

add_library(test_framework STATIC framework/TestFramework.cpp)
target_include_directories(test_framework PUBLIC framework)

# Every firmware module that does not need real hardware to run. Modules left
# out here, such as EnvironmentSensor, block on the sensor and are covered by
# the firmware compilation job instead.
add_library(weather_station STATIC
  "${FIRMWARE_DIR}/AirQualitySensor.cpp"
  "${FIRMWARE_DIR}/DaylightSensor.cpp"
  "${FIRMWARE_DIR}/Logger.cpp"
  "${FIRMWARE_DIR}/MqttPublisher.cpp"
  "${FIRMWARE_DIR}/WaterLevelSensor.cpp"
  "${FIRMWARE_DIR}/WeatherReadingLogger.cpp"
)
target_include_directories(weather_station PUBLIC "${FIRMWARE_DIR}")
target_link_libraries(weather_station PUBLIC arduino_stub)
target_compile_options(weather_station PRIVATE -Wall -Wextra)

set(TEST_NAMES
  LoggerTest
  MqttPublisherTest
  SensorsTest
  WeatherReadingLoggerTest
)

foreach(test_name IN LISTS TEST_NAMES)
  add_executable(${test_name} "${test_name}.cpp" framework/TestMain.cpp)
  target_link_libraries(${test_name} PRIVATE weather_station test_framework)
  add_test(NAME ${test_name} COMMAND ${test_name})
endforeach()
