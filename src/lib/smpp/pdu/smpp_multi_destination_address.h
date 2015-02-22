#ifndef SMPP_MULTI_DESTINATION_ADDRESS_H
#define SMPP_MULTI_DESTINATION_ADDRESS_H

#include "smpp/pdu/smpp_sme_multi_address.h"
#include "smpp/pdu/smpp_distribution_list_address.h"

namespace smpp {

namespace pdu {

class SMPP_EXPORT MultiDestinationAddresses {
  std::list<MultiDestinationAddressBase *> addrs;

  void copy(const std::list<MultiDestinationAddressBase *> &p);

public:

  MultiDestinationAddresses();

  MultiDestinationAddresses(const MultiDestinationAddresses &p);

  virtual ~MultiDestinationAddresses() {
    clear();
  }

  inline void add(const SmeMultiAddress &p) {
    addrs.push_back(new SmeMultiAddress(p));
  }

  inline void add(const DistributionListAddress &p) {
    addrs.push_back(new DistributionListAddress(p));
  }

  inline const std::list<MultiDestinationAddressBase *> &getAll() const {
    return addrs;
  }

  std::list<SmeMultiAddress> getSmeMultiAddress() const;

  std::list<DistributionListAddress> getDistributionListAddress() const;

  void remove(const SmeMultiAddress &p);

  void remove(const DistributionListAddress &p);

  void removeSmeMultiAddress();

  void removeDistributionListAddress();

  void clear();

  uint32 size() const;

  MultiDestinationAddresses &operator =(const MultiDestinationAddresses &p);

  bool operator ==(const MultiDestinationAddresses &p) const;

  bool operator !=(const MultiDestinationAddresses &p) const;
};

} // namespace pdu

} // namespace smpp

#endif // SMPP_MULTI_DESTINATION_ADDRESS_H
