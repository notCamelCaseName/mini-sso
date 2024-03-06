# Mini SSO
This project was used as an example for a presentation on SSO for [MiNET](https://minet.net)

## Requirements
- `gcc`

## Installation

```bash
git clone https://github.com/notCamelCaseName/mini-sso 
cd mini-sso
make run
```

Should launch a server that listens on port 45823.

## Usage
Once the server is started, clients can open a connection using a socket or with the `nc` command to send instructions.

> Only one instruction can be sent per socket

### Registering
First, one can register to the SSO by sending `register [name] [password]`

### Authentication
To authenticate to the server, either
- use credentials by sending `creds [name] [password]`.
If successful, the server should answer with
 ```
OK!
[token]
[cookie]
```
- cookie by sending `cookie [cookie]`.
If successful, the server should answer with
 ```
OK!
[token]
```


## Warning
This program is not made to be used as a real SSO, it was made in a few hours to demonstrate the workings of an SSO and is by no means safe nor usable in a real environment.
