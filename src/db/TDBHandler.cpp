#include <iostream>

#include "TDBHandler.hpp"

using bsoncxx::builder::stream::document;
using bsoncxx::builder::stream::finalize;

using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_document;

TDBHandler::TDBHandler()
    : fPool(mongocxx::uri("mongodb://daq:nim2camac@172.18.4.56/GBS"))
{
}

EnergyDto::ObjectWrapper TDBHandler::GetEnergy()
{
  auto conn = fPool.acquire();
  auto collection = (*conn)["GBS"]["Energy"];

  // cursor has only one data.  opts.limit(1) means take one data from DB
  // order shows the sorting parameter (sort by time, -1 means reverse order)
  auto opts = mongocxx::options::find{};
  opts.limit(1);
  auto order = document{} << "time" << -1 << finalize;
  opts.sort(order.view());
  auto cursor = collection.find({}, opts);

  auto dto = EnergyDto::createShared();
  for (auto doc : cursor) {
    dto->id = doc["_id"].get_oid().value.to_string().c_str();
    dto->mean = doc["mean"].get_utf8().value.to_string().c_str();
    dto->fwhm = doc["fwhm"].get_utf8().value.to_string().c_str();
    dto->time = doc["time"].get_utf8().value.to_string().c_str();
    dto->fit = doc["fit"].get_utf8().value.to_string().c_str();
  }

  return dto;
}

List<EnergyDto::ObjectWrapper>::ObjectWrapper TDBHandler::GetEnergyList()
{
  auto conn = fPool.acquire();
  auto collection = (*conn)["GBS"]["Energy"];
  auto cursor = collection.find({});

  auto result = List<EnergyDto::ObjectWrapper>::createShared();
  for (auto doc : cursor) {
    auto dto = EnergyDto::createShared();
    dto->id = doc["_id"].get_oid().value.to_string().c_str();
    dto->mean = doc["mean"].get_utf8().value.to_string().c_str();
    dto->fwhm = doc["fwhm"].get_utf8().value.to_string().c_str();
    dto->time = doc["time"].get_utf8().value.to_string().c_str();
    dto->fit = doc["fit"].get_utf8().value.to_string().c_str();
    result->pushBack(dto);
  }

  return result;
}

List<FluxDto::ObjectWrapper>::ObjectWrapper TDBHandler::GetFluxList()
{
  auto conn = fPool.acquire();
  auto collection = (*conn)["GBS"]["Flux"];
  auto cursor = collection.find({});

  auto result = List<FluxDto::ObjectWrapper>::createShared();
  for (auto doc : cursor) {
    auto dto = FluxDto::createShared();
    dto->id = doc["_id"].get_oid().value.to_string().c_str();
    dto->count = doc["count"].get_utf8().value.to_string().c_str();
    dto->hz = doc["hz"].get_utf8().value.to_string().c_str();
    dto->time = doc["time"].get_utf8().value.to_string().c_str();

    result->pushBack(dto);
  }

  return result;
}

PositionDto::ObjectWrapper TDBHandler::GetPosition()
{
  auto conn = fPool.acquire();
  auto collection = (*conn)["GBS"]["Position"];

  // cursor has only one data.  opts.limit(1) means take one data from DB
  // order shows the sorting parameter (sort by time, -1 means reverse order)
  auto opts = mongocxx::options::find{};
  opts.limit(1);
  auto order = document{} << "time" << -1 << finalize;
  opts.sort(order.view());
  auto cursor = collection.find({}, opts);

  auto dto = PositionDto::createShared();
  for (auto doc : cursor) {
    dto->id = doc["_id"].get_oid().value.to_string().c_str();
    dto->img2D = doc["img2D"].get_utf8().value.to_string().c_str();
    dto->comment2D = doc["comment2D"].get_utf8().value.to_string().c_str();
    dto->imgH = doc["imgH"].get_utf8().value.to_string().c_str();
    dto->commentH = doc["commentH"].get_utf8().value.to_string().c_str();
    dto->imgV = doc["imgV"].get_utf8().value.to_string().c_str();
    dto->commentV = doc["commentV"].get_utf8().value.to_string().c_str();
    dto->commentAll = doc["commentAll"].get_utf8().value.to_string().c_str();
  }

  return dto;
}

PositionDto::ObjectWrapper TDBHandler::PostPosition(
    PositionDto::ObjectWrapper dto)
{
  auto conn = fPool.acquire();
  auto collection = (*conn)["GBS"]["Position"];

  bsoncxx::builder::stream::document buf{};
  buf << "img2D" << dto->img2D->std_str() << "comment2D"
      << dto->comment2D->std_str() << "imgH" << dto->imgH->std_str()
      << "commentH" << dto->commentH->std_str() << "imgV"
      << dto->imgV->std_str() << "commentV" << dto->commentV->std_str()
      << "commentAll" << dto->commentAll->std_str() << "time"
      << std::to_string(time(0));
  collection.insert_one(buf.view());
  buf.clear();

  // echo response.  but image is empty to reduce the trafic
  dto->img2D = "";
  dto->imgH = "";
  dto->imgV = "";

  return dto;
}
