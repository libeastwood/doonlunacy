import sys

env = Environment()
env.ParseConfig('sdl-config --cflags --libs')
env.ParseConfig('pkg-config --cflags --libs zziplib')
env.Append(CPPPATH="#include")

env.Append(CCFLAGS=["-Wall", "-Werror"])

if 1:
  env.Append(CCFLAGS=["-ggdb"])

env.Append(LIBS = [ "SDL_mixer", 
                    "SDL_image",
                    "SDL_net",
                    "SDL_ttf",
                    "SDL_gfx",
                  ])

Export('env')

SConscript("src/SConscript")
