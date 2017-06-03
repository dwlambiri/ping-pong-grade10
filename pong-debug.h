/*
 * pong-debug.h
 *
 *  Created on: May 29, 2017
 *      Author: dwlambiri
 */

#ifndef PONG_DEBUG_H_
#define PONG_DEBUG_H_

/*
 * @author   dwlambiri
 * @date     Jun 2, 2017
 *  I needed a way to test my code while running.
 *  If PONGDEBUG is defined the following defines insert code that print various messages.
 *  If PONGDEBUG is not defined the macros are replaced with NULL strings.
 *  The defines contain special macros to print file names, function names, and code lines.
 *
 */


enum DEBUGENUM { debug_c =0, info_c = 1, warn_c = 2, error_c = 3};
extern bool debugon;
extern bool traceon;
extern int  debuglevel;

char* printDebugTime();
void  functionEnter();
void  functionExit();
char* printCallDepth();

//#define PONGDEBUG

#ifdef PONGDEBUG
#define DEBUG(s) if(debugon && debuglevel == debug_c) printf("Debug[%s %s:%4d] %s\n", printDebugTime(),__FILE__, __LINE__,s)
#define DEBUG2(s,d) if(debugon && debuglevel == debug_c) printf("Debug[%s %s:%4d] %s %d\n", printDebugTime(), __FILE__, __LINE__,s, d)
#define INFO(s) if(debugon && debuglevel <= info_c) printf("Info[%s %s:%4d] %s\n", printDebugTime(), __FILE__, __LINE__,s)
#define WARN(s) if(debugon && debuglevel <= warn_c) printf("Warn[%s %s:%4d] %s\n", printDebugTime(), __FILE__, __LINE__,s)
#define ERROR(s) if(debugon && debuglevel <= error_c) printf("Error[%s %s:%4d] %s\n", printDebugTime(), __FILE__, __LINE__,s)
#define FENTRY() functionEnter()
#define FEXIT()   functionExit()

#define TRACE() if(traceon) printf("Trace<%s: %s:%4d> %s%s;\n", printDebugTime(), __FILE__, __LINE__, printCallDepth(), __PRETTY_FUNCTION__)
#else
#define DEBUG(s)
#define DEBUG2(s,d)
#define INFO(s)
#define WARN(s)
#define ERROR(s)
#define TRACE()
#define FENTRY()
#define FEXIT()
#endif

#endif /* PONG_DEBUG_H_ */
