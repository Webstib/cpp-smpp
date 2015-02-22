#include "smpp/pdu/smpp_multi_destination_address.h"

namespace smpp {

namespace pdu {

MultiDestinationAddresses::MultiDestinationAddresses() {

}

MultiDestinationAddresses::MultiDestinationAddresses(
  const MultiDestinationAddresses &p) {
  copy(p.addrs);
}

void MultiDestinationAddresses::copy(
  const std::list<MultiDestinationAddressBase *> &p) {
  clear();
  for(std::list<MultiDestinationAddressBase *>::const_iterator it = p.begin();
      it != p.end();
      it++) {
    MultiDestinationAddressBase *addr(*it);
    if(addr->getDestFlag() == 0x01) {
      add(*reinterpret_cast<SmeMultiAddress *>(addr));
    } else if(addr->getDestFlag() == 0x02) {
      add(*reinterpret_cast<DistributionListAddress *>(addr));
    }
  }
}

std::list<SmeMultiAddress> MultiDestinationAddresses::getSmeMultiAddress() const {
  std::list<SmeMultiAddress> list;
  for(std::list<MultiDestinationAddressBase *>::const_iterator it =
        addrs.begin();
      it != addrs.end();
      it++) {
    MultiDestinationAddressBase *addr(*it);
    if(addr->getDestFlag() == 0x01) {
      list.push_back(*reinterpret_cast<SmeMultiAddress *>(addr));
    }
  }
  return list;
}

std::list<DistributionListAddress>
MultiDestinationAddresses::getDistributionListAddress() const {
  std::list<DistributionListAddress> list;
  for(std::list<MultiDestinationAddressBase *>::const_iterator it =
        addrs.begin();
      it != addrs.end();
      it++) {
    MultiDestinationAddressBase *addr(*it);
    if(addr->getDestFlag() == 0x02) {
      list.push_back(*reinterpret_cast<DistributionListAddress *>(addr));
    }
  }
  return list;
}

void MultiDestinationAddresses::remove(const SmeMultiAddress &p) {
  for(std::list<MultiDestinationAddressBase *>::iterator it = addrs.begin();
      it != addrs.end();
      it++) {
    MultiDestinationAddressBase *addr(*it);
    if(addr->getDestFlag() == 0x01) {
      SmeMultiAddress *pt(
        reinterpret_cast<SmeMultiAddress *>(addr));
      if(p == *pt) {
        it = addrs.erase(it);
        delete pt;
      }
    }
  }
}

void MultiDestinationAddresses::remove(const DistributionListAddress &p) {
  for(std::list<MultiDestinationAddressBase *>::iterator it = addrs.begin();
      it != addrs.end();
      it++) {
    MultiDestinationAddressBase *addr(*it);
    if(addr->getDestFlag() == 0x02) {
      DistributionListAddress *pt(
        reinterpret_cast<DistributionListAddress *>(addr));
      if(p == *pt) {
        it = addrs.erase(it);
        delete pt;
      }
    }
  }
}

void MultiDestinationAddresses::removeSmeMultiAddress() {
  for(std::list<MultiDestinationAddressBase *>::iterator it = addrs.begin();
      it != addrs.end();
      it++) {
    MultiDestinationAddressBase *addr(*it);
    if(addr->getDestFlag() == 0x01) {
      it = addrs.erase(it);
      delete addr;
    }
  }
}

void MultiDestinationAddresses::removeDistributionListAddress() {
  for(std::list<MultiDestinationAddressBase *>::iterator it = addrs.begin();
      it != addrs.end();
      it++) {
    MultiDestinationAddressBase *addr(*it);
    if(addr->getDestFlag() == 0x02) {
      it = addrs.erase(it);
      delete addr;
    }
  }
}

void MultiDestinationAddresses::clear() {
  while(!addrs.empty()) {
    delete addrs.front();
    addrs.pop_front();
  }
}

uint32 MultiDestinationAddresses::size() const {
  uint32 v(0);
  for(std::list<MultiDestinationAddressBase *>::const_iterator it =
        addrs.begin();
      it != addrs.end();
      it++) {
    v += (*it)->size();
  }
  return v;
}

MultiDestinationAddresses &MultiDestinationAddresses::operator =(
  const MultiDestinationAddresses &p) {
  if(*this != p) {
    copy(p.addrs);
  }
  return *this;
}

bool MultiDestinationAddresses::operator ==(
  const MultiDestinationAddresses &p) const {
  if(addrs.size() == p.addrs.size()) {
    for(std::list<MultiDestinationAddressBase *>::const_iterator it1 =
          addrs.begin(),
        it2 = p.addrs.begin();
        it1 != addrs.end(), it2 != p.addrs.end();
        it1++, it2++) {
      MultiDestinationAddressBase *addr1(*it1);
      MultiDestinationAddressBase *addr2(*it1);
      uint8 flag1(addr1->getDestFlag());
      uint8 flag2(addr2->getDestFlag());
      if(flag1 == flag2 && flag1 == 0x01) {
        if(*reinterpret_cast<SmeMultiAddress *>(addr1) !=
           *reinterpret_cast<SmeMultiAddress *>(addr2))
          return false;
      } else if(flag1 == flag2 && flag1 == 0x02) {
        if(*reinterpret_cast<DistributionListAddress *>(addr1) !=
           *reinterpret_cast<DistributionListAddress *>(addr2))
          return false;
      }
    }
    return true;
  }
  return false;
}

bool MultiDestinationAddresses::operator !=(
  const MultiDestinationAddresses &p) const {
  return !(*this == p);
}

} // namespace pdu

} // namespace smpp
