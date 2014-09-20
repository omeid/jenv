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
  *)
    echo "Invalid command."
    PrintHelp
  esac

