#pragma once

void assert_failed(const char* condition);

#define assert(condition) { if(!(condition)) assert_failed(#condition); }
