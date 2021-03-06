;; Copyright (c) 2015 Intel Corporation
;;
;; Licensed under the Apache License, Version 2.0 (the "License");
;; you may not use this file except in compliance with the License.
;; You may obtain a copy of the License at
;;
;;      http://www.apache.org/licenses/LICENSE-2.0
;;
;; Unless required by applicable law or agreed to in writing, software
;; distributed under the License is distributed on an "AS IS" BASIS,
;; WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
;; See the License for the specific language governing permissions and
;; limitations under the License.

(module
  (memory 40000000 4)

  (func $getter (param i32) (result i32)
    (i32.load (get_local 0))
  )

  (func $setter (param i32) (result i32)
   (local i32 i32)

   (block
    ;; Get two pointers for each vector.
    (set_local 1 (i32.const 0))
    (set_local 2 (get_local 0))

    (label
     (loop
      (if_else
       (i32.eq (get_local 0) (i32.const 0))
       (br 1)
       (block
        (i32.store (i32.mul (get_local 1) (i32.const 4)) (get_local 1))
        (i32.store (i32.mul (get_local 2) (i32.const 4)) (get_local 1))
        (set_local 1 (i32.add (get_local 1) (i32.const 1)))
        (set_local 2 (i32.add (get_local 2) (i32.const 1)))
        (set_local 0 (i32.sub (get_local 0) (i32.const 1)))
       )
      )

      (br 0)
     )
    )
   )
   (return (get_local 0))
  )

  (func $daxpy (param i32) (param i32) (result i32)
    (local i32 i32)
    (block
      ;; Local 0 is the constant value we will use.
      ;; Local 1 is the number of elements.
      ;; Local 2 is the pointer to the first vector.
      ;; Local 3 is the pointer to the second vector.

      ;; Get it * 4 to make the IV go 4 by 4 and not have the multiply in the address calculation.
      (set_local 1 (i32.mul (get_local 1) (i32.const 4)))

      ;; LLVM is really not nice with its generation if I do a count up.
      ;; Let's do a count down for the iterator in location 2.
      (set_local 2 (i32.sub (get_local 1) (i32.const 4)))
      (set_local 3 (i32.add (get_local 2) (get_local 1)))

      (label
        (loop
          (if_else
            (i32.eq (get_local 2) (i32.const 0))
            (br 1)
            (block
              (i32.store (get_local 2)
                         (i32.add (i32.load (get_local 3))
                                  (i32.mul (get_local 0) (i32.load (get_local 2)))))
              (set_local 2 (i32.sub (get_local 2) (i32.const 4)))
              (set_local 3 (i32.sub (get_local 3) (i32.const 4)))
            )
          )

          (br 0)
        )
      )
    )

    (return (i32.const 0))
  )

  (export "daxpy" $daxpy)
  (export "setter" $setter)
  (export "getter" $getter)
)

(assert_return (invoke "setter" (i32.const 1000)) (i32.const 0))
(assert_return (invoke "daxpy" (i32.const 2) (i32.const 1000)) (i32.const 0))
(assert_return (invoke "getter" (i32.const 4)) (i32.const 3))
