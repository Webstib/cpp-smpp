#ifndef SMPP_BIND_RECEIVER_H
#define SMPP_BIND_RECEIVER_H

#include "smpp/pdu/smpp_header.h"

namespace smpp {

namespace pdu {

class SMPP_EXPORT BindReceiver : public Header {
  std::string system_id;
  std::string password;
  std::string system_type;
  uint8 interface_version;
  uint8 addr_ton;
  uint8 addr_npi;
  std::string address_range;

public:

  BindReceiver(
    uint32 sequence_number = 0,
    const std::string &system_id = std::string(),
    const std::string &password = std::string(),
    const std::string &system_type = std::string(),
    uint8 interface_version = 0,
    uint8 addr_ton = 0,
    uint8 addr_npi = 0,
    const std::string &address_range = std::string());

  BindReceiver(const Header &p);

  BindReceiver(const BindReceiver &p);

  virtual ~BindReceiver() {

  }

  inline void setSystemId(const std::string &v) {
    system_id = v;
  }

  inline void setPassword(const std::string &v) {
    password = v;
  }

  inline void setSystemType(const std::string &v) {
    system_type = v;
  }

  inline void setInterfaceVersion(uint8 v) {
    interface_version = v;
  }

  inline void setAddrTon(uint8 v) {
    addr_ton = v;
  }

  inline void setAddrNpi(uint8 v) {
    addr_npi = v;
  }

  inline void setAddressRange(const std::string &v) {
    address_range = v;
  }

  inline const std::string &getSystemId() const {
    return system_id;
  }

  inline const std::string &getPassword() const {
    return password;
  }

  inline const std::string &getSystemType() const {
    return system_type;
  }

  inline uint8 getInterfaceVersion() const {
    return interface_version;
  }

  inline uint8 getAddrTon() const {
    return addr_ton;
  }

  inline uint8 getAddrNpi() const {
    return addr_npi;
  }

  inline const std::string &getAddressRange() const {
    return address_range;
  }

  void calcCommandLength();

  BindReceiver &operator =(const BindReceiver &p);

  bool operator ==(const BindReceiver &p) const;

  bool operator !=(const BindReceiver &p) const;

  static const uint32 MIN_LENGTH = 23;
};

} // namespace pdu

} // namespace smpp

#endif // SMPP_BIND_RECEIVER_H
