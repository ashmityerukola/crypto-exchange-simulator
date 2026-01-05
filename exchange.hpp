#pragma once
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "useraccount.hpp"
#include "utility.hpp"

class Exchange {
// WRITE YOUR CODE HERE
 public:
 std::map<std::string, UserAccount> portfolios;
 Exchange();
  void MakeDeposit(std::string username, std::string asset,
                   int amount);
  void PrintUserPortfolios(std::ostream &os);
  bool MakeWithdrawal(std::string username, std::string asset,
                      int amount);
  bool AddOrder(Order order);
  void PrintUsersOrders(std::ostream &os);

  std::map<std::string, std::vector<Order> > openorders;
  std::map<std::string, std::vector<Order>> filledorders;
  std::vector<Trade> tradehistory;
  
  void PrintTradeHistory(std::ostream &os);
  void PrintBidAskSpread(std::ostream &os);
};