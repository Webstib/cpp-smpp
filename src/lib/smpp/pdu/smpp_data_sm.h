#ifndef SMPP_DATA_SM_H
#define SMPP_DATA_SM_H

#include "smpp/pdu/smpp_header.h"
#include "smpp/pdu/smpp_tlv_fields.h"

namespace smpp {

namespace pdu {

class SMPP_EXPORT DataSm : public Header, public TlvFields {
  std::string service_type;
  uint8 source_addr_ton;
  uint8 source_addr_npi;
  std::string source_addr;
  uint8 dest_addr_ton;
  uint8 dest_addr_npi;
  std::string destination_addr;
  uint8 esm_class;
  uint8 registered_delivery;
  uint8 data_coding;

public:

  DataSm(uint32 sequence_number = 0,
         const std::string &service_type = std::string(),
         uint8 source_addr_ton = 0,
         uint8 source_addr_npi = 0,
         const std::string &source_addr = std::string(),
         uint8 dest_addr_ton = 0,
         uint8 dest_addr_npi = 0,
         const std::string &destination_addr = std::string(),
         uint8 esm_class = 0,
         uint8 registered_delivery = 0,
         uint8 data_coding = 0);

  DataSm(const Header &p);

  DataSm(const DataSm &p);

  virtual ~DataSm() {

  }

  inline void setServiceType(const std::string &v) {
    service_type = v;
  }

  inline void setSourceAddrTon(uint8 v) {
    source_addr_ton = v;
  }

  inline void setSourceAddrNpi(uint8 v) {
    source_addr_npi = v;
  }

  inline void setSourceAddr(const std::string &v) {
    source_addr = v;
  }

  inline void setDestAddrTon(uint8 v) {
    dest_addr_ton = v;
  }

  inline void setDestAddrNpi(uint8 v) {
    dest_addr_npi = v;
  }

  inline void setDestinationAddr(const std::string &v) {
    destination_addr = v;
  }

  inline void setEsmClass(uint8 v) {
    esm_class = v;
  }

  inline void setRegisteredDelivery(uint8 v) {
    registered_delivery = v;
  }

  inline void setDataCoding(uint8 v) {
    data_coding = v;
  }

  inline const std::string &getServiceType() const {
    return service_type;
  }

  inline uint8 getSourceAddrTon() const {
    return source_addr_ton;
  }

  inline uint8 getSourceAddrNpi() const {
    return source_addr_npi;
  }

  inline const std::string &getSourceAddr() const {
    return source_addr;
  }

  inline uint8 getDestAddrTon() const {
    return dest_addr_ton;
  }

  inline uint8 getDestAddrNpi() const {
    return dest_addr_npi;
  }

  inline const std::string &getDestinationAddr() const {
    return destination_addr;
  }

  inline uint8 getEsmClass() const {
    return esm_class;
  }

  inline uint8 getRegisteredDelivery() const {
    return registered_delivery;
  }

  inline uint8 getDataCoding() const {
    return data_coding;
  }

  void calcCommandLength();

  DataSm &operator =(const DataSm &p);

  bool operator ==(const DataSm &p) const;

  bool operator !=(const DataSm &p) const;

  static const uint32 MIN_LENGTH = 26;
};

} // namespace pdu

} // namespace smpp

#endif // SMPP_DATA_SM_H
