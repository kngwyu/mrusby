# mruby API functions

## in `value.h`

### `mrb_float_read`

```c
MRB_API double mrb_float_read(const char*, char**);
```

```rust
extern "C" {
    pub fn mrb_float_read(
        arg1: *const ::std::os::raw::c_char,
        arg2: *mut *mut ::std::os::raw::c_char,
    ) -> f64;
}
```

### `mrb_regexp_p`
```c
MRB_API mrb_bool mrb_regexp_p(struct mrb_state*, mrb_value);
```

```rust
extern "C" {
    pub fn mrb_regexp_p(arg1: *mut mrb_state, arg2: mrb_value) -> mrb_bool;
}
```

## in 'gc.h'

###  `mrb_free_context`

```c
MRB_API void mrb_free_context(struct mrb_state *mrb, struct mrb_context *c);
```

```rust
extern "C" {
    pub fn mrb_free_context(mrb: *mut mrb_state, c: *mut mrb_context);
}
```

###  `mrb_object_dead_p`

```c
MRB_API mrb_bool
mrb_object_dead_p(struct mrb_state *mrb, struct RBasic *object);
```

```rust
extern "C" {
    pub fn mrb_object_dead_p(mrb: *mut mrb_state, object: *mut RBasic) -> mrb_bool;
}
```

## in 'mruby.h'

###  `mrb_define_class`

```c
/**
 * Defines a new class.
 *
 * If you're creating a gem it may look something like this:
 *
 *      !!!c
 *      void mrb_example_gem_init(mrb_state* mrb) {
 *          struct RClass *example_class;
 *          example_class = mrb_define_class(mrb, "Example_Class", mrb->object_class);
 *      }
 *
 *      void mrb_example_gem_final(mrb_state* mrb) {
 *          //free(TheAnimals);
 *      }
 *
 * @param [mrb_state *] mrb The current mruby state.
 * @param [const char *] name The name of the defined class.
 * @param [struct RClass *] super The new class parent.
 * @return [struct RClass *] Reference to the newly defined class.
 * @see mrb_define_class_under
 */
MRB_API struct RClass *mrb_define_class(mrb_state *mrb, const char *name, struct RClass *super);
```

```rust
extern "C" {
    pub fn mrb_define_class(
        mrb: *mut mrb_state,
        name: *const ::std::os::raw::c_char,
        super_: *mut RClass,
    ) -> *mut RClass;
}
```

###  `mrb_define_module`

```c
/**
 * Defines a new module.
 *
 * @param [mrb_state *] mrb_state* The current mruby state.
 * @param [const char *] char* The name of the module.
 * @return [struct RClass *] Reference to the newly defined module.
 */
MRB_API struct RClass *mrb_define_module(mrb_state *, const char*);
```

```rust
extern "C" {
    pub fn mrb_define_module(
        arg1: *mut mrb_state,
        arg2: *const ::std::os::raw::c_char,
    ) -> *mut RClass;
}
```

###  `mrb_singleton_class`
```c
MRB_API mrb_value mrb_singleton_class(mrb_state*, mrb_value);
```
```rust
extern "C" {
    pub fn mrb_singleton_class(arg1: *mut mrb_state, arg2: mrb_value) -> mrb_value;
}
```

###  `mrb_include_module`

```c
/**
 * Include a module in another class or module.
 * Equivalent to:
 *
 *   module B
 *     include A
 *   end
 * @param [mrb_state *] mrb_state* The current mruby state.
 * @param [struct RClass *] RClass* A reference to module or a class.
 * @param [struct RClass *] RClass* A reference to the module to be included.
 */
MRB_API void mrb_include_module(mrb_state*, struct RClass*, struct RClass*);
```

```rust
extern "C" {
    pub fn mrb_include_module(arg1: *mut mrb_state, arg2: *mut RClass, arg3: *mut RClass);
}
```

###  `mrb_prepend_module`

```c
/**
 * Prepends a module in another class or module.
 *
 * Equivalent to:
 *  module B
 *    prepend A
 *  end
 * @param [mrb_state *] mrb_state* The current mruby state.
 * @param [struct RClass *] RClass* A reference to module or a class.
 * @param [struct RClass *] RClass* A reference to the module to be prepended.
 */
MRB_API void mrb_prepend_module(mrb_state*, struct RClass*, struct RClass*);
```

```rust
extern "C" {
    pub fn mrb_prepend_module(arg1: *mut mrb_state, arg2: *mut RClass, arg3: *mut RClass);
}
```

###  `mrb_define_method`

```c
/**
 * Defines a global function in ruby.
 *
 * If you're creating a gem it may look something like this
 *
 * Example:
 *
 *     !!!c
 *     mrb_value example_method(mrb_state* mrb, mrb_value self)
 *     {
 *          puts("Executing example command!");
 *          return self;
 *     }
 *
 *     void mrb_example_gem_init(mrb_state* mrb)
 *     {
 *           mrb_define_method(mrb, mrb->kernel_module, "example_method", example_method, MRB_ARGS_NONE());
 *     }
 *
 * @param [mrb_state *] mrb The MRuby state reference.
 * @param [struct RClass *] cla The class pointer where the method will be defined.
 * @param [const char *] name The name of the method being defined.
 * @param [mrb_func_t] func The function pointer to the method definition.
 * @param [mrb_aspec] aspec The method parameters declaration.
 */
MRB_API void mrb_define_method(mrb_state *mrb, struct RClass *cla, const char *name, mrb_func_t func, mrb_aspec aspec);
```

```rust
extern "C" {
    pub fn mrb_define_method(
        mrb: *mut mrb_state,
        cla: *mut RClass,
        name: *const ::std::os::raw::c_char,
        func: mrb_func_t,
        aspec: mrb_aspec,
    );
}
```

###  `mrb_define_class_method`

```c
/**
 * Defines a class method.
 *
 * Example:
 *
 *     # Ruby style
 *     class Foo
 *       def Foo.bar
 *       end
 *     end
 *     // C style
 *     mrb_value bar_method(mrb_state* mrb, mrb_value self){
 *       return mrb_nil_value();
 *     }
 *     void mrb_example_gem_init(mrb_state* mrb){
 *       struct RClass *foo;
 *       foo = mrb_define_class(mrb, "Foo", mrb->object_class);
 *       mrb_define_class_method(mrb, foo, "bar", bar_method, MRB_ARGS_NONE());
 *     }
 * @param [mrb_state *] mrb_state* The MRuby state reference.
 * @param [struct RClass *] RClass* The class where the class method will be defined.
 * @param [const char *] char* The name of the class method being defined.
 * @param [mrb_func_t] mrb_func_t The function pointer to the class method definition.
 * @param [mrb_aspec] mrb_aspec The method parameters declaration.
 */
MRB_API void mrb_define_class_method(mrb_state *, struct RClass *, const char *, mrb_func_t, mrb_aspec);
```

```rust
extern "C" {
    pub fn mrb_define_class_method(
        arg1: *mut mrb_state,
        arg2: *mut RClass,
        arg3: *const ::std::os::raw::c_char,
        arg4: mrb_func_t,
        arg5: mrb_aspec,
    );
}
```

###  `mrb_define_singleton_method`

```c
MRB_API void mrb_define_singleton_method(mrb_state*, struct RObject*, const char*, mrb_func_t, mrb_aspec);
```

```rust
extern "C" {
    pub fn mrb_define_singleton_method(
        arg1: *mut mrb_state,
        arg2: *mut RObject,
        arg3: *const ::std::os::raw::c_char,
        arg4: mrb_func_t,
        arg5: mrb_aspec,
    );
}
```

###  `mrb_define_module_function`
```c
/**
 *  Defines a module fuction.
 *
 * Example:
 *
 *        # Ruby style
 *        module Foo
 *          def Foo.bar
 *          end
 *        end
 *        // C style
 *        mrb_value bar_method(mrb_state* mrb, mrb_value self){
 *          return mrb_nil_value();
 *        }
 *        void mrb_example_gem_init(mrb_state* mrb){
 *          struct RClass *foo;
 *          foo = mrb_define_module(mrb, "Foo");
 *          mrb_define_module_function(mrb, foo, "bar", bar_method, MRB_ARGS_NONE());
 *        }
 *  @param [mrb_state *] mrb_state* The MRuby state reference.
 *  @param [struct RClass *] RClass* The module where the module function will be defined.
 *  @param [const char *] char* The name of the module function being defined.
 *  @param [mrb_func_t] mrb_func_t The function pointer to the module function definition.
 *  @param [mrb_aspec] mrb_aspec The method parameters declaration.
 */
MRB_API void mrb_define_module_function(mrb_state*, struct RClass*, const char*, mrb_func_t, mrb_aspec);
```

