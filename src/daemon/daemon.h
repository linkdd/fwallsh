#ifndef __DAEMON_H
#define __DAEMON_H

#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <syslog.h>
#include <string.h>
#include <errno.h>

/*!
 * \brief Daemonize the main process.
 * This function does some initializations :
 *  - Fork the main process ;
 *  - Change file mode mask ;
 *  - Open logger (with syslog) ;
 *  - Create a new SID ;
 *  - Change the current working directory to / ;
 *  - Close standard file descriptors (stdin, stdout, stderr).
 *
 * @return -1 on error, 0 on success, 1 if the current process is the parent.
 */
int fwallsh_daemonize (void);

#endif /* __DAEMON_H */
