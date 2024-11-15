# Hardware Information Tool

## Overview

This C++ application is designed to retrieve and display detailed hardware information about a Windows system using WMI (Windows Management Instrumentation). The information is presented in a graphical user interface (GUI) window, where users can view various hardware components such as the system, processor, memory, disks, graphics, network adapters, BIOS, motherboard, sound devices, and USB controllers.

## Features

- **System Information**: Retrieves and displays the manufacturer, model, and total physical memory of the system.
- **Processor Information**: Provides details about the processor(s), including name, manufacturer, maximum clock speed, number of cores, and number of logical processors.
- **Memory Information**: Lists the physical memory modules, including manufacturer, capacity, and speed.
- **Disk Information**: Displays information about the disk drives, including model, size, and interface type.
- **Graphics Information**: Shows details about the video controllers, including name, adapter RAM, and driver version.
- **Network Adapter Information**: Lists physical network adapters, including name, manufacturer, and MAC address.
- **BIOS Information**: Retrieves and displays the BIOS manufacturer, version, and release date.
- **Motherboard Information**: Provides details about the motherboard, including manufacturer, product, and version.
- **Sound Device Information**: Lists sound devices, including name and manufacturer.
- **USB Controller Information**: Displays information about USB controllers, including name and manufacturer.

## Requirements

- Windows operating system
- C++ compiler with support for Windows API and WMI
- Required libraries: `wbemuuid.lib`, `user32.lib`, `gdi32.lib`

## How to Build

1. **Clone the Repository**:
   ```bash
   git clone https://github.com/nerd-bear/Windows-SystemInf
   cd hardware-info-tool
   ```

2. **Compile the Code**:
   - Open a command prompt or terminal.
   - Navigate to the directory containing the source code.
   - Use your preferred C++ compiler (e.g., Visual Studio, MinGW) to compile the code.

   Example using `g++`:
   ```bash
   g++ -o hardware_info_tool main.cpp -lole32 -loleaut32 -luuid -lwbemuuid -luser32 -lgdi32
   ```

3. **Run the Executable**:
   - After successful compilation, run the executable:
   ```bash
   ./hardware_info_tool
   ```

## Usage

- Upon running the application, a window will appear displaying the hardware information in a text area.
- The information is organized by category, and each category is clearly labeled.

## Customization

- **Adding More Information**: You can extend the functionality by adding more WMI queries to retrieve additional hardware or system information.
- **Modifying the GUI**: The GUI is built using the Windows API. You can modify the window layout, add more controls, or change the appearance as needed.

## Troubleshooting

- **WMI Connection Issues**: Ensure that WMI services are running on your system. You can check and start the WMI service using the `services.msc` console.
- **Permissions**: Running the application as an administrator may be required to access certain hardware information.

## License

This project is licensed under the MIT License. See the `LICENSE` file for more details.

## Contributing

Contributions are welcome! Please feel free to submit a pull request or open an issue for any bugs or feature requests.

## Acknowledgments

- Thanks to the Microsoft documentation for providing detailed information on WMI and the Windows API.
- Special thanks to the open-source community for various libraries and tools that made this project possible.

---

For any questions or support, please contact nerd.bear on discord or at support@nerd-bear.org.
