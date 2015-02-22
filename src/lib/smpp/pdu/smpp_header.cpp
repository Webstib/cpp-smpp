#include "smpp/pdu/smpp_header.h"

namespace smpp {

namespace pdu {

Header::Header(uint32 command_length,
               uint32 command_id,
               uint32 command_status,
               uint32 sequence_number) :
  command_length(command_length),
  command_id(command_id),
  command_status(command_status),
  sequence_number(sequence_number) {

}

Header::Header(const Header &p) :
  command_length(p.command_length),
  command_id(p.command_id),
  command_status(p.command_status),
  sequence_number(p.sequence_number) {

}

Header &Header::operator =(const Header &p) {
  if(this != &p) {
    command_length = p.command_length;
    command_id = p.command_id;
    command_status = p.command_status;
    sequence_number = p.sequence_number;
  }
  return *this;
}

bool Header::operator ==(const Header &p) const {
  return (command_length == p.command_length &&
          command_id == p.command_id &&
          command_status == p.command_status &&
          sequence_number == p.sequence_number);
}

bool Header::operator !=(const Header &p) const {
  return !(*this == p);
}

} // namespace pdu

} // namespace smpp
