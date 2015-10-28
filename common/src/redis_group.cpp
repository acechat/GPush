#include "redis_group.h"
#include "base/ef_md5.h"
#include "base/ef_btype.h"
#include "base/ef_utility.h"
#include <stdlib.h>
#include <sstream>
#include <iostream>

namespace gim {

using namespace ef;

RedisGroup::RedisGroup(const Json::Value &config)
{
	init(config);
	m_cmdLog = NULL;
}

RedisGroup::~RedisGroup(){
	clear();
}

void RedisGroup::setCmdLog(LogCb cb)
{
	m_cmdLog = cb;
}

int RedisGroup::init(const Json::Value &config)
{
	m_cfg = config;
	
	std::cout << "redisCG init /n" <<  config.toStyledString() << endl;
	Json::Value urlArray = m_cfg["NodeCfgs"];
	m_dbs.clear();
	m_dbs.resize(urlArray.size());
	for(size_t i = 0; i < m_dbs.size(); ++i){
		m_dbs[i] = NULL;
	}
	
	return 0;	
}

int RedisGroup::clear()
{
	for(size_t i = 0; i < m_dbs.size(); ++i){
		if(m_dbs[i])
			delete  m_dbs[i];
	}

	return 0;
}

DBHandle RedisGroup::getHndl(const string &key)
{
	ef::uint8 md5[16];
    ef::MD5_CTX c;

    ef::MD5Init(&c);
    ef::MD5Update(&c, (ef::uint8 *)key.data(), key.size());
    ef::MD5Final(md5, &c);
    //count the md5
    ef::uint64 i = *(ef::uint64*)md5;
    ef::uint64 idx = i % m_dbs.size();

	if(!m_dbs[idx]) {
		Json::Value url = (m_cfg["NodeCfgs"])[(int)idx];
		m_dbs[idx] = connectCache(url["IPAddr"].asString(), 
			url["Port"].asInt(), url["Passwd"].asString());
		if (!m_dbs[idx]) {
			if (m_cmdLog) {
				stringstream ss;
				ss << "connect " << url["IPAddr"].asString() << ":" << 
					url["Port"].asInt() << ":" << url["Passwd"].asString() << " failed";
                m_cmdLog(ss.str());
            }
			return NULL;
		}
		if (m_cmdLog) {
			stringstream ss;
            ss << "connect " << url["IPAddr"].asString() << ":" << 
				url["Port"].asInt() << ":" << url["Passwd"].asString() << " success";
			m_cmdLog(ss.str());
			m_dbs[idx]->setCmdLog(m_cmdLog);
		}
	} 
	
	return  m_dbs[idx];
}

};
