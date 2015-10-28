#ifndef __REDIS_CG_H__
#define __REDIS_CG_H__

#include "redis_client.h"
#include "json/json.h"

namespace gim {

class RedisGroup {
public:
	RedisGroup(const Json::Value &config);
	~RedisGroup();
	int clear();
	DBHandle getHndl(const string &key);
	void setCmdLog(LogCb cb);
private:
	int init(const Json::Value &config);

	vector <DBHandle> m_dbs;
	Json::Value m_cfg;
	LogCb m_cmdLog;
};

};
#endif
