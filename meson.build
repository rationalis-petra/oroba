
project('oroba', 'cpp')

sfmldep = dependency('sfml-all')
opengldep = dependency('opengl')

incdir = include_directories('include')

sources = []
subdir('src')   # library is built here

executable('oroba', sources: sources,
                    dependencies : [sfmldep, opengldep],
                    include_directories : incdir)
