
project('oroba', 'cpp')

gtkdep = dependency('gtkmm-4.0')
incdir = include_directories('include')

sources = []
subdir('src')   # library is built here

executable('oroba', sources: sources,
                    dependencies: gtkdep,
                    include_directories : incdir)
