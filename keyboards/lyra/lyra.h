/* Copyright 2020 KgOfHedgehogs & ILWrites
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include "quantum.h"

/* This is a shortcut to help you visually see your layout.
 *
 * The first section contains all of the arguments representing the physical
 * layout of the board and position of the keys.
 *
 * The second converts the arguments into a two-dimensional array which
 * represents the switch matrix.
 */

#define XXX KC_NO
#define ___ KC_TRNS

 #define LAYOUT_1UBKSPC_SPLSPC_1UNUM(k01, k37, k02, k38, k03, k39, k04, k40, k05, k41, k06, k42, k07, k43, k08, k44, k09, \
                                      k10, k46, k11, k47, k12, k48, k13, k49, k14, k50, k15, k51,   k16,   k17, k53, k18, \
                                       k19,  k55, k20, k56, k21, k57, k22, k58, k23, k59, k24,  k60,  k25, k26, k62, k27, \
                                      k28, k64, k29,      k65,     k31,      k32,      k33,  k69, k34, k70, k35, k71, k36) \
 {{ k01, k02, k03, k04, k05, k06, k07, k08, k09 },                                 \
  { k10, k11, k12, k13, k14, k15, k16, k17, k18 },                                 \
  { k19, k20, k21, k22, k23, k24, k25, k26, k27 },                                 \
  { k28, k29, XXX, k31, k32, k33, k34, k35, k36 },                                 \
  { k37, k38, k39, k40, k41, k42, k43, k44, XXX },                                 \
  { k46, k47, k48, k49, k50, k51, XXX, k53, XXX },                                 \
  { k55, k56, k57, k58, k59, k60, XXX, k62, XXX },                                 \
  { k64, k65, XXX, XXX, XXX, k69, k70, k71, XXX }}

 #define LAYOUT_1UBKSPC_SPLSPC_2UNUM(k01, k37, k02, k38, k03, k39, k04, k40, k05, k41, k06, k42, k07, k43, k08, k44, k09, \
                                        k10, k46, k11, k47, k12, k48, k13, k49, k14, k50, k15, k51,   k16,   k17, k53, k18, \
                                         k19,  k55, k20, k56, k21, k57, k22, k58, k23, k59, k24,  k60,  k25, k26, k62, k27, \
                                       k28, k64, k29,      k65,     k31,      k32,      k33,  k69, k34, k70,    k35,   k36) \
 {{ k01, k02, k03, k04, k05, k06, k07, k08, k09 },                                 \
  { k10, k11, k12, k13, k14, k15, k16, k17, k18 },                                 \
  { k19, k20, k21, k22, k23, k24, k25, k26, k27 },                                 \
  { k28, k29, XXX, k31, k32, k33, k34, k35, k36 },                                 \
  { k37, k38, k39, k40, k41, k42, k43, k44, XXX },                                 \
  { k46, k47, k48, k49, k50, k51, XXX, k53, XXX },                                 \
  { k55, k56, k57, k58, k59, k60, XXX, k62, XXX },                                 \
  { k64, k65, XXX, XXX, XXX, k69, k70, XXX, XXX }}

 #define LAYOUT_1UBKSPC_6USPC_1UNUM(k01, k37, k02, k38, k03, k39, k04, k40, k05, k41, k06, k42, k07, k43, k08, k44, k09, \
                                     k10, k46, k11, k47, k12, k48, k13, k49, k14, k50, k15, k51,   k16,   k17, k53, k18, \
                                      k19,  k55, k20, k56, k21, k57, k22, k58, k23, k59, k24,  k60,  k25, k26, k62, k27, \
                                    k28, k64, k29,                 k65,              k33,  k69, k34, k70, k35, k71, k36) \
 {{ k01, k02, k03, k04, k05, k06, k07, k08, k09 },                                 \
  { k10, k11, k12, k13, k14, k15, k16, k17, k18 },                                 \
  { k19, k20, k21, k22, k23, k24, k25, k26, k27 },                                 \
  { k28, k29, XXX, XXX, XXX, k33, k34, k35, k36 },                                 \
  { k37, k38, k39, k40, k41, k42, k43, k44, XXX },                                 \
  { k46, k47, k48, k49, k50, k51, XXX, k53, XXX },                                 \
  { k55, k56, k57, k58, k59, k60, XXX, k62, XXX },                                 \
  { k64, k65, XXX, XXX, XXX, k69, k70, k71, XXX }}

 #define LAYOUT_1UBKSPC_6USPC_2UNUM(k01, k37, k02, k38, k03, k39, k04, k40, k05, k41, k06, k42, k07, k43, k08, k44, k09, \
                                     k10, k46, k11, k47, k12, k48, k13, k49, k14, k50, k15, k51,   k16,   k17, k53, k18, \
                                      k19,  k55, k20, k56, k21, k57, k22, k58, k23, k59, k24,  k60,  k25, k26, k62, k27, \
                                    k28, k64, k29,                 k65,              k33,  k69, k34, k70,    k35,   k36) \
 {{ k01, k02, k03, k04, k05, k06, k07, k08, k09 },                                 \
  { k10, k11, k12, k13, k14, k15, k16, k17, k18 },                                 \
  { k19, k20, k21, k22, k23, k24, k25, k26, k27 },                                 \
  { k28, k29, XXX, XXX, XXX, k33, k34, k35, k36 },                                 \
  { k37, k38, k39, k40, k41, k42, k43, k44, XXX },                                 \
  { k46, k47, k48, k49, k50, k51, XXX, k53, XXX },                                 \
  { k55, k56, k57, k58, k59, k60, XXX, k62, XXX },                                 \
  { k64, k65, XXX, XXX, XXX, k69, k70, XXX, XXX }}


 #define LAYOUT_2UBKSPC_SPLSPC_1UNUM(k01, k37, k02, k38, k03, k39, k04, k40, k05, k41, k06,    k07,  k43, k08, k44, k09, \
                                     k10, k46, k11, k47, k12, k48, k13, k49, k14, k50, k15, k51,   k16,   k17, k53, k18, \
                                      k19,  k55, k20, k56, k21, k57, k22, k58, k23, k59, k24,  k60,  k25, k26, k62, k27, \
                                    k28, k64, k29,      k65,     k31,      k32,      k33,  k69, k34, k70, k35, k71, k36) \
 {{ k01, k02, k03, k04, k05, XXX, k07, k08, k09 },                                 \
  { k10, k11, k12, k13, k14, k15, k16, k17, k18 },                                 \
  { k19, k20, k21, k22, k23, k24, k25, k26, k27 },                                 \
  { k28, k29, XXX, k31, k32, k33, k34, k35, k36 },                                 \
  { k37, k38, k39, k40, k41, k42, k43, k44, XXX },                                 \
  { k46, k47, k48, k49, k50, k51, XXX, k53, XXX },                                 \
  { k55, k56, k57, k58, k59, k60, XXX, k62, XXX },                                 \
  { k64, k65, XXX, XXX, XXX, k69, k70, k71, XXX }}

 #define LAYOUT_2UBKSPC_SPLSPC_2UNUM(k01, k37, k02, k38, k03, k39, k04, k40, k05, k41, k42,    k07,  k43, k08, k44, k09, \
                                     k10, k46, k11, k47, k12, k48, k13, k49, k14, k50, k15, k51,   k16,   k17, k53, k18, \
                                      k19,  k55, k20, k56, k21, k57, k22, k58, k23, k59, k24,  k60,  k25, k26, k62, k27, \
                                    k28, k64, k29,      k65,     k31,      k32,      k33,  k69, k34, k70,    k35,   k36) \
 {{ k01, k02, k03, k04, k05, XXX, k07, k08, k09 },                                 \
  { k10, k11, k12, k13, k14, k15, k16, k17, k18 },                                 \
  { k19, k20, k21, k22, k23, k24, k25, k26, k27 },                                 \
  { k28, k29, XXX, k31, k32, k33, k34, k35, k36 },                                 \
  { k37, k38, k39, k40, k41, k42, k43, k44, XXX },                                 \
  { k46, k47, k48, k49, k50, k51, XXX, k53, XXX },                                 \
  { k55, k56, k57, k58, k59, k60, XXX, k62, XXX },                                 \
  { k64, k65, XXX, XXX, XXX, k69, k70, XXX, XXX }}

 #define LAYOUT_2UBKSPC_6USPC_1UNUM(k01, k37, k02, k38, k03, k39, k04, k40, k05, k41, k42,    k07,  k43, k08, k44, k09, \
                                    k10, k46, k11, k47, k12, k48, k13, k49, k14, k50, k15, k51,   k16,   k17, k53, k18, \
                                     k19,  k55, k20, k56, k21, k57, k22, k58, k23, k59, k24,  k60,  k25, k26, k62, k27, \
                                    k28, k64, k29,             k65,              k33,  k69, k34, k70, k35, k71, k36) \
 {{ k01, k02, k03, k04, k05, XXX, k07, k08, k09 },                                 \
  { k10, k11, k12, k13, k14, k15, k16, k17, k18 },                                 \
  { k19, k20, k21, k22, k23, k24, k25, k26, k27 },                                 \
  { k28, k29, XXX, XXX, XXX, k33, k34, k35, k36 },                                 \
  { k37, k38, k39, k40, k41, k42, k43, k44, XXX },                                 \
  { k46, k47, k48, k49, k50, k51, XXX, k53, XXX },                                 \
  { k55, k56, k57, k58, k59, k60, XXX, k62, XXX },                                 \
  { k64, k65, XXX, XXX, XXX, k69, k70, k71, XXX }}


