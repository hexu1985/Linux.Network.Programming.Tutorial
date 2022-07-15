/* TCPmtechod.c - main, tcp_echo_server, prstats */

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>

#include <sys/types.h>
#include <sys/signal.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/wait.h>
#include <sys/errno.h>
#include <netinet/in.h>

#include "err_exit.h"
#include "tcp_passive.h"

#define	QLEN		  32	/* maximum connection queue length	*/
#define	BUFSIZE		4096

#define	INTERVAL	5	/* secs */

struct {
	pthread_mutex_t	st_mutex;
	unsigned int	st_concount;
	unsigned int	st_contotal;
	unsigned long	st_contime;
	unsigned long	st_bytecount;
} stats;

void prstats(void);
int	tcp_echo_server(int fd);

/*------------------------------------------------------------------------
 * main - Concurrent TCP server for ECHO service
 *------------------------------------------------------------------------
 */
int main(int argc, char *argv[])
{
	pthread_t	th;
	pthread_attr_t	ta;
	char	*service = "echo";	/* service name or port number	*/
	struct	sockaddr_in fsin;	/* the address of a client	*/
	unsigned int	alen;		/* length of client's address	*/
	int	msock;			/* master server socket		*/
	int	ssock;			/* slave server socket		*/

	switch (argc) {
	case	1:
		break;
	case	2:
		service = argv[1];
		break;
	default:
		err_exit("usage: tcp_echo_server [port]\n");
	}

	msock = tcp_passive(service, QLEN);

	(void) pthread_attr_init(&ta);
	(void) pthread_attr_setdetachstate(&ta, PTHREAD_CREATE_DETACHED);
	(void) pthread_mutex_init(&stats.st_mutex, 0);

	if (pthread_create(&th, &ta, (void * (*)(void *))prstats, 0) < 0)
		err_exit("pthread_create(prstats): %s\n", strerror(errno));

	while (1) {
		alen = sizeof(fsin);
		ssock = accept(msock, (struct sockaddr *)&fsin, &alen);
		if (ssock < 0) {
			if (errno == EINTR)
				continue;
			err_exit("accept: %s\n", strerror(errno));
		}
		if (pthread_create(&th, &ta, (void * (*)(void *))tcp_echo_server,
		    (void *)ssock) < 0)
			err_exit("pthread_create: %s\n", strerror(errno));
	}
}

/*------------------------------------------------------------------------
 * tcp_echo_server - echo data until end of file
 *------------------------------------------------------------------------
 */
int tcp_echo_server(int fd)
{
	time_t	start;
	char	buf[BUFSIZ];
	int	cc;

	start = time(0);
	(void) pthread_mutex_lock(&stats.st_mutex);
	stats.st_concount++;
	(void) pthread_mutex_unlock(&stats.st_mutex);
	while (cc = read(fd, buf, sizeof buf)) {
		if (cc < 0)
			err_exit("echo read: %s\n", strerror(errno));
		if (write(fd, buf, cc) < 0)
			err_exit("echo write: %s\n", strerror(errno));
		(void) pthread_mutex_lock(&stats.st_mutex);
		stats.st_bytecount += cc;
		(void) pthread_mutex_unlock(&stats.st_mutex);
	}
	(void) close(fd);
	(void) pthread_mutex_lock(&stats.st_mutex);
	stats.st_contime += time(0) - start;
	stats.st_concount--;
	stats.st_contotal++;
	(void) pthread_mutex_unlock(&stats.st_mutex);
	return 0;
}

/*------------------------------------------------------------------------
 * prstats - print server statistical data
 *------------------------------------------------------------------------
 */
void prstats(void)
{
	time_t	now;

	while (1) {
		(void) sleep(INTERVAL);

		(void) pthread_mutex_lock(&stats.st_mutex);
		now = time(0);
		(void) printf("--- %s", ctime(&now));
		(void) printf("%-32s: %u\n", "Current connections",
			stats.st_concount);
		(void) printf("%-32s: %u\n", "Completed connections",
			stats.st_contotal);
		if (stats.st_contotal) {
			(void) printf("%-32s: %.2f (secs)\n",
				"Average complete connection time",
				(float)stats.st_contime /
				(float)stats.st_contotal);
			(void) printf("%-32s: %.2f\n",
				"Average byte count",
				(float)stats.st_bytecount /
				(float)(stats.st_contotal +
				stats.st_concount));
		}
		(void) printf("%-32s: %lu\n\n", "Total byte count",
			stats.st_bytecount);
		(void) pthread_mutex_unlock(&stats.st_mutex);

	}
}
