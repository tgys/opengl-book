set(CMAKE_CXX_COMPILER "/nix/store/88ghxafjpqp5sqpd75r51qqg4q5d95ss-gcc-wrapper-10.3.0/bin/g++")
set(CMAKE_CXX_COMPILER_ARG1 "")
set(CMAKE_CXX_COMPILER_ID "GNU")
set(CMAKE_CXX_COMPILER_VERSION "10.3.0")
set(CMAKE_CXX_COMPILER_VERSION_INTERNAL "")
set(CMAKE_CXX_COMPILER_WRAPPER "")
set(CMAKE_CXX_STANDARD_COMPUTED_DEFAULT "14")
set(CMAKE_CXX_COMPILE_FEATURES "cxx_std_98;cxx_template_template_parameters;cxx_std_11;cxx_alias_templates;cxx_alignas;cxx_alignof;cxx_attributes;cxx_auto_type;cxx_constexpr;cxx_decltype;cxx_decltype_incomplete_return_types;cxx_default_function_template_args;cxx_defaulted_functions;cxx_defaulted_move_initializers;cxx_delegating_constructors;cxx_deleted_functions;cxx_enum_forward_declarations;cxx_explicit_conversions;cxx_extended_friend_declarations;cxx_extern_templates;cxx_final;cxx_func_identifier;cxx_generalized_initializers;cxx_inheriting_constructors;cxx_inline_namespaces;cxx_lambdas;cxx_local_type_template_args;cxx_long_long_type;cxx_noexcept;cxx_nonstatic_member_init;cxx_nullptr;cxx_override;cxx_range_for;cxx_raw_string_literals;cxx_reference_qualified_functions;cxx_right_angle_brackets;cxx_rvalue_references;cxx_sizeof_member;cxx_static_assert;cxx_strong_enums;cxx_thread_local;cxx_trailing_return_types;cxx_unicode_literals;cxx_uniform_initialization;cxx_unrestricted_unions;cxx_user_literals;cxx_variadic_macros;cxx_variadic_templates;cxx_std_14;cxx_aggregate_default_initializers;cxx_attribute_deprecated;cxx_binary_literals;cxx_contextual_conversions;cxx_decltype_auto;cxx_digit_separators;cxx_generic_lambdas;cxx_lambda_init_captures;cxx_relaxed_constexpr;cxx_return_type_deduction;cxx_variable_templates;cxx_std_17;cxx_std_20")
set(CMAKE_CXX98_COMPILE_FEATURES "cxx_std_98;cxx_template_template_parameters")
set(CMAKE_CXX11_COMPILE_FEATURES "cxx_std_11;cxx_alias_templates;cxx_alignas;cxx_alignof;cxx_attributes;cxx_auto_type;cxx_constexpr;cxx_decltype;cxx_decltype_incomplete_return_types;cxx_default_function_template_args;cxx_defaulted_functions;cxx_defaulted_move_initializers;cxx_delegating_constructors;cxx_deleted_functions;cxx_enum_forward_declarations;cxx_explicit_conversions;cxx_extended_friend_declarations;cxx_extern_templates;cxx_final;cxx_func_identifier;cxx_generalized_initializers;cxx_inheriting_constructors;cxx_inline_namespaces;cxx_lambdas;cxx_local_type_template_args;cxx_long_long_type;cxx_noexcept;cxx_nonstatic_member_init;cxx_nullptr;cxx_override;cxx_range_for;cxx_raw_string_literals;cxx_reference_qualified_functions;cxx_right_angle_brackets;cxx_rvalue_references;cxx_sizeof_member;cxx_static_assert;cxx_strong_enums;cxx_thread_local;cxx_trailing_return_types;cxx_unicode_literals;cxx_uniform_initialization;cxx_unrestricted_unions;cxx_user_literals;cxx_variadic_macros;cxx_variadic_templates")
set(CMAKE_CXX14_COMPILE_FEATURES "cxx_std_14;cxx_aggregate_default_initializers;cxx_attribute_deprecated;cxx_binary_literals;cxx_contextual_conversions;cxx_decltype_auto;cxx_digit_separators;cxx_generic_lambdas;cxx_lambda_init_captures;cxx_relaxed_constexpr;cxx_return_type_deduction;cxx_variable_templates")
set(CMAKE_CXX17_COMPILE_FEATURES "cxx_std_17")
set(CMAKE_CXX20_COMPILE_FEATURES "cxx_std_20")

