// Copyright 2022 ILWrites (@PiKeeb)
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include "quantum.h"

#define LAYOUT_all( \
    k00, k01, k02, k03, k04, k05, k06, k07, k08, k09, k0a, k0b, k0c, k0d, k0e,   k0f, \
     k10,  k11, k12, k13, k14, k15, k16, k17, k18, k19, k1a, k1b, k1c,  k1d,     k1f, \
      k20,  k21, k22, k23, k24, k25, k26, k27, k28, k29, k2a, k2b,    k2e,       k2f, \
    k30, k31, k32, k33, k34, k35, k36, k37, k38, k39, k3a, k3b,   k3c,     k3e,  k3f, \
     k40,  k41,  k42,     k44,      k46,   k48,      k4a,   k4b,       k4d, k4e, k4f  \
) { \
    { k00, k01, k02, k03,   k04, k05,   k06, k07,   k08, k09,   k0a, k0b,   k0c,   k0d,   k0e,   k0f }, \
    { k10, k11, k12, k13,   k14, k15,   k16, k17,   k18, k19,   k1a, k1b,   k1c,   k1d,   KC_NO, k1f }, \
    { k20, k21, k22, k23,   k24, k25,   k26, k27,   k28, k29,   k2a, k2b,   KC_NO, KC_NO, k2e,   k2f }, \
    { k30, k31, k32, k33,   k34, k35,   k36, k37,   k38, k39,   k3a, k3b,   k3c,   KC_NO, k3e,   k3f }, \
    { k40, k41, k42, KC_NO, k44, KC_NO, k46, KC_NO, k48, KC_NO, k4a, k4b,   KC_NO, k4d,   k4e,   k4f }  \
}
