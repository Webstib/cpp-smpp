#include "smpp/pdu/smpp_sme_multi_address.h"

namespace smpp {

namespace pdu {

SmeMultiAddress::SmeMultiAddress(uint8 dest_addr_ton,
                                 uint8 dest_addr_npi,
                                 const std::string &destination_addr) :
  dest_addr_ton(dest_addr_ton),
  dest_addr_npi(dest_addr_npi),
  destination_addr(destination_addr) {

}

SmeMultiAddress::SmeMultiAddress(const SmeMultiAddress &p) :
  MultiDestinationAddressBase(p),
  dest_addr_ton(p.dest_addr_ton),
  dest_addr_npi(p.dest_addr_npi),
  destination_addr(p.destination_addr) {

}

SmeMultiAddress &SmeMultiAddress::operator =(const SmeMultiAddress &p) {
  if(this != &p) {
    dest_addr_ton = p.dest_addr_ton;
    dest_addr_npi = p.dest_addr_npi;
    destination_addr = p.destination_addr;
  }
  return *this;
}

bool SmeMultiAddress::operator ==(const SmeMultiAddress &p) const {
  return (dest_addr_ton == p.dest_addr_ton &&
          dest_addr_npi == p.dest_addr_npi &&
          destination_addr == p.destination_addr);
}

bool SmeMultiAddress::operator !=(const SmeMultiAddress &p) const {
  return !(*this == p);
}

} // namespace pdu

} // namespace smpp
