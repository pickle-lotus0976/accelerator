# Weighted Sum Accelerator IP

![Vitis HLS](https://img.shields.io/badge/Vitis_HLS-2025.1-blue) ![Target Device](https://img.shields.io/badge/Target-Zynq_7000-green) ![Language](https://img.shields.io/badge/Language-C++-orange)

## Overview
This repository contains a high-performance hardware accelerator designed using **Vitis HLS** to perform weighted sum computations. The IP core is optimized for the **Xilinx Zynq-7000** platform (`xc7z007sclg400-1`) and utilizes a stream-based dataflow architecture to maximize throughput and parallel execution.

The accelerator processes 12-bit integer inputs against a set of hardcoded internal weights, producing 16-bit results, and is fully interface-compatible with AXI4 protocols for seamless integration into embedded systems.

## System Architecture
The design employs a **Task-Level Parallelism (TLP)** approach using the `DATAFLOW` pragma. The processing pipeline is divided into three distinct stages connected by HLS streams (FIFOs), allowing overlapping execution:

1.  **Read Engine (`read_data`)**: Fetches data from global memory via AXI4-Master and pushes it to an input stream.
2.  **Compute Engine (`compute`)**: Reads from the stream, performs the weighted sum multiplication, and pushes results to an output stream.
3.  **Write Engine (`write_data`)**: Reads results from the stream and writes them back to global memory via AXI4-Master.

### Key Optimizations
* **Pipelining:** All loops are pipelined with `II=1` (Initiation Interval of 1) for maximum throughput.
* **Array Partitioning:** The internal `weights` array is partitioned (`cyclic, factor=4`) to increase memory bandwidth during computation.
* **Dataflow:** Enables the Read, Compute, and Write functions to execute concurrently.

## Hardware Specifications

| Specification | Value |
| :--- | :--- |
| **Target Device** | Xilinx Zynq-7000 (`xc7z007sclg400-1`) |
| **Clock Frequency** | 100 MHz (10.00 ns period) |
| **Input Data Width** | `ap_int<12>` |
| **Output Data Width** | `ap_int<16>` |
| **Interface Protocol** | AXI4-Master (Data), AXI4-Lite (Control) |

## Performance & Resource Utilization

The following metrics are derived from the Vitis HLS 2025.1 **Place & Route (Implementation)** Report.

### Timing Summary
* **Target Clock:** 10.000 ns
* **Achieved Clock (Post-Implementation):** 5.674 ns
* **Worst Negative Slack (WNS):** 4.326 ns (Met)

### Latency & Throughput
* **Latency:** 310 cycles
* **Interval:** 10 cycles (The IP can accept new data every 10 cycles)

### Resource Usage (Place & Route)
| Resource | Used | Utilization |
| :--- | :--- | :--- |
| **LUT** | 1600 | 11.11% |
| **Flip-Flops (FF)** | 2532 | 8.79% |
| **DSP** | 1 | 1.52% |
| **BRAM** | 1 | 1.00% |

## Interface Details

The core exposes the following interfaces for system integration:

* **`s_axi_control` (AXI4-Lite):**
    * Control signals (`ap_start`, `ap_done`, `ap_ready`, `ap_idle`).
    * Configuration registers for `input_mem` and `output_mem` base addresses.
* **`m_axi_gmem0` (AXI4-Master):** Read channel for input data fetching.
* **`m_axi_gmem1` (AXI4-Master):** Write channel for result storage.
