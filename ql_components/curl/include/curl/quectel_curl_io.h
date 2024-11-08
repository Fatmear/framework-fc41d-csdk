/*

quectel curl io api function

*/

#ifndef QUECTEL_CURL_IO_H
#define QUECTEL_CURL_IO_H

#include "netdb.h"


int quectel_curl_getaddrinfo
(
uint32_t simcid,
const char *nodename,
const char *servname,
const struct addrinfo *hints,
struct addrinfo **res
);

void quectel_nCid_local_addr(uint32_t simcid, char *addr, int af, const char *func_name);
void quectel_curl_global_init(void);
void quectel_curl_global_cleanup(void);




#endif /* QUECTEL_CURL_IO_H */
