# BDD testing 6502

This subdirectory contains the code for BDD testing ASM code.

It is a kotlin project using gradle to build. Everything can be run from the command line, however if you
wish to extend tests and add new Steps (the main part of features) you will be better served with an IDE.
I'm using IntelliJ, and there are run configurations setup for using in IntelliJ.

## Building

Because this relies on the [BDD6502 framework](https://github.com/martinpiper/BDD6502), there
are some libraries that first have to be built into your local maven repository.
Hopefully this will eventually change when the author of BDD6502 releases his code to a
public repository.

Install the following:

- java 8 or 11 - [sdkman](https://sdkman.io/) or other mechanism of your choice
- maven - [sdkman](https://sdkman.io/), or [Apache Maven Project](https://maven.apache.org/install.html)

Note: The required BDD6502 libraries currently do not support higher than Java 14.

### Install BDD6502 libraries

Clone the following 3 projects into a directory on your machine, build them, then come back to this project.

```shell
mkdir bdd
cd bdd
git clone https://github.com/martinpiper/ACEServer.git
git clone https://github.com/martinpiper/BDD6502.git
git clone https://github.com/martinpiper/CukesPlus.git

cd ACEServer
mvn install

cd ../CukesPlus
mvn install

cd ../BDD6502
mvn install -DskipTests
```

You will now have the required libraries installed locally to be able to run this project.

## Running tests

Open a terminal, and cd into the bdd-testing directory, and run following:

```shell
./gradlew runFeatures
```

Windows users should be able to run `.\gradlew.bat` instead of `./gradlew` in the above.

## Interactive Development and testing

This is the hot stuff.

Running the following will start up a server for interactive BDD development.

```shell
./gradlew run
```

The service will start a listener at port 8001. Navigate to the following page:

- http://localhost:8001/ace-builds-master/demo/autocompletion.html

This will present you with a CukesPlus test page, with the features listed down the left hand side.

To run one, click on it, and then click the "Run file" button. You can view results on the page, with
additional logs in the terminal where you started the server.

Making changes to the feature can be done in the UI. It has introspection on the steps too, for instance
type "And I run + `ctrl enter`" and it will pop up all the available steps that will complete the step.

Press ctrl-s to save it back to the server.

If you need to create new Steps, do so in the [TestGlue](src/main/kotlin/TestGlue) package.
This is the code that cucumber (the BDD testing framework) will use to bridge the gap between features
and its simple english statements, and the things that actually run when the test is invoked.

After making appropriate new Steps, restart the service, and they will be available in the webclient UI to
use.

## How it works

As mentioned, this project uses, and builds on top of the [BDD6502](https://github.com/martinpiper/BDD6502) framework.

### Features

Tests (or features) should go into [features](features) folder. Here is where the BDD statements are created
that will test the code.

### Macros

You can additionally create "macros" which are combinations of other steps, and place these in the [macros](macros) folder.
These massively simplify common setup in features.
