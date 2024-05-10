# Main build script for BTS7960 test framework.
#
# Only the driver instances that can be unit-tested should be built here.
# I assume that real instances may require external libraries for their HAL
# implementations, that this project will not provide.

project(
  'bts7960',
  'c',
  'cpp',
  version: '1.0',
  license: 'MIT',
  license_files: 'LICENSE',
  default_options: {
    'warning_level': '3',
    'cpp_std': 'c++17',
    'c_std': 'c11',
    'default_library': 'static',
  },
)

bts7960_sources = files('./bts7960/bts7960.c')
bts7960_includes = include_directories('./bts7960/')
bts7960_hals = {
  'mock': {
    'sources': ['./bts7960/hal/mock.c'],
  },
  'mock_freq': {
    'sources': ['./bts7960/hal/mock.c'],
    'defines': ['BTS7960_ENABLE_FREQUENCY_CONTROL'],
  },
  'mock_no_asserts': {
    'sources': ['./bts7960/hal/mock.c'],
    'defines': ['BTS7960_DISABLE_ASSERTS'],
  },
  'mock_freq_no_asserts': {
    'sources': ['./bts7960/hal/mock.c'],
    'defines': ['BTS7960_DISABLE_ASSERTS', 'BTS7960_ENABLE_FREQUENCY_CONTROL'],
  },
}

bts7960_instances = {}
foreach hal_name, hal_props : bts7960_hals
  hal_sources = hal_props['sources']
  c_cpp_args = []
  hal_defines = []

  if 'defines' in hal_props
    hal_defines = hal_props['defines']
    foreach define : hal_defines
      c_cpp_args += ['-D' + define]
    endforeach
  endif

  bts7960_instances += {
    hal_name: {
      'library': library(
        f'bts7960_@hal_name@',
        bts7960_sources + hal_sources,
        include_directories: bts7960_includes,
        c_args: c_cpp_args,
        cpp_args: c_cpp_args,
      ),
      'defines': hal_defines,
    }
  }
endforeach

subdir('tests')
