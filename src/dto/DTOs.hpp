//
//  MyDto.hpp
//  crud
//
//  Created by Leonid on 3/13/18.
//  Copyright © 2018 oatpp. All rights reserved.
//

#ifndef DTOs_hpp
#define DTOs_hpp

#include "oatpp/core/data/mapping/type/Object.hpp"
#include "oatpp/core/macro/codegen.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 *  Data Transfer Object. Object containing fields only.
 *  Used in API for serialization/deserialization and validation
 */
class MyDto : public oatpp::data::mapping::type::Object
{
  DTO_INIT(MyDto, Object)

  DTO_FIELD(Int32, statusCode);
  DTO_FIELD(String, message);
};

class EnergyDto : public oatpp::data::mapping::type::Object
{
  DTO_INIT(EnergyDto, Object)

  DTO_FIELD(String, id);
  DTO_FIELD(String, mean);
  DTO_FIELD(String, fwhm);
  DTO_FIELD(String, time);
  DTO_FIELD(String, fit);
};

class FluxDto : public oatpp::data::mapping::type::Object
{
  DTO_INIT(FluxDto, Object)

  DTO_FIELD(String, id);
  DTO_FIELD(String, count);
  DTO_FIELD(String, hz);
  DTO_FIELD(String, time);
};

class PositionDto : public oatpp::data::mapping::type::Object
{
  DTO_INIT(PositionDto, Object)

  DTO_FIELD(String, id);
  DTO_FIELD(String, img2D);
  DTO_FIELD(String, imgH);
  DTO_FIELD(String, imgV);
  DTO_FIELD(String, comment2D);
  DTO_FIELD(String, commentH);
  DTO_FIELD(String, commentV);
  DTO_FIELD(String, commentAll);
};

#include OATPP_CODEGEN_END(DTO)

#endif /* DTOs_hpp */
