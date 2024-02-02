IceConn     	_IceConnectionObjs[256];
char	    	*_IceConnectionStrings[256];
_IceProtocol 	_IceProtocols[255];
int     	_IceConnectionCount = 0;

int         	_IceLastMajorOpcode = 0;

int		_IceAuthCount = 1;
const char	*_IceAuthNames[] = {"MIT-MAGIC-COOKIE-1"};

_IceWatchProc	*_IceWatchProcs = NULL;
