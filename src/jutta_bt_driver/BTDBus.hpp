#pragma once

//---------------------------------------------------------------------------
namespace jutta_bt_driver {
//---------------------------------------------------------------------------
class BTDBus {
 public:
    BTDBus();
    BTDBus(BTDBus&&) = default;
    BTDBus(const BTDBus&) = default;
    BTDBus& operator=(BTDBus&&) = default;
    BTDBus& operator=(const BTDBus&) = default;
    ~BTDBus() = default;

 private:
};
//---------------------------------------------------------------------------
}  // namespace jutta_bt_driver
//---------------------------------------------------------------------------