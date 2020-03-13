#ifndef TDBHandler_hpp
#define TDBHandler_hpp 1

#include <TCanvas.h>
#include <TGraph.h>

#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/builder/stream/helpers.hpp>
#include <bsoncxx/json.hpp>
#include <memory>
#include <mongocxx/client.hpp>
#include <mongocxx/pool.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>

#include "dto/DTOs.hpp"
#include "oatpp/core/Types.hpp"
#include "oatpp/core/concurrency/SpinLock.hpp"

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

  CalibrationDto::ObjectWrapper PostCalibration(
      CalibrationDto::ObjectWrapper dto);
  List<CalibrationDto::ObjectWrapper>::ObjectWrapper GetCalibrationList();

  List<VacMonDto::ObjectWrapper>::ObjectWrapper GetVacMonList();
  VacMonGraphDto::ObjectWrapper GetVacMonGraph();

 private:
  oatpp::concurrency::SpinLock fLock;

  mongocxx::pool fPool;
  mongocxx::pool fEliadePool;

  std::unique_ptr<TGraph> fGraph;
  std::unique_ptr<TCanvas> fCanvas;
};

#endif
