# OsDev

Repository for learning Αssembly and C languages.
Using various books and tutorials online.
All work in this repo is experimental, and there is a lot of tutorials, online material
and books used to build the OS.

Below is a screenshot of the development stage, as the project has reached up to have a grub file and run in QEMU environment.

![Screenshot from 2025-06-25 20-22-55](https://github.com/user-attachments/assets/09103181-cc18-4b2d-8edf-c9f4f84dfc11)

# Custom Operating System Development (OsDev)

[![Architecture](https://img.shields.io/badge/Architecture-x86__64%20%7C%20i386-blue.svg)](https://wiki.osdev.org/Main_Page)
[![Assembler](https://img.shields.io/badge/Assembler-NASM-orange.svg)](https://nasm.us/)
[![Language](https://img.shields.io/badge/Language-C%20%2C%20Assembly-lightgrey.svg)](https://en.wikipedia.org/wiki/C_(programming_language))
[![Development Branch](https://img.shields.io/badge/Branch-development-red.svg)](https://github.com/IoannisCh/OsDev/tree/development)

A hobbyist operating system built from scratch, designed to explore low-level x86 architecture, bootloader configurations, memory segmentation, and kernel execution transitions. This project documents the process of interfacing directly with bare-metal hardware without relying on an underlying host OS.

> 🛠️ **Note on Current Branch:** This documentation corresponds to the `development` branch, housing active features, experimental boot sequences, and kernel implementations.

---

## 🎯 Project Goals & Features

- **Custom Bootloading:** Initializing low-level system states and establishing transitions from Real Mode (16-bit) to Protected Mode (32-bit) / Long Mode (64-bit).
- **Bare-Metal Output:** Interfacing directly with VGA text mode memory video space (`0xB8000`) for visual display feedback.
- **Hardware Abstraction:** Constructing structural Global Descriptor Tables (GDT) and Interrupt Descriptor Tables (IDT) to manage hardware lines cleanly.
- **Pure Low-Level Stack:** Built cleanly using NASM Assembly for bootstrap phases and freestanding C for primary kernel logic.

---

## 🏗️ Repository Architecture

The project maintains a strict separation between boot sequence scripts, kernel source layers, and build automation assets:

OsDev/
├── src/
│   ├── boot/           # Bootloaders, stage structures, and entry subroutines (NASM)
│   ├── kernel/         # Freestanding C kernel source files and low-level drivers
│   └── include/        # System headers and architectural register blueprints
├── Makefile            # Automation script for toolchain compilation and image linking
├── linker.ld           # Linker script defining physical memory layout entry bounds
└── README.md           # Project documentation

## 🛠️ Toolchain & Prerequisites
sudo apt update
sudo apt install build-essential nasm qemu-system-x86 grub-pc-bin xorriso

## 💻 Building and Emulating

git clone [https://github.com/IoannisCh/OsDev.git](https://github.com/IoannisCh/OsDev.git)
cd OsDev
git checkout development

make clean
make

qemu-system-x86_64 -fda build/os.bin

## 🚀 Development Roadmap
[ ] Implement robust Exception and Interrupt handlers (ISRs / IRQs).

[ ] Add a comprehensive Keyboard Driver to accept hardware scan-code inputs.

[ ] Develop a physical memory manager utilizing basic page-frame allocation models.

[ ] Incorporate custom text printing subroutines to support formatted string scrolling.

## 📚 References & Resources
OSDev Wiki - The definitive knowledge base for custom operating system development.

Intel 64 and IA-32 Architectures Software Developer's Manuals.

## ✉️ Contact
Ioannis Chantolios - GitHub Profile

