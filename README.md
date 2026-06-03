# KS_LCD_2.0

An advanced, decoupled, and modular LCD display and graphic hardware abstraction framework designed for resource-constrained embedded systems (ARM Cortex-M microcontrollers such as STM32, AT32, etc.).

一款专为资源受限的嵌入式系统（ARM Cortex-M 微控制器，如 STM32、AT32 等）设计的高性能、解耦、模块化 LCD 显示与图形硬件抽象框架。

---

## 🚀 Key Features / 核心特性

- **Multi-Layer Abstraction (解耦架构):** Complete separation between high-level graphic applications, low-level LCD controller drivers, and vendor-specific MCU hardware peripherals (SPI/I2C/Parallel).
- **RTOS Ready (实时操作系统支持):** Built-in task-scheduling adapters designed for smooth, thread-safe, and non-blocking asynchronous display rendering under RTOS environments.
- **Zero-Overhead Debugging (高效调试):** Natively integrates `SEGGER_RTT` for high-speed, zero-latency runtime telemetry and log streaming without impacting time-critical display SPI/DMA transfers.
- **Modern Toolchain (现代编译链):** Fully compatible with modern embedded development environments using CMake, GCC toolchains, and independent linkscripts for flexible asset management.
- **Code Standard (代码规范):** Integrated with strict `.clang-format` styling to ensure clean, consistent, and maintainable enterprise-level code quality.

---

## 📂 Repository Structure / 目录结构

```text
├── RTOS/            # RTOS-specific abstraction and safe rendering task logic
├── application/     # Graphic logic and display asset configuration
├── drivers/         # Independent LCD controller chip drivers (e.g., ST7789, ILI9341)
├── platform/        # Vendor SDK abstraction layer (STM32/AT32 peripheral bindings)
├── libraries/       # Third-party utilities, including SEGGER_RTT for debugging
├── linkscripts/     # Custom linker scripts for optimized RAM/Flash memory mapping
├── bin/             # Pre-compiled binaries or initial generation folders
└── doc/             # Technical specifications and hardware documentation
🛠️ Tech Stack & Prerequisites / 开发环境与工具链
Language: Standard C (C99/C11)

Target Architecture: ARM Cortex-M4 / Cortex-M3 (STM32F1xx, AT32F4xx, etc.)

Build System: CMake

Compiler: GNU Arm Embedded Toolchain (GCC)

Formatting: Clang-format

🤖 Acknowledgements & AI Tools /辅助
This project highly leverages cutting-edge LLMs (such as OpenAI's GPT models) to accelerate low-level firmware engineering. AI tools are utilized in this repository for:

Accelerating hundreds-of-pages LCD datasheet parsing into structured register initialization arrays.

Auditing RTOS synchronization primitives (mutexes, semaphores) to ensure flawless thread safety.

Optimizing high-performance graphic drawing math and cross-compiler toolchain configurations.
