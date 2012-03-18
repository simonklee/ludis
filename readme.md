structure

    ludis_context

        holds a redis_handle, an address

        Implementes high level methods to read, write, connect 
        and call redis commands

        TODO: move ludis.c here 

    net_addr

        holds information about a socket address
        create by calling redis_addr_in, redis_address_in6, etc.

        Implements methods to define an anddress

    ludis_handle

        holds a file descriptor, wb, rb.

        Implements mid level methods for connect, 
        i/o on r/w buffers.

    fd

        Implements low level read, write, connect_addr, connect_gai
        methods. These methods are all quiet simple and only handle
        the logic around i/o syscalls

    cw

        Implements http methods to retrieve and store the contents
        of an url

        fetch_page(url)

            fetch webpage by url. 

    str
        
        Implements a str and a buffer.
