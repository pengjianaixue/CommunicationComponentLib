#pragma once
#ifdef DYNAMICLIBEXPORT
#define DYNAMICLIBEXPORT   __declspec(dllexport)
#else
#define DYNAMICLIBEXPORT  _declspec (dllimport)
#endif
