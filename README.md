kQueue
- Best voor mac

## Resources
#### 
[Sockets](https://www.geeksforgeeks.org/socket-programming-cc/)

#
int setsockopt(sockfd, SOL_SOCKET, SO_KEEPALIVE,  const void *optval, socklen_t optlen);
SO_KEEPALIVE enables the periodic transmission of messages	on a connected
socket.  Should the connected party fail to respond to these messages,
the connection is considered broken and processes using the socket	are
notified via a SIGPIPE signal when	attempting to send data.


#### Webserver in C(++)
[Building a simple server with C++](https://ncona.com/2019/04/building-a-simple-server-with-cpp/)
#### TCP/IP
[TCP in depth](https://dev.to/tamerlang/tcp-in-depth-3g83)
#### Kernel Queue
[Kernel Queue: The Complete Guide On The Most Essential Technology For High-Performance I/O](https://habr.com/en/articles/600123/)
#### HTTP 1.1 Standard
[HTTP 1.1 RFC2616](https://tools.ietf.org/html/rfc2616)
#### freeBSD Man Pages
[Socket()](https://man.freebsd.org/cgi/man.cgi?query=socket&apropos=0&sektion=2&manpath=FreeBSD+13.2-RELEASE+and+Ports&arch=default&format=html)

[Bind()](https://man.freebsd.org/cgi/man.cgi?query=bind&apropos=0&sektion=2&manpath=FreeBSD+13.2-RELEASE+and+Ports&arch=default&format=html)

[Listen()](https://man.freebsd.org/cgi/man.cgi?query=listen&apropos=0&sektion=2&manpath=FreeBSD+13.2-RELEASE+and+Ports&arch=default&format=html)

[Accept()](https://man.freebsd.org/cgi/man.cgi?query=accept&apropos=0&sektion=2&manpath=FreeBSD+13.2-RELEASE+and+Ports&arch=default&format=html)

[Kqueue()](https://man.freebsd.org/cgi/man.cgi?query=kqueue&apropos=0&sektion=2&manpath=FreeBSD+13.2-RELEASE+and+Ports&arch=default&format=html)