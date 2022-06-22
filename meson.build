project('c_mui',['c'],default_options:['warning_level=0','c_std=gnu99'], version : '0.0.1')
#############################################################
timeout_bin = find_program('timeout', required : true)
#############################################################
cc = meson.get_compiler('c')
meson_deps_dir = join_paths(meson.current_source_dir(),'submodules/meson_deps')
pwd = meson.current_source_dir()
mui_global_linkflags = ['-Wl,-framework,OpenGL']
add_project_link_arguments(mui_global_linkflags, language : ['c'])
sdl2_dep = dependency('sdl2',fallback:['sdl2', 'sdl2'], required: true)
sdl2_image_dep = dependency('sdl2_image',fallback:['sdl2_image', 'sdl2_image'], required: true)
#############################################################
inc = [
  '.',
  include_directories('.','vendor','vendor/sqlite'),
  include_directories('./mui','./mui-render'),
  './mui-atlas',
  './mui',
  './submodules',
  './submodules/meson_deps',
  './submodules/meson_deps/submodules',
  './submodules/meson_deps/meson',
  include_directories(
    './submodules',
    './submodules/meson_deps',
    './submodules/meson_deps/submodules',
    './submodules/meson_deps/submodules/c_ansi',
    './submodules/meson_deps/vendor',
    './submodules/meson_deps/vendor/sqlite',
  ),
  include_directories(
    './submodules/SDL_image',
    './submodules/SDL_image/Xcode/macOS/SDL2.framework/Versions/A/Headers',
  ),
  include_directories('.'),
]
#//////////////////
configure_file(
  input: 'assets/window_icon.bmp',  
  output: 'window_icon.bmp',   
  copy: true,
)
#//////////////////
subdir('submodules')
#//////////////////
subdir('mui')
subdir('mui-atlas')
subdir('mui-render')
#//////////////////
subdir('mui-test')
#//////////////////
subdir('mui-meson-repos')
#//////////////////
#subdir('mui-colors')
#subdir('mui-colors-test')
#//////////////////
