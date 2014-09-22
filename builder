#/bin/sh

PrintHelp() {

  echo "
  
    Usage:  build.sh commmand

    commands:

      build:    Build the project. see bin/ for results.
      install:  Install the project binary in /bin/. 
                May need root access or sudo.
      test      Some basic test.
      all       Runs build and test.
         ";
}

[ "$1" ] || {
  echo "Please specify a command.";
  PrintHelp
  exit 1
}

build() {
  echo "Building..."
  gcc -std=c99 src/jenv.c -fwhole-program -o bin/jenv && strip -s bin/jenv
  echo "Done."
}

test() {

  test="./test/run"

  if [ -f "$test" ]; then
    . $test;
  else 
    echo "No test found."
  fi;

}

case "$1" in
  all)
      build
      test
  ;;
  build)
      test
  ;;
  install)
        cp src/jenv /bin/
  ;;
  test)
    test
  ;;
  *)
    echo "Invalid command."
    PrintHelp
  esac
