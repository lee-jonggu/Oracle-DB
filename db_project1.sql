select ltrim(client_name) from client;

update client set client_name = ltrim(client_name);