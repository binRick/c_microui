mui_meson_repos_timeout_seconds = '.8'
mui_meson_repos_srcs = [
  'mui-meson-repos.c',
]
mui_meson_repos_inc = [
 inc,
]
mui_meson_repos_deps = [
 sdl2_dep,
 mui_dep,
 mui_render_dep,
]

if get_option('enable-binaries')
  mui_meson_repos_exec = executable('mui-meson-repos',
     mui_meson_repos_srcs,
     dependencies: mui_meson_repos_deps,
     include_directories: mui_meson_repos_inc,
     install : true, 
     install_dir : '/usr/local/bin',
  )
  if get_option('enable-binary-tests')
   if get_option('enable-mui-meson-repos')
    if get_option('timeout-test')
      test('mui-meson-repos', timeout_bin,
        args: [
          '--preserve-status',
          get_option('timeout').to_string(),
          mui_meson_repos_exec,
        ]
      )
    else
      test('mui-meson-repos', mui_meson_repos_exec,
        args: [
        ]
      )
    endif
   endif
  endif
endif
