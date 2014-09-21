# Jenv `/d͡ʒˈɪnv/`
Version v0.1 

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

# Building

## Dependencies
* libc
Yup, that is it.

## How?
just run 
```sh
$ ./builder build && ./builder install
```
> You may need to run `./builder install` with `sudo`.

# Testing

There is a flimsy test for now, just run `./builder test`.


# Limitations

jenv isn't a complete JSON parser, it simply helps replace root nodes using environment variables.
However, it dose not chock on nested `objects` or `arrays` and can hanlde escaping.

# Caveat

You _cannot_ read from a file and write to it in the same pipeline. So this is not possible:

### Problem: 

```sh
$ #Bad example. This won't work. 
$ jsenv config/app.json my_prefix > config/app.json
$ # Nor will this or any other combination. Except sponge(1)*
$ jsenv config/app.json | tee config/app.json
```
Read more on [File Redirection](http://mywiki.wooledge.org/BashGuide/InputAndOutput#File_Redirection) and this [Pitiful specifically](http://mywiki.wooledge.org/BashPitfalls#cat_file_.7C_sed_s.2Ffoo.2Fbar.2F_.3E_file).

> [*sponge(1)](http://man.cx/sponge) is part of [moreutials](https://joeyh.name/code/moreutils/).

### Solution:
  Consider using _template files_ this also means that unprocessed json files are never servered to your services.
  And you can easily chose want should be processed by prefixing it.

  Example:
```sh
$ ls config
app.json.jenv consul.json.jenv

$ for json in config/*.jenv; do
> jenv $js prefix_ > ${js%%\.jenv};
> done;
$ ls *

$ ls config
app.json app.json.jenv console.json consul.json.jenv

$ rm config/*.jenv #optionally delete _templates_.
```


# Contribution
Just send me a pull request or if it is design change or adding features you could consider openning an issue first.
