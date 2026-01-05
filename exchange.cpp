#include "exchange.hpp"
// WRITE YOUR CODE HERE
#include <map>
#include <ostream>

Exchange::Exchange() {}

void Exchange::MakeDeposit(std::string user, std::string asset, int amount){
  if (portfolios.find(user) == portfolios.end()) {
    std::map<std::string, int> emptymap;
    portfolios.insert({user, UserAccount(user, emptymap)});
  }
  portfolios.at(user).Deposit(asset, amount);
}

bool Exchange::MakeWithdrawal(std::string user, std::string asset, int amount){
  if (portfolios.find(user) == portfolios.end()){
    return false;
  }
  return portfolios.at(user).Withdrawal(asset, amount);
}

void Exchange::PrintUserPortfolios(std::ostream &os) {
  os << "User Portfolios (in alphabetical order):\n";
  for(auto &p : portfolios) {
    std::string user = p.first;
    auto &usermap = p.second.usermap;
    os << user << "'s Portfolio: ";
    for (auto &i : usermap) {
      if (i.second == 0){
        continue;
      }
      os<<i.second<<" "<<i.first<<", ";
    }
    os << "\n";
  }
}


bool Exchange::AddOrder(Order order) {
  if (portfolios.find(order.username) == portfolios.end()) return false;
  if (!portfolios.at(order.username).AddOrder(order)) return false;

  int originalamount = order.amount;

  if (order.side == "Buy") {
    while (order.amount > 0) {
      std::string user = "";
      int pos = -1;
      int buyprice = 0;

      for (auto &u : openorders) {
        std::vector<Order> &v = u.second;
        for (size_t i = 0; i < v.size(); i++){
          Order &m = v[i];
          if (m.side == "Sell" && m.asset == order.asset && m.amount > 0 && m.price <= order.price) {
            if(pos == -1 || m.price < buyprice){
              user = u.first;
              pos = i;
              buyprice = m.price;
            }
          }
        }
      }

      if (pos == -1){
        break;
      }

      Order &o = openorders[user][pos];
      int traded = 0;
      if (order.amount < o.amount) {
        traded = order.amount;
      } else {
        traded = o.amount;
      }
      int price = order.price;

      Trade t;
      t.buyer_username = order.username;
      t.seller_username = user;
      t.asset = order.asset;
      t.amount = traded;
      t.price = price;
      tradehistory.push_back(t);

      portfolios.at(order.username).PerformBuy(order, t);
      portfolios.at(user).Deposit("USD", traded * price);

      Order a;
      a.username = order.username;
      a.side = "Buy";
      a.asset = order.asset;
      a.amount = traded;
      a.price = price;
      filledorders[order.username].push_back(a);

      Order b;
      b.username = user;
      b.side = "Sell";
      b.asset = order.asset;
      b.amount = traded;
      b.price = price;
      filledorders[user].push_back(b);

      order.amount -= traded;
      o.amount -= traded;

      if (o.amount == 0) {
        openorders[user].erase(openorders[user].begin() + pos);
      }
    }

    if(order.amount > 0){
      openorders[order.username].push_back(order);
    }
    return true;
  } 
  else{
    while(order.amount > 0){
      std::string user = "";
      int pos = -1;
      int buyprice = 0;

      for (auto &u : openorders) {
        std::vector<Order> &v = u.second;
        for (int i = 0; i < (int)v.size(); i++) {
          Order &m = v[i];
          if (m.side == "Buy" && m.asset == order.asset && m.amount > 0 && m.price >= order.price) {
            if (pos == -1 || m.price > buyprice) {
              user = u.first;
              pos = i;
              buyprice = m.price;
            }
          }
        }
      }

      if (pos == -1) break;

      Order &o = openorders[user][pos];
      int traded = 0;
      if (order.amount < o.amount) {
        traded = order.amount;
      } else {
        traded = o.amount;
      }
      int price = order.price;

      Trade t;
      t.buyer_username = user;
      t.seller_username = order.username;
      t.asset = order.asset;
      t.amount = traded;
      t.price = price;
      tradehistory.push_back(t);

      portfolios.at(user).Deposit(order.asset, traded);
      portfolios.at(order.username).Deposit("USD", traded * price);

      Order b;
      b.username = user;
      b.side = "Buy";
      b.asset = order.asset;
      b.amount = traded;
      b.price = price;
      filledorders[user].push_back(b);

      Order a;
      a.username = order.username;
      a.side = "Sell";
      a.asset = order.asset;
      a.amount = traded;
      a.price = price;
      filledorders[order.username].push_back(a);

      order.amount -= traded;
      o.amount -= traded;

      if (o.amount == 0) {
        openorders[user].erase(openorders[user].begin() + pos);
      }
    }

    if(order.amount > 0){
      openorders[order.username].push_back(order);
    }
    return true;
  }
}

void Exchange::PrintUsersOrders(std::ostream &os) {
  os << "Users Orders (in alphabetical order):" << std::endl;
  for (auto p : portfolios){
    std::string user = p.first;
    os << user << "'s Open Orders (in chronological order):" << std::endl;
    if (openorders.count(user) > 0) {
      std::vector<Order> v = openorders[user];
      for (int i = 0; i < (int)v.size(); i++) {
        os << v[i] << std::endl;
      }
    }
    os << user << "'s Filled Orders (in chronological order):" << std::endl;
    if (filledorders.count(user) > 0) {
      std::vector<Order> fv = filledorders[user];
      for (int i = 0; i < (int)fv.size(); i++) {
        os << fv[i] << std::endl;
      }
    }
  }
}

void Exchange::PrintTradeHistory(std::ostream &os) {
  os << "Trade History (in chronological order):" << std::endl;
  for (size_t i = 0; i < tradehistory.size(); i++) {
    Trade &t = tradehistory[i];
    os<<t.buyer_username<<" Bought "<<t.amount<<" of "<< t.asset<<" From "<<t.seller_username<<" for "<<t.price<<" USD"<<std::endl;
  }
}

void Exchange::PrintBidAskSpread(std::ostream &os){
  os<<"Asset Bid Ask Spread (in alphabetical order):"<<std::endl;
  std::map<std::string, bool> assets;
  for(auto b : openorders){
    std::vector<Order> v = b.second;
    for(int i = 0; i < (int)v.size(); i++){
      assets[v[i].asset] = true;
    }
  }

  for (auto a : assets){
    std::string asset = a.first;
    std::string bid = "NA";
    std::string ask = "NA";

    for(auto u : openorders){
      std::vector<Order> v = u.second;
      for (int i = 0; i < (int)v.size(); i++){
        Order o = v[i];
        if(o.asset == asset && o.side == "Buy"){
          if(bid == "NA" || o.price > std::stoi(bid)){
            bid = std::to_string(o.price);
          }
        }
        if(o.asset == asset && o.side == "Sell"){
          if (ask == "NA" || o.price < std::stoi(ask)){
            ask = std::to_string(o.price);
          }
        }
      }
    }
    os<<asset<<": Highest Open Buy = "<<bid<<" USD and Lowest Open Sell = "<<ask<<" USD"<<std::endl;
  }
}