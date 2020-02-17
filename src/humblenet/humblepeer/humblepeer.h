#ifndef HUMBLEPEER_H
#define HUMBLEPEER_H

// HumbleNet internal, do not include

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <map>

#include "humblenet.h"
#include "humblepeer_generated.h"

#ifdef __GNUC__
#define WARN_UNUSED_RESULT __attribute__ ((warn_unused_result))
#else
#define WARN_UNUSED_RESULT
#endif

namespace humblenet {

	// Ice Server definition
	struct ICEServer {
		HumblePeer::ICEServerType type;
		std::string server;
		std::string username;
		std::string password;

		ICEServer(const std::string& _server)
		: type(HumblePeer::ICEServerType::STUNServer), server(_server) {}
		ICEServer(const std::string& _server, const std::string& _username, const std::string& _password)
		: type(HumblePeer::ICEServerType::TURNServer), server(_server), username(_username), password(_password) {}
	};

	// abstract type for P2P signaling connection
	// we could use C++ classes and inheritance
	// but since there's only ever one type of connection in any binary
	// we opt for static dispatch instead
	struct P2PSignalConnection;
	ha_bool sendP2PMessage(P2PSignalConnection *conn, const uint8_t *buff, size_t length) WARN_UNUSED_RESULT;

	/*
	  P2POffer contains
		PeerID
		  client->server who want to connect to
		  server->client who wants to connect to us
		connectionOffer generated by the browser (stringified JSON)


	   P2PAnswer contains
		PeerID
		connectionOffer generated by the other browser (also str JSON)


	   HelloServer
		auth token which allows server to look up client details in database


	   HelloClient
		PeerID  this is who you are
		IP address?


	  https://developer.mozilla.org/en-US/docs/Web/API/WebRTC_API/Architecture/Connectivity


	  peer1                        server                        peer2

			 P2POffer ->
											   P2POffer ->

											  <- P2PAnswer
			<- P2PAnswer
			 ICECandidate ->
												ICECandidate ->
											 <- ICECandidate
			<- ICECandidate
	*/


	typedef ha_bool(*ProcessMsgFunc) (const HumblePeer::Message* msg, void *data);

	/*
	 returns true if all went wel
			 false if something is wrong, sync is lost and channel should be closed
			 TODO: should this be a tristate (process, noprocess, error)?
			 does anyone care if progress happened or not?
	 */
	ha_bool parseMessage(std::vector<uint8_t> &recvBuf, ProcessMsgFunc processFunc, void *user_data);

	// Peer server connection
	ha_bool sendHelloServer(humblenet::P2PSignalConnection *conn, uint8_t flags,
							const std::string& gametoken, const std::string& gamesecret,
							const std::string& authToken, const std::string& reconnectToken,
							const std::map<std::string, std::string>& attributes);
	ha_bool sendHelloClient(humblenet::P2PSignalConnection *conn, PeerId peerId,
							const std::string& reconnectToken,
							const std::vector<ICEServer>& iceServers);

	// Generic messages
	ha_bool sendError(P2PSignalConnection *conn, ha_requestId requestId, const std::string& error, HumblePeer::MessageType errorType);
	ha_bool sendSuccess(P2PSignalConnection *conn, ha_requestId requestId, HumblePeer::MessageType successType);

	// P2P Handling
	ha_bool sendNoSuchPeer(humblenet::P2PSignalConnection *conn, PeerId peerId);
	ha_bool sendPeerRefused(humblenet::P2PSignalConnection *conn, PeerId peerId);
	ha_bool sendP2PConnect(P2PSignalConnection *conn, PeerId peerId, uint8_t flags, const char* offer);
	ha_bool sendP2PResponse(P2PSignalConnection *conn, PeerId peerId, const char* offer);
	ha_bool sendICECandidate(humblenet::P2PSignalConnection *conn, PeerId peerId, const char* offer);
	ha_bool sendP2PDisconnect(humblenet::P2PSignalConnection *conn, PeerId peer);
	ha_bool sendP2PRelayData(humblenet::P2PSignalConnection *conn, PeerId peer, const void* data, uint16_t length);

	// Name Alias
	ha_requestId sendAliasRegister(P2PSignalConnection *conn, const std::string& alias);
	ha_requestId sendAliasUnregister(P2PSignalConnection *conn, const std::string& alias);
	ha_requestId sendAliasLookup(P2PSignalConnection *conn, const std::string& alias);
	ha_bool sendAliasResolved(P2PSignalConnection *conn, const std::string& alias, PeerId peer, ha_requestId requestId);

}  // namespace humblenet


#endif  // HUMBLEPEER_H
