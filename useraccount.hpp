#pragma once
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "utility.hpp"

class UserAccount {

// WRITE YOUR CODE HERE
  // Suggestions:
 public:
  std::string name;
  std::map<std::string, int> usermap;
  UserAccount(std::string name, std::map<std::string, int> usermap);
  void Deposit(std::string asset, int amount);
  bool Withdrawal(std::string asset, int amount);
  bool AddOrder(Order order);
  void PerformBuy(Order buy_order, Trade trade);
  void PerformSell(Order sell_order, Trade trade);
  std::map<std::string, int> GetPortfolio();
  std::vector<Order> GetOpenOrders();
  void PrintOrders(std::ostream &os);
};