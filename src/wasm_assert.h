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
#ifndef H_WASM_ASSERT
#define H_WASM_ASSERT

// TODO: find better.
#include "llvm/ADT/STLExtras.h"
#include "llvm/Analysis/Passes.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Transforms/Scalar.h"

#include <list>
#include <sstream>

#include "expression.h"

// Forward declaration.
class WasmFile;

class WasmAssert {
  protected:
    Expression* expr_;
    std::string name_;
    std::string mangled_name_;

  public:
    WasmAssert(Expression* expr) : expr_(expr) {
      // Asserts really don't have names but we will want one to call these.
      static int cnt = 0;
      std::ostringstream oss;
      oss << "wasm_assert_";

      // Finally, add the counter.
      oss << cnt;

      cnt++;

      name_ = oss.str();

      mangled_name_ = "wp_";
      mangled_name_ += name_;
    }

    virtual void Codegen(WasmFile* file) {
      BISON_PRINT("No codegen for this assert: %s\n", name_.c_str());
    }

    virtual void Dump(int tabs = 0) const {
      BISON_TABBED_PRINT(tabs, "(Assert %s\n", name_.c_str());

      expr_->Dump(tabs + 1);

      BISON_TABBED_PRINT(tabs, ")\n");
    }

    const std::string& GetName() const {
      return name_;
    }

    const std::string& GetMangledName() const {
      return mangled_name_;
    }
};

class WasmAssertReturn : public WasmAssert {
  public:
    WasmAssertReturn(Expression* expr) : WasmAssert(expr) {
    }

    virtual void Codegen(WasmFile* file);
};

class WasmAssertReturnNan : public WasmAssertReturn {
  public:
    WasmAssertReturnNan(Expression* expr) : WasmAssertReturn(expr) {
    }

    virtual void Codegen(WasmFile* file);
};

class WasmAssertTrap : public WasmAssert {
  protected:
    std::string error_msg_;

  public:
    WasmAssertTrap(Expression* expr) :
      WasmAssert(expr) {
    }

    virtual void Codegen(WasmFile* file);
};

#endif
