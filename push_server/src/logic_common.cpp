#include "logic_common.h"
#include "base/ef_utility.h" 
#include "base/ef_atomic.h"
#include "base/ef_base64.h"
#include "net/ef_sock.h"
#include "msg_head.h"
#include "gpush.pb.h"
#include <stdlib.h>
#include <sstream>

namespace gim{

	int32 constructPacket(const head& h, 
		const std::string& body, std::string& respbuf){
		head rh;
		respbuf.reserve(sizeof(h) + body.size());
		rh.cmd = htonl(h.cmd);
		rh.magic = htonl(h.magic); 		
		rh.len = htonl(sizeof(h) + body.size());
		respbuf.append((char*)&rh, sizeof(rh));
		respbuf.append(body);
		return 0;
	}


	int32 decorationName(int32 svid, int32 conid,
		const std::string& n, std::string& dn){
		
		std::string buf;
		buf.resize(8);

		char* p = (char*)buf.data();
		
		*(int32*)p = svid;
		*(int32*)(p + sizeof(int32)) = conid;

		dn = base64Encode(buf) + n;
		return 0;
	}

	int32 getDecorationInfo(const std::string& dn, int32& svid,
		int32& conid, std::string& n){

		if(dn.size() < 12){
			return -1;
		}

		std::string b64buf = dn.substr(0, 12);

		std::string buf = base64Decode(b64buf); 

		char* p = (char*)buf.data();
		svid = *(int32*)p;
		conid = *(int32*)(p + sizeof(int32));

		n = dn.substr(12);		

		return 0;
	}	

	int32 getSvTypeFromSessid(const std::string& ssid, 
		int32& svtype){
		int32 ret = 0;
		int32 svid;
		int32 conid;
		std::string vid;
		ret = getDecorationInfo(ssid, svid, conid, vid);

		if(ret < 0){
			return ret;
		}

		size_t pos = vid.find("__sv_"); 

		if(pos != 0){
			return -1;
		}

		svtype = atoi(&vid[pos + 5]);

		return 0;

	}

};
