//
//  UserController.hpp
//  web-starter-project
//
//  Created by Leonid on 2/12/18.
//  Copyright © 2018 oatpp. All rights reserved.
//

#ifndef UserController_hpp
#define UserController_hpp

#include <iostream>

#include "TDatabase.hpp"
#include "UserDto.hpp"

#include "oatpp/core/concurrency/SpinLock.hpp"
#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/macro/component.hpp"
#include "oatpp/parser/json/mapping/ObjectMapper.hpp"
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
class UserController : public oatpp::web::server::api::ApiController
{
 public:
  UserController(const std::shared_ptr<ObjectMapper> &objectMapper)
      : oatpp::web::server::api::ApiController(objectMapper)
  {
    // fDatabase.reset(new TDatabase);
    // std::cout << "hit" << std::endl;
    // fMongoInstance.reset(new mongocxx::instance);
    // fMongoInstance = new mongocxx::instance();
  }

 private:
  /**
   *  Inject Database component
   */
  // OATPP_COMPONENT(std::shared_ptr<TDatabase>, fDatabase);
  // mongocxx::instance *fMongoInstance;
  std::unique_ptr<mongocxx::instance> fMongoInstance;

 public:
  /**
   *  Inject @objectMapper component here as default parameter
   *  Do not return bare Controllable* object! use shared_ptr!
   */
  static std::shared_ptr<UserController> createShared(
      OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectMapper))
  {
    return std::make_shared<UserController>(objectMapper);
  }

  /**
   *  Begin ENDPOINTs generation ('ApiController' codegen)
   */
#include OATPP_CODEGEN_BEGIN(ApiController)

  /**
   *  Insert Your endpoints here !!!
   */
  ENDPOINT_INFO(root)
  {
    info->summary = "Index.html page";
    info->addResponse<UserDto::ObjectWrapper>(Status::CODE_200, "text/html");
  }
  ENDPOINT("GET", "/", root)
  {
    const char *html =
        "<html lang='en'>"
        "<head>"
        "<meta charset=utf-8/>"
        "</head>"
        "<body>"
        "<p>Hello CRUD example project!</p>"
        "<a href='swagger/ui'>Checkout Swagger-UI page</a>"
        "</body>"
        "</html>";
    auto response = createResponse(Status::CODE_200, html);
    response->putHeader(Header::CONTENT_TYPE, "text/html");
    return response;
  }

  ENDPOINT_INFO(getAll)
  {
    info->summary = "get all stored data";
    info->addResponse<List<GBSDto::ObjectWrapper>::ObjectWrapper>(
        Status::CODE_200, "application/json");
  }
  ENDPOINT_ASYNC("GET", "/GBS/GetAll", getAll){

      ENDPOINT_ASYNC_INIT(getAll)

      /**
       *  Coroutine entrypoint act()
       *  returns Action (what to do next)
       */
      Action act() override{

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

// ENDPOINT_INFO(getFlux)
// {
//   info->summary = "get all flux data";
//   info->addResponse<List<GBSDto::ObjectWrapper>::ObjectWrapper>(
//       Status::CODE_200, "application/json");
// }
// ENDPOINT_ASYNC("GET", "/GBS/GetFlux", getFlux){
//
//     ENDPOINT_ASYNC_INIT(getFlux)
//
//     /**
//      *  Coroutine entrypoint act()
//      *  returns Action (what to do next)
//      */
//     Action act() override{
//
//         auto result = oatpp::data::mapping::type::List<
//             GBSDto::ObjectWrapper>::createShared();
//
// mongocxx::client conn{mongocxx::uri{"mongodb://192.168.161.73/"}};
// auto collection = conn["node-angular"]["posts"];
// auto cursor = collection.find(make_document(kvp("title", "count")));
// auto counter = 0;
// for (auto doc : cursor) {
//   auto dto = GBSDto::createShared();
//   dto->id = doc["_id"].get_oid().value.to_string().c_str();
//   dto->title = doc["title"].get_utf8().value.to_string().c_str();
//   dto->content = doc["content"].get_utf8().value.to_string().c_str();
//   dto->imagePath = doc["imagePath"].get_utf8().value.to_string().c_str();
//
//   result->pushBack(dto);
// }
//
// return _return(controller->createDtoResponse(Status::CODE_200, result));
// }
// }
// ;

/**
 *  Finish ENDPOINTs generation ('ApiController' codegen)
 */
#include OATPP_CODEGEN_END(ApiController)
}
;

#endif /* UserController_hpp */
