#include "smpp/parameters/smpp_prameters_header.h"
#include "smpp/pdu/smpp_data_sm_resp.h"

namespace smpp {

namespace pdu {

DataSmResp::DataSmResp(uint32 command_status,
                       uint32 sequence_number,
                       const std::string &message_id) :
  Header(MIN_LENGTH,
         parameters::command_id::DATA_SM_RESP,
         command_status,
         sequence_number),
  message_id(message_id) {

}

DataSmResp::DataSmResp(const Header &p) : Header(p) {

}

DataSmResp::DataSmResp(const DataSmResp &p) :
  Header(p),
  TlvFields(p),
  message_id(p.message_id) {

}

void DataSmResp::calcCommandLength() {
  setCommandLength(MIN_LENGTH + message_id.size() + TlvFields::size());
}

DataSmResp &DataSmResp::operator =(const DataSmResp &p) {
  if(this != &p) {
    Header::operator=(p);
    message_id = p.message_id;
    TlvFields::operator=(p);
  }
  return *this;
}

bool DataSmResp::operator ==(const DataSmResp &p) const {
  return(Header::operator==(p) &&
         message_id == p.message_id &&
         TlvFields::operator==(p));
}

bool DataSmResp::operator !=(const DataSmResp &p) const {
  return !(*this == p);
}

} // namespace pdu

} // namespace smpp
