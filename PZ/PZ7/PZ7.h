#pragma once

#ifdef PZ7_EXPORTS
#define PZ7_API __declspec(dllexport)
#else
#define PZ7_API __declspec(dllimport)
#endif
