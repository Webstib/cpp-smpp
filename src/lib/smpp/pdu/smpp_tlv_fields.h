#ifndef SMPP_TLV_FIELDS_H
#define SMPP_TLV_FIELDS_H

#include "smpp/pdu/smpp_tlv.h"

namespace smpp {

namespace pdu {

typedef std::list<smpp::pdu::Tlv *> TlvList;

class SMPP_EXPORT TlvFields {

  void copy(const TlvList &p);

public:

  TlvFields();

  TlvFields(const TlvFields &p);

  virtual ~TlvFields() {
    clear();
  }

  inline void add(const Tlv &tlv) {
    tlv_list.push_back(new Tlv(tlv));
  }

  inline const TlvList &getList() const {
    return tlv_list;
  }

  TlvList find(const uint16 &tag) const;

  void remove(const uint16 &tag);

  void clear();

  uint32 size() const;

  TlvFields &operator =(const TlvFields &p);

  bool operator ==(const TlvFields &p) const;

  bool operator !=(const TlvFields &p) const;

protected:

  TlvList tlv_list;
};

} // namespace pdu

} // namespace smpp

#endif // SMPP_TLV_FIELDS_H
