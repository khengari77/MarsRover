#pragma once

#define ARDUINOJSON_ENABLE_STD_STRING 1
#include <ArduinoJson.h>
#include <string>
#include <functional>
#include <unordered_map>
#include <variant>
#include <cstdint>

using ValueType = std::variant<std::string, 
                               int, 
                               double, 
                               bool,
                               std::int32_t,
                               std::uint32_t>;
using ValueCallback = std::function<const ValueType ()>;

namespace ArduinoJson {
template <>
struct Converter<ValueType> {
    static void toJson(const ValueType& src, JsonVariant dst) {
        std::visit(Visitor{dst}, src);
    }
    
private:
    struct Visitor {
        JsonVariant dst_;
        
        void operator()(int v) const           { dst_.set(v); }
        void operator()(std::uint32_t v) const { dst_.set(v); }
        void operator()(std::int32_t v) const  { dst_.set(v); }
        void operator()(double v) const        { dst_.set(v); }
        void operator()(bool v) const          { dst_.set(v); }
        void operator()(const std::string& v)  { dst_.set(v); }
    };
};
}


class DataTracker {
public:


  DataTracker() {}
  void update();
  void addValueCallback(const std::string& key, ValueCallback callback);
  void addValue(const std::string& key, const ValueType& value);
  ValueType getValue(const std::string& key);


  const std::unordered_map<std::string, ValueType>& getData() const  { return data; }
  const JsonDocument getJson() const;

private:
  std::unordered_map<std::string, ValueType> data;
  std::unordered_map<std::string, ValueCallback> valueCallbacks;

};


