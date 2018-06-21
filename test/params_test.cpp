/*
 * Copyright (c) 2018, Henry Corrigan-Gibbs
 * 
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH
 * REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT,
 * INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
 * LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
 * OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 */

#include <gtest/gtest.h> 
#include "src/params.h"

class ParamTest : public ::testing::TestWithParam<CurveName> {
  public:
    Params p;

  virtual void SetUp() {
    p = Params_new (GetParam());
    EXPECT_TRUE (p != NULL);

  }

  virtual void TearDown() {
    if (p)
      Params_free (p);
  }
};


TEST_P (ParamTest, Init) {
  // Just tests setup and teardown
  EXPECT_TRUE (true);
}

TEST_P (ParamTest, RandPoint) {
  BIGNUM *x = NULL;
  
  x = BN_new ();
  EXPECT_TRUE (x);
  if (!x) {
    BN_free (x);
    return;
  }

  EXPECT_TRUE (OKAY == Params_rand_exponent (p, x));
  BN_free (x);
}

INSTANTIATE_TEST_CASE_P (Init,
                        ParamTest,
                        ::testing::Values(P256, P384, P521));
