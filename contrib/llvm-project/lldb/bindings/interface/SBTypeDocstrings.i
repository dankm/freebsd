%feature("docstring",
"Represents a member of a type."
) lldb::SBTypeMember;

%feature("docstring",
"Represents a member function of a type."
) lldb::SBTypeMemberFunction;

%feature("docstring",
"Represents a data type in lldb.

The actual characteristics of each type are defined by the semantics of the
programming language and the specific language implementation that was used
to compile the target program. See the language-specific notes in the
documentation of each method.

SBType instances can be obtained by a variety of methods.
`SBTarget.FindFirstType` and `SBModule.FindFirstType` can be used to create
`SBType` representations of types in executables/libraries with debug
information. For some languages such as C, C++ and Objective-C it is possible
to create new types by evaluating expressions that define a new type.

Note that most `SBType` properties are computed independently of any runtime
information so for dynamic languages the functionality can be very limited.
`SBValue` can be used to represent runtime values which then can be more
accurately queried for certain information such as byte size.


SBType supports the eq/ne operator. For example,::

    //main.cpp:

    class Task {
    public:
        int id;
        Task *next;
        Task(int i, Task *n):
            id(i),
            next(n)
        {}
    };

    int main (int argc, char const *argv[])
    {
        Task *task_head = new Task(-1, NULL);
        Task *task1 = new Task(1, NULL);
        Task *task2 = new Task(2, NULL);
        Task *task3 = new Task(3, NULL); // Orphaned.
        Task *task4 = new Task(4, NULL);
        Task *task5 = new Task(5, NULL);

        task_head->next = task1;
        task1->next = task2;
        task2->next = task4;
        task4->next = task5;

        int total = 0;
        Task *t = task_head;
        while (t != NULL) {
            if (t->id >= 0)
                ++total;
            t = t->next;
        }
        printf('We have a total number of %d tasks\\n', total);

        // This corresponds to an empty task list.
        Task *empty_task_head = new Task(-1, NULL);

        return 0; // Break at this line
    }

    # find_type.py:

            # Get the type 'Task'.
            task_type = target.FindFirstType('Task')
            self.assertTrue(task_type)

            # Get the variable 'task_head'.
            frame0.FindVariable('task_head')
            task_head_type = task_head.GetType()
            self.assertTrue(task_head_type.IsPointerType())

            # task_head_type is 'Task *'.
            task_pointer_type = task_type.GetPointerType()
            self.assertTrue(task_head_type == task_pointer_type)

            # Get the child mmember 'id' from 'task_head'.
            id = task_head.GetChildMemberWithName('id')
            id_type = id.GetType()

            # SBType.GetBasicType() takes an enum 'BasicType' (lldb-enumerations.h).
            int_type = id_type.GetBasicType(lldb.eBasicTypeInt)
            # id_type and int_type should be the same type!
            self.assertTrue(id_type == int_type)

") lldb::SBType;

%feature("docstring",
    "Returns the number of bytes a variable with the given types occupies in memory.

    Returns ``0`` if the size can't be determined.

    If a type occupies ``N`` bytes + ``M`` bits in memory, this function returns
    the rounded up amount of bytes (i.e., if ``M`` is ``0``,
    this function returns ``N`` and otherwise ``N + 1``).

    Language-specific behaviour:

    * C: The output is expected to match the value of ``sizeof(Type)``. If
      ``sizeof(Type)`` is not a valid expression for the given type, the
      function returns ``0``.
    * C++: Same as in C.
    * Objective-C: Same as in C. For Objective-C classes this always returns
      ``0`` as the actual size depends on runtime information.
    "
) lldb::SBType::GetByteSize;

%feature("docstring",
    "Returns true if this type is a pointer type.

    Language-specific behaviour:

    * C: Returns true for C pointer types (or typedefs of these types).
    * C++: Pointer types include the C pointer types as well as pointers to data
      mebers or member functions.
    * Objective-C: Pointer types include the C pointer types. ``id``, ``Class``
      and pointers to blocks are also considered pointer types.
    "
) lldb::SBType::IsPointerType;

%feature("docstring",
    "Returns true if this type is a reference type.

    Language-specific behaviour:

    * C: Returns false for all types.
    * C++: Both l-value and r-value references are considered reference types.
    * Objective-C: Returns false for all types.
    "
) lldb::SBType::IsReferenceType;

%feature("docstring",
    "Returns true if this type is a polymorphic type.

    Language-specific behaviour:

    * C: Returns false for all types.
    * C++: Returns true if the type is a class type that contains at least one
      virtual member function or if at least one of its base classes is
      considered a polymorphic type.
    * Objective-C: Returns false for all types.
    "
) lldb::SBType::IsPolymorphicClass;

%feature("docstring",
    "Returns true if this type is an array type.

    Language-specific behaviour:

    * C: Returns true if the types is an array type. This includes incomplete
      array types ``T[]`` and array types with integer (``T[1]``) or variable
      length (``T[some_variable]``). Pointer types are not considered arrays.
    * C++: Includes C's array types and dependent array types (i.e., array types
      in templates which size depends on template arguments).
    * Objective-C: Same as in C.
    "
) lldb::SBType::IsArrayType;

