#ifndef __REDIS_CLIENT_H__
#define __REDIS_CLIENT_H__

#include <string>
#include <vector>
#include <map>
#include <float.h>
#include "hiredis/hiredis.h"
#include "base/ef_btype.h"

namespace gim{

using namespace std;
using namespace ef;

enum{
	CONNECT_CACHE_FAILED = -100000,
	CACHE_NOT_EXIST = -1000000,
	INVALID_ARGUMENT = -1000010,
	SERVER_ERROR = -1000020
};

typedef void (*LogCb)(const std::string &cmd);


class RedisCli
{

class Replyer
{
public:
	Replyer(redisReply *reply = NULL):m_reply(reply){};
	~Replyer(){
		if (m_reply) {
			freeReplyObject(m_reply);
			m_reply = NULL;
		}
	}
	redisReply *const reply(){return m_reply;};
	void setReply(redisReply *srcrep)
	{
		if(m_reply) freeReplyObject(m_reply);
		m_reply = srcrep;
	}
private:
	redisReply *m_reply;
};

public: 
	RedisCli():m_c(NULL),m_cmdLog(NULL),m_retry(3){}
	~RedisCli(){
		disconnect();
	}
	int connect(const string& addr, int port, const string &passWd);
	void setCmdLog(LogCb cb){m_cmdLog = cb;};
	void addLog(const string &data);
	int reconnect();
	
	/* key related */
	int keyDel(const string &key);
	int keyExists(const string &key, int &exists);
	int keyExpire(const string &key, int seconds);

	int strGet(const string &key, string &value);
	int strGetSet(const string &key, const string &value, string &oldValue);
	int strIncr(const string &key, int64 &afterIncr);
	int strIncrBy(const string &key, int64 increment, int64 &afterIncr);
	int strSet(const string &key, const string &value);

	int hashDel(const string &key, const string &field);
  	int hashMDel(const string &key, const vector<string> &fields);
	int hashExists(const string &key, const string &field, int &exists);
	int hashGet(const string &key, const string &field, string &value);
	int hashGetAll(const string &key, map<string, string> &mfv);
	int hashMSet(const string &key, const map<string, string> &mfv);
	int hashSet(const string &key, const string &field, const string &value);

	int setAdd(const string &key, const vector<string> &members);
	int setIsMember(const string &key, const string &member, int& isMember);
	int setMembers(const string &key, vector <string> &members);
	int setRem(const string &key, const vector<string> &members);

	int ssetAdd(const string &key, const map<string, string> &members);
	int ssetCard(const string &key, int64 &card);
	int ssetCount(const string &key, double minScore, double maxScore, int64 &count);
	int ssetRange(const string &key, int start, int stop, vector<string> &members);
	int ssetRangeByScoreLimit(const string &key, double minScore, double maxScore,
		int offset, int length, vector<string> &members);
	int ssetRangeWithScore(const string &key, int start, int stop, 
		vector<pair<string, string> > &members);
	int ssetRangeByScoreWithScore(const string &key, double minScore, double maxScore, 
		vector<pair<string, string> > &members);
	int ssetRangeByScoreWithScoreLimit(const string &key, double minScore, double maxScore,
		int offset, int length, vector<pair<string, string> >&members);
	int ssetRemRange(const string &key, int start, int stop);
	int ssetRemRangeByScore(const string &key, double minScore, double maxScore);
	int ssetRevRange(const string &key, int start, int stop, vector<string> &members);
  	int ssetRevRangeByScoreLimit(const string &key, double maxScore, double minScore,
  		int offset, int length, vector<string> &members);
  	int ssetRevRangeByScoreWithScore(const string &key, double maxScore, 
		double minScore, vector<pair<string, string> > &members);
  	int ssetRevRangeByScoreWithScoreLimit(const string &key, double maxScore, 
		double minScore, int offset, int length, vector<pair<string, string> > &members);

	int connAuth(const string &passWord);
	int connPing();
	int connQuit();
	int servBGSave();
	int servCfgGet(const string &parameter, vector<string> &results);
	int servCfgSet(const string &parameter, const string &value);
	int servDBSize();
	int servSlaveOf(const string &masterip, int masterport);
	int servSlaveOfNoOne();

	int transDiscard();
	int transExec();
	int transMulti();
	int transWatch(const string &key);
	int transUnwatch();
	
#ifdef ENABLE_UNSTABLE_INTERFACE
	/* key related */
	int keyDump(const string &key, string &value);	
	int keyExpireAt(const string &key, int64 timeStamp);
	int keyKeys(const string &pattern, vector<string> &keys);
  	int keyMigrate(const string &host, int port, const string &key, 
  		int destDB, int timeout);
  	int keyMove(const string &key, int destDB);
  	int keyObject(const string &subCmd, const string &key, vector<string> &result);
  	int keyPersist(const string &key);
  	int keyPExpire(const string &key, int64 milliSeconds);
  	int keyPExpireAt(const string &key, int64 milliTimeStamp);
  	int keyPTTL(const string &key, int64 &milliTTL);
  	int keyRandomKey(string &key);
  	int keyRename(const string &key, const string &newKey);
  	int keyRenameNx(const string &key, const string &newKey);
  	int keyRestore(const string &key, int ttl, const string &value);
  	int keyTTL(const string &key, int &ttl);
  	int keyType(const string &key, string &type);

