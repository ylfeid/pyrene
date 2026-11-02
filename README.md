# Pyrene

From the ancient greek πυρήν - core, kernel. 

Written from scratch in C for aarch64 and riscv64.

## Status

Early bring-up. Boots on QEMU `virt` (aarch64), reads its hardware layout from the devicetree.

## Milestones

- [x] Boot, early UART console
- [x] printk
- [x] Devicetree parsing (RAM, CPUs, UART)
- [ ] Exceptions
  - [x] Vector table and register save/restore
  - [ ] Syndrome decoding and register dump
  - [ ] Backtrace
- [ ] Physical memory allocator
- [ ] Virtual memory (MMU)
- [ ] riscv64 port
- [ ] Interrupts and timer
- [ ] Threads and scheduler
- [ ] SMP
- [ ] Userspace
- [ ] virtio and networking
- [ ] Raspberry Pi 4