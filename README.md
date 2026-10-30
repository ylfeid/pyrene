Small, portable OS kernel written from scratch in C for aarch64 and riscv64.

## Status

Early bring-up. Boots on QEMU `virt` (aarch64), reads its hardware layout from the devicetree.

## Build and run

    brew install llvm lld qemu dtc
    make run

Exit QEMU with Ctrl-A, then X.

## Milestones

- [x] Boot, early UART console
- [x] printk
- [x] Devicetree parsing (RAM, CPUs, UART)
- [ ] Exception vectors
- [ ] Physical memory allocator
- [ ] Virtual memory (MMU)
- [ ] riscv64 port
- [ ] Interrupts and timer
- [ ] Threads and scheduler
- [ ] SMP
- [ ] Userspace
- [ ] virtio and networking
- [ ] Raspberry Pi 4