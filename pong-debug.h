/*
 * pong-debug.h
 *
 *  Created on: May 29, 2017
 *      Author: dwlambiri
 */

#ifndef PONG_DEBUG_H_
#define PONG_DEBUG_H_

enum DEBUGENUM { debug_c =0, info_c = 1, warn_c = 2, error_c = 3};
extern bool debugon;
extern bool traceon;
extern int  debuglevel;

char* printDebugTime();

#ifdef DARIUS
#define DEBUG(s) if(debugon && debuglevel == debug_c) printf("%s Debug@[%s:%d] %s\n", printDebugTime(),__FILE__, __LINE__,s)
#define DEBUG2(s,d) if(debugon && debuglevel == debug_c) printf("%s Debug@[%s:%d] %s %d\n", printDebugTime(), __FILE__, __LINE__,s, d)
#define INFO(s) if(debugon && debuglevel <= info_c) printf("%s Info@[%s:%d] %s\n", printDebugTime(), __FILE__, __LINE__,s)
#define WARN(s) if(debugon && debuglevel <= warn_c) printf("%s Warn@[%s:%d] %s\n", printDebugTime(), __FILE__, __LINE__,s)
#define ERROR(s) if(debugon && debuglevel <= error_c) printf("%s Error@[%s:%d] %s\n", printDebugTime(), __FILE__, __LINE__,s)

#define TRACE() if(traceon) printf("%s Trace@[%s:%d:%s]\n", printDebugTime(), __FILE__, __LINE__, __PRETTY_FUNCTION__)
#else
#define DEBUG(s)
#define DEBUG2(s,d)
#define INFO(s)
#define WARN(s)
#define ERROR(s)
#define TRACE()
#endif

#endif /* PONG_DEBUG_H_ */
