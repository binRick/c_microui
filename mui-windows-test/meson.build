mui_windows_test_timeout_seconds = '.8'
mui_windows_test_srcs = [
  'mui-windows-test.c',
]
mui_windows_test_inc = [
 inc,
]
mui_windows_test_deps = [
 mui_windows_dep,
]

if get_option('enable-binaries')
  mui_windows_test_exec = executable('mui-windows-test',
     mui_windows_test_srcs,
     dependencies: mui_windows_test_deps,
     include_directories: mui_windows_test_inc,
     install : true, 
     install_dir : '/usr/local/bin',
  )
  if get_option('enable-binary-tests')
   if get_option('enable-mui-windows-test')
    if get_option('timeout-test')
      test('mui-windows-test', timeout_bin,
        args: [
          '--preserve-status',
          get_option('timeout').to_string(),
          mui_windows_test_exec,
        ]
      )
    else
      test('mui-windows-test', mui_windows_test_exec,
        args: [
        ]
      )
    endif
   endif
  endif
endif
