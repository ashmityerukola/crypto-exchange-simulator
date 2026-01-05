#include "useraccount.hpp"

// WRITE YOUR CODE HERE
UserAccount::UserAccount(std::string name, std::map<std::string, int> usermap) : name(name), usermap(usermap) {}

void UserAccount::Deposit(std::string asset, int amount){
  usermap[asset] += amount;
}

bool UserAccount::Withdrawal(std::string asset, int amount){
if(usermap[asset] >= amount){
    usermap[asset] -= amount;
    return true;
  }
  return false;
}

bool UserAccount::AddOrder(Order order) {
  if(order.side != "Buy" && order.side != "Sell"){
    return false;
  }

  if (order.amount <= 0 || order.price <= 0){
    return false;
  }

  if(order.side == "Buy"){
    int cost = order.amount * order.price;
    if (usermap.find("USD") == usermap.end()){
      return false;
    }
    if (usermap["USD"] < cost){
      return false;
    }
    usermap["USD"] -= cost;
    if (usermap["USD"] == 0){
      usermap.erase("USD");
    }
    return true;
  } else{
    if(usermap.find(order.asset) == usermap.end()){
      return false;
    }
    if(usermap[order.asset] < order.amount){
      return false;
    }
    usermap[order.asset] -= order.amount;
    if (usermap[order.asset] == 0){
      usermap.erase(order.asset);
    }
    return true;
  }
}

void UserAccount::PerformBuy(Order buy_order, Trade trade){
  if (trade.amount <= 0) return;
  usermap[trade.asset] += trade.amount;
}