#include "utility.hpp"

#include <iostream>

bool Order::operator==(const Order &other) const {
  return (username == other.username) && (side == other.side) &&
         (asset == other.asset) && (amount == other.amount) &&
         (price == other.price);
}

std::ostream &operator<<(std::ostream &os, const Order &order) {
  os << order.side << " " << order.amount << " " << order.asset << " at "
     << order.price << " USD by " << order.username;
  return os;
}
