#ifndef	__EF_UTILITY_H__
#define __EF_UTILITY_H__

#include "ef_btype.h"
#include <sys/time.h>
#include <time.h>
#include <string>
#include <vector>

namespace ef{

	int64 gettime_ms();

	int32 tv_cmp(struct timeval t1, struct timeval t2);
	struct timeval tv_diff(struct timeval t1, struct timeval t2);

	int split(const std::string& str, std::vector<std::string>& ret_, 
		std::string sep = ",");

	std::string itostr(int64 i);

	int32 sleep_ms (int32 msec);
};

#endif/*EF_UTILITY_H*/