```rust
extern "C" {
    pub fn mrb_define_module_function(
        arg1: *mut mrb_state,
        arg2: *mut RClass,
        arg3: *const ::std::os::raw::c_char,
        arg4: mrb_func_t,
        arg5: mrb_aspec,
    );
}
```

###  `mrb_define_module_function`
```c
/**
 *  Defines a constant.
 *
 * Example:
 *
 *          # Ruby style
 *          class ExampleClass
 *            AGE = 22
 *          end
 *          // C style
 *          #include <stdio.h>
 *          #include <mruby.h>
 *
 *          void
 *          mrb_example_gem_init(mrb_state* mrb){
 *            mrb_define_const(mrb, mrb->kernel_module, "AGE", mrb_fixnum_value(22));
 *          }
 *
 *          mrb_value
 *          mrb_example_gem_final(mrb_state* mrb){
 *          }
 *  @param [mrb_state *] mrb_state* The MRuby state reference.
 *  @param [struct RClass *] RClass* A class or module the constant is defined in.
 *  @param [const char *] name The name of the constant being defined.
 *  @param [mrb_value] mrb_value The value for the constant.
 */
MRB_API void mrb_define_const(mrb_state*, struct RClass*, const char *name, mrb_value);
```

```rust
extern "C" {
    pub fn mrb_define_const(
        arg1: *mut mrb_state,
        arg2: *mut RClass,
        name: *const ::std::os::raw::c_char,
        arg3: mrb_value,
    );
}
```

###  `mrb_undef_method`

```c
/**
 * Undefines a method.
 *
 * Example:
 *
 *     # Ruby style
 *
 *     class ExampleClassA
 *       def example_method
 *         "example"
 *       end
 *     end
 *     ExampleClassA.new.example_method # => example
 *
 *     class ExampleClassB < ExampleClassA
 *       undef_method :example_method
 *     end
 *
 *     ExampleClassB.new.example_method # => undefined method 'example_method' for ExampleClassB (NoMethodError)
 *
 *     // C style
 *     #include <stdio.h>
 *     #include <mruby.h>
 *
 *     mrb_value
 *     mrb_example_method(mrb_state *mrb){
 *       return mrb_str_new_lit(mrb, "example");
 *     }
 *
 *     void
 *     mrb_example_gem_init(mrb_state* mrb){
 *       struct RClass *example_class_a;
 *       struct RClass *example_class_b;
 *       struct RClass *example_class_c;
 *
 *       example_class_a = mrb_define_class(mrb, "ExampleClassA", mrb->object_class);
 *       mrb_define_method(mrb, example_class_a, "example_method", mrb_example_method, MRB_ARGS_NONE());
 *       example_class_b = mrb_define_class(mrb, "ExampleClassB", example_class_a);
 *       example_class_c = mrb_define_class(mrb, "ExampleClassC", example_class_b);
 *       mrb_undef_method(mrb, example_class_c, "example_method");
 *     }
 *
 *     mrb_example_gem_final(mrb_state* mrb){
 *     }
 * @param [mrb_state*] mrb_state* The mruby state reference.
 * @param [struct RClass*] RClass* A class the method will be undefined from.
 * @param [const char*] constchar* The name of the method to be undefined.
 */
MRB_API void mrb_undef_method(mrb_state*, struct RClass*, const char*);
```

```rust
extern "C" {
    pub fn mrb_undef_method(
        arg1: *mut mrb_state,
        arg2: *mut RClass,
        arg3: *const ::std::os::raw::c_char,
    );
}
```

###  `mrb_undef_class_method`

```c
/**
 * Undefine a class method.
 * Example:
 *
 *      # Ruby style
 *      class ExampleClass
 *        def self.example_method
 *          "example"
 *        end
 *      end
 *
 *     ExampleClass.example_method
 *
 *     // C style
 *     #include <stdio.h>
 *     #include <mruby.h>
 *
 *     mrb_value
 *     mrb_example_method(mrb_state *mrb){
 *       return mrb_str_new_lit(mrb, "example");
 *     }
 *
 *     void
 *     mrb_example_gem_init(mrb_state* mrb){
 *       struct RClass *example_class;
 *       example_class = mrb_define_class(mrb, "ExampleClass", mrb->object_class);
 *       mrb_define_class_method(mrb, example_class, "example_method", mrb_example_method, MRB_ARGS_NONE());
 *       mrb_undef_class_method(mrb, example_class, "example_method");
 *      }
 *
 *      void
 *      mrb_example_gem_final(mrb_state* mrb){
 *      }
 * @param [mrb_state*] mrb_state* The mruby state reference.
 * @param [RClass*] RClass* A class the class method will be undefined from.
 * @param [constchar*] constchar* The name of the class method to be undefined.
 */
MRB_API void mrb_undef_class_method(mrb_state*, struct RClass*, const char*);
```

```rust
extern "C" {
    pub fn mrb_undef_class_method(
        arg1: *mut mrb_state,
        arg2: *mut RClass,
        arg3: *const ::std::os::raw::c_char,
    );
}
```

###  `mrb_obj_new`

```c
/**
 * Initialize a new object instace of c class.
 *
 * Example:
 *
 *     # Ruby style
 *     class ExampleClass
 *     end
 *
 *     p ExampleClass # => #<ExampleClass:0x9958588>
 *     // C style
 *     #include <stdio.h>
 *     #include <mruby.h>
 *
 *     void
 *     mrb_example_gem_init(mrb_state* mrb) {
 *       struct RClass *example_class;
 *       mrb_value obj;
 *       example_class = mrb_define_class(mrb, "ExampleClass", mrb->object_class); # => class ExampleClass; end
 *       obj = mrb_obj_new(mrb, example_class, 0, NULL); # => ExampleClass.new
 *       mrb_p(mrb, obj); // => Kernel#p
 *      }
 * @param [mrb_state*] mrb The current mruby state.
 * @param [RClass*] c Reference to the class of the new object.
 * @param [mrb_int] argc Number of arguments in argv
 * @param [const mrb_value *] argv Array of mrb_value to initialize the object
 * @return [mrb_value] The newly initialized object
 */
MRB_API mrb_value mrb_obj_new(mrb_state *mrb, struct RClass *c, mrb_int argc, const mrb_value *argv);
```

```rust
extern "C" {
    pub fn mrb_obj_new(
        mrb: *mut mrb_state,
        c: *mut RClass,
        argc: mrb_int,
        argv: *const mrb_value,
    ) -> mrb_value;
}
```

###  `mrb_instance_new`
```c
MRB_API mrb_value mrb_instance_new(mrb_state *mrb, mrb_value cv);
```

```rust
extern "C" {
    pub fn mrb_instance_new(mrb: *mut mrb_state, cv: mrb_value) -> mrb_value;
}
```

###  `mrb_class_new`
```c
/**
 * Creates a new instance of Class, Class.
 *
 * Example:
 *
 *      void
 *      mrb_example_gem_init(mrb_state* mrb) {
 *        struct RClass *example_class;
 *
 *        mrb_value obj;
 *        example_class = mrb_class_new(mrb, mrb->object_class);
 *        obj = mrb_obj_new(mrb, example_class, 0, NULL); // => #<#<Class:0x9a945b8>:0x9a94588>
 *        mrb_p(mrb, obj); // => Kernel#p
 *       }
 *
 * @param [mrb_state*] mrb The current mruby state.
 * @param [struct RClass *] super The super class or parent.
 * @return [struct RClass *] Reference to the new class.
 */
MRB_API struct RClass * mrb_class_new(mrb_state *mrb, struct RClass *super);
```

```rust
extern "C" {
    pub fn mrb_class_new(mrb: *mut mrb_state, super_: *mut RClass) -> *mut RClass;
}
```

###  `mrb_module_new`
```c
/**
 * Creates a new module, Module.
 *
 * Example:
 *      void
 *      mrb_example_gem_init(mrb_state* mrb) {
 *        struct RClass *example_module;
 *
 *        example_module = mrb_module_new(mrb);
 *      }
 *
 * @param [mrb_state*] mrb The current mruby state.
 * @return [struct RClass *] Reference to the new module.
 */
MRB_API struct RClass * mrb_module_new(mrb_state *mrb);
```

```rust
extern "C" {
    pub fn mrb_module_new(mrb: *mut mrb_state) -> *mut RClass;
}
```

