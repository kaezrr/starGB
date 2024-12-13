# StarGB

A lightweight and efficient Nintendo Game Boy emulator written in C++ using SDL2 for graphics, spdlog for high-performance logging, and fmt for clean and flexible formatting. This project faithfully reproduces the classic Game Boy gaming experience, supporting popular game titles with various Memory Bank Controllers.

---

## 🎮 Features

- **Accurate Emulation**: Faithfully replicates Game Boy hardware behavior for a classic gaming experience.
- **Supported MBCs**: Compatible with games using:
  - MBC1
  - MBC3
  - MBC5
  - No MBC (simple ROMs)
- **Keyboard Support**: Play directly using your keyboard.
- **High-Performance Logging**: Debug seamlessly with `spdlog`.

---

## 📸 Screenshots and Gameplay

### Screenshots  
<img src="https://github.com/user-attachments/assets/cb470c34-6ca3-4911-bbb8-06814a40cf6f" width=300px height=auto>
<img src="https://github.com/user-attachments/assets/a44ec2e0-4578-4f3d-bc5c-ec1efff7824c" width=300px height=auto>
<img src="https://github.com/user-attachments/assets/3947fe24-500f-486e-8764-501d9a2ba052" width=300px height=auto>

### Gameplay GIFs  
<img src="https://github.com/user-attachments/assets/8bf91b3e-e322-4c31-a78e-f1dd295e23c3" width=300px height=auto>

<img src="https://github.com/user-attachments/assets/9d0cd8b2-9926-4d42-98db-44d8e6ccaa27" width=300px height=auto>

<img src="https://github.com/user-attachments/assets/cc4a8e25-04a6-4662-99f3-25e01ae8c385" width=300px height=auto>

---

## 🚀 Getting Started

### Prerequisites

- **C++17 or later**
- **SDL2**
- **spdlog**
- **fmt**

Ensure these dependencies are installed on your system before building the emulator.

### Installation

1. Clone the repository:
   ```bash
   git clone https://github.com/kaezrr/starGB.git
   cd starGB
   ```

2. Build the project using CMake:
   ```bash
   mkdir build
   cd build
   cmake .. -DCMAKE_BUILD_TYPE=Release
   make
   ```
3. Put the roms that you want to run in the `roms/` folder. (NOTE: don't delete the `dmg_boot.bin` file, it is necessary to run the emulator)

4. Run the emulator:
   ```bash
   ./starGB [path/to/rom]
   ```
---

## 🛠️ Limitations and Future Plans

### Current Limitations:
- **No Settings Customization**: All configurations are hardcoded for now.
- **No Audio Support**: Game audio is not yet implemented.

### Future Plans:
- Add audio emulation.
- Introduce customizable key mapping and emulator settings.
- Expand compatibility to Game Boy Color.

---

## 🤝 Contributing

Contributions are welcome! If you'd like to improve the emulator, feel free to fork the repository, make your changes, and submit a pull request.

---

## 🙌 Acknowledgments

- Thanks to the open-source libraries `SDL2`, `spdlog`, and `fmt` for enabling this project.
- Thanks to the wonder folks over at r/emuDev and their discord
- Thanks to [LIJI32](https://github.com/LIJI32/SameBoy) for the copyright free boot rom.
- Thanks to [pandocs](https://gbdev.io/pandocs/) and [hacktix](https://hacktix.github.io/GBEDG/) for their wonderful documentations.
- Thanks to [gekkio](https://github.com/Gekkio/mooneye-test-suite), [blargg](https://github.com/retrio/gb-test-roms) and [adtennant](https://github.com/adtennant/GameboyTestSuites) for their absolutely amazing tests.
- Inspired by the legacy of the Nintendo Game Boy and the retro gaming community.
