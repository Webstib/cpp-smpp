#ifndef SMPP_DISTRIBUTION_LIST_ADDRESS_H
#define SMPP_DISTRIBUTION_LIST_ADDRESS_H

#include "smpp/pdu/smpp_multi_destination_address_base.h"

namespace smpp {

namespace pdu {

class SMPP_EXPORT DistributionListAddress : public MultiDestinationAddressBase {
  std::string destrinution_list_name;

public:

  DistributionListAddress(
    const std::string &destrinution_list_name = std::string());

  DistributionListAddress(const DistributionListAddress &p);

  virtual ~DistributionListAddress() {

  }

  inline void setDlName(const std::string &v) {
    destrinution_list_name = v;
  }

  inline const std::string &getDlName() const {
    return destrinution_list_name;
  }

  inline uint8 getDestFlag() const {
    return 0x02;
  }

  uint32 size() const;

  DistributionListAddress &operator =(const DistributionListAddress &p);

  bool operator ==(const DistributionListAddress &p) const;

  bool operator !=(const DistributionListAddress &p) const;
};

} // namespace pdu

} // namespace smpp

#endif // SMPP_DISTRIBUTION_LIST_ADDRESS_H