%feature("docstring",
    "Returns true if this type is a vector type.

    Language-specific behaviour:

    * C: Returns true if the types is a vector type created with
      GCC's ``vector_size`` or Clang's ``ext_vector_type`` feature.
    * C++: Same as in C.
    * Objective-C: Same as in C.
    "
) lldb::SBType::IsVectorType;

%feature("docstring",
    "Returns true if this type is a typedef.

    Language-specific behaviour:

    * C: Returns true if the type is a C typedef.
    * C++: Same as in C. Also treats type aliases as typedefs.
    * Objective-C: Same as in C.
    "
) lldb::SBType::IsTypedefType;

%feature("docstring",
    "Returns true if this type is an anonymous type.

    Language-specific behaviour:

    * C: Returns true for anonymous unions. Also returns true for
      anonymous structs (which are a GNU language extension).
    * C++: Same as in C.
    * Objective-C: Same as in C.
    "
) lldb::SBType::IsAnonymousType;

%feature("docstring",
    "Returns true if this type is a scoped enum.

    Language-specific behaviour:

    * C: Returns false for all types.
    * C++: Return true only for C++11 scoped enums.
    * Objective-C: Returns false for all types.
    "
) lldb::SBType::IsScopedEnumerationType;

%feature("docstring",
    "Returns true if this type is an aggregate type.

    Language-specific behaviour:

    * C: Returns true for struct values, arrays, and vectors.
    * C++: Same a C. Also includes class instances.
    * Objective-C: Same as C. Also includes class instances.
    "
) lldb::SBType::IsAggregateType;

%feature("docstring",
    "Returns a type that represents a pointer to this type.

    If the type system of the current language can't represent a pointer to this
    type or this type is invalid, an invalid `SBType` is returned.

    Language-specific behaviour:

    * C: Returns the pointer type of this type.
    * C++: Same as in C.
    * Objective-C: Same as in C.
    "
) lldb::SBType::GetPointerType;

%feature("docstring",
    "Returns the underlying pointee type.

    If this type is a pointer type as specified by `IsPointerType` then this
    returns the underlying type. If this is not a pointer type or an invalid
    `SBType` then this returns an invalid `SBType`.

    Language-specific behaviour:

    * C: Returns the underlying type for for C pointer types or typedefs of
      these types). For example, ``int *`` will return ``int``.
    * C++: Same as in C. Returns an `SBType` representation for data members/
      member functions in case the `SBType` is a pointer to data member or
      pointer to member function.
    * Objective-C: Same as in C. The pointee type of ``id`` and ``Class`` is
      an invalid `SBType`. The pointee type of pointers Objective-C types is an
      `SBType` for the non-pointer type of the respective type. For example,
      ``NSString *`` will return ``NSString`` as a pointee type.
    "
) lldb::SBType::GetPointeeType;

