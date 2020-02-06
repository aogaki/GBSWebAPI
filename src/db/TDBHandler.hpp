#ifndef TDBHandler_hpp
#define TDBHandler_hpp 1

#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/builder/stream/helpers.hpp>
#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/pool.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>

#include "dto/DTOs.hpp"
#include "oatpp/core/Types.hpp"

using oatpp::data::mapping::type::List;

class TDBHandler
{
 public:
  TDBHandler();
  ~TDBHandler(){};

  EnergyDto::ObjectWrapper GetEnergy();
  List<EnergyDto::ObjectWrapper>::ObjectWrapper GetEnergyList();

  List<FluxDto::ObjectWrapper>::ObjectWrapper GetFluxList();

  PositionDto::ObjectWrapper GetPosition();
  PositionDto::ObjectWrapper PostPosition(PositionDto::ObjectWrapper dto);

 private:
  mongocxx::pool fPool;
};

#endif
