#include "smpp/pdu/smpp_tlv_fields.h"

namespace smpp {

namespace pdu {

TlvFields::TlvFields() {

}

TlvFields::TlvFields(const TlvFields &p) {
  copy(p.tlv_list);
}

void TlvFields::copy(const TlvList &p) {
  clear();
  for(auto it = p.begin(); it != p.end(); it++) {
    add(**it);
  }
}

TlvList TlvFields::find(const uint16 &tag) const {
  TlvList list;
  for(auto it = tlv_list.begin(); it != tlv_list.end(); it++) {
    if((*it)->getTag() == tag)
      list.push_back(*it);
  }
  return list;
}

void TlvFields::remove(const uint16 &tag) {
  for(auto it = tlv_list.begin(); it != tlv_list.end();) {
    if((*it)->getTag() == tag) {
      delete *it;
      it = tlv_list.erase(it);
    } else {
      it++;
    }
  }
}

void TlvFields::clear() {
  while(!tlv_list.empty()) {
    delete tlv_list.front();
    tlv_list.pop_front();
  }
}

uint32 TlvFields::size() const {
  uint32 v(0);
  for(auto it = tlv_list.begin(); it != tlv_list.end(); it++) {
    v += (*it)->size();
  }
  return v;
}

TlvFields &TlvFields::operator =(const TlvFields &p) {
  if(p != *this) {
    copy(p.tlv_list);
  }
  return *this;
}

bool TlvFields::operator ==(const TlvFields &p) const {
  if(tlv_list.size() == p.tlv_list.size()) {
    for(auto it1 = tlv_list.begin(), it2 = p.tlv_list.begin();
        it1 != tlv_list.end(), it2 != p.tlv_list.end();
        it1++, it2++) {
      if(**it1 != **it2)
        return false;
    }
    return true;
  }
  return false;
}

bool TlvFields::operator !=(const TlvFields &p) const {
  return !(*this == p);
}

} // namespace pdu

} // namespace smpp
