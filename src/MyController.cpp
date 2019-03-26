#include <iostream>

#include "MyController.hpp"

void MyController::Test()
{
  std::cout << "Test from MyController" << std::endl;

  mongocxx::client conn{mongocxx::uri{"mongodb://192.168.161.73/"}};
  auto collection = conn["GBS"]["Flux"];
  auto cursor = collection.find({});
  for (auto doc : cursor) {
    std::cout << doc["time"].get_utf8().value.to_string().c_str() << std::endl;
  }
}
