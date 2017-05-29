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

#define DEBUG(s) if(debugon && debuglevel == debug_c) printf("Debug@[%s:%d] %s\n", __FILE__, __LINE__,s)
#define DEBUG2(s,d) if(debugon && debuglevel == debug_c) printf("Debug@[%s:%d] %s %d\n", __FILE__, __LINE__,s, d)
#define INFO(s) if(debugon && debuglevel <= info_c) printf("Info@[%s:%d] %s\n", __FILE__, __LINE__,s)
#define WARN(s) if(debugon && debuglevel <= warn_c) printf("Warn@[%s:%d] %s\n", __FILE__, __LINE__,s)
#define ERROR(s) if(debugon && debuglevel <= error_c) printf("Error@[%s:%d] %s\n", __FILE__, __LINE__,s)

#define TRACE() if(traceon) printf("Trace@[%s:%d:%s]\n", __FILE__, __LINE__, __PRETTY_FUNCTION__)


#endif /* PONG_DEBUG_H_ */