###  `mrb_class_defined`

```c
/**
 * Returns an mrb_bool. True if class was defined, and false if the class was not defined.
 *
 * Example:
 *     void
 *     mrb_example_gem_init(mrb_state* mrb) {
 *       struct RClass *example_class;
 *       mrb_bool cd;
 *
 *       example_class = mrb_define_class(mrb, "ExampleClass", mrb->object_class);
 *       cd = mrb_class_defined(mrb, "ExampleClass");
 *
 *       // If mrb_class_defined returns 1 then puts "True"
 *       // If mrb_class_defined returns 0 then puts "False"
 *       if (cd == 1){
 *         puts("True");
 *       }
 *       else {
 *         puts("False");
 *       }
 *      }
 *
 * @param [mrb_state*] mrb The current mruby state.
 * @param [const char *] name A string representing the name of the class.
 * @return [mrb_bool] A boolean value.
 */
MRB_API mrb_bool mrb_class_defined(mrb_state *mrb, const char *name);
```

```rust
extern "C" {
    pub fn mrb_class_defined(mrb: *mut mrb_state, name: *const ::std::os::raw::c_char) -> mrb_bool;
}
```

###  `mrb_class_get`
```c
/**
 * Gets a class.
 * @param [mrb_state*] mrb The current mruby state.
 * @param [const char *] name The name of the class.
 * @return [struct RClass *] A reference to the class.
*/
MRB_API struct RClass * mrb_class_get(mrb_state *mrb, const char *name);
```

```rust
extern "C" {
    pub fn mrb_class_get(mrb: *mut mrb_state, name: *const ::std::os::raw::c_char) -> *mut RClass;
}
```

###  `mrb_exc_get`
```c
/**
 * Gets a exception class.
 * @param [mrb_state*] mrb The current mruby state.
 * @param [const char *] name The name of the class.
 * @return [struct RClass *] A reference to the class.
*/
MRB_API struct RClass * mrb_exc_get(mrb_state *mrb, const char *name);
```

```rust
extern "C" {
    pub fn mrb_exc_get(mrb: *mut mrb_state, name: *const ::std::os::raw::c_char) -> *mut RClass;
}

```

###  `mrb_bool mrb_class_defined_under`

```c
/**
 * Returns an mrb_bool. True if inner class was defined, and false if the inner class was not defined.
 *
 * Example:
 *     void
 *     mrb_example_gem_init(mrb_state* mrb) {
 *       struct RClass *example_outer, *example_inner;
 *       mrb_bool cd;
 *
 *       example_outer = mrb_define_module(mrb, "ExampleOuter");
 *
 *       example_inner = mrb_define_class_under(mrb, example_outer, "ExampleInner", mrb->object_class);
 *       cd = mrb_class_defined_under(mrb, example_outer, "ExampleInner");
 *
 *       // If mrb_class_defined_under returns 1 then puts "True"
 *       // If mrb_class_defined_under returns 0 then puts "False"
 *       if (cd == 1){
 *         puts("True");
 *       }
 *       else {
 *         puts("False");
 *       }
 *      }
 *
 * @param [mrb_state*] mrb The current mruby state.
 * @param [struct RClass *] outer The name of the outer class.
 * @param [const char *] name A string representing the name of the inner class.
 * @return [mrb_bool] A boolean value.
 */
MRB_API mrb_bool mrb_class_defined_under(mrb_state *mrb, struct RClass *outer, const char *name);
```

```rust
extern "C" {
    pub fn mrb_class_defined_under(
        mrb: *mut mrb_state,
        outer: *mut RClass,
        name: *const ::std::os::raw::c_char,
    ) -> mrb_bool;
}
```

###  `mrb_class_get_under`

```c
/**
 * Gets a child class.
 * @param [mrb_state*] mrb The current mruby state.
 * @param [struct RClass *] outer The name of the parent class.
 * @param [const char *] name The name of the class.
 * @return [struct RClass *] A reference to the class.
*/
MRB_API struct RClass * mrb_class_get_under(mrb_state *mrb, struct RClass *outer, const char *name);
```

```rust
extern "C" {
    pub fn mrb_class_get_under(
        mrb: *mut mrb_state,
        outer: *mut RClass,
        name: *const ::std::os::raw::c_char,
    ) -> *mut RClass;
}
```

###  `mrb_module_get`

```c
/**
 * Gets a module.
 * @param [mrb_state*] mrb The current mruby state.
 * @param [const char *] name The name of the module.
 * @return [struct RClass *] A reference to the module.
*/
MRB_API struct RClass * mrb_module_get(mrb_state *mrb, const char *name);
```

```rust
extern "C" {
    pub fn mrb_module_get(mrb: *mut mrb_state, name: *const ::std::os::raw::c_char) -> *mut RClass;
}
```

###  `mrb_module_get_under`
```c
/**
 * Gets a module defined under another module.
 * @param [mrb_state*] mrb The current mruby state.
 * @param [struct RClass *] outer The name of the outer module.
 * @param [const char *] name The name of the module.
 * @return [struct RClass *] A reference to the module.
*/
MRB_API struct RClass * mrb_module_get_under(mrb_state *mrb, struct RClass *outer, const char *name);
```

```rust
extern "C" {
    pub fn mrb_module_get_under(
        mrb: *mut mrb_state,
        outer: *mut RClass,
        name: *const ::std::os::raw::c_char,
    ) -> *mut RClass;
}
```

###  `mrb_notimplement_m`
(TODO) what's this

```c
MRB_API mrb_value mrb_notimplement_m(mrb_state*, mrb_value);
```

```rust
extern "C" {
    pub fn mrb_notimplement_m(arg1: *mut mrb_state, arg2: mrb_value) -> mrb_value;
}
```

###  `mrb_obj_dup`

```c
/**
 * Duplicate an object.
 *
 * Equivalent to:
 *   Object#dup
 * @param [mrb_state*] mrb The current mruby state.
 * @param [mrb_value] obj Object to be duplicate.
 * @return [mrb_value] The newly duplicated object.
 */
MRB_API mrb_value mrb_obj_dup(mrb_state *mrb, mrb_value obj);
```

```rust
extern "C" {
    pub fn mrb_obj_dup(mrb: *mut mrb_state, obj: mrb_value) -> mrb_value;
}
```

###  `mrb_check_to_integer`

```c
MRB_API mrb_value mrb_check_to_integer(mrb_state *mrb, mrb_value val, const char *method);
```

```rust
extern "C" {
    pub fn mrb_check_to_integer(
        mrb: *mut mrb_state,
        val: mrb_value,
        method: *const ::std::os::raw::c_char,
    ) -> mrb_value;
}
```

###  `mrb_obj_respond_to`

```c
/**
 * Returns true if obj responds to the given method. If the method was defined for that
 * class it returns true, it returns false otherwise.
 *
 *      Example:
 *      # Ruby style
 *      class ExampleClass
 *        def example_method
 *        end
 *      end
 *
 *      ExampleClass.new.respond_to?(:example_method) # => true
 *
 *      // C style
 *      void
 *      mrb_example_gem_init(mrb_state* mrb) {
 *        struct RClass *example_class;
 *        mrb_sym mid;
 *        mrb_bool obj_resp;
 *
 *        example_class = mrb_define_class(mrb, "ExampleClass", mrb->object_class);
 *        mrb_define_method(mrb, example_class, "example_method", exampleMethod, MRB_ARGS_NONE());
 *        mid = mrb_intern_str(mrb, mrb_str_new_lit(mrb, "example_method" ));
 *        obj_resp = mrb_obj_respond_to(mrb, example_class, mid); // => 1(true in Ruby world)
 *
 *        // If mrb_obj_respond_to returns 1 then puts "True"
 *        // If mrb_obj_respond_to returns 0 then puts "False"
 *        if (obj_resp == 1) {
 *          puts("True");
 *        }
 *        else if (obj_resp == 0) {
 *          puts("False");
 *        }
 *      }
 *
 * @param [mrb_state*] mrb The current mruby state.
 * @param [struct RClass *] c A reference to a class.
 * @param [mrb_sym] mid A symbol referencing a method id.
 * @return [mrb_bool] A boolean value.
 */
MRB_API mrb_bool mrb_obj_respond_to(mrb_state *mrb, struct RClass* c, mrb_sym mid);
```

```rust
extern "C" {
    pub fn mrb_obj_respond_to(mrb: *mut mrb_state, c: *mut RClass, mid: mrb_sym) -> mrb_bool;
}
```

