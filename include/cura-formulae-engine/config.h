#pragma once

#ifdef _WIN32
#    ifdef cura_formulae_engine_EXPORTS
#        define CURA_FORMULAE_ENGINE_API __declspec(dllexport)
#    else
#        define CURA_FORMULAE_ENGINE_API __declspec(dllimport)
#    endif
#else
#    define CURA_FORMULAE_ENGINE_API
#endif
