#pragma once

#include "sock_connect.h"

/*------------------------------------------------------------------------
 * tcp_connect - connect to a specified TCP service on a specified host
 *------------------------------------------------------------------------
 */
static int tcp_connect(const char *host, const char *service)
/*
 * Arguments:
 *      host    - name of host to which connection is desired
 *      service - service associated with the desired port
 */
{
	return sock_connect(host, service, "tcp");
}
