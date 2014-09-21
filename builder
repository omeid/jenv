#/bin/sh

PrintHelp() {

  echo "
  
    Usage:  build.sh commmand

    command:
      build:   Build the project. see bin/ for results.
      Install: Install the project binary in /bin/. 
               May need root access or sudo.
         ";
}

[ "$1" ] || {
  echo "Please specify a command.";
  PrintHelp
  exit 1
}

case "$1" in
  build)
        echo "Building..."
        gcc -std=c99 src/jenv.c -fwhole-program -o bin/jenv && strip -s bin/jenv
        echo "Done."
  ;;
  install)
        cp src/jenv /bin/
  ;;
  test)
        csl_can='{ "handle": ["array", "here"], "param": true }'    \
        csl_node_name='"website-1"'                                 \
        csl_domain='"example.com"'                                  \
        csl_datacenter='"d1"'                                       \
        csl_server=false                                            \
        bin/jenv test/input.json csl_ |                             \
        diff --ignore-all-space -q                                  \
        - test/expect.json                                          \
        && echo "Test passed." || echo "failed."
  ;;
  *)
    echo "Invalid command."
    PrintHelp
  esac
