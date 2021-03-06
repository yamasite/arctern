/*
 * Copyright (C) 2019-2020 Zilliz. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <gtest/gtest.h>
#include "gis/parser.h"

#define ASSERT_TOKEN(str_ptr, token_ptr, expect_type)    \
  arctern::gis::parser::NextToken(str_ptr, (token_ptr)); \
  ASSERT_EQ((token_ptr)->type, (expect_type));           \
  str_ptr = (token_ptr)->start + (token_ptr)->len;

#define ISValid_TRUE(str)   \
  ptr = (const char*)(str); \
  ASSERT_TRUE(arctern::gis::parser::IsValidWkt(ptr));

#define ISValid_FALSE(str)  \
  ptr = (const char*)(str); \
  ASSERT_FALSE(arctern::gis::parser::IsValidWkt(ptr));

TEST(parser_test, next_token) {
  auto point = (const char*)"point ( 2.3  2.89 ) ";
  arctern::gis::parser::TokenInfo token;

  ASSERT_TOKEN(point, &token, arctern::gis::parser::TokenType::WktKey);
  ASSERT_TOKEN(point, &token, arctern::gis::parser::TokenType::LeftBracket);
  ASSERT_TOKEN(point, &token, arctern::gis::parser::TokenType::Number);
  ASSERT_TOKEN(point, &token, arctern::gis::parser::TokenType::Number);
  ASSERT_TOKEN(point, &token, arctern::gis::parser::TokenType::RightBracket);

  ASSERT_FALSE(arctern::gis::parser::NextToken(point, &token));
}

TEST(parser_test, next_token4) {
  auto point = (const char*)"point ( 2.3  2.89 )";
  arctern::gis::parser::TokenInfo token;

  ASSERT_TOKEN(point, &token, arctern::gis::parser::TokenType::WktKey);
  ASSERT_TOKEN(point, &token, arctern::gis::parser::TokenType::LeftBracket);
  ASSERT_TOKEN(point, &token, arctern::gis::parser::TokenType::Number);
  ASSERT_TOKEN(point, &token, arctern::gis::parser::TokenType::Number);
  ASSERT_TOKEN(point, &token, arctern::gis::parser::TokenType::RightBracket);

  ASSERT_FALSE(arctern::gis::parser::NextToken(point, &token));
}

TEST(parser_test, next_token2) {
  auto point = (const char*)"polygon empty";
  arctern::gis::parser::TokenInfo token;

  ASSERT_TOKEN(point, &token, arctern::gis::parser::TokenType::WktKey);
  ASSERT_TOKEN(point, &token, arctern::gis::parser::TokenType::Empty);

  ASSERT_FALSE(arctern::gis::parser::NextToken(point, &token));
}

TEST(parser_test, next_token3) {
  auto point = (const char*)"polygon empty ";
  arctern::gis::parser::TokenInfo token;

  ASSERT_TOKEN(point, &token, arctern::gis::parser::TokenType::WktKey);
  ASSERT_TOKEN(point, &token, arctern::gis::parser::TokenType::Empty);

  ASSERT_FALSE(arctern::gis::parser::NextToken(point, &token));
}

TEST(parser_test, next_token5) {
  auto point = (const char*)"polygon((0 0, 0 1, 1 1, 0 0)) ";
  arctern::gis::parser::TokenInfo token;

  ASSERT_TOKEN(point, &token, arctern::gis::parser::TokenType::WktKey);
  ASSERT_TOKEN(point, &token, arctern::gis::parser::TokenType::LeftBracket);
  ASSERT_TOKEN(point, &token, arctern::gis::parser::TokenType::LeftBracket);
  ASSERT_TOKEN(point, &token, arctern::gis::parser::TokenType::Number);
  ASSERT_TOKEN(point, &token, arctern::gis::parser::TokenType::Number);
  ASSERT_TOKEN(point, &token, arctern::gis::parser::TokenType::Comma);
  ASSERT_TOKEN(point, &token, arctern::gis::parser::TokenType::Number);
  ASSERT_TOKEN(point, &token, arctern::gis::parser::TokenType::Number);
  ASSERT_TOKEN(point, &token, arctern::gis::parser::TokenType::Comma);
  ASSERT_TOKEN(point, &token, arctern::gis::parser::TokenType::Number);
  ASSERT_TOKEN(point, &token, arctern::gis::parser::TokenType::Number);
  ASSERT_TOKEN(point, &token, arctern::gis::parser::TokenType::Comma);
  ASSERT_TOKEN(point, &token, arctern::gis::parser::TokenType::Number);
  ASSERT_TOKEN(point, &token, arctern::gis::parser::TokenType::Number);
  ASSERT_TOKEN(point, &token, arctern::gis::parser::TokenType::RightBracket);
  ASSERT_TOKEN(point, &token, arctern::gis::parser::TokenType::RightBracket);

  ASSERT_FALSE(arctern::gis::parser::NextToken(point, &token));
}

TEST(parser_test, next_token6) {
  auto point = (const char*)"point ( 2.3abc  2.89 )";
  arctern::gis::parser::TokenInfo token;

  ASSERT_TOKEN(point, &token, arctern::gis::parser::TokenType::WktKey);
  ASSERT_TOKEN(point, &token, arctern::gis::parser::TokenType::LeftBracket);
  ASSERT_TOKEN(point, &token, arctern::gis::parser::TokenType::Unknown);
}

TEST(parser_test, isvalidwkt) {
  const char* ptr;

  ISValid_TRUE("point ( 12 12) ");
  ISValid_TRUE("point ( 12 12 12) ");
  ISValid_FALSE("point ( 12 )");
  ISValid_FALSE("point ( 12, 12 )");
  ISValid_FALSE("point ( 12 12, )");
  ISValid_FALSE("point ( 12 12 12, )");
  ISValid_FALSE("point ( 12 12abc)");
  ISValid_FALSE("point ( 12 12) abc");
  ISValid_FALSE("POLYGON ((1 1,1 2,2 2,2 1,1 1)),((dkjfkjd0 0,1 -1,3 4,-2 3,0 0))");
  ISValid_TRUE("MULTILINESTRING ( (0 0, 1 2), (0 0, 0 2, 1 1),(-1 2,3 4,9 -3,-4 100) )");
}