###  `mrb_define_class_under`
```c
/**
 * Defines a new class under a given module
 *
 * @param [mrb_state*] mrb The current mruby state.
 * @param [struct RClass *] outer Reference to the module under which the new class will be defined
 * @param [const char *] name The name of the defined class
 * @param [struct RClass *] super The new class parent
 * @return [struct RClass *] Reference to the newly defined class
 * @see mrb_define_class
 */
MRB_API struct RClass * mrb_define_class_under(mrb_state *mrb, struct RClass *outer, const char *name, struct RClass *super);
```

```rust
extern "C" {
    pub fn mrb_define_class_under(
        mrb: *mut mrb_state,
        outer: *mut RClass,
        name: *const ::std::os::raw::c_char,
        super_: *mut RClass,
    ) -> *mut RClass;
}
```

###  `mrb_define_module_under`

```c
MRB_API struct RClass * mrb_define_module_under(mrb_state *mrb, struct RClass *outer, const char *name);
```

```rust
extern "C" {
    pub fn mrb_define_module_under(
        mrb: *mut mrb_state,
        outer: *mut RClass,
        name: *const ::std::os::raw::c_char,
    ) -> *mut RClass;
}
```

###  `mrb_args_format`

```c
/**
 * Format specifiers for {mrb_get_args} function
 *
 * Must be a C string composed of the following format specifiers:
 *
 * | char | Ruby type      | C types           | Notes                                               |
 * |:----:|----------------|-------------------|----------------------------------------------------|
 * | `o`  | {Object}       | {mrb_value}       | Could be used to retrieve any type of argument     |
 * | `C`  | {Class}/{Module} | {mrb_value}     |                                                    |
 * | `S`  | {String}       | {mrb_value}       | when `!` follows, the value may be `nil`           |
 * | `A`  | {Array}        | {mrb_value}       | when `!` follows, the value may be `nil`           |
 * | `H`  | {Hash}         | {mrb_value}       | when `!` follows, the value may be `nil`           |
 * | `s`  | {String}       | char *, {mrb_int} |  Receive two arguments; `s!` gives (`NULL`,`0`) for `nil`       |
 * | `z`  | {String}       | char *            | `NULL` terminated string; `z!` gives `NULL` for `nil`           |
 * | `a`  | {Array}        | {mrb_value} *, {mrb_int} | Receive two arguments; `a!` gives (`NULL`,`0`) for `nil` |
 * | `f`  | {Float}        | {mrb_float}       |                                                    |
 * | `i`  | {Integer}      | {mrb_int}         |                                                    |
 * | `b`  | boolean        | {mrb_bool}        |                                                    |
 * | `n`  | {Symbol}       | {mrb_sym}         |                                                    |
 * | `&`  | block          | {mrb_value}       |                                                    |
 * | `*`  | rest arguments | {mrb_value} *, {mrb_int} | Receive the rest of arguments as an array.  |
 * | &vert; | optional     |                   | After this spec following specs would be optional. |
 * | `?`  | optional given | {mrb_bool}        | `TRUE` if preceding argument is given. Used to check optional argument is given. |
 *
 * @see mrb_get_args
 */
typedef const char *mrb_args_format;
```

```rust
pub type mrb_args_format = *const ::std::os::raw::c_char;
```

###  `mrb_get_args`

```c
/**
 * Retrieve arguments from mrb_state.
 *
 * When applicable, implicit conversions (such as `to_str`, `to_ary`, `to_hash`) are
 * applied to received arguments.
 * Used inside a function of mrb_func_t type.
 *
 * @param mrb The current MRuby state.
 * @param format [mrb_args_format] is a list of format specifiers
 * @param ... The passing variadic arguments must be a pointer of retrieving type.
 * @return the number of arguments retrieved.
 * @see mrb_args_format
 */
MRB_API mrb_int mrb_get_args(mrb_state *mrb, mrb_args_format format, ...);
```

```rust
extern "C" {
    pub fn mrb_get_args(mrb: *mut mrb_state, format: mrb_args_format, ...) -> mrb_int;
}
```

###  `mrb_get_argc`

```c
/**
 * Retrieve number of arguments from mrb_state.
 *
 * Correctly handles *splat arguments.
 */
MRB_API mrb_int mrb_get_argc(mrb_state *mrb);
```

```rust
extern "C" {
    pub fn mrb_get_argc(mrb: *mut mrb_state) -> mrb_int;
}
```

###  `mrb_get_argv`
```c
MRB_API mrb_value* mrb_get_argv(mrb_state *mrb);
```

```rust
extern "C" {
    pub fn mrb_get_argv(mrb: *mut mrb_state) -> *mut mrb_value;
}
```

###  `mrb_funcall`

```c
/**
 * Call existing ruby functions.
 *
 *      #include <stdio.h>
 *      #include <mruby.h>
 *      #include "mruby/compile.h"
 *
 *      int
 *      main()
 *      {
 *        mrb_int i = 99;
 *        mrb_state *mrb = mrb_open();
 *
 *        if (!mrb) { }
 *        FILE *fp = fopen("test.rb","r");
 *        mrb_value obj = mrb_load_file(mrb,fp);
 *        mrb_funcall(mrb, obj, "method_name", 1, mrb_fixnum_value(i));
 *        fclose(fp);
 *        mrb_close(mrb);
 *       }
 * @param [mrb_state*] mrb_state* The current mruby state.
 * @param [mrb_value] mrb_value A reference to an mruby value.
 * @param [const char*] const char* The name of the method.
 * @param [mrb_int] mrb_int The number of arguments the method has.
 * @param [...] ... Variadic values(not type safe!).
 * @return [mrb_value] mrb_value mruby function value.
 */
MRB_API mrb_value mrb_funcall(mrb_state*, mrb_value, const char*, mrb_int,...);
```

```rust
extern "C" {
    pub fn mrb_funcall(
        arg1: *mut mrb_state,
        arg2: mrb_value,
        arg3: *const ::std::os::raw::c_char,
        arg4: mrb_int,
        ...
    ) -> mrb_value;
}
```

###  `mrb_funcall_argv`

```c
/**
 * Call existing ruby functions. This is basically the type safe version of mrb_funcall.
 *
 *      #include <stdio.h>
 *      #include <mruby.h>
 *      #include "mruby/compile.h"
 *      int
 *      main()
 *      {
 *        mrb_int i = 99;
 *        mrb_state *mrb = mrb_open();
 *
 *        if (!mrb) { }
 *        mrb_sym m_sym = mrb_intern_lit(mrb, "method_name"); // Symbol for method.
 *
 *        FILE *fp = fopen("test.rb","r");
 *        mrb_value obj = mrb_load_file(mrb,fp);
 *        mrb_funcall_argv(mrb, obj, m_sym, 1, &obj); // Calling ruby function from test.rb.
 *        fclose(fp);
 *        mrb_close(mrb);
 *       }
 * @param [mrb_state*] mrb_state* The current mruby state.
 * @param [mrb_value] mrb_value A reference to an mruby value.
 * @param [mrb_sym] mrb_sym The symbol representing the method.
 * @param [mrb_int] mrb_int The number of arguments the method has.
 * @param [const mrb_value*] mrb_value* Pointer to the object.
 * @return [mrb_value] mrb_value mruby function value.
 * @see mrb_funcall
 */
MRB_API mrb_value mrb_funcall_argv(mrb_state*, mrb_value, mrb_sym, mrb_int, const mrb_value*);
```

```rust
extern "C" {
    pub fn mrb_funcall_argv(
        arg1: *mut mrb_state,
        arg2: mrb_value,
        arg3: mrb_sym,
        arg4: mrb_int,
        arg5: *const mrb_value,
    ) -> mrb_value;
}
```

###  `mrb_funcall_with_block`

```c
/**
 * Call existing ruby functions with a block.
 */
MRB_API mrb_value mrb_funcall_with_block(mrb_state*, mrb_value, mrb_sym, mrb_int, const mrb_value*, mrb_value);
```

```rust
extern "C" {
    pub fn mrb_funcall_with_block(
        arg1: *mut mrb_state,
        arg2: mrb_value,
        arg3: mrb_sym,
        arg4: mrb_int,
        arg5: *const mrb_value,
        arg6: mrb_value,
    ) -> mrb_value;
}
```

###  `mrb_intern_cstr`

