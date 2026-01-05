#pragma once
#include <iostream>
#include <string>

class Order {
 public:
  std::string username;
  std::string side;  // Can be "Buy" or "Sell"
  std::string asset;
  int amount;
  int price;

  bool operator==(const Order &other) const;
  // Allows for testing if two orders are equal
};

std::ostream &operator<<(std::ostream &os, const Order &order);
// Allows orders to be written to a stream

class Trade {
 public:
  std::string buyer_username;
  std::string seller_username;
  std::string asset;
  int amount;
  int price;
};