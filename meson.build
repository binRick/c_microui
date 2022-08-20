project('c_mui',['c'],default_options:['warning_level=0','c_std=gnu99','default_library=static'], version : '0.0.1',subproject_dir: 'submodules/c_deps/subprojects')
cc = meson.get_compiler('c')
pwd = meson.current_source_dir()
sdl2_config_bin = find_program('sdl2-config',required:true)

sdl2_libs_args = run_command(sdl2_config_bin, '--libs', check: true).stdout().strip()
sdl2_cflags_args = run_command(sdl2_config_bin, '--cflags', check: true).stdout().strip()
c_deps_dir = join_paths(meson.current_source_dir(),'submodules/c_deps')
add_global_arguments(
  '-I/usr/local/Cellar/sdl2_image/2.6.1/include',
  '-I/usr/local/Cellar/sdl2_image/2.6.1/include/SDL2',
  '-I/usr/local/Cellar/sdl2_ttf/2.20.0/include',
  '-I/usr/local/Cellar/sdl2_ttf/2.20.0/include/SDL2',
  '-I/usr/local/Cellar/sdl2/2.0.22/include',
  '-I/usr/local/Cellar/sdl2/2.0.22/include/SDL2',
  '-Wno-unused-command-line-argument',
  '-D_THREAD_SAFE',
  '-L/usr/local/lib',
  '-lSDL2','-lSDL2_ttf','-lSDL2_image',
  sdl2_cflags_args,
  sdl2_libs_args,
  language : 'c',
)
#############################################################
inc = [
  include_directories('./'),
  './',
  include_directories('./mui-windows'),
  include_directories('/usr/local/include'),
  include_directories('/usr/local/Cellar/sdl2/2.0.22/include'),
  '/usr/local/Cellar/sdl2/2.0.22/include',
  '/usr/local/Cellar/sdl2_ttf/2.20.0/include','/usr/local/Cellar/sdl2_image/2.6.1/include',
  include_directories('./submodules/c_deps/submodules/c_ansi'),
  include_directories('./submodules/c_deps/submodules/c_ansi/ansi-codes'),
  include_directories('./submodules/c_deps/submodules'),
  include_directories(
    './submodules/c_deps/submodules/c_ansi/ansi-codes',
    './submodules/c_deps/submodules/c_ansi',
  ),
  include_directories(
    './submodules/c_deps/submodules/c_darwin',
    './submodules/c_deps/submodules/c_darwin/active-app',
    './submodules/c_deps/submodules/c_darwin/window-utils',
    './submodules/c_deps/submodules/c_darwin/app-utils',
    './submodules/c_deps/submodules/c_darwin/process',
  ),
  './submodules/c_deps/submodules/djbhash',
  './submodules/c_deps/submodules/djbhash/src',
  include_directories('./mui','./mui-render'),
  './mui-atlas',
  './mui-render',
  './mui',
  './',
  './submodules',
  './submodules/c_deps/submodules/c_ansi',
  './submodules/c_deps/submodules/c_ansi/ansi-codes',
  './submodules/c_deps/submodules/c_darwin',
  './submodules/c_deps/submodules/c_darwin/active-app',
  './submodules/c_deps',
  './submodules/c_deps/meson',
  include_directories(
    './submodules',
    './submodules/c_deps',
    './submodules/c_deps/submodules',
    './submodules/c_deps/submodules/c_ansi',
  ),
  include_directories('.'),
]
subdir('meson')
pngs = [
  ]
fonts = [
  'sourcecodepro',
  'ibmplex',
  'nerd',
  'inconsolata',
  'devicons',
  'firacode',
  'retro',
  'froufrou',
  'fontawesome',
  'glasstty',
  ]
#//////////////////
foreach i : pngs
  configure_file(
    input: 'assets/'+i+'.png',
    output: i+'.png',
    copy: true,
  )
endforeach
foreach f : fonts
  configure_file(
    input: 'assets/'+f+'.ttf',
    output: f+'.ttf',
    copy: true,
  )
endforeach
configure_file(
  input: 'assets/window_icon.bmp',  
  output: 'window_icon.bmp',   
  copy: true,
)
#//////////////////
subdir('submodules')
#//////////////////
#//////////////////
#//////////////////
#subdir('mui-meson-repos')
#//////////////////
#subdir('mui-colors')
#subdir('mui-colors-test')
subdir('mui')
subdir('mui-atlas')
subdir('mui-render')
subdir('mui-rectangle')
subdir('mui-basic')
subdir('mui-windows')
#//////////////////
subdir('mui-test')
subdir('mui-basic-test')
subdir('mui-windows-test')
subdir('mui-rectangle-test')
#//////////////////