```c
/**
 * Create a symbol
 *
 *     # Ruby style:
 *     :pizza # => :pizza
 *
 *     // C style:
 *     mrb_sym m_sym = mrb_intern_lit(mrb, "pizza"); //  => :pizza
 * @param [mrb_state*] mrb_state* The current mruby state.
 * @param [const char*] const char* The name of the method.
 * @return [mrb_sym] mrb_sym A symbol.
 */
MRB_API mrb_sym mrb_intern_cstr(mrb_state*,const char*);
```

```rust
extern "C" {
    pub fn mrb_intern_cstr(arg1: *mut mrb_state, arg2: *const ::std::os::raw::c_char) -> mrb_sym;
}
```

###  `mrb_intern` and allocators

(TODO) separate document
```c
MRB_API mrb_sym mrb_intern(mrb_state*,const char*,size_t);
MRB_API mrb_sym mrb_intern_static(mrb_state*,const char*,size_t);
#define mrb_intern_lit(mrb, lit) mrb_intern_static(mrb, lit, mrb_strlen_lit(lit))
MRB_API mrb_sym mrb_intern_str(mrb_state*,mrb_value);
MRB_API mrb_value mrb_check_intern_cstr(mrb_state*,const char*);
MRB_API mrb_value mrb_check_intern(mrb_state*,const char*,size_t);
MRB_API mrb_value mrb_check_intern_str(mrb_state*,mrb_value);
MRB_API const char *mrb_sym2name(mrb_state*,mrb_sym);
MRB_API const char *mrb_sym2name_len(mrb_state*,mrb_sym,mrb_int*);
MRB_API mrb_value mrb_sym2str(mrb_state*,mrb_sym);

MRB_API void *mrb_malloc(mrb_state*, size_t);         /* raise RuntimeError if no mem */
MRB_API void *mrb_calloc(mrb_state*, size_t, size_t); /* ditto */
MRB_API void *mrb_realloc(mrb_state*, void*, size_t); /* ditto */
MRB_API void *mrb_realloc_simple(mrb_state*, void*, size_t); /* return NULL if no memory available */
MRB_API void *mrb_malloc_simple(mrb_state*, size_t);  /* return NULL if no memory available */
MRB_API struct RBasic *mrb_obj_alloc(mrb_state*, enum mrb_vtype, struct RClass*);
MRB_API void mrb_free(mrb_state*, void*);
```

```rust
extern "C" {
    pub fn mrb_intern(
        arg1: *mut mrb_state,
        arg2: *const ::std::os::raw::c_char,
        arg3: usize,
    ) -> mrb_sym;
}
extern "C" {
    pub fn mrb_intern_static(
        arg1: *mut mrb_state,
        arg2: *const ::std::os::raw::c_char,
        arg3: usize,
    ) -> mrb_sym;
}
extern "C" {
    pub fn mrb_intern_str(arg1: *mut mrb_state, arg2: mrb_value) -> mrb_sym;
}
extern "C" {
    pub fn mrb_check_intern_cstr(
        arg1: *mut mrb_state,
        arg2: *const ::std::os::raw::c_char,
    ) -> mrb_value;
}
extern "C" {
    pub fn mrb_check_intern(
        arg1: *mut mrb_state,
        arg2: *const ::std::os::raw::c_char,
        arg3: usize,
    ) -> mrb_value;
}
extern "C" {
    pub fn mrb_check_intern_str(arg1: *mut mrb_state, arg2: mrb_value) -> mrb_value;
}
extern "C" {
    pub fn mrb_sym2name(arg1: *mut mrb_state, arg2: mrb_sym) -> *const ::std::os::raw::c_char;
}
extern "C" {
    pub fn mrb_sym2name_len(
        arg1: *mut mrb_state,
        arg2: mrb_sym,
        arg3: *mut mrb_int,
    ) -> *const ::std::os::raw::c_char;
}
extern "C" {
    pub fn mrb_sym2str(arg1: *mut mrb_state, arg2: mrb_sym) -> mrb_value;
}
extern "C" {
    pub fn mrb_malloc(arg1: *mut mrb_state, arg2: usize) -> *mut ::std::os::raw::c_void;
}
extern "C" {
    pub fn mrb_calloc(
        arg1: *mut mrb_state,
        arg2: usize,
        arg3: usize,
    ) -> *mut ::std::os::raw::c_void;
}
extern "C" {
    pub fn mrb_realloc(
        arg1: *mut mrb_state,
        arg2: *mut ::std::os::raw::c_void,
        arg3: usize,
    ) -> *mut ::std::os::raw::c_void;
}
extern "C" {
    pub fn mrb_realloc_simple(
        arg1: *mut mrb_state,
        arg2: *mut ::std::os::raw::c_void,
        arg3: usize,
    ) -> *mut ::std::os::raw::c_void;
}
extern "C" {
    pub fn mrb_malloc_simple(arg1: *mut mrb_state, arg2: usize) -> *mut ::std::os::raw::c_void;
}
extern "C" {
    pub fn mrb_obj_alloc(arg1: *mut mrb_state, arg2: mrb_vtype, arg3: *mut RClass) -> *mut RBasic;
}
extern "C" {
    pub fn mrb_free(arg1: *mut mrb_state, arg2: *mut ::std::os::raw::c_void);
}
```

###  string apis

```c
MRB_API mrb_value mrb_str_new(mrb_state *mrb, const char *p, size_t len);

/**
 * Turns a C string into a Ruby string value.
 */
MRB_API mrb_value mrb_str_new_cstr(mrb_state*, const char*);
MRB_API mrb_value mrb_str_new_static(mrb_state *mrb, const char *p, size_t len);
```

```c
extern "C" {
    pub fn mrb_str_new(
        mrb: *mut mrb_state,
        p: *const ::std::os::raw::c_char,
        len: usize,
    ) -> mrb_value;
}
extern "C" {
    /// Turns a C string into a Ruby string value.
    pub fn mrb_str_new_cstr(arg1: *mut mrb_state, arg2: *const ::std::os::raw::c_char)
        -> mrb_value;
}
extern "C" {
    pub fn mrb_str_new_static(
        mrb: *mut mrb_state,
        p: *const ::std::os::raw::c_char,
        len: usize,
    ) -> mrb_value;
}
```

###  `mrb_open` and related APIs

```c
/**
 * Creates new mrb_state.
 *
 * @return
 *      Pointer to the newly created mrb_state.
 */
MRB_API mrb_state* mrb_open(void);

/**
 * Create new mrb_state with custom allocators.
 *
 * @param f
 *      Reference to the allocation function.
 * @param ud
 *      User data will be passed to custom allocator f.
 *      If user data isn't required just pass NULL.
 * @return
 *      Pointer to the newly created mrb_state.
 */
MRB_API mrb_state* mrb_open_allocf(mrb_allocf f, void *ud);

/**
 * Create new mrb_state with just the MRuby core
 *
 * @param f
 *      Reference to the allocation function.
 *      Use mrb_default_allocf for the default
 * @param ud
 *      User data will be passed to custom allocator f.
 *      If user data isn't required just pass NULL.
 * @return
 *      Pointer to the newly created mrb_state.
 */
MRB_API mrb_state* mrb_open_core(mrb_allocf f, void *ud);

/**
 * Closes and frees a mrb_state.
 *
 * @param mrb
 *      Pointer to the mrb_state to be closed.
 */
MRB_API void mrb_close(mrb_state *mrb);

/**
 * The default allocation function.
 *
 * @see mrb_allocf
 */
MRB_API void* mrb_default_allocf(mrb_state*, void*, size_t, void*);
```

```rust
extern "C" {
    pub fn mrb_open() -> *mut mrb_state;
}
extern "C" {
    pub fn mrb_open_allocf(f: mrb_allocf, ud: *mut ::std::os::raw::c_void) -> *mut mrb_state;
}
extern "C" {
    pub fn mrb_open_core(f: mrb_allocf, ud: *mut ::std::os::raw::c_void) -> *mut mrb_state;
}
extern "C" {
    pub fn mrb_close(mrb: *mut mrb_state);
}
extern "C" {
    pub fn mrb_default_allocf(
        arg1: *mut mrb_state,
        arg2: *mut ::std::os::raw::c_void,
        arg3: usize,
        arg4: *mut ::std::os::raw::c_void,
    ) -> *mut ::std::os::raw::c_void;
}
```

###  other APIs

