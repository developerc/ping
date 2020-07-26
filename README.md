# ping
Application for sending request and receiving echo response of ICMP packet.\
This application is my experiment with ICMP protocol on openVMS operating system. For running the application you must firstly to compile source code, link object file. 
Then you must to get administrator privileges. Application creates Raw Socket, which potentially can threaten security.
Command for it:\
set proc/priv=all\
I got successful result with loopback interface 127.0.0.1 \
The reason is that the IP address of the operating system kernel is different from the IP address of the virtual machine Haron. 
And they are on different subnets. I cant to ping other computers.\
And I hope on really openVMS it will be all OK.