%feature("docstring",
    "Returns a type that represents a reference to this type.

    If the type system of the current language can't represent a reference to
    this type, an invalid `SBType` is returned.

    Language-specific behaviour:

    * C: Currently assumes the type system is C++ and returns an l-value
      reference type. For example, ``int`` will return ``int&``. This behavior
      is likely to change in the future and shouldn't be relied on.
    * C++: Same as in C.
    * Objective-C: Same as in C.
    "
) lldb::SBType::GetReferenceType;

%feature("docstring",
    "Returns the underlying type of a typedef.

    If this type is a typedef as designated by `IsTypedefType`, then the
    underlying type is being returned. Otherwise an invalid `SBType` is
    returned.

    Language-specific behaviour:

    * C: Returns the underlying type of a typedef type.
    * C++: Same as in C. For type aliases, the underlying type is returned.
    * Objective-C: Same as in C.
    "
) lldb::SBType::GetTypedefedType;

%feature("docstring",
    "Returns the underlying type of a reference type.

    If this type is a reference as designated by `IsReferenceType`, then the
    underlying type is being returned. Otherwise an invalid `SBType` is
    returned.

    Language-specific behaviour:

    * C: Always returns an invalid type.
    * C++: For l-value and r-value references the underlying type is returned.
      For example, ``int &`` will return ``int``.
    * Objective-C: Same as in C.
    "
) lldb::SBType::GetDereferencedType;

%feature("docstring",
    "Returns the unqualified version of this type.

    Language-specific behaviour:

    * C: If this type with any const or volatile specifier removed.
    * C++: Same as in C.
    * Objective-C: Same as in C.
    "
) lldb::SBType::GetUnqualifiedType;

%feature("docstring",
    "Returns the underlying integer type if this is an enumeration type.

    If this type is an invalid `SBType` or not an enumeration type an invalid
    `SBType` is returned.

    Language-specific behaviour:

    * C: Returns the underlying type for enums.
    * C++: Same as in C but also returns the underlying type for scoped enums.
    * Objective-C: Same as in C.
    "
) lldb::SBType::GetEnumerationIntegerType;

%feature("docstring",
    "Returns the array element type if this type is an array type.

    Otherwise returns an invalid `SBType` if this type is invalid or not an
    array type.

    Language-specific behaviour:

    * C: If this is an array type (see `IsArrayType`) such as ``T[]``, returns
      the element type.
    * C++: Same as in C.
    * Objective-C: Same as in C.

    See also `IsArrayType`.
    "
) lldb::SBType::GetArrayElementType;

%feature("docstring",
    "Returns the array type with the given constant size.

    Language-specific behaviour:

    * C: Returns a constant-size array ``T[size]`` for any non-void type.
    * C++: Same as in C.
    * Objective-C: Same as in C.

    See also `IsArrayType` and `GetArrayElementType`.
    "
) lldb::SBType::GetArrayType;

%feature("docstring",
    "Returns the vector element type if this type is a vector type.

    Otherwise returns an invalid `SBType` if this type is invalid or not a
    vector type.

    Language-specific behaviour:

    * C: If this is a vector type (see `IsVectorType`), returns the element
      type.
    * C++: Same as in C.
    * Objective-C: Same as in C.

    See also `IsVectorType`.
    "
) lldb::SBType::GetVectorElementType;

