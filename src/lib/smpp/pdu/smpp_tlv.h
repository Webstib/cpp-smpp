#ifndef SMPP_TLV_H
#define SMPP_TLV_H

#include "smpp/smpp_global.h"

namespace smpp {

namespace pdu {

class SMPP_EXPORT Tlv {
  uint16 tag;
  uint16 value_len;
  int8 *value;

  void renewValue(uint16 len);

public:

  Tlv(uint16 tag = 0, uint16 value_len = 0, int8 *value = 0);

  Tlv(const Tlv &p);

  virtual ~Tlv() {
    renewValue(0);
  }

  inline uint16 getTag() const {
    return tag;
  }

  inline uint16 getLength() const {
    return value_len;
  }

  inline const int8 *getValue() const {
    return value;
  }

  bool getValue(std::string &v) const;

  bool getValue(uint8 &v) const;

  bool getValue(uint16 &v) const;

  bool getValue(uint32 &v) const;

  inline void setTag(uint16 v) {
    tag = v;
  }

  void setValue(const int8 *v, uint16 v_len);

  void setValue(const std::string &v);

  void setValue(uint8 v);

  void setValue(uint16 v);

  void setValue(uint32 v);

  uint32 size() const;

  Tlv &operator =(const Tlv &p);

  bool operator ==(const Tlv &p) const;

  bool operator !=(const Tlv &p) const;

};

} // namespace pdu

} // namespace smpp

#endif // SMPP_TLV_H
