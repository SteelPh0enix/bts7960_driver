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
bts7960_hals = {
  'mock': ['./bts7960/hal-impls/mock.c'] 
}

bts7960_instances = {}
foreach hal_name, hal_sources: bts7960_hals
  bts7960_instances += {
    hal_name: library(f'bts7960_@hal_name@',
                     bts7960_sources + hal_sources,
                     include_directories: ['./bts7960/'])
    }
endforeach
