import sys
import os
opts = Options('config.py')

# default to using sdl-config if not on windows
opts.Add('SDL_INCLUDE_PATH', 'include path for SDL', '')
opts.Add('SDL_LIB_PATH', 'lib path for SDL', '')
opts.Add('SDLNET_INCLUDE_PATH', 'include path for sdl_net', '')
opts.Add('SDLNET_LIB_PATH', 'lib path for sdl_net', '')
opts.Add('SDLMIXER_INCLUDE_PATH', 'include path for sdl_mixer', '')
opts.Add('SDLMIXER_LIB_PATH', 'lib path for sdl_mixer', '')
opts.Add('BOOST_INCLUDE_PATH', 'include path for boost', '')
opts.Add('BOOST_LIB_PATH', 'lib path for boost', '')
opts.Add('SAMPLERATE_LIB_PATH', 'include path for boost', '')
opts.Add('SAMPLERATE_INCLUDE_PATH', 'include path for boost', '')

if sys.platform == 'win32':
    env = Environment(options = opts, ENV=os.environ)
else:
    env = Environment(options = opts)

env.Append(CPPPATH="#include")


if sys.platform != "win32":
    env.ParseConfig('pkg-config --cflags sdl libeastwood')
    env.Append(CCFLAGS=["-Wall", "-pedantic", "-O0", "-Wno-long-long"]) #, "-Werror"])
    #env.Append(CCFLAGS=["-Wall", "-Werror", "-O2", "-ffast-math", "-funroll-loops"])
    #env.Append(LINKFLAGS = ["-ffast-math"])
    env.Append(LIBS = [ "boost_signals-mt",
                        "boost_filesystem-mt",
                        ])
    
    env.Append(CCFLAGS=["-ggdb"])

    if os.path.exists('/usr/lib/ccache/bin'):
        os.environ['PATH']         = '/usr/lib/ccache/bin:' + os.environ['PATH']
        env['ENV']['CCACHE_DIR']   = os.environ['CCACHE_DIR']
  
        env['ENV']['PATH'] = os.environ['PATH']
        env['ENV']['HOME'] = os.environ['HOME']
 	
else:
    env.Append(LIBS = ["SDLmain"])
    env.Append(LINKFLAGS = ["/SUBSYSTEM:CONSOLE", "/DEBUG"])
    env.Append(CCFLAGS = ["/O2", "/EHsc", "/MD", "/DEBUG", "/Zi", "/GR"])

    env.Append(CPPPATH = [ "${SDL_INCLUDE_PATH}",
        "${SDLNET_INCLUDE_PATH}",
        "${SDLMIXER_INCLUDE_PATH}",
        "${SDLTTF_INCLUDE_PATH}",
        "${BOOST_INCLUDE_PATH}",
        "${SAMPLERATE_INCLUDE_PATH}"])
      
    env.Append(LIBPATH = [ "${SDL_LIB_PATH}",
        "${SDLNET_LIB_PATH}",
        "${SDLMIXER_LIB_PATH}",
        "${SDLTTF_LIB_PATH}",
        "${BOOST_LIB_PATH}",
        "${SAMPLERATE_LIB_PATH}"])

env.Append(LIBS = [ "SDL",
                        "SDL_mixer",
                        "SDL_net",
			"libeastwood",
                        ])



#env.Append(CCFLAGS=["-DLOG_DISABLED"])

Export('env')

SConscript("src/SConscript")
