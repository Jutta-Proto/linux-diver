#pragma once

#include <jutta_proto/JuttaConnection.hpp>

//---------------------------------------------------------------------------
namespace jutta_driver {
//---------------------------------------------------------------------------
class JuttaDriver {
 private:
    jutta_proto::JuttaConnection connection;

 public:
    explicit JuttaDriver(std::string&& device);
    JuttaDriver(JuttaDriver&&) = default;
    JuttaDriver(const JuttaDriver&) = default;
    JuttaDriver& operator=(JuttaDriver&&) = delete;
    JuttaDriver& operator=(const JuttaDriver&) = delete;
    ~JuttaDriver() = default;

    void run();
};
//---------------------------------------------------------------------------
}  // namespace jutta_driver
//---------------------------------------------------------------------------
