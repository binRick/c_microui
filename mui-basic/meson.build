mui_basic_timeout_seconds = '.8'
mui_basic_srcs = [
  'mui-basic.c',
]
mui_basic_inc = [
 inc,
 '.',
 '../mui-basic',
 '../mui-atlas',
 '../mui',
 '../mui-render',
 '../submodules/c_deps',
 '../submodules/c_deps/submodules',
 '../submodules/c_deps/submodules/c_ansi',
 '../submodules/c_deps/submodules/c_ansi/ansi-utils',
 '../submodules/c_deps/submodules/djbhash',
 '../submodules/c_deps/submodules/djbhash/src',
]
mui_basic_deps = [
   timestamp_dep,
   djbhash_dep,
   sdl2_dep,
   sdl2_image_dep,
   sdl2_ttf_dep,
   mui_dep,
   mui_render_dep,
   cargs_dep,
   colors_csv_parser_dep,
   colors_json_parser_dep,
   c_stringfn_dep,
   c_vector_dep,
   c_stringfn_dep,
   parson_dep,
   fs_dep,
   rgba_dep,
   ansi_codes_dep,
   ansi_rgb_utils_dep,
   bytes_dep,
   c_string_buffer_dep,
   db_dep,
   ansi_utils_deps, 
]
mui_basic_c_args = [
]

mui_basic_lib = static_library('mui-basic',
  mui_basic_srcs,
  install: false,
  dependencies: mui_basic_deps,
  c_args: mui_basic_c_args,
  include_directories: mui_basic_inc,
)

mui_basic_dep = declare_dependency(
  include_directories: mui_basic_inc,
  link_with: mui_basic_lib,
)