```c
MRB_API mrb_value mrb_top_self(mrb_state *);
MRB_API mrb_value mrb_run(mrb_state*, struct RProc*, mrb_value);
MRB_API mrb_value mrb_top_run(mrb_state*, struct RProc*, mrb_value, unsigned int);
MRB_API mrb_value mrb_vm_run(mrb_state*, struct RProc*, mrb_value, unsigned int);
MRB_API mrb_value mrb_vm_exec(mrb_state*, struct RProc*, mrb_code*);
/* compatibility macros */
#define mrb_toplevel_run_keep(m,p,k) mrb_top_run((m),(p),mrb_top_self(m),(k))
#define mrb_toplevel_run(m,p) mrb_toplevel_run_keep((m),(p),0)
#define mrb_context_run(m,p,s,k) mrb_vm_run((m),(p),(s),(k))

MRB_API void mrb_p(mrb_state*, mrb_value);
MRB_API mrb_int mrb_obj_id(mrb_value obj);
MRB_API mrb_sym mrb_obj_to_sym(mrb_state *mrb, mrb_value name);

MRB_API mrb_bool mrb_obj_eq(mrb_state*, mrb_value, mrb_value);
MRB_API mrb_bool mrb_obj_equal(mrb_state*, mrb_value, mrb_value);
MRB_API mrb_bool mrb_equal(mrb_state *mrb, mrb_value obj1, mrb_value obj2);
MRB_API mrb_value mrb_convert_to_integer(mrb_state *mrb, mrb_value val, mrb_int base);
MRB_API mrb_value mrb_Integer(mrb_state *mrb, mrb_value val);
#ifndef MRB_WITHOUT_FLOAT
MRB_API mrb_value mrb_Float(mrb_state *mrb, mrb_value val);
#endif
MRB_API mrb_value mrb_inspect(mrb_state *mrb, mrb_value obj);
MRB_API mrb_bool mrb_eql(mrb_state *mrb, mrb_value obj1, mrb_value obj2);
```

```rust
extern "C" {
    pub fn mrb_top_self(arg1: *mut mrb_state) -> mrb_value;
}
extern "C" {
    pub fn mrb_run(arg1: *mut mrb_state, arg2: *mut RProc, arg3: mrb_value) -> mrb_value;
}
extern "C" {
    pub fn mrb_top_run(
        arg1: *mut mrb_state,
        arg2: *mut RProc,
        arg3: mrb_value,
        arg4: ::std::os::raw::c_uint,
    ) -> mrb_value;
}
extern "C" {
    pub fn mrb_vm_run(
        arg1: *mut mrb_state,
        arg2: *mut RProc,
        arg3: mrb_value,
        arg4: ::std::os::raw::c_uint,
    ) -> mrb_value;
}
extern "C" {
    pub fn mrb_vm_exec(arg1: *mut mrb_state, arg2: *mut RProc, arg3: *mut mrb_code) -> mrb_value;
}
extern "C" {
    pub fn mrb_p(arg1: *mut mrb_state, arg2: mrb_value);
}
extern "C" {
    pub fn mrb_obj_id(obj: mrb_value) -> mrb_int;
}
extern "C" {
    pub fn mrb_obj_to_sym(mrb: *mut mrb_state, name: mrb_value) -> mrb_sym;
}
extern "C" {
    pub fn mrb_obj_eq(arg1: *mut mrb_state, arg2: mrb_value, arg3: mrb_value) -> mrb_bool;
}
extern "C" {
    pub fn mrb_obj_equal(arg1: *mut mrb_state, arg2: mrb_value, arg3: mrb_value) -> mrb_bool;
}
extern "C" {
    pub fn mrb_equal(mrb: *mut mrb_state, obj1: mrb_value, obj2: mrb_value) -> mrb_bool;
}
extern "C" {
    pub fn mrb_convert_to_integer(mrb: *mut mrb_state, val: mrb_value, base: mrb_int) -> mrb_value;
}
extern "C" {
    pub fn mrb_Integer(mrb: *mut mrb_state, val: mrb_value) -> mrb_value;
}
extern "C" {
    pub fn mrb_Float(mrb: *mut mrb_state, val: mrb_value) -> mrb_value;
}
extern "C" {
    pub fn mrb_inspect(mrb: *mut mrb_state, obj: mrb_value) -> mrb_value;
}
extern "C" {
    pub fn mrb_eql(mrb: *mut mrb_state, obj1: mrb_value, obj2: mrb_value) -> mrb_bool;
}
```

###  gc

```c
MRB_API void mrb_garbage_collect(mrb_state*);
MRB_API void mrb_full_gc(mrb_state*);
MRB_API void mrb_incremental_gc(mrb_state *);
MRB_API void mrb_gc_mark(mrb_state*,struct RBasic*);
#define mrb_gc_mark_value(mrb,val) do {\
  if (!mrb_immediate_p(val)) mrb_gc_mark((mrb), mrb_basic_ptr(val)); \
} while (0)
MRB_API void mrb_field_write_barrier(mrb_state *, struct RBasic*, struct RBasic*);
#define mrb_field_write_barrier_value(mrb, obj, val) do{\
  if (!mrb_immediate_p(val)) mrb_field_write_barrier((mrb), (obj), mrb_basic_ptr(val)); \
} while (0)
MRB_API void mrb_write_barrier(mrb_state *, struct RBasic*);
```

```rust
extern "C" {
    pub fn mrb_garbage_collect(arg1: *mut mrb_state);
}
extern "C" {
    pub fn mrb_full_gc(arg1: *mut mrb_state);
}
extern "C" {
    pub fn mrb_incremental_gc(arg1: *mut mrb_state);
}
extern "C" {
    pub fn mrb_gc_mark(arg1: *mut mrb_state, arg2: *mut RBasic);
}
extern "C" {
    pub fn mrb_field_write_barrier(arg1: *mut mrb_state, arg2: *mut RBasic, arg3: *mut RBasic);
}
extern "C" {
    pub fn mrb_write_barrier(arg1: *mut mrb_state, arg2: *mut RBasic);
}
```

###  object

```c
MRB_API mrb_value mrb_check_convert_type(mrb_state *mrb, mrb_value val, enum mrb_vtype type, const char *tname, const char *method);
MRB_API mrb_value mrb_any_to_s(mrb_state *mrb, mrb_value obj);
MRB_API const char * mrb_obj_classname(mrb_state *mrb, mrb_value obj);
MRB_API struct RClass* mrb_obj_class(mrb_state *mrb, mrb_value obj);
MRB_API mrb_value mrb_class_path(mrb_state *mrb, struct RClass *c);
MRB_API mrb_value mrb_convert_type(mrb_state *mrb, mrb_value val, enum mrb_vtype type, const char *tname, const char *method);
MRB_API mrb_bool mrb_obj_is_kind_of(mrb_state *mrb, mrb_value obj, struct RClass *c);
MRB_API mrb_value mrb_obj_inspect(mrb_state *mrb, mrb_value self);
MRB_API mrb_value mrb_obj_clone(mrb_state *mrb, mrb_value self);
```

```rust
extern "C" {
    pub fn mrb_check_convert_type(
        mrb: *mut mrb_state,
        val: mrb_value,
        type_: mrb_vtype,
        tname: *const ::std::os::raw::c_char,
        method: *const ::std::os::raw::c_char,
    ) -> mrb_value;
}
extern "C" {
    pub fn mrb_any_to_s(mrb: *mut mrb_state, obj: mrb_value) -> mrb_value;
}
extern "C" {
    pub fn mrb_obj_classname(mrb: *mut mrb_state, obj: mrb_value) -> *const ::std::os::raw::c_char;
}
extern "C" {
    pub fn mrb_obj_class(mrb: *mut mrb_state, obj: mrb_value) -> *mut RClass;
}
extern "C" {
    pub fn mrb_class_path(mrb: *mut mrb_state, c: *mut RClass) -> mrb_value;
}
extern "C" {
    pub fn mrb_convert_type(
        mrb: *mut mrb_state,
        val: mrb_value,
        type_: mrb_vtype,
        tname: *const ::std::os::raw::c_char,
        method: *const ::std::os::raw::c_char,
    ) -> mrb_value;
}
extern "C" {
    pub fn mrb_obj_is_kind_of(mrb: *mut mrb_state, obj: mrb_value, c: *mut RClass) -> mrb_bool;
}
extern "C" {
    pub fn mrb_obj_inspect(mrb: *mut mrb_state, self_: mrb_value) -> mrb_value;
}
extern "C" {
    pub fn mrb_obj_clone(mrb: *mut mrb_state, self_: mrb_value) -> mrb_value;
}
```

###  raise bug & print

