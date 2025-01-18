# web-server


# progress: 

- server shows that it's successfully binding and listening over port, but won't accept any connections.

- commands I've tried 
```
curl -v http://localhost:88
curl -v http://[ip_address]:88
netstat -taln | grep 88 # -t (tcp), -u (udp), -l (includes listening sockets too)
nmap -sT localhost
lsof -i :88 # lists open files, -i (selects any files specifiying an internet address that matches the text ,here ending port 88)
ifconfig | grep inet | grep -Fv 127.0.0.1
nc -vnzt [ip_address] 88 # checks if an ip address supports that port 
netstat -an | grep LISTEN # -n (forces address printed as numbers), -a (forces all sockets to be shown, namely unhides server procs)

```

