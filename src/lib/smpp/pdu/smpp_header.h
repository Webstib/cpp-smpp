#ifndef SMPP_HEADER_H
#define SMPP_HEADER_H

#include "smpp/smpp_global.h"

namespace smpp {

namespace pdu {

class SMPP_EXPORT Header {
public:

  Header(uint32 command_length = 0,
         uint32 command_id = 0,
         uint32 command_status = 0,
         uint32 sequence_number = 0);

  Header(const Header &p);

  virtual ~Header() {

  }

  inline void setCommandLength(uint32 v) {
    command_length = v;
  }

  inline void setCommandId(uint32 v) {
    command_id = v;
  }

  inline void setCommandStatus(uint32 v) {
    command_status = v;
  }

  inline void setSequenceNumber(uint32 v) {
    sequence_number = v;
  }

  inline uint32 getCommandLength() const {
    return command_length;
  }

  inline uint32 getCommandId() const {
    return command_id;
  }

  inline uint32 getCommandStatus() const {
    return command_status;
  }

  inline uint32 getSequenceNumber() const {
    return sequence_number;
  }

  virtual Header &operator =(const Header &p);

  virtual bool operator ==(const Header &p) const;

  virtual bool operator !=(const Header &p) const;

  static const uint32 HEADER_LENGTH = 16;

protected:

  uint32 command_length;
  uint32 command_id;
  uint32 command_status;
  uint32 sequence_number;
};

} // namespace pdu

} // namespace smpp

#endif // SMPP_HEADER_H