```c
MRB_API mrb_value mrb_exc_new(mrb_state *mrb, struct RClass *c, const char *ptr, size_t len);
MRB_API mrb_noreturn void mrb_exc_raise(mrb_state *mrb, mrb_value exc);

MRB_API mrb_noreturn void mrb_raise(mrb_state *mrb, struct RClass *c, const char *msg);
MRB_API mrb_noreturn void mrb_raisef(mrb_state *mrb, struct RClass *c, const char *fmt, ...);
MRB_API mrb_noreturn void mrb_name_error(mrb_state *mrb, mrb_sym id, const char *fmt, ...);
MRB_API void mrb_warn(mrb_state *mrb, const char *fmt, ...);
MRB_API mrb_noreturn void mrb_bug(mrb_state *mrb, const char *fmt, ...);
MRB_API void mrb_print_backtrace(mrb_state *mrb);
MRB_API void mrb_print_error(mrb_state *mrb);
```

```rust
extern "C" {
    pub fn mrb_exc_new(
        mrb: *mut mrb_state,
        c: *mut RClass,
        ptr: *const ::std::os::raw::c_char,
        len: usize,
    ) -> mrb_value;
}
extern "C" {
    pub fn mrb_exc_raise(mrb: *mut mrb_state, exc: mrb_value);
}
extern "C" {
    pub fn mrb_raise(mrb: *mut mrb_state, c: *mut RClass, msg: *const ::std::os::raw::c_char);
}
extern "C" {
    pub fn mrb_raisef(mrb: *mut mrb_state, c: *mut RClass, fmt: *const ::std::os::raw::c_char, ...);
}
extern "C" {
    pub fn mrb_name_error(
        mrb: *mut mrb_state,
        id: mrb_sym,
        fmt: *const ::std::os::raw::c_char,
        ...
    );
}
extern "C" {
    pub fn mrb_warn(mrb: *mut mrb_state, fmt: *const ::std::os::raw::c_char, ...);
}
extern "C" {
    pub fn mrb_bug(mrb: *mut mrb_state, fmt: *const ::std::os::raw::c_char, ...);
}
extern "C" {
    pub fn mrb_print_backtrace(mrb: *mut mrb_state);
}
extern "C" {
    pub fn mrb_print_error(mrb: *mut mrb_state);
}
```

###  yield & others

```c

MRB_API mrb_value mrb_yield(mrb_state *mrb, mrb_value b, mrb_value arg);
MRB_API mrb_value mrb_yield_argv(mrb_state *mrb, mrb_value b, mrb_int argc, const mrb_value *argv);
MRB_API mrb_value mrb_yield_with_class(mrb_state *mrb, mrb_value b, mrb_int argc, const mrb_value *argv, mrb_value self, struct RClass *c);

/* continue execution to the proc */
/* this function should always be called as the last function of a method */
/* e.g. return mrb_yield_cont(mrb, proc, self, argc, argv); */
mrb_value mrb_yield_cont(mrb_state *mrb, mrb_value b, mrb_value self, mrb_int argc, const mrb_value *argv);

/* mrb_gc_protect() leaves the object in the arena */
MRB_API void mrb_gc_protect(mrb_state *mrb, mrb_value obj);
/* mrb_gc_register() keeps the object from GC. */
MRB_API void mrb_gc_register(mrb_state *mrb, mrb_value obj);
/* mrb_gc_unregister() removes the object from GC root. */
MRB_API void mrb_gc_unregister(mrb_state *mrb, mrb_value obj);

MRB_API mrb_value mrb_to_int(mrb_state *mrb, mrb_value val);
#define mrb_int(mrb, val) mrb_fixnum(mrb_to_int(mrb, val))
MRB_API void mrb_check_type(mrb_state *mrb, mrb_value x, enum mrb_vtype t);
```

```rust
extern "C" {
    pub fn mrb_yield(mrb: *mut mrb_state, b: mrb_value, arg: mrb_value) -> mrb_value;
}
extern "C" {
    pub fn mrb_yield_argv(
        mrb: *mut mrb_state,
        b: mrb_value,
        argc: mrb_int,
        argv: *const mrb_value,
    ) -> mrb_value;
}
extern "C" {
    pub fn mrb_yield_with_class(
        mrb: *mut mrb_state,
        b: mrb_value,
        argc: mrb_int,
        argv: *const mrb_value,
        self_: mrb_value,
        c: *mut RClass,
    ) -> mrb_value;
}
extern "C" {
    pub fn mrb_yield_cont(
        mrb: *mut mrb_state,
        b: mrb_value,
        self_: mrb_value,
        argc: mrb_int,
        argv: *const mrb_value,
    ) -> mrb_value;
}
extern "C" {
    pub fn mrb_gc_protect(mrb: *mut mrb_state, obj: mrb_value);
}
extern "C" {
    pub fn mrb_gc_register(mrb: *mut mrb_state, obj: mrb_value);
}
extern "C" {
    pub fn mrb_gc_unregister(mrb: *mut mrb_state, obj: mrb_value);
}
extern "C" {
    pub fn mrb_to_int(mrb: *mut mrb_state, val: mrb_value) -> mrb_value;
}
extern "C" {
    pub fn mrb_check_type(mrb: *mut mrb_state, x: mrb_value, t: mrb_vtype);
}
```

###  `mrb_define_alias` and others

```c
MRB_API void mrb_define_alias(mrb_state *mrb, struct RClass *klass, const char *name1, const char *name2);
MRB_API const char *mrb_class_name(mrb_state *mrb, struct RClass* klass);
MRB_API void mrb_define_global_const(mrb_state *mrb, const char *name, mrb_value val);

MRB_API mrb_value mrb_attr_get(mrb_state *mrb, mrb_value obj, mrb_sym id);

MRB_API mrb_bool mrb_respond_to(mrb_state *mrb, mrb_value obj, mrb_sym mid);
MRB_API mrb_bool mrb_obj_is_instance_of(mrb_state *mrb, mrb_value obj, struct RClass* c);
MRB_API mrb_bool mrb_func_basic_p(mrb_state *mrb, mrb_value obj, mrb_sym mid, mrb_func_t func);
```

```rust
extern "C" {
    pub fn mrb_define_alias(
        mrb: *mut mrb_state,
        klass: *mut RClass,
        name1: *const ::std::os::raw::c_char,
        name2: *const ::std::os::raw::c_char,
    );
}
extern "C" {
    pub fn mrb_class_name(mrb: *mut mrb_state, klass: *mut RClass)
        -> *const ::std::os::raw::c_char;
}
extern "C" {
    pub fn mrb_define_global_const(
        mrb: *mut mrb_state,
        name: *const ::std::os::raw::c_char,
        val: mrb_value,
    );
}
extern "C" {
    pub fn mrb_attr_get(mrb: *mut mrb_state, obj: mrb_value, id: mrb_sym) -> mrb_value;
}
extern "C" {
    pub fn mrb_respond_to(mrb: *mut mrb_state, obj: mrb_value, mid: mrb_sym) -> mrb_bool;
}
extern "C" {
    pub fn mrb_obj_is_instance_of(mrb: *mut mrb_state, obj: mrb_value, c: *mut RClass) -> mrb_bool;
}
extern "C" {
    pub fn mrb_func_basic_p(
        mrb: *mut mrb_state,
        obj: mrb_value,
        mid: mrb_sym,
        func: mrb_func_t,
    ) -> mrb_bool;
}
```

###  other APIs

```c
/*
 * Resume a Fiber
 *
 * @mrbgem mruby-fiber
 */
MRB_API mrb_value mrb_fiber_resume(mrb_state *mrb, mrb_value fib, mrb_int argc, const mrb_value *argv);

/*
 * Yield a Fiber
 *
 * @mrbgem mruby-fiber
 */
MRB_API mrb_value mrb_fiber_yield(mrb_state *mrb, mrb_int argc, const mrb_value *argv);

/*
 * FiberError reference
 *
 * @mrbgem mruby-fiber
 */
#define E_FIBER_ERROR (mrb_exc_get(mrb, "FiberError"))

/* memory pool implementation */
typedef struct mrb_pool mrb_pool;
MRB_API struct mrb_pool* mrb_pool_open(mrb_state*);
MRB_API void mrb_pool_close(struct mrb_pool*);
MRB_API void* mrb_pool_alloc(struct mrb_pool*, size_t);
MRB_API void* mrb_pool_realloc(struct mrb_pool*, void*, size_t oldlen, size_t newlen);
MRB_API mrb_bool mrb_pool_can_realloc(struct mrb_pool*, void*, size_t);
MRB_API void* mrb_alloca(mrb_state *mrb, size_t);

MRB_API void mrb_state_atexit(mrb_state *mrb, mrb_atexit_func func);

MRB_API void mrb_show_version(mrb_state *mrb);
MRB_API void mrb_show_copyright(mrb_state *mrb);

MRB_API mrb_value mrb_format(mrb_state *mrb, const char *format, ...);
```