set(CMAKE_CXX_PLATFORM_ID "Linux")
set(CMAKE_CXX_SIMULATE_ID "")
set(CMAKE_CXX_COMPILER_FRONTEND_VARIANT "")
set(CMAKE_CXX_SIMULATE_VERSION "")




set(CMAKE_AR "/nix/store/5ddb4j8z84p6sjphr0kh6cbq5jd12ncs-binutils-2.35.1/bin/ar")
set(CMAKE_CXX_COMPILER_AR "/nix/store/sjhz1j2d1ssn59f66kqp92xj9mpsww2d-gcc-10.3.0/bin/gcc-ar")
set(CMAKE_RANLIB "/nix/store/5ddb4j8z84p6sjphr0kh6cbq5jd12ncs-binutils-2.35.1/bin/ranlib")
set(CMAKE_CXX_COMPILER_RANLIB "/nix/store/sjhz1j2d1ssn59f66kqp92xj9mpsww2d-gcc-10.3.0/bin/gcc-ranlib")
set(CMAKE_LINKER "/nix/store/88ghxafjpqp5sqpd75r51qqg4q5d95ss-gcc-wrapper-10.3.0/bin/ld")
set(CMAKE_MT "")
set(CMAKE_COMPILER_IS_GNUCXX 1)
set(CMAKE_CXX_COMPILER_LOADED 1)
set(CMAKE_CXX_COMPILER_WORKS TRUE)
set(CMAKE_CXX_ABI_COMPILED TRUE)
set(CMAKE_COMPILER_IS_MINGW )
set(CMAKE_COMPILER_IS_CYGWIN )
if(CMAKE_COMPILER_IS_CYGWIN)
  set(CYGWIN 1)
  set(UNIX 1)
endif()

set(CMAKE_CXX_COMPILER_ENV_VAR "CXX")

if(CMAKE_COMPILER_IS_MINGW)
  set(MINGW 1)
endif()
set(CMAKE_CXX_COMPILER_ID_RUN 1)
set(CMAKE_CXX_SOURCE_FILE_EXTENSIONS C;M;c++;cc;cpp;cxx;m;mm;CPP)
set(CMAKE_CXX_IGNORE_EXTENSIONS inl;h;hpp;HPP;H;o;O;obj;OBJ;def;DEF;rc;RC)

foreach (lang C OBJC OBJCXX)
  if (CMAKE_${lang}_COMPILER_ID_RUN)
    foreach(extension IN LISTS CMAKE_${lang}_SOURCE_FILE_EXTENSIONS)
      list(REMOVE_ITEM CMAKE_CXX_SOURCE_FILE_EXTENSIONS ${extension})
    endforeach()
  endif()
endforeach()

set(CMAKE_CXX_LINKER_PREFERENCE 30)
set(CMAKE_CXX_LINKER_PREFERENCE_PROPAGATES 1)

# Save compiler ABI information.
set(CMAKE_CXX_SIZEOF_DATA_PTR "8")
set(CMAKE_CXX_COMPILER_ABI "ELF")
set(CMAKE_CXX_LIBRARY_ARCHITECTURE "")

if(CMAKE_CXX_SIZEOF_DATA_PTR)
  set(CMAKE_SIZEOF_VOID_P "${CMAKE_CXX_SIZEOF_DATA_PTR}")
endif()

if(CMAKE_CXX_COMPILER_ABI)
  set(CMAKE_INTERNAL_PLATFORM_ABI "${CMAKE_CXX_COMPILER_ABI}")
endif()

if(CMAKE_CXX_LIBRARY_ARCHITECTURE)
  set(CMAKE_LIBRARY_ARCHITECTURE "")
endif()

set(CMAKE_CXX_CL_SHOWINCLUDES_PREFIX "")
if(CMAKE_CXX_CL_SHOWINCLUDES_PREFIX)
  set(CMAKE_CL_SHOWINCLUDES_PREFIX "${CMAKE_CXX_CL_SHOWINCLUDES_PREFIX}")
endif()





