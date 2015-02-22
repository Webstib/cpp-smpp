#ifndef SMPP_OUTBIND_H
#define SMPP_OUTBIND_H

#include "smpp/pdu/smpp_header.h"

namespace smpp {

namespace pdu {

class SMPP_EXPORT Outbind : public Header {
  std::string system_id;
  std::string password;

public:

  Outbind(uint32 sequence_number = 0,
          const std::string &system_id = std::string(),
          const std::string &password = std::string());

  Outbind(const Header &p);

  Outbind(const Outbind &p);

  virtual ~Outbind() {

  }

  inline void setSystemId(const std::string &v) {
    system_id = v;
  }

  inline void setPassword(const std::string &v) {
    password = v;
  }

  inline const std::string &getSystemId() const {
    return system_id;
  }

  inline const std::string &getPassword() const {
    return password;
  }

  void calcCommandLength();

  Outbind &operator =(const Outbind &p);

  bool operator ==(const Outbind &p) const;

  bool operator !=(const Outbind &p) const;

  static const uint32 MIN_LENGTH = 18;
};

} // namespace pdu

} // namespace smpp

#endif // SMPP_OUTBIND_H
