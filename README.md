# c-project Documentation

## Project Overview
This project is a C application that utilizes utility functions and a cryptographic library based on the secp256k1 curve. The application is structured to separate the main logic from utility functions for better organization and maintainability.

## Project Structure
```
c-project
├── src
│   ├── main.c        # Entry point of the application
│   └── utils.c       # Implementation of utility functions
├── include
│   └── utils.h       # Header file for utility functions
├── lib
│   └── libsecp256k1.a # Precompiled static library for cryptographic functions
├── Makefile          # Build instructions
└── README.md         # Project documentation
```

## Building the Project
To build the project, navigate to the project directory and run the following command:

```
make
```

This will compile the source files and link them with the `libsecp256k1.a` library.

## Running the Application
After building the project, you can run the application using the following command:

```
./main
```

## Dependencies
This project depends on the `libsecp256k1.a` library, which must be present in the `lib` directory.

## License
This project is licensed under the MIT License. See the LICENSE file for more details.