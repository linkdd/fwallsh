fwallsh is a command line interface to configure your Linux router/server.

The project is composed of 3 parts :

- The shell, which is runned under a predefined user and can be accessed via
  SSH (with password, or with RSA/DSA/EDSA authentication) ;
- The modules. Each modules manage one application (for example: IPTables
  integration, DNS/DHCP integration, ...) ;
- The daemon, which loads modules and get commands from the shell.

This project is co-authored by :

- linkdd
- kdydm
- Deleterios

