/* This file is part of the Spring engine (GPL v2 or later), see LICENSE.html */

#include "GameParticipant.h"

#include "Sim/Misc/GlobalConstants.h"
#include "System/Net/Connection.h"
#include "System/BaseNetProtocol.h"

GameParticipant::GameParticipant()
: myState(UNCONNECTED)
, lastFrameResponse(0)
, speedControl(0)
, luaLockTime(0)
, isLocal(false)
, isReconn(false)
, isMidgameJoin(false)
{
	linkData[MAX_AIS] = PlayerLinkData(false);
}

void GameParticipant::SendData(boost::shared_ptr<const netcode::RawPacket> packet)
{
	if (link)
		link->SendData(packet);
}

void GameParticipant::Connected(boost::shared_ptr<netcode::CConnection> _link, bool local)
{
	link = _link;
	linkData[MAX_AIS].link.reset(new netcode::CLoopbackConnection());
	isLocal = local;
	myState = CONNECTED;
}

void GameParticipant::Kill(const std::string& reason)
{
	if (link)
	{
		link->SendData(CBaseNetProtocol::Get().SendQuit(reason));
		link->Close();
		link.reset();
	}
	linkData[MAX_AIS].link.reset();
#ifdef SYNCCHECK
	syncResponse.clear();
#endif
	myState = DISCONNECTED;
}