set(CMAKE_CXX_IMPLICIT_INCLUDE_DIRECTORIES "/nix/store/91lcdbwj9j9yn7kp1klgvwsqz8gclrwq-python-2.7.18-env/include;/nix/store/dyqj7vxnwmi0i64gv9pdjj3nwf2vs95b-glu-9.0.1-dev/include;/nix/store/3qkrfwai51j0b1sysw8443j8rlw8c09i-libGL-1.3.3-dev/include;/nix/store/5faibianv8qizafps484a9w15ds68si2-assimp-5.0.1/include;/nix/store/y0p9gpk51gds595q1m3cb7wscr3znx2v-freetype-2.10.4-dev/include;/nix/store/1l6jgfsgjmsrbrciz8r714dnkyngvpkb-zlib-1.2.11-dev/include;/nix/store/awy3kjibjzvwbay2b4l5rmpnkvyhvqzq-bzip2-1.0.6.0.2-dev/include;/nix/store/bid5jacyr52nk2bpvbzkk6s32vl05kny-libpng-apng-1.6.37-dev/include;/nix/store/pljnsfxm0f13iacwgpw8zgi88xa8rq7l-libXext-1.3.4-dev/include;/nix/store/88bkfkrvh4r94sy307sspzy9qr20rrkk-xorgproto-2021.3/include;/nix/store/rlhz6569aggcylqgkgh5qh9ncl3j2s3p-libXau-1.0.9-dev/include;/nix/store/jhs93x916spxrn8i2q3d3vbkij4p7ll4-boost-1.69.0-dev/include;/nix/store/6kscfi5i711lhzpi2jisjhlphwdb7k3g-libX11-1.7.2-dev/include;/nix/store/h47nnlncnnnvjsl77nh4ckgv80x8svmj-libxcb-1.14-dev/include;/nix/store/3ffrj7xxqdh0sl82cfb8k9lwj5jpg49l-mesa-21.1.4-dev/include;/nix/store/crl01xc7f5mg016gjwzb6lw75zpkpm5i-libXdamage-1.1.5-dev/include;/nix/store/ss8a6mf00b6cgk97msgcpgvx49xmfxsj-libXfixes-5.0.3-dev/include;/nix/store/z84ni5ryy6kk9yxay1aaa7ri43sw6nh7-libXxf86vm-1.1.4-dev/include;/nix/store/4qpfi447q91m33mr0a9isfyixwsx07gn-libdrm-2.4.105-dev/include;/nix/store/jj40b48rph45y4s44saqvi1dljbwm5ir-libXrandr-1.5.2-dev/include;/nix/store/h581yhbmx6g849xkxsbn6hzig0p1dh6k-libXrender-0.9.10-dev/include;/nix/store/7dqh9p4yzq72bvffy766r74193nmj9ph-libXinerama-1.1.4-dev/include;/nix/store/z9dzxaxgjdhhif3i2llzqpg46prancq3-libXcursor-1.2.0-dev/include;/nix/store/ff7xamspx9bidl3cds0csih6chzq5kri-libXi-1.7.10-dev/include;/nix/store/6rwb8iglx10n57nnwyl2ix973cnvxmrm-freeglut-3.2.1-dev/include;/nix/store/a0j4adpnc5z9000lrhil0sv1xcj9y87k-glfw-3.3.4/include;/nix/store/dacixn382q0ksfz26q7nd27lsqs59hs2-stb-20180211/include;/nix/store/sjhz1j2d1ssn59f66kqp92xj9mpsww2d-gcc-10.3.0/include/c++/10.3.0;/nix/store/sjhz1j2d1ssn59f66kqp92xj9mpsww2d-gcc-10.3.0/include/c++/10.3.0/x86_64-unknown-linux-gnu;/nix/store/sjhz1j2d1ssn59f66kqp92xj9mpsww2d-gcc-10.3.0/include/c++/10.3.0/backward;/nix/store/sjhz1j2d1ssn59f66kqp92xj9mpsww2d-gcc-10.3.0/lib/gcc/x86_64-unknown-linux-gnu/10.3.0/include;/nix/store/sjhz1j2d1ssn59f66kqp92xj9mpsww2d-gcc-10.3.0/include;/nix/store/sjhz1j2d1ssn59f66kqp92xj9mpsww2d-gcc-10.3.0/lib/gcc/x86_64-unknown-linux-gnu/10.3.0/include-fixed;/nix/store/q141hd8jl7in5223jmf7kmx9h517km4p-glibc-2.32-54-dev/include")
set(CMAKE_CXX_IMPLICIT_LINK_LIBRARIES "stdc++;m;gcc_s;gcc;c;gcc_s;gcc")
set(CMAKE_CXX_IMPLICIT_LINK_DIRECTORIES "/nix/store/91lcdbwj9j9yn7kp1klgvwsqz8gclrwq-python-2.7.18-env/lib;/nix/store/c35w5n0prvq4v4priyi8fiiq361pmyvp-libGL-1.3.3/lib;/nix/store/45zg04mnwym5mpjhnbn820xprgzdalsp-libglvnd-1.3.3/lib;/nix/store/pijpdp46xl6262sw5l2jpf049v15kjc4-glu-9.0.1/lib;/nix/store/5faibianv8qizafps484a9w15ds68si2-assimp-5.0.1/lib;/nix/store/ds491f6b5pdk3xxnc2w103asyz1y4cfc-zlib-1.2.11/lib;/nix/store/dp18s34dnzbllx16cyb2pwz0fm19nbnf-bzip2-1.0.6.0.2/lib;/nix/store/81lnjakavryf3c9ss0lfkkkxj67ffm59-libpng-apng-1.6.37/lib;/nix/store/xjs2zxbxphkqpqp6vfgpdg53gmhzh9y1-freetype-2.10.4/lib;/nix/store/487yx75kpzrpvy3cykky49x0ykdivppj-libXau-1.0.9/lib;/nix/store/xwcsn0d3c5d51b3hcafqkn1nvs7pwz7b-libXext-1.3.4/lib;/nix/store/w39sq8msn27a205hz0ccawdx98gxj5ar-boost-1.69.0/lib;/nix/store/qggz6201zlzmzms3zia2c74718ww1cjb-libxcb-1.14/lib;/nix/store/60y70c7swi78g160rqm9cfbrhl8ax854-libX11-1.7.2/lib;/nix/store/fxkf8rdx473cv5kdgrqwprq2r3jmcxia-libXfixes-5.0.3/lib;/nix/store/f6rqrgjfjvg17977lanya1v3hfynz7gn-libXdamage-1.1.5/lib;/nix/store/0pzs563gv1xrfd7xxs8bxkcswi3sipa0-libXxf86vm-1.1.4/lib;/nix/store/4h0351bv3pyh1rrg32sm181dj5wjavqq-libdrm-2.4.105/lib;/nix/store/1fgkxk5valf4kzh14rci0xj6xyvrjwrx-mesa-21.1.4/lib;/nix/store/7mwmrfgcvqxdjjg93ibrqr6x4b00y4p4-libXrender-0.9.10/lib;/nix/store/y40r8876fq6n9x4f9yvsalx0mdfwm9yb-libXrandr-1.5.2/lib;/nix/store/pb7ji4w7n33rvnv3gfyjdg6ky2svcs0r-libXinerama-1.1.4/lib;/nix/store/j5f1xshqxwlka3jmigxs6c6z4qqh8qix-libXcursor-1.2.0/lib;/nix/store/awbddyzi6cw1gqwx6sc9z2l0f8symjx7-libXi-1.7.10/lib;/nix/store/p396sg5dy4n8845ik9jnqgrc3fljihpw-freeglut-3.2.1/lib;/nix/store/a0j4adpnc5z9000lrhil0sv1xcj9y87k-glfw-3.3.4/lib;/nix/store/jsp3h3wpzc842j0rz61m5ly71ak6qgdn-glibc-2.32-54/lib;/nix/store/50msfhkz5wbyk8i78pjv3y9lxdrp7dlm-gcc-10.3.0-lib/lib;/nix/store/88ghxafjpqp5sqpd75r51qqg4q5d95ss-gcc-wrapper-10.3.0/bin;/nix/store/sjhz1j2d1ssn59f66kqp92xj9mpsww2d-gcc-10.3.0/lib/gcc/x86_64-unknown-linux-gnu/10.3.0;/nix/store/sjhz1j2d1ssn59f66kqp92xj9mpsww2d-gcc-10.3.0/lib64;/nix/store/sjhz1j2d1ssn59f66kqp92xj9mpsww2d-gcc-10.3.0/lib")
set(CMAKE_CXX_IMPLICIT_LINK_FRAMEWORK_DIRECTORIES "")
