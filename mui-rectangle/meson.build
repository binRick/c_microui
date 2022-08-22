mui_rectangle_timeout_seconds = '.8'
mui_rectangle_srcs = [
  'mui-rectangle.c',
]
mui_rectangle_inc = [
 inc,
 '.',
 '../mui-rectangle',
 '../mui-atlas',
 '../mui',
 '../mui-render',
 '../submodules/c_deps',
 '../submodules/c_deps/submodules',
 '../submodules/c_deps/submodules/c_darwin',
 '../submodules/c_deps/submodules/c_ansi',
 '../submodules/c_deps/submodules/c_darwin/window-utils',
 '../submodules/c_deps/submodules/c_darwin/active-app',
 '../submodules/c_deps/submodules/c_darwin/process',
 '../submodules/c_deps/submodules/c_darwin/rectangle',
 '../submodules/c_deps/submodules/c_ansi/ansi-utils',
 '../submodules/c_deps/submodules/djbhash',
 '../submodules/c_deps/submodules/djbhash/src',
]
mui_rectangle_deps = [
   window_utils_dep,
   tmt_dep,
   b64_dep,
   rectangle_dep,
   process_dep,
   app_utils_dep,
   timestamp_dep,
   djbhash_dep,
   sdl2_dep,
   sdl2_image_dep,
   sdl2_ttf_dep,
   mui_dep,
   mui_render_dep,
   active_app_dep,
   osx_keys_dep,
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
   process_dep,
   ansi_rgb_utils_dep,
   bytes_dep,
   ms_dep,
   c_string_buffer_dep,
   db_dep,
   ansi_utils_deps, 
]
mui_rectangle_c_args = [
]

mui_rectangle_lib = static_library('mui-rectangle',
  mui_rectangle_srcs,
  install: false,
  dependencies: mui_rectangle_deps,
  c_args: mui_rectangle_c_args,
  include_directories: mui_rectangle_inc,
)

mui_rectangle_dep = declare_dependency(
  include_directories: mui_rectangle_inc,
  link_with: mui_rectangle_lib,
)