	/* string related */
  	int strAppend(const string &key, const string &value);
  	int strBitCount(const string &key, int start, int end);
  	int strBitOp(const string &op, const string &destKey, const vector<string> &keys);
  	int strDecr(const string &key, int64 &afterDecr);
  	int strDecrBy(const string &key, int64 decrement, int64 &afterDecr);
  	int strGetBit(const string &key, int &bit);
  	int strGetRange(const string &key, int start, int end, string &result);
  	int strIncrByFloat(const string &key, float increment, float &afterIncr);
  	int strMGet(const vector<string> &keys, vector<string> &values);
  	int strMSet(const map<string, string> &kvs);
  	int strMSetNx(const map<string, string> &kvs);
  	int strPSetEx(const string &key, int64 milliSeconds, const string &value);
  	int strSetBit(const string &key, int offset, const bool value);
  	int strSetEx(const string &key, int seconds, const string &value);
  	int strSetNx(const string &key, const string &value);
  	int strSetRange(const string &key, int offset, const string &value);
  	int strLen(const string &key, int &len);

	/* hashtable related */
  	int hashIncrBy(const string &key, const string &field, 
		int64 increment, int64 &afterIncr);
  	int hashIncrByFloat(const string &key, const string &field, 
		float increment, float &afterIncr);
  	int hashKeys(const string &key, vector<string> &fields);
  	int hashLen(const string &key, int &len);
  	int hashMGet(const string &key, const vector<string> &fields, vector<string> &values);
  	int hashSetNx(const string &key, const string &field, const string &value);
  	int hashVals(const string &key, vector<string> &values);

	/* list related */
  	int listBLPop(const vector<string> &keys, int timeout, map<string, string> &mlv);
  	int listBRPop(const vector<string> &keys, int timeout, map<string, string> &mlv);
  	int listBRPopLPush(const string &srcList, const string &destList, 
  						int timeout, vector<string> &result);
  	int listIndex(const string &key, int index, string &elmt);
  	int listInsert(const string &key, const string &position, const string &pivot, 
  					const string &value);
  	int listLen(const string &key);
  	int listLPop(const string &key, string &head);
  	int listLPush(const string &key, const vector<string> &elmts);
  	int listLPushx(const string &key, const string &elmt);
  	int listRange(const string &key, int start, int stop, vector<string> &elmts);
  	int listRem(const string &key, int count, const string &value);
  	int listRPop(const string &key, string &tail);
  	int listRPopLPush(const string &srcList, const string &destList, string &value);
  	int listRPush(const string &key, const vector<string> &elmts);
  	int listRPushx(const string &key, const string &elmt);
  	int listSet(const string &key, int index, const string &value);
  	int listTrim(const string &key, int start, int stop);

	/* set related */
  	int setCard(const string &key, int64 &card);
  	int setDiff(const vector<string> &keys, vector<string> &members);
  	int setDiffStore(const string &destKey, const vector<string> &keys);
  	int setInter(const vector<string> &keys, vector<string> &members);
  	int setInterStore(const string &destKey, const vector<string> &keys);
  	int setMove(const string &srcKey, const string &destKey, const string &member);
  	int setPop(const string &key, string &member);
  	int setRandMember(const string &key, int count, vector<string> &members);
  	int setUnion(const vector<string> &keys, vector<string> &members);
  	int setUnionStore(const string &destKey, const vector<string> &keys);

	/* sorted set related */
	
  	int ssetIncrBy(const string &key, double increment, 
		const string &member, string &afterIncr);
  	int ssetRangeByScore(const string &key, double minScore, double maxScore, 
  		vector<string> &members);
  	int ssetRank(const string &key, const string &member, int64 &rank);
  	int ssetRem(const string &key, const vector<string> &members);
  	int ssetRevRangeWithScore(const string &key, int start, int stop, 
		vector<pair<string, string> > &members);
  	int ssetRevRangeByScore(const string &key, double maxScore, double minScore, 
  		vector<string> &members);
  	int ssetRevRank(const string &key, const string &member, int64 &rank);
  	int ssetScore(const string &key, const string &member, string &score);

	/* connection related */
  	int connEcho(const string &msg);
  	int connSelect(int idx);
	/* server related */
  	int servBGRewriteAof();
  	int servCliGetName(string &name);
  	int servCliKill(const string &ip, int port);
  	int servCliList(vector<string> &cliInfos);
  	int servCliSetName(const string &name);
  	int servCfgResetStat();
  	int servCfgRewrite();
  	int servFlushAll();
  	int servFlushDB();
  	int servLastSave(int &saveTime);
  	int servSave();
  	int servShutDown();
  	int servTime(vector<string> &ts);
#endif

private:
	int _exeCmd(Replyer &rpler, const char *fmt, va_list ap);
	int _exeCmd(Replyer &rpler, int argc, const char **argv, const size_t *argvLen);

	int _doExeCmdNoReconnect(Replyer &rpler, const char *fmt, va_list ap);
	int _doExeCmdNoReconnect(Replyer &rpler, int argc, const char **argv, const size_t *argvLen);

	int _exeCmdWithNoOutput(const string &cmd);
	int _exeCmdWithNoOutput(const char *fmt, ...);
	template <typename T> int _exeCmdWithNoOutputM(const string &cmdName, 
		const string &key, const T &input);

	template <typename T> int _exeCmdWithOutput(T &output, const string &cmd);
	template <typename T> int _exeCmdWithOutput(T &output, const char *fmt, ...);
	template <typename T, typename U> int _exeCmdWithOutputM(T &output, const string &cmdName,
		const string &key, const U&input);

	int disconnect();
	string	m_addr;
	int     m_port;
	string m_passWd;
	redisContext *m_c;
	LogCb m_cmdLog;
	int m_retry;
};

typedef RedisCli *DBHandle;

RedisCli *connectCache(const string &ip, int port, const string &passWd);

};

#endif /* __REDIS_CLIENT_H__ */
