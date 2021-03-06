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

#include "src/common.h"
#include "src/vif.h"
#include "src/vrf.h"

TEST(VRF, Prove) {
  int rv = ERROR;
  Params p = NULL;
  EC_POINT *pk = NULL;
  EC_POINT *pk_bad = NULL;
  BIGNUM *sk = NULL;
  BIGNUM *val = NULL;
  VRFProof pf = NULL;
  const uint8_t input[] = "www.example.com";
  const uint8_t input_bad[] = "www.evil.com";
 
  CHECK_A (p = Params_new (P256));
  CHECK_A (pf = VRFProof_new (p));
  CHECK_A (pk = Params_point_new(p));
  CHECK_A (sk = BN_new());
  CHECK_A (val = BN_new());
  CHECK_A (pk_bad = Params_point_new(p));
  CHECK_C (Params_rand_point (p, pk_bad));

  CHECK_C (VRF_keygen (p, pk, sk));
  CHECK_C (VRF_eval (p, sk, input, sizeof input, val, pf));
  EXPECT_EQ (VRF_verify (p, pk, input, sizeof input, val, pf), OKAY);
  EXPECT_EQ (VRF_verify (p, pk, input_bad, sizeof input_bad, val, pf), ERROR);
  EXPECT_EQ (VRF_verify (p, pk, input, sizeof input_bad, val, pf), ERROR);
  EXPECT_EQ (VRF_verify (p, pk, input, 0, val, pf), ERROR);
  EXPECT_EQ (VRF_verify (p, pk_bad, input, sizeof input, val, pf), ERROR);
  EXPECT_EQ (VRF_verify (p, pk, input, sizeof input, sk, pf), ERROR);

cleanup:
  if (pk) EC_POINT_clear_free (pk);
  if (sk) BN_clear_free (sk);
  if (p) Params_free (p);
  if (pf) VRFProof_free (pf);
  if (pk_bad) EC_POINT_clear_free(pk_bad);
  if (val) BN_clear_free(val);
  EXPECT_TRUE (rv == OKAY);
}


