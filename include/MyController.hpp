//
//  MyController.hpp
//  web-starter-project
//
//  Created by Leonid on 2/12/18.
//  Copyright © 2018 oatpp. All rights reserved.
//

#ifndef MyController_hpp
#define MyController_hpp

#include <iostream>

#include "MyDTOs.hpp"
// #include "TDBHandler.hpp"

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
   // protected:
public:
   MyController(const std::shared_ptr<ObjectMapper> &objectMapper)
      : oatpp::web::server::api::ApiController(objectMapper)
   {
   }

private:
   // Think!  How to implementing the DB handler class
   // Now making static functions to handle MongoDB
   // Hmm I feel it is too crazy

   // OATPP_COMPONENT(std::shared_ptr<TDatabase>, fDatabase);
   // OATPP_COMPONENT(std::shared_ptr<TDBHandler>, fDatabase);

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

   ENDPOINT_ASYNC("OPTIONS", "*", Cors)
   {
      ENDPOINT_ASYNC_INIT(Cors);

      Action act() override
      {
         // TODO check origin here if needed:
         // auto origin = request->getHeader("Origin");
         // ...
         // Now bitch style!  All holes are open! Ahhhhhhh
         // std::cout << "hit from\t" << request->getHeader("Origin")->std_str()
         //           << std::endl;

         auto response = controller->createResponse(Status::CODE_200, "");

         response->putHeader("Access-Control-Allow-Origin", "*");
         response->putHeader("Access-Control-Allow-Methods", "*");
         response->putHeader("Access-Control-Allow-Headers", "*");

         return _return(response);
      }
   };

   ENDPOINT_INFO(root)
   {
      info->summary = "Index.html page";
      info->addResponse<TestDto::ObjectWrapper>(Status::CODE_200, "text/html");
   }
   ENDPOINT_ASYNC("GET", "/", root)
   {
      ENDPOINT_ASYNC_INIT(root);

      Action act() override
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
         auto response = controller->createResponse(Status::CODE_200, html);
         response->putHeader(Header::CONTENT_TYPE, "text/html");
         // return response;

         // auto dto = HelloDto::createShared();
         // dto->message = "Hello Async!";
         // dto->server = Header::Value::SERVER;
         // dto->userAgent = request->getHeader(Header::USER_AGENT);
         // return _return(controller->createDtoResponse(Status::CODE_200, dto));
         return _return(response);
      }
   };

   ENDPOINT_INFO(getAll)
   {
      info->summary = "get all stored data";
      info->addResponse<List<GBSDto::ObjectWrapper>::ObjectWrapper>(
         Status::CODE_200, "application/json");
   }
   ENDPOINT_ASYNC("GET", "/GBS/GetAll", getAll)
   {
      // Original example has no ; after ENDPOINT_ASYNC_INIT(),
      // It looks like a define macro
      // If got error, check this
      ENDPOINT_ASYNC_INIT(getAll);
      Action act() override
      {
         auto result = oatpp::data::mapping::type::List<
            GBSDto::ObjectWrapper>::createShared();

         mongocxx::client conn{mongocxx::uri{"mongodb://daq:nim2camac@192.168.161.73/GBS"}};
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
         auto response = controller->createDtoResponse(Status::CODE_200, result);
         response->putHeader("Access-Control-Allow-Origin", "*");
         return _return(response);
      }
   };

   ENDPOINT_INFO(getFlux)
   {
      info->summary = "Get Flux information";
      info->addResponse<List<FluxDto::ObjectWrapper>::ObjectWrapper>(
         Status::CODE_200, "application/json");
   }
   ENDPOINT_ASYNC("GET", "/GBS/GetFlux", getFlux)
   {
      ENDPOINT_ASYNC_INIT(getFlux);
      Action act() override
      {
         auto result = oatpp::data::mapping::type::List<
            FluxDto::ObjectWrapper>::createShared();

         mongocxx::client conn{mongocxx::uri{"mongodb://daq:nim2camac@192.168.161.73/GBS"}};
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
         auto response = controller->createDtoResponse(Status::CODE_200, result);
         response->putHeader("Access-Control-Allow-Origin", "*");
         // fDatabase->Test();
         return _return(response);
      }
   };

   ENDPOINT_INFO(getTest)
   {
      info->summary = "Get Test information";
      info->addResponse<List<TestDto::ObjectWrapper>::ObjectWrapper>(
         Status::CODE_200, "application/json");
   }
   ENDPOINT_ASYNC("GET", "/GBS/GetTest", getTest)
   {
      ENDPOINT_ASYNC_INIT(getTest);
      Action act() override
      {
         auto result = oatpp::data::mapping::type::List<
            TestDto::ObjectWrapper>::createShared();

         mongocxx::client conn{mongocxx::uri{"mongodb://daq:nim2camac@192.168.161.73/GBS"}};
         auto collection = conn["GBS"]["Test"];
         auto cursor = collection.find({});
         auto counter = 0;
         for (auto doc : cursor) {
            auto dto = TestDto::createShared();
            dto->id = doc["_id"].get_oid().value.to_string().c_str();
            dto->mean = doc["mean"].get_utf8().value.to_string().c_str();
            dto->fwhm = doc["fwhm"].get_utf8().value.to_string().c_str();
            dto->imagePath = doc["imagePath"].get_utf8().value.to_string().c_str();
            dto->time = doc["time"].get_utf8().value.to_string().c_str();

            result->pushBack(dto);
         }
         auto response = controller->createDtoResponse(Status::CODE_200, result);
         response->putHeader("Access-Control-Allow-Origin", "*");
         return _return(response);
      }
   };

   ENDPOINT_INFO(getEnergy)
   {
      info->summary = "Get Energy information";
      info->addResponse<List<EnergyDto::ObjectWrapper>::ObjectWrapper>(
         Status::CODE_200, "application/json");
   }
   ENDPOINT_ASYNC("GET", "/GBS/GetEnergy", getEnergy)
   {
      ENDPOINT_ASYNC_INIT(getEnergy);
      Action act() override
      {
         auto result = oatpp::data::mapping::type::List<
            EnergyDto::ObjectWrapper>::createShared();

         mongocxx::client conn{mongocxx::uri{"mongodb://daq:nim2camac@192.168.161.73/GBS"}};
         auto collection = conn["GBS"]["Energy"];
         auto cursor = collection.find({});
         auto counter = 0;
         for (auto doc : cursor) {
            // bsoncxx::document::element e = doc["fit"];
            // std::cout << bsoncxx::to_string(e.type()) << std::endl;
            auto dto = EnergyDto::createShared();
            dto->id = doc["_id"].get_oid().value.to_string().c_str();
            dto->mean = doc["mean"].get_utf8().value.to_string().c_str();
            dto->fwhm = doc["fwhm"].get_utf8().value.to_string().c_str();
            dto->time = doc["time"].get_utf8().value.to_string().c_str();
            dto->fit = doc["fit"].get_utf8().value.to_string().c_str();
            // auto fitJSON = bsoncxx::to_json(doc["fit"].get_document().view());
            // dto->fit = fitJSON.c_str();
            // dto->fit = doc["fit"].get_utf8().value.to_string().c_str();
            // dto->fit = doc["fit"].get_document().value.data();

            result->pushBack(dto);
         }
         auto response = controller->createDtoResponse(Status::CODE_200, result);
         response->putHeader("Access-Control-Allow-Origin", "*");
         return _return(response);
      }
   };

   ENDPOINT_INFO(getPosition)
   {
      info->summary = "Get Position information";
      info->addResponse<List<PositionDto::ObjectWrapper>::ObjectWrapper>(
         Status::CODE_200, "application/json");
   }
   ENDPOINT_ASYNC("GET", "/GBS/GetPosition", getPosition)
   {
      ENDPOINT_ASYNC_INIT(getPosition);
      Action act() override
      {
         auto result = oatpp::data::mapping::type::List<
            PositionDto::ObjectWrapper>::createShared();

         mongocxx::client conn{mongocxx::uri{"mongodb://daq:nim2camac@192.168.161.73/GBS"}};
         auto collection = conn["GBS"]["Position"];
         auto cursor = collection.find({});
         for (auto doc : cursor) {
            auto dto = PositionDto::createShared();
            dto->id = doc["_id"].get_oid().value.to_string().c_str();
            dto->img2D = doc["img2D"].get_utf8().value.to_string().c_str();
            dto->comment2D = doc["comment2D"].get_utf8().value.to_string().c_str();
            dto->imgH = doc["imgH"].get_utf8().value.to_string().c_str();
            dto->commentH = doc["commentH"].get_utf8().value.to_string().c_str();
            dto->imgV = doc["imgV"].get_utf8().value.to_string().c_str();
            dto->commentV = doc["commentV"].get_utf8().value.to_string().c_str();
            dto->commentAll =
               doc["commentAll"].get_utf8().value.to_string().c_str();

            result->pushBack(dto);
         }
         auto response = controller->createDtoResponse(Status::CODE_200, result);
         response->putHeader("Access-Control-Allow-Origin", "*");
         return _return(response);
      }
   };

   ENDPOINT_INFO(postPosition)
   {
      info->summary =
         "Post position information.  The images are in base64 format";
      info->addConsumes<PositionDto::ObjectWrapper>("application/json");
      info->addResponse<List<PositionDto::ObjectWrapper>::ObjectWrapper>(
         Status::CODE_200, "text/plain");
   }
   ENDPOINT_ASYNC("POST", "/GBS/PostPosition", postPosition)
   {
      ENDPOINT_ASYNC_INIT(postPosition);

      Action act() override
      {
         return request->readBodyToDtoAsync<PositionDto>(
            this, &postPosition::returnResponse,
            controller->getDefaultObjectMapper());
      }

      Action returnResponse(const PositionDto::ObjectWrapper &body)
      {
         mongocxx::client conn{mongocxx::uri{"mongodb://daq:nim2camac@192.168.161.73/GBS"}};
         auto collection = conn["GBS"]["Position"];
         bsoncxx::builder::stream::document buf{};
         buf << "img2D" << body->img2D->std_str() << "comment2D"
             << body->comment2D->std_str() << "imgH" << body->imgH->std_str()
             << "commentH" << body->commentH->std_str() << "imgV"
             << body->imgV->std_str() << "commentV" << body->commentV->std_str()
             << "commentAll" << body->commentAll->std_str() << "time"
             << std::to_string(time(0));
         collection.insert_one(buf.view());
         buf.clear();

         // auto response = controller->createResponse(Status::CODE_200,
         // "success");
         body->img2D = "";  // echo response.  but image is empty to reduce trafic
         body->imgH = "";
         body->imgV = "";
         auto response = controller->createDtoResponse(Status::CODE_200, body);
         response->putHeader("Access-Control-Allow-Origin", "*");
         return _return(response);
      }
   };

   ENDPOINT_INFO(expLog)
   {
      info->summary = "POST Experiment log";
      info->addConsumes<ExpLogDto::ObjectWrapper>("application/json");
      info->addResponse<List<ExpLogDto::ObjectWrapper>::ObjectWrapper>(
         Status::CODE_200, "application/json");
   }
   ENDPOINT_ASYNC("POST", "/GBS/PostExpLog", expLog)
   {
      ENDPOINT_ASYNC_INIT(expLog);

      Action act() override
      {
         return request->readBodyToDtoAsync<ExpLogDto>(
            this, &expLog::returnResponse, controller->getDefaultObjectMapper());
      }

      Action returnResponse(const ExpLogDto::ObjectWrapper &body)
      {
         mongocxx::client conn{mongocxx::uri{"mongodb://daq:nim2camac@192.168.161.73/GBS"}};
         auto collection = conn["GBS"]["ExpLog"];
         bsoncxx::builder::stream::document buf{};
         buf << "expName" << body->expName->std_str() << "comment"
             << body->comment->std_str() << "contactPerson"
             << body->contactPerson->std_str() << "dateEnd"
             << body->dateEnd->std_str() << "dateStart"
             << body->dateStart->std_str() << "mailAddress"
             << body->mailAddress->std_str() << "time" << std::to_string(time(0));
         collection.insert_one(buf.view());
         buf.clear();

         auto response = controller->createDtoResponse(Status::CODE_200, body);
         response->putHeader("Access-Control-Allow-Origin", "*");
         return _return(response);
      }
   };

   ENDPOINT_INFO(fluxPar)
   {
      info->summary = "POST Parameter of Flux Monitor";
      info->addConsumes<FluxParDto::ObjectWrapper>("application/json");
      info->addResponse<List<FluxParDto::ObjectWrapper>::ObjectWrapper>(
         Status::CODE_200, "application/json");
   }
   ENDPOINT_ASYNC("POST", "/GBS/PostFluxPar", fluxPar)
   {
      ENDPOINT_ASYNC_INIT(fluxPar);

      Action act() override
      {
         return request->readBodyToDtoAsync<FluxParDto>(
            this, &fluxPar::returnResponse, controller->getDefaultObjectMapper());
      }

      Action returnResponse(const FluxParDto::ObjectWrapper &body)
      {
         mongocxx::client conn{mongocxx::uri{"mongodb://daq:nim2camac@192.168.161.73/GBS"}};
         auto collection = conn["GBS"]["FluxPar"];
         bsoncxx::builder::stream::document buf{};
         buf << "polarity" << body->polarity->std_str()
             << "DCOffset" << body->DCOffset->std_str()
             << "threshold" << body->threshold->std_str()
             << "start" << body->start->std_str()
             << "end" << body->end->std_str()
             << "ch" << body->ch->std_str()
             << "timeInterval" << body->timeInterval->std_str()
             << "time" << std::to_string(time(0));
         collection.insert_one(buf.view());
         buf.clear();

         auto response = controller->createDtoResponse(Status::CODE_200, body);
         response->putHeader("Access-Control-Allow-Origin", "*");
         return _return(response);
      }
   };

   ENDPOINT_INFO(polMeterPar)
   {
      info->summary = "POST Parameter of Flux Monitor";
      info->addConsumes<PolMeterParDto::ObjectWrapper>("application/json");
      info->addResponse<List<PolMeterParDto::ObjectWrapper>::ObjectWrapper>(
         Status::CODE_200, "application/json");
   }
   ENDPOINT_ASYNC("POST", "/GBS/PostPolMeterPar", polMeterPar)
   {
      ENDPOINT_ASYNC_INIT(polMeterPar);

      Action act() override
      {
         return request->readBodyToDtoAsync<PolMeterParDto>(
            this, &polMeterPar::returnResponse, controller->getDefaultObjectMapper());
      }

      Action returnResponse(const PolMeterParDto::ObjectWrapper &body)
      {
         mongocxx::client conn{mongocxx::uri{"mongodb://daq:nim2camac@192.168.161.73/GBS"}};
         auto collection = conn["GBS"]["PolMeterPar"];
         bsoncxx::builder::stream::document buf{};
         buf << "polarity" << body->polarity->std_str()
             << "DCOffset" << body->DCOffset->std_str()
             << "threshold" << body->threshold->std_str()
             << "timeInterval" << body->timeInterval->std_str()
             << "inCh" << body->inCh->std_str()
             << "outCh1" << body->outCh1->std_str()
             << "outCh2" << body->outCh2->std_str()
             << "beamCh" << body->beamCh->std_str()
             << "time" << std::to_string(time(0));
         collection.insert_one(buf.view());
         buf.clear();

         auto response = controller->createDtoResponse(Status::CODE_200, body);
         response->putHeader("Access-Control-Allow-Origin", "*");
         return _return(response);
      }
   };

   ENDPOINT_INFO(getPolMeterPar)
   {
      info->summary = "Get PolMeter parameter information";
      info->addResponse<List<PolMeterParDto::ObjectWrapper>::ObjectWrapper>(
         Status::CODE_200, "application/json");
   }
   ENDPOINT_ASYNC("GET", "/GBS/GetPolMeterPar", getPolMeterPar)
   {
      ENDPOINT_ASYNC_INIT(getPolMeterPar);
      Action act() override
      {
         auto result = oatpp::data::mapping::type::List<
            PolMeterParDto::ObjectWrapper>::createShared();

         mongocxx::client conn{mongocxx::uri{"mongodb://daq:nim2camac@192.168.161.73/GBS"}};
         auto collection = conn["GBS"]["PolMeterPar"];

         // Only taking last data
         auto opts = mongocxx::options::find{};
         opts.limit(1);
         auto order = document{} << "time" << -1 << finalize;
         opts.sort(order.view());
         auto cursor = collection.find({}, opts);

         for (auto doc : cursor) {
            auto dto = PolMeterParDto::createShared();
            dto->id = doc["_id"].get_oid().value.to_string().c_str();
            dto->polarity = doc["polarity"].get_utf8().value.to_string().c_str();
            dto->DCOffset = doc["DCOffset"].get_utf8().value.to_string().c_str();
            dto->threshold = doc["threshold"].get_utf8().value.to_string().c_str();
            dto->timeInterval = doc["timeInterval"].get_utf8().value.to_string().c_str();
            dto->inCh = doc["inCh"].get_utf8().value.to_string().c_str();
            dto->outCh1 = doc["outCh1"].get_utf8().value.to_string().c_str();
            dto->outCh2 = doc["outCh2"].get_utf8().value.to_string().c_str();
            dto->beamCh = doc["beamCh"].get_utf8().value.to_string().c_str();
            dto->time = doc["time"].get_utf8().value.to_string().c_str();
            result->pushBack(dto);
         }

         auto response = controller->createDtoResponse(Status::CODE_200, result);
         response->putHeader("Access-Control-Allow-Origin", "*");
         return _return(response);
      }
   };


   
/**
 *  Finish ENDPOINTs generation ('ApiController' codegen)
 */
#include OATPP_CODEGEN_END(ApiController)
};

#endif /* MyController_hpp */