%feature("docstring",
    "Returns the `BasicType` value that is most appropriate to this type.

    Returns `eBasicTypeInvalid` if no appropriate `BasicType` was found or this
    type is invalid. See the `BasicType` documentation for the language-specific
    meaning of each `BasicType` value.

    **Overload behaviour:** When called with a `BasicType` parameter, the
    following behaviour applies:

    Returns the `SBType` that represents the passed `BasicType` value. Returns
    an invalid `SBType` if no fitting `SBType` could be created.

    Language-specific behaviour:

    * C: Returns the respective builtin type. Note that some types
      (e.g. ``__uint128_t``) might even be successfully created even if they are
      not available on the target platform. C++ and Objective-C specific types
      might also be created even if the target program is not written in C++ or
      Objective-C.
    * C++: Same as in C.
    * Objective-C: Same as in C.
    "
) lldb::SBType::GetBasicType;

%feature("docstring",
    "Returns the number of fields of this type.

    Returns ``0`` if this type does not have fields.

    Language-specific behaviour:

    * C: Returns the number of fields if the type is a struct. If the type
      contains an anonymous struct/union it only counts as a single field (even
      if the struct/union contains several fields).
    * C++: Returns the number of non-static fields if the type is a
      struct/class. If the type contains an anonymous struct/union it only
      counts as a single field (even if the struct/union contains several
      fields). The fields of any base classes are not included in the count.
    * Objective-C: Same as in C for structs. For Objective-C classes the number
      of ivars is returned.

    See also `GetFieldAtIndex`.
    "
) lldb::SBType::GetNumberOfFields;

%feature("docstring",
    "Returns the number of base/parent classes of this type.

    Returns ``0`` if this type doesn't have any base classes.

    Language-specific behaviour:

    * C: Returns always ``0``.
    * C++: The number of direct non-virtual base classes if this type is
      a class.
    * Objective-C: The number of super classes for Objective-C classes.
      As Objective-C doesn't have multiple inheritance this is usually returns 1
      except for NSObject.
    "
) lldb::SBType::GetNumberOfDirectBaseClasses;

%feature("docstring",
    "Returns the number of virtual base/parent classes of this type

    Returns ``0`` if this type doesn't have any base classes.

    Language-specific behaviour:

    * C: Returns always ``0``.
    * C++: The number of direct virtual base classes if this type is a
      class.
    * Objective-C: Returns always ``0``.
    "
) lldb::SBType::GetNumberOfVirtualBaseClasses;

%feature("docstring",
    "Returns the field at the given index.

    Returns an invalid `SBType` if the index is out of range or the current
    type doesn't have any fields.

    Language-specific behaviour:

    * C: Returns the field with the given index for struct types. Fields are
      ordered/indexed starting from ``0`` for the first field in a struct (as
      declared in the definition).
    * C++: Returns the non-static field with the given index for struct types.
      Fields are ordered/indexed starting from ``0`` for the first field in a
      struct (as declared in the definition).
    * Objective-C: Same as in C for structs. For Objective-C classes the ivar
      with the given index is returned. ivars are indexed starting from ``0``.
    "
) lldb::SBType::GetFieldAtIndex;

%feature("docstring",
    "Returns the direct base class as indexed by `GetNumberOfDirectBaseClasses`.

    Returns an invalid SBTypeMember if the index is invalid or this SBType is
    invalid.
    "
) lldb::SBType::GetDirectBaseClassAtIndex;

%feature("docstring",
    "Returns the virtual base class as indexed by
    `GetNumberOfVirtualBaseClasses`.

    Returns an invalid SBTypeMember if the index is invalid or this SBType is
    invalid.
    "
) lldb::SBType::GetVirtualBaseClassAtIndex;

%feature("docstring",
    "Returns the `SBModule` this `SBType` belongs to.

    Returns no `SBModule` if this type does not belong to any specific
    `SBModule` or this `SBType` is invalid. An invalid `SBModule` might also
    indicate that once came from an `SBModule` but LLDB could no longer
    determine the original module.
    "
) lldb::SBType::GetModule;

%feature("autodoc", "GetName() -> string") lldb::SBType::GetName;

