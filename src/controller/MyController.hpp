//
//  MyController.hpp
//  web-starter-project
//
//  Created by Leonid on 2/12/18.
//  Copyright © 2018 oatpp. All rights reserved.
//

#ifndef MyController_hpp
#define MyController_hpp

#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/builder/stream/helpers.hpp>
#include <bsoncxx/json.hpp>
#include <memory>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>

#include "db/TDBHandler.hpp"
#include "dto/DTOs.hpp"
#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/macro/component.hpp"
#include "oatpp/web/server/api/ApiController.hpp"

using bsoncxx::builder::stream::document;
using bsoncxx::builder::stream::finalize;

using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_document;
/**
 * Sample Api Controller.
 */
class MyController : public oatpp::web::server::api::ApiController
{
 public:
  /**
   * Constructor with object mapper.
   * @param objectMapper - default object mapper used to serialize/deserialize DTOs.
   */
  MyController(OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectMapper))
      : oatpp::web::server::api::ApiController(objectMapper)
  {
  }

 private:
  OATPP_COMPONENT(std::shared_ptr<TDBHandler>, database);

 public:
/**
 *  Begin ENDPOINTs generation ('ApiController' codegen)
 */
#include OATPP_CODEGEN_BEGIN(ApiController)

  ENDPOINT("GET", "/", root)
  {
    const String html =
        "<html lang='en'>"
        "<head>"
        "<meta charset=utf-8/>"
        "</head>"
        "<body>"
        "<p>Web API for GBS diagnostics</p>"
        "<a href='swagger/ui'>Checkout Swagger-UI page</a>"
        "</body>"
        "</html>";
    auto response = createResponse(Status::CODE_200, html);
    response->putHeader(Header::CONTENT_TYPE, "text/html");
    return response;
  }

  // TODO Insert Your endpoints here !!!
  ENDPOINT_INFO(getEnergy)
  {
    info->summary = "Get the current/last Energy information";
    info->addResponse<EnergyDto::ObjectWrapper>(Status::CODE_200,
                                                "application/json");
  }
  ADD_CORS(getEnergy)
  ENDPOINT("GET", "/GBS/GetEnergy", getEnergy)
  {
    auto dto = database->GetEnergy();
    auto response = createDtoResponse(Status::CODE_200, dto);
    return response;
  }

  ENDPOINT_INFO(getEnergyList)
  {
    info->summary = "Get the list of Energy information";
    info->addResponse<List<EnergyDto::ObjectWrapper>::ObjectWrapper>(
        Status::CODE_200, "application/json");
  }
  ADD_CORS(getEnergyList)
  ENDPOINT("GET", "/GBS/GetEnergyList", getEnergyList)
  {
    auto dto = database->GetEnergyList();
    auto response = createDtoResponse(Status::CODE_200, dto);
    return response;
  }

  ENDPOINT_INFO(getFluxList)
  {
    info->summary = "Get the list of Flux information";
    info->addResponse<List<FluxDto::ObjectWrapper>::ObjectWrapper>(
        Status::CODE_200, "application/json");
  }
  ADD_CORS(getFluxList)
  ENDPOINT("GET", "/GBS/GetFluxList", getFluxList)
  {
    auto dto = database->GetFluxList();
    auto response = createDtoResponse(Status::CODE_200, dto);
    return response;
  }

  ENDPOINT_INFO(getPosition)
  {
    info->summary = "Get the current/last Position information";
    info->addResponse<PositionDto::ObjectWrapper>(Status::CODE_200,
                                                  "application/json");
  }
  ADD_CORS(getPosition)
  ENDPOINT("GET", "/GBS/GetPosition", getPosition)
  {
    auto dto = database->GetPosition();
    auto response = createDtoResponse(Status::CODE_200, dto);
    return response;
  }

  ENDPOINT_INFO(postPosition)
  {
    info->summary = "Post the current/last Position information";
    info->addConsumes<PositionDto::ObjectWrapper>("application/json");
    info->addResponse<PositionDto::ObjectWrapper>(Status::CODE_200,
                                                  "application/json");
  }
  ADD_CORS(postPosition)
  ENDPOINT("POST", "/GBS/PostPosition", postPosition,
           BODY_DTO(PositionDto::ObjectWrapper, dto))
  {
    auto echo = database->PostPosition(dto);
    auto response = createDtoResponse(Status::CODE_200, echo);
    return response;
  }

/**
 *  Finish ENDPOINTs generation ('ApiController' codegen)
 */
#include OATPP_CODEGEN_END(ApiController)
};

#endif /* MyController_hpp */
