# Developing for the fx-CP400
You'll need your SH4 cross compiler to develop applications for the fx-CP400. If you patched your firmware, you'll already have done this. If not, check the instructions in [this](patching.md) document to build and install one.

## 1. Build the SDK
This only needs to happen once to generate the required object files which will be linked with your application when it's compiled. It isn't necessary to rebuild the SDK itself whenever you change your application code.

`cd` into the `sdk/` directory, and run the `make` command. Ensure the file `sdk.o` is generated - this is the boilerplate code required for your applications to work when compiled.

If you'd like a local copy of the SDK documentation, run the `make docs` command. Open `doc/index.html` to view them.

## 2. Start your project
Copy the contents of the `project_template/` directory to an empty folder.

Edit `main.c` to your hearts content, and add other `.c` and `.s` files as you please. The `Makefile` will automatically detect new source files and compile them.

## 3. Build your application
Point the Makefile to the SDK using the `SDK_DIR` environment variable. For example, if you cloned this repository to `~/hollyhock/`, set `SDK_DIR` to `~/hollyhock/sdk/`. You can do this either by exporting the variable before running any `make` commands, or setting the variable in the same command as you run make. It's recommended you make this an absolute path and not a relative one - otherwise you may run into issues if you attempt to call `make` while not in the same directory as the one you defined `SDK_DIR` relative to.

```sh
# Either do this once, at the start of every session:
export SDK_DIR=~/hollyhock/sdk/
# Or, run make like this:
SDK_DIR=~/hollyhock/sdk/ make
```

The file `run.bin` will be generated, which you can then copy onto the calculator. Have fun!