```rust
extern "C" {
    pub fn mrb_fiber_resume(
        mrb: *mut mrb_state,
        fib: mrb_value,
        argc: mrb_int,
        argv: *const mrb_value,
    ) -> mrb_value;
}
extern "C" {
    pub fn mrb_fiber_yield(mrb: *mut mrb_state, argc: mrb_int, argv: *const mrb_value)
        -> mrb_value;
}
#[repr(C)]
#[derive(Debug, Copy, Clone)]
pub struct mrb_pool {
    _unused: [u8; 0],
}
extern "C" {
    pub fn mrb_pool_open(arg1: *mut mrb_state) -> *mut mrb_pool;
}
extern "C" {
    pub fn mrb_pool_close(arg1: *mut mrb_pool);
}
extern "C" {
    pub fn mrb_pool_alloc(arg1: *mut mrb_pool, arg2: usize) -> *mut ::std::os::raw::c_void;
}
extern "C" {
    pub fn mrb_pool_realloc(
        arg1: *mut mrb_pool,
        arg2: *mut ::std::os::raw::c_void,
        oldlen: usize,
        newlen: usize,
    ) -> *mut ::std::os::raw::c_void;
}
extern "C" {
    pub fn mrb_pool_can_realloc(
        arg1: *mut mrb_pool,
        arg2: *mut ::std::os::raw::c_void,
        arg3: usize,
    ) -> mrb_bool;
}
extern "C" {
    pub fn mrb_alloca(mrb: *mut mrb_state, arg1: usize) -> *mut ::std::os::raw::c_void;
}
extern "C" {
    pub fn mrb_state_atexit(mrb: *mut mrb_state, func: mrb_atexit_func);
}
extern "C" {
    pub fn mrb_show_version(mrb: *mut mrb_state);
}
extern "C" {
    pub fn mrb_show_copyright(mrb: *mut mrb_state);
}
extern "C" {
    pub fn mrb_format(mrb: *mut mrb_state, format: *const ::std::os::raw::c_char, ...)
        -> mrb_value;
}
```

## in `compile.h`

###   `mrbc_context_new` and etc.

```c
MRB_API mrbc_context* mrbc_context_new(mrb_state *mrb);
MRB_API void mrbc_context_free(mrb_state *mrb, mrbc_context *cxt);
MRB_API const char *mrbc_filename(mrb_state *mrb, mrbc_context *c, const char *s);
MRB_API void mrbc_partial_hook(mrb_state *mrb, mrbc_context *c, int (*partial_hook)(struct mrb_parser_state*), void*data);
```

```rust
extern "C" {
    pub fn mrbc_context_new(mrb: *mut mrb_state) -> *mut mrbc_context;
}
extern "C" {
    pub fn mrbc_context_free(mrb: *mut mrb_state, cxt: *mut mrbc_context);
}
extern "C" {
    pub fn mrbc_filename(
        mrb: *mut mrb_state,
        c: *mut mrbc_context,
        s: *const ::std::os::raw::c_char,
    ) -> *const ::std::os::raw::c_char;
}
extern "C" {
    pub fn mrbc_partial_hook(
        mrb: *mut mrb_state,
        c: *mut mrbc_context,
        partial_hook: ::std::option::Option<
            unsafe extern "C" fn(arg1: *mut mrb_parser_state) -> ::std::os::raw::c_int,
        >,
        data: *mut ::std::os::raw::c_void,
    );
}
```

### parse API

```c
MRB_API struct mrb_parser_state* mrb_parser_new(mrb_state*);
MRB_API void mrb_parser_free(struct mrb_parser_state*);
MRB_API void mrb_parser_parse(struct mrb_parser_state*,mrbc_context*);

MRB_API void mrb_parser_set_filename(struct mrb_parser_state*, char const*);
MRB_API char const* mrb_parser_get_filename(struct mrb_parser_state*, uint16_t idx);

/* utility functions */
#ifndef MRB_DISABLE_STDIO
MRB_API struct mrb_parser_state* mrb_parse_file(mrb_state*,FILE*,mrbc_context*);
#endif
MRB_API struct mrb_parser_state* mrb_parse_string(mrb_state*,const char*,mrbc_context*);
MRB_API struct mrb_parser_state* mrb_parse_nstring(mrb_state*,const char*,size_t,mrbc_context*);
MRB_API struct RProc* mrb_generate_code(mrb_state*, struct mrb_parser_state*);
MRB_API mrb_value mrb_load_exec(mrb_state *mrb, struct mrb_parser_state *p, mrbc_context *c);

/* program load functions */
#ifndef MRB_DISABLE_STDIO
MRB_API mrb_value mrb_load_file(mrb_state*,FILE*);
MRB_API mrb_value mrb_load_file_cxt(mrb_state*,FILE*, mrbc_context *cxt);
#endif
MRB_API mrb_value mrb_load_string(mrb_state *mrb, const char *s);
MRB_API mrb_value mrb_load_nstring(mrb_state *mrb, const char *s, size_t len);
MRB_API mrb_value mrb_load_string_cxt(mrb_state *mrb, const char *s, mrbc_context *cxt);
MRB_API mrb_value mrb_load_nstring_cxt(mrb_state *mrb, const char *s, size_t len, mrbc_context *cxt);

```


```rust
extern "C" {
    pub fn mrb_parser_new(arg1: *mut mrb_state) -> *mut mrb_parser_state;
}
extern "C" {
    pub fn mrb_parser_free(arg1: *mut mrb_parser_state);
}
extern "C" {
    pub fn mrb_parser_parse(arg1: *mut mrb_parser_state, arg2: *mut mrbc_context);
}
extern "C" {
    pub fn mrb_parser_set_filename(
        arg1: *mut mrb_parser_state,
        arg2: *const ::std::os::raw::c_char,
    );
}
extern "C" {
    pub fn mrb_parser_get_filename(
        arg1: *mut mrb_parser_state,
        idx: u16,
    ) -> *const ::std::os::raw::c_char;
}
extern "C" {
    pub fn mrb_parse_file(
        arg1: *mut mrb_state,
        arg2: *mut FILE,
        arg3: *mut mrbc_context,
    ) -> *mut mrb_parser_state;
}
extern "C" {
    pub fn mrb_parse_string(
        arg1: *mut mrb_state,
        arg2: *const ::std::os::raw::c_char,
        arg3: *mut mrbc_context,
    ) -> *mut mrb_parser_state;
}
extern "C" {
    pub fn mrb_parse_nstring(
        arg1: *mut mrb_state,
        arg2: *const ::std::os::raw::c_char,
        arg3: usize,
        arg4: *mut mrbc_context,
    ) -> *mut mrb_parser_state;
}
extern "C" {
    pub fn mrb_generate_code(arg1: *mut mrb_state, arg2: *mut mrb_parser_state) -> *mut RProc;
}
extern "C" {
    pub fn mrb_load_exec(
        mrb: *mut mrb_state,
        p: *mut mrb_parser_state,
        c: *mut mrbc_context,
    ) -> mrb_value;
}
extern "C" {
    pub fn mrb_load_file(arg1: *mut mrb_state, arg2: *mut FILE) -> mrb_value;
}
extern "C" {
    pub fn mrb_load_file_cxt(
        arg1: *mut mrb_state,
        arg2: *mut FILE,
        cxt: *mut mrbc_context,
    ) -> mrb_value;
}
extern "C" {
    pub fn mrb_load_string(mrb: *mut mrb_state, s: *const ::std::os::raw::c_char) -> mrb_value;
}
extern "C" {
    pub fn mrb_load_nstring(
        mrb: *mut mrb_state,
        s: *const ::std::os::raw::c_char,
        len: usize,
    ) -> mrb_value;
}
extern "C" {
    pub fn mrb_load_string_cxt(
        mrb: *mut mrb_state,
        s: *const ::std::os::raw::c_char,
        cxt: *mut mrbc_context,
    ) -> mrb_value;
}
extern "C" {
    pub fn mrb_load_nstring_cxt(
        mrb: *mut mrb_state,
        s: *const ::std::os::raw::c_char,
        len: usize,
        cxt: *mut mrbc_context,
    ) -> mrb_value;
}
```