%feature("docstring",
    "Returns the name of this type.

    Returns an empty string if an error occurred or this type is invalid.

    Use this function when trying to match a specific type by name in a script.
    The names returned by this function try to uniquely identify a name but
    conflicts can occur (for example, if a C++ program contains two different
    classes with the same name in different translation units. `GetName` can
    return the same name for both class types.)


    Language-specific behaviour:

    * C: The name of the type. For structs the ``struct`` prefix is omitted.
    * C++: Returns the qualified name of the type (including anonymous/inline
      namespaces and all template arguments).
    * Objective-C: Same as in C.
    "
) lldb::SBType::GetName;

%feature("autodoc", "GetDisplayTypeName() -> string") lldb::SBType::GetDisplayTypeName;

%feature("docstring",
    "Returns the name of this type in a user-friendly format.

    Returns an empty string if an error occurred or this type is invalid.

    Use this function when displaying a type name to the user.

    Language-specific behaviour:

    * C: Returns the type name. For structs the ``struct`` prefix is omitted.
    * C++: Returns the qualified name. Anonymous/inline namespaces are omitted.
      Template arguments that match their default value might also be hidden
      (this functionality depends on whether LLDB can determine the template's
      default arguments).
    * Objective-C: Same as in C.
    "
) lldb::SBType::GetDisplayTypeName;

%feature("autodoc", "GetTypeClass() -> TypeClass") lldb::SBType::GetTypeClass;

%feature("docstring",
    "Returns the `TypeClass` for this type.

    Returns an `eTypeClassInvalid` if this `SBType` is invalid.

    See `TypeClass` for the language-specific meaning of each `TypeClass` value.
    "
) lldb::SBType::GetTypeClass;

%feature("docstring",
    "Returns the number of template arguments of this type.

    Returns ``0`` if this type is not a template.

    Language-specific behaviour:

    * C: Always returns ``0``.
    * C++: If this type is a class template instantiation then this returns the
      number of template parameters that were used in this instantiation. This
      includes both explicit and implicit template parameters.
    * Objective-C: Always returns ``0``.
    "
) lldb::SBType::GetNumberOfTemplateArguments;

%feature("docstring",
    "Returns the type of the template argument with the given index.

    Returns an invalid `SBType` if there is no template argument with the given
    index or this type is not a template. The first template  argument has the
    index ``0``.

    Language-specific behaviour:

    * C: Always returns an invalid SBType.
    * C++: If this type is a class template instantiation and the template
      parameter with the given index is a type template parameter, then this
      returns the type of that parameter. Otherwise returns an invalid `SBType`.
    * Objective-C: Always returns an invalid SBType.
    "
) lldb::SBType::GetTemplateArgumentType;

%feature("docstring",
    "Returns the kind of the template argument with the given index.

    Returns `eTemplateArgumentKindNull` if there is no template argument
    with the given index or this type is not a template. The first template
    argument has the index ``0``.

    Language-specific behaviour:

    * C: Always returns `eTemplateArgumentKindNull`.
    * C++: If this type is a class template instantiation then this returns
      the appropriate `TemplateArgument` value for the parameter with the given
      index. See the documentation of `TemplateArgument` for how certain C++
      template parameter kinds are mapped to `TemplateArgument` values.
    * Objective-C: Always returns `eTemplateArgumentKindNull`.
    "
) lldb::SBType::GetTemplateArgumentKind;

%feature("docstring",
    "Returns the return type if this type represents a function.

    Returns an invalid `SBType` if this type is not a function type or invalid.

    Language-specific behaviour:

    * C: For functions return the return type. Returns an invalid `SBType` if
      this type is a function pointer type.
    * C++: Same as in C for functions and instantiated template functions.
      Member functions are also considered functions. For functions that have
      their return type specified by a placeholder type specifier (``auto``)
      this returns the deduced return type.
    * Objective-C: Same as in C for functions. For Objective-C methods this
      returns the return type of the method.
    "
) lldb::SBType::GetFunctionReturnType;

