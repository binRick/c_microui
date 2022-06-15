mui_test_timeout_seconds = '.8'
mui_test_srcs = [
  'mui-test.c',
]
mui_test_inc = [
 inc,
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
     install_dir : '/usr/local/bin',
  )
  if get_option('enable-binary-tests')
    test('mui-test', timeout_bin,
      args: [
        '--preserve-status',
        get_option('timeout').to_string(),
        mui_test_exec,
      ]
    )
  endif
endif
