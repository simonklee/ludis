#include <stdio.h>
#include <assert.h>

#include "net.h"
#include "test.h"


int
main(void)
{
    return 0;
}

/*int 
main(void) 
{
	int nread, i;
    struct client c;
	char buf[MAXDATASIZE];
    char *out = "*1\r\n$4\r\nPING\r\n";
    struct net_addr addr;

    addr = net_addr_in("127.0.0.1", 6379);

    if (net_connect(&c, addr) != 0) {
        fprintf(stderr, "net_connect err");
        goto error;
    }

    //if (net_connect_gai(&c) != 0) {
    //    fprintf(stderr, "net_connect err");
    //    goto error;
    //}

    for (i = 0; i < 1; i++) {
        write(c.fd, out, strlen(out));
        if ((nread=recv(c.fd, buf, MAXDATASIZE, 0)) == -1) { 
            fprintf(stderr, "recv() error\n");
            goto error;
        }
        buf[nread] = '\0';
        printf("Server Message: %s", buf);
    }

	close(c.fd);
    return 0;
error:
    close(c.fd);
    return 1;
}*/
