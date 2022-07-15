#pragma once

#include "sock_passive.h"

/*------------------------------------------------------------------------
 * tcp_passive - create a passive socket for use in a TCP server
 *------------------------------------------------------------------------
 */
static int tcp_passive(const char *service, int qlen)
/*
 * Arguments:
 *      service - service associated with the desired port
 *      qlen    - maximum server request queue length
 */
{
	return sock_passive(service, "tcp", qlen);
}
