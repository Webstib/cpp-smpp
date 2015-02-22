#include "smpp/parameters/smpp_prameters_header.h"
#include "smpp/pdu/smpp_data_sm.h"

namespace smpp {

namespace pdu {

DataSm::DataSm(uint32 sequence_number,
               const std::string &service_type,
               uint8 source_addr_ton,
               uint8 source_addr_npi,
               const std::string &source_addr,
               uint8 dest_addr_ton,
               uint8 dest_addr_npi,
               const std::string &destination_addr,
               uint8 esm_class,
               uint8 registered_delivery,
               uint8 data_coding) :
  Header(MIN_LENGTH,
         parameters::command_id::DATA_SM,
         parameters::command_status::ESME_ROK,
         sequence_number),
  service_type(service_type),
  source_addr_ton(source_addr_ton),
  source_addr_npi(source_addr_npi),
  source_addr(source_addr),
  dest_addr_ton(dest_addr_ton),
  dest_addr_npi(dest_addr_npi),
  destination_addr(destination_addr),
  esm_class(esm_class),
  registered_delivery(registered_delivery),
  data_coding(data_coding) {

}

DataSm::DataSm(const Header &p) :
  Header(p),
  source_addr_ton(0),
  source_addr_npi(0),
  dest_addr_ton(0),
  dest_addr_npi(0),
  esm_class(0),
  registered_delivery(0),
  data_coding(0) {

}

DataSm::DataSm(const DataSm &p) :
  Header(p),
  TlvFields(p),
  service_type(p.service_type),
  source_addr_ton(p.source_addr_ton),
  source_addr_npi(p.source_addr_npi),
  source_addr(p.source_addr),
  dest_addr_ton(p.dest_addr_ton),
  dest_addr_npi(p.dest_addr_npi),
  destination_addr(p.destination_addr),
  esm_class(p.esm_class),
  registered_delivery(p.registered_delivery),
  data_coding(p.data_coding) {

}

void DataSm::calcCommandLength() {
  setCommandLength(
    MIN_LENGTH +
    service_type.size() +
    source_addr.size() +
    destination_addr.size() +
    TlvFields::size());
}

DataSm &DataSm::operator =(const DataSm &p) {
  if(this != &p) {
    Header::operator=(p);
    service_type = p.service_type;
    source_addr_ton = p.source_addr_ton;
    source_addr_npi = p.source_addr_npi;
    source_addr = p.source_addr;
    dest_addr_ton = p.dest_addr_ton;
    dest_addr_npi = p.dest_addr_npi;
    destination_addr = p.destination_addr;
    esm_class = p.esm_class;
    registered_delivery = p.registered_delivery;
    data_coding = p.data_coding;
    TlvFields::operator=(p);
  }
  return *this;
}

bool DataSm::operator ==(const DataSm &p) const {
  return(Header::operator==(p) &&
         service_type == p.service_type &&
         source_addr_ton == p.source_addr_ton &&
         source_addr_npi == p.source_addr_npi &&
         source_addr == p.source_addr &&
         dest_addr_ton == p.dest_addr_ton &&
         dest_addr_npi == p.dest_addr_npi &&
         destination_addr == p.destination_addr &&
         esm_class == p.esm_class &&
         registered_delivery == p.registered_delivery &&
         data_coding == p.data_coding &&
         TlvFields::operator==(p));
}

bool DataSm::operator !=(const DataSm &p) const {
  return !(*this == p);
}

} // namespace pdu

} // namespace smpp
