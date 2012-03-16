#ifndef LUDIS_COMMON_H
#define LUDIS_COMMON_H

#define LUDIS_OK    0
#define LUDIS_ERR  -1
#define LUDIS_ESYS -2
#define LUDIS_EEOF -3
#define LUDIS_EGAI -4

#define IOBUFLEN 1024 * 16

/* Structure
 *
 * redis_context 
 *    holds a redis_handle, an address
 *
 *    implementes high level methods to read, write, connect 
 *    and call redis commands
 *
 * redis_address (net_addr) 
 *
 *    holds information about a socket address
 *    create by calling redis_addr_in, redis_address_in6, etc.
 *
 *    implements methods to define an anddress
 *
 * redis_handle 
 *
 *    holds a file descriptor, parser, wbuf, rbuf.
 *
 *    implements mid level connect, write_to/from_buffer, 
 *    read_to/from_buffer and status commands on fd.
 *
 * ludis_handle
 *
 *    holds a file descriptor, wb, rb.
 *
 *    implements mid level net funcs
 *
 * fd
 *
 *    implements low level read, write, connect_addr, connect_gai
 *    methods. These methods are all quiet simple and only handle
 *    the logic around i/o syscalls
 *
 *
 */
#endif