#define LAYOUT_2UBKSPC_6USPC_2UNUM(k01, k37, k02, k38, k03, k39, k04, k40, k05, k41, k42,    k07,   k43, k08, k44, k09, \
                                    k10, k46, k11, k47, k12, k48, k13, k49, k14, k50, k15, k51,   k16,   k17, k53, k18, \
                                     k19,  k55, k20, k56, k21, k57, k22, k58, k23, k59, k24,  k60,  k25, k26, k62, k27, \
                                   k28, k64, k29,                k65,               k33,  k69, k34, k70,    k35,   k36) \
{{ k01, k02, k03, k04, k05, XXX, k07, k08, k09 },                                 \
 { k10, k11, k12, k13, k14, k15, k16, k17, k18 },                                 \
 { k19, k20, k21, k22, k23, k24, k25, k26, k27 },                                 \
 { k28, k29, XXX, XXX, XXX, k33, k34, k35, k36 },                                 \
 { k37, k38, k39, k40, k41, k42, k43, k44, XXX },                                 \
 { k46, k47, k48, k49, k50, k51, XXX, k53, XXX },                                 \
 { k55, k56, k57, k58, k59, k60, XXX, k62, XXX },                                 \
 { k64, k65, XXX, XXX, XXX, k69, k70, XXX, XXX }}
