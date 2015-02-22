#include "smpp/pdu/smpp_unsuccess_sme.h"

namespace smpp {

namespace pdu {

UnsuccessSme::UnsuccessSme(uint8 dest_addr_ton,
                           uint8 dest_addr_npi,
                           const std::string &destination_addr,
                           uint32 error_status_code) :
  dest_addr_ton(dest_addr_ton),
  dest_addr_npi(dest_addr_npi),
  destination_addr(destination_addr),
  error_status_code(error_status_code) {

}

UnsuccessSme::UnsuccessSme(const UnsuccessSme &p) :
  dest_addr_ton(p.dest_addr_ton),
  dest_addr_npi(p.dest_addr_npi),
  destination_addr(p.destination_addr),
  error_status_code(p.error_status_code) {

}

UnsuccessSme &UnsuccessSme::operator =(const UnsuccessSme &p) {
  if(this != &p) {
    dest_addr_ton = p.dest_addr_ton;
    dest_addr_npi = p.dest_addr_npi;
    destination_addr = p.destination_addr;
    error_status_code = p.error_status_code;
  }
  return *this;
}

bool UnsuccessSme::operator ==(const UnsuccessSme &p) const {
  return(dest_addr_ton == p.dest_addr_ton &&
         dest_addr_npi == p.dest_addr_npi &&
         destination_addr == p.destination_addr &&
         error_status_code == p.error_status_code);
}

bool UnsuccessSme::operator !=(const UnsuccessSme &p) const {
  return !(*this == p);
}

} // namespace pdu

} // namespace smpp
