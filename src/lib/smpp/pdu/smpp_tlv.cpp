#include "smpp/pdu/smpp_tlv.h"

namespace smpp {

namespace pdu {

void Tlv::renewValue(uint16 len) {
  if(value) {
    delete [] value;
  }

  if(len > 0) {
    value = new int8[len];
  } else {
    value = 0;
  }

  value_len = len;
}

Tlv::Tlv(uint16 tag, uint16 value_len, int8 *value) :
  tag(tag), value_len(value_len), value(0) {
  if(value_len != 0 && value != 0) {
    renewValue(value_len);
    memcpy(this->value, value, value_len);
  }
}

Tlv::Tlv(const Tlv &p) : tag(p.tag), value_len(p.value_len), value(0) {
  if(value_len != 0 && value != 0) {
    renewValue(value_len);
    memcpy(this->value, p.value, value_len);
  }
}

bool Tlv::getValue(std::string &p) const {
  uint16 pos(0);
  while(1) {
    if(pos >= value_len)
      return false;
    if(value[pos++] == '\0')
      break;
  }
  if(pos != value_len)
    return false;
  p.clear();
  p.resize(value_len - 1);
  for(int i = 0; i < value_len - 1; i++)
    p[i] = value[i];
  return true;
}

bool Tlv::getValue(uint8 &p) const {
  if(value_len != 1)
    return false;

  p = value[0];

  return true;
}

bool Tlv::getValue(uint16 &p) const {
  if(value_len != 2)
    return false;

  uint16 v(0);
  memcpy(&v, value, 2);

  p = ntohs(v);

  return true;
}

bool Tlv::getValue(uint32 &p) const {
  if(value_len != 4)
    return false;

  uint32 v(0);
  memcpy(&v, value, 4);

  p = ntohl(v);

  return true;
}

void Tlv::setValue(const int8 *p, uint16 p_len) {
  renewValue(p_len);
  if(p_len != 0 && p != 0) {
    memcpy(value, p, p_len);
  }
}

void Tlv::setValue(const std::string &p) {
  setValue(p.data(), p.size() + 1);
}

void Tlv::setValue(uint8 p) {
  renewValue(1);
  value[0] = p;
}

void Tlv::setValue(uint16 p) {
  renewValue(2);
  uint16 v(htons(p));
  memcpy(value, &v, 2);
}

void Tlv::setValue(uint32 p) {
  renewValue(4);
  uint32 v(htonl(p));
  memcpy(value, &v, 4);
}

uint32 Tlv::size() const {
  return 4 + value_len;
}

Tlv &Tlv::operator =(const Tlv &p) {
  if(*this != p) {
    tag = p.tag;
    value = p.value;
  }
  return *this;
}

bool Tlv::operator ==(const Tlv &p) const {
  return (tag == p.tag &&
          value_len == p.value_len &&
          memcmp(p.value, value, value_len) == 0);
}

bool Tlv::operator !=(const Tlv &p) const {
  return !(*this == p);
}

} // namespace pdu

} // namespace smpp
