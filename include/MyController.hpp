//
//  MyController.hpp
//  web-starter-project
//
//  Created by Leonid on 2/12/18.
//  Copyright © 2018 oatpp. All rights reserved.
//

#ifndef MyController_hpp
#define MyController_hpp

#include "MyDTOs.hpp"

#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/macro/component.hpp"
#include "oatpp/web/server/api/ApiController.hpp"

#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/builder/stream/helpers.hpp>
#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>

using bsoncxx::builder::stream::document;
using bsoncxx::builder::stream::finalize;

using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_document;
/**
 *  EXAMPLE ApiController
 *  Basic examples of howto create ENDPOINTs
 *  More details on oatpp.io
 */
class MyController : public oatpp::web::server::api::ApiController
{
 protected:
  MyController(const std::shared_ptr<ObjectMapper> &objectMapper)
      : oatpp::web::server::api::ApiController(objectMapper)
  {
    // fDatabase.reset(new TDatabase);
    fMongoInstance.reset(new mongocxx::instance);
  }

 private:
  // Think!  How to implementing the DB handler class
  // Now All procedures are inside the ENDPOINT definitions

  // static std::unique_ptr<TDatabase> fDatabase;
  // OATPP_COMPONENT(std::shared_ptr<TDatabase>, fDatabase);
  std::unique_ptr<mongocxx::instance> fMongoInstance;

 public:
  /**
   *  Inject @objectMapper component here as default parameter
   *  Do not return bare Controllable* object! use shared_ptr!
   */
  static std::shared_ptr<MyController> createShared(
      OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectMapper))
  {
    return std::shared_ptr<MyController>(new MyController(objectMapper));
  }

  /**
   *  Begin ENDPOINTs generation ('ApiController' codegen)
   */
#include OATPP_CODEGEN_BEGIN(ApiController)

  // test

  ENDPOINT_INFO(getAll)
  {
    info->summary = "get all stored data";
    info->addResponse<List<GBSDto::ObjectWrapper>::ObjectWrapper>(
        Status::CODE_200, "application/json");
  }
  ENDPOINT_ASYNC("GET", "/GBS/GetAll",
                 getAll){ENDPOINT_ASYNC_INIT(getAll) Action act() override{

      auto result = oatpp::data::mapping::type::List<
          GBSDto::ObjectWrapper>::createShared();

  mongocxx::client conn{mongocxx::uri{"mongodb://192.168.161.73/"}};
  auto collection = conn["node-angular"]["posts"];
  auto cursor = collection.find({});
  auto counter = 0;
  for (auto doc : cursor) {
    auto dto = GBSDto::createShared();
    dto->id = doc["_id"].get_oid().value.to_string().c_str();
    dto->title = doc["title"].get_utf8().value.to_string().c_str();
    dto->content = doc["content"].get_utf8().value.to_string().c_str();
    dto->imagePath = doc["imagePath"].get_utf8().value.to_string().c_str();

    result->pushBack(dto);
  }

  return _return(controller->createDtoResponse(Status::CODE_200, result));
}
}
;

ENDPOINT_INFO(getFlux)
{
  info->summary = "Get Flux information";
  info->addResponse<List<FluxDto::ObjectWrapper>::ObjectWrapper>(
      Status::CODE_200, "application/json");
}
ENDPOINT_ASYNC("GET", "/GBS/GetFlux",
               getFlux){ENDPOINT_ASYNC_INIT(getFlux) Action act() override{

    auto result = oatpp::data::mapping::type::List<
        FluxDto::ObjectWrapper>::createShared();

mongocxx::client conn{mongocxx::uri{"mongodb://192.168.161.73/"}};
auto collection = conn["GBS"]["Flux"];
auto cursor = collection.find({});
auto counter = 0;
for (auto doc : cursor) {
  auto dto = FluxDto::createShared();
  dto->id = doc["_id"].get_oid().value.to_string().c_str();
  dto->count = doc["count"].get_utf8().value.to_string().c_str();
  dto->hz = doc["hz"].get_utf8().value.to_string().c_str();
  dto->time = doc["time"].get_utf8().value.to_string().c_str();

  result->pushBack(dto);
}

return _return(controller->createDtoResponse(Status::CODE_200, result));
}
}
;

ENDPOINT_INFO(getEnergy)
{
  info->summary = "Get Energy information";
  info->addResponse<List<EnergyDto::ObjectWrapper>::ObjectWrapper>(
      Status::CODE_200, "application/json");
}
ENDPOINT_ASYNC("GET", "/GBS/GetEnergy",
               getEnergy){ENDPOINT_ASYNC_INIT(getEnergy) Action act() override{

    auto result = oatpp::data::mapping::type::List<
        EnergyDto::ObjectWrapper>::createShared();

mongocxx::client conn{mongocxx::uri{"mongodb://192.168.161.73/"}};
auto collection = conn["GBS"]["Energy"];
auto cursor = collection.find({});
auto counter = 0;
for (auto doc : cursor) {
  auto dto = EnergyDto::createShared();
  dto->id = doc["_id"].get_oid().value.to_string().c_str();
  dto->mean = doc["mean"].get_utf8().value.to_string().c_str();
  dto->fwhm = doc["fwhm"].get_utf8().value.to_string().c_str();
  dto->imagePath = doc["imagePath"].get_utf8().value.to_string().c_str();
  dto->time = doc["time"].get_utf8().value.to_string().c_str();

  result->pushBack(dto);
}

return _return(controller->createDtoResponse(Status::CODE_200, result));
}
}
;

/**
 *  Finish ENDPOINTs generation ('ApiController' codegen)
 */
#include OATPP_CODEGEN_END(ApiController)
}
;

#endif /* MyController_hpp */
