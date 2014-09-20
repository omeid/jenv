# Jenv v0.1

Super simple tool to replace JSON values with Environment Variables. I needed for Docker and a reason to do brush up my ancient and super rusty C skills.


# Usage:

```sh
$ jenv 
Jenv - version v0.1

Jenv is a mini tool to replace JSON nodes.

Usage:  jenv filename [Prefix]

filename:  The JSON file path. The file is not altered.
           This parameter is required.
Prefix:    The env prefix that holds replacement values.
           Must be valid shell variable name. No whitespace.
           Default: 'jenv_'.

Output:    The result is printed to standard output.

```
## Example:

```sh
$ cat example.json 
{
  "node_name": "hub",
  "datacenter": "dc1",
  "domain": "local.microcloud.io",
  "server": true,
  "bootstrap": true,
  "data_dir": "/consul-data",
  "log_level": "INFO",
  "client_addr": "0.0.0.0",
  "ports": {
    "dns": 53,
    "http": 3000
  },
  "can": {
    "handle": { "nested": ["items", 2] }
  }
}

$ #Pay attention to the strings quote.
$ csl_can='{ "handle": ["array", "here"], "param": true }' \
> csl_node_name='"website-1"'                              \
> csl_domain='"example.com"'                               \
> jenv example.json csl_

{
  "node_name": "website-1",
  "datacenter": "dc1",
  "domain": "example.com",
  "server": true,
  "bootstrap": true,
  "data_dir": "/consul-data",
  "log_level": "INFO",
  "client_addr": "0.0.0.0",
  "ports": {
    "dns": 53,
    "http": 3000
  },
  "can": { "handle": ["array", "here"], "param": true }
}

$ 
```

# Limitations

jenv isn't a complete JSON parser, it simply helps replace root nodes using environment variables.
However, it dose not chock on nested `objects` or `arrays` and can hanlde escaping.


# Contribution
Just send me a pull request or if it is design change or adding features you could consider openning an issue first.