%feature("docstring",
    "Returns the list of argument types if this type represents a function.

    Returns an invalid `SBType` if this type is not a function type or invalid.

    Language-specific behaviour:

    * C: For functions return the types of each parameter. Returns an invalid
      `SBType` if this type is a function pointer. For variadic functions this
      just returns the list of parameters before the variadic arguments.
    * C++: Same as in C for functions and instantiated template functions.
      Member functions are also considered functions.
    * Objective-C: Always returns an invalid SBType for Objective-C methods.
    "
) lldb::SBType::GetFunctionArgumentTypes;

%feature("docstring",
    "Returns the number of member functions of this type.

    Returns ``0`` if an error occurred or this type is invalid.

    Language-specific behaviour:

    * C: Always returns ``0``.
    * C++: If this type represents a struct/class, then the number of
      member functions (static and non-static) is returned. The count includes
      constructors and destructors (both explicit and implicit). Member
      functions of base classes are not included in the count.
    * Objective-C: If this type represents a struct/class, then the
      number of methods is returned. Methods in categories or super classes
      are not counted.
    "
) lldb::SBType::GetNumberOfMemberFunctions;

%feature("docstring",
    "Returns the member function of this type with the given index.

    Returns an invalid `SBTypeMemberFunction` if the index is invalid or this
    type is invalid.

    Language-specific behaviour:

    * C: Always returns an invalid `SBTypeMemberFunction`.
    * C++: Returns the member function or constructor/destructor with the given
      index.
    * Objective-C: Returns the method with the given index.

    See `GetNumberOfMemberFunctions` for what functions can be queried by this
    function.
    "
) lldb::SBType::GetMemberFunctionAtIndex;

%feature("docstring",
    "Returns true if the type is completely defined.

    Language-specific behaviour:

    * C: Returns false for struct types that were only forward declared in the
      type's `SBTarget`/`SBModule`. Otherwise returns true.
    * C++: Returns false for template/non-template struct/class types and
      scoped enums that were only forward declared inside the type's
      `SBTarget`/`SBModule`. Otherwise returns true.
    * Objective-C: Follows the same behavior as C for struct types. Objective-C
      classes are considered complete unless they were only forward declared via
      ``@class ClassName`` in the type's `SBTarget`/`SBModule`. Otherwise
      returns true.
    "
) lldb::SBType::IsTypeComplete;

%feature("docstring",
    "Returns the `TypeFlags` values for this type.

    See the respective `TypeFlags` values for what values can be set. Returns an
    integer in which each `TypeFlags` value is represented by a bit. Specific
    flags can be checked via Python's bitwise operators. For example, the
    `eTypeIsInteger` flag can be checked like this:

    ``(an_sb_type.GetTypeFlags() & lldb.eTypeIsInteger) != 0``

    If this type is invalid this returns ``0``.

    See the different values for `TypeFlags` for the language-specific meanings
    of each `TypeFlags` value.
    "
) lldb::SBType::GetTypeFlags;

%feature("docstring",
    "Searches for a directly nested type that has the provided name.

    Returns the type if it was found.
    Returns invalid type if nothing was found.
    "
) lldb::SBType::FindDirectNestedType;

%feature("docstring",
"Represents a list of :py:class:`SBType` s.

The FindTypes() method of :py:class:`SBTarget`/:py:class:`SBModule` returns a SBTypeList.

SBTypeList supports :py:class:`SBType` iteration. For example,

.. code-block:: cpp

    // main.cpp:

    class Task {
    public:
        int id;
        Task *next;
        Task(int i, Task *n):
            id(i),
            next(n)
        {}
    };

.. code-block:: python

    # find_type.py:

    # Get the type 'Task'.
    type_list = target.FindTypes('Task')
    self.assertTrue(len(type_list) == 1)
    # To illustrate the SBType iteration.
    for type in type_list:
        # do something with type

") lldb::SBTypeList;
