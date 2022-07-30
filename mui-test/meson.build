mui_test_timeout_seconds = '.8'
mui_test_srcs = [
  'mui-test.c',
]
mui_test_inc = [
 inc,
 '../mui-render',
 '../mui',
 '../mui-atlas',
 '../',
]
mui_test_deps = [
 sdl2_dep,
 mui_dep,
 mui_render_dep,
]

if get_option('enable-binaries')
  mui_test_exec = executable('mui-test',
     mui_test_srcs,
     dependencies: mui_test_deps,
     include_directories: mui_test_inc,
     install : true, 
     #     link_args: [osx_ldflags],
     install_dir : '/usr/local/bin',
  )
  if get_option('enable-binary-tests')
   if get_option('enable-mui-test')
    if get_option('timeout-test')
      test('mui-test', timeout_bin,
        args: [
          '--preserve-status',
          get_option('timeout').to_string(),
          mui_test_exec,
        ]
      )
    else
      test('mui-test', mui_test_exec,
        args: [
        ]
      )
    endif
   endif
  endif
endif
