/*
// Copyright (c) 2015 Intel Corporation
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
*/

#include "sum.h"

#include <stdlib.h>

extern void* wasm_module_0_memory_base;

void* init_sum_c(int n) {
  // Just use the linear memory from the wasm module.
  int* tab = wasm_module_0_memory_base;

  int i;
  for (i = 0; i < n; i++) {
    tab[i] = i;
  }

  return wasm_module_0_memory_base;
}

int run_sum_c(void* data, int n) {
  int* tab = data;
  int sum = 0;
  int i;
  for (i = 0; i < n; i++) {
    sum += tab[i];
  }
  return sum;
}
