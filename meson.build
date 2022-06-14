project('c_microui',['c'],default_options:['warning_level=0','c_std=gnu99'])
cc = meson.get_compiler('c')
meson_deps_dir = meson.current_source_dir()
timeout_bin = find_program('timeout', required : true)
sdl2_dep = dependency('sdl2',fallback:['sdl2', 'sdl2'], required: true)
sdl2_cflags = run_command(find_program('sdl2-config', required : true),['--libs'],check: true,).stdout().strip()
#############################################################
mui_global_linkflags = [
  '-Wl,-framework,OpenGL',
]
inc = [
  include_directories('.'),
  include_directories('./mui'),
  include_directories('./mui-render'),
  include_directories('./mui-atlas'),
]
mui_global_cflags = [
  sdl2_cflags,
]
#//////////////////
add_project_arguments(mui_global_cflags, language: 'c')
add_project_link_arguments(mui_global_linkflags, language : ['c'])
#//////////////////
subdir('mui')
subdir('mui-atlas')
subdir('mui-render')
#//////////////////
subdir('mui-test')
#//////////////////
