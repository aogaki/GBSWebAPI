//
//  UserDto.hpp
//  crud
//
//  Created by Leonid on 3/13/18.
//  Copyright © 2018 oatpp. All rights reserved.
//

#ifndef MyDTOs_hpp
#define MyDTOs_hpp

#include "oatpp/core/Types.hpp"
#include "oatpp/core/macro/codegen.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 *  Data Transfer Object. Object containing fields only.
 *  Used in API for serialization/deserialization and validation
 */

class GBSDto : public oatpp::data::mapping::type::Object
{
  DTO_INIT(GBSDto, Object)

  DTO_FIELD(String, id);
  DTO_FIELD(String, title);
  DTO_FIELD(String, content);
  DTO_FIELD(String, imagePath);
};

class FluxDto : public oatpp::data::mapping::type::Object
{
  DTO_INIT(FluxDto, Object)

  DTO_FIELD(String, id);
  DTO_FIELD(String, count);
  DTO_FIELD(String, hz);
  DTO_FIELD(String, time);
};

class TestDto : public oatpp::data::mapping::type::Object
{
  DTO_INIT(TestDto, Object)

  DTO_FIELD(String, id);
  DTO_FIELD(String, mean);
  DTO_FIELD(String, fwhm);
  DTO_FIELD(String, imagePath);
  DTO_FIELD(String, time);
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

class ExpLogDto : public oatpp::data::mapping::type::Object
{
  DTO_INIT(ExpLogDto, Object)

  DTO_FIELD(String, id);
  DTO_FIELD(String, expName);
  DTO_FIELD(String, comment);
  DTO_FIELD(String, contactPerson);
  DTO_FIELD(String, dateEnd);
  DTO_FIELD(String, dateStart);
  DTO_FIELD(String, mailAddress);
};

#include OATPP_CODEGEN_END(DTO)

#endif /* MyDTOs_hpp */
