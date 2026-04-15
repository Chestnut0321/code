#PPSS-Dual Objective Optimization
#Project Overview
#This project implements a dual-objective optimization model for project-supplier selection based on IBM CPLEX Optimization Studio. Its core objectives are to maximize project profit and minimize supplier cooperation risk simultaneously under given constraints, supporting decision optimization and analysis in multi-period, multi-resource, and multi-supplier scenarios.
#By quantifying indicators such as project revenue, procurement cost, and supplier risk, and incorporating constraints including resource requirements, project execution, and supplier capacity, the model generates Pareto-optimal solutions to provide data support for enterprise resource allocation and partnership decision-making.

#Core Features
#Dual-objective Optimization: Synchronously optimizes "profit maximization" and "risk minimization", and supports risk-optimal solutions under given profit threshold constraints.
#Multi-scenario Adaptability: Supports customized numbers of projects, suppliers, periods, and resource types, adapting to decision-making scenarios of different scales.
#Batch Testing Capability: Built-in cyclic testing logic for multiple instances, with automatic output of core optimization results for each test group.
Standardized Result Output: Result files are automatically archived to a specified directory, with clear recording of key indicators such as profit and risk for each instance.
#Memory Safety Control: Complete logic for dynamic memory allocation and deallocation to avoid memory leaks during large-scale data processing.
Environment Dependencies

#Basic Environment
#Operating System: Windows 10+/Linux (Ubuntu 18.04+/CentOS 7+)/macOS
#Programming Language: C++11 or later
#Compiler: GCC/Clang (Linux/macOS), MinGW (Windows)
#Core Dependency
#IBM CPLEX Optimization Studio 12.8 or later (environment variables must be configured in advance).
#Note: A license for CPLEX must be obtained separately (academic users can acquire free licenses through the IBM Academic Initiative).

#Project Structure
#plaintext
#ppss-dual-objective/
├── README.md           # Project documentation
├── src/
│   └── dual-objective model.cpp        # Core optimization logic (dual-objective solving, constraint construction, data reading)
└── results/            # Result output directory (automatically created by the program to store optimization results)

#Quick Start
#1. Environment Configuration
#After installing CPLEX, configure the environment variables:
#Linux/macOS: Run export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/path/to/cplex/lib in the terminal (replace with the actual CPLEX installation path).
#Windows: Add the bin and lib directories of CPLEX to the system environment variables.
#2. Data Preparation
#Create a data folder in the project root directory.
#Place data files in the specified format with the naming rule: {number of projects}-{number of suppliers}-({test number})data.txt
#(Example: 15-10-(1)data.txt represents the 1st test dataset with 15 projects and 10 suppliers).
#The data file must contain the following core fields in fixed order:
#project resource requirements, supplier resource capacity, procurement unit price, supplier risk value, project period, project revenue, periodic government subsidies, periodic budget, project-supplier distance, and inter-project budget saving coefficient.
#3. Compilation and Execution
#Navigate to the project root directory.Compile the source code to generate the executable file ppss.

#Result Description
#Output Path
#Optimization results are automatically saved to results/resuFinal.txt. The results directory is created automatically by the program.

#Result Content
#Each line in the file records the core results of one test group in the following format:
#plaintext
#Instance: 15 Projects-10 Suppliers
#Maximum Profit: 125000.0 | Minimum Risk: 45.2

#Frequently Asked Questions
#1.Compilation error: cannot find -lcplexCause: Incorrect CPLEX environment variable configuration or missing CPLEX library path during compilation.Solution:
g++ src/main.cpp -o ppss -std=c++11 -I/path/to/cplex/include -L/path/to/cplex/lib -lcplex
#2.Runtime error: data file not foundCheck whether the file naming in the data directory complies with the rules, and whether the number of projects and suppliers matches the loop parameters in the code (default loops for 15/20 projects and 10 suppliers).
#3. CPLEX returns no solutionPossible causes: conflicting constraints or insufficient time limit.Solution: Adjust the constraint logic or increase the TIME_LIMIT parameter.
#License Notice
#This project is for academic research and technical communication only. Commercial use is prohibited.Use of the CPLEX software must comply with the relevant license agreements of IBM.

#Contact Information
#For questions or optimization suggestions, please contact via:Email: 2023223028@chd.edu.cn

