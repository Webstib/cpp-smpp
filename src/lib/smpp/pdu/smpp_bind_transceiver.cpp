#include "smpp/parameters/smpp_prameters_header.h"
#include "smpp/pdu/smpp_bind_transceiver.h"

namespace smpp {

namespace pdu {

BindTransceiver::BindTransceiver(
  uint32 sequence_number,
  const std::string &system_id,
  const std::string &password,
  const std::string &system_type,
  uint8 interface_version,
  uint8 addr_ton,
  uint8 addr_npi,
  const std::string &address_range) :
    Header(MIN_LENGTH,
           parameters::command_id::BIND_TRANSCEIVER,
           parameters::command_status::ESME_ROK,
           sequence_number),
    system_id(system_id),
    password(password),
    system_type(system_type),
    interface_version(interface_version),
    addr_ton(addr_ton),
    addr_npi(addr_npi),
    address_range(address_range) {

}

BindTransceiver::BindTransceiver(const Header &p) : Header(p) {

}

BindTransceiver::BindTransceiver(const BindTransceiver &p) :
  Header(p),
  system_id(p.system_id),
  password(p.password),
  system_type(p.system_type),
  interface_version(p.interface_version),
  addr_ton(p.addr_ton),
  addr_npi(p.addr_npi),
  address_range(p.address_range) {

}

void BindTransceiver::calcCommandLength() {
  setCommandLength(
    MIN_LENGTH +
    system_id.size() +
    password.size() +
    system_type.size() +
    address_range.size());
}

BindTransceiver &BindTransceiver::operator =(const BindTransceiver &p) {
  if(this != &p) {
    Header::operator=(p);
    system_id = p.system_id;
    password = p.password;
    system_type = p.system_type;
    interface_version = p.interface_version;
    addr_ton = p.addr_ton;
    addr_npi = p.addr_npi;
    address_range = p.address_range;
  }
  return *this;
}

bool BindTransceiver::operator ==(const BindTransceiver &p) const {
  return(Header::operator==(p) &&
         system_id == p.system_id &&
         password == p.password &&
         system_type == p.system_type &&
         interface_version == p.interface_version &&
         addr_ton == p.addr_ton &&
         addr_npi == p.addr_npi &&
         address_range == p.address_range);
}

bool BindTransceiver::operator !=(const BindTransceiver &p) const {
  return !(*this == p);
}

} // namespace pdu

} // namespace smpp
