
# ThTank

## 1. Introduction

### 1.1 What is ThTank?
ThTank is a software tool for the simulation of sound wave propagation, which has been implemented in the C++ programming language in combination with the Qt framework. The method used for the simulation is the Transmission Lines Matrix (TLM) which is based on a discretized and simplified version of the Huygens principle. 

The user can set, export, and load 2-Dimensional closed or open-field scenarios of different sizes, placing sound sources and boundaries. The type of the sources and their properties like frequency, amplitude, or phase are configurable. Several boundary shapes with configurable absorption coefficients are available. Microphones can be placed, and the pressure values in specific positions can be observed in a separate window. 

ThTank presents an alternative to the software used for the laboratory sessions of the technical acoustics course attended by students of media technology and electrical engineering degrees at the Technische Hochschule KÃ¶ln. The new software maintains physical accuracy, is more user-friendly and interactive, and is also open-source.

### 1.2 Running Environment
The software has been compiled for both Windows 32/64 bit and OS X 64 bit. For other operating systems, the project should be compiled again. If that is the case, please contact the administrator.

### 1.3 History and Versions
ThTank was developed and implemented during 2015-2016 at Cologne Technical University of Applied Sciences as a final thesis of the Technical Informatics masterâs degree. The software will be used for the laboratory practices included in the technical acoustics course that the university offers. The version described in this manual is the 1.0. For updates, please see the contact section.

### 1.4 Contact
For any further requests, technical support, or bug reports, feel free to contact:
- Arnau VÃ¡zquez-Giner, vazquez.arnau@gmail.com

For software updates and further information visit:
- [ThTank Project Page](http://www.audiogroup.web.th-koeln.de)

## 2. Installation

### 2.1 Prerequisites
- Qt framework
- C++ compiler

### 2.2 Building from Source
1. Clone the repository:
   \`\`\`sh
   git clone https://github.com/yourusername/ThTank.git
   \`\`\`
2. Navigate to the project directory:
   \`\`\`sh
   cd ThTank
   \`\`\`
3. Open the project with Qt Creator:
   \`\`\`sh
   qtcreator ThTank.pro
   \`\`\`
4. Configure the project and ensure all dependencies are met.
5. Build and run the project from Qt Creator.

### 2.3 Running the Executable
- For Windows, run the `ThTank.exe` file.
- For macOS, run the `ThTank.app` file.

## 3. Usage

### 3.1 Setting Up a Scenario
1. Launch ThTank.
2. Create a new scenario or load an existing one.
3. Place sound sources and boundaries as required.
4. Configure the properties of the sources such as frequency, amplitude, and phase.
5. Add microphones to observe pressure values at specific positions.

### 3.2 Exporting and Loading Scenarios
- Use the export feature to save your current setup.
- Load saved scenarios using the import feature.

## 4. Contributions

Contributions are welcome. Please fork the repository and submit a pull request with your changes. For major changes, please open an issue first to discuss what you would like to change.

## 5. License

ThTank is released under the MIT License. See `LICENSE` for more information.
